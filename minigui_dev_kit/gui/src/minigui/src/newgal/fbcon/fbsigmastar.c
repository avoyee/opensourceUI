#include <stdio.h>
#include <sys/ioctl.h>
#include "common.h"
#include "newgal.h"
#include "blit.h"
#include "fbsigmastar.h"
#include "fbvideo.h"
#ifdef FB_SIGMASTAR
#include "mstarFb.h"
#endif


#include <time.h>

#if defined (FB_ACCEL_SIGMASTAR_I2) || defined (FB_ACCEL_SIGMASTAR_I2M)

// TODO:: change impl & name
unsigned int GALFmtToMStarFmt(GAL_Surface *pSurface)
{
    MI_GFX_ColorFmt_e eGFXcolorFmt = E_MI_GFX_FMT_MAX;

    switch(pSurface->format->BytesPerPixel) {
        case 1: {
            eGFXcolorFmt = E_MI_GFX_FMT_I8;
        }
        break;

        case 2: {
            switch(pSurface->format->Ashift) {
                case 0:
                    eGFXcolorFmt = E_MI_GFX_FMT_RGB565;
                    break;

                case 12:
                    eGFXcolorFmt = E_MI_GFX_FMT_ARGB4444;
                    break;

                case 15:
                    eGFXcolorFmt = E_MI_GFX_FMT_ARGB1555;
                    break;

                default: {

                    fprintf(stderr, "%s %d default to E_MI_GFX_FMT_RGBA5551\n", __FUNCTION__, __LINE__);
                    eGFXcolorFmt = E_MI_GFX_FMT_RGBA5551;
                    break;

                }
            }
        }
        break;

        case 4: {
            eGFXcolorFmt = E_MI_GFX_FMT_ARGB8888;
        }
        break;

        default: {

            fprintf(stderr, "%s %d unkown color format\n", __FUNCTION__, __LINE__);

        }
    }

    //fprintf(stderr, "%s %d  color format %d\n", __FUNCTION__, __LINE__,eGFXcolorFmt);

    return eGFXcolorFmt;
}
static int Sigmastar_I2_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color);


