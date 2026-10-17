#include "fonts.h"
#include "Bitmap.h"

Fonts* loadFonts() {
	Fonts* f = (Fonts*) malloc(sizeof(Fonts));

	f->a = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/a.bmp");
	f->aa = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/aa.bmp");
	f->b = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/b.bmp");
	f->bb = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/bb.bmp");
	f->c = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/c.bmp");
	f->cc = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/cc.bmp");
	f->d = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/d.bmp");
	f->dd = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/dd.bmp");
	f->e = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/e.bmp");
	f->ee = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/ee.bmp");
	f->f = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/f.bmp");
	f->ff = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/ff.bmp");
	f->g = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/g.bmp");
	f->gg = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/gg.bmp");
	f->h = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/h.bmp");
	f->hh = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/hh.bmp");
	f->i = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/i.bmp");
	f->ii = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/ii.bmp");
	f->j = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/j.bmp");
	f->jj = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/jj.bmp");
	f->k = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/k.bmp");
	f->kk = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/kk.bmp");
	f->l = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/l.bmp");
	f->ll = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/ll.bmp");
	f->m = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/m.bmp");
	f->mm = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/mm.bmp");
	f->n = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/n.bmp");
	f->nn = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/nn.bmp");
	f->o = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/o.bmp");
	f->oo = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/oo.bmp");
	f->p = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/p.bmp");
	f->pp = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/pp.bmp");
	f->q = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/q.bmp");
	f->qq = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/qq.bmp");
	f->r = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/r.bmp");
	f->rr = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/rr.bmp");
	f->s = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/s.bmp");
	f->ss = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/ss.bmp");
	f->t = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/t.bmp");
	f->tt = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/tt.bmp");
	f->u = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/u.bmp");
	f->uu = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/uu.bmp");
	f->v = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/v.bmp");
	f->vv = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/vv.bmp");
	f->x = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/x.bmp");
	f->xx = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/xx.bmp");
	f->y = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/y.bmp");
	f->yy = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/yy.bmp");
	f->w = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/w.bmp");
	f->ww = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/ww.bmp");
	f->z = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/z.bmp");
	f->zz = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/zz.bmp");
	//symbols
	f->barra = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/barra.bmp");
	f->doispontos = loadBitmap(
			"/home/lcom/lcom1516-t2g12/proj/res/fonts/doispontos.bmp");
	f->espaco = loadBitmap(
			"/home/lcom/lcom1516-t2g12/proj/res/fonts/espaco.bmp");
	f->traco = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/traco.bmp");
	//numbers
	f->zero = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/0.bmp");
	f->um = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/1.bmp");
	f->dois = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/2.bmp");
	f->tres = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/3.bmp");
	f->quatro = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/4.bmp");
	f->cinco = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/5.bmp");
	f->seis = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/6.bmp");
	f->sete = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/7.bmp");
	f->oito = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/8.bmp");
	f->nove = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/fonts/9.bmp");

	return f;
}

char receiveCharMinusculo(unsigned long scancode) {
	switch (scancode) {
	case codeA:
		return 'a';
		break;
	case codeB:
		return 'b';
		break;
	case codeC:
		return 'c';
		break;
	case codeD:
		return 'd';
		break;
	case codeE:
		return 'e';
		break;
	case codeF:
		return 'f';
		break;
	case codeG:
		return 'g';
		break;
	case codeH:
		return 'h';
		break;
	case codeI:
		return 'i';
		break;
	case codeJ:
		return 'j';
		break;
	case codeK:
		return 'k';
		break;
	case codeL:
		return 'l';
		break;
	case codeM:
		return 'm';
		break;
	case codeN:
		return 'n';
		break;
	case codeO:
		return 'o';
		break;
	case codeP:
		return 'p';
		break;
	case codeQ:
		return 'q';
		break;
	case codeR:
		return 'r';
		break;
	case codeS:
		return 's';
		break;
	case codeT:
		return 't';
		break;
	case codeU:
		return 'u';
		break;
	case codeV:
		return 'v';
		break;
	case codeX:
		return 'x';
		break;
	case codeY:
		return 'y';
		break;
	case codeW:
		return 'w';
		break;
	case codeZ:
		return 'z';
		break;
	case code0:
		return '0';
		break;
	case code1:
		return '1';
		break;
	case code2:
		return '2';
		break;
	case code3:
		return '3';
		break;
	case code4:
		return '4';
		break;
	case code5:
		return '5';
		break;
	case code6:
		return '6';
		break;
	case code7:
		return '7';
	case code8:
		return '8';
		break;
	case code9:
		return '9';
		break;
	case codeEspaco:
		return ' ';
		break;
	default:
		break;
	}
}
char receiveCharMaiusculo(unsigned long scancode) {
	switch (scancode) {
	case codeA:
		return 'A';
		break;
	case codeB:
		return 'B';
		break;
	case codeC:
		return 'C';
		break;
	case codeD:
		return 'D';
		break;
	case codeE:
		return 'E';
		break;
	case codeF:
		return 'F';
		break;
	case codeG:
		return 'G';
		break;
	case codeH:
		return 'H';
		break;
	case codeI:
		return 'I';
		break;
	case codeJ:
		return 'J';
		break;
	case codeK:
		return 'K';
		break;
	case codeL:
		return 'L';
		break;
	case codeM:
		return 'M';
		break;
	case codeN:
		return 'N';
		break;
	case codeO:
		return 'O';
		break;
	case codeP:
		return 'P';
		break;
	case codeQ:
		return 'Q';
		break;
	case codeR:
		return 'R';
		break;
	case codeS:
		return 'S';
		break;
	case codeT:
		return 'T';
		break;
	case codeU:
		return 'U';
		break;
	case codeV:
		return 'V';
		break;
	case codeX:
		return 'X';
		break;
	case codeY:
		return 'Y';
		break;
	case codeW:
		return 'W';
		break;
	case codeZ:
		return 'Z';
		break;
	default:
		break;
	}
}

