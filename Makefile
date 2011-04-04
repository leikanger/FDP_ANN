CFLAGS=-g -O2 -Wall #-pthread
CPPFLAGS=${CFLAGS}



#somaEnhet.o: soma/soma.cpp soma/soma.h andreKildefiler/tid.cpp andreKildefiler/tid.h dendritt_axon/axon.h synapse/synapse.cpp synapse/synapse.h
#	g++ ${CFLAGS} -c soma/soma.cpp synapse/synapse.cpp -o soma.o

#soma.o: andreKildefiler/tid.cpp andreKildefiler/tid.h neuroElements/soma.h neuroElements/axon.h neuroElements/synapse.cpp neuroElements/synapse.h
#	g++ ${CFLAGS} -c neuroElements/soma.cpp -o soma.o

neuroElement.o: andreKildefiler/tid.cpp andreKildefiler/tid.h neuroElements/axon.h neuroElements/dendrite.h neuroElements/synapse.h
	g++ ${CFLAGS} -c neuroElements/neuroElement.cpp -o neuroElement.o



auroNett.out: main.o neuroElement.o
	clear; echo "make auroNett.out:\n\n"; g++ ${CFLAGS} main.o neuroElement.o -o auroNett.out



main.o: andreKildefiler/main.cpp andreKildefiler/main.h andreKildefiler/tid.h andreKildefiler/tid.cpp 
	g++ ${CFLAGS} -c andreKildefiler/main.cpp 



c:
	make clean; clear; echo "\nHAR FJÆNA ALLE .out FILENE. Kjører make\n\n"; make auroNett.out


clean:
	rm -f *.out *.o

# *storbox og *litabox (med stjerne på slutten) lager dette, etter mi .vimrc . (med stjerne på slutten og.
