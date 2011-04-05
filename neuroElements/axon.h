
#ifndef AXON_H_
#define AXON_H_

#include "../andreKildefiler/time.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/auron.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
class synapse;
class auron;
//class timeInterface;

class axon : public timeInterface{
	inline void doTask();

	const auron* pElementAvAuron; // TODO ENDRE TIL pElementAvAuron, eller noke. Sjå kva eg har skreve i rapporten.
	std::list<synapse*> pUtSynapser;

	public:
	/**************************************************
	****     axon::axon()  - constructor           ****
	****        -pElementAvAuron(pAuronArg)        ****
	****        -timeInterface("axon")              ****
	****                                           ****
	**************************************************/
	axon(const auron* pAuronArg);// : timeInterface("axon"), pElementAvAuron(pAuronArg){ //TODO tanke er at axon må tilhøre eit auron. Difor auronpeiker.
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
	friend class auron;
	friend class synapse;
	friend void testFunksjon_slett(auron*);
	//friend std::ostream & operator<< (std::ostream & ut, axon );
	friend std::ostream & operator<< (std::ostream & ut, axon* );
	friend int main(int, char**); //TODO SLETT
};

#endif
