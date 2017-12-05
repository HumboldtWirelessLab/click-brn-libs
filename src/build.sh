#!/bin/sh

PROJECTS="click-lwip cuneiform tcc tinyxml tinyxml2 openann"

if [ "x$1" = "xrealclean" ]; then
  for i in $PROJECTS; do
    (cd $i; ./build.sh realclean)
  done
  rm -rf ../include ../lib ../share ../bin
else
  for i in $PROJECTS; do
    (cd $i; ./build.sh $1)
  done
fi

exit 0
