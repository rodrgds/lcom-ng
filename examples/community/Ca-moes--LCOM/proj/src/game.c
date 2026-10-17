#include <lcom/lcf.h>

#include "game.h"
#include "highscores.h"

extern uint8_t SCANCODE[2];
extern bool SCANCODE_2B; // true if it is a 2 byte scancode
extern uint32_t TIMER_COUNTER;
extern int MOUSE_HOOK_ID;
extern uint8_t PACKET;
extern int PACKET_BYTE_COUNTER;

extern letter * abc , *errorMessage;
extern number * numbers;
extern letter ** sentences;
extern char * greenChar,* redChar,* blackChar;

int gameStart(){
  enum xpm_image_type type= XPM_5_6_5;

  loadLetters();  // passa todas as letras para abc[]
  loadSentences(); // makes an array of letters out of an array of strings(chars)

  game = (Game*)malloc(sizeof(Game));

  game->state.mode = MENU;
  game->state.start = true;
  game->state.drawGame = false;

  //define estado inicial das variáveis associadas ao jogo
  game->moveCar = 0; //marca a posição onde desenha o carro
  game->oneMove = 0; //define tamanho de um movimento do carro

  game->keyState = NOT_VALID;
  game->nLet = 0;
  game->numberOfLetters = 0;
  game->capital = false;
  game->CapsMakeCount = 0;

  game->completed = false;
  game->errorState =false;
  game->errors = 0;

  //highscores
  game->NUMBEROFSCORES = 0;
  game->highscores = (highScore*) malloc(sizeof(highScore) * 5);
  game->date = (realTime*)malloc(sizeof(realTime));
  game->highscores[0].name.nameAr = (char*) malloc(sizeof(char)*(10 + 1));
  game->highscores[1].name.nameAr = (char*) malloc(sizeof(char)*(10 + 1));
  game->highscores[2].name.nameAr = (char*) malloc(sizeof(char)*(10 + 1));
  game->highscores[3].name.nameAr = (char*) malloc(sizeof(char)*(10 + 1));
  game->highscores[4].name.nameAr = (char*) malloc(sizeof(char)*(10 + 1));

  game->highscores[0].name.nameXpm = (letter*) malloc(sizeof(letter)*10);
  game->highscores[1].name.nameXpm = (letter*) malloc(sizeof(letter)*10);
  game->highscores[2].name.nameXpm = (letter*) malloc(sizeof(letter)*10);
  game->highscores[3].name.nameXpm = (letter*) malloc(sizeof(letter)*10);
  game->highscores[4].name.nameXpm = (letter*) malloc(sizeof(letter)*10);
  
  get_highscores(game);

  //mouse
  game->mc.mouseX=get_hres()/2;
  game->mc.mouseY=get_vres()/2;
  game->mc.lbPressed = false;

  //player's name
  game->name = (name*)malloc(sizeof(name));
  game->name->nameAr = (char*) malloc(sizeof(char)*(10 + 1));
  game->name->nameXpm = (letter*) malloc(sizeof(letter)*10);
  game->name->nameSize = 0;

  //set colors in rgb565
  greenChar = (char*)malloc(3*sizeof(char));
  uint32_t greenCopy = 0x5D00;
  memcpy(greenChar,&greenCopy,3*sizeof(char));
  redChar = (char*)malloc(3*sizeof(char));
  uint32_t redCopy = 0xB800;
  memcpy(redChar,&redCopy,3*sizeof(char));
  blackChar = (char*)malloc(3*sizeof(char));
  uint32_t blackCopy = 0x0000;
  memcpy(blackChar,&blackCopy,3*sizeof(char));

  //load xpm's
  game->road_sprite = xpm_load(road2,type,&game->roadImg);
  game->menu_sprite = xpm_load(menu,type,&game->menuImg);
  game->button1_sprite = xpm_load(button_unpressed,type,&game->button1Img);
  game->button2_sprite = xpm_load(button_unpressed2,type,&game->button2Img);
  game->button1Pressed_sprite = xpm_load(button_pressed,type,&game->button1PressedImg);
  game->button2Pressed_sprite = xpm_load(button_pressed2,type,&game->button2PressedImg);
  game->car_sprite = xpm_load(car,type,&game->carImg);
  game->field_sprite = xpm_load(text_field,type,&game->fieldImg);
  game->error_sprite = xpm_load(error,type,&game->errorImg);
  game->mouse_sprite = xpm_load(mouse,type,&game->mouseImg);
  game->results_sprite = xpm_load(results,type,&game->resultsImg);
  game->menu_button_sprite = xpm_load(menu_button,type,&game->menu_buttonImg);
  game->menu_button_pressed_sprite = xpm_load(menu_button_pressed,type,&game->menu_button_pressedImg);
  game->redlight_sprite = xpm_load(traffic_red,type,&game->redlightImg);
  game->yellowlight_sprite = xpm_load(traffic_yellow,type,&game->yellowlightImg);
  game->greenlight_sprite = xpm_load(traffic_green,type,&game->greenlightImg);
  game->highscores_sprite = xpm_load(highscores, type, &game->highscoresImg);
  game->highscores_button_pressed_sprite = xpm_load(highscores_button_pressed, type, &game->highscores_button_pressedImg);
  game->highscores_button_unpressed_sprite = xpm_load(highscores_button_unpressed, type, &game->highscores_button_unpressedImg);
  game->saveScores_sprite = xpm_load(saveScore,type,&game->saveScoresImg);
  game->saveScore_button_sprite = xpm_load(save_score_button,type,&game->saveScore_buttonImg);
  game->saveScore_button_unpressed_sprite = xpm_load(save_score_button_unpressed,type,&game->saveScore_button_unpressedImg);  

  //set graphics mode
  if (graphics_mode(0x117) != 0) return 1;

  if(gameStateHandler() != 0) return 1;

  free(greenChar);
  free(redChar);
  free(blackChar);
  free(game->highscores);
  free(game->date);
  free(game->name);
  free_sprites();
  free(game);

  return 0;
}

