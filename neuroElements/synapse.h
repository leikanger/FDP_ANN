
#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/soma.h"
#include "../andreKildefiler/tid.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class axon;
class dendrite;

class synapse : public tidInterface{
	
	/*const*/ axon* pPreNodeAxon; 				//bør være const
	/*const*/ dendrite* pPostNodeDendrite; 		//bør være const

	const bool bInhibitorisk_effekt;
	float fSynaptiskVekt;


	void doTask(){
		//Skal handteres i aktivitetsObj:
		// aktivitetsObj.kall-rette-funksjoner();
		// For SANN: summer input.
		// For KANN: oppdater kappa.
		//aktivitetsObj. TODO
	}

	public:
	//synapse() : tidInterface("synapse"), bInhibitorisk_effekt(false){} 		//XXX SKAL VEKK XXX

	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX
	//synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) : bInhibitorisk_effekt(bInhibEffekt_Arg){
	//	synapse(pPresynAuron_arg->pOutputAxon, pPostsynAuron_arg->pInputDendrite, bInhibEffekt_Arg, fSynVekt_Arg);
	//}

	//synapse(axon* pPresynAxon_arg, dendrite* pPostsynDendrite_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) ;
	synapse(auron*, auron*, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) ;
	~synapse(); //Sjå neuroElement.cpp


	//friend std::ostream & operator<< (std::ostream & ut, synapse*); //TODO
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );
};



#endif
