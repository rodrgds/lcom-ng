#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

#define FPS 60 

// Player data
#include "player_data/database.h"

// Device controllers
#include "device_controllers/video/video.h"
#include "device_controllers/kbc/keyboard.h"
#include "device_controllers/kbc/kbc.h"
#include "device_controllers/kbc/mouse.h"
#include "device_controllers/timer/timer.h"
#include "device_controllers/rtc/rtc.h"
#include "device_controllers/uart/uart.h"

// Models
#include "model/cursor.h"
#include "model/button.h"
#include "model/bar.h"
#include "model/stateModels/mainMenu.h"
#include "model/stateModels/mainRoom.h"
#include "model/stateModels/minigameMenu.h"
#include "model/stateModels/nameMinigotchi.h"
#include "model/stateModels/tictactoe.h"
#include "model/stateModels/rockPaperScissors.h"

// Viewers
#include "viewer/guiDrawer.h"
#include "viewer/menus/mainMenuViewer.h"
#include "viewer/menus/mainRoomViewer.h"
#include "viewer/menus/minigameMenuViewer.h"
#include "viewer/menus/nameMinigotchiViewer.h"
#include "viewer/minigames/ticTacToeViewer.h"
#include "viewer/minigames/rockPaperScissorsViewer.h"

// Controllers
#include "controller/menus/mainMenuController.h"
#include "controller/menus/mainRoomController.h"
#include "controller/menus/minigameMenuController.h"
#include "controller/menus/nameMinigotchiController.h"
#include "controller/minigames/ticTacToeController.h"
#include "controller/minigames/rockPaperScissorsController.h"


// Database
static Database* database;

typedef enum {MAIN_MENU, MAIN_ROOM, NAME_MINIGOTCHI, MINIGAMES_WINDOW, MINIGAME_1, MINIGAME_2, EXIT} state_t;
typedef enum {DAY, EVENING, NIGHT} timeOfDay_t;
static state_t game_state = MAIN_MENU;
static bool newGame = false;
static timeOfDay_t dateTime;

/**
 * @brief Main function that will start the LCF 
 */
int main(int argc, char *argv[]) {
	lcf_set_language("EN-US");
	if (lcf_start(argc, argv)) return 1;
	lcf_cleanup();
	return 0;
}

/**
 * @brief Setter for game state 
 */
void proj_set_state(state_t state){
	game_state = state;
}

/**
 * @brief Convert 2's complement to binary
 */
int16_t twosComplementToBinary(int16_t n) {
	int8_t sign = ((n & 0x0100) >> 8) ;
	if (sign){
		n = ~n + 1;
		n = n & 0x01FF;
		return -n;
	}
	return n;
}

/**
 * @brief Convert 2's complement to signed int (so that we change the cursor position)
 */
int twosComplementToSignedInt(unsigned char value) {
    if (value & 0x80) {
        return (int)((unsigned int)value | 0xFFFFFF00);
    } else {
        return (int)value;
    }
}


/**
 * @brief Project main function that starts the game
 * @details This function includes the game loop (as a state machine) and handles all the device controller interrupts
 */
