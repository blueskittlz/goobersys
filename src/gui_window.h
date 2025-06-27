#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <Uefi.h>
#include <Library/UefiLib.h>

// draw the window every frame
void DrawWindow();

// mouse event handlers
void OnMouseDown(int mx, int my);
void OnMouseUp();
void OnMouseMove(int mx, int my);

// optional b.s. expose window pos for term drawing
extern int winX;
extern int winY;
extern int winW;
extern int winH;

#endif // GUI_WINDOW_H