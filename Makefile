CFLAGS=-g -O2 -Wall #-pthread
CPPFLAGS=${CFLAGS}



#all: andreKildefiler/main.cpp andreKildefiler/main.h andreKildefiler/tid.cpp andreKildefiler/time.h neuroElements/axon.h neuroElements/dendrite.h neuroElements/synapse.h neuroElements/neuroElement.cpp
#	clear; echo "make all:\n\n"; g++ ${CFLAGS} andreKildefiler/main.cpp neuroElements/neuroElement.cpp -o auroNett.out

auroNett.out: main.o neuroElement.o
	echo "\n\n\n\nmake auroNett.out:\n\n"; g++ ${CFLAGS} main.o neuroElement.o -o auroNett.out; echo "HURRA. ALT VEL!\n\n";

neuroElement.o: andreKildefiler/tid.cpp andreKildefiler/time.h neuroElements/axon.h neuroElements/dendrite.h neuroElements/synapse.h
	g++ ${CFLAGS} -c neuroElements/neuroElement.cpp -o neuroElement.o

main.o: andreKildefiler/main.cpp andreKildefiler/main.h andreKildefiler/time.h andreKildefiler/tid.cpp 
	g++ ${CFLAGS} -c andreKildefiler/main.cpp 



visBareErrors: main.o neuroElement.o
	make c 2>&1 | grep error #Funker om du skriver dette rett i shell!

c:
	make clean; clear; echo "\nHAR FJÆNA ALLE .out FILENE. Kjører make\n\n"; make auroNett.out


clean:
	rm -f *.out *.o

# *storbox og *litabox (med stjerne på slutten) lager dette, etter mi .vimrc . (med stjerne på slutten og.
