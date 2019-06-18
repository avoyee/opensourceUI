/*
 *   This file is part of MiniGUI, a mature cross-platform windowing
 *   and Graphics User Interface (GUI) support system for embedded systems
 *   and smart IoT devices.
 *
 *   Copyright (C) 2002~2018, Beijing FMSoft Technologies Co., Ltd.
 *   Copyright (C) 1998~2002, WEI Yongming
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Or,
 *
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 *
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 *
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdarg.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch/mgncs4touch.h"

#include "sstardisp.h"

#include <time.h>

#if defined(__cplusplus) || defined(c_plusplus)
extern "C"
{
#endif

    clock_t start, end;

    static int GMAINWND_W = 0;
    static int GMAINWND_H = 0;
#define MAINWND_W GMAINWND_W
#define MAINWND_H GMAINWND_H

#define HEADER_LOGO_HEIGHT 50
#define HEADER_LOGO_WIDTH 200

    BITMAP *bmp = NULL;

    static char _VARG_FENCE;
    static char *_PVARG_FENCE = &_VARG_FENCE;
#define VARG_FENCE _PVARG_FENCE
    typedef struct tagDspItem
    {
        BOOL cdpath;
        char path[PATH_MAX + 1];
        char name[NAME_MAX + 1];
        char layer[LEN_LAYER_NAME + 1];
        char bmp_path[PATH_MAX + NAME_MAX + 1];
        mImagePiece *imagepiece;
        BITMAP bmp;
    } DSPITEM;

    typedef struct _ICON_INFO
    {
        int nr_apps;
        int focus;
        DSPITEM *app_items;
    } ICON_INFO;

    static ICON_INFO icon_info;

    static PLOGFONT createLogFont(unsigned size)
    {
        return CreateLogFont("ttf", "helvetica", "UTF-8",
                             FONT_WEIGHT_BOOK, FONT_SLANT_ROMAN,
                             FONT_SETWIDTH_NORMAL, FONT_OTHER_AUTOSCALE,
                             FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
                             size, 0);
    }

    static BOOL onkeydown(mWidget *widget, int message, int code, DWORD keyStatus)
    {
        mHScrollViewPiece *view = (mHScrollViewPiece *)((mContainerCtrl *)widget)->body;
        RECT rc;
        int offset_x = 0;
        int offset_y = 0;

        _c(view)->getViewport(view, &rc);

        switch (code)
        {
        case SCANCODE_J:
            offset_y = 10;
            break;

        case SCANCODE_K:
            offset_y = -10;
            break;

        case SCANCODE_L:
            offset_x = 10;
            break;

        case SCANCODE_H:
            offset_x = -10;
            break;

        case SCANCODE_S:
        {
            static BOOL show = TRUE;
            _c(view)->showScrollBar(view, show);
            show = !show;
        }
        break;

        default:
            return FALSE;
        }

        _c(view)->moveViewport(view, rc.left + offset_x, rc.top + offset_y);
        return TRUE;
    }

    static NCS_EVENT_HANDLER g_handles[] = {
        {MSG_KEYDOWN, onkeydown},
        {0, NULL}};
    static char *mk_time(char *buff)
    {
        time_t timer;
        struct tm *tblock;
        timer = time(NULL);
        tblock = localtime(&timer);
        sprintf(buff, "%s", asctime(tblock));
        return buff;
    }

    static BOOL onTimer(mWidget *widget, int message, int id, DWORD tick)
    {
        mTextPiece *textPiece;
        mPanelPiece *panelPiece = (mPanelPiece *)((mContainerCtrl *)widget)->body;
        char *b = NULL;

        //SetDlgItemText (widget->hwnd, 0, mk_time(buff));
        if (panelPiece)
            textPiece = (mTextPiece *)_c(panelPiece)->childAt(panelPiece, 0, 0);

        if (textPiece)
        {
            time_t timer;
            struct tm *tblock;
            timer = time(NULL);
            tblock = localtime(&timer);
            _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_LABEL, (DWORD)asctime(tblock));
            b = (char *)_c(textPiece)->getProperty(textPiece, NCSP_LABELPIECE_LABEL);
            InvalidateRect(widget->hwnd, NULL, TRUE);
        }
        return TRUE;
    }

    static NCS_EVENT_HANDLER g_timerhandles[] = {
        {MSG_TIMER, onTimer},
        {0, NULL}};
    pid_t exec_app(const char *file_name, char *app_name, ...)
    {
        pid_t pid = 0;
        const unsigned short u8ParamLimit = 10;
        unsigned short u8ParamNum = 0;
        char *argv[u8ParamLimit];

        va_list argp;
        memset(argv, 0, sizeof(argv));
        argv[u8ParamNum++] = app_name;
        va_start(argp, app_name);

        do
        {
            argv[u8ParamNum] = va_arg(argp, typeof(char *));
        } while (argv[u8ParamNum++] != VARG_FENCE);

        u8ParamNum--;
        va_end(argp);

        if ((pid = vfork()) > 0)
        {
            fprintf(stderr, "new child, pid: %d.\n", pid);
        }
        else if (pid == 0)
        {
            printf("%s %d %s\n", __FUNCTION__, __LINE__, file_name);

            if (execv(file_name, argv) < 0)
                fprintf(stderr, "execv error\n");

            perror("execv");
            _exit(1);
        }
        else
        {
            perror("vfork");
        }

        return pid;
    }

    static int s_onMouseEvent(mHotPiece *_self, int message, WPARAM wParam, LPARAM lParam, mObject *owner)
    {
        printf("%s(message=%d)\n", __FUNCTION__, message);

        switch (message)
        {
        case MSG_LBUTTONDOWN:
        {

            RECT rc;
            WORD x, y;
            int i;
            DSPITEM *item;
            char buff[PATH_MAX + NAME_MAX + 1];
            char resolution[4][8];
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            mPanelPiece *panelPiece = (mPanelPiece *)_self;
            mImagePiece *imagePiece = (mImagePiece *)_c(panelPiece)->childAt(panelPiece, x, y);

            for (i = 0; i < icon_info.nr_apps; i++)
            {
                if (icon_info.app_items[i].imagepiece == imagePiece)
                {
                    break;
                }
            }

            if (i == icon_info.nr_apps)
            {
                printf("cann't find imagepiece\n");
                return -1;
            }

            item = icon_info.app_items + i;

            if (item->cdpath)
            {
                chdir(item->path);
            }

            strcpy(buff, item->path);
            strcat(buff, item->name);

            memset(resolution, 0, 2 * 8 * sizeof(char));

            sprintf(resolution[0], "%d", 0);
            sprintf(resolution[1], "%d", HEADER_LOGO_HEIGHT);
            sprintf(resolution[2], "%d", MAINWND_W);
            sprintf(resolution[3], "%d", MAINWND_H - HEADER_LOGO_HEIGHT);
            exec_app(buff, item->name, resolution[0], resolution[1], resolution[2], resolution[3], "-layer", VARG_FENCE);
            break;
        }
        break;

        default:
        {
        }
        }

        return -1;
    }

    static void naviBar_freeItems(void)
    {
        int i;
        DSPITEM *item;

        item = icon_info.app_items;

        for (i = 0; i < icon_info.nr_apps; i++, item++)
        {
            if (item->bmp.bmBits)
            {
                UnloadBitmap(&item->bmp);
                item->bmp.bmBits = NULL;
            }
        }

        free(icon_info.app_items);
        icon_info.app_items = NULL;
    }

    static BOOL naviBar_getItems(void)
    {
#define APP_INFO_FILE "hscrollview.rc"
        int i;
        DSPITEM *item;
        char section[10];
        SIZE size;

        if (GetIntValueFromEtcFile(APP_INFO_FILE, "navibar", "app_nr", &icon_info.nr_apps) != ETC_OK)
            return FALSE;

        if (icon_info.nr_apps <= 0)
            return FALSE;

        if ((icon_info.app_items = (DSPITEM *)calloc(icon_info.nr_apps, sizeof(DSPITEM))) == NULL)
        {
            return FALSE;
        }

        item = icon_info.app_items;

        for (i = 0; i < icon_info.nr_apps; i++, item++)
        {
            sprintf(section, "navi-app%d", i);

            if (GetValueFromEtcFile(APP_INFO_FILE, section, "path", item->path, PATH_MAX) != ETC_OK)
                goto error;

            if (GetValueFromEtcFile(APP_INFO_FILE, section, "name", item->name, NAME_MAX) != ETC_OK)
                goto error;

            if (GetValueFromEtcFile(APP_INFO_FILE, section, "layer", item->layer, LEN_LAYER_NAME) != ETC_OK)
                goto error;

            if (GetValueFromEtcFile(APP_INFO_FILE, section, "icon", item->bmp_path, PATH_MAX + NAME_MAX) != ETC_OK)
                goto error;

            if (LoadBitmap(HDC_SCREEN, &item->bmp, item->bmp_path) != ERR_BMP_OK)
            {
                fprintf(stderr, "desktop load resource:%s error. \n", item->bmp_path);
                goto error;
            }

            item->cdpath = TRUE;
            //GetTextExtent(HDC_SCREEN, item->name, -1, &size);
        }

        return TRUE;
    error:
        naviBar_freeItems();
        return FALSE;
    }

    static BOOL _mymain_onCreate_naviBar(mMainWnd *self, DWORD dwAddData)
    {

#define ITEM_INTERVAL 25
        mContainerCtrl *ctnr;
        mPanelPiece *content;
        mHScrollViewPiece *view;
        mHotPiece *backPiece;
        int i, shiftx;
        RECT rc;
        int barW = 0;
        int barH = 0;
        int itemH = 0;
        int naviBarItemNum;
        clock_t start1 = clock();
        naviBar_getItems();
        clock_t end1 = clock();

        printf("time spent on load navi icons = %f(s)\n", (float)(end1 - start1) / CLOCKS_PER_SEC);
        naviBarItemNum = icon_info.nr_apps;

        GetClientRect(self->hwnd, &rc);

        barW = RECTW(rc);
        barH = RECTH(rc) >> 2;
        ctnr = (mContainerCtrl *)ncsCreateWindow(NCSCTRL_CONTAINERCTRL,
                                                 "navibar",
                                                 WS_VISIBLE, 0, 0,
                                                 0, (RECTH(rc) - barH), RECTW(rc), RECTH(rc) >> 2,
                                                 self->hwnd,
                                                 NULL, NULL, NULL, 0);

        content = (mPanelPiece *)NEWPIECE(mPanelPiece);
        _c(content)->appendEventHandler(content, MSG_LBUTTONDOWN, s_onMouseEvent);
        _c(content)->appendEventHandler(content, MSG_LBUTTONUP, s_onMouseEvent);
        _c(content)->appendEventHandler(content, MSG_MOUSEMOVE, s_onMouseEvent);
        _c(content)->appendEventHandler(content, MSG_MOUSEMOVEIN, s_onMouseEvent);

        view = NEWPIECE(mHScrollViewPiece);
        GetClientRect(ctnr->hwnd, &rc);
        _c(view)->setRect(view, &rc);
        _c(view)->addContent(view, (mHotPiece *)content, 0, 0);

        _c(ctnr)->setBody(ctnr, (mHotPiece *)view);

        /* add a rect piece as background.*/
        backPiece = (mHotPiece *)NEWPIECE(mRectPiece);
        GetClientRect(ctnr->hwnd, &rc);
        itemH = (barH >> 1);
        shiftx = rc.left + (itemH + ITEM_INTERVAL) * naviBarItemNum + ITEM_INTERVAL;

        if (shiftx < rc.right)
        {
            shiftx = (rc.right - shiftx) / 2;
        }
        else
        {
            rc.right = shiftx;

            shiftx = 0;
        }

        _c(content)->setRect(content, &rc);
        _c(backPiece)->setRect(backPiece, &rc);
        _c(backPiece)->setProperty(backPiece, NCSP_RECTPIECE_FILLCOLOR, MakeRGBA(0xf0, 0xf0, 0xf0, 0xff));

        _c(content)->addContent(content, backPiece, 0, 0);

        rc.left = rc.top = 0;

        for (i = 0; i < naviBarItemNum; i++)
        {
            mHotPiece *imagePiece = (mHotPiece *)NEWPIECE(mImagePiece);
            mHotPiece *textPiece = (mHotPiece *)NEWPIECE(mTextPiece);

            rc.right = itemH;
            rc.bottom = itemH;

            _c(imagePiece)->setRect(imagePiece, &rc);
            _c(imagePiece)->setProperty(imagePiece, NCSP_IMAGEPIECE_IMAGE, (DWORD)&icon_info.app_items[i].bmp);
            _c(imagePiece)->setProperty(imagePiece, NCSP_IMAGEPIECE_DRAWMODE, NCS_DM_SCALED);
            _c(content)->addContent(content, imagePiece, ITEM_INTERVAL + (itemH + ITEM_INTERVAL) * i + shiftx, 10);
            icon_info.app_items[i].imagepiece = (mImagePiece *)imagePiece;

            rc.bottom = 30;
            rc.right = itemH;
            _c(textPiece)->setRect(textPiece, &rc);
            _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_LABEL, (DWORD)icon_info.app_items[i].name);
            _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_TEXTCOLOR, (DWORD)0xFF0000FF);
            _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_LOGFONT, (DWORD)createLogFont(15));
            _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_AUTOWRAP, (DWORD)TRUE);
            _c(content)->addContent(content, textPiece, ITEM_INTERVAL + (itemH + ITEM_INTERVAL) * i + shiftx, itemH + ITEM_INTERVAL);
        }

        ncsSetComponentHandlers((mComponent *)ctnr, g_handles, -1);
        //free(bmp);
        return TRUE;
    }

    static BOOL _mymain_onCreate_Header(mMainWnd *self, DWORD dwAddData)
    {

#define HEADER_LOGO_MARGIN 2
        mContainerCtrl *ctnr;
        mPanelPiece *view;
        mHotPiece *backPiece;
        RECT rc;
        static BITMAP logo;

        LoadBitmap(HDC_SCREEN, &logo, "res/sigmalogo.jpg");
        GetClientRect(self->hwnd, &rc);

        ctnr = (mContainerCtrl *)ncsCreateWindow(NCSCTRL_CONTAINERCTRL,
                                                 "logo",
                                                 WS_VISIBLE, 0, 0,
                                                 0, 0, RECTW(rc), HEADER_LOGO_HEIGHT,
                                                 self->hwnd,
                                                 NULL, NULL, NULL, 0);

        view = (mPanelPiece *)NEWPIECE(mPanelPiece);
        _c(ctnr)->setBody(ctnr, (mHotPiece *)view);

        /* add a rect piece as background.*/
        backPiece = (mHotPiece *)NEWPIECE(mRectPiece);
        GetClientRect(ctnr->hwnd, &rc);
        _c(view)->setRect(view, &rc);
        _c(backPiece)->setRect(backPiece, &rc);
        _c(backPiece)->setProperty(backPiece, NCSP_RECTPIECE_FILLCOLOR, MakeRGBA(0xff, 0xff, 0xff, 0xff));

        _c(view)->addContent(view, backPiece, 0, 0);

        /* add 18 imagePiece */
        rc.left = rc.top = 0;
        rc.right = HEADER_LOGO_WIDTH;
        rc.bottom = HEADER_LOGO_HEIGHT - 2 * HEADER_LOGO_MARGIN;

        mHotPiece *imagePiece = (mHotPiece *)NEWPIECE(mImagePiece);

        _c(imagePiece)->setRect(imagePiece, &rc);
        _c(imagePiece)->setProperty(imagePiece, NCSP_IMAGEPIECE_IMAGE, (DWORD)&logo);
        _c(imagePiece)->setProperty(imagePiece, NCSP_IMAGEPIECE_DRAWMODE, NCS_DM_SCALED);
        _c(view)->addContent(view, imagePiece, HEADER_LOGO_MARGIN, HEADER_LOGO_MARGIN);
        return TRUE;
    }

    static BOOL _mymain_onCreateBriefInfo(mMainWnd *self, DWORD dwAddData)
    {
#define BRIEF_TMP_HEIGHT 60
        mContainerCtrl *ctnr;
        mPanelPiece *view;
        mHotPiece *backPiece;
        RECT rc;
        static BITMAP weather;
        static const char *tmp = "sunny \n 15 ~ 25 *C";

        LoadBitmap(HDC_SCREEN, &weather, "res/cloud_1.png");
        GetClientRect(self->hwnd, &rc);

        ctnr = (mContainerCtrl *)ncsCreateWindow(NCSCTRL_CONTAINERCTRL,
                                                 "logo",
                                                 WS_VISIBLE, 0, 0,
                                                 0, HEADER_LOGO_HEIGHT, RECTW(rc) / 2, RECTH(rc) - HEADER_LOGO_HEIGHT - (RECTH(rc) >> 2),
                                                 self->hwnd,
                                                 NULL, NULL, NULL, 0);

        view = (mPanelPiece *)NEWPIECE(mPanelPiece);
        _c(ctnr)->setBody(ctnr, (mHotPiece *)view);

        /* add a rect piece as background.*/
        backPiece = (mHotPiece *)NEWPIECE(mRectPiece);
        GetClientRect(ctnr->hwnd, &rc);
        _c(view)->setRect(view, &rc);
        _c(backPiece)->setRect(backPiece, &rc);
        _c(backPiece)->setProperty(backPiece, NCSP_RECTPIECE_FILLCOLOR, MakeRGBA(0xff, 0xff, 0xff, 0xff));

        _c(view)->addContent(view, backPiece, 0, 0);

        /* add 18 imagePiece */
        rc.left = rc.top = 2;
        rc.right = RECTW(rc) / 2;
        rc.bottom = (float)rc.right * weather.bmHeight / weather.bmWidth;

        mHotPiece *imagePiece = (mHotPiece *)NEWPIECE(mImagePiece);

        _c(imagePiece)->setRect(imagePiece, &rc);
        _c(imagePiece)->setProperty(imagePiece, NCSP_IMAGEPIECE_IMAGE, (DWORD)&weather);
        _c(imagePiece)->setProperty(imagePiece, NCSP_IMAGEPIECE_DRAWMODE, NCS_DM_SCALED);
        _c(view)->addContent(view, imagePiece, HEADER_LOGO_MARGIN, HEADER_LOGO_MARGIN);

        _c(imagePiece)->getRect(imagePiece, &rc);
        mHotPiece *textPiece = (mHotPiece *)NEWPIECE(mTextPiece);

        _c(view)->addContent(view, textPiece, rc.right + 2, RECTH(rc) / 2 - BRIEF_TMP_HEIGHT / 2);
        rc.left = rc.top = 2;
        rc.right = rc.right - 2; //RECTW(rc) / 2 -4;
        rc.bottom = BRIEF_TMP_HEIGHT;
        _c(textPiece)->setRect(textPiece, &rc);
        _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_LABEL, (DWORD)tmp);
        _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_TEXTCOLOR, (DWORD)0xFF0000FF);
        _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_TEXTSHADOWCOLOR, (DWORD)0xFF0000DD);
        _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_LOGFONT, (DWORD)createLogFont(30));
        _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_AUTOWRAP, (DWORD)TRUE);

        return TRUE;
    }

    static BOOL _mymain_onCreateTime(mMainWnd *self, DWORD dwAddData)
    {

        mContainerCtrl *ctnr;
        mTextPiece *textPiece;
        RECT rc;

        GetClientRect(self->hwnd, &rc);
        ctnr = (mContainerCtrl *)ncsCreateWindow(NCSCTRL_CONTAINERCTRL,
                                                 "time",
                                                 WS_VISIBLE, 0, 0,
                                                 HEADER_LOGO_WIDTH, 2, RECTW(rc) - HEADER_LOGO_WIDTH, HEADER_LOGO_HEIGHT - 2 * HEADER_LOGO_MARGIN,
                                                 self->hwnd,
                                                 NULL, NULL, NULL, 0);

        mPanelPiece *view = (mPanelPiece *)NEWPIECE(mPanelPiece);
        textPiece = (mTextPiece *)NEWPIECE(mTextPiece);
        _c(ctnr)->setBody(ctnr, (mHotPiece *)view);

        rc.top = rc.left = 0;
        rc.right = RECTW(rc) - HEADER_LOGO_WIDTH;
        rc.bottom = HEADER_LOGO_HEIGHT - 2 * HEADER_LOGO_MARGIN;
        _c(textPiece)->setRect(textPiece, &rc);
        _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_LABEL, (DWORD) "--:--:--");
        _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_TEXTCOLOR, (DWORD)0xFF0000FF);
        _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_ALIGN, (DWORD)(NCS_ALIGN_RIGHT));
        _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_TEXTSHADOWCOLOR, (DWORD)0xFF0000DD);
        _c(textPiece)->setProperty(textPiece, NCSP_TEXTPIECE_LOGFONT, (DWORD)createLogFont(30));
        _c(textPiece)->setProperty(textPiece, NCSP_LABELPIECE_AUTOWRAP, (DWORD)TRUE);

        _c(view)->setRect(view, &rc);
        _c(view)->addContent(view, textPiece, 0, 0);

        SetTimer((mWidget *)ctnr->hwnd, 100, 100);
        ncsSetComponentHandlers((mComponent *)ctnr, g_timerhandles, -1);
        return TRUE;
    }

    static BOOL mymain_onCreateCalendar(mMainWnd *self, DWORD dwAddData)
    {
        mContainerCtrl *ctnr;
        mTextPiece *textPiece;
        RECT rc;

        GetClientRect(self->hwnd, &rc);
        SetWindowFont(self->hwnd, createLogFont(30));
        ctnr = (mContainerCtrl *)ncsCreateWindow(NCSCTRL_MONTHCALENDAR,
                                                 "calendar",
                                                 WS_VISIBLE, WS_EX_USEPARENTFONT, 0,
                                                 RECTW(rc) / 2, HEADER_LOGO_HEIGHT, RECTW(rc) / 2, RECTH(rc) - HEADER_LOGO_HEIGHT - (RECTH(rc) >> 2),
                                                 self->hwnd,
                                                 NULL, NULL, NULL, 0);
        return TRUE;
    }

    static BOOL mymain_onCreate(mMainWnd *self, DWORD dwAddData)
    {
        BOOL ret = FALSE;
        ret = _mymain_onCreate_naviBar(self, dwAddData);
        ret = _mymain_onCreate_Header(self, dwAddData);
        ret = _mymain_onCreateBriefInfo(self, dwAddData);
        ret = _mymain_onCreateTime(self, dwAddData);
        ret = mymain_onCreateCalendar(self, dwAddData);
        return ret;
    }

    static BOOL mymain_onClose(mWidget *self, int message)
    {
        free(bmp);
        DestroyMainWindow(self->hwnd);
        return TRUE;
    }

    static NCS_EVENT_HANDLER mymain_handlers[] = {
        {MSG_CREATE, mymain_onCreate},
        {MSG_CLOSE, mymain_onClose},
        {MSG_LBUTTONDOWN, SpeedMeterMessageHandler},
        {MSG_LBUTTONUP, SpeedMeterMessageHandler},
        {MSG_MOUSEMOVE, SpeedMeterMessageHandler},
        {0, NULL}};

#include <time.h>
    int MiniGUIMain(int argc, const char *argv[])
    {
        MSG Msg;
        mWidget *mymain;
        RECT rc;
        int idlecnt = 1;
        start = clock();

        if (argc < 3)
        {
            printf("usage: <width> <height>\n");
            return -1;
        }
        else
        {
            GMAINWND_W = atoi(argv[1]);
            GMAINWND_H = atoi(argv[2]);
        }

        if (!ServerStartup(0, 0, 0))
        {
            fprintf(stderr,
                    "Can not start the server of MiniGUI-Processes: mginit.\n");
            return 1;
        }

#ifdef _MGRM_PROCESSES
        JoinLayer(NAME_DEF_LAYER, "helloworld", 0, 0);
#endif

        ncsInitialize();
        ncs4TouchInitialize();

        mymain = ncsCreateMainWindow(NCSCTRL_MAINWND, "home",
                                     WS_NONE | WS_VISIBLE,
                                     WS_EX_AUTOSECONDARYDC,
                                     1,
                                     0, 0, MAINWND_W, MAINWND_H,
                                     HWND_DESKTOP,
                                     0, 0,
                                     NULL,
                                     NULL,
                                     mymain_handlers,
                                     0);

        mGEffInit();

        while (GetMessage(&Msg, mymain->hwnd))
        {
            if (idlecnt && Msg.message == MSG_IDLE)
            {
                idlecnt++;

                if (idlecnt > 2)
                {
                    end = clock();

                    printf("time spent on minigui show = %f(s)\n", (float)(end - start) / CLOCKS_PER_SEC);
                    idlecnt = 0;
                }
            }

            TranslateMessage(&Msg);

            DispatchMessage(&Msg);
        }

        MainWindowThreadCleanup(mymain->hwnd);

        mGEffDeinit();

        ncs4TouchUninitialize();
        ncsUninitialize();

        return 0;
    }

#ifdef _USE_MINIGUIENTRY
    int main(int argc, const char *argv[])
    {
        MI_DISP_PubAttr_t stDispPubAttr;
        start = clock();
        stDispPubAttr.eIntfType = E_MI_DISP_INTF_LCD;
        stDispPubAttr.eIntfSync = E_MI_DISP_OUTPUT_USER;
        sstar_disp_init(&stDispPubAttr);
        end = clock();
        printf("time spent on disp init = %f(s)\n", (float)(end - start) / CLOCKS_PER_SEC);
        main_entry(argc, argv);
        return 0;
    }
#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif
