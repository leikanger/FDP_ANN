//{ GNU GPL v.3
/***************************************************************************
*           auronNet - Implementation of event-driven spiking ANN          *
*                           -------------------                            *
* copyright            : (C) 2011 by Per R. Leikanger                      *
* email                : leikange@gmail.com                                *
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/
//}


#if 0
Haha! Fett. Ny måte å kommentere ut ting på! Thank you, K.
#endif

#if 1
// Ikkje kommentert ut
#endif


/*
 * main.cpp
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */
#include "main.h"
//#include "../neuroElements/auron.h"
#include "time.h"
#include "sensorFunk.h"

#include <sstream> //Testing

void initialiserArbeidsKoe();
void skrivUtArgumentKonvensjoner(std::string);
void* taskSchedulerFunction(void*);

//deklarasjoner
extern std::list<timeInterface*> 				time_class::pWorkTaskQue;
extern std::list<timeInterface*> 				time_class::pCalculatationTaskQue;
extern std::list<timeInterface*> 				time_class::pPeriodicElements;

extern std::list<i_auron*> i_auron::pAllAurons;
extern std::list<K_auron*> K_auron::pAllKappaAurons;
extern std::list<K_sensor_auron*> K_sensor_auron::pAllSensorAurons;
extern std::list<s_sensor_auron*> s_sensor_auron::pAllSensorAurons;
extern std::list<recalcKappaClass*> recalcKappaClass::pAllRecalcObj;

extern unsigned long time_class::ulTidsiterasjoner;

unsigned long comparisonClass::ulNumberOfCallsTo_doTask = 0;
unsigned long comparisonClass::ulNumberOfCallsToKappa_doCalculations = 0;

extern unsigned long ulAntallTidsiterasjonerTESTING_SLETT;

std::ostream & operator<<(std::ostream& ut, i_auron* pAuronArg );




void neuroElement_testFunk(K_auron* pK_arg);
void neuroElement_syn_testFunk(K_synapse* pK_syn_arg);


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

	// XXX Veit ikkje heilt med returverdien her. Trur det funka med ==256. Eg tenker det er bedre med !=0 (dersom den returnerer anna enn 0 bare ved feil?
	// JEss: mkdir returnerer 0 ved successfull completion.
	// Dersom ./datafiles_for_evaluation/ ikkje finnes, lages den. Dersm den finnes gjør ikkje kallet noke:
	if( system("mkdir datafiles_for_evaluation") != 0 ){
	// XXX Veit ikkje med rm. Finn ut kva returverdien er for denne. Satser (temporært) på at det er det samme:
		cout<<"Could not make directory for log files [./datafiles_for_evaluation/]. Directory probably already exist."
			<<"\n\tIn case this directory does not exist, please make this directory manually in the current directory.\n\n"; 
	}
	//Renser opp i ./datafiles_for_evaluation/
	if( system("rm ./datafiles_for_evaluation/log_*.oct") != 0)
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
	

	// Testoppsett:
	
 	#if 0  	// SANN: test-oppsett.
 	//{ SANN: TEST-oppsett. Lager mange neuron..
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
	new s_synapse(A9, A1, 1111);
	new s_synapse(A4, A1, 311);

	new s_synapse(A1, E, 15);
	new s_synapse(A2, E, 20);
	new s_synapse(A3, E, 15);
	new s_synapse(A4, E, 20);
	new s_synapse(A5, E, 15);
	new s_synapse(A6, E, 20);
	//new s_synapse(A7, E, 13);
	new s_synapse(A8, E, 20);
	new s_synapse(A9, E, 25);
	//new s_synapse(A1, E, 16);
	
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
	//}
	#endif

	#if 0  	//	SANN: 1-neurons testoppsett: (med 2-aurons oscillator-krets som input)
	//{ 	SANN - 1-auron
  	cout<<"\n\nLAGER SANN\n\n";
	s_auron* s1 = new s_auron("s1");
	s_auron* s2 = new s_auron("s2");
	new s_synapse(s1, s2, 1111);
	new s_synapse(s2, s1, 1111);

	s_auron* sA = new s_auron("sA");
#define FORHOLD_MELLOM_ALPHA_OG_INPUT_FAKTOR ALPHA
#define TALL 17.6
	new s_synapse(s1, sA, TALL);//(1.5 * FORHOLD_MELLOM_ALPHA_OG_INPUT_FAKTOR * FYRINGSTERSKEL ) );
	new s_synapse(s2, sA, TALL);//(1.5 * FORHOLD_MELLOM_ALPHA_OG_INPUT_FAKTOR * FYRINGSTERSKEL ) ); 
	
	s1->doTask();
	//}
  	#endif

	s_sensor_auron* sSensor  = new s_sensor_auron( "sSensor", &statiskSensorFunk);
	K_sensor_auron* kSensor  = new K_sensor_auron( "kSensor", &statiskSensorFunk);
	#if 0 	// SANN: Tester s_sensor_auron

	s_sensor_auron* sSensor  = new s_sensor_auron( "sSensor", &sensorFunk3a);
	s_auron* s1 = new s_auron("s1");
	new s_synapse(sSensor, s1, 50);

	s_auron* sUT = new s_auron("sUT");

	K_sensor_auron* KSensor  = new K_sensor_auron( "kSensor", &sensorFunk3a);
	K_auron* k1 = new K_auron("k1");
	new K_synapse(KSensor, k1, 50);

	K_auron* kUT = new K_auron("kUT");
	#endif

	#if 0 	// 	KANN-Test
 	//{ KANN: TEST-oppsett.

	cout<<"\n\nLAGER KANN\n\n";

	//K_auron* kA = new K_auron("kA", 2.07*FYRINGSTERSKEL);

