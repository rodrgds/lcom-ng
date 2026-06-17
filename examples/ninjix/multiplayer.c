/**
 * @file multiplayer.c
 * @brief Multiplayer communication implementation over the serial port.
 * @ingroup group_multiplayer
 *
 * Implements the multiplayer protocol, packet encoding/decoding,
 * connection handshake, role synchronisation, event dispatching,
 * pause/resume coordination, and gameplay message exchange between peers.
 *
 * This file contains the internal implementation of the multiplayer
 * subsystem. The public API is declared in multiplayer.h.
 */
#include "multiplayer.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "serial_port.h"

/**
 * @name Protocol constants
 */
/** @{ */
#define MP_MAGIC_0 0xA7            /**< First magic byte in every packet. */
#define MP_MAGIC_1 0x4D            /**< Second magic byte in every packet. */
#define MP_PROTOCOL_VERSION 1      /**< Current protocol version. */
#define MP_HELLO_INTERVAL_TICKS 30 /**< Ticks between HELLO retransmissions. */
#define MP_ROLE_INTERVAL_TICKS 30  /**< Ticks between ROLE retransmissions. */
/** @} */

/**
 * @brief Internal message type identifiers.
 *
 * These are the type values encoded in the TYPE field of each packet.
 * They form two ranges: connection management (1-4, 32-35) and
 * gameplay actions (16-20).
 */
typedef enum {
  MP_MSG_HELLO = 1,          /**< Connection handshake initiator. */
  MP_MSG_HELLO_ACK = 2,      /**< Connection handshake acknowledgement. */
  MP_MSG_ROLE = 3,            /**< Role announcement. */
  MP_MSG_START = 4,           /**< Game-start signal. */
  MP_MSG_TOWER = 16,          /**< Tower placement action. */
  MP_MSG_ENEMY = 17,          /**< Enemy spawn action. */
  MP_MSG_TOWER_UPGRADE = 18,  /**< Tower upgrade action. */
  MP_MSG_TOWER_SELL = 19,     /**< Tower sell action. */
  MP_MSG_TOWER_TARGET = 20,   /**< Tower targeting mode change. */
  MP_MSG_PAUSE = 32,          /**< Pause request. */
  MP_MSG_RESUME_READY = 33,   /**< Resume-ready signal. */
  MP_MSG_LEAVE = 34,          /**< Disconnect notification. */
  MP_MSG_RESULT_ACTION = 35,  /**< End-of-game action proposal. */
} mp_message_type_t;

/**
 * @brief Receiver state machine states.
 *
 * The receiver assembles packets byte-by-byte from the serial stream.
 * This enum tracks which field the receiver is currently expecting.
 */
typedef enum {
  MP_RX_MAGIC_0,  /**< Waiting for the first magic byte (0xA7). */
  MP_RX_MAGIC_1,  /**< Waiting for the second magic byte (0x4D). */
  MP_RX_TYPE,     /**< Reading the message type. */
  MP_RX_SEQ,      /**< Reading the sequence number. */
  MP_RX_LEN,      /**< Reading the payload length. */
  MP_RX_PAYLOAD,  /**< Accumulating payload bytes. */
  MP_RX_CHECKSUM, /**< Reading the checksum byte. */
} mp_rx_state_t;

/**
 * @name Internal helpers
 */
/** @{ */

/**
 * @brief Compute the 8-bit checksum for a packet.
 *
 * Sum of TYPE + SEQ + LEN + all payload bytes, modulo 256.
 *
 * @param type    Message type.
 * @param seq     Sequence number.
 * @param len     Payload length.
 * @param payload Payload data (may be NULL if len is 0).
 * @return 8-bit checksum.
 */
static uint8_t mp_checksum(uint8_t type, uint8_t seq, uint8_t len,
                           const uint8_t *payload)
{
  uint8_t sum = (uint8_t)(type + seq + len);

  for (uint8_t i = 0; i < len; i++)
    sum = (uint8_t)(sum + payload[i]);

  return sum;
}

