#include "game.h"
#include "list.h"

extern enemy_node* enemy_list_head;

extern Sprite* character;
extern Sprite* soul_blast_atk;
extern Sprite* bully1;
extern Sprite* crawlie1;
extern Sprite* fly1;
extern Sprite* fly_knight1;
extern Sprite* gunk_bully1;
extern Sprite* hpempty;
extern Sprite* hpfull;
extern Sprite* boss;
extern Sprite *boss_attack;
extern Sprite *boss_crouch;
extern Sprite *boss_stand;

invis_hitbox l1f1;
invis_hitbox l1f2;
invis_hitbox l1w1;
invis_hitbox l1w2;
invis_hitbox l1c1;
invis_hitbox l1d1;
invis_hitbox l1d2;
invis_hitbox l1d3;

invis_hitbox l2w1;
invis_hitbox l2p1;
invis_hitbox l2p2;
invis_hitbox l2s1;
invis_hitbox l2d1;
invis_hitbox l2d2;
invis_hitbox l2c1;
invis_hitbox l2c2;
invis_hitbox l2f1;
enemy_character l2e1;
enemy_character l2e2;

invis_hitbox l3w1;
invis_hitbox l3p1;
invis_hitbox l3p2;
invis_hitbox l3p3;
invis_hitbox l3f1;
invis_hitbox l3w2;
invis_hitbox l3w3;
invis_hitbox l3s1;
invis_hitbox l3d1;
invis_hitbox l3d2;
invis_hitbox l3c1;
enemy_character l3e1;
enemy_character l3e2;

invis_hitbox l4c1;
invis_hitbox l4p1;
invis_hitbox l4f1;
invis_hitbox l4w1;
invis_hitbox l4w2;
invis_hitbox l4d1;
invis_hitbox double_jump;
enemy_character l4e1;
enemy_character l4e2;
enemy_character l4e3;
enemy_character l4e4;

invis_hitbox l5c1;
invis_hitbox l5c2;
invis_hitbox l5p1;
invis_hitbox l5p2;
invis_hitbox l5w1;
invis_hitbox l5w2;
invis_hitbox l5f1;
invis_hitbox l5d1;
invis_hitbox l5d2;
enemy_character l5e1;
enemy_character l5e2;
enemy_character l5e3;

invis_hitbox l6w1;
invis_hitbox l6w2;
invis_hitbox l6f1;
invis_hitbox l6p1;
invis_hitbox l6d1;
invis_hitbox l6w3;
invis_hitbox l6c1;
invis_hitbox soul_blast;
enemy_character l6e1;
enemy_character l6e2;
enemy_character l6e3;

invis_hitbox l7c1;
invis_hitbox l7w1;
invis_hitbox l7f1;
invis_hitbox l7w2;
invis_hitbox l7d1;
boss_character l7boss;

invis_hitbox slash_hitbox;

int enemy_init_dir = 1;

player_character init_player(){
    player_character player;
    character_class backend;
    backend.inv_time = 0;
    backend.x_pos=594;
    backend.y_pos=465;
    backend.x_vel=0;
    backend.y_vel=3;
    backend.char_sprite=character;
    backend.hp=10;
    backend.drawChar=drawSprite;
    backend.hitbox = init_hitbox(594, 465, character->width, character->height-10, "character");
    player.feethitbox = init_hitbox(594, 445+character->height , character->width, 20, "character");
    player.state=WALKING;
    player.hasDoubleJump=false;
    player.hasSoulBlast=false;
    player.character=backend;
    return player;
}

