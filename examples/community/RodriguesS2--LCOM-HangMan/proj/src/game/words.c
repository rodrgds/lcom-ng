#include "words.h"
#include "../assets/font.h"
#include "../peripherals/i8042.h"

KeyMap key_map[26] = {
  {0x1E, 'A'},
  {0x30, 'B'},
  {0x2E, 'C'},
  {0x20, 'D'},
  {0x12, 'E'},
  {0x21, 'F'},
  {0x22, 'G'},
  {0x23, 'H'},
  {0x17, 'I'},
  {0x24, 'J'},
  {0x25, 'K'},
  {0x26, 'L'},
  {0x32, 'M'},
  {0x31, 'N'},
  {0x18, 'O'},
  {0x19, 'P'},
  {0x10, 'Q'},
  {0x13, 'R'},
  {0x1F, 'S'},
  {0x14, 'T'},
  {0x16, 'U'},
  {0x2F, 'V'},
  {0x11, 'W'},
  {0x2D, 'X'},
  {0x15, 'Y'},
  {0x2C, 'Z'},
};

//max word size is 10 (now)
const GuessWord guesswords[] = {
    // LCOM category
    {"COMPUTER", "LCOM"},
    {"SOFTWARE", "LCOM"},
    {"TIMER", "LCOM"},
    {"TERMINAL", "LCOM"},
    {"NETWORK", "LCOM"},
    {"KEYBOARD", "LCOM"},
    {"MOUSE", "LCOM"},
    {"PROCESSOR", "LCOM"},
    {"PROGRAMMING", "LCOM"},
    {"ALGORITHM", "LCOM"},
    {"FUNCTION", "LCOM"},
    {"COMPILER", "LCOM"},

    // JOBS category
    {"TEACHER", "JOBS"},
    {"ENGINEER", "JOBS"},
    {"DOCTOR", "JOBS"},
    {"LAWYER", "JOBS"},
    {"NURSE", "JOBS"},
    {"ARCHITECT", "JOBS"},
    {"MECHANIC", "JOBS"},
    {"SCIENTIST", "JOBS"},
    {"FARMER", "JOBS"},

    // ANIMALS category
    {"ELEPHANT", "ANIMALS"},
    {"KANGAROO", "ANIMALS"},
    {"GIRAFFE", "ANIMALS"},
    {"PENGUIN", "ANIMALS"},
    {"CROCODILE", "ANIMALS"},
    {"TIGER", "ANIMALS"},
    {"DOLPHIN", "ANIMALS"},
    {"RABBIT", "ANIMALS"},
    {"SQUIRREL", "ANIMALS"},
    {"ZEBRA", "ANIMALS"},

    // COUNTRIES category
    {"PORTUGAL", "COUNTRIES"},
    {"BRAZIL", "COUNTRIES"},
    {"CANADA", "COUNTRIES"},
    {"GERMANY", "COUNTRIES"},
    {"JAPAN", "COUNTRIES"},
    {"AUSTRALIA", "COUNTRIES"},
    {"INDIA", "COUNTRIES"},
    {"EGYPT", "COUNTRIES"},
    {"NORWAY", "COUNTRIES"},
    {"MEXICO", "COUNTRIES"}
};




const int guesswords_count = sizeof(guesswords) / sizeof(guesswords[0]);
const char *guess_word;

//array of booleans to represent if the letter was already gessed
bool guessed_letters[26] = {false};
char displayed_word[MAX_WORD_LENGTH] = {0}; //aux so we can track the "word state"


void generate_guessword(const char *category) {
  // Count how many words match the category
  int count = 0;
  for (int i = 0; i < guesswords_count; i++) {
    if (strcmp(guesswords[i].category, category) == 0) {
      count++;
    }
  }

  // If no words found, fallback
  if (count == 0) return;

  // Pick a random index among matching words
  int random_index = rand() % count;
  int idx = 0;
  for (int i = 0; i < guesswords_count; i++) {
    if (strcmp(guesswords[i].category, category) == 0) {
      if (idx == random_index) {
        guess_word = guesswords[i].word;
        break;
      }
      idx++;
    }
  }

  // Initialize displayed_word with '_'
  int word_size = strlen(guess_word);
  for (int i = 0; i < word_size; i++) {
    displayed_word[i] = '_';
  }
  displayed_word[word_size] = '\0';
}


char scancode_to_letter(uint8_t scancode) {
  
  for (int i = 0; i < 26; i++) {  
    
    if (key_map[i].scancode == scancode) {
      return key_map[i].letter;
    }
  }
  
  return 0;  
}

