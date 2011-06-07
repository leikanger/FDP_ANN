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

/*******************
recalcKappaClass brukes for å rekalkulere kappa for K_auron. Dette for å fjærne integralfeil..
*******************/
class recalcKappaClass : public timeInterface
{
	public:
	recalcKappaClass(K_auron* pKnyttaTilKappaAuron_arg) : timeInterface("Kappa-recalc. obj."), pKappaAuron_obj(pKnyttaTilKappaAuron_arg){
		//ulEstimatedTaskTime_for_object = MIN_PERIODE_MELLOM_REKALKULERING_AV_KAPPA;	
		ulEstimatedTaskTime_for_object = 1; // Denne blir overskrevet første gang den kjører.

		// Fjærn neste linje (om eg ikkje finner en nytte for pAllRecalcObj-liste).
		pAllRecalcObj.push_back(this);

		// Legger til element-peiker i std::list<timeInterface*> pPeriodicElements:
		time_class::addElementIn_pPeriodicElements( this );
	}

	inline void doTask();
	virtual void doCalculation(){
		// Bruker bare .doTask() ... enda.
		cout<<"Brutal Feilsjekk. recalculateKappa::doCalculation() er ikkje i bruk. Feilmelding dj331 @ neuroElemen.cpp\n\n\n";
		exit(0);
	}

	K_auron* pKappaAuron_obj;
	// Og fra timeInterface:  long uulEstimatedTaskTime_for_object; 

	static std::list<recalcKappaClass*> pAllRecalcObj;

	friend class K_auron;
	//friend class K_sensor_auron;
};



/*********************
**   	Auron 		**
*********************/

class i_auron : public timeInterface
{ 	//{		
	// Variablana pOutputAxon og pInputDendrite overlagres i underklassene. F.eks. i s_auron lages pOutputAxon som en s_axon*. 
	// 	   Effekten av dette blir at alle funksjoner og variabler fra i_axon kan kalles fra utsida (for i_axon--peikerar), mens også de modellspesifikke kan kalles fra andre modellspesifikke element (s_axon-funker kan kalles fra s_auron)!

	//Deler av auronet: (Ligger som s_axon og s_dendrite i s_auron. Samme for K_auron..) TODO SKal ligge der også ?
	i_axon* pOutputAxon; 			// Trenger å ha dei meir spesifikk for contruction av bl.a. synapse - s_synapse legger til pElementAvAuron->pInputDendrite (som må være av typen ?? XXX prøver igjen..
 	i_dendrite* pInputDendrite; 

	
	
	
	// Treng eg desse i i_auron? Bare for SANN? Vettafaen! XXX Kan kanskje ligge i s_auron.

	// BARE FOR SANN: ... og for KANN trenger eg en b EndraKappaDennePerioden, som blir satt til false kvar fyring av auronet. XXX
	unsigned long ulTimestampForrigeInput; 	 //Er begge naudsynt? sjå gjennom!
	// FOR BEGGE (SANN og KANN)
	unsigned long ulTimestampForrigeFyring;  //Er begge naudsynt? sjø gjennom!


	//dopamin: for å styre synaptisk plastisitet. En ide inspirert av naturen.
	//også her kan eg bruke \kappa for å finne nivået i kvart neuron..


	std::ofstream depol_logFile;
	
	// For å lage fin vertikal "strek" ved AP:
	inline virtual const void writeAPtoLog()
	{
		// Lager en vertikal "strek" fra v=0 til v=Terskel*(110%)
		for(float fTerkelProsent_temp = 0; fTerkelProsent_temp<1.2; fTerkelProsent_temp+=0.001)
		{
			depol_logFile 	<<time_class::getTid() <<"\t" <<fTerkelProsent_temp*FYRINGSTERSKEL <<"; \t #Depol\n" ;
		}
	 	depol_logFile.flush();
	}


	protected:
	virtual inline void doTask() =0;

	//container som inneholder alle auron som har vore til ila. programkjøringa:
	static std::list<i_auron*> pAllAurons;
	//static mdl. funk som destruerer alle i denne lista, men først de modellspesifikke K_auron og s_auron.
	static void callDestructorForAllAurons();

