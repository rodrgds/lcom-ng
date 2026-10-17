#include "menu.h"
#include <ctype.h>
extern uint8_t scancode;
extern unsigned int h_res, v_res;
extern int COUNTER;
extern uint8_t last;
extern int score;
struct packet pack;
char wordletter[10];
int word[10];
int wordsize=0;
int letterx = 150;
int lettery = 300;

extern Cursor *cursor;
extern GameState gameState;

void(loadMainMenu)() {
  xpm_load(menu_xpm, XPM_8_8_8, &menu_img);
  xpm_load(highlight_button_xpm, XPM_8_8_8, &highlight_button_img);
  xpm_load(logotip_xpm, XPM_8_8_8, &logotip_img);
  xpm_load(black_background_xpm,XPM_8_8_8,&black_background_img); 
  cursor = create_cursor();
  draw_main_menu_background();
  draw_cursor();
}

void(draw_main_menu_background)() {
  draw_xpm(black_background_img.bytes,&black_background_img,0,0);
  draw_xpm(logotip_img.bytes, &logotip_img, 250, 50);
  draw_xpm(menu_img.bytes, &menu_img, 0, 0);
}

void MainMenuInterruptHandler(Device device) {
  static struct mouse_ev *mouseEvent;
  static bool overExit = false, overPlay = false, overScoreboard = false;
  bool clickedExit = false, clickedPlay = false, clickedScoreboard = false;

  switch (device) {
    case TIMER:
      if (COUNTER % 4 == 0) {
        switch (check_collision_main_menu()) {
          case 0: // cursor is not in any button
            if (overExit) {
              overExit = false;
              delete_xpm(highlight_button_img, 440, 435); // deletes the highlight for 'EXIT'
            }
            else if (overPlay) {
              overPlay = false;
              delete_xpm(highlight_button_img, 450, 205); // deletes the higtlight  for 'PLAY'
            }
            else if (overScoreboard) {
              overScoreboard = false;
              delete_xpm(highlight_button_img, 530, 320); // deletes the highlight for 'SCOREBOARD'
            }
            break;
          case 1: // cursor is over 'PLAY'
            if (mouseEvent->type == LB_RELEASED) {
              clickedPlay = true;
              break;
            }
            if (!overPlay) {
              overPlay = true;
            }
            break;
          case 2: // cursor is over 'SCOREBOARD'
            if (mouseEvent->type == LB_RELEASED) {
              clickedScoreboard = true;
              break;
            }
            if (!overScoreboard) {
              overScoreboard = true;
            }
            break;

          case 3: // cursor is over 'EXIT'
            if (mouseEvent->type == LB_RELEASED) {
              clickedExit = true;
              break;
            }
            if (!overExit) {
              overExit = true;
            }
            break;
        }

        clean_cursor();

        draw_main_menu_background();

        if (overPlay) {
          draw_xpm(highlight_button_img.bytes, &highlight_button_img, 450, 205);
        }
        else if (overScoreboard) {
          draw_xpm(highlight_button_img.bytes, &highlight_button_img, 530, 320);
        }
        else if (overExit) {
          draw_xpm(highlight_button_img.bytes, &highlight_button_img, 440, 435);
        }

        draw_cursor();
      }
      break;
    case KEYBOARD:
      break;
    case MOUSE:
      mouseEvent = mouse_events(&pack);
      mouse_update(&pack);
      break;
    case RTC:
      print_clock();
      break;
  }

  if (clickedExit) { // Sair
    gameState = EXIT;
  }

  else if (clickedPlay) { // Entrar no modo PLAY
    gameState = PLAY;
    wordsize = 0;
    letterx = 150;
    lettery = 300;
    for(int i = 0;i<10;i++){
      wordletter[i]='\0';
    }
    loadGame();
    printf("loaded game\n");
    return;
  }

  else if (clickedScoreboard) { // Entrar no Scoreboard
    gameState = SCOREBOARD;
    draw_scoreboard();
    return;
  }
}

void ScoreBoardInterruptHandler(Device device) {
  
  switch (device) {
    case TIMER:
      break;
    case KEYBOARD:
      if (last == ESC) {
        gameState = MAINMENU;
        delete_xpm(black_background_img,0,0);
        score = 0;
        loadMainMenu();
      }
      break;
    case MOUSE:
      break;
    case RTC:
      break;
  }
}

void GameLostInterruptHandler(Device device) {

  switch (device) {
    case TIMER:
      break;
    case KEYBOARD:
      if (last == ESC) {
        gameState = SAVE;
        delete_xpm(yourScoreWas_img,206,300);
        delete_xpm(youLost_img,265,230);
        delete_xpm(black_background_img,0,0);
        loadSaveMenu();
      }
      break;
    case MOUSE:
      break;
    case RTC:
      break;
  }

}

