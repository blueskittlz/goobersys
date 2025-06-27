#ifndef GLOBALS_H
#define GLOBALS_H

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

extern UINT32* gFramebuffer;
extern UINT32 gScreenWidth;
extern UINT32 gScreenHeight;
extern UINT32 gPixelsPerScanLine;

extern EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphics;
extern EFI_SYSTEM_TABLE* gST;

#endif // GLOBALS_H
