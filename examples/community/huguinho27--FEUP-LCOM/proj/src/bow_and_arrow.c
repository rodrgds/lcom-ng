#include "bow_and_arrow.h"
#include "sprite.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "mouse.h"
#include "pixmap.h"
#include "utils.h"
#include "kbd.h"
#include "timer.h"
#include <math.h>
#include "hero.h"
#include "Bitmap.h"
#include "Balloon.h"
#include "arrow.h"
#include "video_gr.h"
#include <string.h>
#include "rtc.h"

balloon_t *balloons_1[BALLOONS_FIRST_FASE];
balloon_t *balloons_2[BALLOONS_SECOND_FASE];
arrow_t *arrows[STARTING_ARROWS + SECOND_LEVEL_ARROW_NUMBER];

static Bitmap *second_level_image;
static Bitmap *win_screen_image;
static Bitmap *lose_screen_image;
static Bitmap *arrow_display_image;
static Bitmap *score_image;

/* --------------------------------- DISPLAY -------------------------------------*/

void showSecondLevelScreen()
{
	drawBitmap(second_level_image, 512, 274, ALIGN_CENTER);
}

void showHeroWinScreen()
{
	drawBitmap(win_screen_image, 512, 274, ALIGN_CENTER);
}

void showHeroLoseScreen()
{
	drawBitmap(lose_screen_image, 512, 274, ALIGN_CENTER);
}

void showArrowNumber()
{
	drawBitmap(arrow_display_image, 900, 17, ALIGN_LEFT);
}

void showScore()
{
	drawBitmap(score_image, 20, 23, ALIGN_LEFT);
}

/* --------------------------------- BALLOON -------------------------------------*/

int checkIfAnyBalloonIntact_firstlevel()
{
	int i;
	for (i = 0; i < BALLOONS_FIRST_FASE; i++)
	{
		if (isBalloonIntact(balloons_1[i]))
			return 1;
	}
	return 0;
}

int checkIfAnyBalloonIntact_secondlevel()
{
	int i;
	for (i = 0; i < BALLOONS_SECOND_FASE; i++)
	{
		if (isBalloonIntact(balloons_2[i]))
			return 1;
	}
	return 0;
}

void blowAllBalloonsFirstLevel()
{
	int i;
	for (i = 0; i < BALLOONS_FIRST_FASE; i++)
	{
		setBalloonHit(balloons_1[i]);
	}
}

void blowAllBalloonsSecondLevel()
{
	int i;
	for (i = 0; i < BALLOONS_SECOND_FASE; i++)
	{
		setBalloonHit(balloons_2[i]);
	}
}

void blowCurrentLevelBalloons()
{
	switch (current_game_level)
	{
	case FIRST_STAGE:
		blowAllBalloonsFirstLevel();
		break;

	case SECOND_STAGE:
		blowAllBalloonsSecondLevel();
		break;
	}
}

int checkAllBalloonsDestroyed_firstlevel()
{
	int i;
	for (i = 0; i < BALLOONS_FIRST_FASE; i++)
	{
		if (!isBalloonDestroyed(balloons_1[i]))
			return 0;
	}
	return 1;
}

int checkAllBalloonsDestroyed_secondlevel()
{
	int i;
	for (i = 0; i < BALLOONS_SECOND_FASE; i++)
	{
		if (!isBalloonDestroyed(balloons_2[i]))
			return 0;
	}
	return 1;
}

void initializeBalloons_firstLevel()
{
	balloons_1[0] = createBalloon(450, 900);
	balloons_1[1] = createBalloon(476, 900);
	balloons_1[2] = createBalloon(502, 900);
	balloons_1[3] = createBalloon(528, 900);
	balloons_1[4] = createBalloon(554, 900);
	balloons_1[5] = createBalloon(580, 900);
	balloons_1[6] = createBalloon(606, 900);
	balloons_1[7] = createBalloon(632, 900);
	balloons_1[8] = createBalloon(658, 900);
	balloons_1[9] = createBalloon(684, 900);
	balloons_1[10] = createBalloon(710, 900);
	balloons_1[11] = createBalloon(736, 900);
	balloons_1[12] = createBalloon(762, 900);
	balloons_1[13] = createBalloon(788, 900);
	balloons_1[14] = createBalloon(814, 900);
}

