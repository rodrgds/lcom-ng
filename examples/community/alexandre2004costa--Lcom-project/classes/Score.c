#include "Score.h"

/** @defgroup Score Score
    * @{
 */

int finalScore = 0;
Sprite* numbers;
Sprite* alphabet;
Sprite* smallerNumbers;
Sprite* slash;
Sprite* hyphen;
Sprite* colon;


/**
 * @brief Increments finalScore by s.
 * @param s Value to be added to the score.
 */
void update_score(int s) {
    finalScore += s;
}


// Functions to add a new result and sort all written results


/**
 * @brief Reads each game results line by line.
 * @param filename The file which will be read.
 * @param results An array that will save each line in the form of GameResult objects.
 * @param count Reference to the number of read lines.
 */
void readGameResults(const char *filename, GameResult results[], int *count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[100]; 
    *count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL && *count < MAX_RESULTS) {
        int year, month, day, hour, minute, second, score;
        sscanf(buffer, "Date of game: %d/%d/%d at %d:%d:%d. Score: %d",
               &year, &month, &day, &hour, &minute, &second, &score);

        results[*count].date[0] = year;
        results[*count].date[1] = month;
        results[*count].date[2] = day;
        results[*count].date[3] = hour;
        results[*count].date[4] = minute;
        results[*count].date[5] = second;
        results[*count].score = score;

        (*count)++;
    }

    fclose(file);
}


/**
 * @brief Compares the score of two games.
 * @param a One game.
 * @param b Another game.
 * @return Score difference between games.
 */
int compareScores(const void *a, const void *b) {
    GameResult *gameA = (GameResult *)a;
    GameResult *gameB = (GameResult *)b;
    return gameB->score - gameA->score;
}


/**
 * @brief Writes the date and the score of each game line by line.
 * @param filename The file which will be read.
 * @param results An array of GameResult objects representing each game.
 * @param count Number of read lines.
 */
void writeGameResults(const char *filename, GameResult results[], int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "Date of game: %02d/%02d/%02d at %02d:%02d:%02d. Score: %d\n",
                results[i].date[0], results[i].date[1], results[i].date[2],
                results[i].date[3], results[i].date[4], results[i].date[5], results[i].score);
    }

    fclose(file);
}


/**
 * @brief Writes scores and dates of 9 games sorted by descending order of scores.
 * @see readGameResults()
 * @see writeGameResults()
 */
void updateGameResults() {
    int newDate[6] = {realTime.year, realTime.month, realTime.day, realTime.hour, realTime.minute, realTime.second};
    const char *filename = "home/lcom/labs/proj/game_results.txt";
    GameResult results[MAX_RESULTS + 1]; // +1 to temporarily hold the new result
    int count = 0;

    readGameResults(filename, results, &count);

    // Add the new result
    if (count < MAX_RESULTS) {
        count++;
    }
    results[count - 1].date[0] = newDate[0];
    results[count - 1].date[1] = newDate[1];
    results[count - 1].date[2] = newDate[2];
    results[count - 1].date[3] = newDate[3];
    results[count - 1].date[4] = newDate[4];
    results[count - 1].date[5] = newDate[5];
    results[count - 1].score = finalScore;

    // Sort results
    qsort(results, count, sizeof(GameResult), compareScores);

    // Write back top results 
    if (count > MAX_RESULTS) {
        count = MAX_RESULTS;
    }
    writeGameResults(filename, results, count);
}



// Functions to show date. Look at "drawFinals" for explanation.


/**
 * @brief Creates a sprite for each xpm.
 * @see create_sprite() for used method of creating a sprite.
 */
void loadScoreSprites() {
    numbers = create_sprite((xpm_map_t) numbers_xpm, 0, 0);
    alphabet = create_sprite((xpm_map_t) alphabet_xpm, 0, 0);
    smallerNumbers = create_sprite((xpm_map_t) smallerNumbers_xpm, 0, 0);
    slash = create_sprite((xpm_map_t) slash_xpm, 0, 0);
    hyphen = create_sprite((xpm_map_t) hyphen_xpm, 0, 0);
    colon = create_sprite((xpm_map_t) colon_xpm, 0, 0);
}

/**
 * @brief Draws the desired digit in the desired position. 
 * @param sp Reference to the sprite from which the digit will be selected. Used to differentiate between xpms. 
 * @param digit Desired digit.
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @param dist Adjustment based on sprite.
 * @return Horizontal displacement.
 * @see drawMultipleSprite() to understand how division of the total xpm works.
 */
int drawDigit(Sprite* sp, int digit, int x, int y, int dist) {
    sp->x = x;
    if (digit < 5) {
        sp->y = y;
        drawMultipleSprite(digit, 5, 2, sp);
    } else {
        sp->y = y - dist;
        drawMultipleSprite(digit, 5, 2, sp);
    }

    return x + DIST_NUMS;
}

