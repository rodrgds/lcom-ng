#include "mp_test.h"
#include "../../include/monster_path_test/mp_test.h"
#include "../../../lab5/spriteXMP.h"
#include "../../include/utils/inputHandler.h"
#include "../../resources/1.h"
#include "../../include/data_structures/vector.h"
#include "../../include/monster_path_test/monster.h"
#include "../../include/monster_path_test/path.h"


static SpriteXPM* sprite;
static Vector* pathPoints;
static Path* path;

static Vector* listOfMonsters;

static float initial_value=0.25;
static float counter;

void freeListOfMonsters(void* li){
    Monster* m=*(Monster**)li;
    monsterFree(m);
}

 void startGame() {
    counter=-1;//initial_value;

    sprite = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8, 0, 0);
    //path 1
    //pathPoints=vectorCreateWithFloatingData(4,4*2,10.0f,10.0f,450.0f,10.0f,10.0f,450.0f,450.0f,450.0f);
    // star path
    //pathPoints = vectorCreateWithFloatingData(sizeof(float), NULL,11 * 2, 
    //400.0f, 50.0f,    // 1.  Ponta Topo
    //456.1f, 222.7f,   // 2.  Vale Superior Direito
    //637.8f, 222.7f,   // 3.  Ponta Direita
    //490.8f, 329.5f,   // 4.  Vale Inferior Direito
    //546.9f, 502.2f,   // 5.  Ponta Inferior Direita
    //400.0f, 395.5f,   // 6.  Vale Central Inferior
    //253.1f, 502.2f,   // 7.  Ponta Inferior Esquerda
    //309.2f, 329.5f,   // 8.  Vale Inferior Esquerdo
    //162.2f, 222.7f,   // 9.  Ponta Esquerda
    //343.9f, 222.7f,   // 10. Vale Superior Esquerdo
    //400.0f, 50.0f     // 11. Ponta Topo (Fecha o caminho)
    //);

    // super star path 


    // 1. Inicializa apenas a estrela original (11 pontos / 22 floats)
    pathPoints = vectorCreateWithFloatingData(sizeof(float), NULL, 11 * 2, 
        400.0f, 50.0f,    // 1.  Ponta Topo
        456.1f, 222.7f,   // 2.  Vale Superior Direito
        637.8f, 222.7f,   // 3.  Ponta Direita
        490.8f, 329.5f,   // 4.  Vale Inferior Direito
        546.9f, 502.2f,   // 5.  Ponta Inferior Direita
        400.0f, 395.5f,   // 6.  Vale Central Inferior
        253.1f, 502.2f,   // 7.  Ponta Inferior Esquerda
        309.2f, 329.5f,   // 8.  Vale Inferior Esquerdo
        162.2f, 222.7f,   // 9.  Ponta Esquerda
        343.9f, 222.7f,   // 10. Vale Superior Esquerdo
        400.0f, 50.0f     // 11. Ponta Topo (Fecha o caminho)
    );

    // 2. Array seguro com todos os 100 pontos (200 floats) do círculo
    float circle_points[] = {
        // --- 1º Quadrante (Topo para a Direita) ---
        400.0f, 50.0f, 415.7f, 50.5f, 431.3f, 52.0f, 446.8f, 54.4f, 462.2f, 57.9f,
        477.3f, 62.2f, 492.0f, 67.6f, 506.4f, 73.8f, 520.4f, 80.9f, 534.0f, 88.9f,
        546.9f, 97.7f, 559.4f, 107.4f, 571.1f, 117.8f, 582.2f, 128.9f, 592.6f, 140.6f,
        602.3f, 153.1f, 611.1f, 166.0f, 619.1f, 179.6f, 626.2f, 193.6f, 632.4f, 208.0f,
        637.8f, 222.7f, 642.1f, 237.8f, 645.6f, 253.2f, 648.0f, 268.7f, 649.5f, 284.3f,
        650.0f, 300.0f,

        // --- 2º Quadrante (Direita para Baixo) ---
        649.5f, 315.7f, 648.0f, 331.3f, 645.6f, 346.8f, 642.1f, 362.2f, 637.8f, 377.3f,
        632.4f, 392.0f, 626.2f, 406.4f, 619.1f, 420.4f, 611.1f, 434.0f, 602.3f, 446.9f,
        592.6f, 459.4f, 582.2f, 471.1f, 571.1f, 482.2f, 559.4f, 492.6f, 546.9f, 502.2f,
        534.0f, 511.1f, 520.4f, 519.1f, 506.4f, 526.2f, 492.0f, 532.4f, 477.3f, 537.8f,
        462.2f, 542.1f, 446.8f, 545.6f, 431.3f, 548.0f, 415.7f, 549.5f, 400.0f, 550.0f,

        // --- 3º Quadrante (Baixo para Esquerda) ---
        384.3f, 549.5f, 368.7f, 548.0f, 353.2f, 545.6f, 337.8f, 542.1f, 322.7f, 537.8f,
        308.0f, 532.4f, 293.6f, 526.2f, 279.6f, 519.1f, 266.0f, 511.1f, 253.1f, 502.2f,
        240.6f, 492.6f, 228.9f, 482.2f, 217.8f, 471.1f, 207.4f, 459.4f, 197.7f, 446.9f,
        188.9f, 434.0f, 180.9f, 420.4f, 173.8f, 406.4f, 167.6f, 392.0f, 162.2f, 377.3f,
        157.9f, 362.2f, 154.4f, 346.8f, 152.0f, 331.3f, 150.5f, 315.7f, 150.0f, 300.0f,

        // --- 4º Quadrante (Esquerda para Topo) ---
        150.5f, 284.3f, 152.0f, 268.7f, 154.4f, 253.2f, 157.9f, 237.8f, 162.2f, 222.7f,
        167.6f, 208.0f, 173.8f, 193.6f, 180.9f, 179.6f, 188.9f, 166.0f, 197.7f, 153.1f,
        207.4f, 140.6f, 217.8f, 128.9f, 228.9f, 117.8f, 240.6f, 107.4f, 253.1f, 97.7f,
        266.0f, 88.9f, 279.6f, 80.9f, 293.6f, 73.8f, 308.0f, 67.6f, 322.7f, 62.2f,
        337.8f, 57.9f, 353.2f, 54.4f, 368.7f, 52.0f, 384.3f, 50.5f
    };

    // 3. Fazer o Push Back iterando sobre o array
    int total_floats = sizeof(circle_points) / sizeof(circle_points[0]);
    for (int i = 0; i < total_floats; i++) {
        vectorPushBack(pathPoints, (char*)&circle_points[i]);
    }

    // 4. Criar o caminho final com todos os 111 pontos integrados
    
    path=pathCreate(5.0f,pathPoints);

    listOfMonsters=vectorCreate(sizeof(Monster*),freeListOfMonsters);

    

    



    
}

