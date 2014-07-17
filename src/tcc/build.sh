#!/bin/sh


if [ "x$1" = "xclean" ]; then
  rm -rf tcc*
  exit 0
fi

if [ ! -f tcc-0.9.26.tar.bz2 ]; then
  wget http://download.savannah.gnu.org/releases/tinycc/tcc-0.9.26.tar.bz2
fi

if [ ! -d tcc-0.9.26 ]; then
  tar xvfj tcc-0.9.26.tar.bz2
fi

(cd tcc-0.9.26; ./configure --prefix=$BRN_TOOLS_PATH/click-brn-libs; make; make install)
(cd tcc-0.9.26; ./configure --prefix=$BRN_TOOLS_PATH/click-brn-libs; make clean; DISABLE_STATIC=1 make; DISABLE_STATIC=1 make install)
#mv 

exit 0
