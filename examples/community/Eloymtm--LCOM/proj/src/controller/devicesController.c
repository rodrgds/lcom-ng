#include "devicesController.h"
#include "../viewer/instrViewer.h"

int keyController(Game *game, uint8_t scancode, int *time) {

    Board *board = game->isPlayer1 ? game->board1 : game->board2;
    bool isPlayer1 = game->isPlayer1;

    switch (game->state) {
        case MENU:
            handleMenuState(game, board, scancode, time);
            break;
        case WAIT:
            handleWaitState(game, board, scancode, time);
            break;
        case SETUP:
            if(scancode == ESC){
            resetGame(game, time);
        }

        
        if (board->ships[board->selected].initial_coordinates.row == -1) { // OUTSIDE THE BOARD

            if (scancode == 0xCD) { // Move selection up

                drawSelected_ship(board->selected, WHITE);

                while (board->selected < 7 && board->ships[board->selected + 1].initial_coordinates.row != -1) {
                    board->selected++;
                }
                board->selected++;

                int last = lastShip(board);
                int first = firstShip(board);

                if (board->selected > last) {
                    board->selected = first;
                }

                drawSelected_ship(board->selected, SELECTED_SHIP);
            }

            if (scancode == 0xCB) { // Move selection down
                drawSelected_ship(board->selected, WHITE);

                while (board->selected > 0 && board->ships[board->selected - 1].initial_coordinates.row != -1) {
                    board->selected--;
                }
                board->selected--;

                int last = lastShip(board);
                int first = firstShip(board);

                if (board->selected < first) {
                    board->selected = last;
                }

                drawSelected_ship(board->selected, SELECTED_SHIP);
            }

            if (scancode == ENTER) { // ENTER key press

                
                Coordinate coor = getInitialCoor(board->selected);
                drawShip(coor.col, coor.row, board->ships[board->selected].size, board->ships[board->selected].vertical, WHITE); // paint white

                Coordinate coorTo = firstEmptyPosition(board, board->ships[board->selected].size);
                board->ships[board->selected].initial_coordinates.row = coorTo.row;
                board->ships[board->selected].initial_coordinates.col = coorTo.col;

                Coordinate pixel = convertCoord(isPlayer1, board->ships[board->selected].initial_coordinates);
                int x = pixel.col;
                int y = pixel.row;
                drawShip(x, y, board->ships[board->selected].size, board->ships[board->selected].vertical, SHIP_COLOR); // paint orange
        
                drawSelected_ship(board->selected,WHITE);
                drawSelected_shipBoard(isPlayer1, board->ships[board->selected],SELECTED_SHIP);
        
            }
        }

        else{ // ONSIDE THE BOARD
            
            handleScancode(board, game, isPlayer1, scancode, time);

        }

        break;

    case PLAYING:

        if(scancode == ESC){
            resetGame(game, time);
        }


        if (targetCanMove(board, scancode)){

            drawSelected_shipBoard(game->isPlayer1, board->hit, BLACK);  
            board->hit.initial_coordinates = moveSelected(board, board->hit.initial_coordinates, scancode); 
            drawSelected_shipBoard(game->isPlayer1, board->hit, SELECTED_SHIP);

            
        }

        if (scancode == ENTER && board->matrix[board->hit.initial_coordinates.row][board->hit.initial_coordinates.col] != HIT && board->matrix[board->hit.initial_coordinates.row][board->hit.initial_coordinates.col] != MISS) {
            
            drawSelected_shipBoard(game->isPlayer1,board->hit, BLACK);
            Coordinate hitCoord = board->hit.initial_coordinates;
           
            if (board->matrix[hitCoord.row][hitCoord.col] == SHIP) {
                
                board->matrix[hitCoord.row][hitCoord.col] = HIT;
                Coordinate coor = convertCoord(game->isPlayer1, hitCoord);
                drawShip(coor.col, coor.row, 1, true, SHIP_COLOR); 
             } 
            else {
                
                board->matrix[hitCoord.row][hitCoord.col] = MISS;
                Coordinate coor = convertCoord(game->isPlayer1, hitCoord);
                drawShip(coor.col, coor.row, 1, true, 0xFF0000);  
            }

            if(areAllShipsGuessed(board)){
                printf("Perdi o jogo!!");
                game->state =EXIT;
                *time = 0;
                drawSelected_shipBoard(game->isPlayer1,board->hit, BLACK);
                drawWinPlayer(isPlayer1);
                
            }else{
                game->isPlayer1 = !game->isPlayer1;
                board->hit.initial_coordinates.col = 0;
                board->hit.initial_coordinates.row = 0;
                
                drawSelected_shipBoard(game->isPlayer1, board->hit, SELECTED_SHIP);
                *time = 0;
            }
        }

        break;
    
    case RULES:
        if(scancode == ESC){
            resetGame(game, time);
        }
        break;


    case EXIT:
        if(scancode == ENTER){
            resetGame(game, time);
        }

        break;

    default:
        break;
    }

    return 0;
}

