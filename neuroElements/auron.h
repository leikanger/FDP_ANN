//{ GNU GPL v.3
/***************************************************************************
*           auronNet - Implementation of event-driven spiking ANN          *
*                           -------------------                            *
* copyright            : (C) 2011 by Per R. Leikanger                      *
* email                : leikange@gmail.com                                *
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/
//}

/*
 * auron.h
 *
 *  Created on: 9. feb. 2011
 *      Author: kybpc1036
 */

#include <fstream> //file streams

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



/*
 * 	Skriver først auron for spiking ANN
 */
class i_auron : public timeInterface
{ 	//{		
	// Variablana pOutputAxon og pInputDendrite overlagres i underklassene. F.eks. i s_auron lages pOutputAxon som en s_axon*. 
	// 		Effekten av dette blir at alle funksjoner og variabler fra i_axon kan kalles fra utsida (for i_axon--peikerar), mens også de modellspesifikke kan kalles fra andre modellspesifikke element (s_axon-funker kan kalles fra s_auron)!

	//Deler av auronet: (Ligger som s_axon og s_dendrite i s_auron. Samme for K_auron..) TODO SKal ligge der også ?
	i_axon* pOutputAxon; 			// Trenger å ha dei meir spesifikk for contruction av bl.a. synapse - s_synapse legger til pElementAvAuron->pInputDendrite (som må være av typen ?? XXX prøver igjen..
 	i_dendrite* pInputDendrite; 

	
	
	
	// Treng eg desse i i_auron? Bare for SANN? Vettafaen! XXX Kan kanskje ligge i s_auron.

	// BARE FOR SANN: ... og for KANN trenger eg en bEndraKappaDennePerioden, som blir satt til false kvar fyring av auronet. XXX
	unsigned long ulTimestampForrigeInput; 	 //Er begge naudsynt? sjå gjennom!
	// FOR BEGGE (SANN og KANN)
	unsigned long ulTimestampForrigeFyring;  //Er begge naudsynt? sjø gjennom!


	// aktivitetsobjekt: Om dette er KANN eller SANN er avhengig av kva dAktivitetsVariabel skal bety (kappa eller depol..).
	// Tidligere: 	int nAktivitetsVariabel;
	double dAktivitetsVariabel;

	//dopamin: for å styre synaptisk plastisitet. En ide inspirert av naturen.
	//også her kan eg bruke \kappa for å finne nivået i kvart neuron..


	std::ofstream depol_logFile;
 	
	virtual const void skrivAktivitetsVarLogg(){
	//const void loggAktivitetsVar_i_AktivitetsVarLoggFil(){
	 	depol_logFile 	<<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"; \t #Activity variable\n" ;
	 	depol_logFile.flush();
	}
	

	protected:
	virtual inline void doTask() =0;

	//container som inneholder alle auron som har vore til ila. programkjøringa:
	static std::list<i_auron*> pAllAurons;


	public:
	i_auron(std::string sNavn_Arg ="unnamed", double dStartAktVar = 0); 		//: timeInterface("auron"), ao_AuronetsAktivitet(this), sNavn(sNavn_Arg) {
	~i_auron();

	std::string sNavn; //for utskrift
	const std::string getNavn(){ return sNavn; }

	int getAktivityVar(){ return dAktivitetsVariabel; }

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

	friend std::ostream & operator<< (std::ostream & ut, i_axon* );

	friend int main(int, char**); //TODO SLETT
};  //}

class s_auron : public i_auron
{ //{
	//Deler av auronet: 		OVERLAGRA fra i_auron
	s_axon* pOutputAxon; 			// Overlagrer i_auron::i_axon til s_auron::s_axon. Dette er alternativ til å caste pOutputAxon ved accessering til s_auron::pOutputAxon
 	s_dendrite* pInputDendrite;  	// Samme for pInputDendrite.

void slettTESTfunk(){ cout<<"s_auron::TESTfunk()\n"; } 			//TODO TODO TDOD TODO SLETT
	inline void doTask();
	inline void doCalculation() { cout<<"s_auron::doCalculation()\n";} 		//XXX UTSETTER. Foreløpig gjør denne ingenting (anna enn å gjøre at s_auron ikkje er abstract)


	public:
	s_auron(std::string sNavn_Arg ="unnamed", int nStartDepol = 0); 	
	~s_auron();

//{friend
	friend class s_axon;
	friend class s_synapse;
	friend class s_dendrite;
	friend void testFunksjon_slett(s_auron*);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );

	friend int main(int, char**); //TODO SLETT
//}

}; //}

class K_auron : public i_auron
{ // {
	//Deler av auronet: 		OVERLAGRA fra i_auron
	K_axon* pOutputAxon; 			// Overlagrer i_auron::i_axon til K_auron::K_axon. Dette er alternativ til å caste pOutputAxon ved accessering til K_auron::pOutputAxon
 	K_dendrite* pInputDendrite;  	// Samme for pInputDendrite.
	

	inline void doTask();
	inline void doCalculation();

	unsigned long ulStartOfTimewindow;
	double dDepolAtStartOfTimeWindow;

	unsigned uLastCalculatedPeriod_promille;
	unsigned uLastCalculatedPeriodInverse_promille;
	int nChangeInPeriodInverse_promille;

	unsigned long uEstimertTidTilFyring;

	inline void changeKappa( double );
	inline double getKappa(){ return dAktivitetsVariabel; }

	// todo TODO TODO TODO For KANN trenger eg en bEndraKappaDennePerioden, som blir satt til false kvar fyring av auronet. XXX
	bool bEndraKappaDennePerioden;


	// For debugging: trenger ei liste over alle K_auron, slik at eg kan skrive log for depol kvar tidsiterasjon:
	// Legger til i constructor og fjærner i destructor (akkurat som for i_auron::pAllAurons)
	static std::list<K_auron*> pAllKappaAurons;


	//Overlager denne, siden eg må regne ut depol. TODO TODO TODO TODO OTDO OTOD TODD TODO TODO TODO
	/*const void skrivAktivitetsVarLogg(){
		// XXX XXX GÅR UT FRA AT DENNE KALLES ETTER NYTT TIDSVINDU!
	 	depol_logFile <<time_class::getTid() <<"\t" <<nDepolAtStartOfTimeWindow <<"; \t #depol\n" ;
	 	depol_logFile.flush();
	}*/

	public:
	K_auron(std::string sNavn_Arg ="unnamed", int nStartKappa = FYRINGSTERSKEL, unsigned uStartDepol_prosent =0); 	
	~K_auron();

//{2 friend
	friend class K_axon;
	friend class K_synapse;
	friend class K_dendrite;
	friend void testFunksjon_slett(s_auron*);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );

	friend int main(int, char**); //TODO SLETT
	friend void loggeFunk_K_auron();// TODO SAMME
//}1

}; // }






#endif /* AURON_H_ */
// vim:fdm=marker:fmr=//{,//}
