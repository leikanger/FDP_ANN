/*
 * main.cpp
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */
#include "main.h"
#include "time.h"
//#include "../neuroElements/dendrite.h" //KAANSKJE DEN SKAL VÆRE MED? XXX

void initialiserArbeidsKoe();
void testFunksjon_slett(auron*);
void skrivUtArgumentKonvensjoner(std::string);
void* taskSchedulerFunction(void*);

//deklarasjoner
extern std::list<timeInterface*> time_class::pTaskArbeidsKoe_List;
extern unsigned long time_class::ulTidsiterasjoner;
std::ostream & operator<<(std::ostream& ut, auron* pAuronArg );
std::ostream & operator<< (std::ostream & ut, auron auronArg );

//extern std::list<timeInterface*> time_class::pTaskArbeidsKoe_List;

// Foreløpig testvariabel: 		Global variabel som skal lese inn fra argv**. 	
// XXX Gå over til funk-som-global-variabel!   Og referer til stroustrup i rapport.
unsigned long ulAntallTidsiterasjonerTESTING_SLETT;
//{ Alternativt med referanse-returnerende funk. : 
/**********************************************************************************
*** 	int& nAntallTidsiterasjoner()
*** 		returnerer referanse til static int nTidsgrenseForSimulering 
***					(som eksisterer heile kjøretida til programmet).
*** 		Vil dermed være mulig å skrive nAntallTidsiterasjoner()=5;
***
**********************************************************************************/
//int& nAntallTidsiterasjoner()
// {
//	static int nTidsgrenseForSimulering = DEFAULT_ANTALL_TIDSITERASJONER;
//	return nTidsgrenseForSimulering;
//}


//extern unsigned long u lTidsiterasjoner; // ligg i main.h

