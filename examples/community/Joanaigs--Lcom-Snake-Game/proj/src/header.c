#include "header.h"
#include "images/0.xpm"
#include "images/1.xpm"
#include "images/2.xpm"
#include "images/3.xpm"
#include "images/4.xpm"
#include "images/5.xpm"
#include "images/6.xpm"
#include "images/7.xpm"
#include "images/8.xpm"
#include "images/9.xpm"
#include "images/clock.xpm"
#include "images/heart-left.xpm"
#include "images/heart-right.xpm"
#include "objects.h"
#include "apple.h"
#include "snake.h"
#include <math.h>
#include <stdio.h>
#include <string.h>


void initHeaderElements(){
    timeClock.map = xpm_load((xpm_map_t) clock_xpm, XPM_8_8_8, &(timeClock.img));
    rightHeart.map = xpm_load((xpm_map_t) heart_right_xpm, XPM_8_8_8, &(rightHeart.img));
    leftHeart.map = xpm_load((xpm_map_t) heart_left_xpm, XPM_8_8_8, &(leftHeart.img));
    one.map = xpm_load((xpm_map_t) one_xpm, XPM_8_8_8, &(one.img));
    two.map = xpm_load((xpm_map_t) two_xpm, XPM_8_8_8, &(two.img));
    three.map = xpm_load((xpm_map_t) three_xpm, XPM_8_8_8, &(three.img));
    four.map = xpm_load((xpm_map_t) four_xpm, XPM_8_8_8, &(four.img));
    five.map = xpm_load((xpm_map_t) five_xpm, XPM_8_8_8, &(five.img));
    six.map = xpm_load((xpm_map_t) six_xpm, XPM_8_8_8, &(six.img));
    seven.map = xpm_load((xpm_map_t) seven_xpm, XPM_8_8_8, &(seven.img));
    eight.map = xpm_load((xpm_map_t) eight_xpm, XPM_8_8_8, &(eight.img));
    nine.map = xpm_load((xpm_map_t) nine_xpm, XPM_8_8_8, &(nine.img));
    zero.map = xpm_load((xpm_map_t) zero_xpm, XPM_8_8_8, &(zero.img));
}

void draw_number(int number, int x, int y) {
    int num_dig = 0;
    int n = number;
    while (n != 0) {
        num_dig++;
        n /= 10;
    }
    while (num_dig > 0) {
        int num = number;
        int digit;
        for (int i = 1; i <= num_dig; i++) {
            digit = num % 10;
            num /= 10;
        }
        switch (digit) {
            case 0:
                draw_xpm(zero.img, zero.map, x, y);
                break;
            case 1:
                draw_xpm(one.img, one.map, x, y);
                break;
            case 2:
                draw_xpm(two.img, two.map, x, y);
                break;
            case 3:
                draw_xpm(three.img, three.map, x, y);
                break;
            case 4:
                draw_xpm(four.img, four.map, x, y);
                break;
            case 5:
                draw_xpm(five.img, five.map, x, y);
                break;
            case 6:
                draw_xpm(six.img, six.map, x, y);
                break;
            case 7:
                draw_xpm(seven.img, seven.map, x, y);
                break;
            case 8:
                draw_xpm(eight.img, eight.map, x, y);
                break;
            case 9:
                draw_xpm(nine.img, nine.map, x, y);
                break;
        }
        x += 17;
        num_dig--;
    }
}

void erase_number(int number, int x, int y) {
    int num_dig = 0;
    int n = number;
    while (n != 0) {
        num_dig++;
        n /= 10;
    }
    while (num_dig > 0) {
        int num = number;
        int digit;
        for (int i = 1; i <= num_dig; i++) {
            digit = num % 10;
            num /= 10;
        }
        switch (digit) {
            case 0:
                erase_xpm(zero.img, x, y, background.img);
                break;
            case 1:
                erase_xpm(one.img, x, y, background.img);
                break;
            case 2:
                erase_xpm(two.img, x, y, background.img);
                break;
            case 3:
                erase_xpm(three.img, x, y, background.img);
                break;
            case 4:
                erase_xpm(four.img, x, y, background.img);
                break;
            case 5:
                erase_xpm(five.img, x, y, background.img);
                break;
            case 6:
                erase_xpm(six.img, x, y, background.img);
                break;
            case 7:
                erase_xpm(seven.img, x, y, background.img);
                break;
            case 8:
                erase_xpm(eight.img, x, y, background.img);
                break;
            case 9:
                erase_xpm(nine.img, x, y, background.img);
                break;
        }
        x += 17;
        num_dig--;
    }
}

void drawHearts(double number, int x, int y) {
    int temp = 1;
    for (double i = 0.5; i <= number; i += 0.5) {
        if (temp % 2) {
            draw_xpm(leftHeart.img, leftHeart.map, x, y);
            x += 20;
        } else {
            draw_xpm(rightHeart.img, rightHeart.map, x, y);
            x += 30;
        }
        temp++;
    }
}

void eraseHearts(double number, int x, int y) {
    int temp = 1;
    for (double i = 0.5; i <= number; i += 0.5) {
        if (temp % 2) {
            erase_xpm(leftHeart.img, x, y, background.img);
            x += 20;
        } else {
            erase_xpm(rightHeart.img, x, y, background.img);
            x += 30;
        }
        temp++;
    }
}

void eraseTime() {
    erase_number(gameTime, 65, 30);
}

void eraseAppleNumb() {
    erase_number(numOfApplesEaten, 445, 30);
}

void drawHeader() {
    draw_xpm(timeClock.img, timeClock.map, 20, 20);
    draw_number(gameTime, 65, 30);
    drawHearts(lives, 540, 20);
    draw_xpm(redAppleXpm.img, redAppleXpm.map, 400, 20);
    draw_number(numOfApplesEaten, 445, 30);
}
