/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem */

#ifndef _Included_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
#define _Included_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
 * Method:    wfmAllocateBuffer0
 * Signature: ([B[B)I
 */
JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_wfmAllocateBuffer0
  (JNIEnv *, jobject, jbyteArray, jbyteArray);

/*
 * Class:     at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
 * Method:    wfmAllocateMore0
 * Signature: ([B[B[B)I
 */
JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_wfmAllocateMore0
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jbyteArray);

/*
 * Class:     at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
 * Method:    wfmFreeBuffer
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_wfmFreeBuffer
  (JNIEnv *, jobject, jbyteArray);

/*
 * Class:     at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
 * Method:    write0
 * Signature: ([BI[B)V
 */
JNIEXPORT void JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_write0
  (JNIEnv *, jobject, jbyteArray, jint, jbyteArray);

/*
 * Class:     at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
 * Method:    read0
 * Signature: ([BII)[B
 */
JNIEXPORT jbyteArray JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_read0
  (JNIEnv *, jobject, jbyteArray, jint, jint);

/*
 * Class:     at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem
 * Method:    sizeof
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_at_o2xfs_xfs_spi_std_memory_StdSpiMemorySystem_sizeof
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
