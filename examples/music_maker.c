#include "example_ui.h"

#include <lcom/ac97.h>
#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/lcom.h>
#include <lcom/vbe.h>

#include <stdbool.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MUSIC_STEPS 16u
#define MUSIC_NOTES 12u
#define MUSIC_TRACKS 4u

typedef enum {
  SYNTH_CHIP,
  SYNTH_BASS,
  SYNTH_BELL,
  SYNTH_DRUM,
} synth_kind_t;

typedef struct {
  const char *name;
  const char *subtitle;
  uint32_t color;
  synth_kind_t synth;
} track_def_t;

typedef struct {
  uint8_t notes[MUSIC_TRACKS][MUSIC_STEPS];
  bool muted[MUSIC_TRACKS];
  uint16_t tempo;
} pattern_snapshot_t;

typedef struct {
  uint8_t notes[MUSIC_TRACKS][MUSIC_STEPS];
  bool muted[MUSIC_TRACKS];
  uint8_t cursor_step;
  uint8_t cursor_row;
  uint8_t track;
  uint8_t play_step;
  uint16_t tempo;
  uint16_t step_phase;
  uint32_t submitted_steps;
  bool playing;
  bool extended;
  bool help;
  example_modifiers_t modifiers;
  int mouse_x, mouse_y;
  uint8_t mouse_bytes[3], mouse_index, mouse_buttons;
  bool mouse_ack;
  pattern_snapshot_t history[32];
  unsigned history_length, history_cursor;
  const char *path;
  char status[96];
  bool save_blocked, close_prompt;
  pattern_snapshot_t saved;

} sequencer_state_t;

static const char *note_names[MUSIC_NOTES] = {
    "B4", "A#4", "A4", "G#4", "G4", "F#4",
    "F4", "E4", "D#4", "D4", "C#4", "C4",
};

static const uint16_t note_hz[MUSIC_NOTES] = {
    494, 466, 440, 415, 392, 370, 349, 330, 311, 294, 277, 262,
};

static const track_def_t tracks[MUSIC_TRACKS] = {
    {"Chip", "Pulse lead", 0xFFBE55, SYNTH_CHIP},
    {"Bass", "Low square", 0x56D5C5, SYNTH_BASS},
    {"Bell", "Soft pluck", 0xA98BFF, SYNTH_BELL},
    {"Drums", "Kit", 0xFF7D6E, SYNTH_DRUM},
};

static int ac97_start_pcm(uint32_t sample_rate, size_t byte_count) {
  if (byte_count == 0 || byte_count > 0xFFFFu) return LCOM_ERR;
  if (lcom_port_write16(AC97_NAM_BASE + AC97_PCM_FRONT_DAC_RATE,
                        (uint16_t)sample_rate) != LCOM_OK) return LCOM_ERR;
  if (lcom_port_write16(AC97_BM_BASE + AC97_PO_PICB,
                        (uint16_t)byte_count) != LCOM_OK) return LCOM_ERR;
  return lcom_port_write8(AC97_BM_BASE + AC97_PO_CR, AC97_PO_CR_RUN);
}

static void ac97_stop_pcm(void) {
  lcom_port_write8(AC97_BM_BASE + AC97_PO_CR, 0);
}

static void sequencer_seed(sequencer_state_t *st) {
  for (uint8_t track = 0; track < MUSIC_TRACKS; track++) {
    for (uint8_t step = 0; step < MUSIC_STEPS; step++) st->notes[track][step] = 0;
    st->muted[track] = false;
  }

  const uint8_t lead[MUSIC_STEPS] = {12, 0, 10, 0, 8, 0, 7, 0, 8, 0, 5, 0, 7, 0, 10, 0};
  const uint8_t bass[MUSIC_STEPS] = {12, 0, 0, 0, 8, 0, 0, 0, 7, 0, 0, 0, 10, 0, 0, 0};
  const uint8_t bell[MUSIC_STEPS] = {0, 8, 0, 0, 0, 5, 0, 0, 0, 8, 0, 0, 0, 10, 0, 5};
  const uint8_t drum[MUSIC_STEPS] = {12, 2, 2, 2, 7, 2, 2, 2, 12, 2, 2, 2, 7, 2, 2, 2};
  for (uint8_t step = 0; step < MUSIC_STEPS; step++) {
    st->notes[0][step] = lead[step];
    st->notes[1][step] = bass[step];
    st->notes[2][step] = bell[step];
    st->notes[3][step] = drum[step];
  }

  st->cursor_step = 0;
  st->cursor_row = 11;
  st->track = 0;
  st->play_step = 0;
  st->tempo = 126;
  st->step_phase = 0;
  st->submitted_steps = 0;
  st->playing = false;
  st->extended = false;
}