void SaveInterruptHandler(Device device) {
  switch (device) {
    case TIMER:
      break;
    case KEYBOARD:
      printf("%d",last);
      if (last == ENTER) {
        savescore(score,wordletter);
        gameState = MAINMENU;
        delete_xpm(yourScoreWas_img,206,300);
        delete_xpm(youLost_img,265,230);
        delete_xpm(black_background_img,0,0);
        score = 0;
        loadMainMenu();
      }
      else{
        (letterhandle)(last);
      }
      break;
    case MOUSE:
      break;
    case RTC:
      break;
  }

}

void (loadLostMenu)(){
  xpm_load(black_background_xpm,XPM_8_8_8,&black_background_img);
  xpm_load(yourScoreWas_xpm,XPM_8_8_8,&yourScoreWas_img);
  xpm_load(youLost_xpm,XPM_8_8_8,&youLost_img);
  draw_xpm(black_background_img.bytes,&black_background_img,0,0);
  draw_xpm(youLost_img.bytes,&youLost_img,265,230);
  draw_xpm(yourScoreWas_img.bytes,&yourScoreWas_img,206,300);
  draw_score(585,305);
}
void (loadSaveMenu)(){
  xpm_load(writename,XPM_8_8_8,&name);
  xpm_load(black_background_xpm,XPM_8_8_8,&black_background_img);
  xpm_load(a_xpm,XPM_8_8_8,&a);
  xpm_load(b_xpm,XPM_8_8_8,&b);
  xpm_load(c_xpm,XPM_8_8_8,&c);
  xpm_load(d_xpm,XPM_8_8_8,&d);
  xpm_load(e_xpm,XPM_8_8_8,&e);
  xpm_load(f_xpm,XPM_8_8_8,&f);
  xpm_load(g_xpm,XPM_8_8_8,&g);
  xpm_load(h_xpm,XPM_8_8_8,&h);
  xpm_load(i_xpm,XPM_8_8_8,&i);
  xpm_load(j_xpm,XPM_8_8_8,&j);
  xpm_load(k_xpm,XPM_8_8_8,&k);
  xpm_load(l_xpm,XPM_8_8_8,&l);
  xpm_load(m_xpm,XPM_8_8_8,&m);
  xpm_load(n_xpm,XPM_8_8_8,&n);
  xpm_load(o_xpm,XPM_8_8_8,&o);
  xpm_load(p_xpm,XPM_8_8_8,&p);
  xpm_load(q_xpm,XPM_8_8_8,&q);
  xpm_load(r_xpm,XPM_8_8_8,&r);
  xpm_load(s_xpm,XPM_8_8_8,&s);
  xpm_load(t_xpm,XPM_8_8_8,&t);
  xpm_load(u_xpm,XPM_8_8_8,&u);
  xpm_load(v_xpm,XPM_8_8_8,&v);
  xpm_load(w_xpm,XPM_8_8_8,&w);
  xpm_load(x_xpm,XPM_8_8_8,&x);
  xpm_load(y_xpm,XPM_8_8_8,&y);
  xpm_load(z_xpm,XPM_8_8_8,&z);
  draw_xpm(black_background_img.bytes,&black_background_img,0,0);
  draw_xpm(name.bytes,&name,0,0);
}