int gameStateHandler(){

  int ipc_status;
  uint8_t bit_number;
  message msg;
  int r;
  uint32_t irq_set_kbc,irq_set_timer,irq_set_mouse;
  struct packet p;

  bool timedResults=false;
  bool sec3counter = true;
  bool timerreset = false; // true para fazer a contagem;

  letter * sentence= NULL; // n = numero de letras da frase
  letter * input = NULL; // aloca espaço para o array de input - deixa espaço para 5 erros no fim
  

  //subscribe the timer
  if(timer_subscribe_int(&bit_number) != 0)
      return 1;
  irq_set_timer = BIT(bit_number);

  //enable stream mode and data reporting by mouse in stream mode
  if(mouse_enable_data_rep() != 0)
    return 1;

  //subscribe the keyboard
  if(kbc_subscribe_int(&bit_number) != 0)
      return 1;
  irq_set_kbc = BIT(bit_number);
  
  //subscribe the mouse
  if(mouse_subscribe_int(&bit_number) != 0)
      return 1; 
  irq_set_mouse = BIT(bit_number);

  unsigned int index=0; // until this index it was written correctly

  while(game->state.mode != EXIT){ //enquanto não se sai do jogo
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    } if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          //mouse
          if(msg.m_notify.interrupts & irq_set_mouse){
            mouse_ih();
            switch (game->state.mode){
            case MENU:
              mouse_handler(&p);
              break;
            case RESULTS:
              mouse_handler(&p);
              break;
            case HIGHSCORES:
              mouse_handler(&p);  
              break;
            case SAVE_SCORE:
              mouse_handler(&p);
              break;
            default:
              break;
            }
          }	
          //keyboard
          if (msg.m_notify.interrupts & irq_set_kbc){
            kbc_ih();
            switch (game->state.mode){
            case MENU:
              //exit game if Esc is pressed
              if(SCANCODE[0] == MC_ESC){
                game->state.mode = EXIT;
              }
              break;
            case CLASSIC://classic mode
              if (sec3counter){
                //go back to menu if Esc is pressed and the countdown has finished
                if(SCANCODE[0] == MC_ESC){
                  game->state.mode = MENU;
                  game->state.start = true;
                }
              }
              //if sentence not completed and not the initial frame, handle KBC in classic mode
              if(!game->completed && !game->state.start){
                kbc_handle(&sentence,&input,&index);
              }
              break;
            case TIMED:
              if (sec3counter){
                //go back to menu if Esc is pressed and the countdown has finished
                if(SCANCODE[0] == MC_ESC){
                  game->state.mode = MENU;
                  game->state.start = true;
                }
              }
              //if sentence not completed and not the initial frame, handle KBC in classic mode
              if(!game->completed && !game->state.start && !timedResults){
                kbc_handle(&sentence,&input,&index);
              }
              break;
            case RESULTS:
              // verifica ESC para sair 
              if(SCANCODE[0] == MC_ESC){
                game->state.mode = SAVE_SCORE;
                game->state.start = true;
              }
              break;
            case HIGHSCORES:
              // verifica ESC para sair 
              if(SCANCODE[0] == MC_ESC){
                game->state.mode = MENU;
                game->state.start = true;
              }
              break;
            //the esc is only temporary
            case SAVE_SCORE:
              //handle KBC in save score
              playerName(SCANCODE);
              break;
            default:
              break;
            }
          }
          
          //timer
          if(msg.m_notify.interrupts & irq_set_timer){
            timer_int_handler();
            switch (game->state.mode){
            case MENU:
              menu_mode(&timedResults,&sec3counter,&timerreset);
              break;
            case CLASSIC: //classic mode
              classic_mode(&sentence, &input, &index, &sec3counter,&timerreset);
              break;
            case TIMED:
              timed_mode(&sentence, &input, &index, &sec3counter, &timerreset,&timedResults);
              break;
            case RESULTS:
              resultScreen(game->result.timeSpentSec,game->result.timeSpentCentSec,game->result.speed);
              back_to_menu();
              break;
            case HIGHSCORES:
              highScoresScreen();
              back_to_menu();
              break;
            case SAVE_SCORE:
              save_score_screen();
              back_to_menu();
              break;
            default:
              break;
            }
          }
        break;  // break do case HARDWARE
      default:
        break;  // break do defaut de switch (_ENDPOINT_P(msg.m_source))
      }
    }
  }

  set_highscores(game);
  
  destroySentencesAndLetters();
  free(input);
  free_double_buffer();
  
  if(vg_exit()!= 0) return 1;

  if(mouse_unsubscribe_int() != 0) return 1;
  if(kbc_unsubscribe_int() != 0) return 1;
  if (mouse_disable_data_rep() != 0) return 1;
  if(timer_unsubscribe_int() != 0) return 1;

  return 0;
  
}

void counterStart(){
  draw_xpm_image(game->road_sprite,0,0,game->roadImg,get_double_buffer());
  if(TIMER_COUNTER > 0 && TIMER_COUNTER <=60)
    draw_xpm_image(game->redlight_sprite,437,132,game->redlightImg,get_double_buffer());
  if(TIMER_COUNTER > 60 && TIMER_COUNTER <= 120)
    draw_xpm_image(game->yellowlight_sprite,437,132,game->yellowlightImg,get_double_buffer());
  if(TIMER_COUNTER > 120 && TIMER_COUNTER < 160)
    draw_xpm_image(game->greenlight_sprite,437,132,game->greenlightImg,get_double_buffer());
  double_buffering();  
}