static void snapshot(const sequencer_state_t *st, pattern_snapshot_t *out) {
  memcpy(out->notes, st->notes, sizeof(out->notes));
  memcpy(out->muted, st->muted, sizeof(out->muted));
  out->tempo = st->tempo;
}

static bool same_pattern(const sequencer_state_t *st, const pattern_snapshot_t *snap) {
  return !memcmp(st->notes, snap->notes, sizeof(st->notes)) &&
         !memcmp(st->muted, snap->muted, sizeof(st->muted)) && st->tempo == snap->tempo;
}

static void remember_pattern(sequencer_state_t *st) {
  if (st->history_length && same_pattern(st, &st->history[st->history_cursor])) return;
  if (st->history_length) st->history_length = st->history_cursor + 1;
  if (st->history_length == 32) {
    memmove(st->history, st->history + 1, sizeof(st->history[0]) * 31);
    st->history_length--;
  }
  snapshot(st, &st->history[st->history_length++]);
  st->history_cursor = st->history_length - 1;
}

static void undo_pattern(sequencer_state_t *st, bool redo) {
  remember_pattern(st);
  if ((redo && st->history_cursor + 1 >= st->history_length) || (!redo && !st->history_cursor)) {
    snprintf(st->status, sizeof(st->status), "Nothing to %s", redo ? "redo" : "undo"); return;
  }
  st->history_cursor += redo ? 1 : -1;
  const pattern_snapshot_t *snap = &st->history[st->history_cursor];
  memcpy(st->notes, snap->notes, sizeof(st->notes));
  memcpy(st->muted, snap->muted, sizeof(st->muted)); st->tempo = snap->tempo;

  snprintf(st->status, sizeof(st->status), "%s", redo ? "Redone" : "Undone");
}

static void save_pattern(sequencer_state_t *st) {
  if (st->save_blocked) { snprintf(st->status, sizeof(st->status), "Cannot overwrite an unreadable pattern. Choose another path."); return; }
  char temporary[1024];
  int n = snprintf(temporary, sizeof(temporary), "%s.tmp", st->path);
  if (n < 0 || (size_t)n >= sizeof(temporary)) { snprintf(st->status, sizeof(st->status), "Save failed: path is too long"); return; }
  FILE *out = fopen(temporary, "wbx");
  if (!out) { snprintf(st->status, sizeof(st->status), "Save failed. Check folder permissions and temporary files."); return; }
  bool okay = fprintf(out, "LCOM-PATTERN 1 %u\n", st->tempo) > 0;
  for (unsigned t = 0; t < MUSIC_TRACKS; t++) {
    okay = fprintf(out, "%u", st->muted[t] ? 1 : 0) > 0 && okay;
    for (unsigned i = 0; i < MUSIC_STEPS; i++) okay = fprintf(out, " %u", st->notes[t][i]) > 0 && okay;
    okay = fputc('\n', out) != EOF && okay;
  }
  if (fclose(out) != 0) okay = false;
  if (!okay || rename(temporary, st->path) != 0) { remove(temporary); snprintf(st->status, sizeof(st->status), "Save failed. Original pattern preserved."); return; }
   snapshot(st, &st->saved);
  snprintf(st->status, sizeof(st->status), "Pattern saved. Reopen this file to keep composing.");
}

static void open_pattern(sequencer_state_t *st) {
  FILE *in = fopen(st->path, "rb");
  if (!in) {
    if (errno != ENOENT) { st->save_blocked = true; snprintf(st->status, sizeof(st->status), "Cannot read pattern. Choose a readable file."); }
    return;
  }
  pattern_snapshot_t loaded = {0}; unsigned version, tempo;
  bool okay = fscanf(in, "LCOM-PATTERN %u %u", &version, &tempo) == 2 && version == 1 && tempo >= 70 && tempo <= 200;
  for (unsigned t = 0; t < MUSIC_TRACKS && okay; t++) {
    unsigned mute = 0; okay = fscanf(in, "%u", &mute) == 1 && mute <= 1; loaded.muted[t] = mute != 0;
    for (unsigned i = 0; i < MUSIC_STEPS && okay; i++) {
      unsigned note; okay = fscanf(in, "%u", &note) == 1 && note <= MUSIC_NOTES;
      if (okay) loaded.notes[t][i] = (uint8_t)note;
    }
  }
  int extra;
  do { extra = fgetc(in); } while (extra == ' ' || extra == '\n' || extra == '\r' || extra == '\t');
  if (extra != EOF || ferror(in)) okay = false;
  fclose(in);
  if (!okay) { st->save_blocked = true; snprintf(st->status, sizeof(st->status), "Not a valid pattern. Existing file will be preserved."); return; }
  memcpy(st->notes, loaded.notes, sizeof(st->notes)); memcpy(st->muted, loaded.muted, sizeof(st->muted)); st->tempo = (uint16_t)tempo;
  snprintf(st->status, sizeof(st->status), "Pattern opened from disk");
}