void initializeBalloons_secondLevel()
{
	balloons_2[0] = createBalloon(450, 900);
	balloons_2[1] = createBalloon(476, 900);
	balloons_2[2] = createBalloon(502, 900);
	balloons_2[3] = createBalloon(528, 900);
	balloons_2[4] = createBalloon(554, 900);
	balloons_2[5] = createBalloon(580, 900);
	balloons_2[6] = createBalloon(606, 900);
	balloons_2[7] = createBalloon(632, 900);
	balloons_2[8] = createBalloon(658, 900);
	balloons_2[9] = createBalloon(684, 900);
	balloons_2[10] = createBalloon(710, 900);
	balloons_2[11] = createBalloon(736, 900);
	balloons_2[12] = createBalloon(762, 900);
	balloons_2[13] = createBalloon(788, 900);
	balloons_2[14] = createBalloon(814, 900);
}

void freeMemoryFromBalloons()
{
	int i;
	for (i = 0; i < BALLOONS_SECOND_FASE; i++)
	{
		free(balloons_2[i]);
	}

	for (i = 0; i < BALLOONS_FIRST_FASE; i++)
	{
		free(balloons_1[i]);
	}
}

/* --------------------------------- ARROW -------------------------------------*/

void freeMemoryFromArrows()
{
	int i;
	for (i = 0; i < arrow_size; i++)
	{
		free(arrows[i]);
	}
}

int isAnArrowInsideBalloonArea(int areaX, int areaY, int pointX, int pointY)
{
	if (pointX > (areaX - 16) && pointY >= (areaY - 2) && pointX < (areaX + 16) && pointY <= areaY + 22)
		return 1;
	return 0;
}

int checkIfAnyArrowInsideScreen_firstlevel()
{
	int i;
	for (i = 0; i < arrow_size; i++)
	{
		if (isArrowInsideScreen(arrows[i]))
			return 1;
	}
	return 0;
}

int checkIfAnyArrowInsideScreen_secondlevel()
{
	int i;
	for (i = STARTING_ARROWS; i < arrow_size; i++)
	{
		if (isArrowInsideScreen(arrows[i]))
			return 1;
	}
	return 0;
}

void addNewArrowToArray(hero_t *h)
{
	arrows[arrow_size] = createArrow(getHeroXPos(h) + 53, getHeroYPos(h) + 43);
	arrow_size++;
}

void updateArrowNumber(hero_t *h)
{
	switch (getNumberOfArrows(h))
	{
	case 0:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num0.bmp");
		break;
	case 1:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num1.bmp");
		break;
	case 2:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num2.bmp");
		break;
	case 3:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num3.bmp");
		break;
	case 4:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num4.bmp");
		break;
	case 5:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num5.bmp");
		break;
	case 6:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num6.bmp");
		break;
	case 7:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num7.bmp");
		break;
	case 8:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num8.bmp");
		break;
	case 9:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num9.bmp");
		break;
	case 10:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num10.bmp");
		break;
	case 11:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num11.bmp");
		break;
	case 12:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num12.bmp");
		break;
	case 13:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num13.bmp");
		break;
	case 14:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num14.bmp");
		break;
	case 15:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num15.bmp");
		break;
	case 16:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num16.bmp");
		break;
	case 17:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num17.bmp");
		break;
	case 18:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num18.bmp");
		break;
	case 19:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num19.bmp");
		break;
	case 20:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num20.bmp");
		break;
	case 21:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num21.bmp");
		break;
	case 22:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num22.bmp");
		break;
	case 23:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num23.bmp");
		break;
	case 24:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num24.bmp");
		break;
	case 25:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num25.bmp");
		break;
	case 26:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num26.bmp");
		break;
	case 27:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num27.bmp");
		break;
	case 28:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num28.bmp");
		break;
	case 29:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num29.bmp");
		break;
	case 30:
		arrow_display_image = loadBitmap("/home/lcom/proj/src/resources/num30.bmp");
		break;
	}
}

