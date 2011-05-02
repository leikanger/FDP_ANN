
#ifndef AXON_H_
#define AXON_H_

#include "../andreKildefiler/time.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
class i_synapse;
class s_synapse;
class K_synapse;
class i_auron;
class s_auron;
class K_auron;

class s_axon;
class K_axon;

/* <<interface>> */
class i_axon : public timeInterface{
	// XXX XXX XXX Utsetter doCalculation() for alle axon: (definerer den til å gjøre ingenting her for å unngå at klassene under blir abstract..)
	virtual void doCalculation() {}
	
	// Desse overlagres i de modell-speisfikke elementa (s_axon og K_axon), for å gi mulighet til å kalle modellspesifikke funksjoner og variabler for s_axon/K_axon.
	i_auron* pElementAvAuron; 		 // Flytta til i_axon
	std::list<i_synapse*> pUtSynapser; // Flytta til i_axon

	//SLETT:
	virtual void SLETTtypeId(){ cout<<"Dette er i_axon\n"; }
	
//	protected:
//	inline void doTask();

	public:
	i_axon(std::string sKlasseNavn /*="dendrite"*/);
	~i_axon();
	
	
	
	friend void testFunksjon_slett(s_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX XXX denne skal ha argument (..., i_axon* ); XXX
	friend int main(int, char**); //TODO SLETT

	friend class i_synapse;
//	friend class i_axon;
	friend class i_auron;
	friend class i_dendrite;

	friend class K_auron;
	friend class K_synapse;
	friend class K_axon;
	

	friend class s_synapse;
	friend class s_axon;
	friend class s_auron;
	friend class s_dendrite;
};

class s_axon : public i_axon{
	inline void doTask();

	// Desse overlagres i de modell-speisfikke elementa (s_axon og K_axon), for å gi mulighet til å kalle modellspesifikke funksjoner og variabler for s_axon/K_axon.
	s_auron* pElementAvAuron;
	std::list<s_synapse*> pUtSynapser;

	public:
	/**************************************************
	****     axon::axon()  - constructor           ****
	****        -pElementAvAuron(pAuronArg)        ****
	****        -timeInterface("axon")              ****
	****                                           ****
	**************************************************/
	s_axon(s_auron* pAuronArg);// : timeInterface("axon"), pElementAvAuron(pAuronArg) 		//XXX Bare mens eg utvikler i_auron. Kan fjærne etterkvart..
	//axon(const i_auron* p_iAuronArg);
	~s_axon();

	//SLETT:
	virtual void SLETTtypeId(){ cout<<"Dette er i_axon\n"; }
	void slettTESTFUNK(){ cout<<"s_axon::slettTESTFUNK()\n"; } 									// TODO SLETT TODO




	friend class i_auron;
	//friend class s_auron;
	friend class s_synapse;
	friend void testFunksjon_slett(s_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX XXX denne skal ha argument (..., i_axon* ); XXX
	friend int main(int, char**); //TODO SLETT
};

class K_axon : public i_axon{
	inline void doTask();

	// Desse overlagres i de modell-speisfikke elementa (s_axon og K_axon), for å gi mulighet til å kalle modellspesifikke funksjoner og variabler for s_axon/K_axon.
	K_auron* pElementAvAuron;
	std::list<K_synapse*> pUtSynapser;

	public:
	K_axon(K_auron*);
	~K_axon();

	//SLETT:
	virtual void SLETTtypeId(){ cout<<"Dette er i_axon\n"; }

	//Skal vekk:
	friend class K_synapse;
};
#endif
// vim:fdm=marker:fmr=//{,//}
