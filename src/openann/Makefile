OPENANN_HOME = ../..
TINYXML2_HOME = ../..
CXX = g++

INCLUDE=-I${OPENANN_HOME}/include -I${TINYXML2_HOME}/include
LIBS=-L${TINYXML2_HOME}/lib -ltinyxml2 -L${OPENANN_HOME}/lib -lopenann


all: ann-train

#alglib:
#	${CXX} -static -O3 -DNDEBBUG -fPIC 

openann:
	mkdir -p build && cd build && \
	${CXX} -static -Dopenann_EXPORTS -O3 -DNDEBUG -fPIC \
	-I../OpenANN/. \
	-I../OpenANN/lib/CMA-ESpp \
	-I../OpenANN/lib/ALGLIB \
	-I../OpenANN/test/lib/CPP-Test \
	-I../eigen/ \
	-g0 -s -DOPENANN_LOGLEVEL=OpenANN::Log::INFO \
	-c ../OpenANN/src/*.cpp ../OpenANN/lib/ALGLIB/*.cpp
	ar rcs  build/libopenann.a build/*.o
	

ann-train:
	${CXX} ${INCLUDE} ann-train.cpp -o $@ \
	${CFLAGS} ${LDFLAGS} ${LIBS}

install:
	mkdir -p ../../lib
	cp build/libopenann* ../../lib
	mkdir -p ../../include/
	cp -r OpenANN/OpenANN ../../include/
	cp -r eigen/Eigen ../../include/


clean:
	@rm -rf build/* ann-train