static void change_tempo(sequencer_state_t *st, int amount) {
  int tempo = st->tempo + amount;
  if (tempo < 70 || tempo > 200) return;
  remember_pattern(st); st->tempo = (uint16_t)tempo;
  snprintf(st->status, sizeof(st->status), "Tempo %u BPM", st->tempo);
}

static uint32_t count_active(const sequencer_state_t *st) {
  uint32_t active = 0;
  for (uint8_t track = 0; track < MUSIC_TRACKS; track++) {
    for (uint8_t step = 0; step < MUSIC_STEPS; step++) {
      if (st->notes[track][step] != 0) active++;
    }
  }
  return active;
}

static uint32_t frames_for_step(uint16_t tempo) {
  uint32_t frames = AC97_DEFAULT_SAMPLE_RATE * 60u / ((uint32_t)tempo * 4u);
  if (frames < 1800u) frames = 1800u;
  if (frames > 12000u) frames = 12000u;
  return frames;
}

static int16_t square_wave(uint32_t frame, uint16_t hz, int32_t amplitude) {
  uint32_t period = AC97_DEFAULT_SAMPLE_RATE / hz;
  if (period < 2u) period = 2u;
  return (frame % period) < period / 2u ? (int16_t)amplitude : (int16_t)-amplitude;
}

static int16_t triangle_wave(uint32_t frame, uint16_t hz, int32_t amplitude) {
  uint32_t period = AC97_DEFAULT_SAMPLE_RATE / hz;
  if (period < 4u) period = 4u;
  uint32_t phase = frame % period;
  int32_t value = phase < period / 2u
                      ? -amplitude + (int32_t)(phase * (uint32_t)(4 * amplitude) / period)
                      : 3 * amplitude - (int32_t)(phase * (uint32_t)(4 * amplitude) / period);
  return (int16_t)value;
}

static int16_t drum_wave(uint32_t frame, uint32_t total, uint8_t note) {
  uint32_t decay = total > frame ? total - frame : 0;
  if (note >= 10u) {
    uint32_t hz = 72u - (frame * 34u / (total == 0 ? 1u : total));
    int32_t amp = (int32_t)(6800u * decay / total);
    return square_wave(frame, (uint16_t)hz, amp);
  }
  uint32_t noise = frame * 1103515245u + 12345u + (uint32_t)note * 977u;
  int32_t sign = ((noise >> 16) & 1u) != 0 ? 1 : -1;
  uint32_t gate = note <= 3u ? total / 4u : total / 2u;
  if (frame >= gate) return 0;
  int32_t amp = note <= 3u ? 2600 : 4600;
  return (int16_t)(sign * amp * (int32_t)(gate - frame) / (int32_t)gate);
}

static int16_t synth_sample(synth_kind_t synth, uint8_t note, uint32_t frame,
                            uint32_t total) {
  if (note == 0 || note > MUSIC_NOTES) return 0;
  uint8_t row = (uint8_t)(note - 1u);
  uint16_t hz = note_hz[row];
  uint32_t gate = total * 7u / 8u;
  if (synth != SYNTH_DRUM && frame >= gate) return 0;
  uint32_t remaining = gate > frame ? gate - frame : 0;
  switch (synth) {
  case SYNTH_CHIP:
    return (int16_t)((int32_t)square_wave(frame, hz, 3100) * (int32_t)remaining /
                     (int32_t)gate);
  case SYNTH_BASS:
    return square_wave(frame, (uint16_t)(hz / 2u), 3900);
  case SYNTH_BELL: {
    int32_t body = triangle_wave(frame, (uint16_t)(hz * 2u), 2500);
    int32_t overtone = triangle_wave(frame, (uint16_t)(hz * 3u), 1100);
    int32_t env = (int32_t)(remaining * remaining / gate);
    return (int16_t)((body + overtone) * env / (int32_t)gate);
  }
  case SYNTH_DRUM:
    return drum_wave(frame, total, note);
  }
  return 0;
}