/* --------------------------------- LOGIC , UPDATE , PERIPHERALS -------------------------------------*/

void collisionDetection(hero_t *h)
{
	int i, j;
	switch (current_game_level)
	{
	case FIRST_STAGE:
		for (j = 0; j < arrow_size; j++)
		{
			if (!isArrowInsideScreen(arrows[j]))
			{
				continue;
			}
			for (i = 0; i < BALLOONS_FIRST_FASE; i++)
			{
				if (isBalloonDestroyed(balloons_1[i]) || isBalloonBlown(balloons_1[i]))
					continue;
				if (isAnArrowInsideBalloonArea(getBalloonPos_X(balloons_1[i]), getBalloonPos_Y(balloons_1[i]), getArrowPos_X(arrows[j]) + 25,
						getArrowPos_Y(arrows[j]) + 2))
				{
					setBalloonHit(balloons_1[i]);
					heroDestroyedBalloon(h);
				}
			}
		}
		break;
	case SECOND_STAGE:
		for (j = 0; j < arrow_size; j++)
		{
			if (!isArrowInsideScreen(arrows[j]))
			{
				continue;
			}
			for (i = 0; i < BALLOONS_SECOND_FASE; i++)
			{
				if (isBalloonDestroyed(balloons_2[i]) || isBalloonBlown(balloons_2[i]))
					continue;
				if (isAnArrowInsideBalloonArea(getBalloonPos_X(balloons_2[i]), getBalloonPos_Y(balloons_2[i]), getArrowPos_X(arrows[j]) + 25,
						getArrowPos_Y(arrows[j]) + 2))
				{
					setBalloonHit(balloons_2[i]);
					heroDestroyedBalloon(h);
				}
			}
		}
		break;
	}
}

void updateScore(hero_t *h)
{
	switch (getNumberOfBalloonsDestroyed(h))
	{
	case 0:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score0.bmp");
		break;
	case 1:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score1.bmp");
		break;
	case 2:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score2.bmp");
		break;
	case 3:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score3.bmp");
		break;
	case 4:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score4.bmp");
		break;
	case 5:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score5.bmp");
		break;
	case 6:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score6.bmp");
		break;
	case 7:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score7.bmp");
		break;
	case 8:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score8.bmp");
		break;
	case 9:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score9.bmp");
		break;
	case 10:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score10.bmp");
		break;
	case 11:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score11.bmp");
		break;
	case 12:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score12.bmp");
		break;
	case 13:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score13.bmp");
		break;
	case 14:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score14.bmp");
		break;
	case 15:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score15.bmp");
		break;
	case 16:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score16.bmp");
		break;
	case 17:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score17.bmp");
		break;
	case 18:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score18.bmp");
		break;
	case 19:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score19.bmp");
		break;
	case 20:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score20.bmp");
		break;
	case 21:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score21.bmp");
		break;
	case 22:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score22.bmp");
		break;
	case 23:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score23.bmp");
		break;
	case 24:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score24.bmp");
		break;
	case 25:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score25.bmp");
		break;
	case 26:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score26.bmp");
		break;
	case 27:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score27.bmp");
		break;
	case 28:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score28.bmp");
		break;
	case 29:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score29.bmp");
		break;
	case 30:
		score_image = loadBitmap("/home/lcom/proj/src/resources/score30.bmp");
		break;
	}
}

