#include <Uefi.h>
#include <Library/UefiLib.h>
#include "../common/globals.h"
#include "../common/uefi_shims.h"
#include <Protocol/GraphicsOutput.h>

extern UINT32* gFramebuffer;
extern UINT32 gScreenWidth;
extern UINT32 gScreenHeight;
extern UINT32 gPixelsPerScanLine;

EFI_STATUS InitGFX(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_STATUS status;

    // Locate the Graphics Output Protocol
    status = SystemTable->BootServices->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (void**)&gGraphics);
    if (EFI_ERROR(status)) {
        return status;
    }

    // Get current mode info
    gScreenWidth = gGraphics->Mode->Info->HorizontalResolution;
    gScreenHeight = gGraphics->Mode->Info->VerticalResolution;
    gPixelsPerScanLine = gGraphics->Mode->Info->PixelsPerScanLine;

    // Get framebuffer base address
    gFramebuffer = (UINT32*)gGraphics->Mode->FrameBufferBase;

    return EFI_SUCCESS;
}

void PutPixel(int x, int y, UINT32 color) {
    if (x < 0 || y < 0 || x >= (int)gScreenWidth || y >= (int)gScreenHeight)
        return;
    gFramebuffer[y * gPixelsPerScanLine + x] = color;
}

void DrawRect(int x, int y, int width, int height, UINT32 color) {
    if (x < 0) {
        width += x; // reduce width if x < 0
        x = 0;
    }
    if (y < 0) {
        height += y; // reduce height if y < 0
        y = 0;
    }
    if (x + width > (int)gScreenWidth)
        width = gScreenWidth - x;
    if (y + height > (int)gScreenHeight)
        height = gScreenHeight - y;
    if (width <= 0 || height <= 0)
        return;

    UINT32* row = gFramebuffer + y * gPixelsPerScanLine + x;
    for (int py = 0; py < height; py++) {
        UINT32* pixel = row;
        for (int px = 0; px < width; px++) {
            *pixel++ = color;
        }
        row += gPixelsPerScanLine;
    }
}
