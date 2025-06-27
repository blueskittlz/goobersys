#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleTextIn.h>
#include "../common/globals.h"

static EFI_SIMPLE_TEXT_INPUT_PROTOCOL* gKeyboard = nullptr;
static bool keyboardInitialized = false;

// Initialize keyboard input protocol
EFI_STATUS InitKeyboard(EFI_SYSTEM_TABLE* SystemTable) {
    gKeyboard = SystemTable->ConIn;
    if (!gKeyboard) return EFI_DEVICE_ERROR;
    keyboardInitialized = true;
    return EFI_SUCCESS;
}

// Read a single key if available; returns false if no key
bool PollKey(CHAR16* outChar) {
    if (!keyboardInitialized) return false;

    EFI_INPUT_KEY key;
    EFI_STATUS status = gKeyboard->ReadKeyStroke(gKeyboard, &key);
    if (status == EFI_SUCCESS) {
        *outChar = key.UnicodeChar;
        return true;
    }
    return false;
}
