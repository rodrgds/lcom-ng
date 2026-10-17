#include "menus.h"

menus_t startMenus() {
  menus_t menus;

  menus.mainTittle0 = loadBitmap("/home/lcom/labs/proj/res/projTittle0.bmp");
  menus.mainTittle1 = loadBitmap("/home/lcom/labs/proj/res/projTittle1.bmp");

	menus.diffMenu = loadBitmap("/home/lcom/labs/proj/res/diffmenu.bmp");
	menus.difficulty = 2;
	menus.hardBox = loadBitmap("/home/lcom/labs/proj/res/hard.bmp");
	menus.mediumBox = loadBitmap("/home/lcom/labs/proj/res/medium.bmp");
	menus.easyBox = loadBitmap("/home/lcom/labs/proj/res/easy.bmp");

	menus.cursor = loadBitmap("/home/lcom/labs/proj/res/blue.bmp");
	menus.cursor_x = 512;
	menus.cursor_y = 384;

	menus.gameOverMenu = loadBitmap("/home/lcom/labs/proj/res/gameover.bmp");
	menus.pauseMenu = loadBitmap("/home/lcom/labs/proj/res/pause.bmp");

  return menus;
}

extern uint8_t scancode;
extern uint32_t time_counter;

/*
uint32_t irq_set;
uint8_t irq_set_timer;
*/

extern uint32_t irq_set;
extern uint8_t irq_set_timer;
extern uint32_t irq_set_mouse;

int mainTitle(menus_t* menus) {
  /* uint32_t bit_no;
	if (timer_subscribe_int(&irq_set_timer) != 0 || keyboard_subscribe_int(&bit_no) != 0) {
    return 1;
  }
  irq_set = bit_no;
	*/

  int ipc_status;
  message msg;
  uint8_t r;

  time_counter = 0;
  int i = 0;

    drawBitmap(menus->mainTittle0,0,0,getAuxBuffer());
		doubleBuffering();

    while (scancode != ENTER_MAKE) {
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_set_timer) { /* subscribed interrupt */
                      timer_int_handler();   /* process it */
                      if (time_counter % 60 == 0 && i % 2 == 0) {
                        resetSecondBuffer();
                        drawBitmap(menus->mainTittle1,0,0,getAuxBuffer());
                        doubleBuffering();
                        i++;
                      }
                      else if (time_counter % 60 == 0 && i % 2 != 0) {
                        resetSecondBuffer();
                        drawBitmap(menus->mainTittle0,0,0,getAuxBuffer());
                        doubleBuffering();
                        i++;
                      }
                    }
                    if (msg.m_notify.interrupts & irq_set) {
                      kbc_ih();
                    }
                    break;
              default:
                break; /* no other notifications expected: do nothing */
            }
        } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }

	/*
  if (timer_unsubscribe_int() != 0 || keyboard_unsubscribe_int() != 0) {
    return 1;
  }
	*/
  return 0;
}

extern uint16_t byte_packet[3];
extern uint32_t byte;
extern struct packet prPacket;

