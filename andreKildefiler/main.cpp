/*
 * main.cpp
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */
#include "main.h"
#include "tid.h"

void initialiserArbeidsKoe();
void testFunksjon_slett(auron*);
void skrivUtArgumentKonvensjoner(std::string);
void* taskSchedulerFunction(void*);

//deklarasjoner
extern std::list<tidInterface*> tid::pTaskArbeidsKoe_List;
extern unsigned long tid::ulTidsiterasjoner;

//extern std::list<tidInterface*> tid::pTaskArbeidsKoe_List;

// Foreløpig testvariabel: 		Global variabel som skal lese inn fra argv**. 	
// XXX Gå over til funk-som-global-variabel!   Og referer til stroustrup i rapport.
int nAntallTidsiterasjonerTESTING_SLETT;
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
	// Initierer arbeidskø (tid::pTaskArbeidsKoe_List)
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
					if( 		(nAntallTidsiterasjonerTESTING_SLETT = atoi( &argv[innArgumentPos][2])) ) 	cout<<"Anntall tidsiterasjoner er satt til " <<nAntallTidsiterasjonerTESTING_SLETT <<endl;
					// Ellers: sjekker om det er på neste argument (med mellomrom):
					else if( 	(nAntallTidsiterasjonerTESTING_SLETT = atoi( argv[++innArgumentPos]) ) )		cout<<"Anntall tidsiterasjoner er satt til " <<nAntallTidsiterasjonerTESTING_SLETT <<endl;
					else{
						cout<<"Can not decode the argument. Please follow the conventions for arguments." <<endl;
						skrivUtArgumentKonvensjoner(argv[0]);
						exit(-1);
					}
				default:
					cout<<"Unknown argument: " <<argv[innArgumentPos] <<"\tPlease follow conventions and try again." <<endl;
					skrivUtArgumentKonvensjoner(argv[0]);
					exit(-1);
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
			if( (nInnInt = atoi( argv[innArgumentPos])>0) ) //Skal eg sette øvre grense også?
			{
				cout<<"Argument gives number of iterations to be: \t\t" <<nInnInt <<endl;
				nAntallTidsiterasjonerTESTING_SLETT=nInnInt;
			}else{
				cout<<"Number of iterations must be a positive number.\nUse default: " <<DEFAULT_ANTALL_TIDSITERASJONER <<endl;
			}
		}
	}else{ // for if(argc > 1)
		cout<<"No arguments listed. Continue with default values:\tNumber of iterations: " <<DEFAULT_ANTALL_TIDSITERASJONER <<endl;
		nAntallTidsiterasjonerTESTING_SLETT = DEFAULT_ANTALL_TIDSITERASJONER;

		skrivUtArgumentKonvensjoner(argv[0]);
	} //}1
	

	cout<<"******************************************\n*** BEGYNNER KJØRING AV auronNett.out: ***\n******************************************\n\n";

		




	auron A;
	auron B;
	
	auron C("C");
	cout<<C.sNavn;

	//synapse(&A, &B); //TODO Ikkje ferdig enda..

	testFunksjon_slett(&A);
	

	/******************************************* Starter void taskSchedulerFunction(void*); ****************************************************/
	taskSchedulerFunction(0);



	cout<<"\n\nWIN!\n";
	return 0;
}








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

	
	// Lager instans av tid, og legger den i det frie lageret.
	tid* pHovedskille = new tid();
	// Legger til denne peikeren i arbeidskøa (som ligger som static-element i class tid) :
 	tid::pTaskArbeidsKoe_List 	.push_back( pHovedskille );

	// No ligger peikeren pHovedskille som einaste element i pTaskArbeidsKoe_List. Kvar gang denne kjører doTask() vil den ikkje fjærne seg fra arbeidsliste, men flytte seg bakerst i køa isteden.

	// static bInitialisertAllerede vil eksistere kvar gang denne funksjonen kalles. Setter dermed bInitialisertAllerede til true for å forhindre at fleire tid legges til arbeidsKoe.
	bInitialisertAllerede = true;

} //}1


void testFunksjon_slett(auron* pA) //XXX
{ //{
	tid::pTaskArbeidsKoe_List .push_back( pA->pAxon_output );
	tid::pTaskArbeidsKoe_List .push_back( pA );

	//taskSchedulerFunction(0);
	
	/*
	cout<<"Første fyring i testFunksjon_slett():\n\n";
	pA->fyr();
	
	cout<<"Itererer tid:\n\n";
	tid::u lTidsiterasjoner++;

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


extern int nAntallTidsiterasjonerTESTING_SLETT;
/*****************************************************************
** 	void* taskSchedulerFunction(void*)							**
** 																**
** 		- Har ansvaret for å schedule arbeidet. 				**
** 			- kaller nesteJobb.doTask()							**
** 			- For nesteJobb==tid vil tid::doTask() kalles 		**
** 				Dette itererer tid, og gjør anna tidsrelevant 	**
** 					arbeid										**
** 																**
*****************************************************************/
void* taskSchedulerFunction(void* )
{ //{1
	cout<<"\n\nKjører void* taskSchedulerFunction(void*);\n";
	for(int i=0; i<=nAntallTidsiterasjonerTESTING_SLETT; i++) // XXX Skal bli "uendelig" løkke etterkvart:
	//while(/*En eller anna avsluttings-bool =*/true)
	{
			/*FEILSJEKK (kan takast vekk)*/
			if(tid::pTaskArbeidsKoe_List.empty()){ cout<<"\n\n\nFEIL. tid::pTaskArbeidsKoe_List er tom. Skal aldri skje. \nFeilmelding: [tid.h taskSchedulerFunction::c01]\n\n\n"; exit(-1);}

			// DEBUG: 	Skriv ut klassenavn på element:
			cout<<"\tKjører element i arbeidskø: " <<tid::pTaskArbeidsKoe_List.front() ->klasseNavn <<endl; 		
	

			// Setter igang utføring av neste jobb i lista:
			tid::pTaskArbeidsKoe_List.front() ->doTask(); 		//Dette er i orden, siden pTaskArbeidsKoe_List er av type list<tidInterface*> og alle arvinger av tidInterface har overlagra funksjonen doTask().

			// Tar vekk jobben fra pTaskArbeidsKoe_List:
			tid::pTaskArbeidsKoe_List.pop_front();
			
			//Evt annet som skal gjøres kvart tidssteg. Type sjekke etter andre events, legge til fleire synapser, etc.

			// For KANN: skal også sjekke om noko neuron er estimert til å fyre denne iterasjonen.
	}

	return 0;
} //}1



