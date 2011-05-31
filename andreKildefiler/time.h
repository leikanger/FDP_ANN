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
#ifndef TID_H_
#define TID_H_

// TODO sjekk om desse er nødvendig (kommenter ut, når kompilering funker..)
//#include "main.h"
#include "../neuroElements/auron.h"

using std::cout;
using std::cerr;
using std::endl;

// DEKLARASJONER:
//extern unsigned long ulTidsiterasjoner;
void loggeFunk_K_auron();


// BARE FOR TESTING! TODO fjærn neste linja:
class i_auron;

class K_auron;


/****************************************
***      abstract class !             ***
****************************************/
class timeInterface
{
	public:
	timeInterface(std::string s) : lEstimatedTaskTime_for_object(0), sClassName(s){}
	timeInterface() 				{} 				//For mens eg itererer i utviklinga.  Trur ikkje eg skal ha denne etterpå..

	virtual void doTask() =0;
	virtual void doCalculation() =0;

	long lEstimatedTaskTime_for_object; //Brukes for når man skal flytte ledd i pEstimatedTaskTime. Veldig viktig for KANN!


	//for debugging:
	std::string sClassName;
};









/*********************************************************************************
** 	class time  																	**
** 		- time skal bare være en instans av denne klassen (i planen så langt). 		**
** 		 																		**
*********************************************************************************/
class time_class : public timeInterface {
	static unsigned long ulTidsiterasjoner;
	
	// TODO Endre neste til std::que ? XXX Sjå stroustrup s.576
	static std::list<timeInterface*> pWorkTaskQue;
	// Kanskje bedre å gå over til vector, og gå gjennom lista og sjå etter duplikat i time_class::doCalculation()
	static std::list<timeInterface*> pCalculatationTaskQue;
	// std::set er en container der key og value er det samme. Unique elements!

	//liste bestående av list<timeInterface*>* (merk: består av list-PEIKERE. Dette for å kunne bruke [new list<>] i tillegg av tidspunkt.. 	//XXX HER NYNYNY NY XXX
	static std::list< std::list<timeInterface*>* > pEstimatedTaskTime;

	// Ligger i .doTask();
	//static inline void movePlannedElementsTo_pWorkTaskQue();
	#if 0
	{
		for( std::list<K_auron*>::iterator K_iter = K_auron::pAllKappaAurons.begin() ; K_iter != K_auron::pAllKappaAurons.end() ; K_iter++ )
		{
			if( (*K_iter)->lEstimatedTaskTime_for_object == timeIterations+1 )
			{
				addTaskIn_pWorkTaskQue( (*K_iter) );
			}
		}
	}
	#endif

	protected:
	void doTask();
	void doCalculation()
	{ //{
		/**************************************************************************************
		*** Gjennomføre kalkulering på alle kalkuleringsoppgaver (pCalculatationTaskQue) 	***
		**************************************************************************************/

		//DEBUG("Inne i time_class::doCalculation()\n");

		if( pCalculatationTaskQue.empty() ) return;

		// Organiserer liste slik at kvar oppføring er unik: 			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<timeInterface*>::iterator iter = pCalculatationTaskQue.begin(); iter != pCalculatationTaskQue.end(); iter++ )
		{
			// Testet. Virker som det funker no.
			std::list<timeInterface*>::iterator iter2 = iter; 
			iter2++;
			while(iter2!=pCalculatationTaskQue.end()){
				// ser om iteratorene peker til samme minneadresse (samme timeInterface-element). Isåfall: fjærn det andre elementet.
			 	if( (*iter2) == (*iter) ){ //c out<<"Var duplikat. Fjærner andre element.\n";
					// Øker iterator før eg sletter element på iter2.
					std::list<timeInterface*>::iterator slettIter = iter2;
					iter2++;
					pCalculatationTaskQue.erase(slettIter);
					continue;
				}
				iter2++;
			}
		}
	
		while( !pCalculatationTaskQue.empty() ){
			// Kaller pCalculatationTaskQue.front()->pCalculatationTaskQue();
			#if DEBUG_UTSKRIFTS_NIVAA > 3
			cout<<"Kaller " <<pCalculatationTaskQue.front()->sClassName <<".doCalculation()\n";
			#endif

			pCalculatationTaskQue.front()->doCalculation();
			pCalculatationTaskQue.pop_front();
		
		}
	} //}

