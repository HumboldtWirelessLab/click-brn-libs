#!/bin/sh


if [ "x$1" = "xrealclean" ]; then
  rm -rf ../include ../lib ../share ../bin
else
  for i in click-lwip  tcc  tinyxml tinyxml2 openann; do
    (cd $i; ./build.sh $1)
  done
fi

exit 0
