#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "../common/globals.h"

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

extern UINT32* gFramebuffer;
extern UINT32 gScreenWidth;
extern UINT32 gScreenHeight;
extern UINT32 gPixelsPerScanLine;

EFI_STATUS InitGFX(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
void PutPixel(int x, int y, UINT32 color);
void DrawRect(int x, int y, int width, int height, UINT32 color);

#endif // FRAMEBUFFER_H
