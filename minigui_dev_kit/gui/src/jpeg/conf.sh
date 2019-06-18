PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6
./configure  --prefix=${PREFIX} --exec-prefix=${EPREFIX} --host=${MINIGUI_CROSS_COMPILE}