void menu_mode(bool * timedResults, bool * sec3counter, bool * timerreset){
  if(game->state.start){
    *game->date = rtc_read_date();

    /*draw menu components*/
    game->mc.lbPressed =false;
    game->mc.mouseX = get_hres()/2;
    game->mc.mouseY = get_vres()/2;
    drawMainMenu(game->mc.mouseX,game->mc.mouseY);
    game->state.start = false;
  }
  else{
    drawMainMenu();
    if(game->mc.mouseX >= 283 && game->mc.mouseX <= 479 && game->mc.mouseY>=400 && game->mc.mouseY<= 396+game->button1Img.height-23 && game->mc.lbPressed){
      game->state.mode = CLASSIC;
      game->keyState = NOT_VALID;
      game->state.drawGame = false;
      game->state.start = true;
      *sec3counter = true;
      *timerreset = false;
    }
    else if(game->mc.mouseX >= 550 && game->mc.mouseX <= 746 && game->mc.mouseY>=400 && game->mc.mouseY<= 396+game->button1Img.height-23 && game->mc.lbPressed){
      game->state.mode = TIMED;
      game->keyState = NOT_VALID;
      game->state.drawGame = false;
      game->state.start = true;
      TIMER_COUNTER = 0;
      *timedResults=false;
      game->nLet = 0;
      *sec3counter = true;
      *timerreset = false;
    }
    else if((game->mc.mouseX >= (int)(get_hres()-game->highscores_button_unpressedImg.width-14) && game->mc.mouseX <= (int)(get_hres()-25) && game->mc.mouseY>=(int)(get_vres()-game->highscores_button_pressedImg.height-13) && game->mc.mouseY<=(int)(get_vres()-10)) && game->mc.lbPressed && game->NUMBEROFSCORES > 0){
      game->state.mode = HIGHSCORES;
      game->state.start = true;
      game->mc.lbPressed = false;
    }
  }
}

void drawMainMenu(){
  draw_xpm_image(game->menu_sprite,0,0,game->menuImg,get_double_buffer());
  if(game->mc.mouseX >= 283 && game->mc.mouseX <= 479 && game->mc.mouseY>=400 && game->mc.mouseY<= 396+game->button1Img.height-23){
    draw_xpm_image(game->button1Pressed_sprite,225,396,game->button1PressedImg,get_double_buffer());
    draw_xpm_image(game->button2_sprite,480,396,game->button2Img,get_double_buffer());

    if(game->NUMBEROFSCORES > 0){
       draw_xpm_image(game->highscores_button_unpressed_sprite,get_hres()-game->highscores_button_unpressedImg.width-10,get_vres()-game->highscores_button_pressedImg.height-10,game->highscores_button_unpressedImg,get_double_buffer());
    }
    
  }
  else if(game->mc.mouseX >= 540 && game->mc.mouseX <= 736 && game->mc.mouseY>=400 && game->mc.mouseY<= 396+game->button1Img.height-23){
    draw_xpm_image(game->button1_sprite,225,396,game->button1Img,get_double_buffer());
    draw_xpm_image(game->button2Pressed_sprite,480,396,game->button2PressedImg,get_double_buffer());

    if(game->NUMBEROFSCORES > 0){
      draw_xpm_image(game->highscores_button_unpressed_sprite,get_hres()-game->highscores_button_unpressedImg.width-10,get_vres()-game->highscores_button_pressedImg.height-10,game->highscores_button_unpressedImg,get_double_buffer());
    }
  }
  else if(game->mc.mouseX >= (int)(get_hres()-game->highscores_button_unpressedImg.width-14) && game->mc.mouseX <= (int)(get_hres()-25) && game->mc.mouseY>=(int)(get_vres()-game->highscores_button_pressedImg.height-13) && game->mc.mouseY<=(int)(get_vres()-10)){
    draw_xpm_image(game->button1_sprite,225,396,game->button1Img,get_double_buffer());
    draw_xpm_image(game->button2_sprite,480,396,game->button2Img,get_double_buffer());

    if(game->NUMBEROFSCORES > 0){
      draw_xpm_image(game->highscores_button_pressed_sprite,get_hres()-game->highscores_button_unpressedImg.width-10,get_vres()-game->highscores_button_pressedImg.height-10,game->highscores_button_pressedImg,get_double_buffer());
    }
  }
  else{
    draw_xpm_image(game->button1_sprite,225,396,game->button1Img,get_double_buffer());
    draw_xpm_image(game->button2_sprite,480,396,game->button2Img,get_double_buffer());

    if(game->NUMBEROFSCORES > 0){
      draw_xpm_image(game->highscores_button_unpressed_sprite,get_hres()-game->highscores_button_unpressedImg.width-10,get_vres()-game->highscores_button_pressedImg.height-10,game->highscores_button_unpressedImg,get_double_buffer());
    }
  }
  draw_date();
  draw_xpm_image(game->mouse_sprite,game->mc.mouseX,game->mc.mouseY,game->mouseImg,get_double_buffer());

  double_buffering();
}

void classic_mode(letter ** sentence, letter ** input, unsigned int * index, bool * sec3counter, bool * timerreset){

  if (*sec3counter){  
    if(!*timerreset){
      TIMER_COUNTER = 0;
      *timerreset = true;
    }
    counterStart();
    if(TIMER_COUNTER >= 160){
      *sec3counter = false;
      *timerreset = false;
    }  
    return;
  }
  
  if(game->state.start){
    *index = 0;
    game->errors = 0;
    game->nLet=0;
    game->errorState = false;
    game->completed = false;
    classicModeStart(sentence, input);
    game->state.start = false;
    TIMER_COUNTER = 0;
    return;
  }
  
  
  if(game->keyState == ESC){// se for Esc sai do loop e não recebe mais interrupções
    game->state.mode = MENU;
    game->state.start = true;
    *sec3counter = true;
    free(*input);
    free(*sentence);
    return;
  }
  else if(game->keyState == DOT && game->errors == 0 && game->errorState == false){
    sentenceCompleted();
    *game->date = rtc_read_date();
    game->mc.lbPressed =false;
    game->nLet = game->numberOfLetters;
    game->state.mode = RESULTS;
    game->state.start = true;
    *sec3counter = true;

    free(*input);
    free(*sentence);
    return;
  }
  else if(game->state.drawGame){
    classicModeDraw(*sentence, *input, *index);
    game->state.drawGame = false;
    if(game->completed){
      *sec3counter = true;
      return;
    } 
  }
}

