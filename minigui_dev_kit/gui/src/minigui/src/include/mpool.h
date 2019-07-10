#ifndef __MPOOL_H__
#define __MPOOL_H__
//3 //sdk
#include <mi_common.h>
#include <mi_sys_datatype.h>

#define MMA_BUFFER_SIZE_STR "mmaBufSize"
#define MMA_BUFFER_DEFAULT_SIZE 0x800000 //3 //8M
#define MMA_HEAP_NAME "#GUIMPOOL"
#define MMA_CTX_SHM_KEY 0xAFBFCFDF
#define MMA_CTX_SHM_HANDLE 0xAEBECEDE
#define MMA_CTX_LOCK_HANDLE 0xADBDCDDD

#define MMA_CTX_SEM_NAME "mmampoollock"
// TODO:: need mutlipool support ?? 
int mpInit(void); 
int mpDeInit(void);
int mpMalloc(size_t size,MI_PHY *phyAddr,void** virtAddr);
int mpFree(size_t size,MI_PHY phyAddr,void *virtAddr);

#endif