void (savescore)(int score, char* name){
  FILE* fp;

  fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "a");

  if (fp == NULL){
    printf("Highscore file not found\n");
    return;
  } 
  fprintf(fp, "%d - %s\n", score,name);
  fclose(fp);
}
void draw_scoreboard(){
  xpm_load(scoreboard,XPM_8_8_8,&scoreboard_img);
  xpm_load(a_xpm,XPM_8_8_8,&a);
  xpm_load(b_xpm,XPM_8_8_8,&b);
  xpm_load(c_xpm,XPM_8_8_8,&c);
  xpm_load(d_xpm,XPM_8_8_8,&d);
  xpm_load(e_xpm,XPM_8_8_8,&e);
  xpm_load(f_xpm,XPM_8_8_8,&f);
  xpm_load(g_xpm,XPM_8_8_8,&g);
  xpm_load(h_xpm,XPM_8_8_8,&h);
  xpm_load(i_xpm,XPM_8_8_8,&i);
  xpm_load(j_xpm,XPM_8_8_8,&j);
  xpm_load(k_xpm,XPM_8_8_8,&k);
  xpm_load(l_xpm,XPM_8_8_8,&l);
  xpm_load(m_xpm,XPM_8_8_8,&m);
  xpm_load(n_xpm,XPM_8_8_8,&n);
  xpm_load(o_xpm,XPM_8_8_8,&o);
  xpm_load(p_xpm,XPM_8_8_8,&p);
  xpm_load(q_xpm,XPM_8_8_8,&q);
  xpm_load(r_xpm,XPM_8_8_8,&r);
  xpm_load(s_xpm,XPM_8_8_8,&s);
  xpm_load(t_xpm,XPM_8_8_8,&t);
  xpm_load(u_xpm,XPM_8_8_8,&u);
  xpm_load(v_xpm,XPM_8_8_8,&v);
  xpm_load(w_xpm,XPM_8_8_8,&w);
  xpm_load(x_xpm,XPM_8_8_8,&x);
  xpm_load(y_xpm,XPM_8_8_8,&y);
  xpm_load(z_xpm,XPM_8_8_8,&z);
  xpm_load(zero_xpm,XPM_8_8_8,&numbers_img[0]);
  xpm_load(one_xpm,XPM_8_8_8,&numbers_img[1]);
  xpm_load(two_xpm,XPM_8_8_8,&numbers_img[2]);
  xpm_load(three_xpm,XPM_8_8_8,&numbers_img[3]);
  xpm_load(four_xpm,XPM_8_8_8,&numbers_img[4]);
  xpm_load(five_xpm,XPM_8_8_8,&numbers_img[5]);
  xpm_load(six_xpm,XPM_8_8_8,&numbers_img[6]);
  xpm_load(seven_xpm,XPM_8_8_8,&numbers_img[7]);
  xpm_load(eight_xpm,XPM_8_8_8,&numbers_img[8]);
  xpm_load(nine_xpm,XPM_8_8_8,&numbers_img[9]);
 xpm_load(black_background_xpm,XPM_8_8_8,&black_background_img);
 draw_xpm(black_background_img.bytes,&black_background_img,0,0);
 draw_xpm(scoreboard_img.bytes,&scoreboard_img,0,0);
 int liney = 230;
 FILE* fp;

  fp = fopen("/home/lcom/labs/proj/src/highscores.txt", "r");

  if (fp == NULL){
    printf("Highscore file not found\n");
    return;
  }
  char line[50];
  char name[50];
  char number[50];
  for (int i = 0; i<50;i++){
      name[i] = '\0';
      line[i] = '\0';
      number[i] = '\0';
    }
  while (fgets(line, 50, fp)) {
    int i = 0 , j = 0;
    while(line[i]!=' '){
      number[i] = line[i];
      i++;
    }
    i+=3;
    while(line[i]!='\n'){
      name[j] = line[i];
      i++;
      j++;
    }
    draw_number(number,liney);
    draw_name(name,liney);
    printf("number is %s and name is %s\n",number,name);
    for (int i = 0; i<50;i++){
      name[i] = '\0';
      line[i] = '\0';
      number[i] = '\0';
    }
    
    liney+=45;
  }
    
  fclose(fp);
}
void draw_number(char *number,int liney){
  int i = 0;
  int x = 80;
  while(number[i]!='\0'){
    printf("%d\n",(int)number[i]-48);
    draw_xpm(numbers_img[(int)number[i]-48].bytes,&numbers_img[(int)number[i]-48],x,liney);
    printf("%d\n",(int)number[i]-48);
    x+=numbers_img[(int)number[i]-48].width;
    i++;
    
  }
}
void draw_name(char *name,int liney){
  int index = 0;
  int linex = 450;
  while(name[index]!='\0'){
    printf("%c\n",name[index]);
    if(name[index]=='a'){
      draw_xpm(a.bytes,&a,linex,liney);
      linex+=a.width;
    }
    else if(name[index]=='b'){
      draw_xpm(b.bytes,&b,linex,liney);
      linex+=b.width;
    }
    else if(name[index]=='c'){
      draw_xpm(c.bytes,&c,linex,liney);
      linex+=c.width;
    }
    else if(name[index]=='d'){
      draw_xpm(d.bytes,&d,linex,liney);
      linex+=d.width;
    }
    else if(name[index]=='e'){
      draw_xpm(e.bytes,&e,linex,liney);
      linex+=e.width;
    }
    else if(name[index]=='f'){
      draw_xpm(f.bytes,&f,linex,liney);
      linex+=f.width;
    }
    else if(name[index]=='g'){
      draw_xpm(g.bytes,&g,linex,liney);
      linex+=g.width;
    }
    else if(name[index]=='h'){
      draw_xpm(h.bytes,&h,linex,liney);
      linex+=h.width;
    }
    else if(name[index]=='i'){
      draw_xpm(i.bytes,&i,linex,liney);
      linex+=i.width;
    }
    else if(name[index]=='j'){
      draw_xpm(j.bytes,&j,linex,liney);
      linex+=j.width;
    }
    else if(name[index]=='k'){
      draw_xpm(k.bytes,&k,linex,liney);
      linex+=k.width;
    }
    else if(name[index]=='l'){
      draw_xpm(l.bytes,&l,linex,liney);
      linex+=l.width;
    }
    else if(name[index]=='m'){
      draw_xpm(m.bytes,&m,linex,liney);
      linex+=m.width;
    }
    else if(name[index]=='n'){
      draw_xpm(n.bytes,&n,linex,liney);
      linex+=n.width;
    }
    else if(name[index]=='o'){
      draw_xpm(o.bytes,&o,linex,liney);
      linex+=o.width;
    }
    else if(name[index]=='p'){
      draw_xpm(p.bytes,&p,linex,liney);
      linex+=p.width;
    }
    else if(name[index]=='q'){
      draw_xpm(q.bytes,&q,linex,liney);
      linex+=q.width;
    }
    else if(name[index]=='r'){
      draw_xpm(r.bytes,&r,linex,liney);
      linex+=r.width;
    }
    else if(name[index]=='s'){
      draw_xpm(s.bytes,&s,linex,liney);
      linex+=s.width;
    }
    else if(name[index]=='t'){
      draw_xpm(t.bytes,&t,linex,liney);
      linex+=t.width;
    }
    else if(name[index]=='u'){
      draw_xpm(u.bytes,&u,linex,liney);
      linex+=u.width;
    }
    else if(name[index]=='v'){
      draw_xpm(v.bytes,&v,linex,liney);
      linex+=v.width;
    }
    else if(name[index]=='w'){
      draw_xpm(w.bytes,&w,linex,liney);
      linex+=w.width;
    }
    else if(name[index]=='x'){
      draw_xpm(x.bytes,&x,linex,liney);
      linex+=x.width;
    }
    else if(name[index]=='y'){
      draw_xpm(y.bytes,&y,linex,liney);
      linex+=y.width;
    }
    else if(name[index]=='z'){
      draw_xpm(z.bytes,&z,linex,liney);
      linex+=z.width;
    }
    index++;
  }
}

