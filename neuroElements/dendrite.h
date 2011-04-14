
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
class i_synapse;
class s_synapse;
class s_auron;

/*****************************************************************
** class dendritt 			-  	Notater:
** 	
** 		- dendritt må ha aktivitetsVariabel.
**
*****************************************************************/
class i_dendrite : public timeInterface{
	i_auron* pElementAvAuron;
	std::list<i_synapse*> pInnSynapser;

 	virtual inline void doTask() =0;
	// XXX XXX XXX Utsetter doCalculations() for alle dendrite: (definerer den til å gjøre ingenting her for å unngå at klassene under blir abstract..)
	virtual void doCalculations() {}

	virtual inline void newInputSignal( int ) =0;
	virtual inline void feedbackToDendrite() =0;

	public:
	i_dendrite(i_auron* pElementAvAuron_arg, std::string sNavn /*="dendrite"*/);
	~i_dendrite();

	friend int main(int, char**);
	friend std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );

	friend class i_synapse;
	friend class i_axon;
	friend class i_auron;
	//friend class i_dendrite;

	friend class s_synapse;
	friend class s_axon;
	friend class s_auron;
	friend class s_dendrite;

	friend class K_synapse;
	friend class K_axon;
	friend class K_auron;
	friend class K_dendrite;
};


class s_dendrite : public i_dendrite{
	inline void doTask();
	inline void feedbackToDendrite();

//	s_auron* pElementAvAuron;
//	std::list<s_synapse*> pInnSynapser;

	inline void newInputSignal( int nNewSignal );
	inline void calculateLeakage(); 		//Bare for SANN

	bool bBlockInput_refractionTime; 		//Bare for SANN 	//Blokkere input når refraction period eller når depol er over terskel.
												// XXX Flytta til s_auron.

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
	friend class s_axon;
	friend class s_synapse;
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* );

	friend int main(int, char**); //TODO SLETT
};

class K_dendrite : public i_dendrite{
 	inline void doTask();
	inline void feedbackToDendrite();

	inline void newInputSignal(int);

	public:
	K_dendrite( K_auron* );
	~K_dendrite();
};

#endif
