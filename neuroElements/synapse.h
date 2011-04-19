#include <fstream> //filestreams

#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/auron.h"
#include "../andreKildefiler/time.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class i_axon;
class s_axon;
class i_dendrite;
class s_dendrite;
class s_auron;
class K_auron;


// <<interface>>
class i_synapse : public timeInterface{



	virtual inline void doTask() =0; //XXX Reint virtuell klasse!
	
	// XXX XXX XXX Utsetter doCalculation() for alle synapser: (definerer den til å gjøre ingenting her for å unngå at klassene under blir abstract..)
	virtual void doCalculation() {}
	
	protected:
	const bool bInhibitorisk_effekt;
	unsigned uSynaptiskVekt;

	std::ofstream synWeight_loggFil;

 	i_axon* pPreNodeAxon;
	i_dendrite* pPostNodeDendrite; 


//	axon* pPreNodeAxon; 					Flytta til s_synapse
//	s_dendrite* pPostNodeDendrite; 			Skal være i_dendrite, slik at det er mulgi å bruke K_dendrite også..
// Sjå under, i s_synapse. Skal være slik for k_synapse også.

	public:
	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX

	//synapse(axon* pPresynAxon_arg, dendrite* pPostsynDendrite_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) ; XXX Vekk med den? XXX 07.04.2011
	//i_synapse(s_auron*, s_auron*, float fSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	i_synapse(i_axon* pPresynAxon_arg, i_dendrite* pPostsynDendrite_arg, unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ );
	//~i_synapse();  TRUR IKKJE DENNE TRENGS. Ingen peikere i i_synapse ?
	// KANSKJE EG SKAL HA UTSKRIFT HER? --avslutting av loggen for synapsen..  TODO
	
	int getSynVekt(){ return uSynaptiskVekt; }

	friend int make(int, char**);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );
 	friend std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );

	//friend class s_synapse;
};



class s_synapse : public i_synapse{

	inline void doTask();

	public:
	s_synapse(s_auron*, s_auron*, unsigned uSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~s_synapse();

	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX gjør om arg: (..., s_axon* => i_axon*) XXX
};



class K_synapse : public i_synapse{
	inline void doTask();
	
	unsigned forrigeUtregnaPresynPeriode;
	//unsigned uKappa;

	// TEST: & : inline unsigned& regnutPresynPeriode();
	// Optimalisering? XXX ha denne i presyn istedenfor. Det er en presyn, men mange synapser..
	// Beste er kanskje å regne ut ved AP, og lagre i axon. Kan hentes av synapsene derretter..
	inline unsigned regnutPresynPeriode();

	public:
	K_synapse(K_auron*, K_auron*, unsigned uSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~K_synapse();

	friend int main(int, char**);
};

#endif
// vim:fdm=marker:fmr=//{,//}
