#include "board.h"

Board* initializeBoard() {
    Board* board = (Board*) malloc(sizeof(Board));
    board->matrix = (char**) malloc(BOARD_SIZE * sizeof(char*));
    

    for (int i = 0; i < BOARD_SIZE; i++) {
        board->matrix[i] = (char*) malloc(BOARD_SIZE * sizeof(char));
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board->matrix[i][j] = WATER;
            
        }
    }

    board->num_ships = 0;
    board->selected = 0;
    Coordinate coor = {0,0};

    board->hit.initial_coordinates = coor;
    board->hit.size = 1;
    board->hit.vertical= false;

    board->ships = (Ship*) malloc(8 * sizeof(Ship));

    int sizeShip = 1;
    // Inicializar navios
    for (int i = 0; i < 8; i = i + 2) {
        board->num_ships += 2;
        Coordinate initial_coordinate = {-1, -1}; // Coordenadas iniciais para exemplo
        initializeShip(&board->ships[i], sizeShip, initial_coordinate, false);
        initializeShip(&board->ships[i + 1], sizeShip, initial_coordinate, false); // Inicializa cada navio
        sizeShip++;
    }

    return board;
}

bool(isEmpty)(Board board, Coordinate coordinate){


    if(board.matrix[coordinate.row][coordinate.col] == WATER){
        return true;
    }
    return false;
    
}

bool (canRotate)(Board* board, Ship *ship) {
    int startRow = ship->initial_coordinates.row;
    int startCol = ship->initial_coordinates.col;
    int endRow, endCol;

    if (ship->vertical) {
        // Verifica se a rotação para horizontal está dentro dos limites
        endRow = startRow;
        endCol = startCol + ship->size - 1;

        if (endCol >= BOARD_SIZE) {
            return false; // Fora dos limites
        }

        // Verifica se há espaço livre para a rotação
        for (int col = startCol; col <= endCol; col++) {
            if (board->matrix[startRow][col] != WATER) {
                return false; // Outro navio está no caminho
            }
        }
    } 
    else {
        // Verifica se a rotação para vertical está dentro dos limites
        endRow = startRow + ship->size - 1;
        endCol = startCol;

        if (endRow >= BOARD_SIZE) {
            return false; // Fora dos limites
        }

        // Verifica se há espaço livre para a rotação
        for (int row = startRow; row <= endRow; row++) {
            if (board->matrix[row][startCol] != WATER) {
                return false; // Outro navio está no caminho
            }
        }
    }

    return true; // Pode rotacionar
}

bool(canMove)(Board* board, Ship *ship, int direction){
    int startRow = ship->initial_coordinates.row;
    int startCol = ship->initial_coordinates.col;

    switch (direction) {
        case 0xC8: // Up
            if (startRow == 0) return false; // Fora dos limites
            if (ship->vertical) {
                if (board->matrix[startRow - 1][startCol] != WATER) return false; // Colisão
            } 
            else {
                for (int i = 0; i < ship->size; i++) {
                    if (board->matrix[startRow - 1][startCol + i] != WATER) return false; // Colisão
                }
            }
            break;
        case 0xD0: // Down
            if (ship->vertical) {
                if (startRow + ship->size >= BOARD_SIZE) return false; // Fora dos limites
                if (board->matrix[startRow + ship->size][startCol] != WATER) return false; // Colisão
            } 
            else {
                if (startRow + 1 >= BOARD_SIZE) return false; // Fora dos limites
                for (int i = 0; i < ship->size; i++) {
                    if (board->matrix[startRow + 1][startCol + i] != WATER) return false; // Colisão
                }
            }
            break;
        case 0xCB: // Left
            if (startCol == 0) return false; // Fora dos limites
            if (ship->vertical) {
                for (int i = 0; i < ship->size; i++) {
                    if (board->matrix[startRow + i][startCol - 1] != WATER) return false; // Colisão
                }
            } 
            else {
                if (board->matrix[startRow][startCol - 1] != WATER) return false; // Colisão
            }
            break;
        case 0xCD: // Right
            if (ship->vertical) {
                if (startCol + 1 >= BOARD_SIZE) return false; // Fora dos limites
                for (int i = 0; i < ship->size; i++) {
                    if (board->matrix[startRow + i][startCol + 1] != WATER) return false; // Colisão
                }
            } 
            else {
                if (startCol + ship->size >= BOARD_SIZE) return false; // Fora dos limites
                if (board->matrix[startRow][startCol + ship->size] != WATER) return false; // Colisão
            }
            break;
        default:
            return false; // Direção inválida
    }

    return true; // Pode mover

}

