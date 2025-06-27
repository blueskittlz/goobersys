#pragma once
#include <Uefi.h>
#include <Library/UefiLib.h>

EFI_STATUS InitMouse(EFI_SYSTEM_TABLE* SystemTable);
void UpdateMouse(int* x, int* y, bool* leftBtn);
