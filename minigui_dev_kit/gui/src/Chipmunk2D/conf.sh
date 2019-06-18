PREFIX=$1
rm -f CMakeCache.txt
cmake -D CMAKE_INSTALL_PREFIX=${PREFIX} ./