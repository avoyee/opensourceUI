SDKLIBS=$3
SDKINCS=$4

SDKLIBS=$(echo "${SDKLIBS}" | sed  's/\//\\\//g' )
SDKINCS=$(echo "${SDKINCS}" | sed  's/\//\\\//g' )

sed -i "s/SDK_LIBS=.*/SDK_LIBS=${SDKLIBS}/g" makefile
sed -i "s/SDK_INCS=.*/SDK_INCS=${SDKINCS}/g" makefile
sed -i "s/PLAT_DEPENDENT_LIB=.*/PLAT_DEPENDENT_LIB=${PLATDEPENDENTLIB}/g" makefile
