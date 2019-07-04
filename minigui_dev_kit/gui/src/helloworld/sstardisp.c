#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include "mi_sys.h"
#include "sstardisp.h"

#include "mi_panel_datatype.h"
#include "mi_panel.h"
#include "mi_disp_datatype.h"
#include "mi_disp.h"
//#include "RM68200_720x1280.h"
#include "SAT070AT50_800x480.h"


#if defined(__cplusplus)||defined(c_plusplus)
extern "C"{
#endif
#ifdef MI_HDMI

stTimingArray_t astTimingArray[] = {
    {
        .desc = "user",
        .eOutputTiming = E_MI_DISP_OUTPUT_USER,
        .eHdmiTiming = E_MI_HDMI_TIMING_MAX,
    },
    {.desc = "480p60",
     .eOutputTiming = E_MI_DISP_OUTPUT_480P60,
     .eHdmiTiming = E_MI_HDMI_TIMING_480_60P,
     .u16Width = 640,
     .u16Height = 480},
    {.desc = "576p50",
     .eOutputTiming = E_MI_DISP_OUTPUT_576P50,
     .eHdmiTiming = E_MI_HDMI_TIMING_576_50P,
     .u16Width = 720,
     .u16Height = 576},
    {.desc = "720p50",
     .eOutputTiming = E_MI_DISP_OUTPUT_720P50,
     .eHdmiTiming = E_MI_HDMI_TIMING_720_50P,
     .u16Width = 1280,
     .u16Height = 720},
    {.desc = "720p60",
     .eOutputTiming = E_MI_DISP_OUTPUT_720P60,
     .eHdmiTiming = E_MI_HDMI_TIMING_720_60P,
     .u16Width = 1280,
     .u16Height = 720},
    {.desc = "1024x768_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1024x768_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1024x768_60P,
     .u16Width = 1024,
     .u16Height = 768},
    {.desc = "1080p24",
     .eOutputTiming = E_MI_DISP_OUTPUT_1080P24,
     .eHdmiTiming = E_MI_HDMI_TIMING_1080_24P,
     .u16Width = 1920,
     .u16Height = 1080},
    {.desc = "1080p25",
     .eOutputTiming = E_MI_DISP_OUTPUT_1080P25,
     .eHdmiTiming = E_MI_HDMI_TIMING_1080_25P,
     .u16Width = 1920,
     .u16Height = 1080},
    {.desc = "1080p30",
     .eOutputTiming = E_MI_DISP_OUTPUT_1080P30,
     .eHdmiTiming = E_MI_HDMI_TIMING_1080_30P,
     .u16Width = 1920,
     .u16Height = 1080},
    {.desc = "1080p50",
     .eOutputTiming = E_MI_DISP_OUTPUT_1080P50,
     .eHdmiTiming = E_MI_HDMI_TIMING_1080_50P,
     .u16Width = 1920,
     .u16Height = 1080},
    {.desc = "1080p60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1080P60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1080_60P,
     .u16Width = 1920,
     .u16Height = 1080},
    {.desc = "1280x800_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1280x800_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1280x800_60P,
     .u16Width = 1280,
     .u16Height = 800},
    {.desc = "1280x1024_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1280x1024_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1280x1024_60P,
     .u16Width = 1280,
     .u16Height = 1024},
    {.desc = "1366x768_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1366x768_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1366x768_60P, //HDMI don't support this timing
     .u16Width = 1366,
     .u16Height = 768},
    {.desc = "1440x900_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1440x900_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1440x900_60P,
     .u16Width = 1440,
     .u16Height = 900},
    {.desc = "1680x1050_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1680x1050_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1680x1050_60P, //HDMI don't support this timing
     .u16Width = 1680,
     .u16Height = 1050},
    {.desc = "1600x1200_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_1600x1200_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_1600x1200_60P,
     .u16Width = 1600,
     .u16Height = 1200},
    {.desc = "2560x1440_30",
     .eOutputTiming = E_MI_DISP_OUTPUT_2560x1440_30,
     .eHdmiTiming = E_MI_HDMI_TIMING_MAX, //HDMI don't support this timing
     .u16Width = 2560,
     .u16Height = 1440},
    {.desc = "2560x1440_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_MAX, //not defined
     .eHdmiTiming = E_MI_HDMI_TIMING_MAX,   //HDMI don't support this timing
     .u16Width = 2560,
     .u16Height = 1440},
    {.desc = "2560x1600_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_2560x1600_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_MAX, //HDMI don't support this timing
     .u16Width = 2560,
     .u16Height = 1600},
    {.desc = "3840x2160_30",
     .eOutputTiming = E_MI_DISP_OUTPUT_3840x2160_30,
     .eHdmiTiming = E_MI_HDMI_TIMING_4K2K_30P,
     .u16Width = 3840,
     .u16Height = 2160},
    {.desc = "3840x2160_60",
     .eOutputTiming = E_MI_DISP_OUTPUT_3840x2160_60,
     .eHdmiTiming = E_MI_HDMI_TIMING_4K2K_60P,
     .u16Width = 3840,
     .u16Height = 2160},
};

static MI_S32 Hdmi_Start(MI_HDMI_DeviceId_e eHdmi, MI_HDMI_TimingType_e eTimingType)
{
    MI_HDMI_Attr_t stAttr;
    memset(&stAttr, 0, sizeof(MI_HDMI_Attr_t));
    if (eTimingType == E_MI_HDMI_TIMING_MAX)
    {
        printf("[%s][%d]unsupported hdmi timing %d,reset to 1080p60\n", __FUNCTION__, __LINE__, eTimingType);
        eTimingType = E_MI_HDMI_TIMING_1080_60P;
    }
    stAttr.stEnInfoFrame.bEnableAudInfoFrame = FALSE;
    stAttr.stEnInfoFrame.bEnableAviInfoFrame = FALSE;
    stAttr.stEnInfoFrame.bEnableSpdInfoFrame = FALSE;
    stAttr.stAudioAttr.bEnableAudio = TRUE;
    stAttr.stAudioAttr.bIsMultiChannel = 0;
    stAttr.stAudioAttr.eBitDepth = E_MI_HDMI_BIT_DEPTH_16;
    stAttr.stAudioAttr.eCodeType = E_MI_HDMI_ACODE_PCM;
    stAttr.stAudioAttr.eSampleRate = E_MI_HDMI_AUDIO_SAMPLERATE_48K;
    stAttr.stVideoAttr.bEnableVideo = TRUE;
    stAttr.stVideoAttr.eColorType = E_MI_HDMI_COLOR_TYPE_YCBCR444; //default color type
    stAttr.stVideoAttr.eDeepColorMode = E_MI_HDMI_DEEP_COLOR_MAX;
    stAttr.stVideoAttr.eTimingType = eTimingType;
    stAttr.stVideoAttr.eOutputMode = E_MI_HDMI_OUTPUT_MODE_HDMI;
    MI_HDMI_SetAttr(eHdmi, &stAttr);
    MI_HDMI_Start(eHdmi);

    return MI_SUCCESS;
}

static stTimingArray_t *gettiming(MI_DISP_OutputTiming_e eOutputTiming)
{
    int index = 0;

    for (index = 0; index < sizeof(astTimingArray); index++)
    {
        if (eOutputTiming == astTimingArray[index].eOutputTiming)
        {
            return astTimingArray + index;
        }
    }
    return NULL;
}
#endif
int sstar_disp_init(MI_DISP_PubAttr_t *pstDispPubAttr)
{
    MI_PANEL_LinkType_e eLinkType;
    MI_DISP_InputPortAttr_t stInputPortAttr;
#ifdef MI_HDMI
    stTimingArray_t *pstTiming = gettiming(pstDispPubAttr->eIntfSync);
#endif
    MI_SYS_Init();

    memset(&stInputPortAttr, 0, sizeof(stInputPortAttr));
    stInputPortAttr.u16SrcWidth = pstTiming->u16Width;
    stInputPortAttr.u16SrcHeight = pstTiming->u16Height;
    stInputPortAttr.stDispWin.u16X = 0;
    stInputPortAttr.stDispWin.u16Y = 0;
    stInputPortAttr.stDispWin.u16Width = pstTiming->u16Width;
    stInputPortAttr.stDispWin.u16Height = pstTiming->u16Height;
#ifdef MI_HDMI
    if (pstDispPubAttr->eIntfType == E_MI_DISP_INTF_HDMI)
    {
        MI_HDMI_InitParam_t stInitParam;
        memset(&stInitParam, 0, sizeof(MI_HDMI_InitParam_t));
        stInitParam.pCallBackArgs = NULL;
        stInitParam.pfnHdmiEventCallback = NULL;
        MI_HDMI_Init(&stInitParam);
        MI_HDMI_Open(E_MI_HDMI_ID_0);
        Hdmi_Start(E_MI_HDMI_ID_0, pstTiming->eHdmiTiming);
    }
#else
    if(0){}
#endif
    else if (pstDispPubAttr->eIntfType == E_MI_DISP_INTF_VGA)
    {
        eLinkType = E_MI_PNL_LINK_DAC_P;
    }
    else if (pstDispPubAttr->eIntfType == E_MI_DISP_INTF_LCD)
    {
        pstDispPubAttr->stSyncInfo.u16Vact = stPanelParam.u16Height;
        pstDispPubAttr->stSyncInfo.u16Vbb = stPanelParam.u16VSyncBackPorch;
        pstDispPubAttr->stSyncInfo.u16Vfb = stPanelParam.u16VTotal - (stPanelParam.u16VSyncWidth +
                                                                      stPanelParam.u16Height + stPanelParam.u16VSyncBackPorch);
        pstDispPubAttr->stSyncInfo.u16Hact = stPanelParam.u16Width;
        pstDispPubAttr->stSyncInfo.u16Hbb = stPanelParam.u16HSyncBackPorch;
        pstDispPubAttr->stSyncInfo.u16Hfb = stPanelParam.u16HTotal - (stPanelParam.u16HSyncWidth +
                                                                      stPanelParam.u16Width + stPanelParam.u16HSyncBackPorch);
        pstDispPubAttr->stSyncInfo.u16Bvact = 0;
        pstDispPubAttr->stSyncInfo.u16Bvbb = 0;
        pstDispPubAttr->stSyncInfo.u16Bvfb = 0;
        pstDispPubAttr->stSyncInfo.u16Hpw = stPanelParam.u16HSyncWidth;
        pstDispPubAttr->stSyncInfo.u16Vpw = stPanelParam.u16VSyncWidth;
        pstDispPubAttr->stSyncInfo.u32FrameRate = stPanelParam.u16DCLK * 1000000 / (stPanelParam.u16HTotal * stPanelParam.u16VTotal);
        pstDispPubAttr->eIntfSync = E_MI_DISP_OUTPUT_USER;
        pstDispPubAttr->eIntfType = E_MI_DISP_INTF_LCD;
        eLinkType = E_MI_PNL_LINK_TTL;
        stInputPortAttr.u16SrcWidth = stPanelParam.u16Width;
        stInputPortAttr.u16SrcHeight = stPanelParam.u16Height;
        stInputPortAttr.stDispWin.u16X = 0;
        stInputPortAttr.stDispWin.u16Y = 0;
        stInputPortAttr.stDispWin.u16Width = stPanelParam.u16Width;
        stInputPortAttr.stDispWin.u16Height = stPanelParam.u16Height;
    }

    MI_DISP_SetPubAttr(0, pstDispPubAttr);
    MI_DISP_Enable(0);
    MI_DISP_BindVideoLayer(0, 0);
    MI_DISP_EnableVideoLayer(0);

    MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr);
    MI_DISP_EnableInputPort(0, 0);
    MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN);

    if (pstDispPubAttr->eIntfType == E_MI_DISP_INTF_LCD)
    {
        MI_PANEL_Init(eLinkType);
        MI_PANEL_SetPanelParam(&stPanelParam);
        if(eLinkType == E_MI_PNL_LINK_MIPI_DSI)
        {
            //MI_PANEL_SetMipiDsiConfig(&stMipiDsiConfig);
        }
    }
    return 0;
}
#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

