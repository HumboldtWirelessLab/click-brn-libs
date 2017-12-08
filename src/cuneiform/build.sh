#!/bin/sh

GIT=/usr/bin/git

if [ "x$1" = "xclean" ]; then
  if [ -d cuneiform ]; then
    (cd cuneiform; make clean)
  fi
  exit 0
fi

if [ ! -d cuneiform ]; then
  ${GIT} clone https://github.com/joergen7/cuneiform.git cuneiform
  #(cd cuneiform; ${GIT} branch -d brn; git checkout -b brn f0297b542c87bba1100ac737560d96bd4a1afa70)
fi

if [ ! -d rebar3 ]; then
  #${GIT} clone https://github.com/erlang/rebar3.git rebar3
  #(cd rebar3; ${GIT} branch -d brn; git checkout -b brn c21a5f7ab2309bedfd430e71ece576cbfaa93114)
  mkdir rebar3
fi

echo "Build rebar3"

if [ ! -f rebar3/rebar3 ]; then
  #(cd rebar3; ./bootstrap)
  (cd rebar3; wget https://s3.amazonaws.com/rebar3/rebar3 && chmod +x rebar3)
fi

echo "Build cuneiform"

if [ ! -f cuneiform/rebar3 ]; then
  cp rebar3/rebar3 cuneiform
fi

(cd cuneiform; ./rebar3 escriptize)

cp cuneiform/_build/default/bin/cuneiform $BRN_TOOLS_PATH/click-brn-libs/bin/

exit 0
