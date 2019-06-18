#!/bin/bash
PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6

LOCAL_INCS=-I${PREFIX}/include
FT2_INCS=${LOCAL_INCS}/freetype2
LOCAL_LIBS=-L${EPREFIX}/lib

declare -x PKG_CONFIG_PATH=${EPREFIX}/lib/pkgconfig
declare -x CPPFLAGS=${LOCAL_INCS}" "${FT2_INCS}
declare -x LDFLAGS=${LOCAL_LIBS}

./configure  --host=${MINIGUI_CROSS_COMPILE} --enable-ft2support="yes" --prefix=${PREFIX} --exec-prefix=${EPREFIX}