enemy_character init_enemy(Sprite* sprite, int x_pos, int y_pos, uint8_t hp, uint8_t damage, int std_x_vel, int std_y_vel, int (*enemyAI)(enemy_character*, invis_hitbox, player_character)){
    enemy_character enemy;
    enemy.character.inv_time = 0;
    enemy.character.char_sprite = sprite;
    enemy.character.x_pos = x_pos;
    enemy.character.y_pos = y_pos;
    enemy.character.drawChar = drawSprite;
    enemy.character.x_vel = std_x_vel*enemy_init_dir;
    enemy.character.y_vel = 0;
    enemy.character.hp = hp;
    enemy.character.hitbox = init_hitbox(x_pos, y_pos, sprite->width, sprite->height-10, "enemy");
    enemy.feethitbox = init_hitbox(x_pos, y_pos + enemy.character.hitbox.length, sprite->width, 20, "enemy");
    enemy.enemyAI = enemyAI; 
    enemy.head_collision = 0;
    enemy.feet_collision = 0;
    enemy.std_x_vel = std_x_vel;
    enemy.std_y_vel = std_y_vel;
    enemy.damage = damage;

    enemy_init_dir *= -1;

    return enemy;
}

int init_all_hitboxes(){
    l1f1= init_hitbox(0,570,330,30,"wall");
    l1f2= init_hitbox(465,570,335,30,"wall");
    l1w1 = init_hitbox(719,300,81,265,"wall");
    l1w2 = init_hitbox(670,300,130,80,"wall");
    l1c1 = init_hitbox(0,0,800,43,"wall");
    l1d1 = init_hitbox(0,0,30,800,"door");
    l1d2 = init_hitbox(330,585,135,15,"door");
    l1d3 = init_hitbox(785,43,15,257,"door");
    l1d1.destination = init_gate(0,1,720,450);
    l1d2.destination = init_gate(0,4,330,50);
    l1d3.destination = init_gate(0,6,15,6);

    l2w1 = init_hitbox(0,0,10,600,"wall");
    l2f1 = init_hitbox(0,570,800,30,"wall");
    l2p1 = init_hitbox(60,370,242,55,"wall");
    l2p2 = init_hitbox(314,208,250,46,"wall");
    l2c1 = init_hitbox(0,0,340,32,"wall");
    l2c2 = init_hitbox(443,0,357,38,"wall");
    l2s1 = init_hitbox(0,534,300,66,"damage");
    l2d1 = init_hitbox(792,42,8,526,"door");
    l2d2 = init_hitbox(341,0,100,30,"door");
    l2d1.destination=init_gate(1,0,50,450);
    l2d2.destination=init_gate(1,2,230,460);
    l2e1 = init_enemy(bully1,320,470,3,1, 2, 0, standard_enemy_ai);
    l2e2 = init_enemy(crawlie1,70,290,3,1, 2, 0, standard_enemy_ai);
    l2e1.lim_hitbox = l2f1;
    l2e2.lim_hitbox = l2p1;

    l3p1 = init_hitbox(0,328,424,82,"wall");
    l3p2 = init_hitbox(421,353,187,58,"wall");
    l3p3 = init_hitbox(161,157,639,55,"wall");
    l3w1 = init_hitbox(0,383,74,217,"wall");
    l3f1 = init_hitbox(224,568,576,32,"wall");
    l3w2 = init_hitbox(783,189,17,382,"wall");
    l3w3 = init_hitbox(0,0,23,332,"wall");
    l3c1 = init_hitbox(0,0,800,30,"wall");
    l3s1 = init_hitbox(20,193,77,138,"damage");
    l3d1 = init_hitbox(71,590,153,10,"door");
    l3d2 = init_hitbox(793,30,7,129,"door");
    l3d1.destination = init_gate(2,1,350,40);
    l3d2.destination = init_gate(2,3,50,45);
    l3e1 = init_enemy(bully1, 100, 228, 3, 1, 2, 0, standard_enemy_ai);
    l3e2 = init_enemy(fly_knight1, 450,57, 3, 1, 2, 0, standard_enemy_ai);
    l3e1.lim_hitbox = l3p1;
    l3e2.lim_hitbox = l3p3;

    l4c1 = init_hitbox(0,0,800,30,"wall");
    l4p1 = init_hitbox(0,194,126,89,"wall");
    l4w1 = init_hitbox(0,278,16,304,"wall");
    l4f1 = init_hitbox(0,570,800,30,"wall");
    l4w2 = init_hitbox(781,0,19,577,"wall");
    l4d1 = init_hitbox(0,28,13,172,"door");
    l4d1.destination = init_gate(3,2,740,40);
    l4e1 = init_enemy(fly_knight1, 126,470, 3, 1, 2, 0, standard_enemy_ai);
    l4e2 = init_enemy(fly1, 450, 194, 3, 1, 1, 1, flying_enemy_ai);
    l4e3 = init_enemy(fly1, 226, 300, 3, 1, 1, 1, flying_enemy_ai);
    l4e4 = init_enemy(fly1, 650, 400, 3, 1, 1, 1, flying_enemy_ai);
    l4e1.lim_hitbox = l4f1;
    l4e2.lim_hitbox = l4f1;
    l4e3.lim_hitbox = l4f1;
    l4e4.lim_hitbox = l4f1;

    l5c1 = init_hitbox(0,0,319,32,"wall");
    l5c2 = init_hitbox(501,0,299,30, "wall");
    l5w1 = init_hitbox(778,25,22,402,"wall");
    l5p1 = init_hitbox(289,169,271,60,"wall");
    l5p2 = init_hitbox(586,348,198,63,"wall");
    l5d1 = init_hitbox(790,428,10,143,"door");
    l5f1 = init_hitbox(0,570,800,30,"wall");
    l5w2 = init_hitbox(0,30,130,542,"wall");
    l5d2 = init_hitbox(319,0,182,13,"door");
    l5d2.destination = init_gate(4,0,270,465);
    l5d1.destination = init_gate(4,5,20,485);
    l5e1 = init_enemy(fly1, 600, 250, 3, 1, 1, 1, flying_enemy_ai);
    l5e2 = init_enemy(gunk_bully1, 450, 450, 3, 1, 2, 0, standard_enemy_ai);
    l5e3 = init_enemy(gunk_bully1, 350, 450, 3, 1, 2, 0, standard_enemy_ai);
    l5e1.lim_hitbox = l5f1;
    l5e2.lim_hitbox = l5f1;
    l5e3.lim_hitbox = l5f1;

    l6w1 = init_hitbox(0,0,26,429,"wall");
    l6w2 = init_hitbox(706,0,94,800,"wall");
    l6w3 = init_hitbox(634,0,166,166,"wall");
    l6p1 = init_hitbox(18,352,65,45,"wall");
    l6d1 = init_hitbox(0,426,10,169,"door");
    l6f1 = init_hitbox(0,593,800,7,"wall");
    l6c1 = init_hitbox(0,0,800,10,"wall");
    l6d1.destination = init_gate(5,4, 730, 460);
    l6e1 = init_enemy(gunk_bully1, 250, 473, 3, 1, 2, 0, standard_enemy_ai);
    l6e2 = init_enemy(fly1, 300, 250, 3, 1, 1, 1, flying_enemy_ai);
    l6e3 = init_enemy(fly1, 600, 200, 3, 1, 1, 1, flying_enemy_ai);
    l6e1.lim_hitbox = l6f1;
    l6e2.lim_hitbox = l6f1;
    l6e3.lim_hitbox = l6f1;

    l7c1 = init_hitbox(0,0,800,5,"wall");
    l7w1 = init_hitbox(0,100,97,500,"wall");
    l7f1 = init_hitbox(0,556,800,44,"wall");
    l7w2 = init_hitbox(758,0,42,600,"wall");
    l7d1 = init_hitbox(0,0,5,113,"wall");
    l7boss.enemystruct=init_enemy(boss,572,368,12,1, 0, 0, no_ai);
    Sprite* temparray[] = {boss,boss_crouch,boss_stand,boss_attack};
    memcpy(l7boss.array,temparray,sizeof(temparray));
    l7boss.animation_time=300;

    soul_blast = init_hitbox(496,494,174,59,"powerup");
    double_jump = init_hitbox(619,415,138,138,"powerup");
    
    return 0;
}