int (proj_main_loop)(int argc, char **argv) {
	// Get mode information
	if (video_get_mode_information(0x14C) != 0){
		panic("Error in video_get_mode_information()\n");
		if (vg_exit() != 0) panic("Error in vg_exit()\n");
		return 1;
	}

	// Write to VRAM to display on the screen what is requested
	if (video_map_vram() != 0){
		panic("Error in video_write_to_vram()\n");
		if (vg_exit() != 0) panic("Error in vg_exit()\n");
		return 1;
	}

    // Configure video card for desired graphics mode
	if (video_set_graphics_mode(0x14C) != 0){
		panic("Error in video_set_graphics_mode()\n");
		if (vg_exit() != 0) panic("Error in vg_exit()\n");
		return 1;
	}

	setup_sprites();

	// Subscribe keyboard interrupts
    uint8_t kbd_bit_no = 0;
    if (subscribe_interrupts_kbd(&kbd_bit_no) != 0){
        return 1;
    }

	// Enable data reporting (mouse)
	if (send_byte_to_mouse(MOUSE_ENABLE_DATA_REPORTING) != 0) {
		return 1;
	}

	// Subscribe mouse interrupts
	uint8_t mouse_bit_no = 0;
	if (mouse_subscribe_int(&mouse_bit_no) != 0) {
    	return 1;
  	}

	uint8_t timer_bit_no = 0;
	if (timer_subscribe_int(&timer_bit_no) != 0) {
		return 1;
	}

	if (rtc_clear_flags() != 0) {
        return 1;
    }

	uint32_t rtc_bit_no = 0;
    if (rtc_subscribe_int(&rtc_bit_no) != 0) {
        return 1;
    }

	if (disable_pie_int() != 0) {
        return 1;
    }

	if (disable_uie_int() != 0) {
        return 1;
    }

	if (enable_aie_int() != 0) {
        return 1;
    }

	RTC_Config rtcConfig = rtc_get_config();

	if (rtcConfig.hours >= 0x08 && rtcConfig.hours < 0x18) { // Day time

		setRTCWindow(guiDrawer_get_day_window());
		dateTime = DAY;
		if (rtc_set_alarm(0,0x00,0x18) != 0) {
			return 1;
		}

	} else if (rtcConfig.hours >= 0x18 && rtcConfig.hours < 0x21) { // Evening time
		setRTCWindow(guiDrawer_get_evening_window());
		dateTime = EVENING;
		if (rtc_set_alarm(0,0x00,0x21) != 0) {
			return 1;
		}

	} else { // Night time

		setRTCWindow(guiDrawer_get_night_window());
		dateTime = NIGHT;
		if (rtc_set_alarm(0,0x00,0x08) != 0) {
			return 1;
		}

	}	

	// Subscribe and prepare uart interface.
	uint8_t uart_bit_no = 0;
	if (serial_subscribe_int(&uart_bit_no) != 0) {
		return 1;
	}

	if (serial_init() != 0) {
		return 1;
	}

	struct packet pp;
	uint8_t kbd_irq_set = BIT(kbd_bit_no);
	uint8_t mouse_irq_set = BIT(mouse_bit_no);
	uint8_t timer_irq_set = BIT(timer_bit_no);
	uint32_t rtc_irq_set = BIT(rtc_bit_no);
	bool endGame = false;
	bool canSave = true;

	int ipc_status, r;

	// Get database instance
	database = new_database();
	if (database_check_file_exists()){
		newGame = false;
		database_load_from_file(database);
	} else {
		newGame = true;
	}
	setDatabase(database);

	Cursor* cursor = new_cursor(100, 100);
	switchBackground(0);

	int minutes_passed = 0;
	int minutes_aux_cnt = 0;

	bool lockKeyboard = false;
    uint8_t scanCodes[2];
	
	scanCodes[0] = 0x00;
	scanCodes[1] = 0x00;	
	
    uint8_t counter = 0;
    uint8_t scanCodeCounter = 0;
	bool quitTTT = false;
	while(!endGame) {
		if (((getTimerCounter() - (sys_hz() / FPS)) == 0)){
			minutes_aux_cnt++;

			uint8_t scancode_first_byte = 0x00;
			uint8_t scancode_second_byte = 0x00;

			switch (game_state) {
				case NAME_MINIGOTCHI:
					nameMinigotchiController_step(scanCodes[0]);
					lockKeyboard = false;
					nameMinigotchiViewer_draw();

					setNameMinigotchiCursor(cursor);

					if (nameMinigotchiController_getButtonEvent() == CONTINUE_NAMEMINIGOTCHI && nameMinigotchiViewer_get_spriteVector()->size != 0) {

						char* minigotchiName = malloc(11*sizeof(char));
						minigotchiName = "\0\0\0\0\0\0\0\0\0\0";

						for (size_t i = 0; i < nameMinigotchiViewer_get_spriteVector()->size; i++) {
							char toBeConcatenated = ((char)(nameMinigotchiViewer_get_spriteVector()->data[i])) + 'A';
							minigotchiName[i] = toBeConcatenated;
						}
						database_set_minigotchiName(database, minigotchiName);
						

						switchBackground(1);
						game_state = MAIN_ROOM;
						newGame = false;
						nameMinigotchiController_delete_nameMinigotchi();
						break;
					} 

					if (nameMinigotchiController_getButtonEvent() == QUIT_NAMEMINIGOTCHI) {
						database_delete_file(database);
						nameMinigotchiController_delete_nameMinigotchi();
						canSave = false;
						endGame = true;
						break;
					}

					nameMinigotchiController_setButtonEvent(NOP_NAMEMINIGOTCHI);
					
					break;
				case MAIN_MENU:
					mainMenuController_step();
					mainMenuViewer_draw();
					
					setMainMenuCursor(cursor);

					if (mainMenuController_getButtonEvent() == START){ // Start the game
						mainMenuController_delete_mainMenu(); // Free the main menu
						if (newGame) {
							switchBackground(3);
							game_state = NAME_MINIGOTCHI;
							break;
						}
						switchBackground(1);
						game_state = MAIN_ROOM;	
					}

					if (mainMenuController_getButtonEvent() == QUIT){ // Quit the game
						mainMenuController_delete_mainMenu(); // Free the main menu
						canSave = false;
						endGame = true;
					}
					break;
				case MAIN_ROOM:
					mainRoomController_step();
					mainRoomViewer_draw();
					
					setMainRoomCursor(cursor);

					if (lockKeyboard) {
						scancode_first_byte = scanCodes[0];
						scancode_second_byte = scanCodes[1];
							
							if (scancode_first_byte == 0xe0 && scancode_second_byte == 0x4d) {
								mainRoomController_Hotbar_goRight();
							}
    						if (scancode_first_byte == 0xe0 && scancode_second_byte == 0x4b) {
								mainRoomController_Hotbar_goLeft();
							}
							if (scancode_first_byte == 0x1c) {
								mainRoomController_feed_minigotchi();
							}
						

						if (scanCodes[0] == E) {
							mainRoomController_toggleHotbar();
						}
						
						scanCodes[0] = 0x00;
						scanCodes[1] = 0x00;
						lockKeyboard = false;
					}

					// Open minigames 
					if (mainRoomController_getButtonEvent() == MINIGAMES_MAINROOM){ 
						mainRoomController_setButtonEvent(NOP_MAINROOM);
						switchBackground(2);
						game_state = MINIGAMES_WINDOW;
					}

					if (mainRoomController_getButtonEvent() == QUIT_MAINROOM){ // Quit the game
						endGame = true;	
					}
					break;
				case MINIGAMES_WINDOW: 
					minigameMenuController_step();
					minigameMenuViewer_draw();
					setMinigameMenuCursor(cursor);
					
					if (minigameMenuController_getButtonEvent() == QUIT_MINIGAMEMENU){ // Open minigames 
						minigameMenuController_setButtonEvent(NOP_MINIGAMEMENU);
						switchBackground(1);
						game_state = MAIN_ROOM;	
					}

					if (minigameMenuController_getButtonEvent() == MINIGAME1_MINIGAMEMENU){ // Tic tac toe
						minigameMenuController_setButtonEvent(NOP_MINIGAMEMENU);
						setup_ttt_sprites();
						ttt_set_canClick(); // To avoid clicking the buttons at first
						game_state = MINIGAME_1;
						switchBackground(5);
					}

					if (minigameMenuController_getButtonEvent() == MINIGAME2_MINIGAMEMENU){ // Rock paper scissors
						minigameMenuController_setButtonEvent(NOP_MINIGAMEMENU);
						switchBackground(4);
						rps_set_canClick(); // To avoid clicking the buttons at first
						game_state = MINIGAME_2;
					}
					
					break;
				case MINIGAME_1:

					if (quitTTT) {
						ticTacToeController_setButtonEvent(NOP_TTT);
						reset_ticTacToe(ticTacToeController_get_saved_ttt());
						reset_ticTacToe(ticTacToeViewer_get_saved_ttt());

						if (!isEmpty(serial_get_receive_queue())) {
							dequeue(serial_get_receive_queue());
						} else {
							switchBackground(1);
							game_state = MAIN_ROOM;
							quitTTT = false;
						}
						
					} else {
						ticTacToeController_step();
						

						if (ticTacToeController_getButtonEvent() == QUIT_TTT){ // Quit TicTacToe
							quitTTT = true;
						}

						ticTacToeViewer_draw();
						setTicTacToeCursor(cursor);
					}					
					break;
				case MINIGAME_2:
					rockPaperScissorsController_step();
					if (rockPaperScissorsController_getButtonEvent() != QUIT_RPS){
						rockPaperScissorsViewer_draw();
						setRockPaperScissorsCursor(cursor);
					}
					

					if (rockPaperScissorsController_getButtonEvent() == QUIT_RPS){ // Open minigames
						rockPaperScissorsController_setButtonEvent(NOP_RPS);
						switchBackground(1);
						game_state = MAIN_ROOM;
					}

					break;
				case EXIT:
					
					break;
				default:
					return 1;
					break;
			}
			if (minutes_aux_cnt == (60 * 60)){
				int current_happiness = database_get_happiness(getDatabase());
				int current_hunger = database_get_hunger(getDatabase());
				
				// Check happiness
				if (current_happiness <= 0) {
					database_set_happiness(database, 0); // Minigotchi is very unhappy
				} else {
					database_set_happiness(getDatabase(), current_happiness - 100);  // Minigotchi loses 100 happiness every minute
				}

				// Check hunger
				if (current_hunger <= 0) {
					database_set_hunger(database, 0); // Minigotchi is very hungry
				} else {
					database_set_hunger(getDatabase(), current_hunger - 100);  // Minigotchi loses 100 happiness every minute
				}
				
				minutes_aux_cnt = 0;
				minutes_passed = 0;
			}
			setTimerCounter(0);
		}

		message msg;
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)){ 
		switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if(msg.m_notify.interrupts & BIT(uart_bit_no)) {
                 	serial_interrupt_handler();
                }

				if (msg.m_notify.interrupts & rtc_irq_set) {
					rtc_ih();
					if (rtc_get_interrupt_was_periodic()) {
						rtc_set_interrupt_periodic_processed();
					}
					if (rtc_get_interrupt_was_update()) {
						rtc_set_interrupt_update_processed();
					}
					if (rtc_get_interrupt_was_alarm()) {
						rtc_set_interrupt_alarm_processed();

						switch (dateTime) {
							case DAY:
								dateTime = EVENING;
								if (rtc_set_alarm(0,0x00,0x21) != 0) {
									return 1;
								}
								setRTCWindow(guiDrawer_get_evening_window());
								break;
							case EVENING:
								dateTime = NIGHT;
								if (rtc_set_alarm(0,0x00,0x08) != 0) {
									return 1;
								}
								setRTCWindow(guiDrawer_get_night_window());
								break;
							case NIGHT:
								dateTime = DAY;
								if (rtc_set_alarm(0,0x00,0x18) != 0) {
									return 1;
								}
								setRTCWindow(guiDrawer_get_day_window());
								break;
							default:
								break;
						}


					}
					
				}

				if ((msg.m_notify.interrupts & timer_irq_set) != 0) {
					timer_int_handler();
				}

				if ((msg.m_notify.interrupts & mouse_irq_set )!= 0) {
					mouse_ih();

					uint8_t byte = mouse_get_readByte();

					if (byte == 0x00 && mouse_get_error()){
						mouse_set_error(false);
						continue;
					}

					switch (counter){
					case 0:
						pp.bytes[counter] = byte;
						pp.lb = (byte & 0x01);
						pp.rb = ((byte & 0x02) != 0);
						pp.mb = ((byte & 0x04) != 0);
						pp.delta_x = (byte & 0x10) << 4;
						pp.delta_y = (byte & 0x20) << 3;
						pp.x_ov = ((byte & 0x40) != 0);
						pp.y_ov = ((byte & 0x80) != 0);
						break;
					case 1:
						pp.bytes[counter] = byte;
						pp.delta_x = pp.delta_x | byte;
						pp.delta_x = twosComplementToBinary(pp.delta_x);
						break;
					case 2:
						pp.bytes[counter] = byte;
						pp.delta_y = pp.delta_y | byte;
						pp.delta_y = twosComplementToBinary(pp.delta_y);
						break;
					default:
						break;
					}

					counter++;

					if (counter >= 3){
						counter = 0;
    					int delta_x = pp.delta_x;
						int delta_y = pp.delta_y;

						if (true && cursor != NULL) {
							set_buttonClicked(pp.lb, cursor);
							if (!pp.x_ov) cursor_set_x(cursor, cursor_get_x(cursor) + delta_x);
							if (!pp.y_ov) cursor_set_y(cursor, cursor_get_y(cursor) - delta_y);	
						}

					}

				}

				if (msg.m_notify.interrupts & kbd_irq_set) {
                        kbc_ih();
                        
                        uint8_t scanCode = getScanCode(); // Fetch the scancode.

						if (scanCode & 0x80) {
							nameMinigotchi_setKeyReleased();
						}
                        
                        switch (scanCodeCounter){
                        case 0:
                            if (!lockKeyboard) scanCodes[scanCodeCounter] = scanCode;

                            if (scanCode == 0xe0){
                            	scanCodeCounter++;
                            } else {
								lockKeyboard = true;
                            	scanCodeCounter = 0;
                            }

                            break;
                        case 1:
                            if (!lockKeyboard) scanCodes[scanCodeCounter] = scanCode;
							lockKeyboard = true;
                            scanCodeCounter = 0;
                            break;
                        default:
                            return -1;
                            break;
                        }
                    }

				break;
			default:
				break;
			}
		} 

	}

	if (canSave)
		database_save_to_file(database);

	if (!canSave && newGame)
		database_delete_file(database);

	delete_cursor(cursor);

	// Unsubscribe uart interrupts
	if (serial_unsubscribe_int() != 0) {
		return 1;
	}

	serial_exit();

	// Unsubscribe rtc interrupts.
	if (disable_aie_int() != 0) {
        return 1;
    }

    if (rtc_clear_flags() != 0) {
        return 1;
    }

    if (rtc_unsubscribe_int() != 0) {
        return 1;
    }

	// Unsubscribe timer's interrupts.
	if (timer_unsubscribe_int() != 0) {
		return 1;
	}

	// Unsubscribe keyboard interrupts.
	if (unsubscribe_interrupts_kbd() != 0){
        return 1;
    }

	// Unsubscribe mouse interrupts.
	if (mouse_unsubscribe_int() != 0) {
    	return 1;
  	}

	// Disable data reporting (mouse).
	if (send_byte_to_mouse(MOUSE_DISABLE_DATA_REPORTING) != 0){
		return 1;
	}

    // Return to text mode.
    if (vg_exit() != 0){
		printf("Error in vg_exit()\n");
		return 1;
	}

    return 0;
}


