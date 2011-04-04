

#ifndef TID_H_
#define TID_H_

// TODO sjekk om desse er nødvendig (kommenter ut, når kompilering funker..)
//#include "main.h"
#include "../neuroElements/auron.h"

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







/**************************TODO : ************

Ordne slik at alle element som legges inn sjekker om andre identiske element eksisterer. I så fall skal de andre fjærnes.
Typisk om en dendrite legges inn etter ei synapses overføring, så legges samme dendritt til for omregning etter ei til overføring samme tidsiterasjon. I dette fallet skal bare en av de ligge i lista. (samme hvilken?)
	NEINEINEI: Det som bare skal skje en gang er lekkasjen. Begge overføringer skal selvfølgelig inn!

Kvar gang det er overføring:
	- kjøre depol.-lekkasje dersom det er en eller fleire iter siden forrige gang.
	- plusse på ny depol.endring.

**********************************************/


/*********************************************************************************
** 	class tid  																	**
** 		- tid skal bare være en instans av denne klassen (i planen så langt). 		**
** 		 																		**
*********************************************************************************/
class tid : public tidInterface {
	static unsigned long ulTidsiterasjoner;
	
	static std::list<tidInterface*> pTaskArbeidsKoe_List;

	
	protected:
	inline void doTask(){ 	//overlagring av tidInterface::doTask() - som med axon, auron, dendritt, synapse osv.
		// Legger til egenpeiker på slutt av pNesteJobb_ArbeidsKoe
		pTaskArbeidsKoe_List.push_back(this);	

		//itererer tid:
		ulTidsiterasjoner++;
		
		cout<<"\n\nAUKER TID: \t" <<ulTidsiterasjoner-1 <<" => " <<ulTidsiterasjoner <<" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n";
		
		// utskrift:
		//cout<<"Auker tid til " <<ulTidsiterasjoner <<endl;
	}

	public:
	tid() : tidInterface("tid"){}


	static void leggTilTask( tidInterface* pArg )
	{
	 	pTaskArbeidsKoe_List.push_back( pArg );
	}
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
