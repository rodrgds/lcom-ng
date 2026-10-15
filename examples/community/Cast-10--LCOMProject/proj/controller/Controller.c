#include "Controller.h"
#include "ButtonController.h"

Controller *controllerCreate() {
    Controller *ctrl = malloc(sizeof(Controller));
    if (!ctrl) return NULL;

    ctrl->gameCtrl = NULL;
    ctrl->viewer = NULL;
    ctrl->shopCtrl = NULL;

    if (!stateInit(&ctrl->state)) {
        free(ctrl);
        return NULL;
    }

    ctrl->state.state = STATE_MENU;

    keyboardStateInit(&ctrl->kbd);
    mouseStateInit(&ctrl->mouse, MODE105_WIDTH / 2, MODE105_HEIGHT / 2);

    ctrl->gameCtrl = gameControllerCreate(MODE105_WIDTH, MODE105_HEIGHT);
    if (!ctrl->gameCtrl) {
        stateDestroy(&ctrl->state);
        free(ctrl);
        return NULL;
    }

    ctrl->shopCtrl = shopControllerCreate();
    if (!ctrl->shopCtrl) {
        gameControllerDestroy(ctrl->gameCtrl);
        stateDestroy(&ctrl->state);
        free(ctrl);
        return NULL;
    }

    ctrl->viewer = viewerCreate();
    if (!ctrl->viewer) {
        shopControllerDestroy(ctrl->shopCtrl);
        gameControllerDestroy(ctrl->gameCtrl);
        stateDestroy(&ctrl->state);
        free(ctrl);
        return NULL;
    }

    return ctrl;
}

void controllerUpdate(Controller *ctrl, bool second_elapsed) {
    if (!ctrl) return;

    if (ctrl->kbd.exit) {
        gameQuit(&ctrl->state);
        return;
    }

    if (ctrl->kbd.shop) {
        if (ctrl->state.state == STATE_PLAYING) {
            gamePause(&ctrl->state);
            mouseClearEvents(&ctrl->mouse);
        } 
        else if (ctrl->state.state == STATE_PAUSED) {
            gameResume(&ctrl->state);
            mouseClearEvents(&ctrl->mouse);
        }

        ctrl->kbd.shop = false;
    }

    switch (ctrl->state.state) {
        case STATE_MENU:
            menuControllerUpdate(ctrl->state.menu, &ctrl->kbd, &ctrl->mouse);

            if (menuStateShouldStart(ctrl->state.menu)) {
                menuStateReset(ctrl->state.menu);
                mouseClearEvents(&ctrl->mouse);
                gameStart(&ctrl->state);
            }

            break;

        case STATE_PLAYING: {
            bool p1_looped = false;
            bool p2_looped = false;

            gameViewerUpdate(
                ctrl->viewer->gameViewer,
                ctrl->state.game->p1,
                ctrl->state.game->p2,
                &ctrl->state.game->minions,
                &p1_looped,
                &p2_looped
            );

            gameControllerUpdate(
                ctrl->gameCtrl,
                ctrl->state.game->p1,
                ctrl->state.game->p2,
                &ctrl->state.game->minions,
                &ctrl->state.game->towers,
                &ctrl->kbd,
                &ctrl->mouse,
                second_elapsed,
                p1_looped,
                p2_looped
            );

            if (ctrl->state.game->towers.blue.hp <= 0) {
                mouseClearEvents(&ctrl->mouse);
                gameOver(&ctrl->state, TEAM_RED);
            } else if (ctrl->state.game->towers.red.hp <= 0) {
                mouseClearEvents(&ctrl->mouse);
                gameOver(&ctrl->state, TEAM_BLUE);
            }

            break;
        }

        case STATE_PAUSED:
            shopControllerUpdate(ctrl->shopCtrl, ctrl->state.shop, &ctrl->mouse, ctrl->state.game);
            break;

        case STATE_GAME_OVER:
            buttonControllerUpdate(ctrl->state.gameOver->restartButton, &ctrl->mouse);
            buttonControllerUpdate(ctrl->state.gameOver->quitButton, &ctrl->mouse);

            if (ctrl->state.gameOver->restartButton->clicked) {
                gameRestart(&ctrl->state);
            } else if (ctrl->state.gameOver->quitButton->clicked) {
                gameQuit(&ctrl->state);
            }
            break;
    }

    mouseClearEvents(&ctrl->mouse);
}

void controllerDraw(Controller *ctrl) {
    if (!ctrl) return;

    viewerDraw(
        ctrl->viewer,
        &ctrl->state,
        ctrl->mouse.x,
        ctrl->mouse.y
    );
}

void controllerDestroy(Controller *ctrl) {
    if (!ctrl) return;

    if (ctrl->viewer) {
        viewerDestroy(ctrl->viewer);
    }

    if (ctrl->shopCtrl) {
        shopControllerDestroy(ctrl->shopCtrl);
    }

    if (ctrl->gameCtrl) {
        gameControllerDestroy(ctrl->gameCtrl);
    }

    stateDestroy(&ctrl->state);

    free(ctrl);
}
