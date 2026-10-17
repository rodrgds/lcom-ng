#include <lcom/lcf.h>

#include "map.h"

extern Map* map1;


Map* (create_map)(int level) {
    char* path = (char*)malloc(200 * sizeof(char));
    char* level_str = (char*)malloc(10 * sizeof(char));

    sprintf(path, MAPS_PATH);
    sprintf(level_str, "%d", level);
    strcat(path, level_str);
    strcat(path, MAPS_EXT);

    sprintf(path, "/home/lcom/labs/shared/proj/src/maps/map%d.txt", level);
    FILE* file = fopen(path, "r");

    Map* map = (Map*)malloc(sizeof(Map));

    if (!file || !map) { return NULL; }
    fscanf(file, "%u %u", &map->columns, &map->rows);
    map->map = (char*)malloc(map->rows * map->columns * sizeof(char));

    char c;
    fscanf(file, "%c", &c); // Read the \n
    int j=0;
    while (fscanf(file, "%c", &c) != EOF) {
        if(c!='\n' && c!='\r'){ 
            map->map[ j] = c;
            j++;
        }
    }
    /*for (uint32_t i = 0; i < map->rows; i++) {
        for (uint32_t j = 0; j < map->columns; j++) {
            fscanf(file, "%c", &c);
             printf("Character: %c\n", c);
            if (c != '\n') { map->map[i * map->columns + j] = c; }
        }
        fscanf(file, "%c", &c); // Read the \n
    }*/

    map->x = map->y = 0;
    map->n_blocks=0;
    fclose(file);
    free(path);
    free(level_str);

    return map;
}

int (load_maps)() {
    map1 = create_map(1);
    map2 = create_map(2);
    map3=create_map(3);
    current_map = map1;

    if (!map1 || !map2 || !map3) { return 1; }

    return 0;
}

