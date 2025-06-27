#include <Uefi.h>
#include <Library/UefiLib.h>

#include "uefi_shims.h"
#include "framebuffer.h"
#include "gui_window.h"

// for the future dev trying to debug this, the reason why this is so messy is because im tired and its thursday night and i just want to get this working so i can go to bed, so if you see something that looks like it shouldn't be there, it probably shouldn't be there but i was too lazy to fix it, so just ignore it and move on

// Mouse input
extern EFI_STATUS InitMouse(EFI_SYSTEM_TABLE* SystemTable);
extern void UpdateMouse(int* x, int* y, bool* leftBtn);

// Keyboard input
extern EFI_STATUS InitKeyboard(EFI_SYSTEM_TABLE* SystemTable);
extern bool PollKey(CHAR16* outChar);

// Terminal
extern void TerminalPutChar(CHAR16 c);
extern void TerminalDraw(int x, int y);

// Window drag/resize
extern void OnMouseDown(int mx, int my);
extern void OnMouseUp();
extern void OnMouseMove(int mx, int my);

UINT32* gFramebuffer = nullptr;
UINT32 gScreenWidth = 0;
UINT32 gScreenHeight = 0;
UINT32 gPixelsPerScanLine = 0;

EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphics = nullptr;
EFI_SYSTEM_TABLE* gST = nullptr;

EFI_STATUS InitGFX(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_STATUS status = SystemTable->BootServices->LocateProtocol(&gopGuid, nullptr, (void**)&gGraphics);
    if (EFI_ERROR(status)) return status;

    gFramebuffer = (UINT32*)gGraphics->Mode->FrameBufferBase;
    gScreenWidth = gGraphics->Mode->Info->HorizontalResolution;
    gScreenHeight = gGraphics->Mode->Info->VerticalResolution;
    gPixelsPerScanLine = gGraphics->Mode->Info->PixelsPerScanLine;

    return EFI_SUCCESS;
}

extern "C" EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    gST = SystemTable;

    EFI_STATUS status = InitGFX(ImageHandle, SystemTable);
    if (EFI_ERROR(status)) {
        TPrint(UEFI_STR("Graphics init failed\n"));
        return status;
    }

    status = InitMouse(SystemTable);
    if (EFI_ERROR(status)) {
        TPrint(UEFI_STR("Mouse init failed\n"));
        return status;
    }

    status = InitKeyboard(SystemTable);
    if (EFI_ERROR(status)) {
        TPrint(UEFI_STR("Keyboard init failed\n"));
        return status;
    }

    int mouseX = gScreenWidth / 2;
    int mouseY = gScreenHeight / 2;
    bool leftBtn = false;
    bool mouseWasDown = false;

    // Clear screen once before loop
    DrawRect(0, 0, gScreenWidth, gScreenHeight, 0x000000);

    while (true) {
        UpdateMouse(&mouseX, &mouseY, &leftBtn);

        if (leftBtn && !mouseWasDown) {
            OnMouseDown(mouseX, mouseY);
        } else if (!leftBtn && mouseWasDown) {
            OnMouseUp();
        }

        if (leftBtn) {
            OnMouseMove(mouseX, mouseY);
        }

        mouseWasDown = leftBtn;

        CHAR16 c;
        while (PollKey(&c)) {
            TerminalPutChar(c);
        }

        // Redraw window & terminal each frame
        DrawWindow();
        TerminalDraw(105, 130);
    }

    return EFI_SUCCESS;
}