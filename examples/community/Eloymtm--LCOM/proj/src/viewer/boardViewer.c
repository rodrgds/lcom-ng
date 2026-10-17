#include "boardViewer.h"

extern vbe_mode_info_t info;

int (drawBoard)(bool ships){

    uint16_t x = 400;
    uint16_t y = 0;

    uint16_t x1 = 220;
    uint16_t y1 = 200;

    uint16_t x2 = 678;
    uint16_t y2 = 200;

    reset_video();

    if(vg_draw_rectangle(0, 0, 1152, 864, WHITE)!=0)return 1;

    if(show_xpm(batalhaNaval1,x,y)!=0)return 1;

    if(show_xpm(player1,x1,y1)!=0)return 1;

    if(show_xpm(player2,x2,y2)!=0)return 1;


    if(drawGrid(128)!=0)return 1;

    if(drawGrid(600)!=0)return 1;

    if(ships){
      if(drawShips()!=0)return 1;
    }
   
    return 0;
}

int (drawGrid)(uint16_t xi){

    for (uint16_t y = 278; y < 700; y+=42){
      if(vg_draw_hline(xi, y, 422, 0x000000) != 0)return 1;
      if(vg_draw_hline(xi, y + 1, 422, 0x000000) != 0)return 1;
      
    }

    for (uint16_t x = xi; x < xi + 422; x+=42){
      if(vg_draw_vline(x, 278, 422, 0x000000) != 0)return 1;
      if(vg_draw_vline(x+1, 278, 422, 0x000000) != 0)return 1;
      
    }

    return 0;
}

int (drawShips)(){

    if(drawShip(210, 735, 1, false,0xFFA500) != 0)return 1;
    if(drawShip(290, 735, 1, false,0xFFA500) != 0)return 1;

    if(drawShip(370, 735, 2, false,0xFFA500) != 0)return 1;
    if(drawShip(490, 735, 2, false,0xFFA500) != 0)return 1;

    if(drawShip(610, 735, 3, false,0xFFA500) != 0)return 1;
    if(drawShip(770, 735, 3, false,0xFFA500) != 0)return 1;

    if(drawShip(370, 795, 4, false,0xFFA500) != 0)return 1;
    if(drawShip(610, 795, 4, false,0xFFA500) != 0)return 1;

    return 0;
}

int (drawShip)(int x, int y, int n, bool vertical,uint32_t color){

    for (int i = 0; i < n; i++) {
        if (vertical) {
            if (vg_draw_rectangle(x, y + i * 42, 40, 40, color) != 0) return 1;
        } else {
            if (vg_draw_rectangle(x + i * 42, y, 40, 40, color) != 0) return 1;
        }
    }
    return 0;
}

int (paintBoard1)(Board *board){

  for (int i = 0; i < BOARD_SIZE; i++){
    for (int j = 0; j < BOARD_SIZE; j++){
      
      if(vg_draw_rectangle(130 + (i * 42), 280 + (j* 42), 40, 40, 0x4169E1)!=0)return 1;
         
    }
  }

  return 0;

}
int (paintBoard2)(Board *board){

  for (int i = 0; i < BOARD_SIZE; i++){
    for (int j = 0; j < BOARD_SIZE; j++){
        
      if(vg_draw_rectangle(602 + (i * 42), 280 + (j* 42), 40, 40, 0x4169E1)!=0)return 1;
          
    }
  }

  return 0;

}

