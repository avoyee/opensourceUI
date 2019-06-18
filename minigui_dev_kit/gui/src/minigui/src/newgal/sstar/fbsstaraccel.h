#ifndef __FBSSTAR_ACCEL_H__
#define __FBSSTAR_ACCEL_H__

#include "fbsstaraccel_gfx.h"
#include "fbsstaraccel_dma.h"
#include "fbsstar.h"
int Sstar_GFX_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color);
int Sstar_GFX_HWAccelBlit(GAL_Surface *src, GAL_Rect *srcrect, GAL_Surface *dst, GAL_Rect *dstrect);
int Sstar_GFX_HWAccelColorKey(_THIS, GAL_Surface *src, Uint32 key);
int Sstar_GFX_HWAccelAlpha(_THIS, GAL_Surface *surface, Uint8 alpha);
int Sstar_DMA_HWAccelBlit(GAL_Surface *src, GAL_Rect *srcrect, GAL_Surface *dst, GAL_Rect *dstrect);
int Sstar_DMA_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color);
int Sstar_DMA_HWAccelColorKey(_THIS, GAL_Surface *src, Uint32 key);
int Sstar_DMA_HWAccelAlpha(_THIS, GAL_Surface *surface, Uint8 alpha);

void FB_SstarAccel(_THIS);
void FB_SstarRotate(MI_PHY DrawPhyAddr,MI_PHY DrawPhyAddr1, MI_U16 u16SrcBufW, MI_U16 u16SrcBufH,
    GAL_Surface *dst, MI_S32 s32Rotate, MI_S32 s32AlignStride);

#endif
