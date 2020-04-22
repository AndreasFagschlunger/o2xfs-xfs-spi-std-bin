#ifndef O2XFS_XFS_SPI_STD_H
#define O2XFS_XFS_SPI_STD_H

#include <Windows.h>
#include "XFSADMIN.h"

typedef HRESULT(WINAPI *WFMAllocateBuffer_t)  (ULONG ulSize, ULONG ulFlags, LPVOID *lppvData);
typedef HRESULT(WINAPI *WFMAllocateMore_t)  (ULONG ulSize, LPVOID lpvOriginal, LPVOID *lppvData);
typedef HRESULT(WINAPI *WFMFreeBuffer_t)  (LPVOID lpvData);

#endif