int main(int argc, char *argv[])
{
	// Initierer arbeidskø (time_class::pTaskArbeidsKoe_List)
	initialiserArbeidsKoe();



	//Leser inn argumenter: 
	if(argc > 1 ) //{1 	  //	 (argc>1 betyr at det står meir enn bare programkall)
	{
		int innArgumentPos = 1;

		while(argv[innArgumentPos][0] == '-')
		{
			cout<<"argv[" <<innArgumentPos <<"]: " <<argv[innArgumentPos] <<endl;

			switch( argv[innArgumentPos][1] )
			{
				case 'i':
					// Sjekker om antall iterasjoner er i samme argument (uten mellomrom):
					if( 		(ulAntallTidsiterasjonerTESTING_SLETT = atoi( &argv[innArgumentPos][2])) ) 	cout<<"Simulation length set to " <<ulAntallTidsiterasjonerTESTING_SLETT <<" time steps\n";
					// Ellers: sjekker om det er på neste argument (med mellomrom):
					else if( 	(ulAntallTidsiterasjonerTESTING_SLETT = atoi( argv[innArgumentPos+1]) ) ){
						++innArgumentPos;
						cout<<"Anntall tidsiterasjoner er satt til " <<ulAntallTidsiterasjonerTESTING_SLETT <<endl;
					}else{
						cout<<"Can not read argument. Please follow the conventions:" <<endl;
						skrivUtArgumentKonvensjoner(argv[0]);
						continue;
						//exit(-1);
					}
				default:
					if( atoi( argv[innArgumentPos] ) )
						cout<<"Her var eit tall, ja!\n";

					cout<<"Unknown argument: " <<argv[innArgumentPos] <<"\tUnable to complete request. Try again." <<endl;
					skrivUtArgumentKonvensjoner(argv[0]);
			}

			// Går vidare til neste argument.
			if( argv[innArgumentPos+1] ) 
				innArgumentPos++;
			else // dersom det ikkje finnes fleire argument å lese: break.
				break;
		}
		// ser på siste argument. Dersom dette er en int skal antall iterasjoner settes til dette:
		if( innArgumentPos == argc-1 )
		{
			int nInnInt;
			//innArgumentPos er på siste argumentet for programkallet.
			if( ( (nInnInt = atoi( argv[innArgumentPos]))>0) ) //Skal eg sette øvre grense også?
			{
				cout<<"Argument gives number of iterations to be: \t\t" <<nInnInt <<endl;
				ulAntallTidsiterasjonerTESTING_SLETT=nInnInt;
			}else{
				cout<<"Number of iterations must be a positive number.\nUse default: " <<DEFAULT_ANTALL_TIDSITERASJONER <<endl;
			}
		}
	}else{ // for if(argc > 1)
		cout<<"No arguments listed. Continue with default values:\tNumber of iterations: " <<DEFAULT_ANTALL_TIDSITERASJONER <<endl;
		ulAntallTidsiterasjonerTESTING_SLETT = DEFAULT_ANTALL_TIDSITERASJONER;

		skrivUtArgumentKonvensjoner(argv[0]);
	} //}1
	

	cout<<"******************************************\n*** BEGYNNER KJØRING AV ANN: ***\n******************************************\n\n";

		


	//TODO For at alle skal destrueres automatisk, legg alle peikere inn i std::vector, og destruer alle element i vector på slutten av main.


	auron* paA1 = new auron("A1");
	auron* paA2 = new auron("A2");
	auron* paA3 = new auron("A3");
	auron* paA4 = new auron("A4");
	auron* paA5 = new auron("A5");

	auron* paB = new auron("B");
	auron* paC = new auron("C");
	auron* paD = new auron("D");
	auron* paE = new auron("E", 50);


	new synapse(paA1, paA2, 41);
	new synapse(paA2, paA5, 111);
	new synapse(paA1, paA3, 111);
	new synapse(paA3, paA4, 111);
	new synapse(paA4, paA5, 111);
	new synapse(paA5, paA1, 111);

	new synapse(paA1, paB, 11);
	new synapse(paA2, paB, 11);
	new synapse(paA3, paB, 11);
	new synapse(paA4, paB, 11);
	new synapse(paA5, paB, 11);
	
	new synapse(paA1, paB, 11);
	new synapse(paA2, paB, 11);
	new synapse(paA3, paB, 11);
	new synapse(paA4, paB, 11);
	new synapse(paA5, paB, 11);
	
	new synapse(paA5, paC, 21);
	
	new synapse(paA5, paD, 11);
	new synapse(paC,  paD, 20);

	

	// Setter igang den neurale kretsen! -fyrer paA
	paA1->doTask();

	cout<<"lengde på arbeidkø (i tillegg til [time_class] ): " <<time_class::pTaskArbeidsKoe_List.size()-1 <<endl;


//for å lage gjenkjennelig start på utskrifta:
	for(int x = 0; x<20; x++){
		cout<<"pow(LEKKASJEFAKTOR_FOR_DEPOL, x): " <<pow(LEKKASJEFAKTOR_FOR_DEPOL, x) <<"\t=>\t" <<100*pow(LEKKASJEFAKTOR_FOR_DEPOL, x) <<endl;
	}


/****************** taskSchedulerFunction(0) ***************/
	sleep(1);
	taskSchedulerFunction(0);
	






	cout<<"\n\n\n\n\n\nSLETTER \t\t\tpaA\n\n";
	delete paA1;
	delete paA2;
	delete paA3;
	delete paA4;
	delete paA5;
	delete paB;
	delete paC;
 	delete paD;

	delete paE;

	/* <<aTest funker, men <<A.pOutputAxon funker ikkje (segfault). Må være feil med constructor..  Thats right! Brukte feil constructor pga. andre argument..*/
	
	//cout<<*(A.pOutputAxon) <<endl;
	
	
	/******************************************* Starter void taskSchedulerFunction(void*); ****************************************************/
//	taskSchedulerFunction(0);


	cout<<"\n\nWIN!\n";
	return 0;
}