	static inline void addCalculationIn_pCalculatationTaskQue( timeInterface* pObject_arg)
	{
	 	pCalculatationTaskQue.push_back( pObject_arg );
	}

/***************************************************************************
*
* 		TODO for dette opplegget:
* 			- lag bidirectional peiker. Peiker til rett timeInterface her, og peiker til rett iterator (hit) i timeInterface elementet. TODO Det er dermed kanskje best å returnere iterator fra funksjonen under.
* 			- når timeInterface-elementet får nytt estimat, skal elementet flyttes (fjærnes, så legges inn ELLER bare flyttes den relative avstanden (kanskje best))
* 				Det er fundamentalt at det bare er en timeInterface peiker tilstede i lista (ellers får vi ekstra fyringer = overflødig arbeid..) 
** 				[LØSNING: lag en funksjon som også tar inn en {list< list<timeInterface*>* >::iterator}, og denne vil bare flytte elementet 			]
** 				[ 			og når elementet blir gjennomført, flyttes elementet (?) til plass gitt av no+periode (dvs. med bare periode som argument. 	]
** 				[ 			Dersom elementet skal fjærnes (f.eks. for s_auron), så kan vi lagre NULL i peikeren (?) 									]
* 			- Bli sikker på korleis indekseringa er under. 1 indeksert? 0-indeksert? Trur det skal være antall tidsiterasjoner etter NESTE tidsiter. Også viktig for å få rett funksjonalitet!
*
* 		Så: implementere i time_class::doTask() at den skal gjøre alle jobbene i [neste-tids-iter]-list i pEstimated TaskTime (for så å ta vekk denne [neste-tidIter]-list i p EstimatedTaskTime.
* 			- kvart element skal sjølv være ansvarlig for å flytte seg i p EstimatedTaskTime etter AP.
*
****************************************************************************/




#if 0 // Kommenterer ut gammel pEstimatedTaskTime-liste.
//{
	/********************************************************************************
	*  	list<list*>::iterator get_iteration_in_pEstimatedTaskTime( unsigned ) 		*
	* 	  Argument: 	unsigned uTid: tid til rett tidsiterator (i fremtida) 		*
	* 	  Retur: 		list<list*>::iterator til rett tidsiterasjon. 				*
	********************************************************************************/
	// TODO TODO Undersøk om det er mulig å lagre iteratoren i objektet, og bruke denne istedenfor å heile tida kalle get_iteration_in_pEstimatedTaskTime(). 
	// unngikk dette siden det stod i stroustrup, men eg trur han snakka om std::vector..
	static inline std::list< std::list<timeInterface*>* >::iterator   get_iteration_in_pEstimatedTaskTime_list( int nTimeDifference_arg )
	{ //{
		// Dersom nTimeDifference_arg seier den allerede skulle fyrt, legg den til fyring neste tidsiter.
		if( nTimeDifference_arg <= 0){
			return pEstimatedTaskTime.begin();
		}
//		if( nTimeDifference_arg > MAKS_LENGDE_PAA_pEstimatedTaskTime_JOBBER)
//			return 												TODO TODO TODO TODO Finn på noke lurt å returnere her (dersom vi ikkje vil finne elementet (out-of-scope) )

		// Dersom estimert tid er utafor lista av estimert tid: auk lista.
		int nDiff = nTimeDifference_arg - pEstimatedTaskTime.size()  ;
		if( nDiff >= 0 ) //Mangler ledd. Legg til rett antall.
		{
//			cout<<"Mangler ledd. Legger til de manglende ledda. Mangler " <<nDiff <<" ledd.\n";
			for(int i=0; i <= nDiff; i++){
				pEstimatedTaskTime.push_back( new std::list<timeInterface*> );
			}
		}

		std::list< std::list<timeInterface*>* >::iterator estTimeIter;

		// Finne rett plassering (skal returneres)
		if( nTimeDifference_arg < (pEstimatedTaskTime.size())/2 ){ 
			//Søk fra begynnelsen.
			estTimeIter = pEstimatedTaskTime.begin();
/*
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
Veit ikkje om for() er bra under. Trur det er broblemer med index. Var (fra 0;<=;++) blei (fra 1; < ; ++)

Meir: Veit ikkje om den som søker fra oppsida er rett heller. No er eg jævla sliten!
*/


			for(unsigned u=1; u < nTimeDifference_arg; u++)
				estTimeIter++;
				// gjorde <= om til < . Veit ikkje om det blir rett. TEST
		}else if(nTimeDifference_arg <= pEstimatedTaskTime.size() ){  // Sjekker om den ligger i resten av lista. Skal det være [<=] her? Sjekk når du kjeder deg..
			//Søk fra slutten:
			estTimeIter = (pEstimatedTaskTime.end());
			for(unsigned u = pEstimatedTaskTime.size(); u >= nTimeDifference_arg; u--)
				estTimeIter--;
		}else{ cout<<"Utrulig! Avslutter program: exit(-123); \nLagde jo nettop nok til å unngå denne case'en\nUtskrift av dette er en umulighet.\n"; 
			exit(-123);
		}

		//returnerer peiker til rett element
		return estTimeIter;
	} //}


