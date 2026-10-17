#include "database.h"

const char* filePath = "/usr/save.txt";
static Database *_database;

Database* getDatabase() {
    return _database;
}

void setDatabase(Database* database) {
    _database = database;
}

/**
 * @brief Creates a new database "object" and returns a pointer to it
 */
Database *new_database(){
    Database *database = malloc(sizeof(*database));

    if (database == NULL) return NULL;
    
    int food_array[] = {0,0,0,0,0,0,0,0,0}; 

    char* tempName = "MINIGOTCHI";
    memcpy(database->minigotchi_name, tempName, 11*sizeof(char));

    database->minigotchi_hunger = 1000;
    database->minigotchi_happiness = 1000; 
    database->coins = 0;
    memcpy(database->food_array, food_array, sizeof(food_array));


    _database = database;
    return database;
}

/**
 * @brief Loads the player data from the save file specified in filePath 
 */
int database_load_from_file(Database *database){
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }

    // Variables to store the data
    char name[11]; 
    int hunger, happiness, coins;
    int food_array[9];

    // Read Minigotchi name
    if (fgets(name, sizeof(name), file) != NULL) {
        printf("Name: %s\n", name);
    } else {
        printf("Error reading minigotchi name");
        fclose(file);
        return 1;
    }

    // Read Minigotchi hunger status
    if (fscanf(file, "%d", &hunger) == 1) {
        printf("Hunger: %d\n", hunger);
    } else {
        printf("Error reading hunger status");
        fclose(file);
        return 1;
    }

    // Read Minigotchi happiness status
    if (fscanf(file, "%d", &happiness) == 1) {
        //database_set_happiness(_database, 2);
        printf("Happiness: %d\n", happiness);
    } else {
        printf("Error reading happiness status");
        fclose(file);
        return 1;
    }

    // Read player coins
    if (fscanf(file, "%d", &coins) == 1) {
        printf("Coins: %d\n", coins);
    } else {
        printf("Error reading coins");
        fclose(file);
        return 1;
    }

    // Read food array
    for (int i = 0; i < 9; i++){
        if (fscanf(file, "%d", &food_array[i]) == 1) {
            printf("Food %u: %d\n", i, food_array[i]);
        } else {
            printf("Error reading food %u\n", i);
            fclose(file);
            return 1;
        }
    }

    database_set_minigotchiName(database, name);
    database_set_happiness(database, happiness);
    database_set_hunger(database, hunger);
    database_set_coins(database, coins);
    database_set_foodArray(database, food_array);

    fclose(file);
    return 0;
}

/**
 * @brief Write the player data to the file to resume from where he started, the next time he opens the game 
 */
int database_save_to_file(Database *db){
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }

    fprintf(file, "%s\n", db->minigotchi_name);
    fprintf(file, "%d\n", db->minigotchi_hunger);
    fprintf(file, "%d\n", db->minigotchi_happiness);
    fprintf(file, "%d\n", db->coins);
    for (int i = 0; i < 9; i++){
        fprintf(file, "%d\n", db->food_array[i]);
    }

    return 0;
}

/**
 * @brief Check if the player data file already exists. Creates the save file in case it doesn't exist
 */
bool database_check_file_exists(){
    if (access(filePath, F_OK) != 0) {

        FILE *file = fopen(filePath, "w");
        if (file == NULL) {
            printf("Error creating file\n");
            return false;
        }

        // Write default values to the file
        fprintf(file, "MINIGOTCHI\n");    // Default name
        fprintf(file, "%d\n", 1000);  // Default hunger
        fprintf(file, "%d\n", 1000);  // Default happiness
        fprintf(file, "%d\n", 0);  // Default coins
        for (int i = 0; i < 9; i++){
            fprintf(file, "%d\n", 0);  // Default food
        }

        fclose(file);

        return false;
    } 
    else{
        return true;
    }
    return false;
}

/**
 * @brief Deletes the player save state (in case new game creation is cancelled)
 */
int database_delete_file(){
    if (remove(filePath) != 0) {
        printf("Error deleting save file");
        return 1;
    }
    return 0;
}


// Getters and Setters

int database_get_hunger(Database *db) {
    return db->minigotchi_hunger;
}
int database_get_happiness(Database *db) {
    return db->minigotchi_happiness;
}
int database_get_coins(Database *db) {
    return db->coins;
}
char* database_get_minigotchiName(Database *db) {
    return db->minigotchi_name;
}

int* database_get_foodArray(Database *db){
    return db->food_array;
}


void database_set_foodArray(Database *db, int* food_array){
    memcpy(db->food_array, food_array, sizeof(db->food_array));
}

void database_set_hunger(Database *db, int value) {
    db->minigotchi_hunger = value;
}
void database_set_happiness(Database *db, int value) {
    db->minigotchi_happiness = value;
}
void database_set_coins(Database *db, int value) {
    db->coins = value;
}
void database_set_minigotchiName(Database *db, char* value) {
    memcpy(db->minigotchi_name, value, 10*sizeof(char));
}

/**
 * @brief Searches for an empty slot on the hotbar and in case the hotbar is full doesn't add it
 */
void database_add_food_to_array(Database *db, int food_item){
    for (int i = 0; i < 9; i++){
        if (db->food_array[i] == 0){
            db->food_array[i] = food_item;
            return;
        }
    }
}
