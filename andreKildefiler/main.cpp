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
void skrivUtArgumentKonvensjoner(std::string);
void* taskSchedulerFunction(void*);

//deklarasjoner
extern std::list<timeInterface*> time_class::pWorkTaskQue;
extern std::list<i_auron*> i_auron::pAllAurons;
extern unsigned long time_class::ulTidsiterasjoner;
std::ostream & operator<<(std::ostream& ut, i_auron* pAuronArg );
//std::ostream & operator<< (std::ostream & ut, s_auron* pAuronArg );

//extern std::list<timeInterface*> time_class::pWorkTaskQue;

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
	// Initierer arbeidskø (time_class::pWorkTaskQue)
	initialiserArbeidsKoe();

	// Dersom ./datafiles_for_evaluation/ ikkje finnes, lages den. Dersm den finnes gjør ikkje kallet noke:
	if( system("mkdir datafiles_for_evaluation") == -1 ){
		cout<<"Could not make directory for log files [./datafiles_for_evaluation/]. For logging node values, please make this directory manually in the current directory."; 
	}
	//Renser opp i ./datafiles_for_evaluation/
	if( system("rm ./datafiles_for_evaluation/log_*.oct") == -1)
		cout<<"Could not remove old log files. Please do this manually to avoid accidendtally plotting old results.\n";

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
	
	
	//TODO For at alle skal destrueres automatisk, legg alle peikere inn i std::vector, og destruer alle element i vector på slutten av main.


/*  //{ SANN: TEST-oppsett. Lager mange neuron..
	s_auron* A1 = new s_auron("A1");
	s_auron* A2 = new s_auron("A2");
	s_auron* A3 = new s_auron("A3");
	s_auron* A4 = new s_auron("A4");
	s_auron* A5 = new s_auron("A5");
	s_auron* A6 = new s_auron("A6");
	s_auron* A7 = new s_auron("A7");
	s_auron* A8 = new s_auron("A8");
	s_auron* A9 = new s_auron("A9");

	s_auron* E = new s_auron("E");
	s_auron* F = new s_auron("F");

	cout<<"new s_synapse(A,B);\n";

	new s_synapse(A1, A2, 1111);
	new s_synapse(A8, A2, 111);
	
	new s_synapse(A4, A2, 111);
	new s_synapse(A6, A2, 111);

	new s_synapse(A2, A3, 1111);
	new s_synapse(A1, A3, 111);
	new s_synapse(A4, A3, 211);
	new s_synapse(A9, A3, 111);
	new s_synapse(A8, A3, 111);

	new s_synapse(A3, A4, 1111);
	new s_synapse(A4, A5, 1111);
	new s_synapse(A5, A6, 1111);
	new s_synapse(A6, A7, 1111);
	new s_synapse(A7, A8, 1111);
	new s_synapse(A8, A9, 1111);
	new s_synapse(A8, A9, 1111);
	new s_synapse(A9, A1, 1111);
	new s_synapse(A4, A1, 311);

	new s_synapse(A1, E, 151);
	new s_synapse(A2, E, 131);
	new s_synapse(A3, E, 121);
	new s_synapse(A3, E, 101);
	new s_synapse(A3, E, 121);
	new s_synapse(A5, E, 161);
	new s_synapse(A6, E, 131);
	new s_synapse(A8, E, 151);
	new s_synapse(A9, E, 191);
	new s_synapse(A1, E, 51);
	
	new s_synapse(A1, F, 251);
	new s_synapse(A2, F, 131);
	new s_synapse(A3, F, 221);
	new s_synapse(A3, F, 201);
	new s_synapse(A3, F, 121);
	new s_synapse(A5, F, 161);
	new s_synapse(A6, F, 231);
	new s_synapse(A8, F, 251);
	new s_synapse(A9, F, 191);
	new s_synapse(A1, F, 51);
	new s_synapse(E, F, 200);

	//Setter i gang ANN
	A1->doTask();
/**/ //}


	K_auron* A1 = new K_auron("K01", 50*FYRINGSTERSKEL);
	K_auron* B  = new K_auron("K_B", 50*FYRINGSTERSKEL);

	new K_synapse(A1, B, 1111);
	new K_synapse(B, A1, 1111);


	cout<<"lengde på arbeidkø (i tillegg til [time_class] ): " <<time_class::pWorkTaskQue.size()-1 <<endl;




	cout<<"******************************************\n*** BEGYNNER KJØRING AV ANN: ***\n******************************************\n\n";








