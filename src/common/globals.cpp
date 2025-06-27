#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>
#include "globals.h"


// Define the GUID symbol here
EFI_GUID gEfiGraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
// Global framebuffer info shared between source files
UINT32* gFramebuffer = nullptr;
UINT32 gScreenWidth = 0;
UINT32 gScreenHeight = 0;
UINT32 gPixelsPerScanLine = 0;

EFI_GRAPHICS_OUTPUT_PROTOCOL* gGraphics = nullptr;
EFI_SYSTEM_TABLE* gST = nullptr;
