#include "text.h"


#define XPM_ENTRY(ch,c) { (ch), character_##c##_xpm }

typedef struct {
    char        character;
    const char **xpm;
} CharMap;

static const CharMap char_map[] = {
    XPM_ENTRY('A',A), XPM_ENTRY('B',B), XPM_ENTRY('C',C), XPM_ENTRY('D',D), XPM_ENTRY('E',E),
    XPM_ENTRY('F',F), XPM_ENTRY('G',G), XPM_ENTRY('H',H), XPM_ENTRY('I',I), XPM_ENTRY('J',J),
    XPM_ENTRY('K',K), XPM_ENTRY('L',L), XPM_ENTRY('M',M), XPM_ENTRY('N',N), XPM_ENTRY('O',O),
    XPM_ENTRY('P',P), XPM_ENTRY('Q',Q), XPM_ENTRY('R',R), XPM_ENTRY('S',S), XPM_ENTRY('T',T),
    XPM_ENTRY('U',U), XPM_ENTRY('V',V), XPM_ENTRY('W',W), XPM_ENTRY('X',X), XPM_ENTRY('Y',Y),
    XPM_ENTRY('Z',Z),
    XPM_ENTRY('0',0), XPM_ENTRY('1',1), XPM_ENTRY('2',2), XPM_ENTRY('3',3), XPM_ENTRY('4',4),
    XPM_ENTRY('5',5), XPM_ENTRY('6',6), XPM_ENTRY('7',7), XPM_ENTRY('8',8), XPM_ENTRY('9',9),
};
static const size_t char_map_n = sizeof(char_map) / sizeof(*char_map);

static const char **get_character_xpm(char c) {
    char key = toupper((unsigned char)c);
    for (size_t i = 0; i < char_map_n; i++) {
        if (char_map[i].character == key) {
            return char_map[i].xpm;
        }
    }
    return NULL;
}

static void drawText(Text *self) {
    if (!self || !self->characters) return;

    for (int i = 0; i < self->size; i++) {
        Sprite* characterSprite= self->characters[i];
        if (characterSprite) {
            characterSprite->draw(characterSprite);
        }
    }
}



static void destroyText(Text *self) {
    if (!self) return;
    for (int i = 0; i < self->size; i++) {
        if (self->characters[i]) {
            self->characters[i]->destroy(self->characters[i]);
        }
    }
    free(self->characters);
    free(self->text);
    free(self);
}

void updateText(Text* self, const char* newText) {
    if (!self) return;

    // Destroy old sprites
    for (int i = 0; i < self->size; i++) {
        if (self->characters[i])
            self->characters[i]->destroy(self->characters[i]);
    }
    free(self->characters);

    self->size = (int)strlen(newText);
    self->text = strdup(newText);

    self->characters = calloc(self->size, sizeof(Sprite *));
    if (!self->text || !self->characters) {
        destroyText(self);
        return;
    }

    int x = self->initial_x;
    for (int i = 0; i < self->size; i++) {
        if(newText[i]==' '){
            x+=64+self->spacing;
            continue;
        }
        const char **xpm = get_character_xpm(newText[i]);
        if (!xpm) continue; 
        Sprite *spr = create_sprite(xpm, x, self->initial_y, 0, 0);
        if (spr) {
            self->characters[i] = spr;
            x += spr->width + self->spacing;
        }
    }
    
}

Text *create_Text(const char *text, int initial_x, int initial_y, int spacing) {
    if (!text) return NULL;
    Text *obj = malloc(sizeof(Text));
    if (!obj) return NULL;

    obj->size = (int)strlen(text);
    obj->text = strdup(text);
    obj->initial_x = initial_x;
    obj->initial_y = initial_y;
    obj->spacing = spacing;
    obj->characters = calloc(obj->size, sizeof(Sprite *));
    if (!obj->text || !obj->characters) {
        destroyText(obj);
        return NULL;
    }

    obj->draw = drawText;
    obj->destroy = destroyText;
    obj->updateText= updateText;

    int x = initial_x;
    for (int i = 0; i < obj->size; i++) {
        if(text[i]==' '){
            x+=64+spacing;
            continue;
        }
        const char **xpm = get_character_xpm(text[i]);
        if (!xpm) continue; 
        Sprite *spr = create_sprite(xpm, x, initial_y, 0, 0);
        if (spr) {
            obj->characters[i] = spr;
            x += spr->width + spacing;
        }
    }
    return obj;
}