int (drawSelected_ship)(int selected, uint32_t color) {
    int x_base = 208;
    int y_base = 733;
    int height = 44; 
    int tamanho = 0; 

    // Calcular x_base e width com base no barco selecionado
    switch (selected) {
        case 0:
            x_base = 208;
            tamanho = 42;  // 1 cubo
            break;
        case 1:
            x_base = 288;
            tamanho = 42;  // 1 cubo
            break;
        case 2:
            x_base = 368;  // Distância dos dois primeiros barcos + distância
            tamanho = 84;  // 2 cubos
            break;
        case 3:
            x_base = 488;  // Distância dos três primeiros barcos + distância
            tamanho = 84;  // 2 cubos
            break;
        case 4:
            x_base = 610;  // Distância dos cinco primeiros barcos + distância
            tamanho = 124;  // 3 cubos
            break;
        case 5:
            x_base = 770;  // Distância dos seis primeiros barcos + distância
            tamanho = 124;  // 3 cubos
            break;
        case 6:
            x_base = 370;  // Distância dos cinco primeiros barcos + distância
            tamanho = 166; 
            y_base = 793;// 3 cubos
            break;
        case 7:
            x_base = 610;  // Distância dos seis primeiros barcos + distância
            tamanho = 166;
            y_base = 793; // 3 cubos
            break;   
        default:
            return 1;  // Valor de selected inválido
    }


    // Desenhar as linhas verticais do contorno
    if (vg_draw_vline(x_base, y_base, height, color) != 0) return 1;
    if (vg_draw_vline(x_base + 1, y_base, height, color) != 0) return 1;
    if (vg_draw_vline(x_base + tamanho - 1, y_base, height, color) != 0) return 1;
    if (vg_draw_vline(x_base + tamanho , y_base, height, color) != 0) return 1;

    // Desenhar as linhas horizontais do contorno
    if (vg_draw_hline(x_base, y_base, tamanho, color) != 0) return 1;
    if (vg_draw_hline(x_base , y_base + 1, tamanho, color) != 0) return 1;
    if (vg_draw_hline(x_base, y_base + height-2, tamanho, color) != 0) return 1;
    if (vg_draw_hline(x_base, y_base + height-1, tamanho, color) != 0) return 1;

    return 0;
}

Coordinate (convertCoord)(bool isPlayer1, Coordinate coor){

  Coordinate coordinate = {-1, -1};
  int x = coor.col;
  int y = coor.row;

  if(isPlayer1){
    x = (130 + (42*x));
    y = (280 + (42*y));
  }

  else{
    x = (602 + (42*x));
    y = (280 + (42*y));
  }

  coordinate.col = x; 
  coordinate.row = y;

  return coordinate;
}

int (drawSelected_shipBoard)(bool isPlayer1, Ship ship, uint32_t color) {
   Coordinate coor = convertCoord(isPlayer1, ship.initial_coordinates);
    
    int x = coor.col;
    int y = coor.row;
    int size = ship.size;

    if(ship.vertical){
      if (vg_draw_vline(x-2, y-2, size * 42, color) != 0) return 1;
        if (vg_draw_vline(x-1, y-2, size * 42, color) != 0) return 1;
        if (vg_draw_vline(x-2 + 42, y-2, size * 42, color) != 0) return 1;
        if (vg_draw_vline(x-1 + 42, y-2, size * 42, color) != 0) return 1;

        if (vg_draw_hline(x-2, y-2, 42, color) != 0) return 1;
        if (vg_draw_hline(x-2, y-1, 42, color) != 0) return 1;
        if (vg_draw_hline(x-2, y-2 + (size * 42), 42+2, color) != 0) return 1;
        if (vg_draw_hline(x-2, y-1 + (size * 42), 42+2, color) != 0) return 1;
    }

    else{
      if (vg_draw_vline(x-2, y-2, 42, color) != 0) return 1;
      if (vg_draw_vline(x-1, y-2, 42, color) != 0) return 1;
      if (vg_draw_vline((x -2) + (size*42) , y -2, 42+2, color) != 0) return 1;
      if (vg_draw_vline((x -1)+ (size*42) , y - 2, 42+2, color) != 0) return 1;

      if (vg_draw_hline(x - 2, y -2, size * 42, color) != 0) return 1;
      if (vg_draw_hline(x - 2 , y -1 , size * 42, color) != 0) return 1;
      if (vg_draw_hline(x - 2, y-2 + 42, (size * 42)+1, color) != 0) return 1;
      if (vg_draw_hline(x - 2, y -1 + 42, (size * 42)+1, color) != 0) return 1;
    }
    
  return 0;
}

bool (setUpDone)(Board *board){
  
  for(int i = 0; i < 8; i++){
    if(board->ships[i].initial_coordinates.col == -1){
      return false;
    }
  }

  return true;
}
