//3 //std
#include <pthread.h>
#include <unistd.h>
#include <string.h>
//3 //kernel
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
//3 //sdk
#include <mi_sys.h>
//3 //local
#include <common.h>
#include <minigui.h>
#include "mplite.h"
#include "mpool.h"

//#define _MGRM_PROCESSES
#define NATIVE_MMA
typedef struct mpoolContext {
    BOOL cached;
    size_t size;
    MI_PHY phyAddr;
    char *virtAddr;
    mplite_t *handle;
#ifdef _MGRM_PROCESSES
    sem_t *lock;
#else
    pthread_mutex_t lock;
#endif
    mplite_lock_t* lockh;
    int user;
} mpoolCtx;


#ifdef _MGRM_PROCESSES

static int __mp_sem_wait(void* lock)
{
    int ret;
    printf("%s in\n",__FUNCTION__);
    ret = sem_wait(lock);
    printf("%s out\n",__FUNCTION__);
    return ret;
}
static int __mp_sem_post(void* lock)
{
    int ret;
    printf("%s in\n",__FUNCTION__);
    ret = sem_post(lock);
    printf("%s out\n",__FUNCTION__);
    return ret;
}


#elif _MGRM_THREADS

#endif

#ifdef NATIVE_MMA
int mpInit()
{
    MI_SYS_Init();
    return 0;
}

#else
static mpoolCtx *Ctx = NULL;

