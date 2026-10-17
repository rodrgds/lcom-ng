#include <lcom/lcf.h>

#include "i8254.h"
#include "i8042.h"
#include "keyboard.h"
#include "mouse.h"
#include "video_gr.h"

#include "Game.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (get_video_graphic_option)(){

  if ((vg_init(0x14c)) == NULL) {
    
    free(vg_bg_buffer);
    
    free(vg_elem_buffer);
    
    free(vg_mouse_buffer);
    
    return 1;
  }    

  xpm_map_t a = (xpm_map_t) choose_mode_xpm; 
  if ((draw_xpm_(a, 0, 0)) != 0) return 1;

  uint8_t bit_no = 1; 
  uint32_t irq_set = BIT(bit_no);

  int r;

  int ipc_status;
  message msg;

  bool keep_byte = false, is_make;
  int n_bytes = 0;
  uint8_t bytes_to_read[3];

  kb_data = 0x00;

  if ((kbc_subscribe_int(&bit_no)) != 0) return 1;
  
  int ret_value = 0;
  bool waiting = true;

  while (waiting) { 
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:  				
          if (msg.m_notify.interrupts & irq_set) {
            found_err = 0; 
            data_read_to = 0;

            kbc_ih();

            if (data_read_to != 0) return 1;

            if (found_err != 0) return 1;

            bytes_to_read[n_bytes] = kb_data;

            if (keep_byte != 0){
              keep_byte = false;
              n_bytes = 0;
            }
              
            else {  

              is_make = !(kb_data & KBC_MAKE_CODE_CHECK);
              
              if(kb_data == KBC_2_BYTE){
                keep_byte = true;
                n_bytes++;
              }

              else {
                switch (kb_data) {
                  case KP_NUMBER_0_BR:  
                  case NUMBER_0_BR: {
                    ret_value = 0;
                    waiting = false;
                    break;
                  }

                  case KP_NUMBER_1_BR:
                  case NUMBER_1_BR: {
                    ret_value = -1;
                    waiting = false;
                    break;
                  }

                  case KP_NUMBER_2_BR:
                  case NUMBER_2_BR: {
                    ret_value = -2;
                    waiting = false;
                    break;
                  }

                  case KP_NUMBER_3_BR:
                  case NUMBER_3_BR: {
                    ret_value = -3;
                    waiting = false;
                    break;
                  }

                  default: break;
                }

                keep_byte = false;
                n_bytes = 0;
              }
            } 	
          }
          break;
        default: break;
      }
    }
    else {}
  }

  if ((kbc_unsubscribe_int()) != 0) return 1;

  if (my_vg_exit()) return 1;

  return ret_value;
}