static int Sigmastar_I2_HWAccelBlit(GAL_Surface *src, GAL_Rect *srcrect, GAL_Surface *dst, GAL_Rect *dstrect)
{

    MI_GFX_Surface_t stSrc;
    MI_GFX_Rect_t stSrcRect;
    MI_GFX_Surface_t stDst;
    MI_GFX_Rect_t stDstRect;
    MI_GFX_Opt_t stOpt;
    MI_U16 u16Fence;
    Uint8 keyR, keyG, keyB;
    //clock_t start,end;
    stDst.phyAddr = ABS(dst->phy_addr);

    stDst.eColorFmt = GALFmtToMStarFmt(dst);
    stDst.u32Width = dst->w ;
    stDst.u32Height = dst->h ;
    stDst.u32Stride = dst->pitch ;

    stDstRect.s32Xpos = dstrect->x;
    stDstRect.s32Ypos = dstrect->y;
    stDstRect.u32Width = dstrect->w;
    stDstRect.u32Height = dstrect->h;

    stSrc.phyAddr = ABS(src->phy_addr);


    stSrc.eColorFmt = GALFmtToMStarFmt(src);
    stSrc.u32Width = src->w ;
    stSrc.u32Height = src->h ;
    stSrc.u32Stride = src->pitch ;

    stSrcRect.s32Xpos = srcrect->x;
    stSrcRect.s32Ypos = srcrect->y;
    stSrcRect.u32Width = srcrect->w;
    stSrcRect.u32Height = srcrect->h;

    memset(&stOpt, 0, sizeof(stOpt));

    stOpt.bEnGfxRop = FALSE;
    stOpt.eRopCode = E_MI_GFX_ROP_WHITE;
    stOpt.eSrcDfbBldOp = E_MI_GFX_DFB_BLD_SRCCOLOR;
    stOpt.eDstDfbBldOp = E_MI_GFX_DFB_BLD_DESTCOLOR;
    stOpt.eMirror = E_MI_GFX_MIRROR_NONE;
    stOpt.eRotate = E_MI_GFX_ROTATE_0;
    stOpt.eSrcYuvFmt = 0;
    stOpt.eDstYuvFmt = 0;
    stOpt.stClipRect.s32Xpos = 0;
    stOpt.stClipRect.s32Ypos = 0;
    stOpt.stClipRect.u32Width  = 0;
    stOpt.stClipRect.u32Height = 0;

    //MI_GFX_SetAlpha(TRUE, E_MI_GFX_COEF_ASRC, E_MI_GFX_ABL_FROM_ADST,src->format->alpha);
    GAL_GetRGB(src->format->colorkey, src->format, &keyR, &keyG, &keyB);
    /*
    stOpt.stSrcColorKeyInfo.bEnColorKey = TRUE;
    stOpt.stSrcColorKeyInfo.eCKeyFmt = stSrc.eColorFmt;
    stOpt.stSrcColorKeyInfo.eCKeyOp = E_MI_GFX_ARGB_OP_EQUAL;
    stOpt.stSrcColorKeyInfo.stCKeyVal.u32ColorStart = src->format->colorkey; 
    stOpt.stSrcColorKeyInfo.stCKeyVal.u32ColorEnd = src->format->colorkey;
    printf("%s %d srcck %x\n", __FUNCTION__, __LINE__,src->format->colorkey);
    
    stOpt.stDstColorKeyInfo.bEnColorKey = FALSE;
    stOpt.stDstColorKeyInfo.eCKeyFmt = stDst.eColorFmt;
    stOpt.stDstColorKeyInfo.eCKeyOp = E_MI_GFX_ARGB_OP_EQUAL;
    stOpt.stDstColorKeyInfo.stCKeyVal.u32ColorStart = dst->format->colorkey;
    stOpt.stDstColorKeyInfo.stCKeyVal.u32ColorEnd = dst->format->colorkey;
    printf("%s %d dstck %x\n", __FUNCTION__, __LINE__,dst->format->colorkey);
    
    */
    //start = clock();
/*
    if(src->phy_addr<0) {
        MI_SYS_FlushInvCache(((vidmem_bucket *)src->hwdata)->base, ((vidmem_bucket *)src->hwdata)->size);
    }
*/
    MI_GFX_BitBlit(&stSrc, &stSrcRect, &stDst, &stDstRect, &stOpt, &u16Fence);
    MI_GFX_WaitAllDone(FALSE, u16Fence);
    //end = clock();
    //printf("%s %d %f %d %d\n", __FUNCTION__, __LINE__, (float)(end - start) / CLOCKS_PER_SEC, stDstRect.u32Width, stDstRect.u32Height);
    return TRUE ;
}
static int Sigmastar_I2_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color)
{
    MI_GFX_Surface_t stDst;
    MI_GFX_Rect_t stDstRect;
    MI_U32 u32ColorVal;
    MI_U16 u16Fence;
    //void *A = NULL;
    MI_U8  a,r,g,b;
    stDst.phyAddr = ABS(dst->phy_addr);


    stDst.eColorFmt = GALFmtToMStarFmt(dst);
    stDst.u32Width = dst->w ;
    stDst.u32Height = dst->h ;
    stDst.u32Stride = dst->pitch ;

    stDstRect.s32Xpos = rect->x;
    stDstRect.s32Ypos = rect->y;
    stDstRect.u32Width = rect->w;
    stDstRect.u32Height = rect->h;

    GAL_GetRGBA(color,dst->format,&r,&g,&b,&a);
    u32ColorVal = a<<24|r<<16|g<<8|b;
    //start = clock();
/*
    if(dst->phy_addr<0) {
        MI_SYS_FlushInvCache(((vidmem_bucket *)dst->hwdata)->base, ((vidmem_bucket *)dst->hwdata)->size);
        //MI_SYS_Mmap(stDst.phyAddr, ((vidmem_bucket *)dst->hwdata)->size, &A, 2);
    }
        
*/
    //end = clock();
    //printf("%s %d %f %d %d\n", __FUNCTION__, __LINE__, (float)(end - start) / CLOCKS_PER_SEC, stDstRect.u32Width, stDstRect.u32Height);
    //start = clock();
    MI_GFX_QuickFill(&stDst, &stDstRect, u32ColorVal, &u16Fence);
    dst->gfx_fence = u16Fence;
    //end = clock();
    //printf("%s %d %f %d %d\n", __FUNCTION__, __LINE__, (float)(end - start) / CLOCKS_PER_SEC, stDstRect.u32Width, stDstRect.u32Height);
    MI_GFX_WaitAllDone(FALSE, u16Fence);
    return TRUE;
}

