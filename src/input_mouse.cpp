#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimplePointer.h>
#include "framebuffer.h"  // For PutPixel()

EFI_SIMPLE_POINTER_PROTOCOL* gPointer = nullptr;

bool mouseInitialized = false;
UINTN lastX = 0, lastY = 0;
bool leftButtonDown = false;

const int CURSOR_SIZE = 11;
const UINT32 CURSOR_COLOR = 0xFFFFFFFF; // White

void DrawPlusCursor(int x, int y) {
    int mid = CURSOR_SIZE / 2;
    for (int i = 0; i < CURSOR_SIZE; i++) {
        PutPixel(x + mid, y + i, CURSOR_COLOR);  // vertical line
        PutPixel(x + i, y + mid, CURSOR_COLOR);  // horizontal line
    }
}

EFI_STATUS InitMouse(EFI_SYSTEM_TABLE* SystemTable) {
    EFI_GUID pointerGuid = EFI_SIMPLE_POINTER_PROTOCOL_GUID;
    EFI_STATUS status = SystemTable->BootServices->LocateProtocol(&pointerGuid, nullptr, (void**)&gPointer);
    if (EFI_ERROR(status)) {
        return status;
    }
    mouseInitialized = true;
    return EFI_SUCCESS;
}

void UpdateMouse(int* outX, int* outY, bool* outLeftButton) {
    if (!mouseInitialized) return;

    EFI_SIMPLE_POINTER_STATE state;
    EFI_STATUS status = gPointer->GetState(gPointer, &state);
    if (EFI_ERROR(status)) return;

    lastX += state.RelativeMovementX;
    lastY += state.RelativeMovementY;

    if (lastX < 0) lastX = 0;
    if (lastY < 0) lastY = 0;
    // Replace 800 and 600 with your actual screen width/height variables
    if (lastX > 799) lastX = 799;
    if (lastY > 599) lastY = 599;

    *outX = (int)lastX;
    *outY = (int)lastY;
    *outLeftButton = state.LeftButton;

    // Draw cursor at updated position
    DrawPlusCursor(*outX, *outY);
}
