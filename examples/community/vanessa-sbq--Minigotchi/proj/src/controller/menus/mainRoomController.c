#include "mainRoomController.h"

static MainRoom *mainRoom = NULL;
static collision_type_mainRoom_t currentButtonEvent = NOP_MAINROOM;
static Database* database;

/**
 * @brief Load the main room (first time)
 */
void mainRoomController_load_mainRoom(){
    if (mainRoom == NULL){
        setup_happiness_sprites();
        setup_hunger_sprites();
        database = getDatabase();
        mainRoom = new_mainRoom();
        mainRoomViewer_setMainRoom(mainRoom);
        setup_text_sprite();
        
        // Load text into vector
        char* minigotchiName = database_get_minigotchiName(database);
        printf("Minini: %s\n", minigotchiName);

        int i = 0;
        /* do { 
            printf("Char %d is %c\n", i, minigotchiName[i]);
            i++;
        } while (minigotchiName[i] != '\0'); */


        if (getSpriteVector() == NULL) {
            Vector* vector = init_vector(1);
            while (true) {
                char ch = minigotchiName[i];    
                push_back(vector, (uint8_t)(ch - 'A'));
                if (ch == '\0') break;
                i++;
            }
            setSpriteVector(vector);
        }
        

        
        //spriteVector = init_vector(i - 1); // Store the chars in the vector

        


        printf("%s \n", database->minigotchi_name);
        mainRoom->minigotchi->name = database->minigotchi_name;
    }
}

/**
 * @brief Function that toggles the hotbar
 */
void mainRoomController_toggleHotbar() {
    mainRoom->hotbar->hidden = !mainRoom->hotbar->hidden;
}

/**
 * @brief Function that enables going right on the hotbar
 */
void mainRoomController_Hotbar_goRight() {
    mainRoom->hotbar->highlighted++;
    mainRoom->hotbar->highlighted %= 9;

    
}

/**
 * @brief Function that enables going left on the hotbar
 */
void mainRoomController_Hotbar_goLeft() {
    if (mainRoom->hotbar->highlighted == 0) {
        mainRoom->hotbar->highlighted = 9;
    }
    mainRoom->hotbar->highlighted--;
}

/**
 * @brief Functions that handles the minigotchi feeding event, by removing the item from the hotbar and updating the minigotchi's hunger
 */
void mainRoomController_feed_minigotchi() {
    Hotbar* hotbar = mainRoom_get_hotbar(mainRoom);
    if (!hotbar_is_hidden(hotbar)){
        uint8_t highlighted_index = hotbar_get_highlighted(hotbar);
        Item* highlighted_item = hotbar_get_highlighted_item(hotbar);
        if (item_get_id(highlighted_item) != 0){
            if ((item_get_feedLevel(highlighted_item) + database_get_hunger(database)) >= MAX_HUNGER) {
                database_set_hunger(database, MAX_HUNGER);
            } else {
                database_set_hunger(database, database_get_hunger(database) + item_get_feedLevel(highlighted_item));
            }
            int* food_array = database_get_foodArray(database); 
            food_array[highlighted_index] = 0;
            database_set_foodArray(database, food_array);
        }
    }
}

/**
 * @brief Function that checks sprite collisions with the cursor
 */
bool mainRoomController_checkCollision(Sprite* sprite, int x, int y) {
    Cursor* cursor = mainRoom_get_cursor(mainRoom);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

    int sprite_width = sprite_get_width(sprite);
    int sprite_height = sprite_get_height(sprite);

    int left_upper_corner_x = x;

    int right_upper_corner_x = left_upper_corner_x + sprite_width;

    int right_upper_corner_y = y;

    int left_lower_corner_y = right_upper_corner_y + sprite_height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
        return true;
    }

    return false;
}

collision_type_mainRoom_t mainRoomController_getButtonEvent() {
    return currentButtonEvent;
}

void mainRoomController_setButtonEvent(collision_type_mainRoom_t ct) {
    currentButtonEvent = ct;
}

/**
 * @brief Controller function that is called once every frame and updates / checks every action in the main room
 */
void mainRoomController_step(){
    mainRoomController_load_mainRoom();
    
    // Check if "Quit" clicked
    Button* quit_button = mainRoom_get_quitButton(mainRoom);
    if (mainRoomController_checkCollision(button_get_sprite(quit_button), button_get_x(quit_button), button_get_y(quit_button))){
        currentButtonEvent = QUIT_MAINROOM;
    }

    // Check if "Minigames" clicked
    Button* minigames_button = mainRoom_get_miniGamesButton(mainRoom);
    if (mainRoomController_checkCollision(button_get_sprite(minigames_button), button_get_x(minigames_button), button_get_y(minigames_button))){
        currentButtonEvent = MINIGAMES_MAINROOM;
    }

    // Update Happiness
    Bar* happiness_bar = mainRoom_get_happinessBar(mainRoom);
    bar_set_level(happiness_bar, database_get_happiness(getDatabase()));
    bar_set_sprite(happiness_bar, guiDrawer_get_bar_sprite(0, database_get_happiness(getDatabase())));

    // Update Hunger
    Bar* hunger_bar = mainRoom_get_hungerBar(mainRoom);
    bar_set_level(hunger_bar, database_get_hunger(getDatabase()));
    bar_set_sprite(hunger_bar, guiDrawer_get_bar_sprite(1, database_get_hunger(getDatabase())));

    // Update Hotbar
    Hotbar* hotbar = mainRoom_get_hotbar(mainRoom);
    hotbar_update_items(hotbar, database_get_foodArray(getDatabase())); // Load database items into hotbar

    Minigotchi* minigotchi = mainRoom_get_minigotchi(mainRoom);
    if (mainRoomController_checkCollision(minigotchi_get_sprite(minigotchi), minigotchi_get_x(minigotchi), minigotchi_get_y(minigotchi))) {
        minigotchi_set_sprite(minigotchi, guiDrawer_get_minigotchi_cuddle_sprite());
        minigotchi_set_cuddles(minigotchi, true);
        int current_happiness = database_get_happiness(getDatabase());
        if (current_happiness < MAX_HUNGER) database_set_happiness(getDatabase(), current_happiness + 1);
    } else {
        minigotchi_set_sprite(minigotchi, guiDrawer_get_minigotchi_normal_sprite());
        minigotchi_set_cuddles(minigotchi, false);
    }

}

/**
 * @brief Calls the MainRoom destructor
*/
void mainRoomController_delete_mainRoom(){
    delete_mainRoom(mainRoom);
}

Cursor* getMainRoomCursor() {
    currentButtonEvent = NOP_MAINROOM;
    return mainRoom->cursor;
}

void setMainRoomCursor(Cursor* cursor) {
    mainRoom->cursor = cursor;
}
