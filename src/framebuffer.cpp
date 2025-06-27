#include <Uefi.h>
#include <Library/UefiLib.h>

extern UINT32* gFramebuffer;
extern UINT32 gScreenWidth;
extern UINT32 gScreenHeight;
extern UINT32 gPixelsPerScanLine;

void PutPixel(int x, int y, UINT32 color) {
    if (x < 0 || y < 0 || x >= (int)gScreenWidth || y >= (int)gScreenHeight)
        return;
    gFramebuffer[y * gPixelsPerScanLine + x] = color;
}

void DrawRect(int x, int y, int width, int height, UINT32 color) {
    for (int py = y; py < y + height; py++) {
        for (int px = x; px < x + width; px++) {
            PutPixel(px, py, color);
        }
    }
}