/**
 * @brief Reset the receiver state machine to MAGIC_0.
 * @param mp Multiplayer instance.
 */
static void mp_rx_reset(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  memset(&mp->rx, 0, sizeof(mp->rx));
  mp->rx.state = MP_RX_MAGIC_0;
}

/**
 * @brief Push an event onto the event queue.
 * @param mp    Multiplayer instance.
 * @param event Event to push (copied by value).
 * @return true on success, false if the queue is full.
 */
static bool mp_push_event(multiplayer_t *mp, const multiplayer_event_t *event)
{
  if (mp == NULL || event == NULL)
    return false;

  if (ringbuf_full(&mp->event_queue))
  {
    printf("mp_push_event: event queue full, dropping event\n");
    return false;
  }

  return ringbuf_push(&mp->event_queue, event, sizeof(*event)) == 0;
}

/**
 * @brief Push a simple (no-payload) event onto the queue.
 * @param mp   Multiplayer instance.
 * @param type Event type.
 */
static void mp_push_simple_event(multiplayer_t *mp, multiplayer_event_type_t type)
{
  multiplayer_event_t event;
  memset(&event, 0, sizeof(event));
  event.type = type;
  (void)mp_push_event(mp, &event);
}

/**
 * @brief Push a result-action event onto the queue.
 * @param mp     Multiplayer instance.
 * @param action Result action to push.
 */
static void mp_push_result_event(multiplayer_t *mp, multiplayer_result_action_t action)
{
  multiplayer_event_t event;
  memset(&event, 0, sizeof(event));
  event.type = MULTIPLAYER_EVENT_RESULT_ACTION;
  event.data.result.action = action;
  (void)mp_push_event(mp, &event);
}

/**
 * @brief Encode a 16-bit value in little-endian byte order.
 * @param dst   Destination buffer (2 bytes).
 * @param value Value to encode.
 */
static void mp_write_u16(uint8_t *dst, uint16_t value)
{
  dst[0] = (uint8_t)(value & 0xFFU);
  dst[1] = (uint8_t)((value >> 8) & 0xFFU);
}

/**
 * @brief Decode a 16-bit value from little-endian byte order.
 * @param src Source buffer (2 bytes).
 * @return Decoded 16-bit value.
 */
static uint16_t mp_read_u16(const uint8_t *src)
{
  return (uint16_t)src[0] | (uint16_t)((uint16_t)src[1] << 8);
}
/** @} */

/**
 * @name Packet transmission helpers
 */
/** @{ */

/**
 * @brief Build and send a protocol packet over the serial port.
 *
 * Writes the magic bytes, type, sequence number, payload length,
 * payload data, and checksum via ser_send_char_int().
 *
 * @param mp      Multiplayer instance.
 * @param type    Message type.
 * @param payload Payload data (may be NULL if len is 0).
 * @param len     Payload length (must be ≤ 8).
 * @return 0 on success, non-zero on serial I/O error.
 */
static int mp_send_packet(multiplayer_t *mp, uint8_t type,
                          const uint8_t *payload, uint8_t len)
{
  if (mp == NULL || len > sizeof(mp->rx.payload))
    return 1;

  uint8_t seq = mp->tx_seq++;
  uint8_t checksum = mp_checksum(type, seq, len, payload);

  if (ser_send_char_int(COM1_BASE, MP_MAGIC_0) != 0 ||
      ser_send_char_int(COM1_BASE, MP_MAGIC_1) != 0 ||
      ser_send_char_int(COM1_BASE, type) != 0 ||
      ser_send_char_int(COM1_BASE, seq) != 0 ||
      ser_send_char_int(COM1_BASE, len) != 0)
    return 1;

  for (uint8_t i = 0; i < len; i++)
  {
    if (ser_send_char_int(COM1_BASE, payload[i]) != 0)
      return 1;
  }

  return ser_send_char_int(COM1_BASE, checksum) == 0 ? 0 : 1;
}

