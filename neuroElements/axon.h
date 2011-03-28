
#ifndef AXON_H_
#define AXON_H_

#include "../andreKildefiler/tid.h"
#include "../andreKildefiler/main.h"
#include "../neuroElements/synapse.h"
#include "../neuroElements/soma.h"
//#include "../andreKildefiler/aktivitetsObj.h"


//Deklarasjoner:
class synapse;
class auron;
//class tidInterface;

class axon : public tidInterface{
	inline void doTask()
	{ //{ // initierAksjonspotensial()
		cout<<"\n\n\n\n\n\nJADA\n\n\n\n";

		// For meir nøyaktig simulering av tid kan alle synaper få verdi for 'time lag' før fyring. No fokuserer eg heller på effektivitet. 
	 	for( std::list<synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
		{ // Legger alle pUtSynapser inn i arbeidskø: (FIFO-kø)
			
			//tid::pTaskArbeidsKoe_List.push_back( (*iter) );
			cout<<"\tinne i loop for_a001(;;)\n";
	//TODO ordne dette:
			cout<<(*iter) <<endl;
		}
	} //}

	const auron* pElementAvAuron; // TODO ENDRE TIL pElementOfAuron, eller noke. Sjå kva eg har skreve i rapporten.
	std::list<synapse*> pUtSynapser;

	public:
	/**************************************************
	****     axon::axon()  - constructor           ****
	****        -pElementAvAuron(pAuronArg)        ****
	****        -tidInterface("axon")              ****
	****                                           ****
	**************************************************/
	axon(const auron* pAuronArg);// : tidInterface("axon"), pElementAvAuron(pAuronArg){ //TODO tanke er at axon må tilhøre eit auron. Difor auronpeiker.
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