int mouseController(Game *game, const struct packet pp, int *xx, int *yy){

    *xx = *xx + pp.delta_x;
    *yy = *yy - pp.delta_y;
    switch (game->state) {
    case MENU:
        
        break;
    
    case PLAYING:
        
        break;

    default:
        break;
    }

    return 0;
}

int timerController(Game *game, int *time){
    switch (game->state){
    case WAIT:
        if (*time == 5) {
            if (game->isPlayer1){
                game->state = SETUP;
                *time = 0;
                if(drawBoard(true)!=0)return 1;
                paintBoard1(game->board1);
                drawSelected_ship(0, SELECTED_SHIP);
            }
            else{
                game->state = SETUP;
                *time = 0;
                if(drawBoard(true)!=0)return 1;
                paintBoard2(game->board2);
                drawSelected_ship(0, SELECTED_SHIP);
            }
        }
        break;
    
    case PLAYING:
        if (*time == 15) {
            Board *board = game->isPlayer1 ? game->board1 : game->board2;

            drawSelected_shipBoard(game->isPlayer1,board->hit, BLACK);

            game->isPlayer1 = !game->isPlayer1;
            board->hit.initial_coordinates.col = 0;
            board->hit.initial_coordinates.row = 0;
                
            drawSelected_shipBoard(game->isPlayer1, board->hit, SELECTED_SHIP);
            *time = 0;
        
        }
    
    default:
        break;
    }

    return 0;
}


void handleMenuState(Game *game, Board *board, uint8_t scancode, int *time) {
    if (scancode == UP || scancode == DOWN) {
        drawSelected(board->selected, WHITE);

        if (scancode == DOWN) {
            board->selected++;
        } else if (scancode == UP) {
            board->selected--;
        }

        if (board->selected == 3) {
            board->selected = 0;
        } else if (board->selected == -1) {
            board->selected = 2;
        }

        drawSelected(board->selected, BLACK);
    }

    if (scancode == ENTER) {
        switch (board->selected) {
            case 0:
                game->state = WAIT;
                *time = 0;
                drawWaiting(game->isPlayer1);
                break;
            case 1:
                game->state = RULES;
                *time = 0;
                drawInstr();
                break;
            case 2:
                game->state = END;
                *time = 0;
                break;
        }
    }
}

void handleWaitState(Game *game, Board *board, uint8_t scancode, int *time) {
    if (scancode == ENTER) {
        game->state = SETUP;
        *time = 0;
        if (drawBoard(true) != 0) return;
        if (game->isPlayer1) {
            paintBoard1(game->board1);
        } else {
            paintBoard2(game->board2);
        }
        drawSelected_ship(0, SELECTED_SHIP);
    }
}

void moveShip(Board *board, bool isPlayer1, int scancode) {
    // Determine direction based on scancode
    int dCol = 0, dRow = 0;
    switch (scancode) {
        case RIGHT:
            dCol = 1;
            break;
        case LEFT:
            dCol = -1;
            break;
        case UP:
            dRow = -1;
            break;
        case DOWN:
            dRow = 1;
            break;
        default:
            return; // Invalid scancode
    }

    if (canMove(board, &board->ships[board->selected], scancode)) {
        // Erase the current ship
        Coordinate pixel = convertCoord(isPlayer1, board->ships[board->selected].initial_coordinates);
        drawShip(pixel.col, pixel.row, board->ships[board->selected].size, board->ships[board->selected].vertical, WATER_COLOR);
        drawSelected_shipBoard(isPlayer1, board->ships[board->selected], BLACK);

        // Update ship coordinates
        board->ships[board->selected].initial_coordinates.col += dCol;
        board->ships[board->selected].initial_coordinates.row += dRow;

        // Draw the ship at the new position
        pixel = convertCoord(isPlayer1, board->ships[board->selected].initial_coordinates);
        drawShip(pixel.col, pixel.row, board->ships[board->selected].size, board->ships[board->selected].vertical, SHIP_COLOR);
        drawSelected_shipBoard(isPlayer1, board->ships[board->selected], SELECTED_SHIP);
    }
}


void resetGame(Game *game, int *time){

    *time = 0;
    game->state = MENU;
    game->board1 = initializeBoard();
    game->board2 = initializeBoard();
    game->isPlayer1 = true;
    drawMenu();

}

