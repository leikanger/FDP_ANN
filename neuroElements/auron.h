
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
class s_dendrite;
class axon;
class spiking_aktivitetsObj;






/*
 * 	Skriver først auron for spiking ANN
 */
class i_auron : public timeInterface
{ 			
//	protected: gjorde private: 29.03
	//Deler av auronet:
	axon* pOutputAxon; 			
 	s_dendrite* pInputDendrite; 

	// aktivitetsobjekt: Om dette er KANN eller SANN er avhengig av kva nAktivitetsVariabel skal bety (kappa eller depol..).
	int nAktivitetsVariabel;

	//dopamin: for å styre synaptisk plastisitet. En ide inspirert av naturen.
	//også her kan eg bruke \kappa for å finne nivået i kvart neuron..


	std::ofstream aktivitetsVar_loggFil;
 	
	const void loggAktivitetsVar_i_AktivitetsVarLoggFil(){
		aktivitetsVar_loggFil<<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<";#refraction time until now.\n";
	}

/*** XXX BARE FOR SANN *****/
	//bare for spiking Auron:  Skal vekk: ?
	//unsigned long ulTimestampForrigeInput;
	//unsigned long ulTimestampForrigeFyring;
/*** til hit: XXX BARE FOR SANN **/
	

	protected:
	virtual inline void doTask();



	public:
	/*auron() 					: timeInterface("auron"), ao_AuronetsAktivitet(this), sNavn("unnamed") {
		pOutputAxon = new axon(this); 						// TODdO XdXX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 
	 	pInputDendrite = new dendrite(this); 				// TOdDO XXdX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 

	} //X XX  Utesta før aktivitetsObj er i orden.
	*/
	i_auron(std::string sNavn_Arg ="unnamed", int nStartDepol = 0); 		//: timeInterface("auron"), ao_AuronetsAktivitet(this), sNavn(sNavn_Arg) {
	~i_auron();

	std::string sNavn; //for utskrift
	const std::string getNavn(){ return sNavn; }

	int getAktivityVar(){ return nAktivitetsVariabel; }

	//testfunksjon:
	void exiterNeuronTilFyringGjennomElectrode()
	{
		doTask(); //gjør samme som gamle: auron::fyr();
	}

	friend class s_auron;
	//friend class K_auron;
	friend class axon;
	friend class s_synapse;
	friend class s_dendrite;
	friend void testFunksjon_slett(i_auron*);
	friend std::ostream & operator<< (std::ostream& , i_auron);
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );

	friend int main(int, char**); //TODO SLETT
}; 





class s_auron : public i_auron
{ //{
	unsigned long ulTimestampForrigeInput; 	 //Er begge naudsynt? sjå gjennom!
	unsigned long ulTimestampForrigeFyring;  //Er begge naudsynt? sjø gjennom!

	inline void doTask();
	
	public:
	s_auron(std::string sNavn_Arg ="unnamed", int nStartDepol = 0); 	
	~s_auron();

//{friend
	friend class axon;
	friend class s_synapse;
	friend class s_dendrite;
	friend void testFunksjon_slett(i_auron*);
	friend std::ostream & operator<< (std::ostream& , i_auron);
	//friend std::ostream & operator<< (std::ostream & ut, axon  );
	friend std::ostream & operator<< (std::ostream & ut, axon* );

	friend int main(int, char**); //TODO SLETT
//}

}; //}









#endif /* AURON_H_ */
