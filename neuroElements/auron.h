
#include <fstream> //file streams
//#include <list>

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
class i_axon;
class s_axon;


/* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	
		Lag ei std::list<s_auron*> s_auron::pAlleAuron; som inneholder alle auron. På slutten av main() kan eg kjøre while(!s_auron::pAlleAuron.empty(){ delete s_auron::pAlleAuron->begin(); }
		(og legge til auron (i auron::pAlleAuron) i auron's constructor..)

   TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO */



/*
 * 	Skriver først auron for spiking ANN
 */
class i_auron : public timeInterface
{ 			
	//Deler av auronet: (Ligger som s_axon og s_dendrite i s_auron. Samme for K_auron..) TODO SKal ligge der også ?
	i_axon* pOutputAxon; 			// Trenger å ha dei meir spesifikk for contruction av bl.a. synapse - s_synapse legger til pElementAvAuron->pInputDendrite (som må være av typen ?? XXX prøver igjen..
 	i_dendrite* pInputDendrite; 

	
	
	
	// Treng eg desse i i_auron? Bare for SANN? Vettafaen! XXX Kan kanskje ligge i s_auron.

	// BARE FOR SANN: ... og for KANN trenger eg en bEndraKappaDennePerioden, som blir satt til false kvar fyring av auronet. XXX
	unsigned long ulTimestampForrigeInput; 	 //Er begge naudsynt? sjå gjennom!
	// FOR BEGGE (SANN og KANN)
	unsigned long ulTimestampForrigeFyring;  //Er begge naudsynt? sjø gjennom!


	// aktivitetsobjekt: Om dette er KANN eller SANN er avhengig av kva nAktivitetsVariabel skal bety (kappa eller depol..).
	int nAktivitetsVariabel;

	//dopamin: for å styre synaptisk plastisitet. En ide inspirert av naturen.
	//også her kan eg bruke \kappa for å finne nivået i kvart neuron..


	std::ofstream activityVar_logFile;
 	
	const void skrivAktivitetsVarLogg(){
	//const void loggAktivitetsVar_i_AktivitetsVarLoggFil(){
	 	activityVar_logFile 	<<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<"; \t #Activity variable\n" ;
	 	activityVar_logFile.flush();
	}
	

	protected:
	virtual inline void doTask() =0;

	//container som inneholder alle auron som har vore til ila. programkjøringa:
	static std::list<i_auron*> pAllAurons;


	public:
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
	friend class K_auron;

	friend class i_axon; // VEKK med den?
	friend class s_axon;
	friend class K_axon;

	friend class i_synapse;
	friend class s_synapse;
	friend class K_synapse;
	
	friend class i_dendrite;
	friend class s_dendrite;
	friend class K_dendrite;

	friend std::ostream & operator<< (std::ostream& , s_auron);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );

	friend int main(int, char**); //TODO SLETT
}; 





class s_auron : public i_auron
{ //{
	//Deler av auronet:

	inline void doTask();


	public:
	s_auron(std::string sNavn_Arg ="unnamed", int nStartDepol = 0); 	
	~s_auron();

//{friend
	friend class s_axon;
	friend class s_synapse;
	friend class s_dendrite;
	friend void testFunksjon_slett(s_auron*);
	friend std::ostream & operator<< (std::ostream& , s_auron);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );

	friend int main(int, char**); //TODO SLETT
//}

}; //}



class K_auron : public i_auron
{ //{
	
	std::multimap<unsigned long, timeInterface>::iterator p_iterAuronsPositionInEstimatedTaskTime;

	inline void doTask();

	// todo TODO TODO TODO For KANN trenger eg en bEndraKappaDennePerioden, som blir satt til false kvar fyring av auronet. XXX
	bool bEndraKappaDennePerioden;

	// returnerer tidspunkt for fyring med nye kappa.
	const unsigned long estimateFiringTime();
	void newKappaDueToNew_inputLevel(int);

	public:
	K_auron(std::string sNavn_Arg ="unnamed", int nStartKappa = FYRINGSTERSKEL); 	
	~K_auron();

//{friend
	friend class K_axon;
	friend class K_synapse;
	friend class K_dendrite;
	friend void testFunksjon_slett(s_auron*);
	friend std::ostream & operator<< (std::ostream& , s_auron);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );

	friend int main(int, char**); //TODO SLETT
//}

}; //}






#endif /* AURON_H_ */
