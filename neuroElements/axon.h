
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
class i_auron;
class s_auron;
class s_axon;

/* <<interface>> */
class i_axon : public timeInterface{
//	std::list<i_synapse*> pUtSynapser; 				//TODO TODO TODO TODO TODO TODO ENDRE TIL i_synapse 		i_synapse! 	XXX
	
	i_auron* pElementAvAuron; 		 // Flytta til i_axon
	std::list<i_synapse*> pUtSynapser; // Flytta til i_axon

	
	protected:
	inline void doTask();

	public:
	i_axon(i_auron* pAuronArg, std::string sKlasseNavn /*="dendrite"*/);
	
	
	
	friend class i_auron;
	//friend class s_auron;
	friend class s_synapse;
	friend void testFunksjon_slett(s_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX XXX denne skal ha argument (..., i_axon* ); XXX
	friend int main(int, char**); //TODO SLETT

	friend class i_synapse;
	friend class i_axon;
	friend class i_auron;
	friend class i_dendrite;

	friend class s_synapse;
	friend class s_axon;
	friend class s_auron;
	friend class s_dendrite;
};

class s_axon : public i_axon{
	inline void doTask();

	//s_auron* pElementAvAuron; 		 // Flytta til i_axon
	//std::list<s_synapse*> pUtSynapser; // Flytta til i_axon

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

	friend class i_auron;
	//friend class s_auron;
	friend class s_synapse;
	friend void testFunksjon_slett(s_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX XXX denne skal ha argument (..., i_axon* ); XXX
	friend int main(int, char**); //TODO SLETT
};

#endif
