PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6

./configure  --enable-debug="no"  --enable-invert="static" --enable-dejitter="static" --enable-linear="static" --enable-input="static"  --enable-pthres="static" --host=${MINIGUI_CROSS_COMPILE} --prefix=${PREFIX} --exec-prefix=${EPREFIX} --enable-static