static size_t render_step_audio(const sequencer_state_t *st, uint8_t step,
                                int16_t *pcm, size_t pcm_bytes) {
  const uint8_t channels = AC97_DEFAULT_CHANNELS;
  uint32_t frames = frames_for_step(st->tempo);
  size_t sample_count = (size_t)frames * channels;
  size_t byte_count = sample_count * sizeof(int16_t);
  if (byte_count > pcm_bytes || byte_count > 0xFFFFu) return 0;
  for (uint32_t frame = 0; frame < frames; frame++) {
    int32_t left = 0;
    int32_t right = 0;
    uint8_t voices = 0;
    for (uint8_t track = 0; track < MUSIC_TRACKS; track++) {
      if (st->muted[track] || st->notes[track][step] == 0) continue;
      int32_t sample = synth_sample(tracks[track].synth, st->notes[track][step], frame, frames);
      if (track == 0 || track == 2) {
        left += sample;
        right += sample * 3 / 4;
      } else {
        left += sample * 3 / 4;
        right += sample;
      }
      voices++;
    }
    if (voices > 2) {
      left = left * 2 / 3;
      right = right * 2 / 3;
    }
    if (left > 15000) left = 15000;
    if (left < -15000) left = -15000;
    if (right > 15000) right = 15000;
    if (right < -15000) right = -15000;
    pcm[frame * channels] = (int16_t)left;
    pcm[frame * channels + 1u] = (int16_t)right;
  }
  return byte_count;
}

static int play_current_step(sequencer_state_t *st, int16_t *pcm, size_t pcm_bytes) {
  size_t byte_count = render_step_audio(st, st->play_step, pcm, pcm_bytes);
  if (byte_count == 0) return LCOM_ERR;
  if (ac97_start_pcm(AC97_DEFAULT_SAMPLE_RATE, byte_count) != LCOM_OK) return LCOM_ERR;
  st->submitted_steps++;
  return LCOM_OK;
}

static void toggle_playback(sequencer_state_t *st, int16_t *pcm, size_t pcm_bytes) {
  st->playing = !st->playing;
  st->step_phase = 0;
  if (st->playing) {
    play_current_step(st, pcm, pcm_bytes);
  } else {
    ac97_stop_pcm();
  }
}

static void move_cursor_extended(sequencer_state_t *st, uint8_t code) {
  switch (code) {
  case 0x4B: st->cursor_step = (uint8_t)((st->cursor_step + MUSIC_STEPS - 1u) % MUSIC_STEPS); break;
  case 0x4D: st->cursor_step = (uint8_t)((st->cursor_step + 1u) % MUSIC_STEPS); break;
  case 0x48: st->cursor_row = (uint8_t)((st->cursor_row + MUSIC_NOTES - 1u) % MUSIC_NOTES); break;
  case 0x50: st->cursor_row = (uint8_t)((st->cursor_row + 1u) % MUSIC_NOTES); break;
  default: break;
  }
}

static void clear_track(sequencer_state_t *st) {
  for (uint8_t step = 0; step < MUSIC_STEPS; step++) st->notes[st->track][step] = 0;
}

static void handle_keyboard_byte(sequencer_state_t *st, uint8_t data, bool *running,
                                 int16_t *pcm, size_t pcm_bytes) {
  if (example_modifier_byte(&st->modifiers, data)) { st->extended = false; return; }
  if (data == KBD_TWO_BYTE_PFX) {
    st->extended = true;
    return;
  }
  bool break_code = (data & KBD_BREAK_BIT) != 0;
  uint8_t code = (uint8_t)(data & 0x7Fu);
  if (st->extended) {
    if (!break_code && !st->help && !st->close_prompt) {
      if (code == 0x47) { st->play_step = 0; st->step_phase = 0; if (st->playing) play_current_step(st, pcm, pcm_bytes); }
      else if (code == 0x53) { remember_pattern(st); st->notes[st->track][st->cursor_step] = 0; }
      else move_cursor_extended(st, code);
    }
    st->extended = false;
    return;
  }
  if (data == ESC_BREAK) {
    if (st->help) { st->help = false; return; }
    if (st->close_prompt) { st->close_prompt = false; return; }
    if (!same_pattern(st, &st->saved)) st->close_prompt = true; else *running = false;
    return;
  }
  if (break_code) return;
  if (st->close_prompt) {
    if (code == 0x20) *running = false;
    else if (code == 0x1F || code == 0x1C) { save_pattern(st); if (same_pattern(st, &st->saved)) *running = false; }
    return;
  }
  if (st->help && code != 0x3B) return;
  if (example_primary(&st->modifiers)) {
    if (code == 0x11) { if (!same_pattern(st, &st->saved)) st->close_prompt = true; else *running = false; return; }
    if (code == 0x1F) save_pattern(st);
    else if (code == 0x2C) undo_pattern(st, st->modifiers.shift != 0);
    else if (code == 0x15) undo_pattern(st, true);
    return;
  }
  if (code == 0x3B) { st->help = !st->help; return; }
  if (st->help) return;
  if (code == 0x1C || code == 0x0E || code == 0x2E) {
    remember_pattern(st);
  }
  if (code == 0x1C) {
    uint8_t note = (uint8_t)(st->cursor_row + 1u);
    st->notes[st->track][st->cursor_step] =
        st->notes[st->track][st->cursor_step] == note ? 0 : note;
  } else if (code == 0x0E) {
    st->notes[st->track][st->cursor_step] = 0;
  } else if (code == 0x0F) {
    st->track = (uint8_t)((st->track + 1u) % MUSIC_TRACKS);
  } else if (code >= 0x02 && code <= 0x05) {
    st->track = (uint8_t)(code - 0x02);
  } else if (code == 0x39 || code == 0x19) {
    toggle_playback(st, pcm, pcm_bytes);
  } else if (code == 0x0C && st->tempo > 70) {
    change_tempo(st, -2);
  } else if (code == 0x0D && st->tempo < 200) {
    change_tempo(st, 2);
  } else if (code == 0x32) {
    remember_pattern(st);
    st->muted[st->track] = !st->muted[st->track];
  } else if (code == 0x2E) {
    clear_track(st);
  } else if (code == 0x1F || code == 0x3C) {
    save_pattern(st);
  } else if (code == 0x13) {
    remember_pattern(st);
    bool was_playing = st->playing;
    sequencer_seed(st);
    if (was_playing) toggle_playback(st, pcm, pcm_bytes);
  }
}

