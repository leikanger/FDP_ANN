

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
	timeInterface(std::string s) : sClassName(s){}
	timeInterface() 				{} 				//For mens eg itererer i utviklinga.  Trur ikkje eg skal ha denne etterpå..

	// SKAL VÆRE private ? XXX XXX XXX
	virtual void doTask() =0;
	virtual void doCalculation() =0;

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
	


		// Sjekk etter (og gjennomfør) tasks i pEstimatedTaskTime for neste tidsiter: *************** *******************************

		if( ! pEstimatedTaskTime.front()->empty() ){
			cout<<"Planlagt oppgave i tid: " <<time_class::getTid() <<endl;
			cout<<"Skriver ut pEstimatedTaskTime, før:\n";
			TEST_skrivUt_pEstimatedTaskTime();
			cout<<"\n\n";
		}
		while( ! pEstimatedTaskTime.front()->empty() ) // Fjør alle element i første element av pEstimatedTaskTime.
		{

// 			cout<<"pEstimatedTaskTime er ikkje .empty()\t" <<"(pEstimatedTaskTime->front()).size = " <<pEstimatedTaskTime.front()->size() 
//			 	<<"\t.. legger til timeInterface* i pWorkTaskQue, av type " <<(pEstimatedTaskTime.front())->front()->sClassName <<endl;

//cout<<"H1 før segfault i SANN\n";
			cout<<"pEstimatedTaskTime fører til at eg legger til " <<pEstimatedTaskTime.front()->front()->sClassName <<" i pWorkTaskQue\n";
			pWorkTaskQue.push_back( (pEstimatedTaskTime.front())->front() ); //Legger til første element av første liste i pWorkTaskQue
			(pEstimatedTaskTime.front()) ->pop_front(); 			//Fjærn første element (av første lista).
		
//cout<<"H2\n";
		}
		
		// Organiserer pEstimatedTaskTime. Fjærner første ledd:
		delete pEstimatedTaskTime.front();  // XXX XXX XXX Blir dette rett? XXX (laga den med new std::list<timeInterface*> (den er bare en peiker til list<> i frie lageret..))
		pEstimatedTaskTime.pop_front(); //Fjærn heile lista med estimerte oppgaver for neste time_iteration.

		// Undersøker om lengden på lista er kortere enn MIN_LENGDE_PAA_pEstimatedTaskTime:
		//cout<<"pEstimatedTaskTime.Size() = " <<pEstimatedTaskTime.size() <<endl;

		// Legger inn MIN_LENGDE_PAA_pEstimatedTaskTime antall element.
		if(pEstimatedTaskTime.size() < MIN_LENGDE_PAA_pEstimatedTaskTime )
		{
			//cout<<"legg til fleire element\n";

	 		//Legger til fleire element:
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
			cout<<"\tAUKER TID: \t" <<ulTidsiterasjoner-1 <<" => " <<ulTidsiterasjoner <<" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * "
				<<ulTidsiterasjoner <<" * \n";
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


	// MERK: uRelativeTime_arg er 1 indeksert. TODO gjør den 0-indeksert (antall iterasjoner etter neste)
	static inline void addTaskIn_pEstimatedTaskTime( timeInterface* pTI_arg, unsigned uRelativeTime_arg )
	{ //{2

		// XXX TEST: uRelativeTime_arg = 5;

		// Dersom estimert tid er utafor lista av estimert tid: auk lista.
		//if( uRelativeTime_arg > pEstimatedTaskTime.size() )
		int nDiff = uRelativeTime_arg - pEstimatedTaskTime.size()  ; // Begynner neste tidsiter: grunn til "+1"
		if( nDiff >= 0 ) //Mangler ledd. Legg til rett antall.
		{
//			cout<<"Mangler ledd. Legger til de manglende ledda. Mangler " <<nDiff <<" ledd.\n";
			for(int i=0; i <= nDiff; i++){
				pEstimatedTaskTime.push_back( new std::list<timeInterface*> );
			}
		}


		
		std::list< std::list<timeInterface*>* >::iterator estTimeIter;

		// Finne rett plassering, og legge til task der:
		if( uRelativeTime_arg < (pEstimatedTaskTime.size())/2 ){ // plusser på en (/2) for å få rett avrunding..
//			cout<<"SØKER FRA BEGYNNELSEN\t(uRelativeTime_arg = " <<uRelativeTime_arg <<"\t, size() = " <<pEstimatedTaskTime.size() <<")\n";

			//Søk fra begynnelsen.
			estTimeIter = pEstimatedTaskTime.begin();
			for(unsigned u=0; u <= uRelativeTime_arg; u++)
				estTimeIter++;
		}else{
//			cout<<"SØKER FRA SLUTTEN    \t(uRelativeTime_arg = " <<uRelativeTime_arg <<"\t, size() = " <<pEstimatedTaskTime.size() <<")\n";

			//Søk fra slutten:
			estTimeIter = (pEstimatedTaskTime.end())--;
			for(unsigned u = pEstimatedTaskTime.size(); u >= uRelativeTime_arg; u--)
				estTimeIter--;
		}/**/

		// legg til element i denne vektorenPEIKEREN:
		(*estTimeIter)->push_back( pTI_arg );
	} //}2

	static void TEST_skrivUt_pEstimatedTaskTime()
	{ //{2
		cout<<"Skriver ut pEstimatedTaskTime: \n";
		int nYtreIter = 1;
		int nIndreIter;
		// itererer gjennom ytre liste:
		for(std::list< std::list<timeInterface*>* >::iterator ll_iter = pEstimatedTaskTime.begin(); 	ll_iter != pEstimatedTaskTime.end() ; 	ll_iter++ )
		{
			//cout<<"\tNeste ytreIter: " <<nYtreIter <<std::endl;
	
			nIndreIter = 1;
			//itererer gjennom indre vector:
			std::list<timeInterface*>::iterator listIter = (*ll_iter)->begin();
			for( ; listIter != (*ll_iter)->end(); listIter++ )
			{
		 		cout<<"\t\tElement [" <<nYtreIter <<", " <<nIndreIter++ <<"]:\t" <<(*listIter)->sClassName <<endl;
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
