
#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>

#include "common.h"
#include "newgal.h"
#include "blit.h"
#include "mstarFb.h"
#include "fbsstaraccel.h"
#if defined(FB_ACCEL_SSTAR_DMA)

int Sstar_DMA_HWAccelBlit(GAL_Surface *src, GAL_Rect *srcrect, GAL_Surface *dst, GAL_Rect *dstrect)
{

    return TRUE ;
}

int Sstar_DMA_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color)
{

    return TRUE;
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
void FB_SstarRotate(MI_PHY DrawPhyAddr,MI_PHY DrawPhyAddr1, MI_U16 u16SrcBufW, MI_U16 u16SrcBufH,
    GAL_Surface *surface, MI_S32 s32Rotate, MI_S32 s32AlignStride)
{
}

#endif