	// aktivitetsobjekt: Om dette er KANN eller SANN er avhengig av kva dAktivitetsVariabel skal bety (kappa eller depol..).
	double dAktivitetsVariabel;

	public:
	i_auron(std::string sNavn_Arg ="unnamed", double dStartAktVar = 0); 		//: timeInterface("auron"), ao_AuronetsAktivitet(this), sNavn(sNavn_Arg) {
	~i_auron();

	std::string sNavn; //for utskrift
	const std::string getNavn(){ return sNavn; }

	int getAktivityVar(){ return dAktivitetsVariabel; }

	// Overlagres forskjellig i s_auron og K_auron for å finne depol.
	virtual inline const void writeDepolToLog() =0;

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

//void slettTESTfunk(){ cout<<"s_auron::TESTfunk()\n"; } 			//TODO TODO TDOD TODO SLETT
	inline void doTask();
	inline void doCalculation() { cout<<"s_auron::doCalculation()\n";} 		//XXX UTSETTER. Foreløpig gjør denne ingenting (anna enn å gjøre at s_auron ikkje er abstract)

	protected:
	//Deler av auronet: 		OVERLAGRA fra i_auron
	s_axon* pOutputAxon; 			// Overlagrer i_auron::i_axon til s_auron::s_axon. Dette er alternativ til å caste pOutputAxon ved accessering til s_auron::pOutputAxon
 	s_dendrite* pInputDendrite;  	// Samme for pInputDendrite.

	public:
	s_auron(std::string sNavn_Arg ="unnamed", int nStartDepol = 0); 	
	~s_auron();


	inline virtual const void writeDepolToLog()
	{
	 	depol_logFile 	<<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"; \t #Depol\n" ;
	 	depol_logFile.flush();
	}
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
	
	// Kappa - loggfil:
	std::ofstream kappa_logFile;


	double dChangeInKappa_this_iter;

	inline void doTask();
	inline void doCalculation();

	double dDepolAtStartOfTimeWindow;
	unsigned long ulStartOfTimewindow;

	double dLastCalculatedPeriod;
	double dPeriodINVERSE;
	double dChangeInPeriodINVERSE;

	inline double getKappa(){ return dAktivitetsVariabel; }
	
	//bool bKappaLargerThanThreshold_lastIter;

	// todo TODO TODO TODO For KANN trenger eg en bEndraKappaDennePerioden, som blir satt til false kvar fyring av auronet. XXX
	//bool bEndraKappaDennePerioden;


	// For debugging: trenger ei liste over alle K_auron, slik at eg kan skrive log for depol kvar tidsiterasjon:
	// Legger til i constructor og fjærner i destructor (akkurat som for i_auron::pAllAurons)
	static std::list<K_auron*> pAllKappaAurons;
	static void callDestructorForAllKappaAurons();

	bool bAuronHarPropagertAtDenErInaktiv;

	protected:
	inline void changeKappa( double );
	// Rekalkulerer feil i Kappa for auronet.

	inline virtual double recalculateKappa();
	recalcKappaClass kappaRecalculator;

	//Liste over alle Kappa auron: 	
	std::list<K_auron*> pAlleKappaAuron;

	public:
	K_auron(std::string sNavn_Arg ="unnamed", double dStartKappa_arg = 0, unsigned uStartDepol_prosent =0); 	
	~K_auron();


