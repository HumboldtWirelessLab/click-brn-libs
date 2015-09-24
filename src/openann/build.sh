#!/bin/sh

#TODOSymlinks

GIT=/usr/bin/git

if [ "x$1" = "xclean" ]; then
  rm -rf OpenANN
  rm -rf eigen
  rm -rf build
  make clean

  exit 0
fi

if [ ! -d eigen ]; then
  hg clone https://bitbucket.org/eigen/eigen/
fi

if [ ! -d eigen ]; then
  wget http://bitbucket.org/eigen/eigen/get/3.2.5.tar.gz
  tar xvfz 3.2.5.tar.gz
  mv `ls -1 | grep eigen` eigen
  rm -f 3.2.5.tar.gz
fi

if [ ! -d OpenANN ]; then
  ${GIT} clone https://github.com/OpenANN/OpenANN OpenANN
  (cd OpenANN/lib; sh ./setup.sh CMA-ESpp ALGLIB)
  (cd OpenANN/test/lib; sh ./setup.sh CPP-Test)
fi

make openann
make install
make

exit 0