bool(targetCanMove)(Board* board, int direction){
    int startRow = board->hit.initial_coordinates.row;
    int startCol = board->hit.initial_coordinates.col;

    switch (direction) {
        case 0xC8: // Up
            if (startRow == 0) return false; // Fora dos limites
            break;
        case 0xD0: // Down
            
                
            if (startRow + 1 >= BOARD_SIZE) return false; // Fora dos limites
                
            break;
        case 0xCB: // Left
            if (startCol == 0) return false; // Fora dos limites
    
            break;
        case 0xCD: // Right
            
            if (startCol + 1 >= BOARD_SIZE) return false; // Fora dos limites
                
            break;
        default:
            return false; // Direção inválida
    }

    return true; // Pode mover

}

bool (rotateShip)(Board* board, Ship *ship){
    if(canRotate(board, ship)){
        
        // Rotacionar o navio
        ship->vertical = !ship->vertical;

        return true;
    }
    return false;

}



Coordinate (getInitialCoor)(int shipN){ //pseudo Coordinate
     Coordinate coor = {-1, -1};
    switch (shipN)
    {
    case 0: 
    coor.col = 210;
    coor.row = 735;
        break;
    
    case 1: 
    coor.col = 290;
    coor.row = 735;
        break;

    case 2: 
    coor.col = 370;
    coor.row = 735;
        break;

    case 3: 
    coor.col = 490;
    coor.row = 735;
        break;

    case 4: 
    coor.col = 610;
    coor.row = 735;
        break;

    case 5: 
    coor.col = 770;
    coor.row = 735;
        break;

    case 6: 
    coor.col = 370;
    coor.row = 795;
        break;

    case 7: 
    coor.col = 610;
    coor.row = 795;
        break;
    default:
        break;
    }


return coor;

}

int (lastShip)(Board *board){
    int last = -1;
    for(int i = 0; i < 8; i++){
        if(board->ships[i].initial_coordinates.col == -1){
            last = i;
        }

    }
    return last;

}

int (firstShip)(Board *board){

    for(int i = 0; i < 8; i++){
        if(board->ships[i].initial_coordinates.col == -1){
            return i;
        }

    }
    return -1;

}


Coordinate (firstEmptyPosition)(Board *board, int ship_size) {
    Coordinate coor = {-1, -1};

    for (int i = 0; i < BOARD_SIZE; i++) {
        int count = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board->matrix[i][j] == WATER) {
                if (count == 0) {
                    coor.row = i;
                    coor.col = j;
                }
                count++;
                if (count == ship_size) {
                    return coor;
                }
            } else {
                count = 0;
            }
        }
    }
    return coor;
}
Coordinate (nextEmptyPosition)(Board *board, Ship ship){
    Coordinate array[sizeof(Coordinate)*ship.size];
    Coordinate xy = {-1,-1};
    Coordinate ship_coor = ship.initial_coordinates;

    if (ship.vertical){
        for (int i = 0; i < ship.size; i++){
            xy.col = ship_coor.col;
            xy.row = ship_coor.row + i;
            array[i] = xy; 
        }
        for (int j = 0; j < BOARD_SIZE; j++) {
            int count = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                if (board->matrix[i][j] == WATER && isNoInArray(j, i, array, ship.size)) {
                    if (count == 0) {
                        ship_coor.row = i;
                        ship_coor.col = j;
                    }
                    count++;
                    if (count == ship.size) {
                        return ship_coor;
                    }
                } else {
                    count = 0;
                }
            }
        }
    }
    else{
        for (int i = 0; i < ship.size; i++){
            xy.col = ship_coor.col + i;
            xy.row = ship_coor.row;
            array[i] = xy; 
        }

        for (int i = 0; i < BOARD_SIZE; i++) {
            int count = 0;
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board->matrix[i][j] == WATER && isNoInArray(j, i, array, ship.size)) {
                    if (count == 0) {
                        ship_coor.row = i;
                        ship_coor.col = j;
                    }
                    count++;
                    if (count == ship.size) {
                        return ship_coor;
                    }
                } else {
                    count = 0;
                }
            }
        }
    }

    return ship_coor;
}

Coordinate (moveSelected)(Board *board,Coordinate pos,int scancode){

    Coordinate coor = pos;

    switch (scancode) {
        case UP:
            if (pos.row > 0) coor.row--;
            break;
        case DOWN:
            if (pos.row < BOARD_SIZE - 1) coor.row++;
            break;
        case LEFT:
            if (pos.col > 0) coor.col--;
            break;
        case RIGHT:
            if (pos.col < BOARD_SIZE - 1) coor.col++;
            break;
        default:
            break;
    }

    return coor;
}

Coordinate (firstEmptyHitPosition)(Board *board) {
    Coordinate coor = {-1, -1};

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board->matrix[i][j] != HIT && board->matrix[i][j] != MISS) {
                coor.row = i;
                coor.col = j;
                return coor;
            }
        }
    }
    return coor;
}

bool (isNoInArray)(int x,int y,Coordinate array[], int size){

    for (int i = 0; i < size; i++){
        if(array[i].col == x && array[i].row == y){
            return false;
        }
    }
    return true;
}

bool (areAllShipsGuessed)(Board *board){
    for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board->matrix[i][j] ==SHIP) {
                    return false;
            }
        }
    }
    return true;
}
