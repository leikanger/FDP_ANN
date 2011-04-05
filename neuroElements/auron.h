
#include <fstream> //file streams

/*
 * auron.h
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */

#ifndef AURON_H_
#define AURON_H_


#include "../andreKildefiler/main.h"
#include "../andreKildefiler/time.h"
//#include "../neuroElements/andreAuronKlasser.h" UTDATERT IDE.
#include "../neuroElements/axon.h"
#include "../neuroElements/dendrite.h"


using std::endl;
using std::cout;


// klasse deklarasjoner:
class dendrite;
class axon;
class spiking_aktivitetsObj;



/*
 * 	Skriver først auron for spiking ANN
 */
class auron : public timeInterface
{ 			
//	protected: gjorde private: 29.03
	//Deler av auronet:
	axon* pOutputAxon; 			
 	dendrite* pInputDendrite; 
	// FUUNKER DETTE: dendritt dendritt_input(this);
	//Dendritter (alle input) 	Bør være fleire enn en for simulering, men for no kjører eg med bare en.  Kan for eksempel bli: //std::vector<sDendritt> dendritter;

	// aktivitetsobjekt: Om dette er KANN eller SANN er avhengig av kva nAktivitetsVariabel skal bety (kappa eller depol..).
	int nAktivitetsVariabel;

	//dopamin: for å styre synaptisk plastisitet. En ide inspirert av naturen.
	//også her kan eg bruke \kappa for å finne nivået i kvart neuron..


	std::ofstream aktivitetsVar_loggFil;
 	

	//bare for spiking Auron:  Skal vekk: ?
	unsigned long ulTimestampForrigeInput;
	unsigned long ulTimestampForrigeFyring;
	


	/* //{ Om doTask() 
		KVA SKAL doTask() GJØRE?
 			auron::doTask() skal kanskje gjøre lekkasje for spiking neuron?
				( Overføring i sy napsene, som fører til depol. i postsyn gjøres no i class sy napse.. )
				Anna:
					-
	*/ //}
	inline void doTask();



	public:
	/*auron() 					: timeInterface("auron"), ao_AuronetsAktivitet(this), sNavn("unnamed") {
		pOutputAxon = new axon(this); 						// TODdO XdXX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 
	 	pInputDendrite = new dendrite(this); 				// TOdDO XXdX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 

	} //X XX  Utesta før aktivitetsObj er i orden.
	*/
	auron(std::string sNavn_Arg ="unnamed", int nStartDepol = 0); 		//: timeInterface("auron"), ao_AuronetsAktivitet(this), sNavn(sNavn_Arg) {
	~auron();


	std::string sNavn; //for utskrift
	const std::string getNavn(){ return sNavn; }

	int getAktivityVar(){ return nAktivitetsVariabel; }

	//testfunksjon:
	void exiterNeuronTilFyringGjennomElectrode()
	{
		doTask(); //gjør samme som gamle: auron::fyr();
	}

	friend class axon;
	friend class synapse;
	friend class dendrite;
	friend void testFunksjon_slett(auron*);
	friend std::ostream & operator<< (std::ostream& , auron);
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );

	friend int main(int, char**); //TODO SLETT
}; 






#endif /* AURON_H_ */
