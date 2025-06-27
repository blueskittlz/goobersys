#pragma once
#include <Uefi.h>

extern EFI_SYSTEM_TABLE* gST;

#define TPrint(msg) gST->ConOut->OutputString(gST->ConOut, (CHAR16*)(msg))
#define UEFI_STR(str) ((CHAR16*)u##str)