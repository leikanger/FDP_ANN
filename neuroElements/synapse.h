#include <fstream> //filestreams

#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/auron.h"
#include "../andreKildefiler/time.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class axon;
class s_dendrite;
class s_auron;


// <<interface>>
class i_synapse : public timeInterface{


	virtual inline void doTask() =0; //XXX Reint virtuell klasse!
	
	protected:
	const bool bInhibitorisk_effekt;
	unsigned uSynaptiskVekt;

	std::ofstream synWeight_loggFil;
//	axon* pPreNodeAxon; 					Flytta til s_synapse
//	s_dendrite* pPostNodeDendrite; 			Skal være i_dendrite, slik at det er mulgi å bruke K_dendrite også..
// Sjå under, i s_synapse. Skal være slik for k_synapse også.

	public:
	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX

	//synapse(axon* pPresynAxon_arg, dendrite* pPostsynDendrite_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) ; XXX Vekk med den? XXX 07.04.2011
	//i_synapse(s_auron*, s_auron*, float fSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	i_synapse(float uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ );
	//~i_synapse();  TRUR IKKJE DENNE TRENGS. Ingen peikere i i_synapse ?
	// KANSKJE EG SKAL HA UTSKRIFT HER? --avslutting av loggen for synapsen..  TODO
	
	int getSynVekt(){ return uSynaptiskVekt; }

	//friend std::ostream & operator<< (std::ostream & ut, synapse*); //TODO
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );
};



class s_synapse : public i_synapse{

	virtual void doTask();

 	axon* pPreNodeAxon;
	s_dendrite* pPostNodeDendrite; 

	public:
	s_synapse(s_auron*, s_auron*, unsigned uSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~s_synapse();

	friend std::ostream & operator<< (std::ostream & ut, axon* );
};


/*

class s_synapse : public timeInterface{

	axon* pPreNodeAxon;
	s_dendrite* pPostNodeDendrite; //TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO  Skal være i_dendrite, slik at det er mulgi å bruke K_dendrite også..

	const bool bInhibitorisk_effekt;
	unsigned uSynaptiskVekt;

	std::ofstream synWeight_loggFil;

	virtual inline void doTask();

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
*/

#endif
