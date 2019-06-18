#ifndef __SSTARDISP__H__
#define __SSTARDISP__H__

#include <mi_disp_datatype.h>
#include <mi_hdmi_datatype.h>
typedef struct stTimingArray_s
{
    char desc[50];
    MI_DISP_OutputTiming_e eOutputTiming;
    MI_HDMI_TimingType_e eHdmiTiming;
    MI_U16 u16Width;
    MI_U16 u16Height;
}stTimingArray_t;

int sstar_disp_init(MI_DISP_PubAttr_t* pstDispPubAttr);
#endif