	/********************************************************************************
	* void addTask_in_pEstimatedTaskTime( timeInterface*, unsigned) 				*
	* 	Argument: 	- timeInterface* som skal legges inn i pEstimatedTaskTime. 		*
	* 				- int nRelativeTime_arg: tid til task. 							*
	* 	Retur: 		void 															*
	********************************************************************************/
	// MERK: uRelativeTime_arg er 1 indeksert.  Gjør den 0-indeksert (antall iterasjoner etter neste) ?
	static inline void addTask_in_pEstimatedTaskTime_list( timeInterface* pTI_arg, int nRelativeTime_arg )
	{ //{2

		#if DEBUG_UTSKRIFTS_NIVAA > 3
		cout<<"addTask_in_pEstimatedTaskTime_list( [" <<pTI_arg->sClassName <<"] , " <<nRelativeTime_arg <<");\n";
		#endif
	
		if( nRelativeTime_arg <= 0 ) 
		{
			#if DEBUG_UTSKRIFTS_NIVAA > 1
	 		cout<<"addTask_in_pEstimatedTaskTime_list(X) med argumentet \t\t X <= 0 . \tReturnerer\n";
			#endif
			return;
		}
		#if DEBUG_UTSKRIFTS_NIVAA > 1
			cout<<"addTask_in_pEstimatedTaskTime_list( " <<nRelativeTime_arg <<" )\n\n";
		#endif


		// Lagrer "posisjon" (i pEstimatedTaskTime-lista) i pTI_arg->ul EstimatedTaskTime_for_object.
		pTI_arg->lEstimatedTaskTime_for_object = ulTidsiterasjoner + nRelativeTime_arg;

		// legg til element i denne vektorenPEIKEREN:
		(*get_iteration_in_pEstimatedTaskTime_list( nRelativeTime_arg ))->push_back( pTI_arg );

	} //}2


	// Utesta:
	static inline void removeTask_in_pEstimatedTaskTime_list( timeInterface* pTI_arg )
	{ //{
		std::list< std::list<timeInterface*>* >::iterator oldTimeEstimate_iterator = get_iteration_in_pEstimatedTaskTime_list( pTI_arg->lEstimatedTaskTime_for_object - time_class::getTid() );
		(*oldTimeEstimate_iterator)->remove( pTI_arg );
	} //}