/**
 * @brief Draws the desired number in the desired position using drawDigit.
 * @param sp Reference to the sprite from which the digit will be selected. Used to differentiate between xpms. 
 * @param num Desired number.
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @param distx Difference in width between digits.
 * @param adjust Adjustment based on sprite.
 * @return Displacement on the x axis.
 * @see drawDigit() for single digit drawing.
 */
int drawNumber(Sprite* sp, int num, int x, int y, int distx, int adjust) {
    int numDigits = log10(num) + 1; 
    int divisor = pow(10, numDigits - 1);

    while (divisor > 0) {
        int digit = num / divisor;
        drawDigit(sp, digit, x, y, adjust);
        x += distx; 
        num %= divisor;
        divisor /= 10;
    }

    return x;  // return the displacement on the x axis
}

/**
 * @brief Draws the desired letter in the desired position.
 * @param letter Letter to draw.
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @see drawMultipleSprite() to understand how division of the total xpm works.
 */
void drawLetter(char letter, int x, int y) {
    int l = letter - 'a';
    alphabet->x = x;
    if (l < 7) {
        alphabet->y = y;
        drawMultipleSprite(l, 7, 4, alphabet);
    } else if (l < 14) {
        alphabet->y = y - 3;
        drawMultipleSprite(l, 7, 4, alphabet);
    } else if (l < 21) {
        alphabet->y = y - 8;
        drawMultipleSprite(l, 7, 4, alphabet);
    } else {
        alphabet->y = y - 13;
        drawMultipleSprite(l, 7, 4, alphabet);
    }
}

/**
 * @brief Draws the desired letter in the desired position using drawLetter.
 * @param string Array holding all chars of the desired string.
 * @param size Size of the array.
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @return Displacement on the x axis.
 * @see drawLetter() for single letter drawing.
 */
int drawString(char string[], int size, int x, int y) {

    for (int i = 0; i < size; i++) {
        drawLetter(string[i], x, y);
        x += 90;
        if (i == size / 2 - 1) x += 20;
    }

   return x;  
}

/**
 * @brief Draws a special character in the desired position.
 * @param sp Sprite to draw.
 * @param x Position on the x axis.
 * @param y Position on the y axis.
 * @see drawSprite() to understand how sprite drawing works.
 */
int drawSpecialCharacter(Sprite* sp, int x, int y) {
    sp->x = x;
    sp->y = y;
    drawSprite(sp);
    return x;
}

/**
 * @brief Draws the score when the game is over.
 * @details Call this function whenever you want to show date and hour on the screen.
 * @see drawString() for a word drawing.
 * @see drawNumber() for a number drawing.
 */
void drawScoreGameOver(int score) {

    char endScore[5] = {'s', 'c', 'o', 'r', 'e'};
    drawString(endScore, 5, 370, 40); // x = 80; y = 70;

    drawNumber(numbers, score, 450, 200, 90, 20); // x = 450; y = 200;

    finalScore = 0;
}

/**
 * @brief Draws the score and date of a certain game result.
 * @see drawNumber() for a number drawing.
 * @see drawDigit() for single digit drawing.
 * @details Auxiliary function for each line of the file.
 */
void drawDate(GameResult result, int x, int y) {

    x = drawNumber(smallerNumbers, result.score, x, y, DIST_NUMS, ADJUST_NUMS) + 10;
    x = drawSpecialCharacter(hyphen, x - 30, y - 40) + 100;
    x = 400;

    for (int i = 0; i < 6; i++) {

        if (result.date[i] < 10) {
            x = drawDigit(smallerNumbers, 0, x, y, ADJUST_NUMS);
        } 

        x = drawNumber(smallerNumbers, result.date[i], x, y, DIST_NUMS, ADJUST_NUMS); 
        if (i < 2) x = drawSpecialCharacter(slash, x, y) + 30;
        else if (i == 2) x += 30;
        else if (i < 5) {
            x = drawSpecialCharacter(colon, x - 55, y - 40) + 85;
        }
    }

}

/**
 * @brief Draws the score and date of all game results.
 * @see readGameResults()
 * @see drawDate() to see score and date drawings.
 * @details Reads all lines and prints each.
 */
void drawScoreGameScore() {
    const char *filename = "home/lcom/labs/proj/game_results.txt";
    GameResult results[MAX_RESULTS + 1]; 
    int count = 0;

    readGameResults(filename, results, &count);

    int x = 165;
    int y = 150;

    for (int i = 0; i < count; i++) {
        drawDate(results[i], x, y);
        y += 62;
    }
}

/**
 * @brief Draws the score number during game play.
 */
void drawScoreGamePlay() {
    drawNumber(smallerNumbers, finalScore, 500, 20, DIST_NUMS, ADJUST_NUMS);
}

/**@}*/





