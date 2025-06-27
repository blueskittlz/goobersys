#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <Uefi.h>
#include <Library/UefiLib.h>

extern UINT32* gFramebuffer;
extern UINT32 gScreenWidth;
extern UINT32 gScreenHeight;
extern UINT32 gPixelsPerScanLine;

void PutPixel(int x, int y, UINT32 color);
void DrawRect(int x, int y, int width, int height, UINT32 color);

#endif // FRAMEBUFFER_H