int (draw_map)(Map* map) {
    unsigned int background = 0;
    unsigned int wall = 0;
    unsigned int lava = 0;
    unsigned int water = 0;
    unsigned int rope_count = 0;
    unsigned int falling_block_count = 0;
    
    unsigned int fire_door = 0;
    unsigned int water_door = 0;

    unsigned int poison = 0;

    for (uint32_t i = 0; i < map->rows; i++) {
        for (uint32_t j = 0; j < map->columns; j++) {
            uint32_t index = i * map->columns + j;
            uint32_t x = map->x + j * TILE_SIZE, y = map->y + i * TILE_SIZE;

            if (map->map[index] == 'B' || map->map[index] == 'R' || map->map[index] == 'X') {
                switch (background % 3)
                {
                case 0:
                    if(draw_xpm((xpm_map_t)level_background1_xpm, x, y)){
                        printf("Failed to load first image of the background\n");
                        return 1;
                    }
                    break;
                case 1:
                   if( draw_xpm((xpm_map_t)level_background2_xpm, x, y)){
                        printf("Failed to load second image of the background\n");
                        return 1;
                    }
                case 2:
                    if(draw_xpm((xpm_map_t)level_background3_xpm, x, y)){
                        printf("Failed to load third image of the background\n");
                        return 1;
                    }
                default:
                    if(draw_xpm((xpm_map_t)level_background1_xpm, x, y)){
                        printf("Failed to load fourth image of the background\n");
                        return 1;
                    }
                    break;
                }
                background++;

                if (map->map[index] == 'R') {
                    int i = 0;
                    
                    Falling_Block* falling_block = (Falling_Block*)malloc(sizeof(Falling_Block));
                    while (map->map[index] == 'R') {
                        map->map[index] = 'B';
                        falling_block->rope[i++]= create_sprite((xpm_map_t)rope_xpm, x, y, 0, 0);
                        index += map->columns;
                        y += TILE_SIZE;
                    }
                    
                    falling_block->n_rope=i;
                    falling_block->is_cut=false;
                    falling_block->is_on_ground=false;

                    /* Falling Block */
                    if (map->map[index] == 'X') {
                        map->map[index-1] = 'X';
                        map->map[index+1]='X';
                     
                        Sprite* block1 = create_sprite((xpm_map_t)wall1_xpm, x-32, y, 0, 0);
                        Sprite* block2 = create_sprite((xpm_map_t)wall1_xpm, x, y, 0, 0);
                        Sprite* block3 = create_sprite((xpm_map_t)wall2_xpm, x+32, y, 0, 0);
                        falling_block->sprite[1] = block1;
                        falling_block->sprite[2] = block2;
                        falling_block->sprite[0] = block3;
                        map->blocks[falling_block_count++] = falling_block;
                        map->n_blocks=falling_block_count;
                      
                    }

                    rope_count++;
                    continue;
                }
            }
            if (map->map[index] == 'A') {
                switch (wall % 2)
                {
                case 1:
                    if(draw_xpm((xpm_map_t)wall1_xpm, x, y)){
                        printf("Failed to load first image of the wall\n");
                        return 1;
                    }
                    break;

                default:
                    if(draw_xpm((xpm_map_t)wall2_xpm, x, y)){
                        printf("Failed to load second image of the wall\n");
                        return 1;
                    }
                    break;
                }
                wall++;
            }
            if (map->map[index] == 'L') {
                if (lava % 5 == 0) {
                    if(draw_xpm((xpm_map_t)lava1_xpm, x, y)){
                            printf("Failed to load first image of the lava\n");
                            return 1;
                    }
                }
                else if (lava % 5 == 4){
                    if(draw_xpm((xpm_map_t)lava3_xpm, x, y)){
                        printf("Failed to load second image of the lava\n");
                        return 1;
                    }
                }
                else{ 
                    if(draw_xpm((xpm_map_t)lava2_xpm, x, y)){
                        printf("Failed to load third image of the lava\n");
                        return 1;
                    }
                }
                lava++;
            }
            if (map->map[index] == 'P') {
                if (water % 5 == 0){
                    if(draw_xpm((xpm_map_t)water1_xpm, x, y)){
                        printf("Failed to load first image of the water\n");
                        return 1;
                    }
                }
                else if (water % 5 == 4){
                    if(draw_xpm((xpm_map_t)water3_xpm, x, y)){
                        printf("Failed to load second image of the water\n");
                        return 1;
                    }
                }
                else{ 
                    if(draw_xpm((xpm_map_t)water2_xpm, x, y)){
                        printf("Failed to load third image of the water\n");
                        return 1;
                    }
                }
                water++;
            }

            if(map->map[index]=='F'){
                switch (fire_door)
                {
                case 0:
                    if(draw_xpm((xpm_map_t)fire_door1_xpm, x, y)){
                        printf("Failed to load first image of the fire door\n");
                        return 1;
                    }
                    break;
                case 1:
                    if(draw_xpm((xpm_map_t)fire_door2_xpm, x, y)){
                        printf("Failed to load second image of the fire door\n");
                        return 1;
                    }
                    break;
                case 2:
                    if(draw_xpm((xpm_map_t)fire_door3_xpm, x, y)){
                        printf("Failed to load third image of the fire door\n");
                        return 1;
                    }
                    break;
                case 3:
                    if(draw_xpm((xpm_map_t)fire_door4_xpm, x, y)){
                        printf("Failed to load fourth image of the fire door\n");
                        return 1;
                    }
                    break;          

                default:
                    break;
                }
                fire_door++;
            }
            if (map->map[index] == 'W') {
                switch (water_door)
                {
                case 0:
                    if(draw_xpm((xpm_map_t)water_door1_xpm, x, y)){
                        printf("Failed to load first image of the water door\n");
                        return 1;
                    }
                    break;
                case 1:
                    if(draw_xpm((xpm_map_t)water_door2_xpm, x, y)){
                        printf("Failed to load second image of the water door\n");
                        return 1;
                    }
                    break;
                case 2:
                    if(draw_xpm((xpm_map_t)water_door3_xpm, x, y)){
                        printf("Failed to load third image of the water door\n");
                        return 1;
                    }
                    break;
                case 3:
                    if(draw_xpm((xpm_map_t)water_door4_xpm, x, y)){
                        printf("Failed to load fourth image of the water door\n");
                        return 1;
                    }
                    break;          

                default:
                    break;
                }
                water_door++;
            }
            if(map->map[index] == 'V') {
                if (poison % 5 == 0) {
                    if(draw_xpm((xpm_map_t)poison1_xpm, x, y)){
                        printf("Failed to load first image of the poison\n");
                        return 1;
                    }
                    }
                else if (poison % 5 == 4){ 
                    if(draw_xpm((xpm_map_t)poison3_xpm, x, y)){
                        printf("Failed to load second image of the poison\n");
                        return 1;
                    }
                }
                else{ 
                    if(draw_xpm((xpm_map_t)poison2_xpm, x, y)){
                        printf("Failed to load third image of the poison\n");
                        return 1;
                    }
                }
                poison++;

                }
            
        }

    }
    draw_background();
    return 0;
}