int update_player_position(player_character* player, int x, int y){
    player->character.x_pos=x;
    player->character.y_pos=y;
    player->character.hitbox.x_pos=x;
    player->character.hitbox.y_pos=y;
    player->feethitbox.x_pos=x;
    player->feethitbox.y_pos=y+player->character.char_sprite->height-20;
    return 0;
}

uint8_t attack_dir(player_character player, int mouseX, int mouseY){
    bool x_range = (mouseX > (player.character.x_pos - 100)) && (mouseX < (player.character.x_pos + player.character.char_sprite->width + 100));

    if((mouseY < (player.character.y_pos)) && x_range) return TOP;
    else if((mouseY > (player.character.y_pos + player.character.char_sprite->height)) && x_range) return BOTTOM;
    else if(mouseX < player.character.x_pos+(player.character.char_sprite->width/2)) return LEFT;
    else if(mouseX >= player.character.x_pos+(player.character.char_sprite->width/2)) return RIGHT;

    return NO;
}

int knockback(character_class* character, uint8_t dir, uint16_t knockback){
    
    if(dir & RIGHT){
        character->x_pos -= knockback;
    }
    else if(dir & LEFT){
        character->x_pos += knockback;
    }
    return 0;
}

int blast(player_character player, attack* blast_entity, int mouseX, int mouseY, uint32_t timerCounter){
    char attackdir = attack_dir(player, mouseX, mouseY);
    if (attackdir==LEFT || attackdir==RIGHT){
        blast_entity->y_vel = 0;
        if (attackdir==RIGHT){
            blast_entity->x_pos = player.character.char_sprite->width+player.character.x_pos+20;
            blast_entity->x_vel = 10;
            blast_entity->y_pos = (player.character.char_sprite->height/2)+player.character.y_pos;
            blast_entity->drawFunc=drawSprite;
        }
        else{
            blast_entity->x_pos = player.character.x_pos-20;
            blast_entity->x_vel = -10;
            blast_entity->y_pos = (player.character.char_sprite->height/2)+player.character.y_pos;
            blast_entity->drawFunc=draw_sprite_flipped;
        }
    }
    else if (attackdir==TOP||attackdir==BOTTOM){
        blast_entity->x_vel = 0;
        if (attackdir==TOP){
            blast_entity->x_pos = (player.character.char_sprite->width/2)+player.character.x_pos;
            blast_entity->y_vel = -10;
            blast_entity->y_pos = player.character.y_pos-20;
            blast_entity->drawFunc=draw_sprite_up;
        }
        else{
            blast_entity->x_pos = (player.character.char_sprite->width/2)+player.character.x_pos;
            blast_entity->y_vel = 10;
            blast_entity->y_pos = player.character.y_pos+player.character.char_sprite->height+20;
            blast_entity->drawFunc=draw_sprite_down;
        }
    }
    blast_entity->hitbox.x_pos = blast_entity->x_pos;
    blast_entity->hitbox.y_pos = blast_entity->y_pos;
    blast_entity->end_time=timerCounter+600;
    return 0;
}

