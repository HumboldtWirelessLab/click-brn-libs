#!/bin/sh


if [ "x$1" = "xclean" ]; then
  rm -rf tiny**
  exit 0
fi

if [ ! -f tinyxml_2_6_2.zip ]; then
#  wget -O tinyxml_2_6_2.zip http://downloads.sourceforge.net/project/tinyxml/tinyxml/2.6.2/tinyxml_2_6_2.zip?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Ftinyxml%2F&ts=1402468254&use_mirror=heanet
  wget -O tinyxml_2_6_2.zip http://downloads.sourceforge.net/project/tinyxml/tinyxml/2.6.2/tinyxml_2_6_2.zip
fi

echo "Unzip"

if [ ! -d tinyxml ]; then
  unzip tinyxml_2_6_2
  if [ -d tinyxml ]; then
    cp Makefile.lib tinyxml
  fi
fi

(cd tinyxml; make; make -f Makefile.lib; make -f Makefile.lib install)

exit 0