int  (nextLevel)(){
    if(current_map==map1){ 
        
        current_map= map2;
        return 0;
    }
    if(current_map==map2){
        current_map=map3;
        return 0;
    }
    if(current_map==map3){
        current_map=map1;
        return 1;
    }
    return 1;
}

void (reset_falling_blocks)(){
    for(int i=0; i< current_map->n_blocks;i++){
        
        if(!current_map->blocks[i]) continue;
          
            if(!current_map->blocks[i]->is_cut){ 
                
                continue;
                }
           
            current_map->blocks[i]->is_cut=false;
            current_map->blocks[i]->is_on_ground=false;
             
        
            int map_index= (current_map->blocks[i]->sprite[0]->y+1) / TILE_SIZE * current_map->columns + current_map->blocks[i]->sprite[0]->x / TILE_SIZE;
            current_map->map[map_index]='B';
         
            map_index=(current_map->blocks[i]->sprite[0]->y+1) / TILE_SIZE * current_map->columns + current_map->blocks[i]->sprite[1]->x / TILE_SIZE;
            current_map->map[map_index]='B';
         
            map_index= (current_map->blocks[i]->sprite[0]->y+1) / TILE_SIZE * current_map->columns + current_map->blocks[i]->sprite[2]->x / TILE_SIZE;
            current_map->map[map_index]='B';
         
            int y= current_map->blocks[i]->rope[current_map->blocks[i]->n_rope-1]->y + TILE_SIZE;
            int x= current_map->blocks[i]->rope[current_map->blocks[i]->n_rope-1]->x ;
            map_index=  y / TILE_SIZE * current_map->columns + x / TILE_SIZE;
            current_map->map[map_index]='B';
        
            erase_sprite(current_map->blocks[i]->sprite[0]);
            erase_sprite(current_map->blocks[i]->sprite[1]);
            erase_sprite(current_map->blocks[i]->sprite[2]);
           
            current_map->blocks[i]->sprite[0]->x=x-TILE_SIZE;
            current_map->blocks[i]->sprite[1]->x=x;
            current_map->blocks[i]->sprite[2]->x=x+TILE_SIZE;
            current_map->blocks[i]->sprite[0]->y=y;
            current_map->blocks[i]->sprite[1]->y=y;
            current_map->blocks[i]->sprite[2]->y=y;
            map_index= (current_map->blocks[i]->sprite[0]->y+1) / TILE_SIZE * current_map->columns + current_map->blocks[i]->sprite[0]->x / TILE_SIZE;
            current_map->map[map_index]='X';
         
            map_index=(current_map->blocks[i]->sprite[0]->y+1) / TILE_SIZE * current_map->columns + current_map->blocks[i]->sprite[1]->x / TILE_SIZE;
            current_map->map[map_index]='X';
         
            map_index= (current_map->blocks[i]->sprite[0]->y+1) / TILE_SIZE * current_map->columns + current_map->blocks[i]->sprite[2]->x / TILE_SIZE;
            current_map->map[map_index]='X';
         
            
        
           
        
    }
}

char get_tile(Map* map, u_int32_t x, u_int32_t y) {
    uint32_t index = y / TILE_SIZE * map->columns + x / TILE_SIZE;

    return map->map[index];
}
