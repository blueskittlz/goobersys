#include <Uefi.h>
#include <Library/UefiLib.h>
#include "framebuffer.h"  // Your low-level drawing functions like DrawRect, PutPixel

extern UINT32* gFramebuffer;
extern UINT32 gScreenWidth;
extern UINT32 gScreenHeight;

// Window state
static int winX = 100;
static int winY = 100;
static int winW = 400;
static int winH = 300;

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

// Check if mouse is inside title bar (for dragging)
static bool IsInTitleBar(int mx, int my) {
    return mx >= winX && mx <= winX + winW &&
           my >= winY && my <= winY + TITLEBAR_HEIGHT;
}

// Check if mouse is inside resize handle (bottom-right corner)
static bool IsInResizeHandle(int mx, int my) {
    return mx >= winX + winW - RESIZE_HANDLE_SIZE && mx <= winX + winW &&
           my >= winY + winH - RESIZE_HANDLE_SIZE && my <= winY + winH;
}

// Draw a simple mouse cursor (white cross)
static void DrawMouseCursor(int mx, int my) {
    const UINT32 white = 0xFFFFFF;
    for (int i = -5; i <= 5; i++) {
        if (mx + i >= 0 && mx + i < (int)gScreenWidth && my >= 0 && my < (int)gScreenHeight)
            PutPixel(mx + i, my, white);
        if (my + i >= 0 && my + i < (int)gScreenHeight && mx >= 0 && mx < (int)gScreenWidth)
            PutPixel(mx, my + i, white);
    }
}

// Draw the entire window UI
void DrawWindow() {
    // Window border - gray
    DrawRect(winX, winY, winW, winH, 0xC0C0C0);

    // Title bar - dark blue
    DrawRect(winX + BORDER_SIZE, winY + BORDER_SIZE, winW - 2 * BORDER_SIZE, TITLEBAR_HEIGHT - BORDER_SIZE, 0x000080);

    // Content area - white
    DrawRect(winX + BORDER_SIZE, winY + TITLEBAR_HEIGHT, winW - 2 * BORDER_SIZE, winH - TITLEBAR_HEIGHT - BORDER_SIZE, 0xFFFFFF);

    // Resize handle - black triangle in bottom-right corner
    for (int i = 0; i < RESIZE_HANDLE_SIZE; i++) {
        for (int j = 0; j <= i; j++) {
            int px = winX + winW - 1 - i;
            int py = winY + winH - 1 - j;
            if (px >= 0 && px < (int)gScreenWidth && py >= 0 && py < (int)gScreenHeight)
                PutPixel(px, py, 0x000000);
        }
    }

    // Optional: Draw title text here (requires font rendering or UEFI Print on screen buffer)
}

// Call when mouse button pressed at (mx, my)
void OnMouseDown(int mx, int my) {
    if (IsInResizeHandle(mx, my)) {
        resizing = true;
    } else if (IsInTitleBar(mx, my)) {
        dragging = true;
        dragOffsetX = mx - winX;
        dragOffsetY = my - winY;
    }
}

// Call when mouse button released
void OnMouseUp() {
    dragging = false;
    resizing = false;
}

// Call when mouse moves with button down at (mx, my)
void OnMouseMove(int mx, int my) {
    if (dragging) {
        winX = mx - dragOffsetX;
        winY = my - dragOffsetY;

        // Clamp inside screen bounds
        if (winX < 0) winX = 0;
        if (winY < 0) winY = 0;
        if (winX + winW > (int)gScreenWidth) winX = gScreenWidth - winW;
        if (winY + winH > (int)gScreenHeight) winY = gScreenHeight - winH;
    }
    else if (resizing) {
        winW = mx - winX;
        winH = my - winY;

        // Minimum size
        if (winW < MIN_WIDTH) winW = MIN_WIDTH;
        if (winH < MIN_HEIGHT) winH = MIN_HEIGHT;

        // Clamp inside screen bounds
        if (winX + winW > (int)gScreenWidth) winW = gScreenWidth - winX;
        if (winY + winH > (int)gScreenHeight) winH = gScreenHeight - winY;
    }
}

// Main GUI frame update - call each frame
void UpdateGui(int mouseX, int mouseY, bool leftBtn, bool &wasDown) {
    if (leftBtn && !wasDown) {
        OnMouseDown(mouseX, mouseY);
    } else if (!leftBtn && wasDown) {
        OnMouseUp();
    }
    if (leftBtn) {
        OnMouseMove(mouseX, mouseY);
    }
    wasDown = leftBtn;

    // Clear screen (black)
    DrawRect(0, 0, gScreenWidth, gScreenHeight, 0x000000);

    DrawWindow();

    DrawMouseCursor(mouseX, mouseY);
}