void testFunksjon_slett(auron* pA) //XXX
{ //{
	//time_class::pTaskArbeidsKoe_List .push_back( pA->pOutputAxon );
	//time_class::pTaskArbeidsKoe_List .push_back( pA );

//	pA->ao_AuronetsAktivitet.updateDepol();
//	cout<<pA <<" sender inn 100.\n";
//	pA->ao_AuronetsAktivitet.incomingSignal(100);

//	time_class::ulTidsiterasjoner++;
//	pA->ao_AuronetsAktivitet.updateDepol();
//	cout<<"\n\nA:\t" <<*pA <<endl;


	(pA->pOutputAxon)->doTask();

	cout<<"Neste.\n";
	cout<<(pA->pOutputAxon) <<endl;

	//taskSchedulerFunction(0);
	
	/*
	cout<<"Første fyring i testFunksjon_slett():\n\n";
	pA->fyr();
	
	cout<<"Itererer time:\n\n";
	time_class::u lTidsiterasjoner++;

	cout<<"Andre fyring i testFunksjon_slett():\n\n";
	pA->fyr();
*/

} //}

void skrivUtArgumentKonvensjoner(std::string programKall)
{ //{
	cout<<"\n\nConventions for running auron.out: \n"
		<<"\t"<<programKall <<"[-options] [number of iterations]\n"
		<<"\t\tOptions: \n\t\t\t-i [n] \t number of iterations on simulation."
		<<"\nHUGS Å LEGGE VED VALG OM [spiking ANN], [K ANN], [sammenligning], osv.\n\n\n\n"; 		//TODO

} //}


/*****************************************************************
** 	void initialiserArbeidsKoe() 								**
**		-skal kjøre en gang(kun 1) for å initialisere arbeidskø.**
**		Ligger som egen funksjon for å få ryddig kode i main()	**
*****************************************************************/
void initialiserArbeidsKoe()
{ //{1
	// Sjekker om arbeidskø er initialisert fra før. I så fall returner uten å gjøre meir. (dette vil aldri skje - forsikrer meg mot det alikevel)
	static bool bInitialisertAllerede = false;
	if(bInitialisertAllerede) return;

	
	// Lager instans av time, og legger den i det frie lageret.
	time_class* pHovedskille = new time_class();
	// Legger til denne peikeren i arbeidskøa (som ligger som static-element i class time) :
 	time_class::pTaskArbeidsKoe_List 	.push_back( pHovedskille );

	// No ligger peikeren pHovedskille som einaste element i pTaskArbeidsKoe_List. Kvar gang denne kjører doTask() vil den ikkje fjærne seg fra arbeidsliste, men flytte seg bakerst i køa isteden.

	// static bInitialisertAllerede vil eksistere kvar gang denne funksjonen kalles. Setter dermed bInitialisertAllerede til true for å forhindre at fleire time legges til arbeidsKoe.
	bInitialisertAllerede = true;

} //}1


extern unsigned long ulAntallTidsiterasjonerTESTING_SLETT;
/*****************************************************************
** 	void* taskSchedulerFunction(void*)							**
** 																**
** 		- Har ansvaret for å schedule arbeidet. 				**
** 			- kaller nesteJobb.doTask()							**
** 			- For nesteJobb==time vil time_class::doTask() kalles 		**
** 				Dette itererer time, og gjør anna tidsrelevant 	**
** 					arbeid										**
** 																**
*****************************************************************/
void* taskSchedulerFunction(void* )
{ //{1
	cout<<"\n\n\t\t\t\t\tKjører void* taskSchedulerFunction(void*);\n";
	while( time_class::ulTidsiterasjoner <= ulAntallTidsiterasjonerTESTING_SLETT) // XXX Skal bli "uendelig" løkke etterkvart:
	//while(/*En eller anna avsluttings-bool =*/true)
	{
			/*FEILSJEKK (kan takast vekk)*/
			//cout<<"feilsjekk: Antall element i pTaskArbeidsKoe_List :  " <<time_class::pTaskArbeidsKoe_List.size() <<endl;
			if(time_class::pTaskArbeidsKoe_List.empty()){ cout<<"\n\n\nFEIL. time_class::pTaskArbeidsKoe_List er tom. Skal aldri skje. \nFeilmelding: [time.h taskSchedulerFunction::c01]\n\n\n"; exit(-1);}

			// DEBUG: 	Skriv ut klassenavn på element:
			cout<<time_class::pTaskArbeidsKoe_List.front() ->klasseNavn <<"\t:\t\t"; 		
	

			// Setter igang utføring av neste jobb i lista:
			time_class::pTaskArbeidsKoe_List.front() ->doTask(); 		//Dette er i orden, siden pTaskArbeidsKoe_List er av type list<timeInterface*> og alle arvinger av timeInterface har overlagra funksjonen doTask().

			// Tar vekk jobben fra pTaskArbeidsKoe_List:
			time_class::pTaskArbeidsKoe_List.pop_front();
			
			//Evt annet som skal gjøres kvart timessteg. Type sjekke etter andre events, legge til fleire synapser, etc.

			// For KANN: skal også sjekke om noko neuron er estimert til å fyre denne iterasjonen.
	}

	return 0;
} //}1



