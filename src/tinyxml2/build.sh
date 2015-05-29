#!/bin/sh

#TODOSymlinks

GIT=/usr/bin/git

if [ "x$1" = "xclean" ]; then
  rm -rf tinyxml2

  exit 0
fi

if [ ! -d tinyxml2 ]; then
  ${GIT} clone https://github.com/leethomason/tinyxml2 tinyxml2
fi

cp Makefile.lib tinyxml2
(cd tinyxml2; CFLAGS="-fpic" make -f Makefile.lib)
(cd tinyxml2; make -f Makefile.lib install)


exit 0
