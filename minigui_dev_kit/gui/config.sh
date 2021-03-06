#!/bin/bash

MODULE=$1
PREFIX=$2
EPREFIX=$3
SDKLIB=$4
SDKINC=$5
UAPIINC=$6
MINIGUI_CROSS_COMPILE=$7
PLAT_DEPENDENT_LIB=$8
GRAPHICACCEL=$9
export PATH=$PATH:${EPREFIX}/bin
cd ${MODULE}
if [ -e "configure" ]; then
touch ./configure
fi
if [ -e "conf.sh" ]; then
export PLATDEPENDENTLIB=${PLAT_DEPENDENT_LIB}
sh conf.sh ${PREFIX} ${EPREFIX} ${SDKLIB} ${SDKINC} ${UAPIINC} ${MINIGUI_CROSS_COMPILE} ${GRAPHICACCEL}
fi
pwd
