#include "o2xfs-xfs-spi-std.h"
#include "at_o2xfs_xfs_spi_std_core_Win32MessageQueue.h"
#include "at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem.h"
#include "o2xfs-common.h"
#include "o2xfs-logger.h"

o2xfs::Logger LOG;

HMODULE hInstance;
WFMAllocateBuffer_t lpWFMAllocateBuffer;
WFMAllocateMore_t lpWFMAllocateMore;
WFMFreeBuffer_t lpWFMFreeBuffer;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		LOG.setFileName(_T("C:\\Temp\\o2xfs-xfs-spi-std.log"));
		break;
	case DLL_PROCESS_DETACH:
		LOG.debug(_T("DLL_PROCESS_DETACH"));
		if (hInstance != NULL) {
			FreeLibrary(hInstance);
		}
		break;
	}
	return TRUE;
}

BOOL isInitialized() {
	if (hInstance == NULL) {
		hInstance = LoadLibrary(TEXT("xfs_supp.dll"));
		if (hInstance == NULL) {
			LOG.error(_T("Error loading xfs_supp.dll: %d"), GetLastError());
			return FALSE;
		}
	}
	if (lpWFMAllocateBuffer == NULL) {
		lpWFMAllocateBuffer = (WFMAllocateBuffer_t) GetProcAddress(hInstance, "WFMAllocateBuffer");
		if (lpWFMAllocateBuffer == NULL) {
			LOG.error(_T("Can't find WFMAllocateBuffer: %d"), GetLastError());
			return FALSE;
		}
	}
	if (lpWFMAllocateMore == NULL) {
		lpWFMAllocateMore = (WFMAllocateMore_t) GetProcAddress(hInstance, "WFMAllocateMore");
		if (lpWFMAllocateMore == NULL) {
			LOG.error(_T("Can't find WFMAllocateMore: %d"), GetLastError());
			return FALSE;
		}
	}
	if (lpWFMFreeBuffer == NULL) {
		lpWFMFreeBuffer = (WFMFreeBuffer_t) GetProcAddress(hInstance, "WFMFreeBuffer");
		if (lpWFMFreeBuffer == NULL) {
			LOG.error(_T("Can't find WFMFreeBuffer: %d"), GetLastError());
			return FALSE;
		}
	}
	return TRUE;
}

JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_wfmAllocateBuffer0(JNIEnv *env, jobject obj, jbyteArray array, jbyteArray lpDataObj) {
	HRESULT hResult = WFS_ERR_INTERNAL_ERROR;
	if (isInitialized()) {
		LPVOID lpData = NULL;
		hResult = lpWFMAllocateBuffer(env->GetArrayLength(array), WFS_MEM_SHARE | WFS_MEM_ZEROINIT, &lpData);
		LOG.debug(_T("WFMAllocateBuffer: result=%d,lpData=%p"), hResult, lpData);
		if (hResult == WFS_SUCCESS) {
			env->GetByteArrayRegion(array, 0, env->GetArrayLength(array), (jbyte*) lpData);
			env->SetByteArrayRegion(lpDataObj, 0, env->GetArrayLength(lpDataObj), (jbyte*) &lpData);
		}
	}
	return hResult;
}

JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_wfmAllocateMore0(JNIEnv *env, jobject obj, jbyteArray array, jbyteArray lpvOriginalObj, jbyteArray lpDataObj) {
	HRESULT hResult = WFS_ERR_INTERNAL_ERROR;
	if (isInitialized()) {
		LPVOID lpvOriginal = o2xfs::ToPointer(env, lpvOriginalObj);
		LPVOID lpData = NULL;
		LOG.debug(_T("WFMAllocateMore: lpvOriginal=%p"), lpvOriginal);
		hResult = lpWFMAllocateMore(env->GetArrayLength(array), lpvOriginal, &lpData);
		LOG.debug(_T("WFMAllocateMore: result=%d,lpData=%p"), hResult, lpData);
		if (hResult == WFS_SUCCESS) {
			env->GetByteArrayRegion(array, 0, env->GetArrayLength(array), (jbyte*) lpData);
			env->SetByteArrayRegion(lpDataObj, 0, env->GetArrayLength(lpDataObj), (jbyte*) &lpData);
		}
	}
	return hResult;
}

JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_wfmFreeBuffer(JNIEnv* env, jobject obj, jbyteArray lpvDataObj) {
	HRESULT hResult = WFS_ERR_INTERNAL_ERROR;
	if (isInitialized()) {
		LPVOID lpvData = o2xfs::ToPointer(env, lpvDataObj);
		LOG.debug(_T("WFMFreeBuffer: lpvData=%p"), lpvData);
		hResult = lpWFMFreeBuffer(lpvData);
	}
	return hResult;
}

JNIEXPORT void JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_write0(JNIEnv *env, jobject obj, jbyteArray lpObj, jint offset, jbyteArray array) {
	LPVOID buf = (LPVOID) ((char*) o2xfs::ToPointer(env, lpObj) + offset);
	LOG.debug(TEXT("Write: buf=%p"), buf);
	env->GetByteArrayRegion(array, 0, env->GetArrayLength(array), (jbyte*) buf);
}

JNIEXPORT jbyteArray JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_read0(JNIEnv* env, jobject obj, jbyteArray array, jint offset, jint len) {
	LPVOID p = o2xfs::ToPointer(env, array);
	LOG.debug(_T("READ: lpvData=%p,len=%d"), p, len);
	jbyteArray result = env->NewByteArray(len);
	env->SetByteArrayRegion(result, 0, len, (jbyte*) p + offset);
	return result;
}

JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_sizeof(JNIEnv* env, jclass cls) {
	return sizeof(LPVOID);
}

JNIEXPORT void JNICALL Java_at_o2xfs_xfs_spi_std_core_Win32MessageQueue_postMessage0(JNIEnv *env, jobject obj, jbyteArray hWndObj, jlong msg, jbyteArray lParamObj) {
	HWND hWnd = (HWND) o2xfs::ToPointer(env, hWndObj);
	LPARAM lParam = (LPARAM) o2xfs::ToPointer(env, lParamObj);
	LOG.debug(_T("PostMessage: hWnd=%p,lParam=%p"), hWnd, lParam);
	BOOL succees = PostMessage(hWnd, msg, 0, lParam);
	if (!succees) {
		LOG.error(_T("PostMessage: hWnd=%p,lParam=%p,lastError=%d"), hWnd, lParam, GetLastError());
	}
}

