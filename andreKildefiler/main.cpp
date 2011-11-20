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

/*
 * main.cpp
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */


#include "main.h"
//#include "../neuroElements/auron.h"
#include "../neuroElements/synapse.h"
#include "time.h"
#include "sensorFunk.h"

#include <sstream> 

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

extern unsigned long time_class::ulTime;

extern unsigned long ulLengthOfSimulation;

std::ostream & operator<<(std::ostream& ut, i_auron* pAuronArg );





// Foreløpig testvariabel: 		Global variabel som skal lese inn fra argv**. 	
// XXX Gå over til funk-som-global-variabel!   Og referer til stroustrup i rapport.
unsigned long ulLengthOfSimulation;
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



int main(int argc, char *argv[])
{
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
					if( 		(ulLengthOfSimulation = atoi( &argv[innArgumentPos][2])) ) 	cout<<"Simulation length set to " <<ulLengthOfSimulation <<" time steps\n";
					// Ellers: sjekker om det er på neste argument (med mellomrom):
					else if( 	(ulLengthOfSimulation = atoi( argv[innArgumentPos+1]) ) ){
						++innArgumentPos;
						cout<<"Anntall tidsiterasjoner er satt til " <<ulLengthOfSimulation <<endl;
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
				ulLengthOfSimulation=nInnInt;
			}else{
				cout<<"Number of iterations must be a positive number.\nUse default: " <<DEFAULT_ANTALL_TIDSITERASJONER <<endl;
			}
		}
	}else{ // for if(argc > 1)
		cout<<"No arguments listed. Continue with default values:\tNumber of iterations: " <<DEFAULT_ANTALL_TIDSITERASJONER <<endl;
		ulLengthOfSimulation = DEFAULT_ANTALL_TIDSITERASJONER;

		skrivUtArgumentKonvensjoner(argv[0]);
	} //}1
	
	// Returverdien på systemkallet returnerer -1 (eller andre feilmeldinger) ved feil og 0 når det går bra.
	// Dersom ./datafiles_for_evaluation/ ikkje finnes, lages den. Dersm den finnes gjør ikkje kallet noke:
	if( system("mkdir datafiles_for_evaluation") != 0 ){
		cout<<"Could not make directory for log files [./datafiles_for_evaluation/]. Directory probably already exist."
			<<"\n\tIn case this directory does not exist, please make this directory manually in the current directory.\n\n"; 
	}
	//Renser opp i ./datafiles_for_evaluation/
	if( system("rm ./datafiles_for_evaluation/log_*.oct") != 0)
		cout<<"Could not remove old log files. Please do this manually to avoid accidendtally plotting old results.\n";


	// Testoppsett:
// Blanda:

	#if 1
	//STATISK
	K_auron* Ks = new K_sensor_auron("_sKN", &statiskSensorFunk);
	s_auron* Ss = new s_sensor_auron("_sSN", &statiskSensorFunk);
	#endif
	//DYNAMISK
	K_auron* Kd = new K_sensor_auron("_dKN", &dynamiskSensorFunk);
	s_auron* Sd = new s_sensor_auron("_dSN", &dynamiskSensorFunk);

//  BARE KAPPA:
	#if 0
	K_sensor_auron* Ks1 = new K_sensor_auron("K_sensor_auron", &sensorFunk1a);
	K_auron* k1 = new K_auron("k1");
	new K_synapse(Ks1, k1, true);

//	#else 	// 	KANN-Test
	 	//{ KANN: TEST-oppsett.
		#if 0

		cout<<"\n\nLAGER KANN\n\n";

		K_auron* K1 = new K_auron("K1" /*, arg2 = 0? */);
		K_auron* K2 = new K_auron("K2" );
		K_auron* K3 = new K_auron("K3" );
	
		K_sensor_auron* KsStatisk = new K_sensor_auron("KsStatisk", &statiskSensorFunk);
		new K_synapse(KsStatisk, K1, 200);
	
	

		K_sensor_auron* Ks1 = new K_sensor_auron( "Ks1", &sensorFunk1 );
		new K_synapse( Ks1, K1, 400, false ); 
	
		K_sensor_auron* Ks2 = new K_sensor_auron( "Ks2", &sensorFunk2 );
		new K_synapse( Ks2, K1, 400, false ); 
		new K_synapse( Ks2, KsStatisk, 333, false);

		new K_synapse( KsStatisk, K1, 300, true );

	
		new K_synapse( Ks1, K2, 400);
		new K_synapse( Ks2, K2, 200);
		new K_synapse( Ks1, K3, 250);
		new K_synapse( Ks2, K3, 100);
		new K_synapse( K2, K3, 100);
		new K_synapse( K1, K3, 200, true);
		new K_synapse( K3, K1, 100);
		new K_synapse( K3, K1, 100, true);

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

	//} Slutt KANN-testopplegg
	#endif




	cout<<"******************************************\n*** BEGYNNER KJØRING AV ANN: ***\n******************************************\n\n";


