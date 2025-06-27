#ifndef INPUT_MOUSE_H
#define INPUT_MOUSE_H
#include "../framebuffer/framebuffer.h"
#include "../common/globals.h"
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimplePointer.h>

EFI_STATUS InitMouse(EFI_SYSTEM_TABLE* SystemTable);
void UpdateMouse(int* outX, int* outY, bool* outLeftButton);
void DrawMouseCursor(int x, int y);

#endif // INPUT_MOUSE_H
