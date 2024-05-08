/*++

Copyright (c) 2021-2024 DuoWoA authors. All Rights Reserved.

Module Name:

    SurfaceTouchPanelAlignment.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>
#include <ntdddisk.h>
#include <windef.h>
#include <wdfdriver.h>
#include <ntstrsafe.h>

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD SurfaceTouchPanelAlignmentEvtDeviceAdd;

#define POOL_TAG_FILEPATH  '0PFS'
#define POOL_TAG_DRIVEINFO '1PFS'

#define MAXIMUM_NUMBERS_OF_LUNS 6

#define MAX_FILE_OPEN_ATTEMPTS 10

/*
* Rob Green, a member of the NTDEV list, provides the
* following set of macros that'll keep you from having
* to scratch your head and count zeros ever again.
* Using these defintions, all you'll have to do is write:
*
*interval.QuadPart = RELATIVE(SECONDS(5));
*/
#ifndef ABSOLUTE
#define ABSOLUTE(wait) (wait)
#endif

#ifndef RELATIVE
#define RELATIVE(wait) (-(wait))
#endif

#ifndef NANOSECONDS
#define NANOSECONDS(nanos) \
	(((signed __int64)(nanos)) / 100L)
#endif

#ifndef MICROSECONDS
#define MICROSECONDS(micros) \
	(((signed __int64)(micros)) * NANOSECONDS(1000L))
#endif

#ifndef MILLISECONDS
#define MILLISECONDS(milli) \
	(((signed __int64)(milli)) * MICROSECONDS(1000L))
#endif

#ifndef SECONDS
#define SECONDS(seconds) \
	(((signed __int64)(seconds)) * MILLISECONDS(1000L))
#endif

#define SURFACE_FIRMWARE_PROVISIONING_DATA L"sfpd"

#define HEAT_VENDOR_SPECIFIC L"Heat\\VendorSpecific"
#define HEAT_LEFT_SHIFT L"PixelShiftForLeftPanelInMicrometer"
#define HEAT_RIGHT_SHIFT L"PixelShiftForRightPanelInMicrometer"

typedef enum _SFPD_DISPLAY_PIXEL_ALIGNMENT_DATA_PANEL_SIDE
{
	UNKNOWN_SIDE,
	LEFT_SIDE = 1,
	RIGHT_SIDE = 2,
	MAX_DISPLAY_SIDES = RIGHT_SIDE
} SFPD_DISPLAY_PIXEL_ALIGNMENT_DATA_PANEL_SIDE;
typedef struct _SFPD_DISPLAY_PIXEL_ALIGNMENT_DATA
{
	DWORD Reserved0; // 0
	DWORD Reserved1; // 31 37 17 C2
	SFPD_DISPLAY_PIXEL_ALIGNMENT_DATA_PANEL_SIDE Panel0Side;
	SFPD_DISPLAY_PIXEL_ALIGNMENT_DATA_PANEL_SIDE Panel1Side;
	DWORD Panel0Shift;
	DWORD Panel1Shift;
} SFPD_DISPLAY_PIXEL_ALIGNMENT_DATA, * PSFPD_DISPLAY_PIXEL_ALIGNMENT_DATA;

//
// Function to initialize the device and its callbacks
//
NTSTATUS
SurfaceTouchPanelAlignmentCreateDevice(
	_Inout_ PWDFDEVICE_INIT DeviceInit
);

NTSTATUS WriteHeatVendorParameters(WDFDEVICE device, PSFPD_DISPLAY_PIXEL_ALIGNMENT_DATA PixelAlignmentData);
NTSTATUS GetSFPDPixelAlignmentData(WDFDEVICE device, PSFPD_DISPLAY_PIXEL_ALIGNMENT_DATA PixelAlignmentData);
NTSTATUS GetSFPDVolumePath(WDFDEVICE device, WCHAR* VolumePath, DWORD VolumePathLength);
NTSTATUS GetSFPDItemSize(WDFDEVICE device, WCHAR* ItemPath, DWORD* ItemSize);
NTSTATUS GetSFPDItem(WDFDEVICE device, WCHAR* ItemPath, PVOID Data, DWORD DataSize);

EXTERN_C_END
