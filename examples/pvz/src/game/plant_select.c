#include "plant_select.h"
#include "seed_bank.h"

/* Todas as plantas disponíveis */
static const PlantType all_plants[TOTAL_AVAILABLE_PLANTS] = {
    SUNFLOWER, PEASHOOTER, REPEATER, CABBAGE,
    WALL_NUT,  POTATO_MINE, SPIKEWEED
};

/* Deck: 6 posições fixas; -1 = vazio */
static int deck[MAX_DECK_SIZE];

/* Cursor de navegação na grelha (índice 0-5) */
static int selection_cursor = 0;
static bool cursor_on_button = false;

/* Timer do flash vermelho (ticks restantes) */
static int deck_full_flash_timer = 0;

/* ── Helpers ─────────────────────────────────────────────────────── */

static int find_in_deck(PlantType type) {
    for (int i = 0; i < MAX_DECK_SIZE; i++) {
        if (deck[i] == (int)type) return i;
    }
    return -1;
}

/* ── API ─────────────────────────────────────────────────────────── */

void init_plant_selection(void) {
    for (int i = 0; i < MAX_DECK_SIZE; i++) deck[i] = -1;
    selection_cursor  = 0;
    cursor_on_button  = false;
    deck_full_flash_timer = 0;
}

bool is_cursor_on_button(void) {
    return cursor_on_button;
}

/* Atribui planta a slot específico (livre ordering) */
void assign_plant_to_slot(PlantType type, int slot) {
    if (slot < 0 || slot >= MAX_DECK_SIZE) return;

    /* Remove de onde estava (se já estiver no deck) */
    int current = find_in_deck(type);
    if (current >= 0) deck[current] = -1;

    /* Coloca no slot destino (substitui o que lá estava) */
    deck[slot] = (int)type;
}

/* Remove planta de um slot do deck */
void remove_plant_from_deck_slot(int slot) {
    if (slot >= 0 && slot < MAX_DECK_SIZE) deck[slot] = -1;
}

/* Toggle via click no inventário — adiciona ao primeiro slot livre */
void toggle_plant_in_deck(PlantType type) {
    int current = find_in_deck(type);
    if (current >= 0) {
        deck[current] = -1;
    } else {
        for (int i = 0; i < MAX_DECK_SIZE; i++) {
            if (deck[i] < 0) { deck[i] = (int)type; break; }
        }
    }
}

/* Cursor */
void move_selection_cursor(int dx, int dy) {
    deck_full_flash_timer = 0;

    if (cursor_on_button) {
        /* No botão: UP volta ao inventário, LEFT/RIGHT ficam no botão */
        if (dy < 0) {
            cursor_on_button = false;
        }
        return;
    }

    int col = selection_cursor % SEL_COLS;
    int row = selection_cursor / SEL_COLS;
    col = (col + dx + SEL_COLS) % SEL_COLS;

    /* DOWN na última linha → vai para o botão */
    if (dy > 0 && row == SEL_ROWS - 1) {
        cursor_on_button = true;
        return;
    }

    row = (row + dy + SEL_ROWS) % SEL_ROWS;
    selection_cursor = row * SEL_COLS + col;
}

int get_selection_cursor(void) { return selection_cursor; }

/* Consulta */
bool is_plant_in_deck(PlantType type) { return find_in_deck(type) >= 0; }

int get_deck_slot_for_type(PlantType type) { return find_in_deck(type); }

int get_deck_plant_at_slot(int slot) {
    if (slot < 0 || slot >= MAX_DECK_SIZE) return -1;
    return deck[slot];
}

int get_selected_deck_size(void) {
    int count = 0;
    for (int i = 0; i < MAX_DECK_SIZE; i++) if (deck[i] >= 0) count++;
    return count;
}

const PlantType *get_available_plants(void) { return all_plants; }

/* Hit detection */
bool get_plant_from_inventory_click(int x, int y, PlantType *out_type) {
    for (int row = 0; row < SEL_ROWS; row++) {
        for (int col = 0; col < SEL_COLS; col++) {
            int cx = SEL_GRID_X + col * (SEL_CARD_W + SEL_CARD_GAP);
            int cy = SEL_GRID_Y + row * (SEL_CARD_H + SEL_CARD_GAP);
            if (x >= cx && x < cx + SEL_CARD_W &&
                y >= cy && y < cy + SEL_CARD_H) {
                int idx = row * SEL_COLS + col;
                if (idx < TOTAL_AVAILABLE_PLANTS) {
                    *out_type = all_plants[idx];
                    return true;
                }
            }
        }
    }
    return false;
}

bool get_deck_slot_from_position(int x, int y, int *out_slot) {
    if (y < SEL_DECK_Y || y >= SEL_DECK_Y + SEL_CARD_H) return false;
    for (int i = 0; i < MAX_DECK_SIZE; i++) {
        int sx = SEL_DECK_X + i * (SEL_CARD_W + SEL_DECK_GAP);
        if (x >= sx && x < sx + SEL_CARD_W) {
            *out_slot = i;
            return true;
        }
    }
    return false;
}

bool is_start_button_clicked(int x, int y) {
    return x >= SEL_BTN_X && x < SEL_BTN_X + SEL_BTN_W &&
           y >= SEL_BTN_Y && y < SEL_BTN_Y + SEL_BTN_H;
}

void trigger_deck_full_flash(void) {
    deck_full_flash_timer = DECK_FULL_FLASH_TICKS;
}

void tick_deck_full_flash(void) {
    if (deck_full_flash_timer > 0) deck_full_flash_timer--;
}

bool is_deck_full_flash_active(void) {
    return deck_full_flash_timer > 0;
}

/* Confirma — empacota slots não-vazios em ordem e aplica ao seed bank */
void confirm_plant_selection(void) {
    PlantType packed[MAX_DECK_SIZE];
    int count = 0;
    for (int i = 0; i < MAX_DECK_SIZE; i++) {
        if (deck[i] >= 0) packed[count++] = (PlantType)deck[i];
    }
    if (count > 0) set_seed_slots(packed, count);
}
