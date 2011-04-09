

#ifndef TID_H_
#define TID_H_

// TODO sjekk om desse er nødvendig (kommenter ut, når kompilering funker..)
//#include "main.h"
#include "../neuroElements/auron.h"

using std::cout;
using std::endl;

// DEKLARASJONER:
//extern unsigned long ulTidsiterasjoner;
class s_auron;
// Flytta vekk herfra:
//extern std::list<timeInterface*> time_class::pTaskArbeidsKoe_List;
//extern unsigned long time_class::ulTidsiterasjoner;


/****************************************
***      abstract class !             ***
****************************************/
class timeInterface
{
	public:
	timeInterface(std::string s) : klasseNavn(s){}
	timeInterface() 				{} 				//For mens eg itererer i utviklinga.  Trur ikkje eg skal ha denne etterpå..

	virtual void doTask() =0;

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
	
	static std::list<timeInterface*> pTaskArbeidsKoe_List;

	
	protected:
	inline void doTask(){ 	//overlagring av timeInterface::doTask() - som med de andre klassene som arver timeInterface..
		// Legger til egenpeiker på slutt av pNesteJobb_ArbeidsKoe
		pTaskArbeidsKoe_List.push_back(this);	

		//itererer time:
		ulTidsiterasjoner++;
		
		cout<<"\n\tAUKER TID: \t" <<ulTidsiterasjoner-1 <<" => " <<ulTidsiterasjoner <<" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * "
			<<ulTidsiterasjoner <<" * \n\n";
		
		// utskrift:
		//cout<<"Auker time til " <<ulTidsiterasjoner <<endl;
	}

	public:
	time_class() : timeInterface("time"){}


	static void leggTilTask( timeInterface* pArg )
	{
	 	pTaskArbeidsKoe_List.push_back( pArg );
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

	friend void testFunksjon_slett(s_auron*);
};





#endif