void stringDisplay(Fonts* f,  char string[], int x, int y, int length) {

	//char str[15];
	//sprintf(str, "%s", string);
	int i;
	for (i = 0; i < length; i++) {
		char letra = string[i];
		switch (letra) {
		case '_':
			drawBitmap(f->espaco, x, y, ALIGN_LEFT);
			break;
		case 'a':
			drawBitmap(f->a, x, y, ALIGN_LEFT);
			break;
		case 'A':
			drawBitmap(f->aa, x, y, ALIGN_LEFT);
			break;
		case 'b':
			drawBitmap(f->b, x, y, ALIGN_LEFT);
			break;
		case 'B':
			drawBitmap(f->bb, x, y, ALIGN_LEFT);
			break;
		case 'c':
			drawBitmap(f->c, x, y, ALIGN_LEFT);
			break;
		case 'C':
			drawBitmap(f->cc, x, y, ALIGN_LEFT);
			break;
		case 'd':
			drawBitmap(f->d, x, y, ALIGN_LEFT);
			break;
		case 'D':
			drawBitmap(f->dd, x, y, ALIGN_LEFT);
			break;
		case 'e':
			drawBitmap(f->e, x, y, ALIGN_LEFT);
			break;
		case 'E':
			drawBitmap(f->ee, x, y, ALIGN_LEFT);
			break;
		case 'f':
			drawBitmap(f->f, x, y, ALIGN_LEFT);
			break;
		case 'F':
			drawBitmap(f->ff, x, y, ALIGN_LEFT);
			break;
		case 'g':
			drawBitmap(f->g, x, y, ALIGN_LEFT);
			break;
		case 'G':
			drawBitmap(f->gg, x, y, ALIGN_LEFT);
			break;
		case 'h':
			drawBitmap(f->h, x, y, ALIGN_LEFT);
			break;
		case 'H':
			drawBitmap(f->hh, x, y, ALIGN_LEFT);
			break;
		case 'i':
			drawBitmap(f->i, x, y, ALIGN_LEFT);
			break;
		case 'I':
			drawBitmap(f->ii, x, y, ALIGN_LEFT);
			break;
		case 'j':
			drawBitmap(f->j, x, y, ALIGN_LEFT);
			break;
		case 'J':
			drawBitmap(f->jj, x, y, ALIGN_LEFT);
			break;
		case 'k':
			drawBitmap(f->k, x, y, ALIGN_LEFT);
			break;
		case 'K':
			drawBitmap(f->kk, x, y, ALIGN_LEFT);
			break;
		case 'l':
			drawBitmap(f->l, x, y, ALIGN_LEFT);
			break;
		case 'L':
			drawBitmap(f->ll, x, y, ALIGN_LEFT);
			break;
		case 'm':
			drawBitmap(f->m, x, y, ALIGN_LEFT);
			break;
		case 'M':
			drawBitmap(f->mm, x, y, ALIGN_LEFT);
			break;
		case 'n':
			drawBitmap(f->n, x, y, ALIGN_LEFT);
			break;
		case 'N':
			drawBitmap(f->nn, x, y, ALIGN_LEFT);
			break;
		case 'o':
			drawBitmap(f->o, x, y, ALIGN_LEFT);
			break;
		case 'O':
			drawBitmap(f->oo, x, y, ALIGN_LEFT);
			break;
		case 'p':
			drawBitmap(f->p, x, y, ALIGN_LEFT);
			break;
		case 'P':
			drawBitmap(f->pp, x, y, ALIGN_LEFT);
			break;
		case 'q':
			drawBitmap(f->q, x, y, ALIGN_LEFT);
			break;
		case 'Q':
			drawBitmap(f->qq, x, y, ALIGN_LEFT);
			break;
		case 'r':
			drawBitmap(f->r, x, y, ALIGN_LEFT);
			break;
		case 'R':
			drawBitmap(f->rr, x, y, ALIGN_LEFT);
			break;
		case 's':
			drawBitmap(f->s, x, y, ALIGN_LEFT);
			break;
		case 'S':
			drawBitmap(f->ss, x, y, ALIGN_LEFT);
			break;
		case 't':
			drawBitmap(f->t, x, y, ALIGN_LEFT);
			break;
		case 'T':
			drawBitmap(f->tt, x, y, ALIGN_LEFT);
			break;
		case 'u':
			drawBitmap(f->u, x, y, ALIGN_LEFT);
			break;
		case 'U':
			drawBitmap(f->uu, x, y, ALIGN_LEFT);
			break;
		case 'v':
			drawBitmap(f->v, x, y, ALIGN_LEFT);
			break;
		case 'V':
			drawBitmap(f->vv, x, y, ALIGN_LEFT);
			break;
		case 'x':
			drawBitmap(f->x, x, y, ALIGN_LEFT);
			break;
		case 'X':
			drawBitmap(f->xx, x, y, ALIGN_LEFT);
			break;
		case 'y':
			drawBitmap(f->y, x, y, ALIGN_LEFT);
			break;
		case 'Y':
			drawBitmap(f->yy, x, y, ALIGN_LEFT);
			break;
		case 'w':
			drawBitmap(f->w, x, y, ALIGN_LEFT);
			break;
		case 'W':
			drawBitmap(f->ww, x, y, ALIGN_LEFT);
			break;
		case 'z':
			drawBitmap(f->z, x, y, ALIGN_LEFT);
			break;
		case 'Z':
			drawBitmap(f->zz, x, y, ALIGN_LEFT);
			break;
		case '-':
			drawBitmap(f->traco, x, y, ALIGN_LEFT);
			break;
		case '/':
			drawBitmap(f->barra, x, y, ALIGN_LEFT);
			break;
		case ':':
			drawBitmap(f->doispontos, x, y, ALIGN_LEFT);
			break;
		case ' ':
			drawBitmap(f->espaco, x, y, ALIGN_LEFT);
			break;
		case '0':
			drawBitmap(f->zero, x, y, ALIGN_LEFT);
			break;
		case '1':
			drawBitmap(f->um, x, y, ALIGN_LEFT);
			break;
		case '2':
			drawBitmap(f->dois, x, y, ALIGN_LEFT);
			break;
		case '3':
			drawBitmap(f->tres, x, y, ALIGN_LEFT);
			break;
		case '4':
			drawBitmap(f->quatro, x, y, ALIGN_LEFT);
			break;
		case '5':
			drawBitmap(f->cinco, x, y, ALIGN_LEFT);
			break;
		case '6':
			drawBitmap(f->seis, x, y, ALIGN_LEFT);
			break;
		case '7':
			drawBitmap(f->sete, x, y, ALIGN_LEFT);
			break;
		case '8':
			drawBitmap(f->oito, x, y, ALIGN_LEFT);
			break;
		case '9':
			drawBitmap(f->nove, x, y, ALIGN_LEFT);
			break;

		default:
			break;
		}
		x += 10;
	}

}