/******************************************* Starter void taskSchedulerFunction(void*); ****************************************************/
	taskSchedulerFunction(0);
/*******************************************************************************************************************************************/


	cout<<"\n\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\nAvslutter.\n\n\n";

	

	cout<<"\n\n\n\nSkriver ut alle auron: \t\t";
	for( std::list<i_auron*>::iterator iter = i_auron::pAllAurons.begin() ;  iter != i_auron::pAllAurons.end() ;  iter++ )
	{
		cout<<"[ " <<(*iter)->sNavn <<" ]\t";
	}
	cout<<"\n\n";

	#if DEBUG_UTSKRIFTS_NIVAA > 3
		time_class::skrivUt_pPeriodicElements();
	#endif


	// Avlutt alle loggane rett:
	i_auron::callDestructorForAllAurons();


	cout<<"\n\nWIN!\n\n\n";
	return 0;
}


void skrivUtArgumentKonvensjoner(std::string programKall)
{ //{
	cout <<"\n\nConventions for executing auron.out: \n"
		 <<"\t"<<programKall <<"[-options] [number of iterations]\n"
		 <<"\t\tOptions: \n\t\t\t-i [n] \t number of iterations on simulation."
		 <<"\n\n\n\n\n";
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
	
	/****************************
	**  Initierer kjøring :    **
	****************************/

	// Initialiserer tid: begynner på iter 1. Dette (t_0=1) er viktig for å få rett initiering av K_auron som begynner med en konst kappa (gir K=(v_0-K)e^(-a*(1-0)) istedenfor K=..*e^0)
	time_class::ulTime = 0;

	// Initierer arbeidskø (time_class::pWorkTaskQue)
	initialiserArbeidsKoe();


	// Initialiserer 'time window' for alle K_auron:
	for( std::list<K_auron*>::iterator K_iter = K_auron::pAllKappaAurons.begin(); K_iter != K_auron::pAllKappaAurons.end(); K_iter++ )
	{
		// Initierer 'time window':
			// (*K_iter)->doTask();    // Løsninga for FDP: dette skaper en spike ved t=0 i plot av depol.
		// Setter v_0 til 0 og t_0 til [no]:
		(*K_iter)->dDepolAtStartOfTimeWindow = 0;
		(*K_iter)->dStartOfTimeWindow = (double)time_class::getTid();
		// Regner ut resulterende periode, osv.
		(*K_iter)->doCalculation();
	}


	/* * * * * * * * Begynner vanlig kjøring av auroNett * * * * * * * * */

	// Hoved-loop:
	while( time_class::ulTime <= ulLengthOfSimulation) 
	{

		// Setter igang utføring av neste jobb i lista:
		time_class::pWorkTaskQue.front() ->doTask(); 		//Dette er i orden, siden pWorkTaskQue er av type list<timeInterface*> og alle arvinger av timeInterface har overlagra funksjonen doTask().

		// Tar vekk jobben fra pWorkTaskQue:
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
	ut<<"Utsynapser fra axon tilhørende neuron " <<(pAxonArg->pElementOfAuron)->sNavn <<endl; 

	// Utsynapser:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
	for( std::list<s_synapse*>::iterator iter = pAxonArg->pUtSynapser.begin(); iter != pAxonArg->pUtSynapser.end(); iter++ ){
	 	ut 	<<"\t\t\t|\t" <<(pAxonArg->pElementOfAuron)->sNavn <<" -> "    <<(*iter)->pPostNodeDendrite->pElementOfAuron->sNavn <<"\t\t|\n";
	}


	return ut;
} //}





// vim:fdm=marker:fmr=//{,//}
