
#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>

#include "common.h"
#include "newgal.h"
#include "blit.h"
#include "mstarFb.h"
#include "fbsstaraccel.h"
#include "mi_sys_datatype.h"
#include "mi_sys.h"

#if defined(FB_ACCEL_SSTAR_DMA)

int Sstar_DMA_HWAccelBlit(GAL_Surface *src, GAL_Rect *srcrect, GAL_Surface *dst, GAL_Rect *dstrect)
{

    return TRUE ;
}

int Sstar_DMA_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color)
{
    //clock_t start,end;
    MI_S32 ret = 0;
    MI_SYS_WindowRect_t rect;
    MI_SYS_FrameData_t stSysFrame;
    memcpy(&stSysFrame, 0, sizeof(MI_SYS_FrameData_t));
    stSysFrame.phyAddr = ABS(dst->phy_addr);
    if(dstRect) {
        rect.u16X = dstRect->s32Xpos;
        rect.u16Y = dstRect->s32Ypos;
        rect.u16Height = dstRect->u32Height;
        rect.u16Width = dstRect->u32Width;
    } else {
        rect.u16X = 0;
        rect.u16Y = 0;
        rect.u16Height = stSysFrame.u16Height;
        rect.u16Width = stSysFrame.u16Width;

    }

    DBG_INFO("rect %d %d %d %d \n", rect.u16X, rect.u16Y
             , rect.u16Width, rect.u16Height);

    //start = clock();
    /*
        if(src->phy_addr<0) {
            MI_SYS_FlushInvCache(((vidmem_bucket *)src->hwdata)->base, ((vidmem_bucket *)src->hwdata)->size);
        }
    */

    ret = mi_sys_BufFillPa(&stSysFrame, u32ColorVal, &rect);

    //end = clock();
    //printf("%s %d %f %d %d\n", __FUNCTION__, __LINE__, (float)(end - start) / CLOCKS_PER_SEC, stDstRect.u32Width, stDstRect.u32Height);
    return TRUE ;
}



int Sstar_DMA_HWAccelColorKey(_THIS, GAL_Surface *src, Uint32 key)
{
    MI_FB_ColorKey_t stColorKet;
    src->format->colorkey = key;
    src->flags |= GAL_SRCCOLORKEY;
    stColorKet.bKeyEnable = TRUE;
    stColorKet.u8Red = GetRValue(key);
    stColorKet.u8Green =  GetGValue(key);
    stColorKet.u8Blue =  GetBValue(key);


    if(ioctl(console_fd, FBIOSET_COLORKEY, &stColorKet) < 0) {
        GAL_SetError("NEWGAL>FBCON: Couldn't set sigmastar fb color key");
        return (-1);
    }

    return 0;
}

int Sstar_DMA_HWAccelAlpha(_THIS, GAL_Surface *surface, Uint8 alpha)
{

    MI_FB_GlobalAlpha_t stGlobalAlpha;
    surface->format->alpha = alpha;
    surface->flags |= GAL_SRCALPHA;
    stGlobalAlpha.bAlphaEnable = TRUE;	/* alpha enable flag */
    stGlobalAlpha.bAlphaChannel = TRUE;	/* alpha channel enable flag */
    stGlobalAlpha.u8Alpha0 = 0; /*alpha0 value*/
    stGlobalAlpha.u8Alpha1 = 0; /*alpha1 value*/
    stGlobalAlpha.u8GlobalAlpha = alpha;	/* global alpha value */

    if(ioctl(console_fd, FBIOSET_GLOBAL_ALPHA, &stGlobalAlpha) < 0) {
        GAL_SetError("NEWGAL>FBCON: Couldn't set sigmastar fb global alpha");
        return (-1);
    }

    return 0;
}
void FB_SstarRotate(MI_PHY DrawPhyAddr, MI_PHY DrawPhyAddr1, MI_U16 u16SrcBufW, MI_U16 u16SrcBufH,
                    GAL_Surface *surface, MI_S32 s32Rotate, MI_S32 s32AlignStride)
{
}

#endif
