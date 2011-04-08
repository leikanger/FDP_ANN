
#ifndef AXON_H_
#define AXON_H_

#include "../andreKildefiler/time.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
//class i_synapse;
class s_synapse;
//class i_auron; 		//XXX skal vekk.
//class s_auron;

class axon : public timeInterface{
	inline void doTask();

	i_auron* pElementAvAuron;
	//const i_auron* pElementAvAuronInterface;

	std::list<s_synapse*> pUtSynapser;

	public:
	/**************************************************
	****     axon::axon()  - constructor           ****
	****        -pElementAvAuron(pAuronArg)        ****
	****        -timeInterface("axon")              ****
	****                                           ****
	**************************************************/
	axon(i_auron* pAuronArg);// : timeInterface("axon"), pElementAvAuron(pAuronArg) 		//XXX Bare mens eg utvikler i_auron. Kan fjærne etterkvart..
	//axon(const i_auron* p_iAuronArg);
	~axon();

	friend class i_auron;
	//friend class s_auron;
	friend class s_synapse;
	friend void testFunksjon_slett(i_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, axon* );
	friend int main(int, char**); //TODO SLETT
};

#endif
