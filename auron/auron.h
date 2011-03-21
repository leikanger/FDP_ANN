/*
 * auron.h
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */

#ifndef AURON_H_
#define AURON_H_


#include "../andreKildefiler/main.h"
#include "../andreKildefiler/tid.h"
#include "../dendritt_axon/axon.h"
#include "../dendritt_axon/dentrite.h"
#include "../andreKildefiler/aktivitetsObj.h"


using std::endl;
using std::cout;


// klasse deklarasjoner:
class dendrite;
class axon;
class spiking_aktivitetsObj;



/*
 * 	Skriver først auron for spiking ANN
 */
class auron : public tidInterface
{ 			//{auron for spiking ANN
	protected:
	//Deler av auronet:
	axon* pAxon_output;
	dendrite* pDendritt_input; 
	// FUUNKER DETTE: dendritt dendritt_input(this);
	//Dendritter (alle input) 	Bør være fleire enn en for simulering, men for no kjører eg med bare en.  Kan for eksempel bli: //std::vector<sDendritt> dendritter;

	// aktivitetsobjekt: Om dette er KANN eller SANN er avhengig av kva abstract_aktivitetsOvj som brukes. (kva nedarva klasse)
	spiking_aktivitetsObj ao_AuronetsAktivitet;

	//bare for spiking Auron: XXX Skal vekk: (over i aktivitetsObj..)
	unsigned long ulTimestampForrigeInput;
	unsigned long ulTimestampForrigeFyring;
	

	// static liste over alle nodene. For å regne ut lekkasje for spiking auron.
	static std::list<auron*> lAlleNodeneSomSkal_doTask; 	//skal være med i både s_auron og k_auron.

	/* XXX Flytta inn i class aktivitetsObj (for spiking_aktivitetsObj) //{3 KOMMENTERT UT
	// Lekkasje for 'leaky integrator'. Skal kjøres kvar klokkeiterasjon i SANN. (er bare med i SANN)
	void verdiLekkasje()
	{ // {2
		// Gjør simulert asynkronitet redundant tankearbeid for SANN, men det vil ikkje minke effektivitet for SANN
		aktivitetsVariabel *= LEKKASJEFAKTOR_FOR_DEPOL;
	} // }2
 	*/ //}3

	virtual void fyr() // UTDATERT! Har gått over til aktivitetsObj. Alt blir tatt hand om der..
	{ 
		
		//Axon hillock: send aksjonspotensial 	-- innkapsling gir at axon skal ta hånd om all output. // bestiller at axon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i axonet.
		tid::pTaskArbeidsKoe_List .push_back( pAxon_output );

		// Test..
		cout<<"\n\t\tauron: PANG:\t\t";// <<sNavn "\n\n";

	/* //{2 	UTKOMMENTERT 		//Skal bl.a. oppdatere DA-nivå i dendritter, osv.
		//oppdaterNeuron() //er dette naudsynt FØR fyring?

		// 'Refraction time' skal være implementert i overføring-funk (i sy napse)..
	 	if( ulTimestampForrigeFyring == tid::ul Tidsiterasjoner )
		{
			cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
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

	/* //{2 Om doTask() 
 			auron::doTask() skal kanskje gjøre lekkasje for spiking neuron?
				( Overføring i sy napsene, som fører til depol. i postsyn gjøres no i class sy napse.. )
				Anna:
					-
	*/ //}2
	inline void doTask(){
		//XXX Kva skal skje for auron::doTask() ?   // Initiere A.P. ? 
													// Lekkasje? 
													// `backpropagating action potential' (syn.plast.)? (type sjekke overføringstid for sy napser og gjennomføre syn.plast.)
	}


	//AktivitetsVariabel = depolarisasjon for spiking Auron.
	//float aktivitetsVariabel; 		Ny: ao_AuronetsAktivitet (er av classe spiking_aktivitetsObj)


	//dopamin: for å styre synaptisk plastisitet. En ide inspirert av naturen.
	//også her kan eg bruke \kappa for å finne nivået i kvart neuron..


	public:
	auron() 					: tidInterface("auron"), ao_AuronetsAktivitet(this), sNavn("unnamed") {
		pAxon_output = new axon(this); 						// TODO XXX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). XXX TODO
	 	pDendritt_input = new dendrite(this); 				// TODO XXX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). XXX TODO

	} //XXX  Utesta før aktivitetsObj er i orden.
	
	auron(std::string sNavn_Arg) : tidInterface("auron"), ao_AuronetsAktivitet(this), sNavn(sNavn_Arg) {
		auron();
	}
	
	std::string sNavn; //for utskrift

	//testfunksjon:
	void exiterNeuronTilFyringGjennomElectrode()
	{
	 	fyr();
	}

	friend void testFunksjon_slett(auron*);

}; //}






#endif /* AURON_H_ */
