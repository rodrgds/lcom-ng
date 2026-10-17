#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <lcom/lcf.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct{
    char minigotchi_name[11];
    int minigotchi_hunger;
    int minigotchi_happiness;
    int coins;
    int food_array[9];
} Database;

Database* getDatabase();
void setDatabase(Database* database);

Database *new_database();
int database_load_from_file(Database *database);
int database_save_to_file(Database *database);
bool database_check_file_exists();
int database_delete_file();

int database_get_hunger(Database *database);
int database_get_happiness(Database *database);
int database_get_coins(Database *database);
char* database_get_minigotchiName(Database *database);
int* database_get_foodArray(Database *database);

void database_set_hunger(Database *database, int value);
void database_set_happiness(Database *database, int value);
void database_set_coins(Database *database, int value);
void database_set_minigotchiName(Database *database, char* value);
void database_set_foodArray(Database *database, int* food_array);
void database_add_food_to_array(Database *database, int food_item);

#endif /* _DATABASE_H_ */