void classicModeStart(letter ** sentence, letter ** input){
  
  //draw game scene and main objects
  draw_xpm_image(game->road_sprite,0,0,game->roadImg,get_double_buffer());
  draw_xpm_image(game->field_sprite,get_hres()/2.0-game->fieldImg.width/2,get_vres()/2-64,game->fieldImg,get_double_buffer());
  draw_xpm_image(game->car_sprite,0,get_vres()-game->carImg.height,game->carImg,get_double_buffer());

  //draw random sentence to start the game
  generateRandomSentence(sentence); // n = numero de letras da frase
  *input =  (letter*)malloc((game->numberOfLetters+5)*sizeof(letter)); // aloca espaço para o array de input - deixa espaço para 5 erros no fim

  //numero de pixels a avançar no moveCar counter
  game->oneMove = ((float)get_hres()-game->carImg.width)/(float)game->numberOfLetters ;
  game->moveCar = 0;

  draw_sentence(*sentence,get_hres()/2.0-350 ,get_vres()/2.0-250,get_double_buffer(),game->errors,-1);
  double_buffering();

}

void classicModeDraw(letter * sentence, letter * input, unsigned int index){
  draw_xpm_image(game->road_sprite,0,0,game->roadImg,get_double_buffer());

  draw_xpm_image(game->field_sprite,get_hres()/2.0-game->fieldImg.width/2,get_vres()/2-64,game->fieldImg,get_double_buffer());

  draw_input_sentence(input,sentence,get_hres()/2.0 - 350 ,get_vres()/2-30,get_double_buffer(),game->errors,index-1);

  if(game->errorState){
    draw_xpm_image(game->error_sprite,get_hres()/2.0-game->fieldImg.width/2,30,game->errorImg,get_double_buffer());
    draw_sentence(errorMessage,get_hres()/2.0-game->fieldImg.width/2 + game->errorImg.width + 15 ,40,get_double_buffer(),0,-1);
  }

  draw_xpm_image(game->car_sprite,game->moveCar,get_vres()-game->carImg.height,game->carImg,get_double_buffer());

  draw_sentence(sentence,get_hres()/2.0 - 350 ,get_vres()/2.0 - 250,get_double_buffer(),game->errors,index-1);

  double_buffering();
}

void timed_mode(letter ** sentence, letter ** input, unsigned int * index, bool * sec3counter, bool * timerreset, bool * timedResults){
  if (*sec3counter){
    counterStart();
    if(TIMER_COUNTER >= 160){
      *sec3counter = false;
      *timerreset = false;
    }  
    return;
  }
  
  if(game->state.start){
    *index = 0;
    game->errors = 0;
    game->errorState = false;
    game->completed = false;
    if(!*timerreset) TIMER_COUNTER = 0;
    *timerreset = true;
    timedModeStart(sentence, input);
    game->state.start = false;
    return;
  }

  if(TIMER_COUNTER / 60 == 60){// passou um minuto
    *timedResults = true;
  }
    
  if(game->keyState == ESC){// se for Esc sai do loop e não recebe mais interrupções
    game->state.mode = MENU;
    game->state.start = true;
    free(*input);
    free(*sentence);
    return;
  }
  else if(*timedResults){
    sentenceCompletedTimed();
    game->mc.lbPressed =false;
    *game->date = rtc_read_date();
    game->state.mode = RESULTS;
    game->state.start = true;
    free(*input);
    free(*sentence);
    return;
  }
  else if(game->keyState == DOT && game->errors == 0 && game->errorState == false ){
    game->keyState = NOT_VALID;
    game->mc.lbPressed =false;
    game->state.mode = TIMED;
    game->state.start = true;
    free(*input);
    free(*sentence);
    return;
  }
    
  timedModeDraw(*sentence, *input, *index);
  if(game->completed) return;
}

void timedModeStart(letter ** sentence, letter ** input){
  
  //draw game scene and main objects
  draw_xpm_image(game->road_sprite,0,0,game->roadImg,get_double_buffer());
  draw_xpm_image(game->field_sprite,get_hres()/2.0-game->fieldImg.width/2,get_vres()/2-64,game->fieldImg,get_double_buffer());
  draw_xpm_image(game->car_sprite,0,get_vres()-game->carImg.height,game->carImg,get_double_buffer());

  //draw random sentence to start the game
  generateRandomSentence(sentence); // n = numero de letras da frase
  *input =  (letter*)malloc((game->numberOfLetters+5)*sizeof(letter)); // aloca espaço para o array de input - deixa espaço para 5 erros no fim

  //numero de pixels a avançar no moveCar counter
  game->oneMove = ((float)get_hres()-game->carImg.width)/(float)game->numberOfLetters ;
  game->moveCar = 0;

  draw_sentence(*sentence,get_hres()/2.0-350 ,get_vres()/2.0-250,get_double_buffer(),game->errors,-1);
  double_buffering();

}

void timedModeDraw(letter * sentence, letter * input, unsigned int index){
  draw_xpm_image(game->road_sprite,0,0,game->roadImg,get_double_buffer());

  drawTimeElapsed();

  draw_xpm_image(game->field_sprite,get_hres()/2.0-game->fieldImg.width/2,get_vres()/2-64,game->fieldImg,get_double_buffer());
  draw_input_sentence(input,sentence,get_hres()/2.0 - 350 ,get_vres()/2-30,get_double_buffer(),game->errors,index-1);
  if(game->errorState){
    draw_xpm_image(game->error_sprite,get_hres()/2.0-game->fieldImg.width/2,30,game->errorImg,get_double_buffer());
    draw_sentence(errorMessage,get_hres()/2.0-game->fieldImg.width/2 + game->errorImg.width + 15 ,40,get_double_buffer(),0,-1);
  }

  draw_xpm_image(game->car_sprite,game->moveCar,get_vres()-game->carImg.height,game->carImg,get_double_buffer());
  draw_sentence(sentence,get_hres()/2.0 - 350 ,get_vres()/2.0 - 250,get_double_buffer(),game->errors,index-1);

  double_buffering();
}

