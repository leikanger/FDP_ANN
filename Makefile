CFLAGS=-g -O2 -Wall #-pthread
CPPFLAGS=${CFLAGS}

all: auroNett.out main.o auron.o synapse.o auron/auron.cpp auron/auron.h andreKildefiler/*.cpp andreKildefiler/*.h andreKildefiler/tid.cpp dendritt_axon/axon.h
	clear; echo "make auronNett.out:\n\n"; make auroNett.out

auroNett.out: main.o auron.o synapse.o
	g++ ${CFLAGS} main.o auron.o synapse.o -o auroNett.out

#auronEnhet.o: auron/auron.cpp auron/auron.h andreKildefiler/tid.cpp andreKildefiler/tid.h dendritt_axon/axon.h synapse/synapse.cpp synapse/synapse.h
#	g++ ${CFLAGS} -c auron/auron.cpp synapse/synapse.cpp -o auron.o

auron.o: auron/auron.cpp auron/auron.h andreKildefiler/tid.cpp andreKildefiler/tid.h dendritt_axon/axon.h synapse/synapse.cpp synapse/synapse.h
	g++ ${CFLAGS} -c auron/auron.cpp -o auron.o



synapse.o: andreKildefiler/tid.cpp andreKildefiler/tid.h dendritt_axon/axon.h synapse/synapse.cpp synapse/synapse.h
	g++ ${CFLAGS} -c synapse/synapse.cpp -o synapse.o



main.o: andreKildefiler/main.cpp andreKildefiler/main.h andreKildefiler/tid.h andreKildefiler/tid.cpp dendritt_axon/axon.h 
	g++ ${CFLAGS} -c andreKildefiler/main.cpp 



uten_o_filer:  auron/auron.cpp auron/auron.h andreKildefiler/tid.cpp andreKildefiler/tid.h dendritt_axon/axon.h synapse/synapse.cpp synapse/synapse.h andreKildefiler/main.cpp andreKildefiler/main.h 
	g++ ${CFLAGS} andreKildefiler/main.cpp andreKildefiler/tid.cpp auron/auron.cpp synapse/synapse.cpp -o auroNett.out


c:
	make clean; clear; echo "\nHAR FJÆNA ALLE .out FILENE. Kjører make\n\n"; make auroNett.out


clean:
	rm -f *.out *.o

# *storbox og *litabox (med stjerne på slutten) lager dette, etter mi .vimrc . (med stjerne på slutten og.