	/********************************************************************************
	* 	void moveTask_in_pEstimatedTaskTime( timeInterface*, unsigned ) 			*
	* 	  Argument: - timeInterface* : peiker til aktuelle timeInterface objekt 	*
	* 				- unsigned uTid  : fremtidstidspunkt den skal flyttes til. 		*
	* 	  Retur: 	void 															*
	********************************************************************************/
	static inline void moveTask_in_pEstimatedTaskTime_list( timeInterface* pTI_arg, int nRelativeTime_arg ) 			//TODO TODO TODO Ikkje nRelativeTime_arg men uRelativeTime_arg! GJØR OM ! XXX
	{ //{2
		if( nRelativeTime_arg < 0 ) nRelativeTime_arg = 0;

		//#if DEBUG_UTSKRIFTS_NIVAA > 3
		//cout<<"moveTask_in_pEstimatedTaskTime(" <<pTI_arg->sClassName <<", " <<nRelativeTime_arg <<")\n";
		//#endif
	

		// Trenger å finne gamle plassen til [pTI_arg]
		/***************
		* PLAN:
		* 	- mest sannsynlige er at det nye estimatet ligger i nærleiken av gamle estimatet (som eg skal finne, og slette).
		* 	 	så vi bør begynne søket der. Veit ikkje om elementet ligger over eller under, så bør alternere mellom å sjekke element før og etter.o
		*
		* 	NEI: Bedre er det å lagre en unsigned uEstimertTaskTid i kvar timeInterface*. Denne kan brukes for å finne posisjon i pEstimatedTaskTime!
		*
		***************/


		// bruker timeInterface::lEstimated TaskTime_for_object og get_iteration_in_pEstimatedTaskTime():
		// Finner rett estimerte iterasjon: pIterator_in_pEstimatedTaskTime_temp  				(get_iteration_in_pEstimatedTaskTime() tar argument: tid til taskTime)
//		std::list<timeInterface*>::iterator pIterator_in_pEstimatedTaskTime_temp = (*get_iteration_in_pEstimatedTaskTime( pTI_arg->ul EstimatedTaskTime_for_object - time_class::getTid()  ))->begin();

		// Fjærner element med verdien [pTI_arg] fra lista i tidspunkt [timeInterface::ul EstimatedTaskTime_for_object]
//		pIterator_in_pEstimatedTaskTime_temp.remove( pTI_arg ); 
		//Dette kaller også destruktor, men dette blir ikkje problem siden element bare er en peiker..
		
		// TODO TODO OTDO TODO TODO Flytt neste sjekken inn i get_iteration_in_pEstimatedTaskTime() - funksjonen. Samme gjelder neste neste if-setning.
		//if(pTI_arg->lEstimatedTaskTime_for_object + time_class::getTid() < MAKS_LENGDE_PAA_pEstimatedTaskTime_JOBBER)
		{
			std::list< std::list<timeInterface*>* >::iterator oldTimeEstimate_iterator = get_iteration_in_pEstimatedTaskTime_list( pTI_arg->lEstimatedTaskTime_for_object - time_class::getTid() );  // XXX XXX sjå over XXX
			(*oldTimeEstimate_iterator)->remove( pTI_arg );
		}
	
		//#if DEBUG_UTSKRIFTS_NIVAA > 5
		//cout<<"Iterasjon i pEstimatedTaskTime: " <<((int)pTI_arg->lEstimatedTaskTime_for_object - (int)time_class::getTid()) <<"\t(lEstimatedTaskTime_for_object (" <<pTI_arg->lEstimatedTaskTime_for_object <<") - tid.getTid() ["
		//	<<time_class::getTid() <<"]\n";
		//#endif
		
		// Legger inn elementet på ny plass:
		// TODO: Dårlig: har ikkje feilsjekk!  ELLER KANSKJE. Orker ikkje meir no. Trur get_iteration_in_pEstimatedTaskTime() har feilsjekk/feilordning slik at det blir bra. 
		// TODO ER dette farlig? Kanskje.. ALDRI bruk iterator etter at størrelsen har blitt endra. Dette er kanskje greit? :
		// Gjelder bare for vector: "bruk aldri iterator etter at størrelsen har blitt endra.

		//newTimeEstimate_iterator får verdien til iteratoren som peiker på rett iterasjon.
		std::list< std::list<timeInterface*>* >::iterator newTimeEstimate_iterator = get_iteration_in_pEstimatedTaskTime_list( nRelativeTime_arg ); // Eller kanskje nRelativeTime_arg +1 ?
		(*newTimeEstimate_iterator)->push_back( pTI_arg );
		// TODO KANSKJE HELLER 
		// (*  get_iteration_in_pEstimatedTaskTime( pTI_arg->lEstimatedTaskTime_for_object - time_class::getTid() ))->push_back( pTI_arg );
		// eller noke..

		// oppdaterer pTI_arg->lEstimatedTaskTime_for_object 
		pTI_arg->lEstimatedTaskTime_for_object = time_class::getTid() + nRelativeTime_arg;


		
	//	#if DEBUG_UTSKRIFTS_NIVAA > 6
	//	int ni = 1;
	//	//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
	//	for( std::list<timeInterface*>::iterator it = (*oldTimeEstimate_iterator)->begin(); it != (*oldTimeEstimate_iterator)->end(); it++ )
	//	{
	// 		cerr<<"Element nr. " <<ni++ <<" er " <<(*it)->sClassName <<endl;
	//	}
	//	cerr<<"FERIDG\n\n";
	//
	//	cerr<<"\nFerdig med moveTask_in_pEstimatedTaskTime()   _ _ _ _ _ _ _ _ _ _ \n\n\n\n";
	//	#endif
	} //}2



