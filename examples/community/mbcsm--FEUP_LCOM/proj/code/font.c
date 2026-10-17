#include <lcom/lcf.h>
#include "font.h"
#include "video.h"


//#define _GNU_SOURCE /* for using asprintf */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> /* for tolower */

xpm_image_t imgData;
uint16_t *data = NULL;

void printstring(char * text, int size, char * font , int x , int y) {
    uint16_t transp = xpm_transparency_color(XPM_5_6_5);

    xpm_image_t imgTemplate;
    uint16_t *aTemplate;

    aTemplate = (uint16_t*)xpm_load(zero_xpm, XPM_5_6_5, &imgTemplate);

    /*xpm_image_t imgData;
    uint16_t *data = NULL;*/
    printf("%s", font);
    for (int i = 0; i < size; i++){
        if (text[i] == ' '){
            x += imgTemplate.width;
            continue;
        }
        
        if (loadChar(text[i], font[0])){
            draw_xpm(x, y, data, imgData, transp);
        }
        else draw_xpm(x, y, aTemplate, imgTemplate, transp);
        //printf("1");

        x += imgTemplate.width;

        if (x + imgTemplate.width > 1280){
            x = 0;
            y += imgTemplate.height + 5;
        }
        if (y + imgTemplate.height > 1024)
            break;
    }
}

 bool loadChar(char textI, char font){
    textI = tolower((unsigned char)textI);

    if (font == 'b')
        switch (textI)
        {
        case '0':
            data = (uint16_t*)xpm_load(zero_xpm, XPM_5_6_5, &imgData);
            break;
        case '1':
            data = (uint16_t*)xpm_load(one_xpm, XPM_5_6_5, &imgData);
            break;
        case '2':
            data = (uint16_t*)xpm_load(two_xpm, XPM_5_6_5, &imgData);
            break;
        case '3':
            data = (uint16_t*)xpm_load(three_xpm, XPM_5_6_5, &imgData);
            break;
        case '4':
            data = (uint16_t*)xpm_load(four_xpm, XPM_5_6_5, &imgData);
            break;
        case '5':
            data = (uint16_t*)xpm_load(five_xpm, XPM_5_6_5, &imgData);
            break;
        case '6':
            data = (uint16_t*)xpm_load(six_xpm, XPM_5_6_5, &imgData);
            break;
        case '7':
            data = (uint16_t*)xpm_load(seven_xpm, XPM_5_6_5, &imgData);
            break;
        case '8':
            data = (uint16_t*)xpm_load(eight_xpm, XPM_5_6_5, &imgData);
            break;
        case '9':
            data = (uint16_t*)xpm_load(nine_xpm, XPM_5_6_5, &imgData);
            break;
        case 'a':
            data = (uint16_t*)xpm_load(a_xpm, XPM_5_6_5, &imgData);
            break;
        case 'b':
            data = (uint16_t*)xpm_load(b_xpm, XPM_5_6_5, &imgData);
            break;
        case 'c':
            data = (uint16_t*)xpm_load(c_xpm, XPM_5_6_5, &imgData);
            break;
        case 'd':
            data = (uint16_t*)xpm_load(d_xpm, XPM_5_6_5, &imgData);
            break;
        case 'e':
            data = (uint16_t*)xpm_load(e_xpm, XPM_5_6_5, &imgData);
            break;
        case 'f':
            data = (uint16_t*)xpm_load(f_xpm, XPM_5_6_5, &imgData);
            break;
        case 'g':
            data = (uint16_t*)xpm_load(g_xpm, XPM_5_6_5, &imgData);
            break;
        case 'h':
            data = (uint16_t*)xpm_load(h_xpm, XPM_5_6_5, &imgData);
            break;
        case 'i':
            data = (uint16_t*)xpm_load(i_xpm, XPM_5_6_5, &imgData);
            break;
        case 'j':
            data = (uint16_t*)xpm_load(j_xpm, XPM_5_6_5, &imgData);
            break;
        case 'k':
            data = (uint16_t*)xpm_load(k_xpm, XPM_5_6_5, &imgData);
            break;
        case 'l':
            data = (uint16_t*)xpm_load(l_xpm, XPM_5_6_5, &imgData);
            break;
        case 'm':
            data = (uint16_t*)xpm_load(m_xpm, XPM_5_6_5, &imgData);
            break;
        case 'n':
            data = (uint16_t*)xpm_load(n_xpm, XPM_5_6_5, &imgData);
            break;
        case 'o':
            data = (uint16_t*)xpm_load(o_xpm, XPM_5_6_5, &imgData);
            break;
        case 'p':
            data = (uint16_t*)xpm_load(p_xpm, XPM_5_6_5, &imgData);
            break;
        case 'q':
            data = (uint16_t*)xpm_load(q_xpm, XPM_5_6_5, &imgData);
            break;
        case 'r':
            data = (uint16_t*)xpm_load(r_xpm, XPM_5_6_5, &imgData);
            break;
        case 's':
            data = (uint16_t*)xpm_load(s_xpm, XPM_5_6_5, &imgData);
            break;
        case 't':
            data = (uint16_t*)xpm_load(t_xpm, XPM_5_6_5, &imgData);
            break;
        case 'u':
            data = (uint16_t*)xpm_load(u_xpm, XPM_5_6_5, &imgData);
            break;
        case 'v':
            data = (uint16_t*)xpm_load(v_xpm, XPM_5_6_5, &imgData);
            break;
        case 'x':
            data = (uint16_t*)xpm_load(x_xpm, XPM_5_6_5, &imgData);
            break;
        case 'y':
            data = (uint16_t*)xpm_load(y_xpm, XPM_5_6_5, &imgData);
            break;
        case 'z':
            data = (uint16_t*)xpm_load(z_xpm, XPM_5_6_5, &imgData);
            break;
        case '/':
            data = (uint16_t*)xpm_load(slash_xpm, XPM_5_6_5, &imgData);
            break;
        case '.':
            //data = (uint16_t*)xpm_load(dot_xpm, XPM_5_6_5, &imgData);
            break;
        case ':':
            data = (uint16_t*)xpm_load(colon_xpm, XPM_5_6_5, &imgData);
            break;

    
        default:
            return false;
        }
    else if (font == 'w')
        switch (textI)
    {
        case '0':
            data = (uint16_t*)xpm_load(zerow_xpm, XPM_5_6_5, &imgData);
            break;
        case '1':
            data = (uint16_t*)xpm_load(onew_xpm, XPM_5_6_5, &imgData);
            break;
        case '2':
            data = (uint16_t*)xpm_load(twow_xpm, XPM_5_6_5, &imgData);
            break;
        case '3':
            data = (uint16_t*)xpm_load(threew_xpm, XPM_5_6_5, &imgData);
            break;
        case '4':
            data = (uint16_t*)xpm_load(fourw_xpm, XPM_5_6_5, &imgData);
            break;
        case '5':
            data = (uint16_t*)xpm_load(fivew_xpm, XPM_5_6_5, &imgData);
            break;
        case '6':
            data = (uint16_t*)xpm_load(sixw_xpm, XPM_5_6_5, &imgData);
            break;
        case '7':
            data = (uint16_t*)xpm_load(sevenw_xpm, XPM_5_6_5, &imgData);
            break;
        case '8':
            data = (uint16_t*)xpm_load(eightw_xpm, XPM_5_6_5, &imgData);
            break;
        case '9':
            data = (uint16_t*)xpm_load(ninew_xpm, XPM_5_6_5, &imgData);
            break;
        case 'a':
            data = (uint16_t*)xpm_load(aw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'b':
            data = (uint16_t*)xpm_load(bw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'c':
            data = (uint16_t*)xpm_load(cw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'd':
            data = (uint16_t*)xpm_load(dw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'e':
            data = (uint16_t*)xpm_load(ew_xpm, XPM_5_6_5, &imgData);
            break;
        case 'f':
            data = (uint16_t*)xpm_load(fw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'g':
            data = (uint16_t*)xpm_load(gw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'h':
            data = (uint16_t*)xpm_load(hw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'i':
            data = (uint16_t*)xpm_load(iw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'j':
            data = (uint16_t*)xpm_load(jw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'k':
            data = (uint16_t*)xpm_load(kw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'l':
            data = (uint16_t*)xpm_load(lw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'm':
            data = (uint16_t*)xpm_load(mw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'n':
            data = (uint16_t*)xpm_load(nw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'o':
            data = (uint16_t*)xpm_load(ow_xpm, XPM_5_6_5, &imgData);
            break;
        case 'p':
            data = (uint16_t*)xpm_load(pw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'q':
            data = (uint16_t*)xpm_load(qw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'r':
            data = (uint16_t*)xpm_load(rw_xpm, XPM_5_6_5, &imgData);
            break;
        case 's':
            data = (uint16_t*)xpm_load(sw_xpm, XPM_5_6_5, &imgData);
            break;
        case 't':
            data = (uint16_t*)xpm_load(tw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'u':
            data = (uint16_t*)xpm_load(uw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'v':
            data = (uint16_t*)xpm_load(vw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'x':
            data = (uint16_t*)xpm_load(xw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'y':
            data = (uint16_t*)xpm_load(yw_xpm, XPM_5_6_5, &imgData);
            break;
        case 'z':
            data = (uint16_t*)xpm_load(zw_xpm, XPM_5_6_5, &imgData);
            break;
        case '/':
            data = (uint16_t*)xpm_load(slashw_xpm, XPM_5_6_5, &imgData);
            break;
        case '.':
            data = (uint16_t*)xpm_load(dotw_xpm, XPM_5_6_5, &imgData);
            break;
        case ':':
            data = (uint16_t*)xpm_load(colonw_xpm, XPM_5_6_5, &imgData);
            break;

    
        default:
            return false;
    }
    return true;
}