static void handle_mouse_byte(sequencer_state_t *st, uint8_t byte,
                               int16_t *pcm, size_t pcm_bytes, bool *running) {
  if (!st->mouse_ack) { if (byte == MOUSE_ACK) st->mouse_ack = true; return; }
  if (st->mouse_index == 0 && !(byte & MOUSE_SYNC_BIT)) return;
  st->mouse_bytes[st->mouse_index++] = byte;
  if (st->mouse_index < 3) return;
  st->mouse_index = 0;
  uint8_t flags = st->mouse_bytes[0];
  if (flags & 0xC0) return;
  int dx = (int)st->mouse_bytes[1] - ((flags & BIT(4)) ? 256 : 0);
  int dy = (int)st->mouse_bytes[2] - ((flags & BIT(5)) ? 256 : 0);
  st->mouse_x += dx; st->mouse_y -= dy;
  if (st->mouse_x < 0) st->mouse_x = 0;
  if (st->mouse_x > 799) st->mouse_x = 799;
  if (st->mouse_y < 0) st->mouse_y = 0;
  if (st->mouse_y > 599) st->mouse_y = 599;
  bool pressed = (flags & 1) && !(st->mouse_buttons & 1);
  st->mouse_buttons = flags & 7;
  if (!pressed || st->help) return;
  if (st->close_prompt) {
    if (st->mouse_y >= 333 && st->mouse_y < 373) {
      if (st->mouse_x >= 178 && st->mouse_x < 318) { save_pattern(st); if (same_pattern(st, &st->saved)) *running = false; }
      else if (st->mouse_x >= 330 && st->mouse_x < 470) *running = false;
      else if (st->mouse_x >= 482 && st->mouse_x < 622) st->close_prompt = false;
    }
    return;
  }
  int x = st->mouse_x, y = st->mouse_y;
  if (y >= 82 && y < 110) {
    if (x >= 28 && x < 114) save_pattern(st);
    else if (x >= 122 && x < 196) undo_pattern(st, false);
    else if (x >= 204 && x < 278) undo_pattern(st, true);
    else if (x >= 646 && x < 772) st->help = true;
    return;
  }
  if (y >= 20 && y < 56 && x >= 500 && x < 532) { change_tempo(st, -2); return; }
  if (y >= 20 && y < 56 && x >= 604 && x < 636) { change_tempo(st, 2); return; }
  if (x >= 278 && x < 742 && y >= 142 && y < 430) {
    st->cursor_step = (uint8_t)((x - 278) / 29);
    st->cursor_row = (uint8_t)((y - 142) / 24);
    handle_keyboard_byte(st, 0x1C, &(bool){true}, pcm, pcm_bytes);
  } else if (x >= 32 && x < 204 && y >= 148 && y < 420) {
    st->track = (uint8_t)((y - 148) / 68);
    if (x >= 170) { remember_pattern(st); st->muted[st->track] = !st->muted[st->track]; }
  } else if (x >= 278 && x < 742 && y >= 460 && y < 548) {
    st->track = (uint8_t)((y - 460) / 22);
    st->cursor_step = (uint8_t)((x - 278) / 29);
    if (st->notes[st->track][st->cursor_step]) st->cursor_row = st->notes[st->track][st->cursor_step] - 1;
  } else if (x >= 646 && x < 772 && y >= 16 && y < 60) {
    toggle_playback(st, pcm, pcm_bytes);
  }
}

