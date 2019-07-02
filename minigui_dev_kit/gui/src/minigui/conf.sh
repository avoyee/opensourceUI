PREFIX=$1
EPREFIX=$2
SDKINCS=$4
UAPIINCS=$5
MINIGUI_CROSS_COMPILE=$6
PLATFORM=$7

LOCAL_INCS=${PREFIX}/include
LOCAL_LIBS=-L${EPREFIX}/lib
SDK_INCS=-I${SDKINCS}
UAPI_INCS=-I${UAPIINCS}
FT2_INCS=${LOCAL_INCS}/freetype2
declare -x CFLAGS="-Wno-unused-value -Wno-misleading-indentation -rdynamic -funwind-tables "-I${LOCAL_INCS}" "${SDK_INCS}" "${UAPI_INCS}
declare -x LDFLAGS=${LOCAL_LIBS}

sed -i "s/FB_ACCEL_SSTAR_.*/FB_ACCEL_SSTAR_${PLATFORM}, 1, [Define SSTAR GUI ACCELERATOR])/g" configure.ac
autoheader
autoconf

./configure --disable-cursor --with-runmode=ths --enable-miniguientry --enable-videosstar="yes"  --disable-splash  --with-ft2-includes=${FT2_INCS} --host=${MINIGUI_CROSS_COMPILE} --prefix=${PREFIX} --exec-prefix=${EPREFIX} --enable-shared="yes" --enable-static="yes" --enable-videoshadow --disable-videomlshadow --enable-tslibial="yes" --enable-videodummy="no" --enable-videopcxvfb="no" --enable-videofbcon="no" --enable-detaildebug="no"