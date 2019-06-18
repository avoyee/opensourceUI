#!/bin/bash

PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6

LOCAL_INCS=-I${PREFIX}/include

declare -x PKG_CONFIG_PATH=${EPREFIX}/lib/pkgconfig
declare -x CFLAGS="-rdynamic -funwind-tables "${LOCAL_INCS}

./configure    --host=${MINIGUI_CROSS_COMPILE} --prefix=${PREFIX} --exec-prefix=${EPREFIX}