int (proj_main_loop)(int argc, char **argv) {

  uint16_t v_mode = 0x14c;
  Video_Mode v_md_option = DIRECT_8_8_8_8;

  int opt = get_video_graphic_option(); // 0x105 - 1152x864, 0x115 - 800x600, 0x14C - 1152x864

  switch (opt) {
    case -1: {
      v_mode = 0x14c;
      v_md_option = DIRECT_8_8_8_8;
      break;
    }

    case -2: {
      v_mode = 0x115;
      v_md_option = DIRECT_8_8_8;
      break;
    }

    case -3: {
      v_mode = 0X105;
      v_md_option = INDEXED;
      break;
    }

    case 1: return 1;

    default: return 0;
  }


  struct Game game;

  uint8_t mouse_bit_no = 3, timer_bit_no = 0, kb_bit_no = 1;
  uint32_t mouse_irq_set = BIT(mouse_bit_no), timer_irq_set = BIT(timer_bit_no), kb_irq_set = BIT(kb_bit_no);
  
  if ((mouse_issue_cmd(MOUSE_ENABLE_DATA_REPORT)) != 0) return 1;
  
  if ((timer_subscribe_int(&timer_bit_no)) != 0 || (mouse_subscribe_int(&mouse_bit_no)) != 0 || (kbc_subscribe_int(&kb_bit_no)) != 0) return 1;  

  int r;

  int ipc_status;
  message msg;

  int n_byte = 2;
  counter_var = 0;

  bool keep_byte = false, is_make;
  int n_bytes = 0;
  uint8_t bytes_to_read[3];

  kb_data = 0x00;

  if ((vg_init(v_mode)) == NULL) {
    
    free(vg_bg_buffer);
    
    free(vg_elem_buffer);
    
    free(vg_mouse_buffer);
    
    return 1;
  }

  int delta_x = 0, delta_y = 0;
  bool left_click = false, kb_press = false;

  init_game(&game, h_res, v_res, v_md_option);

  while(game.g_state != CLOSE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: {
          if(msg.m_notify.interrupts & timer_irq_set){ // TIMER

            timer_int_handler();

            int m_tmp[2], kb_tmp[4];

            m_tmp[0] = delta_x;
            m_tmp[1] = delta_y;

            left_click = pp.lb;

            kb_tmp[0] = bytes_to_read[0];
            kb_tmp[1] = bytes_to_read[1];
            kb_tmp[2] = bytes_to_read[2];

            if ((process_data(&game, m_tmp, left_click, kb_tmp, kb_press)) != 0){
              my_vg_exit();
              return 1;
            }

            delta_x = 0;
            delta_y = 0;
            kb_press = false;

            if (counter_var % 2 == 0){  
              if (draw_game(&game) != 0) {
                my_vg_exit();
                return 1;
              }
            }
          }

          if (msg.m_notify.interrupts & kb_irq_set) { // KEYBOARD
            found_err = 0; 
            data_read_to = 0;

            kbc_ih();

            if (found_err != 0){
              my_vg_exit();
              return 1;
            }

            if (data_read_to != 0){
              my_vg_exit();
              return 1;
            }

            bytes_to_read[n_bytes] = kb_data;
            kb_press = true;

            if (keep_byte != 0){
              keep_byte = false;
              n_bytes = 0;
            }

            else {
              is_make = !(kb_data & KBC_MAKE_CODE_CHECK);
                
              if(kb_data == KBC_2_BYTE){
                keep_byte = true;
                n_bytes++;
              }

              else if (n_byte == 2){} 

              else {
                keep_byte = false;
                n_bytes = 0;
              }
            }
          }

          if (msg.m_notify.interrupts & mouse_irq_set) { // MOUSE
            found_err = 0;
            data_read_to = 0;
            
            mouse_ih();

            if (found_err != 0){
              my_vg_exit();
              return 1;
            }

            if (data_read_to != 0){
              my_vg_exit();
              return 1;
            }

            if (n_byte == 2){
              if (m_data & MOUSE_CK_FST_BYTE){
                
                n_byte = 0;
                pp.bytes[n_byte] = m_data;

                pp.rb = m_data & MOUSE_R_BUTTON;
                pp.mb = m_data & MOUSE_M_BUTTON;
                pp.lb = m_data & MOUSE_L_BUTTON;
                pp.x_ov = m_data & MOUSE_X_OVFL;
                pp.y_ov = m_data & MOUSE_Y_OVFL;

                pp.delta_x = (m_data & MOUSE_MSB_X_DELTA) ? (0 | 0xff00) : 0;
                pp.delta_y = (m_data & MOUSE_MSB_Y_DELTA) ? (0 | 0xff00) : 0;
              }
            }
            else if(n_byte < 2){ 
              pp.bytes[++n_byte] = m_data;

              if (n_byte == 1){
                pp.delta_x |= m_data;
                delta_x += pp.delta_x;
              }

              else if (n_byte == 2){
                pp.delta_y |= m_data;
                delta_y += pp.delta_y;
              } 
            }
          }
          break;
        }
        default: break;
      }
    }
    else {}
  }
  
  if (my_vg_exit()) return 1;

  if ((kbc_unsubscribe_int()) != 0 || (mouse_unsubscribe_int()) != 0 || (timer_unsubscribe_int()) != 0) return 1;

  if ((mouse_issue_cmd(MOUSE_DISABLE_DATA_REPORT)) != 0) return 1;
  
  return 0;
}