void deleteFonts(Fonts* font) {
	deleteBitmap(font->barra);
	deleteBitmap(font->traco);
	deleteBitmap(font->doispontos);
	deleteBitmap(font->espaco);

	deleteBitmap(font->zero);
	deleteBitmap(font->um);
	deleteBitmap(font->dois);
	deleteBitmap(font->tres);
	deleteBitmap(font->quatro);
	deleteBitmap(font->cinco);
	deleteBitmap(font->seis);
	deleteBitmap(font->sete);
	deleteBitmap(font->oito);
	deleteBitmap(font->nove);

	deleteBitmap(font->a);
	deleteBitmap(font->b);
	deleteBitmap(font->c);
	deleteBitmap(font->d);
	deleteBitmap(font->e);
	deleteBitmap(font->f);
	deleteBitmap(font->g);
	deleteBitmap(font->h);
	deleteBitmap(font->i);
	deleteBitmap(font->j);
	deleteBitmap(font->k);
	deleteBitmap(font->l);
	deleteBitmap(font->m);
	deleteBitmap(font->n);
	deleteBitmap(font->o);
	deleteBitmap(font->p);
	deleteBitmap(font->q);
	deleteBitmap(font->r);
	deleteBitmap(font->s);
	deleteBitmap(font->t);
	deleteBitmap(font->u);
	deleteBitmap(font->v);
	deleteBitmap(font->w);
	deleteBitmap(font->x);
	deleteBitmap(font->y);
	deleteBitmap(font->z);

	deleteBitmap(font->aa);
	deleteBitmap(font->bb);
	deleteBitmap(font->cc);
	deleteBitmap(font->dd);
	deleteBitmap(font->ee);
	deleteBitmap(font->ff);
	deleteBitmap(font->gg);
	deleteBitmap(font->hh);
	deleteBitmap(font->ii);
	deleteBitmap(font->jj);
	deleteBitmap(font->kk);
	deleteBitmap(font->ll);
	deleteBitmap(font->mm);
	deleteBitmap(font->nn);
	deleteBitmap(font->oo);
	deleteBitmap(font->pp);
	deleteBitmap(font->qq);
	deleteBitmap(font->rr);
	deleteBitmap(font->ss);
	deleteBitmap(font->tt);
	deleteBitmap(font->uu);
	deleteBitmap(font->vv);
	deleteBitmap(font->ww);
	deleteBitmap(font->xx);
	deleteBitmap(font->yy);
	deleteBitmap(font->zz);

	free(font);
}

