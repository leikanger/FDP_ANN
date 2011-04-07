
#ifndef AXON_H_
#define AXON_H_

#include "../andreKildefiler/time.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
class s_synapse;
class i_auron; 		//XXX skal vekk.
class s_auron;
//class timeInterface;

class axon : public timeInterface{
	inline void doTask();

	i_auron* pElementAvAuron; // TODO SKAL VEKK, når eg har gått over til i_auron -- klassestruktur. (under)
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

	/* Feilmeldinger fordi eg ikkje har laga synapse enda. Kommenterer ut så lenge
	~axon() //destructor
	{
		synapse* pTempSyn;
		
		while( !pUtSynapser.empty() )
		{
	 	 	//Mellomlagrer peiker til siste synapse
			pTempSyn = pUtSynapser.back();

			cout<<"~axon: fjerner utsyn fra axon tilhørende " <<pTempSyn->pPreNode->sNavn <<" til neuron " <<pTempSyn->pPostNode->sNavn <<endl;

			pUtSynapser.pop_back(); // Denne funskjonen kaller også destructor: ~synapse
			// ~synapse tar hånd om å fjærne synapse fra postsynaptisk neurons dendritt. TODO
		}
	}
	*/
	friend class i_auron;
	//friend class s_auron;
	friend class s_synapse;
	friend void testFunksjon_slett(i_auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, axon* );
	friend int main(int, char**); //TODO SLETT
};

#endif
