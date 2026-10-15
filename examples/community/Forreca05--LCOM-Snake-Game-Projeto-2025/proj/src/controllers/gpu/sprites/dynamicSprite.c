#include "dynamicSprite.h"

static int drawDynamicSprite(DynamicSprite *self)
{
    return self->sprite.draw(&self->sprite);
}

static void destroyDynamicSprite(DynamicSprite *self)
{
    if (!self)
        return;
    if (self->originalMap)
        free(self->originalMap);
    if (self->hoverMap)
        free(self->hoverMap);
    free(self);
}

static void setHover(DynamicSprite *self, bool hover)
{
    if (self->isHover == hover)
    {
        return;
    }

    self->sprite.map = hover ? self->hoverMap : self->originalMap;
    self->isHover = hover;
}

static bool isInsideDynamicSprite(DynamicSprite *self, int x, int y)
{
    int bx = self->sprite.x;
    int by = self->sprite.y;
    int bw = self->sprite.width;
    int bh = self->sprite.height;

    if (x >= bx && x < bx + bw &&
        y >= by && y < by + bh)
        return true;
    return false;
}

DynamicSprite *create_dynamicSprite(const char *xpm[], int x, int y, int xspeed, int yspeed, uint32_t fromColor, uint32_t toColor)
{

    DynamicSprite *self = malloc(sizeof *self);
    if (!self)
        return NULL;

    Sprite *s = create_sprite(xpm, x, y, xspeed, yspeed);
    if (!s)
    {
        free(self);
        return NULL;
    }

    // set sprite to dinamic sprite sprite
    self->sprite = *s;
    self->originalMap = self->sprite.map;
    free(s);

    size_t total = self->sprite.width * self->sprite.height; // max space
    self->hoverMap = malloc(total * sizeof *self->hoverMap);

    for (int i = 0; i < self->sprite.height; i++)
    {
        for (int j = 0; j < self->sprite.width; j++)
        {
            uint32_t color = self->sprite.map[i * self->sprite.width + j];
            if (color == fromColor)
            {
                self->hoverMap[i * self->sprite.width + j] = toColor;
            }
            else
            {
                self->hoverMap[i * self->sprite.width + j] = self->originalMap[i * self->sprite.width + j];
            }
        }
    }

    self->draw = drawDynamicSprite;
    self->destroy = destroyDynamicSprite;
    self->setHover = setHover;
    self->isInside = isInsideDynamicSprite;
    self->isHover = false;

    return self;
}
