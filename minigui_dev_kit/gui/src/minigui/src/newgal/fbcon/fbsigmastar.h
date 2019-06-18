#ifndef __FBSIGMASTAR_H__
#define __FBSIGMASTAR_H__

#include "fbvideo.h"

#define FB_SIGMASTAR
//#define FB_ACCEL_SIGMASTAR_I2


#if defined (FB_ACCEL_SIGMASTAR_I2) || defined (FB_ACCEL_SIGMASTAR_I2M)
#include "fbsigmastar_i2.h"
#elif defined (FB_ACCEL_SIGMASTAR_I5)
#include "fbsigmastar_i5.h"
#endif

void FB_SigmastarAccel(_THIS);

#endif
