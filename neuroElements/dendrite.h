
#ifndef DENDRITE_H_
#define DENDRITE_H_


#include "../andreKildefiler/time.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
//class synapse;
//class auron;
//class timeInterface;

/*****************************************************************
** class dendritt 			-  	Notater:
** 	
** 		- dendritt må ha aktivitetsVariabel.
**
*****************************************************************/
class dendrite : public timeInterface{
	inline void doTask();

	auron* pElementAvAuron;
	std::list<synapse*> pInnSynapser;

	inline void newInputSignal( int nNewSignal );
	inline void calculateLeakage();

	bool bBlokkerInput_refractionTime; //Blokkere input når refraction period eller når depol er over terskel.

	public:
	//Constructor: 	
	dendrite( auron* pPostSynAuron_Arg ); // : pElementAvAuron(pPostSynAuron_Arg)  		Definisjon i neuroElement.cpp
	~dendrite();
	
	//Destructor:
	/* kommenterer ut mens synapser ikkje er laga enda.. //{
	~sDendritt()
	{
		// destruerer alle innsynapser:
	 	while( !pInnSynapser.empty() )
		{
	 		cout<<"~dendritt: fjerner innsyn fra " <<pInnSynapser.back()->pPreNode->sNavn <<" til " <<pInnSynapser.back()->pPostNode->sNavn <<"( " <<pInnSynapser.size() <<" synapser igjen)."	<<endl;
			pInnSynapser.pop_back(); 	//pop_back() fjærner siste ledd i std::vector. Kaller også destructor for dette elementet.
			// ~synapse tar hånd om (fjærner) presynaptisk kobling til denne synapsen. TODO
		}
	}
	*/ //}

	friend class auron;
	friend class axon;
	friend class synapse;
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );

	friend int main(int, char**); //TODO SLETT
};


#endif
