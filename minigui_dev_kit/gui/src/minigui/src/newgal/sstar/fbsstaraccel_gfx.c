#include <stdio.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>
#include "common.h"
#include "newgal.h"
#include "blit.h"
#include "mstarFb.h"
#include "fbsstaraccel.h"

#if defined(FB_ACCEL_SSTAR_GFX)
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
int Sstar_GFX_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color);


int Sstar_GFX_HWAccelBlit(GAL_Surface *src, GAL_Rect *srcrect, GAL_Surface *dst, GAL_Rect *dstrect)
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

    stOpt.eSrcDfbBldOp = E_MI_GFX_DFB_BLD_ONE;
    stOpt.eDstDfbBldOp = E_MI_GFX_DFB_BLD_ZERO;
    stOpt.eMirror = E_MI_GFX_MIRROR_NONE;
    stOpt.eRotate = E_MI_GFX_ROTATE_0;
    stOpt.stClipRect.s32Xpos = 0;
    stOpt.stClipRect.s32Ypos = 0;
    stOpt.stClipRect.u32Width  = 0;
    stOpt.stClipRect.u32Height = 0;
    stOpt.u32GlobalSrcConstColor = 0xFF000000;
    stOpt.u32GlobalDstConstColor = 0xFF000000;

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
int Sstar_GFX_HWAccelFillRect(_THIS, GAL_Surface *dst, GAL_Rect *rect, Uint32 color)
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

MI_U32 convert1555To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
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
MI_U32 convert565To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
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
MI_U32 convert4444To8888(MI_U8 u8Red, MI_U8 u8Green, MI_U8 u8Blue)
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

int Sstar_GFX_HWAccelColorKey(_THIS, GAL_Surface *src, Uint32 key)
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


int Sstar_GFX_HWAccelAlpha(_THIS, GAL_Surface *surface, Uint8 alpha)
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
    MI_U16 u16TargetFence;
    MI_U16 u16DstWidth=0, u16DstHeight=0;
    MI_GFX_Surface_t stSrc, stDst;
    MI_GFX_Rect_t stSrcRect, stDstRect;
    MI_GFX_Opt_t stGfxOpt;
    MI_S32 s32DivX, s32WDiv, s32HDiv, i, j;
    struct timeval tv_start,tv_end;
    MI_S32 s32PixelperByte = 4;//ARGB8888
    MI_GFX_ColorFmt_e s32ColorFmt = GALFmtToMStarFmt(surface);
    if (E_MI_GFX_FMT_ARGB8888 == s32ColorFmt)
    {
        s32PixelperByte = 4;//32bit/8
    }
    else if (E_MI_GFX_FMT_ARGB1555 == s32ColorFmt || E_MI_GFX_FMT_RGB565 == s32ColorFmt || E_MI_GFX_FMT_ARGB4444 == s32ColorFmt)
    {
        s32PixelperByte = 2;//16bit/8
    }
    else
    {
        printf("Unsupport color format(%d)...\n", s32ColorFmt);
        return;
    }
    /*_____________
      |__|__|__|__|
      |__|__|__|__|
      |__|__|__|__|
      s32DivX -> square length == s32AlignStride
      s32DivW -> == 4
      s32DivH -> == 3
    */
    s32DivX = s32AlignStride;// pixel align
    s32WDiv = (u16SrcBufW%s32DivX)?(u16SrcBufW/s32DivX+1):u16SrcBufW/s32DivX;
    s32HDiv = (u16SrcBufH%s32DivX)?(u16SrcBufH/s32DivX+1):u16SrcBufH/s32DivX;

    if ((E_MI_GFX_ROTATE_270 == s32Rotate) || (E_MI_GFX_ROTATE_90 == s32Rotate))
    {
        u16DstWidth = s32HDiv*s32DivX;
        u16DstHeight = s32WDiv*s32DivX;
    }
    else if ((E_MI_GFX_ROTATE_0 == s32Rotate) || (E_MI_GFX_ROTATE_180 == s32Rotate))
    {
        u16DstWidth = s32WDiv*s32DivX;
        u16DstHeight = s32HDiv*s32DivX;
    }


    //bitblit Square buffer to rotation buffer
    stSrc.eColorFmt = s32ColorFmt;
    stSrc.u32Width = u16SrcBufW;
    stSrc.u32Height = u16SrcBufH;
    stSrc.u32Stride = u16SrcBufW * s32PixelperByte;
    stSrc.phyAddr = DrawPhyAddr;//src buffer

    stDst.eColorFmt = s32ColorFmt;
    stDst.u32Width = u16DstWidth;
    stDst.u32Height = u16DstHeight;
    stDst.u32Stride = u16DstWidth * s32PixelperByte;
    stDst.phyAddr = DrawPhyAddr1;

    memset(&stGfxOpt, 0, sizeof(MI_GFX_Opt_t));


    stGfxOpt.eSrcDfbBldOp = E_MI_GFX_DFB_BLD_ONE;
    stGfxOpt.eDstDfbBldOp = E_MI_GFX_DFB_BLD_ZERO;
    stGfxOpt.eMirror = E_MI_GFX_MIRROR_NONE;
    stGfxOpt.eRotate = s32Rotate;
    stGfxOpt.stClipRect.s32Xpos = 0;
    stGfxOpt.stClipRect.s32Ypos = 0;
    stGfxOpt.stClipRect.u32Width  = 0;
    stGfxOpt.stClipRect.u32Height = 0;
    stGfxOpt.u32GlobalSrcConstColor = 0xFF000000;
    stGfxOpt.u32GlobalDstConstColor = 0xFF000000;

    gettimeofday(&tv_start, NULL);

    stSrcRect.u32Width = s32DivX;
    stSrcRect.u32Height = s32DivX;
    stDstRect.u32Width = s32DivX;
    stDstRect.u32Height = s32DivX;
    for (j = 0; j < s32HDiv; j++)
    {
        for (i = 0; i < s32WDiv; i++)
        {
            stSrcRect.s32Xpos = s32DivX * i;
            stSrcRect.s32Ypos = s32DivX * j;
            if (E_MI_GFX_ROTATE_270 == stGfxOpt.eRotate)
            {
                stDstRect.s32Xpos = s32DivX * j;
                stDstRect.s32Ypos = s32DivX * (s32WDiv-i-1);
            }
            else if (E_MI_GFX_ROTATE_180 == stGfxOpt.eRotate)
            {
                stDstRect.s32Xpos = s32DivX * (s32WDiv-i-1);
                stDstRect.s32Ypos = s32DivX * (s32HDiv-j-1);
            }
            else if (E_MI_GFX_ROTATE_90 == stGfxOpt.eRotate)
            {
                stDstRect.s32Xpos = s32DivX * (s32HDiv-j-1);
                stDstRect.s32Ypos = s32DivX * i;
            }
            MI_GFX_BitBlit(&stSrc, &stSrcRect, &stDst, &stDstRect, &stGfxOpt, &u16TargetFence);  
        }
    }

    MI_GFX_WaitAllDone(FALSE, u16TargetFence);//wait gfx done
    gettimeofday(&tv_end, NULL);

    //printf("w=%d h=%d slice time = %ld ms...%ld-%ld  %ld-%ld...\n", u16DstWidth, u16DstHeight, (tv_end.tv_sec*1000 + tv_end.tv_usec/1000)-(tv_start.tv_sec * 1000+tv_start.tv_usec/1000),
    //    tv_end.tv_sec, tv_end.tv_usec, tv_start.tv_sec, tv_start.tv_usec);
}

#endif