/**
 * @brief Send a HELLO packet with the current protocol version.
 * @param mp Multiplayer instance.
 */
static void mp_send_hello(multiplayer_t *mp)
{
  uint8_t payload[1] = { MP_PROTOCOL_VERSION };
  (void)mp_send_packet(mp, MP_MSG_HELLO, payload, sizeof(payload));
}

/**
 * @brief Send a HELLO_ACK packet with the current protocol version.
 * @param mp Multiplayer instance.
 */
static void mp_send_hello_ack(multiplayer_t *mp)
{
  uint8_t payload[1] = { MP_PROTOCOL_VERSION };
  (void)mp_send_packet(mp, MP_MSG_HELLO_ACK, payload, sizeof(payload));
}

/**
 * @brief Send a ROLE packet announcing the local player's role.
 *
 * No-op if the local role is NONE.
 *
 * @param mp Multiplayer instance.
 */
static void mp_send_role(multiplayer_t *mp)
{
  uint8_t payload[1];

  if (mp == NULL || mp->local_role == MULTIPLAYER_ROLE_NONE)
    return;

  payload[0] = (uint8_t)mp->local_role;
  (void)mp_send_packet(mp, MP_MSG_ROLE, payload, sizeof(payload));
}

/**
 * @brief Send a START packet to begin the game.
 * @param mp Multiplayer instance.
 */
static void mp_send_start(multiplayer_t *mp)
{
  uint8_t payload[1] = { 1 };
  (void)mp_send_packet(mp, MP_MSG_START, payload, sizeof(payload));
}

/**
 * @brief Send a RESULT_ACTION packet.
 *
 * No-op if the action is NONE.
 *
 * @param mp     Multiplayer instance.
 * @param action Action to propose.
 */
static void mp_send_result_action(multiplayer_t *mp, multiplayer_result_action_t action)
{
  uint8_t payload[1];

  if (mp == NULL || action == MULTIPLAYER_RESULT_ACTION_NONE)
    return;

  payload[0] = (uint8_t)action;
  (void)mp_send_packet(mp, MP_MSG_RESULT_ACTION, payload, sizeof(payload));
}
/** @} */

/**
 * @brief Dispatch a fully assembled and checksumed packet.
 *
 * Decodes the packet type and payload and pushes the appropriate
 * multiplayer_event_t onto the event queue. Handles bounds checks
 * on all payload fields to prevent malformed-packet crashes.
 *
 * @param mp      Multiplayer instance.
 * @param type    Message type.
 * @param payload Payload data.
 * @param len     Payload length.
 */
