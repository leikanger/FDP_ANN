
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
class s_synapse;
class s_auron;

/*****************************************************************
** class dendritt 			-  	Notater:
** 	
** 		- dendritt må ha aktivitetsVariabel.
**
*****************************************************************/
class s_dendrite : public timeInterface{
	inline void doTask();

	s_auron* pElementAvAuron;
	std::list<s_synapse*> pInnSynapser;

	inline void newInputSignal( int nNewSignal );
	inline void calculateLeakage();

	bool bBlokkerInput_refractionTime; //Blokkere input når refraction period eller når depol er over terskel.

	public:
	//Constructor: 	
	s_dendrite( s_auron* pPostSynAuron_Arg ); // : pElementAvAuron(pPostSynAuron_Arg)  		Definisjon i neuroElement.cpp
	~s_dendrite();


	//TODO LEGG TIL dendrite::leggTilSynapse(synapse*) ! Gjør saker lettere, og bedre innkapsling!
	
	//Destructor:
	/* kommenterer ut mens s_synapser ikkje er laga enda.. //{
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

	friend class s_auron;
	friend class axon;
	friend class s_synapse;
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );

	friend int main(int, char**); //TODO SLETT
};


#endif