void(letterhandle)(uint8_t lastletter) {
  if (lastletter == A && wordsize < 10) {
    draw_xpm(a.bytes, &a, letterx, lettery);
    word[wordsize] = a.width;
    wordletter[wordsize] = 'a';
    letterx += a.width;
    wordsize++;
  }
  else if (lastletter == B && wordsize < 10) {
    draw_xpm(b.bytes, &b, letterx, lettery);
    word[wordsize] = b.width;
    wordletter[wordsize] = 'b';
    letterx += b.width;
    wordsize++;
  }
  else if (lastletter == C && wordsize < 10) {
    draw_xpm(c.bytes, &c, letterx, lettery);
    word[wordsize] = c.width;
    wordletter[wordsize] = 'c';
    letterx += c.width;
    wordsize++;
  }
  else if (lastletter == D && wordsize < 10) {
    draw_xpm(d.bytes, &d, letterx, lettery);
    word[wordsize] = d.width;
    wordletter[wordsize] = 'd';
    letterx += d.width;
    wordsize++;
  }
  else if (lastletter == E && wordsize < 10) {
    draw_xpm(e.bytes, &e, letterx, lettery);
    word[wordsize] = e.width;
    wordletter[wordsize] = 'e';
    letterx += e.width;
    wordsize++;
  }
  else if (lastletter == F && wordsize < 10) {
    draw_xpm(f.bytes, &f, letterx, lettery);
    word[wordsize] = f.width;
    wordletter[wordsize] = 'f';
    letterx += f.width;
    wordsize++;
  }
  else if (lastletter == G && wordsize < 10) {
    draw_xpm(g.bytes, &g, letterx, lettery);
    word[wordsize] = g.width;
    wordletter[wordsize] = 'g';
    letterx += g.width;
    wordsize++;
  }
  else if (lastletter == H && wordsize < 10) {
    draw_xpm(h.bytes, &h, letterx, lettery);
    word[wordsize] = h.width;
    wordletter[wordsize] = 'h';
    letterx += h.width;
    wordsize++;
  }
  else if (lastletter == I && wordsize < 10) {
    draw_xpm(i.bytes, &i, letterx, lettery);
    word[wordsize] = i.width;
    wordletter[wordsize] = 'i';
    letterx += i.width;
    wordsize++;
  }
  else if (lastletter == J && wordsize < 10) {
    draw_xpm(j.bytes, &j, letterx, lettery);
    word[wordsize] = j.width;
    wordletter[wordsize] = 'j';
    letterx += j.width;
    wordsize++;
  }
  else if (lastletter == K && wordsize < 10) {
    draw_xpm(k.bytes, &k, letterx, lettery);
    word[wordsize] = k.width;
    wordletter[wordsize] = 'k';
    letterx += k.width;
    wordsize++;
  }
  else if (lastletter == L && wordsize < 10) {
    draw_xpm(l.bytes, &l, letterx, lettery);
    word[wordsize] = l.width;
    wordletter[wordsize] = 'l';
    letterx += l.width;
    wordsize++;
  }
  else if (lastletter == M && wordsize < 10) {
    draw_xpm(m.bytes, &m, letterx, lettery);
    word[wordsize] = m.width;
    wordletter[wordsize] = 'm';
    letterx += m.width;
    wordsize++;
  }
  else if (lastletter == N && wordsize < 10) {
    draw_xpm(n.bytes, &n, letterx, lettery);
    word[wordsize] = n.width;
    wordletter[wordsize] = 'n';
    letterx += n.width;
    wordsize++;
  }
  else if (lastletter == O && wordsize < 10) {
    draw_xpm(o.bytes, &o, letterx, lettery);
    word[wordsize] = o.width;
    wordletter[wordsize] = 'o';
    letterx += o.width;
    wordsize++;
  }
  else if (lastletter == P && wordsize < 10) {
    draw_xpm(p.bytes, &p, letterx, lettery);
    word[wordsize] = p.width;
    wordletter[wordsize] = 'p';
    letterx += p.width;
    wordsize++;
  }
  else if (lastletter == Q && wordsize < 10) {
    draw_xpm(q.bytes, &q, letterx, lettery);
    word[wordsize] = q.width;
    wordletter[wordsize] = 'q';
    letterx += q.width;
    wordsize++;
  }
  else if (lastletter == R && wordsize < 10) {
    draw_xpm(r.bytes, &r, letterx, lettery);
    word[wordsize] = r.width;
    wordletter[wordsize] = 'r';
    letterx += r.width;
    wordsize++;
  }
  else if (lastletter == S && wordsize < 10) {
    draw_xpm(s.bytes, &s, letterx, lettery);
    word[wordsize] = s.width;
    wordletter[wordsize] = 's';
    letterx += s.width;
    wordsize++;
  }
  else if (lastletter == T && wordsize < 10) {
    draw_xpm(t.bytes, &t, letterx, lettery);
    word[wordsize] = t.width;
    wordletter[wordsize] = 't';
    letterx += t.width;
    wordsize++;
  }
  else if (lastletter == U && wordsize < 10) {
    draw_xpm(u.bytes, &u, letterx, lettery);
    word[wordsize] = u.width;
    wordletter[wordsize] = 'u';
    letterx += u.width;
    wordsize++;
  }
  else if (lastletter == V && wordsize < 10) {
    draw_xpm(v.bytes, &v, letterx, lettery);
    word[wordsize] = v.width;
    wordletter[wordsize] = 'v';
    letterx += v.width;
    wordsize++;
  }
  else if (lastletter == W && wordsize < 10) {
    draw_xpm(w.bytes, &w, letterx, lettery);
    word[wordsize] = w.width;
    wordletter[wordsize] = 'w';
    letterx += w.width;
    wordsize++;
  }
  else if (lastletter == X && wordsize < 10) {
    draw_xpm(x.bytes, &x, letterx, lettery);
    word[wordsize] = x.width;
    wordletter[wordsize] = 'x';
    letterx += x.width;
    wordsize++;
  }
  else if (lastletter == Y && wordsize < 10) {
    draw_xpm(y.bytes, &y, letterx, lettery);
    word[wordsize] = y.width;
    wordletter[wordsize] = 'y';
    letterx += y.width;
    wordsize++;
  }
  else if (lastletter == Z && wordsize < 10) {
    draw_xpm(z.bytes, &z, letterx, lettery);
    word[wordsize] = z.width;
    wordletter[wordsize] = 'z';
    letterx += z.width;
    wordsize++;
  }
  else if (lastletter == BACKSPACE) {
    if (wordsize > 0) {
      wordsize--;
      wordletter[wordsize] = '\0';
      delete_xpm(m, letterx - word[wordsize], lettery);
      vg_draw_rectangle(letterx - word[wordsize], lettery, m.width, m.height, 0x000000);
      letterx -= word[wordsize];
    }
  }
}
