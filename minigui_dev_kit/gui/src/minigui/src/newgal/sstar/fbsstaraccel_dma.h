#ifndef __FB_SSTAR_DMA_H__
#define __FB_SSTAR_DMA_H__

#if defined(FB_ACCEL_SSTAR_DMA)
#include <mi_common.h>
#include <mi_sys.h>
#include <mi_sys_datatype.h>

#define Sstar_HWAccelBlit NULL
#define Sstar_HWAccelFillRect NULL
#define Sstar_HWAccelColorKey Sstar_DMA_HWAccelColorKey
#define Sstar_HWAccelAlpha Sstar_DMA_HWAccelAlpha
#endif
#endif