static bool hovered(const sequencer_state_t *st, int x, int y, int w, int h) {
  return st->mouse_x >= x && st->mouse_y >= y && st->mouse_x < x + w && st->mouse_y < y + h;
}

static void dark_button(example_canvas_t *ctx, const sequencer_state_t *st, int x, int y, int w, int h, const char *text, bool active, bool enabled) {
  uint32_t bg = active ? 0x6D5BA1 : hovered(st,x,y,w,h) && enabled ? 0x3C485D : UI_PANEL;
  example_round_rect(ctx, x, y, w, h, 5, bg);
  example_label(ctx, x + (w - example_label_width(text, 1)) / 2, y + (h - 18) / 2, text, enabled ? UI_LIGHT : 0x8290A5, 1);
}

static void draw_transport(example_canvas_t *ctx, const sequencer_state_t *st) {
  example_heading(ctx, "Pocket Composer", "Four voices. One good idea.", 0xA994EB, true);
  char tempo[24]; snprintf(tempo, sizeof(tempo), "%u BPM", st->tempo);
  dark_button(ctx,st,500,20,32,36,"-",false,st->tempo>70);
  example_label(ctx, 541, 28, tempo, UI_LIGHT, 1);
  dark_button(ctx,st,604,20,32,36,"+",false,st->tempo<200);
  dark_button(ctx,st,646,16,126,44,st->playing ? "Pause" : "Play  Space",st->playing,true);
  dark_button(ctx,st,28,82,86,28,"Save",false,!st->save_blocked);
  dark_button(ctx,st,122,82,74,28,"Undo",false,st->history_cursor>0 || (st->history_length && !same_pattern(st,&st->history[st->history_cursor])));
  dark_button(ctx,st,204,82,74,28,"Redo",false,st->history_cursor+1<st->history_length);
  example_label(ctx, 300, 87, !same_pattern(st, &st->saved) ? "Unsaved pattern" : "16 steps / 4 beats", UI_DIM, 1);
  dark_button(ctx,st,646,82,126,28,"Shortcuts  F1",false,true);
}

static void draw_track_panel(example_canvas_t *ctx, const sequencer_state_t *st) {
  example_label(ctx, 32, 122, "Instruments", UI_DIM, 1);
  for (uint8_t track = 0; track < MUSIC_TRACKS; track++) {
    int y = 148 + track * 68;
    example_round_rect(ctx, 32, y, 172, 56, 6, track == st->track ? 0x38485D : UI_PANEL);
    example_round_rect(ctx, 44, y + 15, 10, 26, 3, tracks[track].color);
    example_label(ctx, 68, y + 5, tracks[track].name, UI_LIGHT, 1);
    example_label(ctx, 68, y + 30, st->muted[track] ? "Muted" : tracks[track].subtitle, UI_DIM, 1);
    example_round_rect(ctx, 174, y + 9, 22, 22, 4, st->muted[track] ? 0x8B4354 : 0x3C485B);
    example_label(ctx, 179, y + 11, st->muted[track] ? "M" : "M", UI_LIGHT, 1);
  }
  example_label(ctx, 32, 432, "Tab  Next instrument", UI_DIM, 1);
}


static void draw_piano_roll(example_canvas_t *ctx, const sequencer_state_t *st) {
  const int grid_x = 278;
  const int grid_y = 142;
  const int cell_w = 29;
  const int cell_h = 24;
  for (uint8_t step = 0; step < MUSIC_STEPS; step++) {
    char label[3];
    snprintf(label, sizeof(label), "%u", (unsigned)(step + 1u));
    uint32_t color = step == st->play_step && st->playing
                         ? example_rgb(178, 148, 255)
                         : example_rgb(58, 65, 81);
    example_fill_rect(ctx, grid_x + step * cell_w, 116, cell_w - 2, 20, color);
    example_draw_text(ctx, grid_x + step * cell_w + (step < 9 ? 9 : 5), 122,
                      label, example_rgb(230, 232, 238), 1);
  }
  for (uint8_t row = 0; row < MUSIC_NOTES; row++) {
    bool black = note_names[row][1] == '#';
    example_fill_rect(ctx, 230, grid_y + row * cell_h, 42, cell_h - 1,
                      black ? example_rgb(48, 54, 68) : example_rgb(221, 223, 228));
    example_draw_text(ctx, 238, grid_y + row * cell_h + 8, note_names[row],
                      black ? example_rgb(225, 228, 236) : example_rgb(52, 58, 72), 1);
    for (uint8_t step = 0; step < MUSIC_STEPS; step++) {
      int x = grid_x + step * cell_w;
      int y = grid_y + row * cell_h;
      uint32_t base = step % 4u == 0 ? example_rgb(48, 54, 69) : example_rgb(39, 45, 58);
      if (st->playing && step == st->play_step) base = example_rgb(60, 61, 82);
      example_fill_rect(ctx, x, y, cell_w - 2, cell_h - 2, base);
      for (uint8_t track = 0; track < MUSIC_TRACKS; track++) {
        if (st->notes[track][step] != row + 1u) continue;
        if (track == st->track) {
          example_fill_rect(ctx, x + 4, y + 5, cell_w - 10, cell_h - 11, tracks[track].color);
        } else {
          example_fill_rect(ctx, x + 4 + track * 5, y + cell_h - 7, 4, 3, tracks[track].color);
        }
      }
    }
  }
  int cursor_x = grid_x + st->cursor_step * cell_w;
  int cursor_y = grid_y + st->cursor_row * cell_h;
  example_stroke_rect(ctx, cursor_x - 2, cursor_y - 2, cell_w + 2, cell_h + 2, 2,
                      example_rgb(247, 246, 250));
}