int checkIfLoseGame(hero_t *h)
{
	if (getNumberOfArrows(h) == 0)
	{
		switch (current_game_level)
		{
		case FIRST_STAGE:
			if (!checkIfAnyArrowInsideScreen_firstlevel())
			{
				if (checkIfAnyBalloonIntact_firstlevel())
				{
					current_game_level = LOST;
				}
			}
			break;
		case SECOND_STAGE:
			if (!checkIfAnyArrowInsideScreen_secondlevel())
			{
				if (checkIfAnyBalloonIntact_secondlevel())
				{
					current_game_level = LOST;
				}
			}
			break;
		}
	}
}

void checkNextLevel(hero_t *h)
{

	switch (current_game_level)
	{
	case FIRST_STAGE:
		if (checkAllBalloonsDestroyed_firstlevel())
		{
			current_game_level = 2;
			setHeroArrowNumber(h, SECOND_LEVEL_ARROW_NUMBER);
			setHeroRestingWithoutArrow(h);
		}
		break;
	case SECOND_STAGE:
		if (checkAllBalloonsDestroyed_secondlevel())
		{
			current_game_level = WON;
		}
		break;
	}
}

void mouseMovement(hero_t *h)
{
	int aux = mouse_handler();
	if (aux == 0)
	{
		if (isCorrectMovement())
		{
			if (heroMovementInsideScreen(h))
			{
				int oldCounter;
				updateHeroPosition(h);
				if (isHeroRestingWithoutArrow(h) && isRightButtonPressed())
				{
					if (timer_counter >= 35)
					{
						if (getNumberOfArrows(h) > 0)
							setHeroRestingWithArrow(h);
						else
							setHeroRestingWithoutArrow(h);
					}
				}
			}
		}
	}
}

