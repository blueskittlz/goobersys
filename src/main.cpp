#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "input/input_mouse.h"
#include "input/input_keyboard.h"
#include "gui/gui_window.h"
#include "terminal/terminal.h"
#include "framebuffer/framebuffer.h"
#include "common/globals.h"

// Terminal window dimensions
const int termX = 10;
const int termY = 10;
const int termW = 400;
const int termH = 200;

// Track last window position to erase old frame
static int oldWinX = -1;
static int oldWinY = -1;
static int oldWinW = 0;
static int oldWinH = 0;

extern "C" EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
    gST = SystemTable;

    // Initialize graphics
    if (EFI_ERROR(InitGFX(ImageHandle, SystemTable))) {
        return EFI_LOAD_ERROR;
    }

    // Initialize mouse
    if (EFI_ERROR(InitMouse(SystemTable))) {
        return EFI_LOAD_ERROR;
    }

    // Initialize keyboard
    if (EFI_ERROR(InitKeyboard(SystemTable))) {
        return EFI_LOAD_ERROR;
    }

    // Mouse tracking
    int mouseX = gScreenWidth / 2;
    int mouseY = gScreenHeight / 2;
    bool leftBtn = false;
    bool mouseWasDown = false;

    // Clear full screen
    DrawRect(0, 0, gScreenWidth, gScreenHeight, 0x000000);

    // Main loop
    while (true) {
        // Mouse input
        UpdateMouse(&mouseX, &mouseY, &leftBtn);

        if (leftBtn && !mouseWasDown)
            OnMouseDown(mouseX, mouseY);
        else if (!leftBtn && mouseWasDown)
            OnMouseUp();

        if (leftBtn)
            OnMouseMove(mouseX, mouseY);

        mouseWasDown = leftBtn;

        // Keyboard input
        CHAR16 c;
        while (PollKey(&c)) {
            TerminalPutChar(c);
        }

        // Clear old window
        if (oldWinX >= 0 && oldWinY >= 0)
            DrawRect(oldWinX, oldWinY, oldWinW, oldWinH, 0x000000);

        // Clear terminal area
        DrawRect(termX, termY, termW, termH, 0x000000);

        // Draw content
        TerminalDraw(termX + 5, termY + 10);
        DrawWindow();
        DrawMouseCursor(mouseX, mouseY);

        // Track current window for next frame
        oldWinX = winX;
        oldWinY = winY;
        oldWinW = winW;
        oldWinH = winH;
    }

    return EFI_SUCCESS;
}
