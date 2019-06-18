#!/bin/bash

PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6

LOCAL_INCS=-I${PREFIX}/include

declare -x PKG_CONFIG_PATH=${EPREFIX}/lib/pkgconfig
declare -x CPPFLAGS=${LOCAL_INCS}

./configure  --prefix=${PREFIX} --exec-prefix=${EPREFIX} --host=${MINIGUI_CROSS_COMPILE} --enable-detail_debug="no" --enable-develmode="no"