static MI_U32 convert1555To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
{
    MI_U32 fullclr = 0;
    MI_U16 clr = (0x1 << 15) | ((u8Red & 0x1f) << 10)
                 | ((u8Green & 0x1f) << 5) | (u8Blue & 0x1f);
    //amplify
    fullclr = ((clr >> 10) & 0x1f) << 3; //R Component
    fullclr |= ((clr >> 12) & 0x7);
    fullclr = fullclr << 8;
    fullclr |= ((clr >> 5) & 0x1f) << 3; // G Component
    fullclr |= ((clr >> 7) & 0x7);
    fullclr = fullclr << 8;
    fullclr |= (clr & 0x1f) << 3;       // B
    fullclr |= ((clr >> 2) & 0x7);
    return fullclr;
}
/**
 *Convert RGB565 chanel from RGB565 to ARGB8888
 */
static MI_U32 convert565To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
{
    MI_U32 fullclr = 0;
    MI_U16 clr = ((u8Red & 0x1f) << 11) | ((u8Green & 0x3f) << 5)
                 | (u8Blue & 0x1f);
    //amplify to ARGB8888
    fullclr = ((clr >> 11) & 0x1f) << 3; //R
    fullclr |= ((clr >> 13) & 0x7);
    fullclr = fullclr << 8;
    fullclr |= ((clr >> 5) & 0x3f) << 2; //G
    fullclr |= ((clr >> 9) & 0x3);
    fullclr = fullclr << 8;
    fullclr |= (clr & 0x1f) << 3; //B
    fullclr |= ((clr >> 2) & 0x7);
    return fullclr;
}
/**
 *Convert ARGB4444 chanel from ARGB4444 to ARGB8888
 */
static MI_U32 convert4444To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
{
    MI_U32 fullclr = 0;
    MI_U16 clr = ((u8Red & 0xf) << 8) | ((u8Green & 0xf) << 4)
                 | ((u8Blue & 0xf));
    //amplify to ARGB8888
    fullclr = ((clr >> 8) & 0xf) << 4; //R
    fullclr |= ((clr >> 8) & 0xf);
    fullclr = fullclr << 8;
    fullclr |= ((clr >> 4) & 0xf) << 4; //G
    fullclr |= ((clr >> 4) & 0xf);
    fullclr = fullclr << 8;
    fullclr |= (clr & 0xf) << 4; //B
    fullclr |= (clr & 0xf);
    return fullclr;
}

