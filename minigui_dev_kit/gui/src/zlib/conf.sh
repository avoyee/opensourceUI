PREFIX=$1
EPREFIX=$2
MINIGUI_CROSS_COMPILE=$6
declare -x CC=${MINIGUI_CROSS_COMPILE}-gcc
./configure  --prefix=${PREFIX} --exec-prefix=${EPREFIX} 