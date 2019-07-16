
/**
 * \file hw_engine.c
 * \author Qingzhou Long <qzlong@minigui.org>
 * \date 2005/12/22
 *
 * This file includes input method interfaces of mGi. 
 */

/*
 * $Id: hw_engine.c 301 2010-03-23 06:19:26Z rlzheng $
 *          mGi (MiniGUI Inputmethod) is a component of MiniGUI.
 *
 *          MiniGUI is a compact cross-platform Graphics User Interface 
 *         (GUI) support system for real-time embedded systems.
 *                  
 *             Copyright (C) 2002-2005 Feynman Software.
 *             Copyright (C) 1998-2002 Wei Yongming.
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifdef HW_CHINESES
#include <minigui/common.h>
#include <zinnia.h>
#endif

#include "../include/mgi.h"

/* null engine */
static int null_recognize(WORD* pTrace,char* pResult,
                    int *nCands,int nPoints, WORD wRange)
{
    memcpy (pResult, "a\0b\0c\0d\0e\0f\0g\0h\0", 16);
    if (nCands)
        *nCands = 8;

    return 0;
}

static int null_init(int w, int h)
{
    return 0;
}

static int null_exit(void)
{
    return 0;
}

MGI_HW_ENGINE hw_null_engine = {
    null_init,
    null_recognize,
    null_exit
};

#ifdef HW_CHINESES
static struct zinnia_engine {
    zinnia_recognizer_t *m_recognizer;
    zinnia_character_t *m_character;
    zinnia_result_t *m_result;
}s_zinnia_engine;

static int zinnia_init(int w, int h)
{
    s_zinnia_engine.m_recognizer = zinnia_recognizer_new();
    if (!zinnia_recognizer_open(s_zinnia_engine.m_recognizer,
            "/usr/local/lib/zinnia/model/tomoe/handwriting-zh_CN.model")) {
        fprintf(stderr, "ERROR: %s\n", zinnia_recognizer_strerror(s_zinnia_engine.m_recognizer));
        return -1;
    }

    s_zinnia_engine.m_character  = zinnia_character_new();
    zinnia_character_clear(s_zinnia_engine.m_character);
    zinnia_character_set_width(s_zinnia_engine.m_character, w);
    zinnia_character_set_height(s_zinnia_engine.m_character, h);

    return 0;
}

static int zinnia_recognize(WORD* pTrace, char* pResult, int *nCands, int nPoints, WORD wRange)
{
    WORD *pre = pTrace;
    pTrace += 6;
    int i = 0, index = 0;
    int finish = 0;
    
    zinnia_character_clear(s_zinnia_engine.m_character); 
    while(i < nPoints) {
        zinnia_character_add(s_zinnia_engine.m_character, index, *pre, *(pre+1));
        
        if ((*(pre+2) != *pTrace) && (*(pre+3) != *(pTrace+1))) {
            index++;
        }
        i++;
        pre += 6;
        pTrace += 6;
    }

    s_zinnia_engine.m_result = zinnia_recognizer_classify(
            s_zinnia_engine.m_recognizer, s_zinnia_engine.m_character, *nCands);
    if (s_zinnia_engine.m_result == NULL) {
        fprintf(stderr, "%s\n", zinnia_recognizer_strerror(s_zinnia_engine.m_recognizer));
        return -1;
    }
    
    memset(pResult, 0, strlen(pResult));
    for (i = 0; i < zinnia_result_size(s_zinnia_engine.m_result); i++) {
        sprintf(pResult, "%s%s", pResult, zinnia_result_value(s_zinnia_engine.m_result, i));
    }
    *nCands = i;
    return 0;
}

static int zinnia_exit(void)
{
    zinnia_result_destroy(s_zinnia_engine.m_result);
    zinnia_character_destroy(s_zinnia_engine.m_character);
    zinnia_recognizer_destroy(s_zinnia_engine.m_recognizer);
}

MGI_HW_ENGINE hw_zinnia_engine = {
    zinnia_init,
    zinnia_recognize,
    zinnia_exit
};
#endif