static void draw_bottom_panel(example_canvas_t *ctx, const sequencer_state_t *st) {
  example_label(ctx, 32, 466, "Pattern overview", UI_DIM, 1);
  example_label(ctx, 32, 494, "Click a lane", UI_DIM, 1);
  example_label(ctx, 32, 516, "to jump to it", UI_DIM, 1);
  for (uint8_t track = 0; track < MUSIC_TRACKS; track++) {
    int y = 460 + track * 22;
    example_label(ctx, 222, y+3, tracks[track].name, track == st->track ? UI_LIGHT : UI_DIM, 1);
    for (unsigned step=0; step<MUSIC_STEPS; step++) {
      int x=278+(int)step*29;
      uint32_t bg = st->playing && step == st->play_step ? 0x4B4868 : 0x273142;
      example_fill_rect(ctx,x,y,27,18,bg);
      if (st->notes[track][step]) {
        int h = 3 + (MUSIC_NOTES-st->notes[track][step]);
        example_fill_rect(ctx,x+4,y+16-h,19,h,st->muted[track]?0x647186:tracks[track].color);
      }
      if (track==st->track) example_fill_rect(ctx,x,y+18,27,1,tracks[track].color);
    }
  }
}

static void draw_music_maker(example_canvas_t *ctx, const sequencer_state_t *st) {
  example_fill_rect(ctx, 0, 0, 800, 600, UI_DARK);
  draw_transport(ctx, st);
  draw_track_panel(ctx, st);
  draw_piano_roll(ctx, st);
  draw_bottom_panel(ctx, st);
  example_label(ctx, 26, 556, st->status, st->save_blocked ? 0xFFA99C : UI_DIM, 1);
  example_label(ctx, 26, 579, "Space Play   Enter Note   1-4 Track   M Mute   Cmd/Ctrl+S Save   F1 Help", UI_DIM, 1);
  example_fill_rect(ctx, st->mouse_x - 4, st->mouse_y, 9, 1, UI_LIGHT);
  example_fill_rect(ctx, st->mouse_x, st->mouse_y - 4, 1, 9, UI_LIGHT);
  if (st->help) {
    example_round_rect(ctx, 116, 126, 568, 350, 10, 0x2C3A4B);
    example_label(ctx, 168, 174, "Composer shortcuts", UI_LIGHT, 2);
    const char *lines[] = {"Space / P   Play or pause       - / +   Tempo", "Arrows   Move      Enter / Click   Toggle note", "1-4 / Tab   Choose instrument      M   Mute", "Backspace / Delete   Remove note   C   Clear track", "Cmd/Ctrl+Z   Undo   Cmd/Ctrl+Shift+Z / Ctrl+Y   Redo", "Cmd/Ctrl+S / F2 / S   Save pattern", "R   Restore demo pattern       F1 / Esc   Close help"};
    for (int i = 0; i < 7; i++) example_label(ctx, 144, 220 + i * 30, lines[i], UI_DIM, 1);
  }
  if (st->close_prompt) {
    example_round_rect(ctx,154,214,492,178,10,UI_PANEL);
    example_label(ctx,178,238,"Save this composition?",UI_LIGHT,2);
    example_label(ctx,178,286,"Your pattern has changes that are not saved.",UI_DIM,1);
    dark_button(ctx,st,178,333,140,40,"Save  Enter",true,true);
    dark_button(ctx,st,330,333,140,40,"Discard  D",false,true);
    dark_button(ctx,st,482,333,140,40,"Cancel  Esc",false,true);
  }
}


int main(int argc, char **argv) {
  if (lcom_init() != LCOM_OK) return 1;
  lcom_vbe_mode_info_t video_info;
  if (lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &video_info) != LCOM_OK) return 1;
  if (lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24) != LCOM_OK) return 1;
  uint8_t *fb = 0;
  if (lcom_phys_map(video_info.framebuffer_phys, video_info.framebuffer_size, (void **)&fb) != LCOM_OK) return 1;
  lcom_ac97_buffer_info_t audio_info;
  if (lcom_ac97_get_buffer(&audio_info) != LCOM_OK) return 1;
  int16_t *pcm = 0;
  if (lcom_phys_map(audio_info.pcm_phys, audio_info.pcm_bytes, (void **)&pcm) != LCOM_OK) return 1;
  lcom_irq_t timer_irq;
  lcom_irq_t kbd_irq;
  lcom_irq_t mouse_irq;
  if (lcom_irq_subscribe(TIMER0_IRQ, 0, &timer_irq) != LCOM_OK) return 1;
  if (lcom_irq_subscribe(KBC_IRQ, 0, &kbd_irq) != LCOM_OK) return 1;
  sequencer_state_t sequencer = {0};
  sequencer_seed(&sequencer);
  sequencer.path = argc > 1 ? argv[1] : "build/pocket-composer.mlpat";
  snprintf(sequencer.status, sizeof(sequencer.status), "Click the piano roll to write. Click M on a track to mute it.");
  open_pattern(&sequencer);
  remember_pattern(&sequencer);
  snapshot(&sequencer, &sequencer.saved);
  sequencer.mouse_x = 400; sequencer.mouse_y = 300;
  if (lcom_irq_subscribe(MOUSE_IRQ, 0, &mouse_irq) != LCOM_OK) return 1;
  if (lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) != LCOM_OK ||
      lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_ENABLE_DR) != LCOM_OK) return 1;

  example_canvas_t canvas = {fb, video_info};
  bool running = true;
  draw_music_maker(&canvas, &sequencer);
  lcom_vbe_present();
  while (running) {
    lcom_event_t ev;
    if (lcom_event_wait(&ev) != LCOM_OK) break;
    if ((ev.irq_mask & timer_irq.mask) != 0) {
      if (sequencer.playing) {
        sequencer.step_phase = (uint16_t)(sequencer.step_phase + sequencer.tempo * 4u);
        if (sequencer.step_phase >= 3600u) {
          sequencer.step_phase = (uint16_t)(sequencer.step_phase - 3600u);
          sequencer.play_step = (uint8_t)((sequencer.play_step + 1u) % MUSIC_STEPS);
          play_current_step(&sequencer, pcm, audio_info.pcm_bytes);
        }
      }
      draw_music_maker(&canvas, &sequencer);
      lcom_vbe_present();
    }
    if ((ev.irq_mask & mouse_irq.mask) != 0) {
      uint8_t status = 0, data = 0;
      while (lcom_port_read8(KBC_ST_REG, &status) == LCOM_OK &&
             (status & KBC_ST_OBF) && (status & KBC_ST_AUX) &&
             lcom_port_read8(KBC_OUT_BUF, &data) == LCOM_OK)
        handle_mouse_byte(&sequencer, data, pcm, audio_info.pcm_bytes, &running);
    }
    if ((ev.irq_mask & kbd_irq.mask) != 0) {
      uint8_t status = 0;
      uint8_t data = 0;
      if (lcom_port_read8(KBC_ST_REG, &status) == LCOM_OK && (status & KBC_ST_OBF) != 0 &&
          (status & KBC_ST_AUX) == 0 && lcom_port_read8(KBC_OUT_BUF, &data) == LCOM_OK) {
        handle_keyboard_byte(&sequencer, data, &running, pcm, audio_info.pcm_bytes);
        draw_music_maker(&canvas, &sequencer);
        lcom_vbe_present();
      }
    }
  }
  lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE);
  lcom_port_write8(KBC_IN_BUF, MOUSE_CMD_DISABLE_DR);
  lcom_irq_unsubscribe(&mouse_irq);
  ac97_stop_pcm();
  draw_music_maker(&canvas, &sequencer);
  lcom_vbe_present();
  lcom_printf("music maker pattern tracks=%u steps=%u active=%u tempo=%u submitted=%u\n",
              MUSIC_TRACKS, MUSIC_STEPS, count_active(&sequencer), sequencer.tempo,
              (unsigned)sequencer.submitted_steps);
  lcom_phys_unmap(pcm, audio_info.pcm_bytes);
  lcom_irq_unsubscribe(&kbd_irq);
  lcom_irq_unsubscribe(&timer_irq);
  lcom_phys_unmap(fb, video_info.framebuffer_size);
  lcom_exit();
  return 0;
}