static void mp_handle_packet(multiplayer_t *mp, uint8_t type,
                             const uint8_t *payload, uint8_t len)
{
  if (mp == NULL)
    return;

  switch ((mp_message_type_t)type)
  {
    case MP_MSG_HELLO:
    {
      bool was_connected = mp->peer_connected;
      mp->peer_connected = true;
      mp_send_hello_ack(mp);
      if (!was_connected)
        mp_push_simple_event(mp, MULTIPLAYER_EVENT_CONNECTED);
      break;
    }

    case MP_MSG_HELLO_ACK:
    {
      bool was_connected = mp->peer_connected;
      mp->peer_connected = true;
      if (!was_connected)
        mp_push_simple_event(mp, MULTIPLAYER_EVENT_CONNECTED);
      break;
    }

    case MP_MSG_ROLE:
      if (len >= 1 && payload[0] <= MULTIPLAYER_ROLE_DEFENDER)
      {
        bool was_connected = mp->peer_connected;
        mp->peer_connected = true;
        if (!was_connected)
          mp_push_simple_event(mp, MULTIPLAYER_EVENT_CONNECTED);
        mp->peer_role = (multiplayer_role_t)payload[0];
        mp_push_simple_event(mp, MULTIPLAYER_EVENT_ROLE_CHANGED);
      }
      break;

    case MP_MSG_START:
      mp->peer_connected = true;
      mp_push_simple_event(mp, MULTIPLAYER_EVENT_START_GAME);
      break;

    case MP_MSG_TOWER:
      if (len >= 5 && payload[0] < TOWER_KIND_COUNT)
      {
        multiplayer_event_t event;
        memset(&event, 0, sizeof(event));
        event.type = MULTIPLAYER_EVENT_TOWER_PLACED;
        event.data.tower.kind = (tower_kind_t)payload[0];
        event.data.tower.x = (int16_t)mp_read_u16(&payload[1]);
        event.data.tower.y = (int16_t)mp_read_u16(&payload[3]);
        (void)mp_push_event(mp, &event);
      }
      break;

    case MP_MSG_ENEMY:
      if (len >= 3 && payload[0] < ENEMY_KIND_COUNT)
      {
        multiplayer_event_t event;
        memset(&event, 0, sizeof(event));
        event.type = MULTIPLAYER_EVENT_ENEMY_SPAWNED;
        event.data.enemy.kind = (enemy_kind_t)payload[0];
        event.data.enemy.wave = mp_read_u16(&payload[1]);
        (void)mp_push_event(mp, &event);
      }
      break;

    case MP_MSG_TOWER_UPGRADE:
      if (len >= 3 && payload[2] > 0)
      {
        multiplayer_event_t event;
        memset(&event, 0, sizeof(event));
        event.type = MULTIPLAYER_EVENT_TOWER_UPGRADED;
        event.data.tower_upgrade.index = mp_read_u16(&payload[0]);
        event.data.tower_upgrade.level = payload[2];
        (void)mp_push_event(mp, &event);
      }
      break;

    case MP_MSG_TOWER_SELL:
      if (len >= 2)
      {
        multiplayer_event_t event;
        memset(&event, 0, sizeof(event));
        event.type = MULTIPLAYER_EVENT_TOWER_SOLD;
        event.data.tower_sell.index = mp_read_u16(&payload[0]);
        (void)mp_push_event(mp, &event);
      }
      break;

    case MP_MSG_TOWER_TARGET:
      if (len >= 3 && payload[2] <= TARGET_REWARD)
      {
        multiplayer_event_t event;
        memset(&event, 0, sizeof(event));
        event.type = MULTIPLAYER_EVENT_TOWER_TARGET_CHANGED;
        event.data.tower_target.index = mp_read_u16(&payload[0]);
        event.data.tower_target.mode = payload[2];
        (void)mp_push_event(mp, &event);
      }
      break;

    case MP_MSG_PAUSE:
      mp->local_resume_ready = false;
      mp->peer_resume_ready = false;
      mp_push_simple_event(mp, MULTIPLAYER_EVENT_PAUSE_REQUESTED);
      break;

    case MP_MSG_RESUME_READY:
      mp->peer_resume_ready = true;
      mp_push_simple_event(mp, MULTIPLAYER_EVENT_RESUME_READY);
      break;

    case MP_MSG_RESULT_ACTION:
      if (len >= 1 && payload[0] >= MULTIPLAYER_RESULT_ACTION_RESTART &&
          payload[0] <= MULTIPLAYER_RESULT_ACTION_NEXT)
      {
        mp->peer_result_action = (multiplayer_result_action_t)payload[0];
        mp_push_result_event(mp, mp->peer_result_action);
      }
      break;

    case MP_MSG_LEAVE:
      mp_push_simple_event(mp, MULTIPLAYER_EVENT_LEAVE);
      break;

    default:
      break;
  }
}

/**
 * @brief Feed one raw byte into the packet receiver state machine.
 *
 * Implements the state machine described in @ref multiplayer_protocol.
 * When a complete packet is assembled and its checksum verifies,
 * calls mp_handle_packet() to dispatch the message.
 *
 * @param mp   Multiplayer instance.
 * @param byte Raw byte from the serial port.
 */
