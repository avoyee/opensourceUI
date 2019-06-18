PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6
LOCAL_INCS=-I${PREFIX}/include


declare -x CFLAGS=${LOCAL_INCS}
declare -x CPPFLAGS=${LOCAL_INCS}


./configure  --prefix=${PREFIX} --exec-prefix=${EPREFIX} --host=${MINIGUI_CROSS_COMPILE}