#ifndef _PLANT_SELECT_H_
#define _PLANT_SELECT_H_

#include <stdbool.h>
#include "constants.h"
#include "plant.h"

/* Inicializa o estado de seleção */
void init_plant_selection(void);

/* Toggle via click no inventário — adiciona ao primeiro slot livre */
void toggle_plant_in_deck(PlantType type);

/* Atribui planta a slot específico (livre ordering) */
void assign_plant_to_slot(PlantType type, int slot);

/* Remove planta de um slot do deck */
void remove_plant_from_deck_slot(int slot);

/* Cursor de navegação na grelha do inventário */
void move_selection_cursor(int dx, int dy);
int  get_selection_cursor(void);
bool is_cursor_on_button(void);

/* Consulta */
bool is_plant_in_deck(PlantType type);
int  get_deck_slot_for_type(PlantType type);    /* slot 0-5, ou -1 se não estiver */
int  get_deck_plant_at_slot(int slot);          /* PlantType ou -1 se vazio */
int  get_selected_deck_size(void);
const PlantType *get_available_plants(void);

/* Hit detection no ecrã de seleção */
bool get_plant_from_inventory_click(int x, int y, PlantType *out_type);
bool get_deck_slot_from_position(int x, int y, int *out_slot);
bool is_start_button_clicked(int x, int y);

/* Aplica o deck escolhido ao seed bank */
void confirm_plant_selection(void);

/* Flash vermelho no cursor — chamado quando o utilizador tenta um slot inválido */
void trigger_deck_full_flash(void);
void tick_deck_full_flash(void);   /* decrementar a cada tick de render */
bool is_deck_full_flash_active(void);

#endif /* _PLANT_SELECT_H_ */
