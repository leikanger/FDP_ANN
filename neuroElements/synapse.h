#include <fstream> //filestreams

#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/auron.h"
#include "../andreKildefiler/time.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class i_axon;
class s_axon;
class K_axon;
class i_dendrite;
class s_dendrite;
class K_dendrite;
class s_auron;
class K_auron;


// <<interface>>
class i_synapse : public timeInterface{



	virtual inline void doTask() =0; //XXX Reint virtuell klasse!
	
	// XXX XXX XXX Utsetter doCalculation() for alle synapser: (definerer den til å gjøre ingenting her for å unngå at klassene under blir abstract..)
	virtual void doCalculation() {}
	
	protected:
	const bool bInhibitorisk_effekt;
	float fSynapticWeight;
	unsigned uSynapticWeight_promille;
	int nSynapticWeightChange_promille;


	std::ofstream synWeight_loggFil;

	// Overlagrer i underklasseene, s_synapse og K_synapse. 	 Gjør at underelement som er sammenkobbla til samme s_[element], veit dette.
 	i_axon* pPreNodeAxon;
	i_dendrite* pPostNodeDendrite; 


	public:
	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX

	//i_synapse(i_axon* pPresynAxon_arg, i_dendrite* pPostsynDendrite_arg, unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ );
	i_synapse(unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ );
	//~i_synapse();  TRUR IKKJE DENNE TRENGS. Ingen peikere i i_synapse ?
	// KANSKJE EG SKAL HA UTSKRIFT HER? --avslutting av loggen for synapsen..  TODO
	
	int getSynVekt(){ return fSynapticWeight; }

	friend int make(int, char**);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );
 	friend std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );

	//friend class s_synapse;
};



class s_synapse : public i_synapse{

	inline void doTask();

	// Overlagrer i underklasseene, s_synapse og K_synapse. 	 Gjør at underelement som er sammenkobbla til samme s_[element], veit dette.
 	s_axon* pPreNodeAxon;
	s_dendrite* pPostNodeDendrite; 


	public:
	s_synapse(s_auron*, s_auron*, unsigned uSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~s_synapse();

	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX gjør om arg: (..., s_axon* => i_axon*) XXX
};



class K_synapse : public i_synapse{
	inline void doTask();
	
	unsigned forrigeUtregnaPresynPeriode;
	//unsigned uKappa;

	// Overlagrer peikarane, for å få rett preElement og postElement for synapsen (av rett modell). 	 Gjør at underelement som er sammenkobbla til samme s_[element], veit dette.
 	K_axon* pPreNodeAxon;
	K_dendrite* pPostNodeDendrite; 

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