void kbc_handle(letter ** sentence, letter ** input, unsigned int * index){
  
  game->keyState = checkScanCode(SCANCODE);
  if(game->keyState == ESC){// se for Esc sai do loop e não recebe mais interrupções
    game->state.mode =MENU;
    game->state.start = true;
    return;
  }
  else if(game->keyState == NOT_VALID) game->state.drawGame = false;
  else if(game->keyState == DOT && *index == game->numberOfLetters-1 && game->errors == 0 && game->errorState == false){//Ponto Final e final da frase
      game->nLet++;
      validateLetter(*sentence,SCANCODE,input,*index);
      *index = *index + 1;
      game->state.drawGame = true;
      game->completed = true;
  }
  else if((game->keyState == VALID || (game->keyState == DOT && *index != game->numberOfLetters-1)) && game->errorState == false ){ //letra, símbolo ou backspace

    
    inputValidation validation;

    validation = validateLetter(*sentence,SCANCODE,input,*index);
    if(validation == CORRECT){ //valid letter
      game->state.drawGame = true;
      *index = *index + 1;
      game->nLet++;
    }
    else if(validation == WRONG || validation == BACKSPACE){ //invalid letter or backspace
      game->state.drawGame = true;
    }
  }
  else if(game->errorState == true && game->keyState != NOT_VALID){
    game->state.drawGame = true;
  }
  
}

void mouse_handler(struct packet * p){
  if(PACKET_BYTE_COUNTER % 3 == 0){
    build_packet_struct(p,&PACKET);
    game->mc.lbPressed = p->lb;
    if(game->mc.mouseX + p->delta_x >= 0 && game->mc.mouseX + p->delta_x + game->mouseImg.width <= (int)get_hres()){
        game->mc.mouseX += p->delta_x;
    }
    if(game->mc.mouseY-p->delta_y >= 0 && game->mc.mouseY - p->delta_y + game->mouseImg.height <= (int)get_vres()){
      game->mc.mouseY -= p->delta_y;
    }
  }
}

