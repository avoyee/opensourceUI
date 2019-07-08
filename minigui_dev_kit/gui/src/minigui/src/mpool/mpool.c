//3 //std
#include <pthread.h>
#include <unistd.h>
#include <string.h>
//3 //kernel
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

//3 //sdk
#include <mi_sys.h>
//3 //local
#include <common.h>
#include <minigui.h>
#include "mplite.h"
#include "mpool.h"



typedef struct mpoolContext {
    BOOL cached;
    size_t size;
    MI_PHY phyAddr;
    char *virtAddr;
    mplite_t handle;
    pthread_mutex_t mutex;
    int user;
} mpoolCtx;


static mpoolCtx *Ctx = NULL;

int mpInit()
{
#define BUFSIZELEN 10
    char bufSizeStr[BUFSIZELEN] = {0};
    size_t bufSize = MMA_BUFFER_DEFAULT_SIZE;
    MI_PHY phyAddr = 0;
    char *virtAddr = NULL;
    size_t pagesize = (size_t) sysconf(_SC_PAGESIZE);

    if(Ctx != NULL)
        return 0;

    if(GetMgEtcValue("system", MMA_BUFFER_SIZE_STR, bufSizeStr, BUFSIZELEN) < 0) {
        fprintf(stderr, "[mpool] mma buffer len not config in Minigui.cfg[system:%s], default to %x\n", \
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

    if(mgIsServer) {
        int shmid = 0;
        shmid = shmget(MMA_CTX_SHM_KEY, sizeof(mpoolCtx), IPC_CREAT | 0666 | IPC_EXCL);
        Ctx = (mpoolCtx *) shmat(shmid, 0, 0);
    } else {
        int shmid = 0;
        shmid = shmget(MMA_CTX_SHM_KEY, 0, 0);
        Ctx = (mpoolCtx *) shmat(shmid, 0, 0);
    }

#elif _MGRM_THREADS
    Ctx = (mpoolCtx *)malloc(sizeof(mpoolCtx));
#else
#error "unsupported run mode"
#endif
    Ctx->cached = FALSE;
    MI_SYS_MMA_Alloc((MI_U8 *)MMA_HEAP_NAME, bufSize, &phyAddr);
    MI_SYS_Mmap(phyAddr, bufSize, (void **)&virtAddr, Ctx->cached);
    pthread_mutex_init(&Ctx->mutex, NULL);
    mplite_init(&Ctx->handle, virtAddr, bufSize, pagesize, NULL);
    Ctx->virtAddr = virtAddr;
    Ctx->phyAddr = phyAddr;
    Ctx->size = bufSize;
    return 0;
}


int mpDeInit(void)
{
    MI_SYS_Munmap(Ctx->virtAddr, Ctx->size);
    MI_SYS_MMA_Free(Ctx->phyAddr);

#ifdef _MGRM_PROCESSES

    if(mgIsServer) {
        pthread_mutex_destroy(&Ctx->mutex);
        shmdt(Ctx);
    }

#elif _MGRM_THREADS
    pthread_mutex_destroy(&Ctx->mutex);
    free(Ctx);
#else
#error "unsupported run mode"
#endif
    return 0;
}

int mpMalloc(size_t size, MI_PHY *phyAddr, void **virtAddr)
{
    void *ret = 0;

    if((ret = mplite_malloc(&Ctx->handle, (const int)size)) == NULL) {
        fprintf(stderr, "[mpool] alloc fail\n");
        return -1;
    }

    *virtAddr = ret;
    *phyAddr = (char *)*virtAddr - Ctx->virtAddr + Ctx->phyAddr;
    return 0;
}
int mpFree(void *virtAddr)
{
    mplite_free(&Ctx->handle, virtAddr);
    return 0;
}