void updateScreen(hero_t *h)
{
	collisionDetection(h);
	clearScreen();
	drawHero(h);
	unsigned long seconds;

	int i;
	switch (current_game_level)
	{
	case FIRST_STAGE:
		for (i = 0; i < BALLOONS_FIRST_FASE; i++)
		{
			updateBalloonPosition(balloons_1[i]);
			resetBalloonPosition(balloons_1[i]);
			drawBalloon(balloons_1[i]);
		}
		break;
	case SECOND_STAGE:
		seconds = readSecondsFromRTC();
		seconds = convertBCD(seconds);

		switch (seconds)
		{
		case 0:
			setBalloonActive(balloons_2[0]);
			setBalloonActive(balloons_2[6]);
			setBalloonActive(balloons_2[10]);
			break;
		case 2:
			setBalloonActive(balloons_2[10]);
			setBalloonActive(balloons_2[5]);
			setBalloonActive(balloons_2[14]);
			break;
		case 4:
			setBalloonActive(balloons_2[5]);
			setBalloonActive(balloons_2[11]);
			setBalloonActive(balloons_2[4]);
			break;
		case 5:
			setBalloonActive(balloons_2[3]);
			setBalloonActive(balloons_2[12]);
			setBalloonActive(balloons_2[9]);
		case 6:
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[14]);
			break;
		case 8:
			setBalloonActive(balloons_2[14]);
			setBalloonActive(balloons_2[7]);
			setBalloonActive(balloons_2[12]);
			break;
		case 10:
			setBalloonActive(balloons_2[2]);
			setBalloonActive(balloons_2[9]);
			setBalloonActive(balloons_2[3]);
			break;
		case 12:
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[4]);
			break;
		case 14:
			setBalloonActive(balloons_2[8]);
			setBalloonActive(balloons_2[12]);
			setBalloonActive(balloons_2[7]);
			break;
		case 15:
			setBalloonActive(balloons_2[13]);
			setBalloonActive(balloons_2[10]);
			setBalloonActive(balloons_2[5]);
			break;
		case 16:
			setBalloonActive(balloons_2[0]);
			setBalloonActive(balloons_2[6]);
			break;
		case 18:
			setBalloonActive(balloons_2[10]);
			setBalloonActive(balloons_2[5]);
			break;
		case 20:
			setBalloonActive(balloons_2[5]);
			setBalloonActive(balloons_2[11]);
			setBalloonActive(balloons_2[13]);
			break;
		case 22:
			setBalloonActive(balloons_2[3]);
			setBalloonActive(balloons_2[8]);
		case 24:
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[14]);
			break;
		case 25:
			setBalloonActive(balloons_2[14]);
			setBalloonActive(balloons_2[7]);
			setBalloonActive(balloons_2[2]);
			break;
		case 26:
			setBalloonActive(balloons_2[2]);
			setBalloonActive(balloons_2[9]);
			break;
		case 28:
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[4]);
			break;
		case 30:
			setBalloonActive(balloons_2[8]);
			setBalloonActive(balloons_2[12]);
			setBalloonActive(balloons_2[0]);
			break;
		case 32:
			setBalloonActive(balloons_2[13]);
			setBalloonActive(balloons_2[5]);
			break;
		case 34:
			setBalloonActive(balloons_2[0]);
			setBalloonActive(balloons_2[6]);
			break;
		case 35:
			setBalloonActive(balloons_2[10]);
			setBalloonActive(balloons_2[5]);
			setBalloonActive(balloons_2[13]);
			break;
		case 36:
			setBalloonActive(balloons_2[5]);
			setBalloonActive(balloons_2[11]);
			break;
		case 38:
			setBalloonActive(balloons_2[3]);
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[7]);
		case 40:
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[14]);
			setBalloonActive(balloons_2[6]);
			break;
		case 42:
			setBalloonActive(balloons_2[14]);
			setBalloonActive(balloons_2[7]);
			break;
		case 44:
			setBalloonActive(balloons_2[2]);
			setBalloonActive(balloons_2[9]);
			break;
		case 45:
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[4]);
			setBalloonActive(balloons_2[10]);
			break;
		case 46:
			setBalloonActive(balloons_2[8]);
			setBalloonActive(balloons_2[12]);
			break;
		case 48:
			setBalloonActive(balloons_2[13]);
			setBalloonActive(balloons_2[1]);
			break;
		case 50:
			setBalloonActive(balloons_2[0]);
			setBalloonActive(balloons_2[6]);
			setBalloonActive(balloons_2[3]);
			break;
		case 52:
			setBalloonActive(balloons_2[10]);
			setBalloonActive(balloons_2[5]);
			break;
		case 54:
			setBalloonActive(balloons_2[5]);
			setBalloonActive(balloons_2[11]);
			setBalloonActive(balloons_2[12]);
			break;
		case 55:
			setBalloonActive(balloons_2[3]);
			setBalloonActive(balloons_2[2]);
			setBalloonActive(balloons_2[9]);
		case 56:
			setBalloonActive(balloons_2[1]);
			setBalloonActive(balloons_2[14]);
			break;
		case 58:
			setBalloonActive(balloons_2[14]);
			setBalloonActive(balloons_2[7]);
			break;
		}

		for (i = 0; i < BALLOONS_SECOND_FASE; i++)
		{
			if (isBalloonActive(balloons_2[i]))
			{
				updateBalloonPosition(balloons_2[i]);
				resetBalloonPosition(balloons_2[i]);
				drawBalloon(balloons_2[i]);
			}
		}
	}

	if (getNumberOfArrows(h) == 0)
		setHeroWithoutAnyArrow(h);

	for (i = 0; i < arrow_size; i++)
	{
		if (isArrowInsideScreen(arrows[i]))
		{
			updateArrowPosition(arrows[i]);
			drawArrow(arrows[i]);
		}
	}

	if (next_level_timer <= 120 && current_game_level == SECOND_STAGE)
		showSecondLevelScreen();

	if (end_game_timer <= 300 && current_game_level == WON)
		showHeroWinScreen();

	if (end_game_timer <= 300 && current_game_level == LOST)
		showHeroLoseScreen();

	topInfoBar();
	updateArrowNumber(h);
	showArrowNumber();
	updateScore(h);
	showScore();
	changeVideoMem();
}