/***************************
*** Utskriftsprosedyrer: ***
***************************/
std::ostream & operator<< (std::ostream & ut, auron auronArg )
{ //{
	ut<<"| " <<auronArg.getNavn() <<"  | verdi: " <<auronArg.getAktivityVar();// <<" \t|\tMed utsynapser:\n";
	
	// Innsynapser:
	//for( std::vector<synapse*>::iterator iter = neuroArg.pInnSynapser.begin(); iter != neuroArg.pInnSynapser.end(); iter++ ){
	// 	ut 	<<"\t" <<(*iter)->pPreNode->navn <<" -> " <<neuroArg.navn <<"\t|" <<endl;
	// }

	// Utsynapser:
	//for( std::vector<synapse*>::iterator iter = neuroArg.pUtSynapser.begin(); iter != neuroArg.pUtSynapser.end(); iter++ ){
	// 	ut 	<<"\t\t\t|\t" <<neuroArg.navn <<" -> " <<(*iter)->pPostNode->navn <<endl;
	//		//<< (*iter)->ulAntallSynapticVesiclesAtt <<" antall syn.vesicles att.  TIL "
	// }


	return ut;
} //}
/* Når eg fjærna denne, vart eg kvitt helvetes-std-feilmeldinger. Trur ikkje denne skal brukes. Slett om nokre veker. No: 05.04.2011
std::ostream & operator<<(std::ostream& ut, auron* pAuronArg )
{
	ut<<(*pAuronArg);
	return ut;
}*/

std::ostream & operator<< (std::ostream & ut, axon* pAxonArg )
{ //{
	ut<<"Utsynapser fra axon tilhørende neuron " <<(pAxonArg->pElementAvAuron)->sNavn <<endl; 

ut<<"størrelse: " <<pAxonArg->pUtSynapser.size() <<endl;
	// Utsynapser:
	for( std::list<synapse*>::iterator iter = pAxonArg->pUtSynapser.begin(); iter != pAxonArg->pUtSynapser.end(); iter++ ){
	 	ut 	<<"\t\t\t|\t" <<(pAxonArg->pElementAvAuron)->sNavn <<" -> "    <<(*iter)->pPostNodeDendrite->pElementAvAuron->sNavn <<"\t\t|\n";
	}


	return ut;
} //}
/*
std::ostream & operator<< (std::ostream & ut, axon axonArg )
{ //{
	ut<<"Utsynapser fra axon tilhørende neuron " <<(axonArg.pElementAvAuron)->sNavn <<endl; 

	// Utsynapser:
	for( std::list<synapse*>::iterator iter = axonArg.pUtSynapser.begin(); iter != axonArg.pUtSynapser.end(); iter++ ){
	 	ut 	<<"\t\t\t|\t" <<(axonArg.pElementAvAuron)->sNavn <<" -> "    <<(*iter)->pPostNodeDendrite->pElementAvAuron->sNavn <<endl;
		
	}


	return ut;
} //}
*/
