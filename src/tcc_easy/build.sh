#!/bin/sh


if [ "x$1" = "xclean" ]; then
  rm -rf tiny_c
  exit 0
fi

if [ ! -e tiny_c ]; then
  git clone git@gitlab.informatik.hu-berlin.de:rempe/tiny_c.git
  echo "TCCBASE          := $BRN_TOOLS_PATH/click-brn-libs/" > tiny_c/makefile.local
fi

(cd tiny_c; make sharedlib;)
mkdir -p $BRN_TOOLS_PATH/click-brn-libs/include/tcceasy;
for i in `cd tiny_c; ls *.h`; do
  echo $i;
  sed "s#include \"#include \"tcceasy/#g" tiny_c/$i > $BRN_TOOLS_PATH/click-brn-libs/include/tcceasy/$i
done

cp tiny_c/libtcceasy.so $BRN_TOOLS_PATH/click-brn-libs/lib/libtcceasy.so

exit 0