	inline const double getCalculateDepol()
	{
		#if 0
		/*DEBUG*/cout<<"Depol: " <<(dDepolAtStartOfTimeWindow - dAktivitetsVariabel)*exp(-(double)ALPHA  * ((time_class::getTid() - ulStartOfTimewindow))) + dAktivitetsVariabel ;
		cout<<"dDepolAtStartOfTimeWindow: " <<dDepolAtStartOfTimeWindow <<"\tulStartOfTimeWindow: " <<ulStartOfTimewindow <<endl
			<<"dAktivitetsVariabel:\t" <<dAktivitetsVariabel <<"\ttid: " <<time_class::getTid() <<endl;
		#endif

		double dDepol_temp = (dDepolAtStartOfTimeWindow - dAktivitetsVariabel)*exp(-(double)ALPHA  * (time_class::getTid() - ulStartOfTimewindow)) + dAktivitetsVariabel ;

		#if DEBUG_UTSKRIFTS_NIVAA > 5
		cerr<<sNavn <<"\t";
		cerr<<"( " <<dDepol_temp <<" - " <<dAktivitetsVariabel <<" )*exp(-" <<ALPHA <<" * ( " <<time_class::getTid() <<" - " <<ulStartOfTimewindow <<" ) ) + " <<dAktivitetsVariabel 
			<<" \t=\t\t" <<dDepol_temp <<"\n\n";
		usleep(70000);
		#endif

		#if 1 // Har testa [litt] og sett at dette skjer typisk når noden er estimert å fyre neste iter. Ta vekk test for å optimalisere.
		//{
		if(dDepol_temp > FYRINGSTERSKEL){
			cout<<"depol over fyringsterskel.\tEstimert fyretid/[notid] :" <<ulEstimatedTaskTime_for_object <<"/" <<time_class::getTid() <<endl;
			//cout<<"Avslutter..\n\n"; 			exit(0);

			// TODO TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO 
			// Dette er quick-fix. Dårlig stil!
			time_class::addTaskIn_pWorkTaskQue( this );
			//doTask();
		} //}
		#endif
		return dDepol_temp;
	}

	
	const inline void writeDepolToLog()
	{
		// Skriver dDepolAtStartOfTimeWindow til logg:
		depol_logFile 	<<time_class::getTid() <<"\t" <<getCalculateDepol() <<"; \t #Depol\n" ;
		depol_logFile.flush();
	}
	const inline void writeKappaToLog()
	{
		// Skriver dDepolAtStartOfTimeWindow til logg:
		kappa_logFile 	<<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"; \t #Kappa\n" ;
		kappa_logFile.flush();
	}

	static const inline void loggeFunk_K_auron()
	{
		// DEBUG: Skriver depol og kappa til log for alle K_auron:
		for( std::list<K_auron*>::iterator iter = K_auron::pAllKappaAurons.begin(); iter != K_auron::pAllKappaAurons.end(); iter++ )
		{
			// Denne er kjøres inne i writeDepolToLog() :  (*iter) ->calculateDepol();
			(*iter) ->writeDepolToLog();

			//(*iter) ->recalculateKappa() ELLER NOKE. TODO TODO TODO TODO TODO TODO TODO TODO TODO 
			(*iter) ->writeKappaToLog();
		}
	}


//{2 friend
	friend class i_auron;
	friend class K_axon;
	friend class K_synapse;
	friend class s_synapse;
	friend class K_dendrite;
	friend class recalcKappaClass;

	friend class time_class;

	friend void neuroElement_testFunk(K_auron*);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );

	friend int main(int, char**); //TODO SLETT
	friend void loggeFunk_K_auron();// TODO SAMME

	friend void* taskSchedulerFunction(void* );
//}1

}; // }

class s_sensor_auron : public s_auron{
	// Function pointer:
	double (*pSensorFunction)(void);

	static std::list<s_sensor_auron*> pAllSensorAurons;

	inline void updateSensorValue();
	static void updateAllSensorAurons();
	
	public:
		s_sensor_auron( std::string sNavn_Arg ,   double (*pFunk_arg)(void) );

		double getSensedValue()
		{
	 		return (*pSensorFunction)();
		}

	friend class time_class;
};

class K_sensor_auron : public K_auron{
	// Function pointer:
	double (*pSensorFunction)(void);

	double dSensedValue;
	double dLastSensedValue;

	static std::list<K_sensor_auron*> pAllSensorAurons;

	inline void updateSensorValue();
	static void updateAllSensorAurons();
	
	protected:
	inline double recalculateKappa();

	public:
		K_sensor_auron( std::string sNavn_Arg ,   double (*pFunk_arg)(void) );

		double getSensedValue()
		{
	 		return (*pSensorFunction)();
		}

	friend class time_class;
};








#endif /* AURON_H_ */
// vim:fdm=marker:fmr=//{,//}
