#include "../include/ui/buildMenu.h"
/** @file buildMenu.c @brief Implementacao; ver comentarios nas funcoes estaticas onde aplicavel. */
#include "../include/utils/cursorHandler.h"
#include "../include/button.h"
#include "../include/data.h"

#include "../resources/icons/icons.h"
#include "../resources/upgradeMenu/upgradeMenu.h"

struct BuildMenu {
    Vec2d position;
    SpriteXPM* background;
    Button* button;

    TowerKind type;
	bool pressedBuild;
};

static void basicTowerButtonFn(Button* self) {}
static void longTowerButtonFn(Button* self) {}

BuildMenu* buildMenuNew(TowerKind type) {
    BuildMenu* buildMenu = malloc(sizeof(BuildMenu));
    buildMenu->type = type;
    SpriteXPM** sprites = malloc(sizeof(SpriteXPM*) * UPGRADE_HAMMER_FRAMES);
    sprites[0] = newSpriteXPM((xpm_map_t)iconsXPM[BUILD_NORMAL_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    sprites[1] = newSpriteXPM((xpm_map_t)iconsXPM[BUILD_PRESSED_BUTTON_INDEX], XPM_8_8_8, 0, 0);

    AnimatedSprite* animatedSprite = animatedSpriteNew(UPGRADE_HAMMER_FRAME_SPEED, sprites, UPGRADE_HAMMER_FRAMES, NORMAL);

    if (type == TOWER_KIND_BASIC) {
        buildMenu->position = (Vec2d){UPGRADE_SCROLL_LEFT_X, UPGRADE_SCROLL_ORIGIN_Y};
        buildMenu->background = newSpriteXPM((xpm_map_t)upgradeMenuXPM[0], XPM_8_8_8, 0, 0);
        buildMenu->button = buttonNew(UPGRADE_HAMMER_LEFT_X, UPGRADE_HAMMER_LEFT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H, animatedSprite, basicTowerButtonFn, basicButtonHoverFn);

    } else {
        buildMenu->position = (Vec2d){UPGRADE_SCROLL_RIGHT_X, UPGRADE_SCROLL_ORIGIN_Y};
        buildMenu->background = newSpriteXPM((xpm_map_t)upgradeMenuXPM[5], XPM_8_8_8, 0, 0);
        buildMenu->button = buttonNew(UPGRADE_HAMMER_RIGHT_X, UPGRADE_HAMMER_RIGHT_Y, UPGRADE_HAMMER_W, UPGRADE_HAMMER_H, animatedSprite, longTowerButtonFn, basicButtonHoverFn);
    }

	return buildMenu;
}

void buildMenuFree(BuildMenu* buildMenu) {
    buttonFree(buildMenu->button);
    freeSpriteXPM(buildMenu->background);
    free(buildMenu);
}

static void buttonHandler(BuildMenu* buildMenu) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressed(getCursor());

	if (boxIsCollidingWithPoint(buttonGetBoxCollider(buildMenu->button), mousePos)) {
		if (leftClick) buttonClick(buildMenu->button);
		buttonHover(buildMenu->button);

	} else {
		buttonReset(buildMenu->button);
	}
}

void buildMenuUpdate(BuildMenu* buildMenu, Terminal* terminal, Arena* arena, float deltaTime) {
	buttonHandler(buildMenu);
    drawSpriteXPM(buildMenu->background, terminal, buildMenu->position.x, buildMenu->position.y, false);
    buttonDraw(buildMenu->button, terminal, deltaTime, false);
}

void buildMenuReset(BuildMenu* buildMenu) {
	buttonReset(buildMenu->button);
}

bool buildMenuIsPressed(BuildMenu* buildMenu) {
	return buttonIsPressed(buildMenu->button);
}
