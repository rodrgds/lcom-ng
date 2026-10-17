#include "coin.h"

SmallCoin small_coins[323];
BigCoin big_coins[5];

void (set_small_coins_positions)() {
    int i;

    // 1ª linha 1ª parte
    i = 195;
    for (int c = 0; c < 15; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 65;
        small_coins[c].pos.bottom_y = 90;
        i += 19;
    }

    // 1ª linha 2ª parte
    i = 555;
    for (int c = 15; c < 30; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 65;
        small_coins[c].pos.bottom_y = 90;
        i += 19;
    }

    // 2ª linha
    i = 460;
    for (int c = 30; c < 36; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 120;
        small_coins[c].pos.bottom_y = 145;
        i += 19;
    }

    // 3ª linha 1ª parte
    i = 195;
    for (int c = 68; c < 80; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 155;
        small_coins[c].pos.bottom_y = 180;
        i += 19;
    }

    // 3ª linha 2ª parte
    i = 610;
    for (int c = 200; c < 212; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 155;
        small_coins[c].pos.bottom_y = 180;
        i += 19;
    }    

    // 4ª linha 1ª parte
    i = 195;
    for (int c = 80; c < 87; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 220;
        small_coins[c].pos.bottom_y = 245;
        i += 19;
    }    

    // 4ª linha 2ª parte
    i = 390;
    for (int c = 58; c < 63; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 220;
        small_coins[c].pos.bottom_y = 245;
        i += 19;
    }

    // 4ª linha 3ª parte
    i = 550;
    for (int c = 63; c < 68; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 220;
        small_coins[c].pos.bottom_y = 245;
        i += 19;
    }

    // 4ª linha 4ª parte
    i = 710;
    for (int c = 87; c < 94; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 220;
        small_coins[c].pos.bottom_y = 245;
        i += 19;
    }    

    // 5ª linha
    i = 385;
    for (int c = 94; c < 108; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 310;
        small_coins[c].pos.bottom_y = 335;
        i += 19;
    } 

    // 6ª linha 1ª parte
    i = 195;
    for (int c = 36; c < 47; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 365;
        small_coins[c].pos.bottom_y = 390;
        i += 19;
    }
    
    // 6ª linha 2ª parte
    i = 630;
    for (int c = 47; c < 58; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 365;
        small_coins[c].pos.bottom_y = 390;
        i += 19;
    }

    // 7ª linha
    i = 385;
    for (int c = 108; c < 122; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 430;
        small_coins[c].pos.bottom_y = 455;
        i += 19;
    }

    // 8ª linha 1ª parte
    i = 195;
    for (int c = 122; c < 137; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 510;
        small_coins[c].pos.bottom_y = 535;
        i += 19;
    }
    
    // 8ª linha 2ª parte
    i = 560;
    for (int c = 137; c < 152; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 510;
        small_coins[c].pos.bottom_y = 535;
        i += 19;
    }

    // 9ª linha 1ª parte
    i = 195;
    for (int c = 152; c < 155; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 580;
        small_coins[c].pos.bottom_y = 605;
        i += 19;
    }

    // 9ª linha 2ª parte
    i = 310;
    for (int c = 155; c < 161; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 580;
        small_coins[c].pos.bottom_y = 605;
        i += 19;
    }

    // 9ª linha 3ª parte
    i = 620;
    for (int c = 161; c < 167; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 580;
        small_coins[c].pos.bottom_y = 605;
        i += 19;
    }

    // 9ª linha 4ª parte
    i = 792;
    for (int c = 167; c < 170; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 580;
        small_coins[c].pos.bottom_y = 605;
        i += 19;
    }

    // 10ª linha
    i = 235;
    for (int c = 170; c < 200; c++) {
        small_coins[c].pos.left_x = i;
        small_coins[c].pos.right_x = i + 25;
        small_coins[c].pos.top_y = 700;
        small_coins[c].pos.bottom_y = 725;
        i += 19;
    }

    // 1ª coluna 1ª parte
    i = 65;
    for (int c = 212; c < 217; c++) {
        small_coins[c].pos.left_x = 195;
        small_coins[c].pos.right_x = 220;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 1ª coluna 2ª parte
    i = 180;
    for (int c = 217; c < 219; c++) {
        small_coins[c].pos.left_x = 195;
        small_coins[c].pos.right_x = 220;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 1ª coluna 3ª parte
    i = 535;
    for (int c = 219; c < 221; c++) {
        small_coins[c].pos.left_x = 195;
        small_coins[c].pos.right_x = 220;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 2ª coluna
    i = 600;
    for (int c = 221; c < 226; c++) {
        small_coins[c].pos.left_x = 233;
        small_coins[c].pos.right_x = 358;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 3ª coluna 1ª parte
    i = 90;
    for (int c = 226; c < 229; c++) {
        small_coins[c].pos.left_x = 310;
        small_coins[c].pos.right_x = 345;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }
    
    // 3ª coluna 2º parte
    i = 175;
    for (int c = 229; c < 231; c++) {
        small_coins[c].pos.left_x = 310;
        small_coins[c].pos.right_x = 345;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 3ª coluna 3º parte
    i = 240;
    for (int c = 231; c < 238; c++) {
        small_coins[c].pos.left_x = 310;
        small_coins[c].pos.right_x = 345;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 3ª coluna 4º parte
    i = 380;
    for (int c = 238; c < 245; c++) {
        small_coins[c].pos.left_x = 310;
        small_coins[c].pos.right_x = 345;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 3ª coluna 5º parte
    i = 535;
    for (int c = 245; c < 247; c++) {
        small_coins[c].pos.left_x = 310;
        small_coins[c].pos.right_x = 345;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 3ª coluna 6º parte
    i = 600;
    for (int c = 247; c < 252; c++) {
        small_coins[c].pos.left_x = 310;
        small_coins[c].pos.right_x = 345;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 4ª coluna 1º parte
    i = 180;
    for (int c = 260; c < 262; c++) {
        small_coins[c].pos.left_x = 390;
        small_coins[c].pos.right_x = 415;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 4ª coluna 2º parte
    i = 330;
    for (int c = 258; c < 260; c++) {
        small_coins[c].pos.left_x = 385;
        small_coins[c].pos.right_x = 410;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 4ª coluna 3º parte
    i = 390;
    for (int c = 256; c < 258; c++) {
        small_coins[c].pos.left_x = 385;
        small_coins[c].pos.right_x = 410;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 4ª coluna 4º parte
    i = 430;
    for (int c = 252; c < 256; c++) {
        small_coins[c].pos.left_x = 385;
        small_coins[c].pos.right_x = 410;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 4ª coluna 5º parte
    i = 600;
    for (int c = 247; c < 252; c++) {
        small_coins[c].pos.left_x = 405;
        small_coins[c].pos.right_x = 430;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 5ª coluna 1º parte
    i = 180;
    for (int c = 262; c < 264; c++) {
        small_coins[c].pos.left_x = 608;
        small_coins[c].pos.right_x = 633;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 5ª coluna 2º parte
    i = 330;
    for (int c = 264; c < 266; c++) {
        small_coins[c].pos.left_x = 632;
        small_coins[c].pos.right_x = 657;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 5ª coluna 3º parte
    i = 390;
    for (int c = 258; c < 260; c++) {
        small_coins[c].pos.left_x = 632;
        small_coins[c].pos.right_x = 657;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 5ª coluna 4º parte
    i = 450;
    for (int c = 266; c < 269; c++) {
        small_coins[c].pos.left_x = 632;
        small_coins[c].pos.right_x = 657;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 5ª coluna 5º parte
    i = 600;
    for (int c = 269; c < 274; c++) {
        small_coins[c].pos.left_x = 615;
        small_coins[c].pos.right_x = 440;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 6ª coluna 1ª parte
    i = 90;
    for (int c = 274; c < 277; c++) {
        small_coins[c].pos.left_x = 705;
        small_coins[c].pos.right_x = 730;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }
    
    // 6ª coluna 2º parte
    i = 175;
    for (int c = 277; c < 279; c++) {
        small_coins[c].pos.left_x = 705;
        small_coins[c].pos.right_x = 730;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 6ª coluna 3º parte
    i = 240;
    for (int c = 279; c < 286; c++) {
        small_coins[c].pos.left_x = 705;
        small_coins[c].pos.right_x = 730;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 6ª coluna 4º parte
    i = 380;
    for (int c = 286; c < 293; c++) {
        small_coins[c].pos.left_x = 705;
        small_coins[c].pos.right_x = 730;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 6ª coluna 5º parte
    i = 535;
    for (int c = 293; c < 295; c++) {
        small_coins[c].pos.left_x = 705;
        small_coins[c].pos.right_x = 730;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 6ª coluna 6º parte
    i = 600;
    for (int c = 295; c < 300; c++) {
        small_coins[c].pos.left_x = 705;
        small_coins[c].pos.right_x = 730;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 7ª coluna
    i = 600;
    for (int c = 308; c < 313; c++) {
        small_coins[c].pos.left_x = 795;
        small_coins[c].pos.right_x = 820;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 8ª coluna 1ª parte
    i = 85;
    for (int c = 300; c < 304; c++) {
        small_coins[c].pos.left_x = 820;
        small_coins[c].pos.right_x = 845;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 8ª coluna 2ª parte
    i = 180;
    for (int c = 304; c < 306; c++) {
        small_coins[c].pos.left_x = 820;
        small_coins[c].pos.right_x = 845;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    // 8ª coluna 3ª parte
    i = 535;
    for (int c = 306; c < 308; c++) {
        small_coins[c].pos.left_x = 825;
        small_coins[c].pos.right_x = 850;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    i = 90;
    for (int c = 308; c < 310; c++) {
        small_coins[c].pos.left_x = 460;
        small_coins[c].pos.right_x = 485;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    i = 230;
    for (int c = 310; c < 314; c++) {
        small_coins[c].pos.left_x = 460;
        small_coins[c].pos.right_x = 485;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    i = 530;
    for (int c = 321; c < 323; c++) {
        small_coins[c].pos.left_x = 460;
        small_coins[c].pos.right_x = 485;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    i = 230;
    for (int c = 314; c < 318; c++) {
        small_coins[c].pos.left_x = 550;
        small_coins[c].pos.right_x = 575;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    i = 530;
    for (int c = 318; c < 321; c++) {
        small_coins[c].pos.left_x = 560;
        small_coins[c].pos.right_x = 585;
        small_coins[c].pos.top_y = i;
        small_coins[c].pos.bottom_y = i + 25;
        i += 19;
    }

    for (int c = 0; i < 323; i++)
        small_coins[c].isCaptured = false;
}

void (set_big_coins_position)() {
    big_coins[0].pos.left_x = 193;
    big_coins[0].pos.right_x = 217;
    big_coins[0].pos.top_y = 365;
    big_coins[0].pos.bottom_y = 390;

    big_coins[1].pos.left_x = 820;
    big_coins[1].pos.right_x = 845;
    big_coins[1].pos.top_y = 363;
    big_coins[1].pos.bottom_y = 387;
    
    big_coins[2].pos.left_x = 195;
    big_coins[2].pos.right_x = 220;
    big_coins[2].pos.top_y = 65;
    big_coins[2].pos.bottom_y = 90;

    big_coins[3].pos.left_x = 820;
    big_coins[3].pos.right_x = 855;
    big_coins[3].pos.top_y = 65;
    big_coins[3].pos.bottom_y = 90;

    big_coins[4].pos.left_x = 508;
    big_coins[4].pos.right_x = 533;
    big_coins[4].pos.top_y = 698;
    big_coins[4].pos.bottom_y = 723;

    for (int i = 0; i < 5; i++)
        big_coins[i].isCaptured = false;
}