static int Sigmastar_I2_HWAccelColorKey(_THIS, GAL_Surface *src, Uint32 key)
{
    MI_FB_ColorKey_t colorKeyInfo;
    src->format->colorkey = key;
    src->flags |= GAL_SRCCOLORKEY;

    MI_U32 fullClrKey = 0;
    colorKeyInfo.bKeyEnable = TRUE;
    colorKeyInfo.u8Red = GetRValue(key);
    colorKeyInfo.u8Green =  GetGValue(key);
    colorKeyInfo.u8Blue =  GetBValue(key);

    MI_GFX_ColorFmt_e eGFXcolorFmt = GALFmtToMStarFmt(src);

    switch(eGFXcolorFmt) {
        case E_MI_GFX_FMT_RGB565: {
            colorKeyInfo.u8Red = (colorKeyInfo.u8Red >> 3) & (0x1f);
            colorKeyInfo.u8Green = (colorKeyInfo.u8Green >> 2) & (0x3f);
            colorKeyInfo.u8Blue = (colorKeyInfo.u8Blue >> 3) & (0x1f);
        }
        break;

        case E_MI_GFX_FMT_ARGB4444: {
            colorKeyInfo.u8Red = (colorKeyInfo.u8Red >> 4) & (0xf);
            colorKeyInfo.u8Green = (colorKeyInfo.u8Green >> 4) & (0xf);
            colorKeyInfo.u8Blue = (colorKeyInfo.u8Blue >> 4) & (0xf);
        }
        break;

        case E_MI_GFX_FMT_ARGB1555: {
            colorKeyInfo.u8Red = (colorKeyInfo.u8Red >> 3) & (0x1f);
            colorKeyInfo.u8Green = (colorKeyInfo.u8Green >> 3) & (0x1f);
            colorKeyInfo.u8Blue = (colorKeyInfo.u8Blue >> 3) & (0x1f);
        }
        break;

        default:
            printf("convertColorKeyByFmt colorfmt is %d\n", eGFXcolorFmt);
            break;
    }

    /*
     *Convert color key expressed as current format to ARGB888
     *Because GOP hw can only support ARGB888 format
     */
    switch(eGFXcolorFmt) {
        case E_MI_GFX_FMT_RGB565: {
            fullClrKey = convert565To8888(colorKeyInfo.u8Red,
                                          colorKeyInfo.u8Green, colorKeyInfo.u8Blue);
        }
        break;

        case E_MI_GFX_FMT_ARGB4444: {
            fullClrKey = convert4444To8888(colorKeyInfo.u8Red,
                                           colorKeyInfo.u8Green, colorKeyInfo.u8Blue);
        }
        break;

        case E_MI_GFX_FMT_ARGB1555: {
            fullClrKey = convert1555To8888(colorKeyInfo.u8Red,
                                           colorKeyInfo.u8Green, colorKeyInfo.u8Blue);
        }
        break;

        case E_MI_GFX_FMT_ARGB8888: {
            fullClrKey = (0xff << 24) | (colorKeyInfo.u8Red << 16)
                         | (colorKeyInfo.u8Red << 8) | (colorKeyInfo.u8Red);
        }
        break;

        default : {
            printf("generateClrKeyFromRGB888 invalid clrfmt=%d\n", eGFXcolorFmt);
        }
        break;
    }

    /**
     *Set RGB chanel according to fullClrKey
     */
    colorKeyInfo.u8Red = (fullClrKey >> 16) & 0xff;
    colorKeyInfo.u8Green = (fullClrKey >> 8) & 0xff;
    colorKeyInfo.u8Blue = fullClrKey & 0xff;

    if(ioctl(console_fd, FBIOSET_COLORKEY, &colorKeyInfo) < 0) {
        GAL_SetError("NEWGAL>FBCON: Couldn't set sigmastar fb color key");
        return (-1);
    }

    return 0;
}


static int Sigmastar_I2_HWAccelAlpha(_THIS, GAL_Surface *surface, Uint8 alpha)
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


#elif defined (FB_ACCEL_SIGMASTAR_I5)

int Sigmastar_I5_HWAccelBlit(GAL_Surface *src, GAL_Rect *srcrect, GAL_Surface *dst, GAL_Rect *dstrect)
{

    return TRUE ;
}

int Sigmastar_I5_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color)
{

    return TRUE;
}


int Sigmastar_I5_HWAccelColorKey(_THIS, GAL_Surface *src, Uint32 key)
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

int Sigmastar_I5_HWAccelAlpha(_THIS, GAL_Surface *surface, Uint8 alpha)
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
#endif


int Sigmastar_CheckHWBlit(_THIS, GAL_Surface *src, GAL_Surface *dst)
{
    src->flags |= GAL_HWACCEL;
    src->map->hw_blit = Sigmastar_HWAccelBlit;
    return 0;
}
void FB_SigmastarAccel(_THIS)
{
#if defined (FB_ACCEL_SIGMASTAR_I2) || defined (FB_ACCEL_SIGMASTAR_I2M)
    MI_GFX_Open();
#elif defined(FB_ACCEL_SIGMASTAR_I5) 
#endif
    /* We have hardware accelerated surface functions */
    this->CheckHWBlit = Sigmastar_CheckHWBlit;
    this->info.blit_hw = ((void *)Sigmastar_HWAccelBlit) == NULL ? FALSE : TRUE;
    /* FillRect */
    this->info.blit_fill = ((void *)Sigmastar_HWAccelFillRect) == NULL ? FALSE : TRUE;
    /* Colorkey */
    //(video_info->blit_hw_CC && (flags & FLAGS_BLIT_HW_CC)) ? 1 : 0;
    this->info.blit_hw_CC = ((void *)Sigmastar_HWAccelBlit) == NULL ? FALSE : TRUE;
    /* Alpha */
    this->info.blit_hw_A  = ((void *)Sigmastar_HWAccelBlit) == NULL ? FALSE : TRUE;

    this->FillHWRect = Sigmastar_HWAccelFillRect;
    this->SetHWColorKey = Sigmastar_HWAccelColorKey;
    this->SetHWAlpha = Sigmastar_HWAccelAlpha;
}
