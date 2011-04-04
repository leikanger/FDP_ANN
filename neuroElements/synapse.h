
#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/auron.h"
#include "../andreKildefiler/tid.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class axon;
class dendrite;

class synapse : public tidInterface{
	
	axon* pPreNodeAxon;
	dendrite* pPostNodeDendrite;

	const bool bInhibitorisk_effekt;
	int nSynaptiskVekt;


	inline void doTask();

	void transmission();

	public:
	//synapse() : tidInterface("synapse"), bInhibitorisk_effekt(false){} 		// SKAL VEKK

	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX

	//synapse(axon* pPresynAxon_arg, dendrite* pPostsynDendrite_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) ;
	synapse(auron*, auron*, float fSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~synapse(); //Sjå neuroElement.cpp
	
	int getSynVekt(){ return nSynaptiskVekt; }

	//friend std::ostream & operator<< (std::ostream & ut, synapse*); //TODO
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );
};



#endif
