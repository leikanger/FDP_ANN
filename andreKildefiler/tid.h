

#ifndef TID_H_
#define TID_H_

// TODO sjekk om desse er nødvendig (kommenter ut, når kompilering funker..)
//#include "main.h"
#include "../auron/auron.h"

using std::cout;
using std::endl;

// DEKLARASJONER:
//extern unsigned long ulTidsiterasjoner;
class auron;
// Flytta vekk herfra:
//extern std::list<tidInterface*> tid::pTaskArbeidsKoe_List;
//extern unsigned long tid::ulTidsiterasjoner;


/****************************************
***      abstract class !             ***
****************************************/
class tidInterface
{
	public:
	tidInterface(std::string s) : klasseNavn(s){}
	tidInterface() 				{} 				//For mens eg itererer i utviklinga.  Trur ikkje eg skal ha denne etterpå..

	virtual void doTask() =0;

	//for debugging:
	std::string klasseNavn;
};


// XXX NY IDE:
// Eg kan lage lokal tid for kvart auron, for å kunne kjøre K(1-e^{-at})+v_0 e^{-at} lettare. Ved oppdatering av K (eller fyring) kan lokal tid settes til 0.
// I så fall bør eg ha en peiker tid::masterTid som peiker på pHovedskille = mew tid; laga i void initialiserArbeidsKoe();
// 	Dette krever forresten meir: trenger en static peiker til hoved-tid, men f.eks. unsigned long ulTidsiterasjoner kan ikkje være static. For å finne denne må man da kalle tid::hovedTid.ulTidsiterasjoner;
// 	pTaskArbeidsKoe_List kan fortsatt være static!

/*********************************************************************************
** 	class tid  																	**
** 		- tid skal bare være en instans av denne klassen (i planen så langt). 		**
** 		 																		**
*********************************************************************************/
class tid : public tidInterface {
	static unsigned long ulTidsiterasjoner;
	
	protected:
	static std::list<tidInterface*> pTaskArbeidsKoe_List;

	inline void doTask(){ 	//overlagring av tidInterface::doTask() - som med axon, soma, dendritt, synapse osv.
		// Legger til egenpeiker på slutt av pNesteJobb_ArbeidsKoe
		pTaskArbeidsKoe_List.push_back(this);	

		//itererer tid:
		ulTidsiterasjoner++;
		
		// utskrift:
		cout<<"Auker tid til " <<ulTidsiterasjoner <<endl;
	}

	public:
	tid() : tidInterface("tid"){}

	static unsigned long getTid(){ return ulTidsiterasjoner; }
	//Noke slikt: XXX 	friend schedulerFunksjon;
	// Viktig med inkapsling!

	// funker ikkje: friend class tidInterface; XXX TODO test igjen.
	friend class auron;
	friend class axon;
	//friend void auron::doTask();
	friend void initialiserArbeidsKoe();
	friend void* taskSchedulerFunction(void*);
	
	friend int main(int, char**);

	friend void testFunksjon_slett(auron*);
};





#endif
