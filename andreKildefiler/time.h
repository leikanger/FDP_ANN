

#ifndef TID_H_
#define TID_H_

// TODO sjekk om desse er nødvendig (kommenter ut, når kompilering funker..)
//#include "main.h"
#include "../neuroElements/auron.h"

using std::cout;
using std::endl;

// DEKLARASJONER:
//extern unsigned long ulTidsiterasjoner;


// BARE FOR TESTING! TODO fjærn neste linja:
class i_auron;


/****************************************
***      abstract class !             ***
****************************************/
class timeInterface
{
	public:
	timeInterface(std::string s) : ulEstimatedTaskTime_for_object(0), sClassName(s){}
	timeInterface() 				{} 				//For mens eg itererer i utviklinga.  Trur ikkje eg skal ha denne etterpå..

	// SKAL VÆRE private ? XXX XXX XXX
	virtual void doTask() =0;
	virtual void doCalculation() =0;

	unsigned long ulEstimatedTaskTime_for_object; //Brukes for når man skal flytte ledd i pEstimatedTaskTime. Veldig viktig for KANN!

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
	// Eller kanskje ikkje.. :
	static std::list< std::list<timeInterface*>* > pEstimatedTaskTime;


	
	protected:
	inline void doTask()
	{ 	//{ 

		// Bare debugging--utskrift:
		//if(pCalculatationTaskQue.empty()) cout<<"TOM pCalculatationTaskQue\n"; // BARE debugging! XXX Fjærn tilslutt..

		// gjennomfører planlagte kalkulasjoner:
		doCalculation();
	



		// pEstimatedTaskTime opplegg ************* 	****** 		********* 		*************  ************************************

		if( ! pEstimatedTaskTime.front()->empty() ){
			cout<<"Planlagt oppgave i tid: " <<time_class::getTid() <<"\n\n";
		}
		
		timeInterface* pTI_temp;
		while( ! (pEstimatedTaskTime.front())->empty() ) // Kjør alle element i FØRSTE element av pEstimatedTaskTime.
		{

			// pTI_temp peiker får verdien til første timeInterface*-element i pEstimatedTaskTime.front()->front():
			pTI_temp = (pEstimatedTaskTime.front())->front();
//cout<<"H1 før segfault i SANN\n";
			cout<<"pEstimatedTaskTime fører til at eg legger til " <<pTI_temp->sClassName <<" i pWorkTaskQue\n";
			pWorkTaskQue.push_back( pTI_temp ); //Legger til første element av første liste i pWorkTaskQue
			// legg elementet i pEstimatedTaskTime om [periode] tid
			
			// fjærner element fra { pEstimatedTaskTime.[dette tidssteget] }
			(pEstimatedTaskTime.front()) ->pop_front(); 			//Fjærn første element (av første lista).

			// Legger til element om [periode] tidsiterasjoner. TODO TODO 													XXX SJEKK AT DETTE ER GJORDT
			// NEI: dette er bare for regelmessige oppgaver. M.a.o klassespesifikt. Gjøres i klassene! 		For K_auron:
			//time_class::addTask_in_pEstimatedTaskTime( pTI_temp, pTI_temp->uLastCalculatedPeriod_promille );

		
//cout<<"H2\n";
		}
		
		// Organiserer pEstimatedTaskTime. Fjærner første ledd:
			// free minnet fra førte element (ligger nemmelig i frie lageret)
		delete pEstimatedTaskTime.front();  // XXX XXX XXX Blir dette rett? XXX (laga den med new std::list<timeInterface*> (den er bare en peiker til list<> i frie lageret..))
			// pop element (ta vekk fra liste)
		pEstimatedTaskTime.pop_front(); //Fjærn heile lista med estimerte oppgaver for neste time_iteration.

		// Undersøker om lengden på lista er kortere enn MIN_LENGDE_PAA_pEstimatedTaskTime:
		//cout<<"pEstimatedTaskTime.Size() = " <<pEstimatedTaskTime.size() <<endl;

		// Legger inn MIN_LENGDE_PAA_pEstimatedTaskTime antall element.
		if(pEstimatedTaskTime.size() < MIN_LENGDE_PAA_pEstimatedTaskTime )
		{
			for( int i=0; i<MIN_LENGDE_PAA_pEstimatedTaskTime; i++)
				pEstimatedTaskTime.push_back( new std::list<timeInterface*> );
		}
			
		// pEstimatedTaskTime opplegg ************* 	****** 		********* 		*************  ************************************



		// Legger til egenpeiker på slutt av pNesteJobb_ArbeidsKoe
		pWorkTaskQue.push_back(this);	
		//itererer time:
		ulTidsiterasjoner++;
		
		// utskrift:
		if(ulTidsiterasjoner%100 == 0)		
			cout<<"\t_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n"
		 		<<"\t* * * * TID: \t  =  " <<ulTidsiterasjoner <<" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * = "
				<<ulTidsiterasjoner <<"\n";
	}//}
	inline void doCalculation()
	{ //{
		/**************************************************************************************
		*** Gjennomføere kalkulering på alle kalkuleringsoppgaver (pCalculatationTaskQue) 	***
		**************************************************************************************/

		if( pCalculatationTaskQue.empty() ) return;

		// Organiserer liste slik at kvar oppføring er unik:
		for( std::list<timeInterface*>::iterator iter = pCalculatationTaskQue.begin(); iter != pCalculatationTaskQue.end(); iter++ )
		{
			// ???: gjør det heller slik: while( ! pCalculatationTaskQue.empty() ){ ... }

			// Testet. Virker som det funker no.
			std::list<timeInterface*>::iterator iter2 = iter; 
			iter2++;
			while(iter2!=pCalculatationTaskQue.end()){
				// ser om iteratorene peker til samme minneadresse (samme timeInterface-element). Isåfall: fjærn det andre elementet.
			 	if( (*iter2) == (*iter) ){ //cout<<"Var duplikat. Fjærner andre element.\n";
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
			cout<<"Kaller " <<pCalculatationTaskQue.front()->sClassName <<".doCalculation()\n";

			pCalculatationTaskQue.front()->doCalculation();
			pCalculatationTaskQue.pop_front();
		
		}
	} //}


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
* 		Så: implementere i time_class::doTask() at den skal gjøre alle jobbene i [neste-tids-iter]-list i pEstimatedTaskTime (for så å ta vekk denne [neste-tidIter]-list i pEstimatedTaskTime.
* 			- kvart element skal sjølv være ansvarlig for å flytte seg i pEstimatedTaskTime etter AP.
*
****************************************************************************/


	static inline std::list< std::list<timeInterface*>* >::iterator   get_iteration_in_pEstimatedTaskTime( unsigned uTimeDifference_arg )
	{
		// Dersom estimert tid er utafor lista av estimert tid: auk lista.
		int nDiff = uTimeDifference_arg - pEstimatedTaskTime.size()  ;
		if( nDiff >= 0 ) //Mangler ledd. Legg til rett antall.
		{
//			cout<<"Mangler ledd. Legger til de manglende ledda. Mangler " <<nDiff <<" ledd.\n";
			for(int i=0; i <= nDiff; i++){
				pEstimatedTaskTime.push_back( new std::list<timeInterface*> );
			}
		}

		std::list< std::list<timeInterface*>* >::iterator estTimeIter;

		// Finne rett plassering (skal returneres)
		if( uTimeDifference_arg < (pEstimatedTaskTime.size())/2 ){ // plusser på en (/2) for å få rett avrunding..
			//Søk fra begynnelsen.
			estTimeIter = pEstimatedTaskTime.begin();
/*
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
Veit ikkje om for() er bra under. Trur det er broblemer med index. Var (fra 0;<=;++) blei (fra 1; < ; ++)

Meir: Veit ikkje om den som søker fra oppsida er rett heller. No er eg jævla sliten!

*/
			for(unsigned u=1; u < uTimeDifference_arg; u++)
				estTimeIter++;
// gjorde <= om til < . Veit ikkje om det blir rett. TEST ZZZ XXX XXX XXX XXX XXX
		}else if(uTimeDifference_arg <= pEstimatedTaskTime.size() ){ 
			//Søk fra slutten:
			estTimeIter = (pEstimatedTaskTime.end());
			for(unsigned u = pEstimatedTaskTime.size(); u >= uTimeDifference_arg; u--)
				estTimeIter--;
		}else{ cout<<"ERROR! exit(-123); \nFeilmelding: asdf13@time.h\n"; 
			exit(-123);
		}

		//returnerer peiker til rett element
		return estTimeIter;
	}

	// MERK: uRelativeTime_arg er 1 indeksert. TODO gjør den 0-indeksert (antall iterasjoner etter neste)
	static inline void addTask_in_pEstimatedTaskTime( timeInterface* pTI_arg, unsigned uRelativeTime_arg )
	{ //{2
		cout<<"addTask_in_pEstimatedTaskTime(" <<pTI_arg->sClassName <<", " <<uRelativeTime_arg <<");\n";



		// Lagrer "posisjon" (i pEstimatedTaskTime-lista) i pTI_arg->ul EstimatedTaskTime_for_object.
		pTI_arg->ulEstimatedTaskTime_for_object = ulTidsiterasjoner + uRelativeTime_arg;


		// legg til element i denne vektorenPEIKEREN:
		(*get_iteration_in_pEstimatedTaskTime( uRelativeTime_arg ))->push_back( pTI_arg );

	} //}2

	static inline void moveTask_in_pEstimatedTaskTime( timeInterface* pTI_arg, unsigned uRelativeTime_arg )
	{ //{2
		cout<<"moveTask_in_pEstimatedTaskTime(" <<pTI_arg->sClassName <<", " <<uRelativeTime_arg <<")\n";
	
		//newTimeEstimate_iterator får verdien til iteratoren som peiker på rett iterasjon.
		std::list< std::list<timeInterface*>* >::iterator newTimeEstimate_iterator = get_iteration_in_pEstimatedTaskTime( uRelativeTime_arg );

		// Trenger å finne gamle plassen til [pTI_arg]
		/***************
		* PLAN:
		* 	- mest sannsynlige er at det nye estimatet ligger i nærleiken av gamle estimatet (som eg skal finne, og slette).
		* 	 	så vi bør begynne søket der. Veit ikkje om elementet ligger over eller under, så bør alternere mellom å sjekke element før og etter.o
		*
		* 	NEI: Bedre er det å lagre en unsigned uEstimertTaskTid i kvar timeInterface*. Denne kan brukes for å finne posisjon i pEstimatedTaskTime!
		*
		***************/


//static inline std::list< std::list<timeInterface*>* >::iterator   get_iteration_in_pEstimatedTaskTime( unsigned uTimeDifference_arg )

		// bruker timeInterface::ulEstimated TaskTime_for_object og get_iteration_in_pEstimatedTaskTime():
		// Finner rett estimerte iterasjon: pIterator_in_pEstimatedTaskTime_temp  				(get_iteration_in_pEstimatedTaskTime() tar argument: tid til taskTime)
//		std::list<timeInterface*>::iterator pIterator_in_pEstimatedTaskTime_temp = (*get_iteration_in_pEstimatedTaskTime( pTI_arg->ul EstimatedTaskTime_for_object - time_class::getTid()  ))->begin();

		// Fjærner element med verdien [pTI_arg] fra lista i tidspunkt [timeInterface::ul EstimatedTaskTime_for_object]
//		pIterator_in_pEstimatedTaskTime_temp.remove( pTI_arg ); 
		//Dette kaller også destruktor, men dette blir ikkje problem siden element bare er en peiker..
		
		// TODO Vettafaen om neste linje er naudsynt? TODO sjå gjennom koden neste linjene!
		std::list< std::list<timeInterface*>* >::iterator oldTimeEstimate_iterator = get_iteration_in_pEstimatedTaskTime( pTI_arg->ulEstimatedTaskTime_for_object - time_class::getTid() );
		cout<<"Iterasjon i pEstimatedTaskTime: " << pTI_arg->ulEstimatedTaskTime_for_object - time_class::getTid() <<endl;
		(*oldTimeEstimate_iterator)->remove( pTI_arg );
		// Legger inn elementet på ny plass:
		// TODO: Dårlig: har ikkje feilsjekk!  ELLER KANSKJE. Orker ikkje meir no. Trur get_iteration_in_pEstimatedTaskTime() har feilsjekk/feilordning slik at det blir bra. 
		// KAnskje dette er bra? :
		// TODO ER dette farlig? Kanskje.. ALDRI bruk iterator etter at størrelsen har blitt endra. Dette er kanskje greit? :
		(*newTimeEstimate_iterator)->push_back( pTI_arg );
		// TODO KANSKJE HELLER 
		// (*  get_iteration_in_pEstimatedTaskTime( pTI_arg->ulEstimatedTaskTime_for_object - time_class::getTid() ))->push_back( pTI_arg );
		// eller noke..

		// oppdaterer pTI_arg->ulEstimatedTaskTime_for_object 
		pTI_arg->ulEstimatedTaskTime_for_object = time_class::getTid() + uRelativeTime_arg;





		cout<<"Skriver ut TEST. Lengde: " <<(*oldTimeEstimate_iterator)->size() <<"\n\n\n";
		int ni = 1;
		for( std::list<timeInterface*>::iterator it = (*oldTimeEstimate_iterator)->begin(); it != (*oldTimeEstimate_iterator)->end(); it++ )
		{
	 		cout<<"Element nr. " <<ni++ <<" er " <<(*it)->sClassName <<endl;
		}
		cout<<"FERIDG\n";

		cout<<"Tester å finne lengde på liste 0. Antall element: " <<(*get_iteration_in_pEstimatedTaskTime(0))->size() <<"\n\n";
		cout<<"Tester å finne lengde på liste 1. Antall element: " <<(*get_iteration_in_pEstimatedTaskTime(1))->size() <<"\n\n";
		cout<<"Tester å finne lengde på liste 2. Antall element: " <<(*get_iteration_in_pEstimatedTaskTime(2))->size() <<"\n\n";
		cout<<"Tester å finne lengde på liste 3. Antall element: " <<(*get_iteration_in_pEstimatedTaskTime(3))->size() <<"\n\n";
		cout<<"Tester å finne lengde på liste 4. Antall element: " <<(*get_iteration_in_pEstimatedTaskTime(4))->size() <<"\n\n";


		/* TODO TODO TODO UNØDVENDIG: slett! : 
		//{ UTKOMMENTERT
		// Søker gjennom denne etter rett timeInterface element:
		for( std::list<timeInterface*>::iterator  iterElement = pIterator_in_pEstimatedTaskTime_temp->begin();  iterElement != pIterator_in_pEstimatedTaskTime_temp.end(); iterElement++ )
		{
			// iterElement er iterator og pTI_arg er peiker. Må derfor dereferere de for å sammenligne
	 		if( (*iterElement) == (*pTI_arg) )
			{
	 			// Har funnet pTI_arg! Fjærner det fra pEstimatedTaskTime.[gammel tid] (skal legge til på ny pos. etterpå.
				
			}
			// husk å oppdatere timeInterface::ul EstimatedTaskTime_for_object
		}
*/ //}
	} //}2

	const static void TEST_skrivUt_pEstimatedTaskTime()
	{ //{2
		cout<<"Skriver ut pEstimatedTaskTime: \n";
		int nYtreIter = 1;
		int nIndreIter;
		// itererer gjennom ytre liste:
		for(std::list< std::list<timeInterface*>* >::iterator ll_iter = pEstimatedTaskTime.begin(); 	ll_iter != pEstimatedTaskTime.end() ; 	ll_iter++ )
		{
			cout<<nYtreIter <<"\tpEstimatedTaskTime[" <<nYtreIter <<"] \t- - - - - - - - - - - - - - - - - - - - - - - - - - - - - " <<"\n";
	
			nIndreIter = 1;
			//itererer gjennom indre vector:
			
			for(std::list<timeInterface*>::iterator listIter = (*ll_iter)->begin() ; listIter != (*ll_iter)->end(); listIter++ )
			{
		 		cout<<"\t\telement nr. " <<nIndreIter++ <<":  \t" <<(*listIter)->sClassName <<"     \t ulEstimatedTaskTime_for_object :  " <<(*listIter)->ulEstimatedTaskTime_for_object 
					<<"\tPeiker: " <<*listIter <<endl;
			}
	
			nYtreIter++;
		}
		cout<<"\n\n";
	} //}2

	public:
	time_class() : timeInterface("time"){}


	static void leggTilTask( timeInterface* pArg )
	{
	 	pWorkTaskQue.push_back( pArg );
	}
	static const unsigned long getTid(){ return ulTidsiterasjoner; }
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
