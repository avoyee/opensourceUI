#include <stdio.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

static int HelloWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
        HDC hdc;
        switch (message) {
                case MSG_PAINT:
                hdc = BeginPaint (hWnd);
                TextOut (hdc, 60, 60, "Hello world!");
                EndPaint (hWnd, hdc);
                return 0;

                case MSG_CLOSE:
                DestroyMainWindow (hWnd);
                PostQuitMessage (hWnd);
                return 0;
        }

        return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

#endif
#include "sstardisp.h"
#include <signal.h>
MI_DISP_PubAttr_t stDispPubAttr;
int MiniGUIMain (int argc, const char* argv[])
{
        MSG Msg;
        HWND hMainWnd;
        MAINWINCREATE CreateInfo;

        #ifdef _MGRM_PROCESSES
    if(!ServerStartup(0, 0, 0)) {
        fprintf(stderr,
                "Can not start the server of MiniGUI-Processes: mginit.\n");
        return 1;
    }
        JoinLayer (NAME_DEF_LAYER , "helloworld" , 0 , 0);
        #endif

        CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
        CreateInfo.dwExStyle = WS_EX_NONE;
        CreateInfo.spCaption = "HelloWorld";
        CreateInfo.hMenu = 0;
        CreateInfo.hCursor = GetSystemCursor (0);
        CreateInfo.hIcon = 0;
        CreateInfo.MainWindowProc = HelloWinProc;
        CreateInfo.lx = 0;
        CreateInfo.ty = 0;
        CreateInfo.rx = 240;
        CreateInfo.by = 180;
        CreateInfo.iBkColor = COLOR_lightwhite;
        CreateInfo.dwAddData = 0;
        CreateInfo.hHosting = HWND_DESKTOP;

        hMainWnd = CreateMainWindow (&CreateInfo);

        if (hMainWnd == HWND_INVALID)
        return -1;

        ShowWindow (hMainWnd, SW_SHOWNORMAL);

        while (GetMessage (&Msg, hMainWnd)) {
                TranslateMessage (&Msg);
                DispatchMessage (&Msg);
        }

        MainWindowThreadCleanup (hMainWnd);
    sstar_disp_Deinit(&stDispPubAttr);
        return 0;
}


#ifdef _USE_MINIGUIENTRY
int main(int argc, const char* argv[])
{
    stDispPubAttr.eIntfType = E_MI_DISP_INTF_LCD;
    stDispPubAttr.eIntfSync = E_MI_DISP_OUTPUT_USER;
    sstar_disp_init(&stDispPubAttr);
    main_entry(argc, argv);
    raise(SIGINT);
    return 0;

}
#endif

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif