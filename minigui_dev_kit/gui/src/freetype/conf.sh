PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6
./configure --host=${MINIGUI_CROSS_COMPILE} --prefix=${PREFIX} --exec-prefix=${EPREFIX}