int slash(player_character player, attack* slash_entity, int mouseX, int mouseY, uint32_t timerCounter){
    
    slash_entity->x_pos = player.character.x_pos;
    slash_entity->y_pos = player.character.y_pos;

    char dir = attack_dir(player, mouseX, mouseY);

    switch (dir)
    {
    case TOP:
        slash_entity->drawFunc = draw_sprite_up;
        slash_entity->hitbox.width = slash_entity->sprite->height;
        slash_entity->hitbox.length = slash_entity->sprite->width;
        slash_entity->y_pos = slash_entity->y_pos - slash_entity->hitbox.length;
        break;
    
    case BOTTOM:
        slash_entity->drawFunc = draw_sprite_down;
        slash_entity->hitbox.width = slash_entity->sprite->height;
        slash_entity->hitbox.length = slash_entity->sprite->width;
        slash_entity->y_pos += player.character.char_sprite->height;
        break;
    
    case RIGHT:
        slash_entity->drawFunc = drawSprite;
        slash_entity->hitbox.width = slash_entity->sprite->width;
        slash_entity->hitbox.length = slash_entity->sprite->height;
        slash_entity->x_pos += player.character.char_sprite->width; 
        break;
    
    case LEFT:
        slash_entity->drawFunc = draw_sprite_flipped;
        slash_entity->hitbox.width = slash_entity->sprite->width;
        slash_entity->hitbox.length = slash_entity->sprite->height;
        slash_entity->x_pos -= slash_entity->hitbox.width;
        break;
    }
    
    slash_entity->hitbox.x_pos = slash_entity->x_pos;
    slash_entity->hitbox.y_pos = slash_entity->y_pos;
    slash_entity->end_time = timerCounter + 15;

    return 0;
}

