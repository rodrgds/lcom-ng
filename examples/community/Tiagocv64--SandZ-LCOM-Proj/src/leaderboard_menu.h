#pragma once

/** @defgroup leaderboard_menu leaderboard_menu
 * @{
 * Functions for manipulating the leaderboard menu
 */

/**
 * @brief      Represents one highscore object.
 */
struct LeaderBoardEntry {
  int score;		/**< score of highscore */
  char date[10];	/**< date of highscore achieving */
  char name[30];	/**< name of highscore achiever */
};

/**
 * @brief      Function to be called on return button press
 */
void func_button_return();

/**
 * @brief      Init leaderboard menu structure
 */
void init_leaderboard_menu();

/**
 * @brief      Destroy leaderboard menu structure and free memory used by it
 */
void destroy_leaderboard_menu();

/**
 * @brief      Populate leaderboard with current highscores
 */
void populate_leaderboard();

/**
 * @brief      Draw leaboard with current highscores
 */
void draw_leaderboard();

/**
 * @brief      Update leaderboard menu
 */
void update_leaderboard_menu();
