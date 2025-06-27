#include <Uefi.h>
#include <Library/UefiLib.h>
#include "../framebuffer/framebuffer.h"
#include "../common/globals.h"
#include "gui_window.h"

// Window state (extern from gui_window.h)
int winX = 100;
int winY = 100;
int winW = 400;
int winH = 300;

static bool dragging = false;
static bool resizing = false;
static int dragOffsetX = 0;
static int dragOffsetY = 0;

// Constants
constexpr int TITLEBAR_HEIGHT = 25;
constexpr int BORDER_SIZE = 2;
constexpr int RESIZE_HANDLE_SIZE = 15;
constexpr int MIN_WIDTH = 100;
constexpr int MIN_HEIGHT = 100;

static bool IsInTitleBar(int mx, int my) {
    return mx >= winX && mx <= winX + winW &&
           my >= winY && my <= winY + TITLEBAR_HEIGHT;
}

static bool IsInResizeHandle(int mx, int my) {
    return mx >= winX + winW - RESIZE_HANDLE_SIZE && mx <= winX + winW &&
           my >= winY + winH - RESIZE_HANDLE_SIZE && my <= winY + winH;
}

// Draw a simple mouse cursor (moved to input_mouse.cpp, so omitted here)

// Draw the window UI
void DrawWindow() {
    // Draw window border - gray
    DrawRect(winX, winY, winW, winH, 0xC0C0C0);

    // Title bar - dark blue
    DrawRect(winX + BORDER_SIZE, winY + BORDER_SIZE, winW - 2 * BORDER_SIZE, TITLEBAR_HEIGHT - BORDER_SIZE, 0x000080);

    // Content area - white
    DrawRect(winX + BORDER_SIZE, winY + TITLEBAR_HEIGHT, winW - 2 * BORDER_SIZE, winH - TITLEBAR_HEIGHT - BORDER_SIZE, 0xFFFFFF);

    // Draw a simple resize handle as a small black square (fix triangle issue)
    DrawRect(winX + winW - RESIZE_HANDLE_SIZE, winY + winH - RESIZE_HANDLE_SIZE, RESIZE_HANDLE_SIZE, RESIZE_HANDLE_SIZE, 0x000000);
}

void OnMouseDown(int mx, int my) {
    if (IsInResizeHandle(mx, my)) {
        resizing = true;
    } else if (IsInTitleBar(mx, my)) {
        dragging = true;
        dragOffsetX = mx - winX;
        dragOffsetY = my - winY;
    }
}

void OnMouseUp() {
    dragging = false;
    resizing = false;
}

void OnMouseMove(int mx, int my) {
    if (dragging) {
        winX = mx - dragOffsetX;
        winY = my - dragOffsetY;

        if (winX < 0) winX = 0;
        if (winY < 0) winY = 0;
        if (winX + winW > (int)gScreenWidth) winX = gScreenWidth - winW;
        if (winY + winH > (int)gScreenHeight) winY = gScreenHeight - winH;
    } else if (resizing) {
        winW = mx - winX;
        winH = my - winY;

        if (winW < MIN_WIDTH) winW = MIN_WIDTH;
        if (winH < MIN_HEIGHT) winH = MIN_HEIGHT;

        if (winX + winW > (int)gScreenWidth) winW = gScreenWidth - winX;
        if (winY + winH > (int)gScreenHeight) winH = gScreenHeight - winY;
    }
}
