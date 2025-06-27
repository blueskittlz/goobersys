#pragma once
#ifndef UEFI_SHIMS_H
#define UEFI_SHIMS_H

#include <Uefi.h>

// Declare the Graphics Output Protocol GUID (define it in one .cpp)
extern EFI_GUID gEfiGraphicsOutputProtocolGuid;

// Global system table pointer
extern EFI_SYSTEM_TABLE* gST;

// Print macro for convenience
static inline EFI_STATUS TPrint(CHAR16* str) {
    return gST->ConOut->OutputString(gST->ConOut, str);
}

// Macro to create UEFI wide strings
#define UEFI_STR(str) L##str

#endif // UEFI_SHIMS_H
