/*
 *   This file is part of mGNCS4Touch, a component for MiniGUI.
 *
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
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
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"


static void mTimePiece_delEventHandler(mTimePiece *self, int message)
{
    list_t *i;
    list_for_each(i, &self->eventHandlers) {
        event_handler_t *entry = (event_handler_t*)i;
        if (entry->message == message) {
            free(entry);
            list_del(i);
        }
    }
}


static int s_onTimer(mHotPiece *_self, int message, WPARAM wParam, LPARAM lParam, mObject *owner){
    mScrollViewPiece *self = (mScrollViewPiece *)_self;
    self->m_bPressed = TRUE;
    self->m_mouseFlag = 0;
    self->m_bTimedout = FALSE;
    self->m_timePressed = GetTickCount();
    self->m_oldMousePos.x = LOSWORD(lParam);
    self->m_oldMousePos.y = HISWORD(lParam);
    self->m_pressMousePos = self->m_oldMousePos;

    if (self->m_animation) {
        mGEffAnimationStop(self->m_animation);
        self->m_animation = NULL;
        self->m_mouseFlag |= 0x02;
    }else{
        KillTimer((HWND)self, (LINT)self);
        SetTimerEx((HWND)self, (LINT)self, PRESS_TIMEOUT, s_onTimer);
    }
    return 0;
}

static void mTimePiece_construct(mTimePiece *self, DWORD add_data)
{
    Class(mTextPiece).construct((mTextPiece *)self, add_data);
#ifdef _MGTIMER_UNIT_10MS
    SetTimer((HWND)self, _ID_TIMER, 100);
#else
    SetTimer((HWND)self, _ID_TIMER, 10);
	_c(self)->appendEventHandler(self, MSG_TIMER, s_onTimer);
}

static void mTimePiece_paint(mTimePiece *self, HDC hdc, mWidget *owner, DWORD add_data)
{
    return Class(mTextPiece).paint((mTextPiece *)self, hdc, owner, add_data);
}

static BOOL mTimePiece_setProperty(mTimePiece *self, int id, DWORD value)
{

    return Class(mLabelPiece).setProperty((mLabelPiece *)self, id, value);

    return TRUE;
}

static DWORD mTimePiece_getProperty(mTimePiece *self, int id)
{

    return Class(mLabelPiece).getProperty((mLabelPiece *)self, id);

}

static BOOL mTimePiece_autoSize(mTimePiece *self, mWidget *owner, const SIZE *pszMin, const SIZE *pszMax)
{
    return Class(mTextPiece).autoSize((mTextPiece *)self, owner, pszMin, pszMax);
}


BEGIN_MINI_CLASS(mTimePiece, mTextPiece)
CLASS_METHOD_MAP(mTimePiece, construct)
CLASS_METHOD_MAP(mTimePiece, paint)
CLASS_METHOD_MAP(mTimePiece, setProperty)
CLASS_METHOD_MAP(mTimePiece, getProperty)
CLASS_METHOD_MAP(mTimePiece, autoSize)

END_MINI_CLASS