int keyboardUpdate(hero_t *h)
{
	unsigned long scancode = kbd_handler();
	if (scancode == ESC_BREAK_CODE)
		return -1;

	if (scancode == SPACE_MAKE_CODE)
	{
		if (isHeroFullPower(h))
		{
			clearScreen();
			drawHero(h);
		} else if (isHeroRestingWithArrow(h))
		{
			setHeroPoweringUp(h);
			clearScreen();
			drawHero(h);
			tickdelay(micros_to_ticks(150000));
			setHeroFullPower(h);
			clearScreen();
			drawHero(h);
		}
	}

	if (scancode == SPACE_BREAK_CODE && isHeroFullPower(h))
	{
		if (getNumberOfArrows(h) > 0)
		{
			releaseArrow(h);
			addNewArrowToArray(h);
			timer_counter = 0;
		}
		setHeroRestingWithoutArrow(h);
	}

	if (scancode == PAGEUP_MAKE_CODE)
		blowCurrentLevelBalloons();

	return 0;
}

/* --------------------------------------------------------------------------------*/

void initializeGameSettings()
{
	arrow_size = 0;
	timer_counter = 0;
	current_game_level = FIRST_STAGE;
	next_level_timer = 0;
	end_game_timer = 0;

	initializeBalloons_firstLevel();
	initializeBalloons_secondLevel();
	resetPackets();
	second_level_image = loadBitmap("/home/lcom/proj/src/resources/2nd_stage_prega.bmp");
	win_screen_image = loadBitmap("/home/lcom/proj/src/resources/you_won_prega.bmp");
	lose_screen_image = loadBitmap("/home/lcom/proj/src/resources/you_lost_prega.bmp");
	score_image = loadBitmap("/home/lcom/proj/src/resources/you_lost_prega.bmp");
}

int gameLoop()
{
	initializeGameSettings();

	int ipc_status, r;
	message msg;

	/* MOUSE */
	int mouse_irq_set = BIT(MOUSE_NOTIFICATION);
	mouse_subscribe_int();
	mouseWrite(SET_STREAM_MODE);
	mouseWrite(ENABLE_STREAM_MODE);

	/* TIMER */
	int timer_irq_set = BIT(TIMER_NOTIFICATION);
	timer_subscribe_int();

	/* KEYBOARD */
	int kbd_irq_set = BIT(KBD_NOTIFICATION);
	kbd_subscribe_int();

	hero_t *h;
	h = createHero();

	while (1)
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			if (_ENDPOINT_P(msg.m_source) == HARDWARE)
			{
				/* MOUSE */
				if (msg.NOTIFY_ARG & mouse_irq_set)
				{
					mouseMovement(h);
				}

				/* TIMER */
				if (msg.NOTIFY_ARG & timer_irq_set)
				{
					updateScreen(h);
					timer_counter++;

					if (current_game_level == FIRST_STAGE)
					{
						checkNextLevel(h);
						checkIfLoseGame(h);
					} else if (current_game_level == SECOND_STAGE)
					{
						next_level_timer++;
						checkNextLevel(h);
						checkIfLoseGame(h);
					} else if (current_game_level == WON || current_game_level == LOST)
					{
						end_game_timer++;
						if (end_game_timer > 300)
						{
							break;
						}
					}
				}

				/* KEYBOARD */
				if (msg.NOTIFY_ARG & kbd_irq_set)
				{
					if (keyboardUpdate(h) == -1)
					{
						int res = pauseMenu();
						if (res == 2)
							break;
					}
				}
			}
		}
	}
	freeMemoryFromBalloons();
	freeMemoryFromArrows();

	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	mouseWrite(DISABLE_STREAM_MODE);
	deleteHero(h);
	mouse_unsubscribe_int();
	return 0;
}
