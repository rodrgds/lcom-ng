#include "sprite.h"

Sprite *createSprite(xpm_map_t sprite)
{
        Sprite *sp = (Sprite *)malloc(sizeof(Sprite));
        if (sp == NULL)
                return NULL;

        xpm_image_t img;
        sp->map = (uint32_t *)xpm_load(sprite, XPM_8_8_8_8, &img);

        if (sp->map == NULL)
        {
                free(sp);
                return NULL;
        }

        sp->height = img.height;
        sp->width = img.width;
        return sp;
}

void destroy_sprite(Sprite *sp)
{
        if (sp == NULL)
                return;
        if (sp->map)
                free(sp->map);
        free(sp);
        // sp = NULL; here does nothing since 'sp' is a local copy of the pointer.
}

int drawSprite(Sprite *sprite, int x, int y)
{
        uint16_t height = sprite->height;
        uint16_t width = sprite->width;

        for (int h = 0; h < height; h++)
        {
                for (int w = 0; w < width; w++)
                {
                        uint32_t current_color = sprite->map[w + h * width];
                        if (current_color == DEFOULT_COLOR)
                                continue;

                        vg_draw_pixel(x + w, y + h, current_color);
                }
        }
        return 0;
}

animated_sprite_t *create_animated_sprite(char **xpms[], int num_frames, int start_x, int start_y)
{
        animated_sprite_t *anim = (animated_sprite_t *)malloc(sizeof(animated_sprite_t));
        if (anim == NULL)
                return NULL;

        anim->no_pixmaps = num_frames;
        anim->cur_pixmap = 0;
        anim->x = start_x;
        anim->y = start_y;

        for (int i = 0; i < num_frames; i++)
        {
                anim->pixmaps[i] = (sprite_t *)malloc(sizeof(sprite_t));

                xpm_image_t img;
                anim->pixmaps[i]->pixmap = (uint32_t *)xpm_load((xpm_map_t)xpms[i], XPM_8_8_8_8, &img);
                anim->pixmaps[i]->width = img.width;
                anim->pixmaps[i]->height = img.height;
        }

        return anim;
}

void destroy_animated_sprite(animated_sprite_t *anim)
{
        if (anim == NULL)
                return;

        for (int i = 0; i < anim->no_pixmaps; i++)
        {
                if (anim->pixmaps[i])
                {
                        if (anim->pixmaps[i]->pixmap)
                        {
                                free(anim->pixmaps[i]->pixmap);
                        }
                        free(anim->pixmaps[i]);
                }
        }

        free(anim);
}

void createAllSprites()
{
        background = createSprite((xpm_map_t)menu_xpm);
        cursor = createSprite((xpm_map_t)cursor_xpm);
        cursorClick = createSprite((xpm_map_t)cursorClick_xpm);
        Exit = createSprite((xpm_map_t)ExitNotPressed_xpm);
        ExitPressed = createSprite((xpm_map_t)ExitPressed_xpm);
        Start = createSprite((xpm_map_t)StartNotPressed_xpm);
        StartPressed = createSprite((xpm_map_t)StartPressed_xpm);
        multiplayer_not = createSprite((xpm_map_t)multiplayer_not_xpm);
        multiplayer = createSprite((xpm_map_t)multiplayer_xpm);
        wait = createSprite((xpm_map_t)wait_xpm);
        Loss = createSprite((xpm_map_t)Loss_xpm);
        Win = createSprite((xpm_map_t)Win_xpm);
        Tie = createSprite((xpm_map_t)Tie_xpm);
        edit_icon = createSprite((xpm_map_t)edit_icon_xpm);
        table = createSprite((xpm_map_t)table_xpm);
        table_pressed = createSprite((xpm_map_t)table_pressed_xpm);
        red_cross = createSprite((xpm_map_t)red_cross_xpm);
        refrashe = createSprite((xpm_map_t)refrashe_xpm);
        // https://opengameart.org/content/super-cool-robot
        //  magick convert robot_sheet.png -crop 9x1@ +repage frame_%d.xpm
        char **robot_xpms[9] = {
            frame_0, frame_1, frame_2,
            frame_3, frame_4, frame_5,
            frame_6, frame_7, frame_8};

        my_robot = create_animated_sprite(robot_xpms, 9, 100, 200);
}

void destroyAllSprites()
{
        destroy_sprite(background);
        destroy_sprite(cursor);
        destroy_sprite(Exit);
        destroy_sprite(cursorClick);
        destroy_sprite(ExitPressed);
        destroy_sprite(Start);
        destroy_sprite(StartPressed);
        destroy_sprite(multiplayer_not);
        destroy_sprite(multiplayer);
        destroy_sprite(wait);
        destroy_sprite(Hit);
        destroy_sprite(HitPressed);
        destroy_sprite(Stand);
        destroy_sprite(StandPressed);
        destroy_sprite(Loss);
        destroy_sprite(Win);
        destroy_sprite(Tie);
        destroy_sprite(hand);
        destroy_sprite(handCloused);
        destroy_sprite(edit_icon);
        destroy_sprite(doubleDot);
        destroy_sprite(table);
        destroy_sprite(table_pressed);
        destroy_sprite(red_cross);
        destroy_sprite(refrashe);

        destroy_animated_sprite(my_robot);
}
