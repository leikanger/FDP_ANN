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
	timeInterface(std::string s) : ulEstimatedTaskTime_for_object(0), sClassName(s){}
	timeInterface() 				{} 				//For mens eg itererer i utviklinga.  Trur ikkje eg skal ha denne etterpå..

	virtual void doTask() =0;
	virtual void doCalculation() =0;

	unsigned long ulEstimatedTaskTime_for_object; 


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

	// Liste som sjekkes ved kvar tidsiterering. Dersom eit element har ulEstimatedTaskTime_for_object til neste tidssted legges peiker inn i pWorkTaskQue.
	static std::list<timeInterface*> pPeriodicElements;
	// Sjå også public: addElementIn_pPeriodicElements()

	static unsigned long ulNumberOfCallsTo_doTask;

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

	static void addElementIn_pPeriodicElements( timeInterface* pArg )
	{
		pPeriodicElements.push_back( pArg );
	}
	static void addTaskIn_pWorkTaskQue( timeInterface* pArg )
	{
	 	pWorkTaskQue.push_back( pArg );
	}
	static const inline unsigned long getTid(){ return ulTidsiterasjoner; }
	
	static const void skrivUt_pPeriodicElements()
	{
		cout<<"Skriver ut pPeriodicElements-lista:\n";
		for( std::list<timeInterface*>::iterator pPE_iter = pPeriodicElements.begin() ; pPE_iter != pPeriodicElements.end() ; pPE_iter++ )
		{
			cout<<"[ " <<(*pPE_iter)->sClassName <<" ]\n";
		}
		cout<<"\n\n";
	}

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
