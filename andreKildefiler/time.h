

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
	timeInterface(std::string s) : klasseNavn(s){}
	timeInterface() 				{} 				//For mens eg itererer i utviklinga.  Trur ikkje eg skal ha denne etterpå..

	// SKAL VÆRE private ? XXX XXX XXX
	virtual void doTask() =0;
	virtual void doCalculations() =0;

	//for debugging:
	std::string klasseNavn;
};







/**************************TODO : ************

Ordne slik at alle element som legges inn sjekker om andre identiske element eksisterer. I så fall skal de andre fjærnes.
Typisk om en dendrite legges inn etter ei synapses overføring, så legges samme dendritt til for omregning etter ei til overføring samme timesiterasjon. I dette fallet skal bare en av de ligge i lista. (samme hvilken?)
	NEINEINEI: Det som bare skal skje en gang er lekkasjen. Begge overføringer skal selvfølgelig inn!

Kvar gang det er overføring:
	- kjøre depol.-lekkasje dersom det er en eller fleire iter siden forrige gang.
	- plusse på ny depol.endring.

**********************************************/


/*********************************************************************************
** 	class time  																	**
** 		- time skal bare være en instans av denne klassen (i planen så langt). 		**
** 		 																		**
*********************************************************************************/
class time_class : public timeInterface {
	static unsigned long ulTidsiterasjoner;
	
	// TODO Endre neste til std::que ? XXX Sjå stroustrup s.576
	static std::list<timeInterface*> pWorkTaskQue;
	// Kanskje bedre å gå over til vector, og gå gjennom lista og sjå etter duplikat i time_class::doCalculations()
	static std::list<timeInterface*> pCalculatationTaskQue;
	// std::set er en container der key og value er det samme. Unique elements!

	static std::multimap<unsigned long, timeInterface*> pEstimatedTaskTime; //XXX HER NYNYNY NY XXX

	
	protected:
	inline void doTask(){ 	//overlagring av timeInterface::doTask() - som med de andre klassene som arver timeInterface..
		// Legger til egenpeiker på slutt av pNesteJobb_ArbeidsKoe
		pWorkTaskQue.push_back(this);	

		// Bare debugging--utskrift:
		if(pCalculatationTaskQue.empty()) cout<<"TOM pCalculatationTaskQue!\n"; // BARE debugging! XXX Fjærn tilslutt..
		
		// gjennomfører planlagte kalkulasjoner:
		doCalculations();


		//itererer time:
		ulTidsiterasjoner++;
		
		// utskrift:
		cout<<"\n\tAUKER TID: \t" <<ulTidsiterasjoner-1 <<" => " <<ulTidsiterasjoner <<" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * "
			<<ulTidsiterasjoner <<" * \n\n";
	}
	inline void doCalculations()
	{
		// Organiserer liste slik at kvar oppføring er unik:
		for( std::list<timeInterface*>::iterator iter = pCalculatationTaskQue.begin(); iter != pCalculatationTaskQue.end(); iter++ )
		{
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
			// Kaller pCalculatationTaskQue.from()->pCalculatationTaskQue();
			pCalculatationTaskQue.front()->doCalculations();
			pCalculatationTaskQue.pop_front();
		
		}
	}
	

	public:
	time_class() : timeInterface("time"){}


	static void leggTilTask( timeInterface* pArg )
	{
	 	pWorkTaskQue.push_back( pArg );
	}
	static unsigned long getTid(){ return ulTidsiterasjoner; }
	//Noke slikt: XXX 	friend schedulerFunksjon;
	// Viktig med inkapsling!

	// funker ikkje: friend class timeInterface; XXX TODO test igjen.
	friend class i_auron;
	friend class i_axon; 		// Usikker om dette funker. Alternativt skriv inn kvar klasse (som under)
	//friend class s_axon;

	friend void initialiserArbeidsKoe();
	friend void* taskSchedulerFunction(void*);
	
	friend int main(int, char**);


	friend void testfunk();
};





#endif
