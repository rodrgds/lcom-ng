#include "mouse_packet.h"

#include "utils.h"

static uint8_t buf[3];
static uint8_t idx = 0;
static bool ready = false;

void mouse_packet_reset(void) {
  idx = 0;
  ready = false;
}

int mouse_process_byte(uint8_t byte) {
  ready = false;
  if (idx == 0 && !(byte & MOUSE_SYNC_BIT)) return 0;
  buf[idx++] = byte;
  if (idx == 3) {
    idx = 0;
    ready = true;
    return 1;
  }
  return 0;
}

int mouse_get_packet(struct packet *pp) {
  if (!ready || pp == NULL) return 1;
  pp->bytes[0] = buf[0];
  pp->bytes[1] = buf[1];
  pp->bytes[2] = buf[2];
  pp->lb = (buf[0] & BIT(MOUSE_LB_BIT)) != 0;
  pp->rb = (buf[0] & BIT(MOUSE_RB_BIT)) != 0;
  pp->mb = (buf[0] & BIT(MOUSE_MB_BIT)) != 0;
  pp->x_ov = (buf[0] & BIT(MOUSE_X_OV_BIT)) != 0;
  pp->y_ov = (buf[0] & BIT(MOUSE_Y_OV_BIT)) != 0;
  pp->delta_x = (buf[0] & BIT(MOUSE_DELTA_X_SIGN_BIT))
                    ? (int16_t)(buf[1] | MOUSE_SIGN_EXTEND_MASK)
                    : (int16_t)buf[1];
  pp->delta_y = (buf[0] & BIT(MOUSE_DELTA_Y_SIGN_BIT))
                    ? (int16_t)(buf[2] | MOUSE_SIGN_EXTEND_MASK)
                    : (int16_t)buf[2];
  return 0;
}

void mouse_apply_packet_delta(const struct packet *pp,
                              int16_t *x,
                              int16_t *y,
                              int16_t min_x,
                              int16_t max_x,
                              int16_t min_y,
                              int16_t max_y) {
  if (pp == NULL || x == NULL || y == NULL || pp->x_ov || pp->y_ov) return;
  *x = util_clamp_i16((int32_t)(*x) + pp->delta_x, min_x, max_x);
  *y = util_clamp_i16((int32_t)(*y) - pp->delta_y, min_y, max_y);
}