void drawTimeElapsed(){

  int n=0;
  //show minutes
  draw_xpm_image(numbers[0].sprite,0,3,numbers[0].img,get_double_buffer());
  draw_xpm_image(numbers[0].sprite,17,3,numbers[0].img,get_double_buffer());

  //show two dots
  draw_xpm_image(numbers[10].sprite,17*2,3,numbers[10].img,get_double_buffer());

  //show seconds
  bool small = (TIMER_COUNTER/60 < 10);
  if(small) draw_xpm_image(numbers[0].sprite,17*3,3,numbers[0].img,get_double_buffer());

  char * sec=itoa(TIMER_COUNTER/60,10);
  for(unsigned int c = 0 ; sec[c] != '\0'; c++){
    n++;
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == sec[c]){
        if(small)
          draw_xpm_image(numbers[i].sprite,17*4+c*17,3,numbers[i].img,get_double_buffer());
        else
          draw_xpm_image(numbers[i].sprite,17*3+c*17,3,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }
  if(n==0){
    draw_xpm_image(numbers[0].sprite,17*4,3,numbers[0].img,get_double_buffer());
  }
}

keyPressed checkScanCode(uint8_t * scancode){
  uint8_t maskMSB= 0x80;

  if(game->errors == ERRORSTATE){
     if(scancode[0] == MC_BACK){
       game->errorState = false;
       return VALID;
     }
     else if(scancode[0] == MC_ESC){
       game->errorState = false;
       return ESC;
     }
     game->errorState = true;
     return ERRORSTATE;
  }
  //check for the type of scancode and print it
  if(!SCANCODE_2B && SCANCODE[0]!=SCANCODE_2B_FB){//if it is a 1 byte scancode
    if((maskMSB & SCANCODE[0])>>7 != 1){//if it is a make code
     
      if(scancode[0] == MC_ESC){
        return ESC;
      }
      else if(scancode[0]== MC_DOT){
        return DOT;
      }
      else if(scancode[0] == MC_SHIFT || scancode[0] == MC_SHIFT2){
        if(game->capital) game->capital = false;
        else game->capital = true;
        return CAPS;
      }
      else if(scancode[0] == MC_CAPS){
        if(!game->capital && game->CapsMakeCount == 0){
          game->CapsMakeCount ++;
          game->capital = true;
        }
        else if(!game->capital && game->CapsMakeCount == 1){
          game->CapsMakeCount = 0;
          game->capital = false;
        }
        else game->CapsMakeCount = 0;
        return CAPS;
      }
      //it's a letter/symbol
      else if((scancode[0] >= 0x10 && scancode[0] <= 0x19) || (scancode[0]>=0x1E && scancode[0] <= 0x26) || (scancode[0]>=0x2C && scancode[0] < 0x34) || scancode[0] == 0x0C || scancode[0] == BC_BACK || scancode[0] == MC_BACK || scancode[0]==0x39){
        if(game->errorState && scancode[0] == MC_BACK)
          game->errorState = false;
        return VALID;
      }
      else return NOT_VALID;
   }
   else{ // it is a break code
    if(scancode[0] == BC_SHIFT || scancode[0] == BC_SHIFT2){
      if(game->capital) game->capital = false;
      else game->capital= true;
      return CAPS;
    }
    else if(scancode[0] == BC_CAPS){
      if(game->capital && game->CapsMakeCount == 0){
        game->capital = false;
      }
      return CAPS;
    }
    return NOT_VALID;
   }
     
  } else return NOT_VALID;
}

void generateRandomSentence(letter ** sentence){
    time_t t;
    srand((unsigned) time(&t));
    unsigned x = rand()%nSentences;
    game->numberOfLetters = 0;
    while(sentences[x][game->numberOfLetters].makeCode !=  MC_DOT) game->numberOfLetters = game->numberOfLetters+1;
    game->numberOfLetters = game->numberOfLetters+1;

    *sentence = (letter*)malloc(game->numberOfLetters*sizeof(letter));
    memcpy(*sentence,sentences[x], game->numberOfLetters*sizeof(letter));
}

void sentenceCompleted(){
  game->result.timeSpentSec = TIMER_COUNTER/60;
  game->result.timeSpentCentSec = (int)(((TIMER_COUNTER%60)/60.0)*100.0);
  game->result.speed = (int)(roundf(game->numberOfLetters/(float)(TIMER_COUNTER/60) * 100) / 100);
}

void sentenceCompletedTimed(){
  game->result.timeSpentSec = TIMER_COUNTER/60;
  game->result.timeSpentCentSec = 0;
  game->result.speed = (int)(roundf(game->nLet/(float)(TIMER_COUNTER/60) * 100) / 100);
}

void resultScreen(){
  int n=0;
  //show results screen
  draw_xpm_image(game->results_sprite,0,0,game->resultsImg,get_double_buffer());

  //draw button to return to menu
  if(game->mc.mouseX >= (int)(get_hres()-15-game->menu_buttonImg.width) && game->mc.mouseX <= (int)(get_hres()-30) && game->mc.mouseY>=(int)(get_vres()-15-game->menu_buttonImg.height) && game->mc.mouseY<= (int)(get_vres()-35)){
    draw_xpm_image(game->menu_button_pressed_sprite,get_hres()-15-game->menu_button_pressedImg.width,get_vres()-game->menu_button_pressedImg.height-15,game->menu_button_pressedImg,get_double_buffer());         
  }
  else{
    draw_xpm_image(game->menu_button_sprite,get_hres()-15-game->menu_buttonImg.width,get_vres()-game->menu_buttonImg.height-15,game->menu_buttonImg,get_double_buffer());
  }

  int positionXi = 590;
  int positionYi = 300;
  int positionX = positionXi;
  int positionY = positionYi;

  //show number of words
  char * letAr=itoa(game->nLet,10);

  for(unsigned int c = 0 ; letAr[c] != '\0'; c++){
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == letAr[c]){
        positionX += 18;
        draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }
  
  positionX = positionXi;
  positionY = positionYi + 53;

  //show seconds
  char * sec=itoa(game->result.timeSpentSec,10);
  for(unsigned int c = 0 ; sec[c] != '\0'; c++){
    n++;
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == sec[c]){
        positionX += 18;
        draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }
  if(n==0) draw_xpm_image(numbers[0].sprite,positionX,positionY,numbers[0].img,get_double_buffer());

  //show two dots
  positionX += 21;
  draw_xpm_image(numbers[10].sprite,positionX,positionY,numbers[10].img,get_double_buffer());

  n=0;
  bool small = (game->result.timeSpentCentSec < 10);
  if(small){
    draw_xpm_image(numbers[0].sprite,positionX+18,positionY,numbers[0].img,get_double_buffer());
    positionX += 18;
  }

  //show centiseconds
  char * centSec=itoa(game->result.timeSpentCentSec,10);
  for(unsigned int c = 0 ;  centSec[c] != '\0'; c++){
    n++;
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == centSec[c]){
        positionX += 18;
        draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }

  positionX = positionXi;
  positionY = positionYi + 53*2;
  //show game->result.speed
  char * speedAr=itoa(game->result.speed,10);
  for(unsigned int c = 0 ; speedAr[c] != '\0'; c++){
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == speedAr[c]){
        positionX += 18;
        draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }
  //draw mouse
  draw_xpm_image(game->mouse_sprite,game->mc.mouseX,game->mc.mouseY,game->mouseImg,get_double_buffer());

  double_buffering();
  free(centSec);
  free(sec);
  free(speedAr);
  free(letAr);
}

inputValidation validateLetter(letter * let, uint8_t * scancode, letter ** input, unsigned index){

  if(scancode[0] == let[index].makeCode && game->errors == 0 && game->capital == let[index].capital){ /*correct input*/
    game->moveCar+= game->oneMove;
    (*input)[index] = let[index];
    return CORRECT;
  }
  else if((scancode[0] != let[index].makeCode && scancode[0] != MC_BACK && scancode[0] != BC_BACK) || (scancode[0] == let[index].makeCode && game->errors == 0 && game->capital == let[index].capital && game->errors > 0)||(scancode[0] == let[index].makeCode && game->errors == 0 && game->capital != let[index].capital)||
  (scancode[0] == let[index].makeCode && game->errors != 0 && game->capital == let[index].capital)||
  (scancode[0] == let[index].makeCode && game->errors != 0 && game->capital != let[index].capital)){ /*wrong input*/
    for(int j=0; j< nLetters; j++){
      if(abc[j].makeCode == scancode[0] && game->capital == abc[j].capital){
        (*input)[index+game->errors] = abc[j];
        break;
      }
    }
    game->errors++;
    return WRONG;
  }
  else if( scancode[0] == MC_BACK){
    if(game->errors > 0){
      (*input)[index+game->errors] = abc[58];
      game->errors--;
    }
    else return OTHER;
    return BACKSPACE;
  }
  else{
    return OTHER;
  }
}

void draw_date(){
  int positionXi = -10;
  int positionYi = 8;
  int positionX = positionXi;
  int positionY = positionYi;

  //show Day
  char * day=itoa(game->date->day,10);
  bool small = (game->date->day < 10);
  if(small){
    draw_xpm_image(numbers[0].sprite,positionX+18,positionY,numbers[0].img,get_double_buffer());
    positionX += 18;
  }
  for(unsigned int c = 0 ; day[c] != '\0'; c++){
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == day[c]){
        positionX += 18;
        draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }

  positionX += 22;
  draw_xpm_image(numbers[11].sprite,positionX,positionY,numbers[11].img,get_double_buffer());

  //show Month
  char * month=itoa(game->date->month,10);
  small = (game->date->month < 10);
  if(small){
    draw_xpm_image(numbers[0].sprite,positionX+18,positionY,numbers[0].img,get_double_buffer());
    positionX += 18;
  }
  for(unsigned int c = 0 ; month[c] != '\0'; c++){
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == month[c]){
        positionX += 18;
        draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }
  positionX += 22;
  draw_xpm_image(numbers[11].sprite,positionX,positionY,numbers[11].img,get_double_buffer());

  //show Year
  char * year=itoa(game->date->year,10);

  for(unsigned int c = 0 ; year[c] != '\0'; c++){
    for(unsigned int i = 0; i < 10; i++){
      if(numbers[i].number == year[c]){
        positionX += 18;
        draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
        break;
      }
    }
  }

  free(year);
  free(month);
  free(day);
}

void highScoresScreen(){
  //show results screen
  draw_xpm_image(game->highscores_sprite,0,0,game->highscoresImg,get_double_buffer());

  //draw button to return to menu
  if(game->mc.mouseX >= (int)(get_hres()-15-game->menu_buttonImg.width) && game->mc.mouseX <= (int)(get_hres()-30) && game->mc.mouseY>=(int)(get_vres()-15-game->menu_buttonImg.height) && game->mc.mouseY<= (int)(get_vres()-35)){
    draw_xpm_image(game->menu_button_pressed_sprite,get_hres()-15-game->menu_button_pressedImg.width,get_vres()-game->menu_button_pressedImg.height-15,game->menu_button_pressedImg,get_double_buffer());         
  }
  else{
    draw_xpm_image(game->menu_button_sprite,get_hres()-15-game->menu_buttonImg.width,get_vres()-game->menu_buttonImg.height-15,game->menu_buttonImg,get_double_buffer());
  }

  int positionX = 150;
  int positionY = 330;

  //draw highscores data
  for(unsigned int j=0; j< game->NUMBEROFSCORES; j++){

    //day
    char * tmp = itoa(game->highscores[j].date.day,10);
    bool small = (game->highscores[j].date.day < 10);
    if(small){
      draw_xpm_image(numbers[0].sprite,positionX+18,positionY,numbers[0].img,get_double_buffer());
      positionX += 18;
    }
    for(unsigned int c = 0 ; tmp[c] != '\0'; c++){
      for(unsigned int i = 0; i < 10; i++){
        if(numbers[i].number == tmp[c]){
          positionX += 18;
          draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
          break;
        }
      }
    }
    free(tmp);
    //month
    positionX += 21;
    draw_xpm_image(numbers[11].sprite,positionX,positionY,numbers[11].img,get_double_buffer());

    tmp = itoa(game->highscores[j].date.month,10);
    small = (game->highscores[j].date.month < 10);
    if(small){
      draw_xpm_image(numbers[0].sprite,positionX+18,positionY,numbers[0].img,get_double_buffer());
      positionX += 18;
    }
    for(unsigned int c = 0 ; tmp[c] != '\0'; c++){
      for(unsigned int i = 0; i < 10; i++){
        if(numbers[i].number == tmp[c]){
          positionX += 18;
          draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
          break;
        }
      }
    }
    free(tmp);
    //year
    positionX += 18;
    draw_xpm_image(numbers[11].sprite,positionX,positionY,numbers[11].img,get_double_buffer());

    tmp = itoa(game->highscores[j].date.year,10);
    for(unsigned int c = 0 ; tmp[c] != '\0'; c++){
      for(unsigned int i = 0; i < 10; i++){
        if(numbers[i].number == tmp[c]){
          positionX += 18;
          draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
          break;
        }
      }
    }
    free(tmp);
    //hour
    positionX += 60;

    tmp = itoa(game->highscores[j].date.hour,10);
    small = (game->highscores[j].date.hour < 10);
    
    if(small){
      draw_xpm_image(numbers[0].sprite,positionX,positionY,numbers[0].img,get_double_buffer());
      positionX += 18;
    }
    for(unsigned int c = 0 ; tmp[c] != '\0'; c++){
      for(unsigned int i = 0; i < 10; i++){
        if(numbers[i].number == tmp[c]){
          positionX += 18;
          draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
          break;
        }
      }
    }
    free(tmp);
    //minutes
    positionX += 18;
    draw_xpm_image(numbers[10].sprite,positionX,positionY,numbers[10].img,get_double_buffer());

    tmp = itoa(game->highscores[j].date.minutes,10);
    small = (game->highscores[j].date.minutes < 10);
    if(small){
      positionX += 18;
      draw_xpm_image(numbers[0].sprite,positionX,positionY,numbers[0].img,get_double_buffer());
    }
    for(unsigned int c = 0 ; tmp[c] != '\0'; c++){
      for(unsigned int i = 0; i < 10; i++){
        if(numbers[i].number == tmp[c]){
          positionX += 18;
          draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
          break;
        }
      }
    }
    free(tmp);
    //speed
    positionX += 60;
    tmp = itoa(game->highscores[j].speed,10);
    for(unsigned int c = 0 ; tmp[c] != '\0'; c++){
      for(unsigned int i = 0; i < 10; i++){
        if(numbers[i].number == tmp[c]){
          positionX += 18;
          draw_xpm_image(numbers[i].sprite,positionX,positionY,numbers[i].img,get_double_buffer());
          break;
        }
      }
    }
    free(tmp);
    //Name
    positionX += 190;
    draw_name(game->highscores[j].name.nameXpm,positionX,positionY,get_double_buffer(),game->highscores[j].name.nameSize);
    
    positionY += 55;
    positionX = 150;
  }

  //draw mouse
  draw_xpm_image(game->mouse_sprite,game->mc.mouseX,game->mc.mouseY,game->mouseImg,get_double_buffer());

  double_buffering();
}

void save_score_screen(){
  //show the screen with the buffer to enter the name
  draw_xpm_image(game->saveScores_sprite,0,0,game->saveScoresImg,get_double_buffer());

  //draw name input
  draw_name(game->name->nameXpm,get_hres()/2.0 - 85 ,get_vres()/2.0 - 90,get_double_buffer(),game->name->nameSize);

  //draw button to save score
  if(game->name->nameSize >=3 && game->mc.mouseX >= (int)(get_hres()/2.0+150) && game->mc.mouseX <= (int)(get_hres()/2.0+130 + game->saveScore_buttonImg.width) && game->mc.mouseY >= (int)(get_vres()/2.0 - 100) && game->mc.mouseY <= (int)(get_vres()/2.0 - 110 + game->saveScore_buttonImg.height)){
    draw_xpm_image(game->saveScore_button_sprite,get_hres()/2.0+150,get_vres()/2.0 - 100,game->saveScore_buttonImg,get_double_buffer());
  }
  else{
    draw_xpm_image(game->saveScore_button_unpressed_sprite,get_hres()/2.0+150,get_vres()/2.0 - 100,game->saveScore_button_unpressedImg,get_double_buffer());
  }

  //draw mouse
  draw_xpm_image(game->mouse_sprite,game->mc.mouseX,game->mc.mouseY,game->mouseImg,get_double_buffer());

  double_buffering();
}

void playerName(uint8_t * scancode){

  game->keyState = checkScanCode(scancode);

  if(game->keyState == VALID && game->name->nameSize < 10 && scancode[0] != abc[52].makeCode && scancode[0] != MC_BACK && scancode[0] != BC_BACK && scancode[0] != abc[53].makeCode && scancode[0] != abc[54].makeCode && scancode[0] != abc[55].makeCode && scancode[0] != abc[56].makeCode && scancode[0] != abc[57].makeCode){
    for(int j=0; j< nLetters; j++){
      if(abc[j].makeCode ==  scancode[0] && game->capital == abc[j].capital){
        game->name->nameXpm[game->name->nameSize] = abc[j];
        break;
      }
    }
    game->name->nameSize ++;
  }
  else if(game->keyState == VALID && game->name->nameSize > 0 && scancode[0] == MC_BACK){
    for(int j=0; j< nLetters; j++){
      if(abc[j].makeCode ==  scancode[0] && game->capital == abc[j].capital){
        game->name->nameXpm[game->name->nameSize] = abc[58];
        break;
      }
    }
    game->name->nameSize --;
  }
}

void back_to_menu(){
  if(!game->state.start){            
    if(game->mc.mouseX >= (int)(get_hres()-15-game->menu_buttonImg.width) && game->mc.mouseX <= (int)(get_hres()-30) && game->mc.mouseY>=(int)(get_vres()-15-game->menu_buttonImg.height) && game->mc.mouseY<= (int)(get_vres()-35) && game->mc.lbPressed){
      if(game->state.mode == HIGHSCORES){
        game->state.mode = MENU;
        game->state.start = true;
        game->mc.lbPressed = false;  
      }
      else if(game->state.mode == RESULTS){
          game->state.mode = SAVE_SCORE;
          game->state.start = true;
          game->mc.lbPressed = false;  
      }
    }
    else if (game->state.mode == SAVE_SCORE && game->name->nameSize >= 3 && game->mc.lbPressed && game->mc.mouseX >= (int)(get_hres()/2.0+150) && game->mc.mouseX <= (int)(get_hres()/2.0+150 + game->saveScore_buttonImg.width) && game->mc.mouseY >= (int)(get_vres()/2.0 - 100) && game->mc.mouseY <= (int)(get_vres()/2.0 - 100 + game->saveScore_buttonImg.height)){
      
      //save char array to write the player's name in the file
      for(unsigned int j = 0; j < game->name->nameSize; j++){
        for(unsigned int k = 0; k < nLetters; k++){
          if(abc[k].makeCode == game->name->nameXpm[j].makeCode && abc[k].capital == game->name->nameXpm[j].capital){
            game->name->nameAr[j] = abc[k].letter;
          }
        }
      }

      game->name->nameAr[game->name->nameSize] = '\0';

      if(new_highscore(game,game->result.speed) < 5) {
        add_new_highscore(&game,new_highscore(game,game->result.speed),game->result.speed,game->date);
      }
      game->state.mode = MENU;
      game->state.start = true;
      game->mc.lbPressed = false;  
    }
  }
  else{
    game->mc.mouseX = get_hres()/2;
    game->mc.mouseY = get_vres()/2;
    game->state.start = false;
  }
}

void free_sprites(){
  free(game->road_sprite);
  free(game->menu_sprite);
  free(game->button1_sprite);
  free(game->button2_sprite);
  free(game->button1Pressed_sprite);
  free(game->button2Pressed_sprite);
  free(game->car_sprite);
  free(game->field_sprite);
  free(game->error_sprite);
  free(game->mouse_sprite);
  free(game->results_sprite);
  free(game->menu_button_sprite);
  free(game->menu_button_pressed_sprite);
  free(game->redlight_sprite);
  free(game->yellowlight_sprite);
  free(game->greenlight_sprite);
  free(game->highscores_sprite);
  free(game->highscores_button_pressed_sprite); 
  free(game->highscores_button_unpressed_sprite);
  free(game->saveScores_sprite);
  free(game->saveScore_button_sprite);
  free(game->saveScore_button_unpressed_sprite);  

}
