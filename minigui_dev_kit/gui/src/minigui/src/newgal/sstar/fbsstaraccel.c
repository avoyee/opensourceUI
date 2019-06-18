#include "common.h"
#include "gdi.h"
#include "newgal.h"
#include "blit.h"
#include "fbsstaraccel.h"

static int FB_CheckHWBlit(_THIS, GAL_Surface *src, GAL_Surface *dst)
{
    src->flags |= GAL_HWACCEL;
    src->map->hw_blit = Sstar_HWAccelBlit;
    return 0;
}


void FB_SstarAccel(_THIS)
{
#if defined (FB_ACCEL_SSTAR_GFX) 
    MI_GFX_Open();
#endif
    /* We have hardware accelerated surface functions */
    this->CheckHWBlit = FB_CheckHWBlit;
    this->info.blit_hw = ((void *)Sstar_HWAccelBlit) == NULL ? FALSE : TRUE;
    /* FillRect */
    this->info.blit_fill = ((void *)Sstar_HWAccelFillRect) == NULL ? FALSE : TRUE;
    /* Colorkey */
    //(video_info->blit_hw_CC && (flags & FLAGS_BLIT_HW_CC)) ? 1 : 0;
    this->info.blit_hw_CC = ((void *)Sstar_HWAccelBlit) == NULL ? FALSE : TRUE;
    /* Alpha */
    this->info.blit_hw_A  = ((void *)Sstar_HWAccelBlit) == NULL ? FALSE : TRUE;

    this->FillHWRect = Sstar_HWAccelFillRect;
    this->SetHWColorKey = Sstar_HWAccelColorKey;
    this->SetHWAlpha = Sstar_HWAccelAlpha;
}
