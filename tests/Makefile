
CC = clang++

# libscotch_PATH = /home/yuan/installed_source/scotch5112b/scotch_5.1.12/src/libscotch
# LINK_PATH = -L$(libscotch_PATH) -pthread -lscotch -lscotcherr

libscotch_PATH = ../thirdParty/libscotch/
LINK_PATH = -L$(libscotch_PATH) -pthread -lscotch-5.1 


INC_PATH = -I../src/ -I$(libscotch_PATH)

Files = ../src/*.cpp

Flags = -std=c++14 -Wall -g -D_USE_LIBSCOTCH=true

compile: $(Files)
	$(CC) test.cpp $(Files) $(INC_PATH) $(LINK_PATH) $(Flags) -o test