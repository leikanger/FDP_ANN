#include <fstream> //filestreams

#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/auron.h"
#include "../andreKildefiler/time.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class axon;
class s_dendrite;
class s_auron;

class s_synapse : public timeInterface{

	axon* pPreNodeAxon;
	s_dendrite* pPostNodeDendrite; //TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO  Skal være i_dendrite, slik at det er mulgi å bruke K_dendrite også..

	const bool bInhibitorisk_effekt;
	unsigned uSynaptiskVekt;

	std::ofstream synWeight_loggFil;

	inline void doTask();

//	void transmission(); Flytta inn i doTask()

	public:
	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX

	//synapse(axon* pPresynAxon_arg, dendrite* pPostsynDendrite_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) ; XXX Vekk med den? XXX 07.04.2011
	s_synapse(s_auron*, s_auron*, float fSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~s_synapse(); //Sjå neuroElement.cpp
	
	int getSynVekt(){ return uSynaptiskVekt; }

	//friend std::ostream & operator<< (std::ostream & ut, synapse*); //TODO
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );
};



#endif
