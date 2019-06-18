#!/bin/bash
PREFIX=$1
EPREFIX=$2
SDKLIBS=$3
MINIGUI_CROSS_COMPILE=$6
PLAT_DEPENDENT_LIB=${PLATDEPENDENTLIB}

LOCAL_INCS=-I${PREFIX}/include
LOCAL_LIBS=-L${EPREFIX}/lib
SKD_LIBS=-L${SDKLIBS}

declare -x PKG_CONFIG_PATH=${EPREFIX}/lib/pkgconfig

declare -x CFLAGS=${LOCAL_INCS}" -rdynamic -funwind-tables"
#declare -x LDFLAGS="-lz -lfreetype -lchipmunk"
declare -x CHIPMUNK_CFLAGS=${LOCAL_INCS}
declare -x CHIPMUNK_LIBS=${LOCAL_LIBS}"  -lz -lchipmunk"
declare -x LIBS=${SKD_LIBS}" "${PLAT_DEPENDENT_LIB}" -lpthread"

./configure   --host=${MINIGUI_CROSS_COMPILE} --prefix=${PREFIX} --exec-prefix=${EPREFIX}