#if 0 //{
	for(int i=0; i<100; i++){
		std::ostringstream tempString;
		tempString<<"K" <<i;
	
		K_auron* kA = new K_auron(tempString.str(), 2.07*FYRINGSTERSKEL);
	} 
#endif //}

	// neuroElement_testFunk() ER FARLIG! Når denne er med, blir v konst lik 0
	// NEI. Problemet er selvfølgelig at neuronet ikkje har input => kappa blir rekalkulert til null! FETT!
	//neuroElement_testFunk( kA );
#if 1

	K_auron* K1 = new K_auron("K1" /*, arg2 = 0? */);

	K_sensor_auron* KsStatisk = new K_sensor_auron("KsStatisk", &statiskSensorFunk);
	new K_synapse(KsStatisk, K1, 200);



	K_sensor_auron* Ks1 = new K_sensor_auron( "Ks1", &sensorFunk1 );
	new K_synapse( Ks1, K1, 400, true ); 
	
	K_sensor_auron* Ks2 = new K_sensor_auron( "Ks2", &sensorFunk2 );
	new K_synapse( Ks2, K1, 400, true ); 

	new K_synapse( KsStatisk, K1, 700, true );
	

//	K_sensor_auron* Ks4 = new K_sensor_auron( "Ks4", &sensorFunk4 );
//	new K_synapse( Ks4, K1, 2E5, true ); //Inhibitorisk synapse

	//s_auron* S1 = new s_auron("S1");
	//new s_synapse( Ks1, S1, 1E2);
#endif

#if 0 // Testoppsett 1, KANN
	K_auron* Ks1 = new K_sensor_auron("Ks1", &sensorFunk_TEST1_s1 );
	K_auron* Ks2 = new K_sensor_auron("Ks2", &sensorFunk_TEST1_s2 );
	K_auron* Ks3 = new K_sensor_auron("Ks3", &sensorFunk_TEST1_s3 );
	K_auron* Ks4 = new K_sensor_auron("Ks4", &sensorFunk_TEST1_s4 );
	K_auron* Ks5 = new K_sensor_auron("Ks5", &sensorFunk_TEST1_s5 );

	K_auron* Kt1 = new K_auron("Kt1");
	new K_synapse(Ks1, Kt1, 100);
	new K_synapse(Ks2, Kt1, 100);
	new K_synapse(Ks3, Kt1, 100);
	new K_synapse(Ks4, Kt1, 100);
	new K_synapse(Ks5, Kt1, 50, true);


	K_auron* Kt2 = new K_auron("Kt2");
	new K_synapse(Ks1, Kt2, 20, true);
	new K_synapse(Ks2, Kt2, 100);
	new K_synapse(Ks3, Kt2, 20, true);
	new K_synapse(Ks4, Kt2, 20, true);
	new K_synapse(Ks5, Kt2, 50);
	new K_synapse(Kt1, Kt2, 200);
	
	K_auron* Kt3 = new K_auron("Kt3");
	new K_synapse(Kt1, Kt3, 200);
	new K_synapse(Kt2, Kt3, 200);
#endif

/* //{
	K_auron* K2 = new K_auron("K_2", 1.2*FYRINGSTERSKEL);
	K_auron* K4 = new K_auron("K_4", 1.4*FYRINGSTERSKEL);
	K_auron* K9 = new K_auron("K_9", 1.9*FYRINGSTERSKEL);

	new K_synapse(K4, K2, 50);
	new K_synapse(K2, K4, 1111);


	K_auron* E = new K_auron("E", 4*FYRINGSTERSKEL);
	new K_synapse(K2, E, 100);
	new K_synapse(K4, E, 500, true);
*/ //}
	//} Slutt KANN-testopplegg
	#endif



	cout<<"lengde på arbeidkø (i tillegg til [time_class] ): " <<time_class::pWorkTaskQue.size()-1 <<endl;


	cout<<"******************************************\n*** BEGYNNER KJØRING AV ANN: ***\n******************************************\n\n";

/******************************************* Starter void taskSchedulerFunction(void*); ****************************************************/
	taskSchedulerFunction(0);




//XXXXXXXXXXXXXX TEST XXXXXXXXXXXXXXXXx
	//neuroElement_testFunk(K1);