void drawHUD(uint8_t health, uint8_t healthMax, int pos_X, int pos_Y){
    for(int i = 0; i<healthMax; i++){
        if(health > i){
            drawSprite(hpfull,pos_X+(i*(hpfull->width)), pos_Y);
        }
        else{
            drawSprite(hpempty, pos_X +(i*(hpempty->width)), pos_Y);
        }
    }
}

enemy_character generateRandomPositionEnemy(){
    return init_enemy(fly1,rand()%800,-80,3,1,1,1,flying_enemy_ai);
}

int generateLevel1(){
    deleteList();
    createList(l1f1);
    addNode(l1f2);
    addNode(l1w1);
    addNode(l1w2);
    addNode(l1c1);
    addNode(l1d1);
    addNode(l1d2);
    addNode(l1d3);

    deleteEnemyList();
    return 0;
}

int generateLevel2(){
    deleteList();
    createList(l2w1);
    addNode(l2f1);
    addNode(l2p1);
    addNode(l2p2);
    addNode(l2c1);
    addNode(l2c2);
    addNode(l2s1);
    addNode(l2d1);
    addNode(l2d2);

    deleteEnemyList();
    createEnemyList(l2e1);
    addEnemyNode(l2e2);
    addEnemyNode(l2e2);
    return 0;
}

int generateLevel3(){
    deleteList();
    createList(l3c1);
    addNode(l3d1);
    addNode(l3d2);
    addNode(l3f1);
    addNode(l3p1);
    addNode(l3p2);
    addNode(l3p3);
    addNode(l3s1);
    addNode(l3w1);
    addNode(l3w2);
    addNode(l3w3);

    deleteEnemyList();
    createEnemyList(l3e1);
    addEnemyNode(l3e2);
    
    return 0;
}

int generateLevel4(){
    deleteList();
    createList(l4c1);
    addNode(l4d1);
    addNode(l4f1);
    addNode(l4p1);
    addNode(l4w1);
    addNode(l4w2);

    deleteEnemyList();
    createEnemyList(l4e1);
    addEnemyNode(l4e2);
    addEnemyNode(l4e3);
    addEnemyNode(l4e4);

    return 0;
}

int generateLevel5(){
    deleteList();
    createList(l5w1);
    addNode(l5f1);
    addNode(l5c1);
    addNode(l5c2);
    addNode(l5d1);
    addNode(l5d2);
    addNode(l5p1);
    addNode(l5p2);
    addNode(l5w2);

    deleteEnemyList();
    createEnemyList(l5e1);
    addEnemyNode(l5e2);
    addEnemyNode(l5e3);

    return 0;
}

