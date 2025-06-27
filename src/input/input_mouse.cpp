#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimplePointer.h>
#include "../framebuffer/framebuffer.h"
#include "../common/globals.h"
#include "input_mouse.h"

// Mouse protocol pointer
static EFI_SIMPLE_POINTER_PROTOCOL* gPointer = nullptr;

// Mouse state
static bool mouseInitialized = false;
static int lastX = 0;
static int lastY = 0;

// Cursor drawing settings
constexpr int CURSOR_SIZE = 11;
constexpr UINT32 CURSOR_COLOR = 0xFFFFFFFF; // White

// Initialize mouse input
EFI_STATUS InitMouse(EFI_SYSTEM_TABLE* SystemTable) {
    EFI_GUID pointerGuid = EFI_SIMPLE_POINTER_PROTOCOL_GUID;
    EFI_STATUS status = SystemTable->BootServices->LocateProtocol(&pointerGuid, nullptr, (void**)&gPointer);
    if (EFI_ERROR(status)) {
        return status;
    }
    mouseInitialized = true;
    return EFI_SUCCESS;
}

// Update mouse position and button state, called every frame
void UpdateMouse(int* outX, int* outY, bool* outLeftButton) {
    if (!mouseInitialized) return;

    EFI_SIMPLE_POINTER_STATE state;
    EFI_STATUS status = gPointer->GetState(gPointer, &state);
    if (EFI_ERROR(status)) return;

    lastX += state.RelativeMovementX;
    lastY += state.RelativeMovementY;

    // Clamp within screen bounds
    if (lastX < 0) lastX = 0;
    if (lastY < 0) lastY = 0;
    if (lastX >= (int)gScreenWidth) lastX = gScreenWidth - 1;
    if (lastY >= (int)gScreenHeight) lastY = gScreenHeight - 1;

    *outX = lastX;
    *outY = lastY;
    *outLeftButton = state.LeftButton;
}

// Draw a plus-shaped mouse cursor centered at (x, y)
void DrawMouseCursor(int x, int y) {
    int half = CURSOR_SIZE / 2;

    // Draw horizontal line
    for (int dx = -half; dx <= half; dx++) {
        int px = x + dx;
        int py = y;
        if (px >= 0 && px < (int)gScreenWidth && py >= 0 && py < (int)gScreenHeight) {
            PutPixel(px, py, CURSOR_COLOR);
        }
    }

    // Draw vertical line
    for (int dy = -half; dy <= half; dy++) {
        int px = x;
        int py = y + dy;
        if (px >= 0 && px < (int)gScreenWidth && py >= 0 && py < (int)gScreenHeight) {
            PutPixel(px, py, CURSOR_COLOR);
        }
    }
}
