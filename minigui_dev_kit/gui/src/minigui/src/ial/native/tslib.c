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
** mou_TSLIB.c: Intelligent PS/2 Mouse Driver
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <tslib.h>

#include "ial.h"


#ifdef _MGCONSOLE_TSLIB



#define SCALE 1  /* default scaling factor for acceleration */
#define THRESH 0 /* default threshhold for acceleration */
static struct tsdev *ts = NULL;
static int mousex = 0;
static int mousey = 0;
static int button = 0;


static int TSLIB_Open(const char *mdev);
static void TSLIB_Close(void);
static int TSLIB_GetButtonInfo(void);
static void TSLIB_GetDefaultAccel(int *pscale, int *pthresh);
static int TSLIB_Read(int *dx, int *dy, int *dz, int *bp);
static void TSLIB_Suspend(void);
static int TSLIB_Resume(void);

MOUSEDEVICE mousedev_TSLIB = {
    TSLIB_Open,
    TSLIB_Close,
    TSLIB_GetButtonInfo,
    TSLIB_GetDefaultAccel,
    TSLIB_Read,
    TSLIB_Suspend,
    TSLIB_Resume};


static int TSLIB_Open(const char *mdev)
{
    const char *tsdevice;
    setenv("TSLIB_TSDEVICE","/dev/input/event0",1);
    setenv("TSLIB_CONFFILE","/etc/ts.conf",1);
    if ((tsdevice = getenv("TSLIB_TSDEVICE")) == NULL)
    {
        tsdevice = mdev;
    }

    if (tsdevice == NULL)
    {
        _MG_PRINTF("IAL>TSLib: Please specify the ts device\n");
        return -1;
    }

    _MG_PRINTF("IAL>TSLib: open ts device %s\n", tsdevice);

    ts = ts_open(tsdevice, 0);

    if (!ts)
    {
        _MG_PRINTF("IAL>TSLib: can not open ts device\n");
        return -1;
    }

    _MG_PRINTF("IAL>TSLib: open ts config %s\n", getenv("TSLIB_CONFFILE"));
    if (ts_config(ts))
    {
        _MG_PRINTF("IAL>TSLib: can not config ts device\n");
        return -1;
    }
    return ts_fd(ts);
}

static void TSLIB_Close(void)
{
    if (ts)
    {
        ts_close(ts);
        ts = NULL;
    }

}

static int TSLIB_GetButtonInfo(void)
{
    return BUTTON_L;
}

static void TSLIB_GetDefaultAccel(int *pscale, int *pthresh)
{
    *pscale = SCALE;
    *pthresh = THRESH;
}

static int TSLIB_Read(int *dx, int *dy, int *dz, int *bp)
{
    struct ts_sample samp;

    if (ts_read(ts, &samp, 1) > 0)
    {
        if (samp.pressure > 0)
        {
            mousex = samp.x;
            mousey = samp.y;
        }

        button = (samp.pressure > 0) ? BUTTON_L : 0;

    }
    *dx = mousex;
    *dy = mousey;
    *bp = button;
    return TRUE;

}

static void TSLIB_Suspend(void)
{
    TSLIB_Close();
}

static int TSLIB_Resume(void)
{
    return TSLIB_Open(IAL_MDev);
}

#endif /* _MGCONSOLE_TSLIB */
