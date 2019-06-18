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
/*
** mou_TOUCHSCREEN.c: Intelligent PS/2 Mouse Driver
**
** Create Date: 2002/10/15
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "common.h"
#include "ial.h"
#include "gal.h"
#include "native.h"
#include <time.h>
#include <linux/input.h>

#ifdef _MGCONSOLE_TOUCHSCREEN

#define SCALE 1  /* default scaling factor for acceleration */
#define THRESH 5 /* default threshhold for acceleration */

static int TOUCHSCREEN_Open(const char *mdev);
static void TOUCHSCREEN_Close(void);
static int TOUCHSCREEN_GetButtonInfo(void);
static void TOUCHSCREEN_GetDefaultAccel(int *pscale, int *pthresh);
static int TOUCHSCREEN_Read(int *dx, int *dy, int *dz, int *bp);
static void TOUCHSCREEN_Suspend(void);
static int TOUCHSCREEN_Resume(void);

MOUSEDEVICE mousedev_TOUCHSCREEN = {
    TOUCHSCREEN_Open,
    TOUCHSCREEN_Close,
    TOUCHSCREEN_GetButtonInfo,
    TOUCHSCREEN_GetDefaultAccel,
    TOUCHSCREEN_Read,
    TOUCHSCREEN_Suspend,
    TOUCHSCREEN_Resume};

static int mouse_fd;

static int TOUCHSCREEN_Open(const char *mdev)
{
    mouse_fd = open(mdev, O_RDWR | O_NONBLOCK);
    if (mouse_fd < 0)
    {
        mouse_fd = open(mdev, O_RDONLY | O_NONBLOCK);
        if (mouse_fd < 0)
            return -1;
    }
    return mouse_fd;
}

static void TOUCHSCREEN_Close(void)
{
    if (mouse_fd > 0)
        close(mouse_fd);

    mouse_fd = -1;
}

static int TOUCHSCREEN_GetButtonInfo(void)
{
    return BUTTON_L | BUTTON_M | BUTTON_R | WHEEL_UP | WHEEL_DOWN;
}

static void TOUCHSCREEN_GetDefaultAccel(int *pscale, int *pthresh)
{
    *pscale = SCALE;
    *pthresh = THRESH;
}

static int TOUCHSCREEN_Read(int *dx, int *dy, int *dz, int *bp)
{
    //static int buttons[4] = {0, BUTTON_L, BUTTON_R, BUTTON_L | BUTTON_R};
    //static int nbytes;
    int x_y_ref = 0;
    int n;
    struct input_event event;

    *dz = 0;
    *bp = BUTTON_L;
    while (x_y_ref ^ 0x3)
    {
        n = read(mouse_fd, &event, sizeof(event));
        if (n > 0)
        {
            //fprintf(stderr, "%-24.24s.%06lu type 0x%04x code 0x%04x value 0x%08x \n",
            //        ctime(&event.time.tv_sec), event.time.tv_usec, event.type, event.code, event.value);
            switch (event.type)
            {
            case EV_ABS:
                switch (event.code)
                {
                case ABS_MT_POSITION_X:
                    *dx = event.value;
                    x_y_ref |= 0x1;
                    break;
                case ABS_MT_POSITION_Y:
                    *dy = event.value;
                    x_y_ref |= 0x2;
                    break;
                case ABS_X:
                    //*dx = event.value;
                    //x_y_ref |= 0x1;
                    break;
                case ABS_Y:
                    //*dy = event.value;
                    //x_y_ref |= 0x2;
                    break;
                }
                break;
            }
        }
    }
    fprintf(stderr, "x %d y %d\n", *dx, *dy);
    return 1;
}

static void TOUCHSCREEN_Suspend(void)
{
    TOUCHSCREEN_Close();
}

static int TOUCHSCREEN_Resume(void)
{
    return TOUCHSCREEN_Open(IAL_MDev);
}

#endif /* _MGCONSOLE_TOUCHSCREEN */