int generateLevel6(){
    deleteList();
    createList(l6c1);
    addNode(l6d1);
    addNode(l6f1);
    addNode(l6p1);
    addNode(l6w1);
    addNode(l6w2);
    addNode(l6w3);

    deleteEnemyList();
    createEnemyList(l6e1);
    addEnemyNode(l6e2);
    addEnemyNode(l6e3);

    return 0;
}

int generateLevel7(){
    deleteList();
    createList(l7c1);
    addNode(l7d1);
    addNode(l7f1);
    addNode(l7w1);
    addNode(l7w2);

    deleteEnemyList();
    createEnemyList(l7boss.enemystruct);

    return 0;
}

int standard_enemy_ai(struct enemy_character* enemy, invis_hitbox hbox, player_character player){

    if(enemy->character.x_pos <= hbox.x_pos || (enemy->character.x_pos+enemy->character.hitbox.width) >= (hbox.x_pos+hbox.width)){
        enemy->character.x_vel *= -1;
    }

    int y_diff = player.feethitbox.y_pos + player.feethitbox.length - enemy->character.y_pos - enemy->character.hitbox.length;
    int x_diff = player.character.x_pos - enemy->character.x_pos;

    if(y_diff > -10 && y_diff < 10 && x_diff > -300 && x_diff < 300){

        if(enemy->character.x_pos < player.character.x_pos){
            enemy->character.x_vel = enemy->std_x_vel;
        }

        if(enemy->character.x_pos > player.character.x_pos){
            enemy->character.x_vel = -1 * enemy->std_x_vel;
        }
    }
    return 0; 
}

int flying_enemy_ai(enemy_character* enemy, invis_hitbox hbox, player_character player){

    if(enemy->character.x_pos <= hbox.x_pos || (enemy->character.x_pos+enemy->character.hitbox.width) >= (hbox.x_pos+hbox.width)){
        enemy->character.x_vel *= -1;
    }

    if(enemy->character.y_pos <= hbox.y_pos || (enemy->character.y_pos+enemy->character.hitbox.length) >= (hbox.y_pos+hbox.length)){
        enemy->character.y_vel *= -1;
    }

    if(enemy->character.y_pos < player.character.y_pos - 10){
        enemy->character.y_vel = enemy->std_y_vel;
    }
    else if(enemy->character.y_pos > player.character.y_pos + 10){
        enemy->character.y_vel = -1 * enemy->std_y_vel;
    }else{
        enemy->character.y_vel = 0;
    }

    if(enemy->character.x_pos < player.character.x_pos - 30){
        enemy->character.x_vel = enemy->std_x_vel;
    }
    else if(enemy->character.x_pos > player.character.x_pos + 30){
        enemy->character.x_vel = -1 * enemy->std_x_vel;
    }else{
        enemy->character.x_vel = 0;
    }

    return 0;
   
} 

int no_ai(struct enemy_character* enemy, invis_hitbox hbox, player_character player){
    return 0;
}

int update_enemy_position(enemy_character* enemy){
    enemy->character.x_pos += enemy->character.x_vel;
    enemy->character.y_pos += enemy->character.y_vel;
    enemy->character.hitbox.x_pos = enemy->character.x_pos;
    enemy->character.hitbox.y_pos = enemy->character.y_pos;
    enemy->feethitbox.x_pos += enemy->character.x_vel;
    enemy->feethitbox.y_pos += enemy->character.y_vel;
    enemy->feet_collision = 0;
    enemy->head_collision = 0;

    if(enemy->character.x_vel <= 0) enemy->character.drawChar = drawSprite;
    else{
        enemy->character.drawChar = draw_sprite_flipped;
    }

    return 0;
}

int update_attack_position(attack* attack){
    attack->x_pos+=attack->x_vel;
    attack->y_pos+=attack->y_vel;
    attack->hitbox.x_pos+=attack->x_vel;
    attack->hitbox.y_pos+=attack->y_vel;

    return 0;
}

int clear_attack(attack* attack){
    attack->x_pos = 1000;
    attack->y_pos = 1000;
    attack->x_vel = 0;
    attack->y_vel = 0;
    attack->hitbox.x_pos = 1000;
    attack->hitbox.y_pos = 1000;

    return 0;
}


int enemies_interactions(player_character *player, uint32_t timerCounter, attack *slash_entity, attack *blast_entity, game_state *game_state, uint32_t* store_timer){
    enemy_node *temp_enemy_node = enemy_list_head;
    
    while (temp_enemy_node != NULL){

        if(temp_enemy_node->node.character.hp == 0){
            if (temp_enemy_node->node.enemyAI==no_ai){
                *store_timer = timerCounter;
                *game_state=CUTSCENE;
                break;
            }
            else{
                enemy_node* to_delete = temp_enemy_node;
                temp_enemy_node=temp_enemy_node->next;
                deleteEnemyNode(to_delete);
            }
        }
        else{
            temp_enemy_node->node.enemyAI(&temp_enemy_node->node,temp_enemy_node->node.lim_hitbox, *player);

            uint8_t hit_player = verifyCollision(player->character.hitbox ,temp_enemy_node->node.character.hitbox);

            if(hit_player && player->character.inv_time <= timerCounter){
            player->character.inv_time = timerCounter + 60;
            knockback(&player->character, hit_player, 50);
            player->character.hp -= temp_enemy_node->node.damage;
            }
            uint8_t hit_slash = verifyCollision(temp_enemy_node->node.character.hitbox, slash_entity->hitbox);
            uint8_t hit_blast = verifyCollision(temp_enemy_node->node.character.hitbox, blast_entity->hitbox);

            if(hit_slash && temp_enemy_node->node.character.inv_time <= timerCounter){
                temp_enemy_node->node.character.inv_time = timerCounter + 30;
                if (temp_enemy_node->node.enemyAI!=no_ai) knockback(&temp_enemy_node->node.character, hit_slash, 50);
                else knockback(&player->character,RIGHT,300);
                temp_enemy_node->node.character.hp -= slash_entity->damage;
            }
            
            if(hit_blast && temp_enemy_node->node.character.inv_time <= timerCounter){
                temp_enemy_node->node.character.inv_time = timerCounter + 30;
                if (temp_enemy_node->node.enemyAI!=no_ai) knockback(&temp_enemy_node->node.character, hit_blast, 50);
                temp_enemy_node->node.character.hp -= blast_entity->damage;
                clear_attack(blast_entity);
            }

            if (((temp_enemy_node->node.head_collision & LEFT) || (temp_enemy_node->node.head_collision & RIGHT)) && (temp_enemy_node->node.enemyAI == standard_enemy_ai)){
                temp_enemy_node->node.character.x_vel *= -1;
            }

            temp_enemy_node->node.character.x_pos += temp_enemy_node->node.character.x_vel;
            temp_enemy_node->node.character.y_pos += temp_enemy_node->node.character.y_vel;
            temp_enemy_node->node.character.hitbox.x_pos = temp_enemy_node->node.character.x_pos;
            temp_enemy_node->node.character.hitbox.y_pos = temp_enemy_node->node.character.y_pos;
            temp_enemy_node->node.feethitbox.x_pos += temp_enemy_node->node.character.x_vel;
            temp_enemy_node->node.feethitbox.y_pos += temp_enemy_node->node.character.y_vel;
            temp_enemy_node->node.feet_collision = 0;
            temp_enemy_node->node.head_collision = 0;

            if(temp_enemy_node->node.character.x_vel <= 0) temp_enemy_node->node.character.drawChar = drawSprite;
            else{
                temp_enemy_node->node.character.drawChar = draw_sprite_flipped;
            } 

            temp_enemy_node->node.character.drawChar(temp_enemy_node->node.character.char_sprite, temp_enemy_node->node.character.x_pos, temp_enemy_node->node.character.y_pos);

            temp_enemy_node = temp_enemy_node->next;
        }
    }
    return 0;
}
