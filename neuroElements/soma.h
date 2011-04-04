/*
 * soma.h
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */

#ifndef AURON_H_
#define AURON_H_


#include "../andreKildefiler/main.h"
#include "../andreKildefiler/tid.h"
#include "../andreKildefiler/aktivitetsObj.h"
#include "../neuroElements/axon.h"
#include "../neuroElements/dendrite.h"


using std::endl;
using std::cout;


// klasse deklarasjoner:
class dendrite;
class axon;
class spiking_aktivitetsObj;



/*
 * 	Skriver først soma for spiking ANN
 */
 //TODO endre navn: 	soma => soma 	 ? 	TODO
class soma : public tidInterface
{ 			
//	protected: gjorde private: 29.03
	//Deler av somaet:
	axon* pOutputAxon; 			
 	dendrite* pInputDendrite; 
	// FUUNKER DETTE: dendritt dendritt_input(this);
	//Dendritter (alle input) 	Bør være fleire enn en for simulering, men for no kjører eg med bare en.  Kan for eksempel bli: //std::vector<sDendritt> dendritter;

	// aktivitetsobjekt: Om dette er KANN eller SANN er avhengig av kva abstract_aktivitetsOvj som brukes. (kva nedarva klasse)
	spiking_aktivitetsObj ao_AuronetsAktivitet;

	//bare for spiking Auron: XXX Skal vekk: (over i aktivitetsObj..)
	//unsigned long ulTimestampForrigeInput;
	//unsigned long ulTimestampForrigeFyring;
	

	virtual void fyr() 
	{ 
		
		//Axon hillock: send aksjonspotensial 	-- innkapsling gir at axon skal ta hånd om all output. // bestiller at axon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i axonet.
		tid::pTaskArbeidsKoe_List .push_back( pOutputAxon );

		// Test..
		cout<<"\n\t\tsoma: PANG:\t\t";// <<sNavn "\n\n";

	/* //{ 	UTKOMMENTERT 		//Skal bl.a. oppdatere DA-nivå i dendritter, osv.
// UTDATERT! Har gått over til aktivitetsObj. Alt blir tatt hand om der..
		//oppdaterNeuron() //er dette naudsynt FØR fyring?

		// 'Refraction time' skal være implementert i overføring-funk (i sy napse)..
	 	if( ulTimestampForrigeFyring == tid::ul Tidsiterasjoner )
		{
			cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ soma.h\n************************\n\n";
			return;
		}

		// Registrerer fyringstid (for feisjekk (over) osv.)
		ulTimestampForrigeFyring = tid::ul Tidsiterasjoner;

		cout<<"** \t\t*\t*  Fyring av neuron " <<sNavn <<" *\t*\t ** \t\t Tid:\t" <<tid::ul Tidsiterasjoner <<endl;

		//Resetter depol.verdi (XXX DENNE SKAL BARE KALLES FRA aktivitetsObj. Da treng eg ikkje resette depol. her)
		//Ordne slik at dette kun kan kalles fra aktivitetsObj! XXX
		aktivitetsVariabel = 0;
		
		*/
		//}
	}

	/* //{ Om doTask() 
		KVA SKAL doTask() GJØRE?
 			soma::doTask() skal kanskje gjøre lekkasje for spiking neuron?
				( Overføring i sy napsene, som fører til depol. i postsyn gjøres no i class sy napse.. )
				Anna:
					-
	*/ //}
	inline void doTask(){
		//XXX Kva skal skje for soma::doTask() ?   // Initiere A.P. ? 
													// Lekkasje? 
													// `backpropagating action potential' (syn.plast.)? (type sjekke overføringstid for sy napser og gjennomføre syn.plast.)
	}


	//AktivitetsVariabel = depolarisasjon for spiking Auron.
	//float aktivitetsVariabel; 		Ny: ao_AuronetsAktivitet (er av classe spiking_aktivitetsObj)


	//dopamin: for å styre synaptisk plastisitet. En ide inspirert av naturen.
	//også her kan eg bruke \kappa for å finne nivået i kvart neuron..


	public:
	/*soma() 					: tidInterface("soma"), ao_AuronetsAktivitet(this), sNavn("unnamed") {
		pOutputAxon = new axon(this); 						// TODdO XdXX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 
	 	pInputDendrite = new dendrite(this); 				// TOdDO XXdX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 

	} //X XX  Utesta før aktivitetsObj er i orden.
	*/
	soma(std::string /*sNavn_Arg ="unnamed"*/); 		//: tidInterface("soma"), ao_AuronetsAktivitet(this), sNavn(sNavn_Arg) {
	~soma();


	std::string sNavn; //for utskrift
	const std::string getNavn(){ return sNavn; }

	//testfunksjon:
	void exiterNeuronTilFyringGjennomElectrode()
	{
	 	fyr();
	}

	friend class axon;
	friend class synapse;
	friend class dendrite;
	friend void testFunksjon_slett(soma*);
	friend std::ostream & operator<< (std::ostream& , soma);
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );

	friend int main(int, char**); //TODO SLETT
}; 






#endif /* AURON_H_ */
