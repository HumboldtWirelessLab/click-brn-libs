#!/bin/sh

#TODOSymlinks

GIT=/usr/bin/git

if [ "x$1" = "xclean" ]; then
  if [ -f brn-lwip/src/core/def.c ]; then
    (cd lib; make clean)
  fi

  rm -rf brn-lwip
  rm -rf brn-lwip-contrib

  exit 0
fi

if [ ! -d brn-lwip ]; then
  ${GIT} clone https://git.savannah.gnu.org/cgit/lwip.git brn-lwip
#else
#  (cd brn-lwip; git checkout master; git pull)
fi

if [ ! -d brn-lwip-contrib ]; then
  ${GIT} clone https://git.savannah.gnu.org/cgit/lwip/lwip-contrib.git brn-lwip-contrib
#else
#  (cd brn-lwip-contrib; git checkout master; git pull)
fi

(cd brn-lwip; ${GIT} branch -d brn; git checkout -b brn 6ca8bc4037fcdb80a9e6bf2a6f09061fc721face)
(cd brn-lwip-contrib; ${GIT} branch -d brn; git checkout -b brn ffd674283b533c0f5b7fbf4771814ebd518b0c57)

(cd lib; make clean)
(cd lib; make)
(cd lib; make install)


exit 0