int diffMenu(menus_t* menus) {
	int ipc_status;
  message msg;
  uint16_t r;
  // uint32_t mouseID = 12;

	bool is_two_byte = false;   
  uint8_t scancode_bytes[2];

	/*
  if (mouse_issue_cmd(ENABLE) != 0) {
    printf("error::mouse_issue_cmd(ENABLE)\n");
    return 1;
  }

	uint32_t bit_no;
	if (keyboard_subscribe_int(&bit_no) != 0 || mouse_subscribe_int(&mouseID) != 0) {
  	return 1;
  }
  irq_set = bit_no;

  uint32_t irq_set_mouse = mouseID;
  */

	uint16_t index = 0;

	resetSecondBuffer();

	drawBitmap(menus->diffMenu,0,0,getAuxBuffer());
	if (menus->difficulty == 1) {
		drawBitmap(menus->easyBox,0,0,getAuxBuffer());
	}
	else if (menus->difficulty == 2) {
		drawBitmap(menus->mediumBox,0,0,getAuxBuffer());
	}
	else if (menus->difficulty == 3) {
		drawBitmap(menus->hardBox,0,0,getAuxBuffer());
	}
	doubleBuffering();


	while (true) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set_mouse) { /* subscribed interrupt */
            mouse_ih();
						if (index == 0 && (byte & CHECK_BYTE1)) {
              byte_packet[0] = byte;
              index += 1;
            }
          	else if (index == 1) {
              byte_packet[1] = byte;
              index += 1;
            }
            else if (index == 2) {
              byte_packet[2] = byte;
              process_packet();
        
							if (prPacket.lb) {
								if (menus->cursor_y >= 360 && menus->cursor_y <= 410) {
									if (menus->cursor_x >= 640 && menus->cursor_x <= 695) {
										menus->difficulty += 1;
										if (menus->difficulty == 4) {
											menus->difficulty = 1;
										}
									}
									if (menus->cursor_x >= 330 && menus->cursor_x <= 385) {
										menus->difficulty -= 1;
										if (menus->difficulty == 0) {
											menus->difficulty = 3;
										}
									}
								}
								if (menus->cursor_y >= 648 && menus->cursor_y <= 715) {
									if (menus->cursor_x >= 445 && menus->cursor_x <= 580)
										/*
										if (mouse_unsubscribe_int() != 0 || keyboard_unsubscribe_int() != 0) {
											return 2;
										}
										if (mouse_issue_cmd(DISABLE) != 0) {
											printf("error::mouse_issue_cmd(DISABLE)\n");
											return 3;
										}
										*/
										return 1; // exit condition	(exit game)
								}
								if (menus->cursor_y >= 518 && menus->cursor_y <= 627) {
									if (menus->cursor_x >= 410 && menus->cursor_x <= 615) {
										/*
										if (mouse_unsubscribe_int() != 0 || keyboard_unsubscribe_int() != 0) {
											return 2;
										}
										if (mouse_issue_cmd(DISABLE) != 0) {
											printf("error::mouse_issue_cmd(DISABLE)\n");
											return 3;
										}
										*/
										return 0; // start game with desired diff (menus->difficulty)
									}
								}
							}
				
							resetSecondBuffer();
							drawBitmap(menus->diffMenu,0,0,getAuxBuffer());
							if (menus->difficulty == 1) {
								drawBitmap(menus->easyBox,0,0,getAuxBuffer());
							}
							else if (menus->difficulty == 2) {
								drawBitmap(menus->mediumBox,0,0,getAuxBuffer());
							}
							else if (menus->difficulty == 3) {
								drawBitmap(menus->hardBox,0,0,getAuxBuffer());
							}
							menus->cursor_x += prPacket.delta_x;
							menus->cursor_y -= prPacket.delta_y;

							drawBitmap(menus->cursor,menus->cursor_x,menus->cursor_y,getAuxBuffer());

							doubleBuffering();

              index = 0;
            }
          }
					if (msg.m_notify.interrupts & irq_set) {
						kbc_ih();
						// only reads one byte at a time
						if (scancode == TWO_BYTE_CODE) {
						is_two_byte = true;
						scancode_bytes[0] = scancode;
						} else {
							if (is_two_byte) {
								is_two_byte = false;
								scancode_bytes[1] = scancode;
							} else {
								scancode_bytes[0] = scancode;
							}
						}
						if (scancode_bytes[1] == LEFT_MAKE) {
							menus->difficulty -= 1;
							if (menus->difficulty == 0) {
								menus->difficulty = 3;
							}

							resetSecondBuffer();
							drawBitmap(menus->diffMenu,0,0,getAuxBuffer());
							if (menus->difficulty == 1) {
								drawBitmap(menus->easyBox,0,0,getAuxBuffer());
							}
							else if (menus->difficulty == 2) {
								drawBitmap(menus->mediumBox,0,0,getAuxBuffer());
							}
							else if (menus->difficulty == 3) {
								drawBitmap(menus->hardBox,0,0,getAuxBuffer());
							}
							doubleBuffering();
						}
						if (scancode_bytes[1] == RIGHT_MAKE) {
							menus->difficulty += 1;
							if (menus->difficulty == 4) {
								menus->difficulty = 1;
							}

							resetSecondBuffer();
							drawBitmap(menus->diffMenu,0,0,getAuxBuffer());
							if (menus->difficulty == 1) {
								drawBitmap(menus->easyBox,0,0,getAuxBuffer());
							}
							else if (menus->difficulty == 2) {
								drawBitmap(menus->mediumBox,0,0,getAuxBuffer());
							}
							else if (menus->difficulty == 3) {
								drawBitmap(menus->hardBox,0,0,getAuxBuffer());
							}
							doubleBuffering();
						}
						if (scancode == ESC_MAKE) {
							/*
							if (mouse_unsubscribe_int() != 0) {
								printf("error::mouse_unsubscribe_int()\n");
								return 2;
							} 
							if (keyboard_unsubscribe_int() != 0) {
    						printf("error::keyboard_unsubscribe_int()\n");
								return 2;
  						}
							if (mouse_issue_cmd(DISABLE) != 0) {
    						printf("error::mouse_issue_cmd(DISABLE)\n");
      					return 3;
  						}
							*/
							return 1;
						}

						if (scancode == ENTER_MAKE) {
							/*
							if (mouse_unsubscribe_int() != 0) {
								printf("error::mouse_unsubscribe_int()\n");
								return 2;
							} 
							if (keyboard_unsubscribe_int() != 0) {
    						printf("error::keyboard_unsubscribe_int()\n");
								return 2;
  						}
							if (mouse_issue_cmd(DISABLE) != 0) {
    						printf("error::mouse_issue_cmd(DISABLE)\n");
      					return 3;
  						}
							*/
							return 0;
						}
						scancode = 0;
						scancode_bytes[1] = 0;
					}
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
    }
  }

 	return 0;
}