	const static void TEST_skrivUt_pEstimatedTaskTime_list()
	{ //{2
		cout<<"Skriver ut pEstimatedTaskTime: \n";

		int nYtreIter = 0;
		int nIndreIter;
		// itererer gjennom ytre liste:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for(std::list< std::list<timeInterface*>* >::iterator ll_iter = pEstimatedTaskTime.begin(); 	ll_iter != pEstimatedTaskTime.end() ; 	ll_iter++ )
		{
			cout<<nYtreIter <<"\tpEstimatedTaskTime[" <<nYtreIter <<"] \t- - - - - - - - - - - - - - - - - - - - - - - - - - - - - +tid = " <<ulTidsiterasjoner+nYtreIter <<"\n";
	
			nIndreIter = 1;
			//itererer gjennom indre vector:
			
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
			for(std::list<timeInterface*>::iterator listIter = (*ll_iter)->begin() ; listIter != (*ll_iter)->end(); listIter++ )
			{
		 		cout<<"\t\telement nr. " <<nIndreIter++ <<":  \t" <<(*listIter)->sClassName <<"     \t lEstimatedTaskTime_for_object :  " 
					<<(*listIter)->lEstimatedTaskTime_for_object <<endl;
			}
	
			nYtreIter++;
		}
		cout<<"\n\n";
	} //}2
//}
#endif

	//static std::list<timeInterface*> pWorkTaskQue;
	const static void TEST_skrivUt_pWorkTaskQue()
	{ //{
		cout<<"Skriver ut pWorkTaskQue: \n";
		int nIter = 0;
		// itererer gjennom ytre liste:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for(std::list<timeInterface*>::iterator l_iter = pWorkTaskQue.begin(); 	l_iter != pWorkTaskQue.end() ; 	l_iter++ )
		{
			cout<<nIter <<"\t" <<(*l_iter)->sClassName <<endl;
			nIter++;
		}
		cout<<"\n\n";
	} //}


	public:
	time_class() : timeInterface("time"){}


	static void addTaskIn_pWorkTaskQue( timeInterface* pArg )
	{
	 	pWorkTaskQue.push_back( pArg );
	}
	static const inline unsigned long getTid(){ return ulTidsiterasjoner; }
	//Noke slikt: XXX 	friend schedulerFunksjon;
	// Viktig med inkapsling!

	// funker ikkje: friend class timeInterface; XX X test igjen. Har gjort det. Funker ikkje..
	friend class K_auron;
	friend class i_auron;
	friend class i_axon; 		// Usikker om dette funker. Alternativt skriv inn kvar klasse (som under)
	//friend class s_axon;

	friend void initialiserArbeidsKoe();
	friend void* taskSchedulerFunction(void*);
	
	friend int main(int, char**);


	friend void testfunk();
};





#endif
// vim:fdm=marker:fmr=//{,//}
