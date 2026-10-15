#include "name_input.h"

NameInput* nameInput;

char* nameBackup;

void create_name(){

  nameInput = malloc(sizeof(NameInput));
  nameInput->limit = 8;
  nameInput->name = malloc(nameInput->limit * sizeof(char));
  nameInput->name[0] = '\0';
  nameInput->x = 500;
  nameInput->y = 400;
  nameInput->sprite = alphabet;
}

void destroy_name(){

  nameBackup = malloc(sizeof(char) * nameInput->limit);
  sprintf(nameBackup, "%s", nameInput->name);

  free(nameInput->name);
}

void add_letter(char letter){

  if (letter == ' ') {
    return;
  }
  unsigned int i = 0;
  while (nameInput->name[i] != '\0') {
    i++;
  }

  if (i < nameInput->limit - 1) {
    nameInput->name[i] = letter;
    nameInput->name[i + 1] = '\0';
  }
}

void delete_letter(){

  unsigned int i = 0;
  while (nameInput->name[i] != '\0') {
    i++;
  }
  if (i > 0) {
    nameInput->name[i - 1] = '\0';
  }
}