void exitGame() {
    freeSpriteXPM(sprite);
    
    vectorFree(listOfMonsters);
    vectorFree(pathPoints);
    pathFree(path);
}

void gameLoop(Terminal* terminal,float delta_time) {
    
    counter-=delta_time;

    if(counter<0 && vectorGetSize(listOfMonsters)<=50){
        counter=initial_value;
        printf("new_monster!\n");
        Monster *m=monsterCreate(400,50,300,0);
        ;
        vectorPushBack(listOfMonsters,(char*)&m);
        pathSubscribe(path,(void *)vectorGetSize(listOfMonsters));
        
    }
    

    Vec2d* n_dir=vec2dCreate(0,0);
    Vec2d* pos=vec2dCreate(0,0);
    for(unsigned int i=0;i<vectorGetSize(listOfMonsters);i++){
        
        Monster* m=*(Monster**)vectorGet(listOfMonsters,i);
        monsterGetPos(m,pos);
        pathFollowingDir(path,(void *)(i+1),pos,n_dir);
        monsterSetDirWithVec(m,n_dir);

        monsterUpdatePos(m,delta_time);

        drawSpriteXPM(sprite, terminal, monsterGetX(m), monsterGetY(m));
        
    }
    vec2dFree(pos);
    vec2dFree(n_dir);
}