int mpInit()
{
#define BUFSIZELEN 10
    char bufSizeStr[BUFSIZELEN] = {0};
    size_t bufSize = MMA_BUFFER_DEFAULT_SIZE;
    MI_PHY phyAddr = 0;
    char *virtAddr = NULL;
    mplite_lock_t pool_lock;
    size_t pagesize = (size_t) sysconf(_SC_PAGESIZE);

    if(Ctx != NULL)
        return 0;

    if(GetMgEtcValue("system", MMA_BUFFER_SIZE_STR, bufSizeStr, BUFSIZELEN) < 0) {
        fprintf(stderr, "[mpool] mma buffer len not config in Minigui.cfg[system:%s], default to 0x%x\n", \
                MMA_BUFFER_SIZE_STR, MMA_BUFFER_DEFAULT_SIZE);
    } else {
        if(strncasecmp(bufSizeStr, "0x", 2)) {
            fprintf(stderr, "[mpool] mma buffer len should be hexdecimal\n");
            return -1;
        }

        bufSize = strtol(bufSizeStr, NULL, 16);

        if(pagesize > bufSize) {
            fprintf(stderr, "[mpool] mma buffer len too small\n");
            return -1;
        }
    }

    MI_SYS_Init();
#ifdef _MGRM_PROCESSES
#if 0

    if(mgIsServer) {
        int shmid = 0;
        shmid = shmget(MMA_CTX_SHM_KEY, sizeof(mpoolCtx), IPC_CREAT | 0666 | IPC_EXCL);
        Ctx = (mpoolCtx *) shmat(shmid, 0, 0);
        Ctx->lock = sem_open(MMA_CTX_SEM_NAME, O_CREAT | O_EXCL, 0666, 0);
        shmid = shmget(MMA_CTX_SHM_HANDLE, sizeof(mplite_t), IPC_CREAT | 0666 | IPC_EXCL);
        Ctx->handle = (mplite_t *)shmat(shmid, 0, 0);
    } else {
        int shmid = 0;
        shmid = shmget(MMA_CTX_SHM_KEY, 0, 0);
        Ctx = (mpoolCtx *) shmat(shmid, 0, 0);
        Ctx->lock = sem_open(MMA_CTX_SEM_NAME, 0);
        shmid = shmget(MMA_CTX_SHM_HANDLE, 0, 0);
        Ctx->handle = (mplite_t *)shmat(shmid, 0, 0);
    }

#endif
    int shmid = 0;

    shmid = shmget(MMA_CTX_SHM_KEY, sizeof(mpoolCtx), IPC_CREAT | 0666 | IPC_EXCL);

    if(shmid < 0) {
        shmid = shmget(MMA_CTX_SHM_KEY, 0, 0);
        Ctx = (mpoolCtx *) shmat(shmid, 0, 0);
        Ctx->lock = sem_open(MMA_CTX_SEM_NAME, 0);
        shmid = shmget(MMA_CTX_SHM_HANDLE, 0, 0);
        Ctx->handle = (mplite_t *)shmat(shmid, 0, 0);
        shmid = shmget(MMA_CTX_LOCK_HANDLE, 0, 0);
        Ctx->lockh = (mplite_lock_t *)shmat(shmid, 0, 0);
        //Ctx->lockh->arg = (void *)Ctx->lock;
        //Ctx->lockh->acquire = (int (*)(void *))__mp_sem_post;
        //Ctx->lockh->release = (int (*)(void *))__mp_sem_wait;
        //Ctx->lockh->acquire(Ctx->lockh->arg);
        //Ctx->lockh->release(Ctx->lockh->arg);
        
        fprintf(stderr, "%s %d %llx\n",__FUNCTION__,__LINE__,Ctx->phyAddr);
        return 0;
    } else {
        Ctx = (mpoolCtx *) shmat(shmid, 0, 0);
        Ctx->lock = sem_open(MMA_CTX_SEM_NAME, O_CREAT | O_EXCL, 0666, 0);
        shmid = shmget(MMA_CTX_SHM_HANDLE, sizeof(mplite_t), IPC_CREAT | 0666 | IPC_EXCL);
        Ctx->handle = (mplite_t *)shmat(shmid, 0, 0);
        shmid = shmget(MMA_CTX_LOCK_HANDLE, sizeof(mplite_lock_t), IPC_CREAT | 0666 | IPC_EXCL);
        Ctx->lockh = (mplite_lock_t *)shmat(shmid, 0, 0);
        Ctx->lockh->arg = (void *)Ctx->lock;
        Ctx->lockh->acquire = (int (*)(void *))__mp_sem_post;
        Ctx->lockh->release = (int (*)(void *))__mp_sem_wait;
    }

#elif _MGRM_THREADS
    Ctx = (mpoolCtx *)malloc(sizeof(mpoolCtx));
    Ctx->handle = (mplite_t *)malloc(sizeof(mplite_t));
    pthread_mutex_init(&Ctx->lock, NULL);
    pool_lock.arg = (void *)&Ctx->lock;
    pool_lock.acquire = (int (*)(void *))pthread_mutex_lock;
    pool_lock.release = (int (*)(void *))pthread_mutex_unlock;
#else
#error "unsupported run mode"
#endif
    Ctx->cached = FALSE;
    MI_SYS_MMA_Alloc((MI_U8 *)MMA_HEAP_NAME, bufSize, &phyAddr);
    MI_SYS_Mmap(phyAddr, bufSize, (void **)&virtAddr, Ctx->cached);
    mplite_init(Ctx->handle, virtAddr, bufSize, pagesize, &pool_lock);
    Ctx->virtAddr = virtAddr;
    Ctx->phyAddr = phyAddr;
    Ctx->size = bufSize;
    fprintf(stderr, "%s %d %llx\n",__FUNCTION__,__LINE__,Ctx->phyAddr);
    return 0;
}
#endif
#ifdef NATIVE_MMA
int mpDeInit(void)
{
    return 0;
}
#else
int mpDeInit(void)
{
    fprintf(stderr, "%s %d %llx\n",__FUNCTION__,__LINE__,Ctx->phyAddr);

    MI_SYS_Munmap(Ctx->virtAddr, Ctx->size);
    MI_SYS_MMA_Free(Ctx->phyAddr);
#ifdef _MGRM_PROCESSES
    if(mgIsServer) {
        sem_close(Ctx->lock);
        shmdt(Ctx->handle);
        shmdt(Ctx);
    }

#elif _MGRM_THREADS
    pthread_mutex_destroy(&Ctx->lock);
    free(Ctx->handle);
    free(Ctx);
#else
#error "unsupported run mode"
#endif
    return 0;
}
#endif

#ifdef NATIVE_MMA
int mpMalloc(size_t size, MI_PHY *phyAddr, void **virtAddr)
{
    MI_SYS_MMA_Alloc((MI_U8*)"#gui", size,phyAddr);
    MI_SYS_Mmap(*phyAddr, size,virtAddr, FALSE);
    return 0;
}
int mpFree(size_t size,MI_PHY phyAddr,void *virtAddr)
{
    MI_SYS_Munmap(virtAddr, size);
    MI_SYS_MMA_Free(phyAddr);
    return 0;
}

#else

int mpMalloc(size_t size, MI_PHY *phyAddr, void **virtAddr)
{
    void *ret = 0;
    
    if((ret = mplite_malloc(Ctx->handle, (const int)size)) == NULL) {
        fprintf(stderr, "[mpool] %s fail\n",__FUNCTION__);
        return -1;
    }
    *virtAddr = ret;
    *phyAddr = (char *)*virtAddr - Ctx->virtAddr + Ctx->phyAddr;
    return 0;
}
int mpFree(size_t size,MI_PHY phyAddr,void *virtAddr)
{
    mplite_free(Ctx->handle, virtAddr);
    return 0;
}
#endif