static void mp_feed_byte(multiplayer_t *mp, uint8_t byte)
{
  if (mp == NULL)
    return;

  multiplayer_rx_t *rx = &mp->rx;

  switch ((mp_rx_state_t)rx->state)
  {
    case MP_RX_MAGIC_0:
      if (byte == MP_MAGIC_0)
        rx->state = MP_RX_MAGIC_1;
      break;

    case MP_RX_MAGIC_1:
      rx->state = (byte == MP_MAGIC_1) ? MP_RX_TYPE : MP_RX_MAGIC_0;
      break;

    case MP_RX_TYPE:
      rx->type = byte;
      rx->state = MP_RX_SEQ;
      break;

    case MP_RX_SEQ:
      rx->seq = byte;
      rx->state = MP_RX_LEN;
      break;

    case MP_RX_LEN:
      rx->len = byte;
      rx->index = 0;
      if (rx->len > sizeof(rx->payload))
      {
        mp_rx_reset(mp);
        return;
      }
      rx->state = (rx->len == 0) ? MP_RX_CHECKSUM : MP_RX_PAYLOAD;
      break;

    case MP_RX_PAYLOAD:
      rx->payload[rx->index++] = byte;
      if (rx->index >= rx->len)
        rx->state = MP_RX_CHECKSUM;
      break;

    case MP_RX_CHECKSUM:
      rx->checksum = byte;
      if (rx->checksum == mp_checksum(rx->type, rx->seq, rx->len, rx->payload))
        mp_handle_packet(mp, rx->type, rx->payload, rx->len);
      mp_rx_reset(mp);
      break;

    default:
      mp_rx_reset(mp);
      break;
  }
}

/**
 * @name Public API
 */
/** @{ */

void multiplayer_init(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  memset(mp, 0, sizeof(*mp));
  ringbuf_init(&mp->event_queue, mp->event_buf, MULTIPLAYER_EVENT_QUEUE_CAP);
  mp_rx_reset(mp);
}

void multiplayer_begin_wait(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  uint8_t next_seq = mp->tx_seq;
  multiplayer_init(mp);
  mp->tx_seq = next_seq;
  mp_send_hello(mp);
}

void multiplayer_mark_game_started(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  mp->game_started = true;
  mp->local_resume_ready = false;
  mp->peer_resume_ready = false;
  mp->local_result_action = MULTIPLAYER_RESULT_ACTION_NONE;
  mp->peer_result_action = MULTIPLAYER_RESULT_ACTION_NONE;
}

void multiplayer_tick(multiplayer_t *mp, uint32_t current_tick)
{
  if (mp == NULL || mp->game_started)
    return;

  if (!mp->peer_connected &&
      current_tick - mp->last_hello_tick >= MP_HELLO_INTERVAL_TICKS)
  {
    mp->last_hello_tick = current_tick;
    mp_send_hello(mp);
  }

  if (mp->peer_connected && mp->local_role != MULTIPLAYER_ROLE_NONE &&
      current_tick - mp->last_role_tick >= MP_ROLE_INTERVAL_TICKS)
  {
    mp->last_role_tick = current_tick;
    mp_send_role(mp);
    if (multiplayer_roles_ready(mp))
      mp_send_start(mp);
  }
}

void multiplayer_poll_serial(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  uint8_t byte;
  while (ser_recv_char_int(&byte) == 0)
    mp_feed_byte(mp, byte);
}

bool multiplayer_next_event(multiplayer_t *mp, multiplayer_event_t *event)
{
  if (mp == NULL || event == NULL)
    return false;

  return ringbuf_pop(&mp->event_queue, event, sizeof(*event)) == 0;
}

void multiplayer_select_role(multiplayer_t *mp, multiplayer_role_t role)
{
  if (mp == NULL || role == MULTIPLAYER_ROLE_NONE)
    return;

  mp->local_role = role;
  mp_send_role(mp);
  if (multiplayer_roles_ready(mp))
    mp_send_start(mp);
}

