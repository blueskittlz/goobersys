#pragma once
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Protocol/SimpleTextIn.h>

EFI_STATUS InitKeyboard(EFI_SYSTEM_TABLE* SystemTable);
bool PollKey(CHAR16* outChar);