//cout<<"sensed value: " <<Ks1->getSensedValue() <<"\n\n\n";




	cout<<"\n\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\nAvslutter.\n\n\n";

	

	cout<<"\n\n\n\nSkriver ut alle auron: \t\t";
	for( std::list<i_auron*>::iterator iter = i_auron::pAllAurons.begin() ;  iter != i_auron::pAllAurons.end() ;  iter++ )
	{
		cout<<"[ " <<(*iter)->sNavn <<" ]\t";
	}
	cout<<"\n\n";

	#if DEBUG_UTSKRIFTS_NIVAA > 3
		time_class::TEST_skrivUt_pWorkTaskQue();

		time_class::skrivUt_pPeriodicElements();
	#endif



	// Avlutt alle loggane rett:
	i_auron::callDestructorForAllAurons();


	cout<<"\n\nAntall kall til doTask() funksjoner: \t" <<comparisonClass::ulNumberOfCallsTo_doTask <<endl;
	cout<<"\n\nAntall kall til K_auron::doCalculation(): \t" <<comparisonClass::ulNumberOfCallsToKappa_doCalculations <<endl;

	cout<<"\n\nWIN!\n\n\n";
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
	// Itererer tid, slik at eg begynner på iter 1. Dette er viktig for å få rett initiering av K_auron som begynner med en konst kappa (gir K=(v_0-K)e^(-a*(1-0)) istedenfor K=..*e^0)
	time_class::ulTidsiterasjoner ++;

	// Rekalkulerer alle K_auron's Kappa:
	#if 1 // Kjører først alle aurons .doTask(), for å initiere alle synaptiske overføringer:
	for( std::list<K_auron*>::iterator K_iter = K_auron::pAllKappaAurons.begin(); K_iter != K_auron::pAllKappaAurons.end(); K_iter++ )
	{
		(*K_iter)->doTask();
		//(*K_iter)->kappaRecalculator.doTask();
	}
	#endif
	#if 1 // Rekalkulerer kappa, etter alle overføringer er initiert:
	for( std::list<K_auron*>::iterator K_iter = K_auron::pAllKappaAurons.begin(); K_iter != K_auron::pAllKappaAurons.end(); K_iter++ )
	{
		(*K_iter)->kappaRecalculator.doTask();
	}
	#endif


	/* * * * * * * * Begynner vanlig kjøring av auroNett * * * * * * * * */
	cout<<"\n\n\t\t\t\t\tKjører void* taskSchedulerFunction(void*);\n";


	while( time_class::ulTidsiterasjoner <= ulAntallTidsiterasjonerTESTING_SLETT) // XXX Skal bli "uendelig" løkke etterkvart:
	//while(/*En eller anna avsluttings-bool =*/true)
	{
			/*FEILSJEKK (kan takast vekk)*/
			#if 0
			if(time_class::pWorkTaskQue.empty()){ 
				cout<<"\n\n\nFEIL. time_class::pWorkTaskQue er tom. Skal aldri skje. \nFeilmelding: [taskSchedulerFunction::c01]\n\n\n"; 
				exit(-1);
			}
			#endif



			// Kortslutning, bare for å teste hypotese:
			




			// Setter igang utføring av neste jobb i lista:
			time_class::pWorkTaskQue.front() ->doTask(); 		//Dette er i orden, siden pWorkTaskQue er av type list<timeInterface*> og alle arvinger av timeInterface har overlagra funksjonen doTask().

			comparisonClass::ulNumberOfCallsTo_doTask ++;

			// Tar vekk jobben fra pWorkTaskQue: FLYTTA INN I time_class::doTask()
			time_class::pWorkTaskQue.pop_front();
			
			//Evt annet som skal gjøres kvart timessteg. Type sjekke etter andre events, legge til fleire synapser, etc.
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



// TODO TODO 
std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg ) //XXX Skal gjøres til i_axon* istaden for s_axon* som argument! XXX
{ //{
	ut<<"Utsynapser fra axon tilhørende neuron " <<(pAxonArg->pElementAvAuron)->sNavn <<endl; 

	// Utsynapser:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
	for( std::list<s_synapse*>::iterator iter = pAxonArg->pUtSynapser.begin(); iter != pAxonArg->pUtSynapser.end(); iter++ ){
	 	ut 	<<"\t\t\t|\t" <<(pAxonArg->pElementAvAuron)->sNavn <<" -> "    <<(*iter)->pPostNodeDendrite->pElementAvAuron->sNavn <<"\t\t|\n";
	}


	return ut;
} //}





/*
cout<<"TESTER NOKE:\n";
#define OUTPUT(x) cout<<x
OUTPUT("jess. OUTPUT(arg)\n");
OUTPUT("Endrer OUTPUT() til å være tom:\n");
#define OUTPUT2(x) 
OUTPUT2("NOPE, OUTPUT2(arg)\n");
OUTPUT("\nSkulle skrevet noke. Jess! Det gjorde det ikkje!\n");
*/
// vim:fdm=marker:fmr=//{,//}