void handleMovement(Board *board, bool isPlayer1, int scancode) {
    if (scancode == RIGHT || scancode == LEFT || scancode == UP || scancode == DOWN) {
        moveShip(board, isPlayer1, scancode);
    }
}

void handleEmptyPosition(Board *board, bool isPlayer1) {
    Coordinate coor = nextEmptyPosition(board, board->ships[board->selected]);
    Coordinate pixel = convertCoord(isPlayer1, board->ships[board->selected].initial_coordinates);
    drawShip(pixel.col, pixel.row, board->ships[board->selected].size, board->ships[board->selected].vertical, WATER_COLOR);
    drawSelected_shipBoard(isPlayer1, board->ships[board->selected], BLACK);

    // Update ship coordinates
    board->ships[board->selected].initial_coordinates.col = coor.col;
    board->ships[board->selected].initial_coordinates.row = coor.row;

    // Draw the ship at the new position
    pixel = convertCoord(isPlayer1, board->ships[board->selected].initial_coordinates);
    drawShip(pixel.col, pixel.row, board->ships[board->selected].size, board->ships[board->selected].vertical, SHIP_COLOR);
    drawSelected_shipBoard(isPlayer1, board->ships[board->selected], SELECTED_SHIP);
}

void handleRotation(Board *board, bool isPlayer1) {
    drawSelected_shipBoard(isPlayer1, board->ships[board->selected], BLACK);
    if (rotateShip(board, &board->ships[board->selected])) {
        // Erase the current ship
        Coordinate pixel = convertCoord(isPlayer1, board->ships[board->selected].initial_coordinates);
        drawShip(pixel.col, pixel.row, board->ships[board->selected].size, !board->ships[board->selected].vertical, WATER_COLOR); // Note the ! to use previous orientation
        drawSelected_shipBoard(isPlayer1, board->ships[board->selected], BLACK);

        // Draw the ship at the new position
        pixel = convertCoord(isPlayer1, board->ships[board->selected].initial_coordinates);
        drawShip(pixel.col, pixel.row, board->ships[board->selected].size, board->ships[board->selected].vertical, SHIP_COLOR);
        drawSelected_shipBoard(isPlayer1, board->ships[board->selected], SELECTED_SHIP);
    }
    drawSelected_shipBoard(isPlayer1, board->ships[board->selected], SELECTED_SHIP);
}

void handleEnter(Board *board, Game *game, bool isPlayer1, int *time) {
    drawSelected_ship(board->selected, WHITE);
    drawSelected_shipBoard(isPlayer1, board->ships[board->selected], BLACK);
    
    Ship ship1 = board->ships[board->selected];
    if (ship1.vertical) {
        for (int i = 0; i < ship1.size; i++) {
            board->matrix[ship1.initial_coordinates.row + i][ship1.initial_coordinates.col] = SHIP;
        }
    } else {
        for (int i = 0; i < ship1.size; i++) {
            board->matrix[ship1.initial_coordinates.row][ship1.initial_coordinates.col + i] = SHIP;
        }
    }

    if (setUpDone(board)) {
        if (isPlayer1) {
            game->state = WAIT;
            *time = 0;
            game->isPlayer1 = !game->isPlayer1;
            drawWaiting(game->isPlayer1);
        } else {
            Board *temp = game->board1;
            game->board1 = game->board2;
            game->board2 = temp;

            game->state = PLAYING;
            *time = 0;
            game->isPlayer1 = !game->isPlayer1;
            drawBoard(false);
            paintBoard1(game->board1);
            paintBoard2(game->board2);
            drawSelected_shipBoard(game->isPlayer1, board->hit, SELECTED_SHIP);
        }
    } else {
        int last = lastShip(board);
        int first = firstShip(board);

        if (board->selected == last) {
            board->selected = first;
        } else {
            board->selected++;
            while (board->ships[board->selected].initial_coordinates.row != -1 && board->selected <= last) {
                board->selected++;
            }
            if (board->selected > last) {
                board->selected = first;
            }
        }

        drawSelected_ship(board->selected, SELECTED_SHIP);
    }
}

void handleScancode(Board *board, Game *game, bool isPlayer1, int scancode, int *time) {
    switch (scancode) {
        case RIGHT:
        case LEFT:
        case UP:
        case DOWN:
            handleMovement(board, isPlayer1, scancode);
            break;
        case E:
            handleEmptyPosition(board, isPlayer1);
            break;
        case R:
            handleRotation(board, isPlayer1);
            break;
        case ENTER:
            handleEnter(board, game, isPlayer1, time);
            break;
        default:
            break;
    }
}