bool multiplayer_roles_ready(const multiplayer_t *mp)
{
  return mp != NULL &&
         mp->local_role != MULTIPLAYER_ROLE_NONE &&
         mp->peer_role != MULTIPLAYER_ROLE_NONE &&
         mp->local_role != mp->peer_role;
}

const char *multiplayer_role_name(multiplayer_role_t role)
{
  switch (role)
  {
    case MULTIPLAYER_ROLE_ATTACKER:
      return "ATTACK";
    case MULTIPLAYER_ROLE_DEFENDER:
      return "DEFEND";
    default:
      return "NONE";
  }
}

void multiplayer_send_tower(multiplayer_t *mp, tower_kind_t kind, int16_t x, int16_t y)
{
  uint8_t payload[5];

  if (mp == NULL || kind >= TOWER_KIND_COUNT)
    return;

  payload[0] = (uint8_t)kind;
  mp_write_u16(&payload[1], (uint16_t)x);
  mp_write_u16(&payload[3], (uint16_t)y);
  (void)mp_send_packet(mp, MP_MSG_TOWER, payload, sizeof(payload));
}

void multiplayer_send_enemy(multiplayer_t *mp, enemy_kind_t kind, uint16_t wave)
{
  uint8_t payload[3];

  if (mp == NULL || kind >= ENEMY_KIND_COUNT)
    return;

  payload[0] = (uint8_t)kind;
  mp_write_u16(&payload[1], wave);
  (void)mp_send_packet(mp, MP_MSG_ENEMY, payload, sizeof(payload));
}

void multiplayer_send_tower_upgrade(multiplayer_t *mp, uint16_t index, uint8_t level)
{
  uint8_t payload[3];

  if (mp == NULL || level == 0 || level > TOWER_MAX_LEVEL)
    return;

  mp_write_u16(&payload[0], index);
  payload[2] = level;
  (void)mp_send_packet(mp, MP_MSG_TOWER_UPGRADE, payload, sizeof(payload));
}

void multiplayer_send_tower_sell(multiplayer_t *mp, uint16_t index)
{
  uint8_t payload[2];

  if (mp == NULL)
    return;

  mp_write_u16(payload, index);
  (void)mp_send_packet(mp, MP_MSG_TOWER_SELL, payload, sizeof(payload));
}

void multiplayer_send_tower_target(multiplayer_t *mp, uint16_t index, uint8_t mode)
{
  uint8_t payload[3];

  if (mp == NULL || mode > TARGET_REWARD)
    return;

  mp_write_u16(&payload[0], index);
  payload[2] = mode;
  (void)mp_send_packet(mp, MP_MSG_TOWER_TARGET, payload, sizeof(payload));
}

void multiplayer_request_pause(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  mp->local_resume_ready = false;
  mp->peer_resume_ready = false;
  (void)mp_send_packet(mp, MP_MSG_PAUSE, NULL, 0);
}

void multiplayer_mark_local_resume_ready(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  mp->local_resume_ready = true;
  (void)mp_send_packet(mp, MP_MSG_RESUME_READY, NULL, 0);
}

bool multiplayer_both_resume_ready(const multiplayer_t *mp)
{
  return mp != NULL && mp->local_resume_ready && mp->peer_resume_ready;
}

void multiplayer_request_result_action(multiplayer_t *mp, multiplayer_result_action_t action)
{
  if (mp == NULL || action == MULTIPLAYER_RESULT_ACTION_NONE)
    return;

  mp->local_result_action = action;
  mp_send_result_action(mp, action);
}

bool multiplayer_both_result_ready(const multiplayer_t *mp)
{
  return mp != NULL &&
         mp->local_result_action != MULTIPLAYER_RESULT_ACTION_NONE &&
         mp->local_result_action == mp->peer_result_action;
}

void multiplayer_send_leave(multiplayer_t *mp)
{
  if (mp == NULL)
    return;

  (void)mp_send_packet(mp, MP_MSG_LEAVE, NULL, 0);
  multiplayer_init(mp);
}
/** @} */
