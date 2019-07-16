#include <stdio.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include "../include/mgi.h"

#define IDC_CHARS1       100
#define IDC_CHARS2       101 
#define IDC_CHAR1         102
#define IDC_CHAR2         103
#define IDC_CHAR3         104

#define IDC_GB2312          111
#define IDC_UTF8            112
#define IDC_AUTOTRACK       114
#define IDC_NOTRACK         115

#define IDC_LATIN       117
#define IDC_ZHCN        118

#define EDIT_NUM        5
static unsigned int EDITS_ID[EDIT_NUM] ={IDC_CHARS1, IDC_CHARS2,IDC_CHAR1, IDC_CHAR2, IDC_CHAR3}; 

#define dbg() printf("%s %d\n", __FUNCTION__, __LINE__);
static PLOGFONT big5_font;
static PLOGFONT utf8_font;
static PLOGFONT gb2312_font;

void notify_ime_status(BOOL);

#ifndef _MGRM_PROCESSES
static HWND sk_ime_hwnd;
#endif 
static HWND g_current_hwnd; 
static BOOL g_ime_opened = FALSE; 
static CTRLDATA CtrlInputChar [] =
{ 
    {
        CTRL_STATIC,
        WS_VISIBLE,
        10, 10, 380, 25, 
        IDC_STATIC,
        "EDITABLE",
        0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE,
        10, 50, 380, 25, 
        IDC_STATIC,
        "PASSWORD",
        0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE,
        10, 90, 380, 25, 
        IDC_STATIC,
        "READ ONLY",
        0
    }, 


    {
        CTRL_SLEDIT,
        WS_VISIBLE | WS_TABSTOP | WS_BORDER,
        120, 10, 180, 25,
        IDC_CHAR1,
        NULL,
        0
    },
    {
        CTRL_SLEDIT,
        WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_PASSWORD,
        120, 50, 180, 25,
        IDC_CHAR2,
        NULL,
        0
    },
    {
        CTRL_SLEDIT,
        WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_READONLY,
        120, 90, 180, 25,
        IDC_CHAR3,
        NULL,
        0
    },

    {
        CTRL_MLEDIT,
        WS_VISIBLE | WS_VSCROLL 
            | ES_BASELINE | ES_AUTOWRAP | ES_NOHIDESEL | ES_NOHIDESEL,//| WS_DISABLED,
        0, 150, 155, 200, 
        IDC_CHARS1, 
        NULL,
        0 
    },
    {
        CTRL_MLEDIT,
        WS_VISIBLE | WS_VSCROLL 
            | ES_BASELINE | ES_AUTOWRAP | ES_NOHIDESEL | ES_NOHIDESEL,//| WS_DISABLED,
        165, 150, 155, 200, 
        IDC_CHARS2, 
        NULL,
        0 
    },

    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CHECKED | WS_TABSTOP | WS_GROUP, 
        20, 380, 80, 34,
        IDC_GB2312,
        "GB2312",
        0
    },
    {
        CTRL_BUTTON,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        20, 420, 80, 34,
        IDC_UTF8,
        "UTF8",
        0
    },

    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CHECKED | WS_TABSTOP | WS_GROUP, 
        120, 380, 80, 34,
        IDC_LATIN,
        "ENG",
        0
    },
    {
        CTRL_BUTTON,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        120, 420, 80, 34,
        IDC_ZHCN,
        "ZHCN",
        0
    },

    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CHECKED | WS_TABSTOP | WS_GROUP, 
        200, 380, 90, 34,
        IDC_AUTOTRACK,
        "AUTOTRACK",
        0
    },

    {
        CTRL_BUTTON,
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        200, 420, 90, 34,
        IDC_NOTRACK,
        "NOTRACK",
        0
    },
}; 

static DLGTEMPLATE DlgBoxInputChar =
{
    WS_CAPTION, 
    WS_EX_NONE,
    0, 0, 320, 500, 
    "",
    0, 0,
    TABLESIZE(CtrlInputChar), NULL,
    0
}; 


static void btn_notif_proc (HWND hwnd, int id, int nc, DWORD add_data) 
{ 
    int i = 0;
    if (id == IDC_GB2312 && nc == BN_CLICKED) { 
        SetIMEStatus(IME_STATUS_ENCODING, IME_ENCODING_LOCAL); 
        printf("hwnd = %d\n", hwnd);
        for(i = 0; i < EDIT_NUM; ++i) {
            SetWindowFont(GetDlgItem(GetParent(hwnd), EDITS_ID[i]),gb2312_font);
        }
    }  else if (id == IDC_UTF8 && nc == BN_CLICKED) { 
        SetIMEStatus(IME_STATUS_ENCODING, IME_ENCODING_UTF8); 
        for(i = 0; i < EDIT_NUM; ++i) {
            SetWindowFont(GetDlgItem(GetParent(hwnd), EDITS_ID[i]), utf8_font);
        }
    } else if (id == IDC_AUTOTRACK && nc == BN_CLICKED) { 
        SetIMEStatus(IME_STATUS_AUTOTRACK, TRUE); 
    } else if (id == IDC_NOTRACK && nc == BN_CLICKED) { 
        SetIMEStatus(IME_STATUS_AUTOTRACK, FALSE); 
    } else if (id == IDC_ZHCN && nc == BN_CLICKED) { 
        SetIMEStatus(IME_STATUS_LANGUAGE, IME_LANGUAGE_ZHCN); 
    } else if (id == IDC_LATIN && nc == BN_CLICKED) { 
        SetIMEStatus(IME_STATUS_LANGUAGE, IME_LANGUAGE_LATIN); 
    } 
}       

static void edit_notif_proc(HWND hDlg, int id, int nc, DWORD add_data)
{
    IME_TARGET_INFO ime_t_info;
    //edit控件的消息处理函数中打印caret的位置
    GetIMETargetInfo(&ime_t_info);
    printf("ime_t_info.ptCaret: (%d, %d)\n", ime_t_info.ptCaret.x, ime_t_info.ptCaret.y);
}

static int InputCharDialogBoxProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    g_current_hwnd = hDlg;
    switch (message) {
    case MSG_INITDIALOG:
        SetNotificationCallback (GetDlgItem (hDlg, IDC_GB2312), btn_notif_proc);
        SetNotificationCallback (GetDlgItem (hDlg, IDC_UTF8), btn_notif_proc);
        SetNotificationCallback (GetDlgItem (hDlg, IDC_AUTOTRACK), btn_notif_proc);
        SetNotificationCallback (GetDlgItem (hDlg, IDC_NOTRACK), btn_notif_proc);
        SetNotificationCallback (GetDlgItem (hDlg, IDC_LATIN), btn_notif_proc);
        SetNotificationCallback (GetDlgItem (hDlg, IDC_ZHCN), btn_notif_proc);

        SetNotificationCallback (GetDlgItem (hDlg, IDC_CHAR1), edit_notif_proc);
        SetNotificationCallback (GetDlgItem (hDlg, IDC_CHARS1), edit_notif_proc);

        utf8_font = CreateLogFontByName("upf-fmsong-rrncnn-12-12-UTF-8");
        gb2312_font = CreateLogFontByName("upf-fmsong-rrncnn-12-12-GB2312");
        big5_font = CreateLogFontByName("upf-fmsong-rrncnn-12-12-BIG5");
        if(NULL == utf8_font->mbc_devfont) {
            printf("utf8_font CreateLogFontByName Failed!\n");
        }

        if(NULL == gb2312_font->mbc_devfont) {
            printf("gb2312_font CreateLogFontByName Failed!\n");
        }

        if (NULL == big5_font->mbc_devfont) {
            printf("big5_font CreateLogFontByName Failed!\n");
        }

        int i = 0;
        for(i = 0; i < EDIT_NUM; ++i) {
            SetWindowFont(GetDlgItem(hDlg, EDITS_ID[i]),gb2312_font);
        }

#ifndef _MGRM_PROCESSES
        sk_ime_hwnd = mgiCreateSoftKeypad(notify_ime_status); 
#endif 
        SetFocusChild(GetDlgItem(hDlg, IDC_CHAR1));
        SetIMEStatus(IME_STATUS_AUTOTRACK, TRUE); 
        SetIMEStatus(IME_STATUS_ENCODING,IME_ENCODING_LOCAL); 
        SetIMEStatus(IME_STATUS_ENABLED, TRUE);
        printf("MSG_INITDIALOG: GetIMEStatus(IME_STATUS_ENABLED): %d\n",  GetIMEStatus(IME_STATUS_ENABLED));
        return 1; 
    case MSG_CLOSE:
        DestroyLogFont(utf8_font);
        DestroyLogFont(gb2312_font);
        DestroyLogFont(big5_font); 
        PostMessage (GetParent(hDlg), MSG_CLOSE, 0, 0); 
        EndDialog (hDlg, IDCANCEL); 
        break;
    } 
    return DefaultDialogProc (hDlg, message, wParam, lParam);
}
void notify_ime_status(BOOL opened)
{
    g_ime_opened = opened;
}

int MiniGUIMain (int argc, const char* argv[])
{
#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "mgi_edit" , 0 , 0);
#endif 

#ifndef _MGRM_THREADS
    if (!InitVectorialFonts ()) {
        printf ("InitVectorialFonts: error.\n");
        return 1;
    }
#endif

    DlgBoxInputChar.controls = CtrlInputChar;
    DialogBoxIndirectParam (&DlgBoxInputChar, HWND_DESKTOP, InputCharDialogBoxProc, 0L);

#ifndef _MGRM_THREADS
    TermVectorialFonts ();
#endif
    return 0;
} 
#ifdef _USE_MINIGUIENTRY
int main(int argc, const char *argv[])
{
    main_entry(argc, argv);
    return 0;

}
#endif

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
