CFLAGS=-g -O2 -Wall #-pthread
CPPFLAGS=${CFLAGS}


auroNett.out: main.o auron.o neuroElement.o
	clear; echo "make auronNett.out:\n\n"; g++ ${CFLAGS} main.o auron.o neuroElement.o -o auroNett.out

#auronEnhet.o: auron/auron.cpp auron/auron.h andreKildefiler/tid.cpp andreKildefiler/tid.h dendritt_axon/axon.h synapse/synapse.cpp synapse/synapse.h
#	g++ ${CFLAGS} -c auron/auron.cpp synapse/synapse.cpp -o auron.o

auron.o: andreKildefiler/tid.cpp andreKildefiler/tid.h neuroElements/auron.cpp neuroElements/auron.h neuroElements/axon.h neuroElements/synapse.cpp neuroElements/synapse.h
	g++ ${CFLAGS} -c neuroElements/auron.cpp -o auron.o

neuroElement.o: andreKildefiler/tid.cpp andreKildefiler/tid.h neuroElements/axon.h neuroElements/dendrite.h neuroElements/synapse.h
	g++ ${CFLAGS} -c neuroElements/neuroElement.cpp -o neuroElement.o






main.o: andreKildefiler/main.cpp andreKildefiler/main.h andreKildefiler/tid.h andreKildefiler/tid.cpp 
	g++ ${CFLAGS} -c andreKildefiler/main.cpp 



c:
	make clean; clear; echo "\nHAR FJÆNA ALLE .out FILENE. Kjører make\n\n"; make auroNett.out


clean:
	rm -f *.out *.o

# *storbox og *litabox (med stjerne på slutten) lager dette, etter mi .vimrc . (med stjerne på slutten og.