/******************************************* Starter void taskSchedulerFunction(void*); ****************************************************/
	sleep(1);
	taskSchedulerFunction(0);



/****************************************** Kaller destructor for alle gjenværande udestruerte auron ***************************************/
	// Sletter alle auron i i_auron::pAllAurons
	while( ! i_auron::pAllAurons.empty() )
	{
		cout<<"Calls destructor for auron " <<i_auron::pAllAurons.front()->sNavn <<endl;
		// remove element from pAllAurons.
	 	delete (*i_auron::pAllAurons.begin());
	}



	cout<<"\n\nWIN!\n";
	return 0;
}


void skrivUtArgumentKonvensjoner(std::string programKall)
{ //{
	cout <<"\n\nConventions for running auron.out: \n"
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
 	time_class::pWorkTaskQue 	.push_back( pHovedskille );

	// No ligger peikeren pHovedskille som einaste element i pWorkTaskQue. Kvar gang denne kjører doTask() vil den ikkje fjærne seg fra arbeidsliste, men flytte seg bakerst i køa isteden.

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
			//cout<<"feilsjekk: Antall element i pWorkTaskQue :  " <<time_class::pWorkTaskQue.size() <<endl;
			if(time_class::pWorkTaskQue.empty()){ cout<<"\n\n\nFEIL. time_class::pWorkTaskQue er tom. Skal aldri skje. \nFeilmelding: [time.h taskSchedulerFunction::c01]\n\n\n"; exit(-1);}

			// DEBUG: 	Skriv ut klassenavn på element:
			cout<<time_class::pWorkTaskQue.front() ->klasseNavn <<"\t:\t\t"; 		
	

			// Setter igang utføring av neste jobb i lista:
			time_class::pWorkTaskQue.front() ->doTask(); 		//Dette er i orden, siden pWorkTaskQue er av type list<timeInterface*> og alle arvinger av timeInterface har overlagra funksjonen doTask().

			// Tar vekk jobben fra pWorkTaskQue:
			time_class::pWorkTaskQue.pop_front();
			
			//Evt annet som skal gjøres kvart timessteg. Type sjekke etter andre events, legge til fleire synapser, etc.

			// For KANN: skal også sjekke om noko neuron er estimert til å fyre denne iterasjonen.
	}

	return 0;
} //}1



/***************************
*** Utskriftsprosedyrer: ***
***************************/
std::ostream & operator<< (std::ostream & ut, i_auron* pAuronArg )
{ //{
	ut<<"| " <<pAuronArg->getNavn() <<"  | verdi: " <<pAuronArg->getAktivityVar();// <<" \t|\tMed utsynapser:\n";
	
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

std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg ) //XXX Skal gjøres til i_axon* istaden for s_axon* som argument! XXX
{ //{
	ut<<"Utsynapser fra axon tilhørende neuron " <<(pAxonArg->pElementAvAuron)->sNavn <<endl; 

	// Utsynapser:
	for( std::list<i_synapse*>::iterator iter = pAxonArg->pUtSynapser.begin(); iter != pAxonArg->pUtSynapser.end(); iter++ ){
	 	ut 	<<"\t\t\t|\t" <<(pAxonArg->pElementAvAuron)->sNavn <<" -> "    <<(*iter)->pPostNodeDendrite->pElementAvAuron->sNavn <<"\t\t|\n";
	}


	return ut;
} //}

