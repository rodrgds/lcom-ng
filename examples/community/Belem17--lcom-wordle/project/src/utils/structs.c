#include "../drivers/graphics.h"
#include "../public/sprites/cursor.xpm"
#include "../public/sprites/Title_stars.xpm"
#include "../public/sprites/startButton.xpm"
#include "../public/sprites/letras/A.xpm"
#include "../public/sprites/letras/B.xpm"
#include "../public/sprites/letras/c.xpm"
#include "../public/sprites/letras/D.xpm"
#include "../public/sprites/letras/E.xpm"
#include "../public/sprites/letras/F.xpm"
#include "../public/sprites/letras/G.xpm"
#include "../public/sprites/letras/H.xpm"
#include "../public/sprites/letras/I.xpm"
#include "../public/sprites/letras/J.xpm"
#include "../public/sprites/letras/K.xpm"
#include "../public/sprites/letras/L.xpm"
#include "../public/sprites/letras/M.xpm"
#include "../public/sprites/letras/N.xpm"
#include "../public/sprites/letras/O.xpm"
#include "../public/sprites/letras/P.xpm"
#include "../public/sprites/letras/Q.xpm"
#include "../public/sprites/letras/R.xpm"
#include "../public/sprites/letras/S.xpm"
#include "../public/sprites/letras/T.xpm"
#include "../public/sprites/letras/U.xpm"
#include "../public/sprites/letras/V.xpm"
#include "../public/sprites/letras/W.xpm"
#include "../public/sprites/letras/X.xpm"
#include "../public/sprites/letras/Y.xpm"
#include "../public/sprites/letras/Z.xpm"
#include "../public/sprites/winscreen.xpm"
#include "../public/sprites/losescreen.xpm"
#include "../public/sprites/grid.xpm"

#include "structs.h"
#include "frames.h"
#include <string.h>

/// @brief Global resources object to hold all game assets (sprites, UI elements, etc.)
Resources resources;

/**
 * @brief Initializes all graphical resources for the game.
 * 
 * This function loads all required XPM images into memory by creating `Frame` objects.
 * These include:
 * - UI elements (title, cursor, start button)
 * - Alphabet letters (A-Z)
 * - Game screens (win and lose)
 * - The grid background
 */
void initialize()
{
  resources.Title = createFrame(Title_xpm);
  resources.Mouse = createFrame(cursor_xpm);
  resources.startButton = createFrame(startButton_xpm);
  resources.Alphabet[0] = createFrame(A_xpm);
  resources.Alphabet[1] = createFrame(B_xpm);
  resources.Alphabet[2] = createFrame(c_xpm);
  resources.Alphabet[3] = createFrame(D_xpm);
  resources.Alphabet[4] = createFrame(E_xpm);
  resources.Alphabet[5] = createFrame(F_xpm);
  resources.Alphabet[6] = createFrame(G_xpm);
  resources.Alphabet[7] = createFrame(H_xpm);
  resources.Alphabet[8] = createFrame(I_xpm);
  resources.Alphabet[9] = createFrame(J_xpm);
  resources.Alphabet[10] = createFrame(K_xpm);
  resources.Alphabet[11] = createFrame(L_xpm);
  resources.Alphabet[12] = createFrame(M_xpm);
  resources.Alphabet[13] = createFrame(N_xpm);
  resources.Alphabet[14] = createFrame(O_xpm);
  resources.Alphabet[15] = createFrame(P_xpm);
  resources.Alphabet[16] = createFrame(Q_xpm);
  resources.Alphabet[17] = createFrame(R_xpm);
  resources.Alphabet[18] = createFrame(S_xpm);
  resources.Alphabet[19] = createFrame(T_xpm);
  resources.Alphabet[20] = createFrame(U_xpm);
  resources.Alphabet[21] = createFrame(V_xpm);
  resources.Alphabet[22] = createFrame(W_xpm);
  resources.Alphabet[23] = createFrame(X_xpm);
  resources.Alphabet[24] = createFrame(Y_xpm);
  resources.Alphabet[25] = createFrame(Z_xpm);
  resources.winscreen = createFrame(winscreen_xpm);
  resources.losescreen = createFrame(losescreen_xpm);
  resources.grid = createFrame(grid_xpm);
}

