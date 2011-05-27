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
#include <fstream> //filestreams

#ifndef SYNAPSE_H_
#define SYNAPSE_H_

#include "../neuroElements/auron.h"
#include "../andreKildefiler/time.h"

//#include "../andreKildefiler/aktivitetsObj.h"
class i_axon;
class s_axon;
class K_axon;
class i_dendrite;
class s_dendrite;
class K_dendrite;
class s_auron;
class K_auron;


// <<interface>>
class i_synapse : public timeInterface{



	virtual inline void doTask() =0; //XXX Reint virtuell klasse!
	
	// XXX XXX XXX Utsetter doCalculation() for alle synapser: (definerer den til å gjøre ingenting her for å unngå at klassene under blir abstract..)
	virtual void doCalculation() {}
	
	protected:
	const bool bInhibitorisk_effekt;
	double dSynapticWeight;
	double dSynapticWeightChange;

// Ta vekk neste to. Dette skal være double.
//	unsigned uSynapticWeight_promille;
//	int nSynapticWeightChange_promille;


	std::ofstream synWeight_loggFil;

	// Overlagrer i underklasseene, s_synapse og K_synapse. 	 Gjør at underelement som er sammenkobbla til samme s_[element], veit dette.
 	i_axon* pPreNodeAxon;
	i_dendrite* pPostNodeDendrite; 


	public:
	// Lag også axon.nySynapse(auron* pOutputAuron){  new synapse(this, pOutputAuron); } XXX

	//i_synapse(i_axon* pPresynAxon_arg, i_dendrite* pPostsynDendrite_arg, unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ );
	i_synapse(double dSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ );
	//~i_synapse();  TRUR IKKJE DENNE TRENGS. Ingen peikere i i_synapse ?
	// KANSKJE EG SKAL HA UTSKRIFT HER? --avslutting av loggen for synapsen..  TODO
	
	double getSynVekt(){ return dSynapticWeight; }

	friend int make(int, char**);
	friend std::ostream & operator<< (std::ostream & ut, i_axon* );
 	friend std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );

	//friend class s_synapse;
};



class s_synapse : public i_synapse{

	inline void doTask();

	// Overlagrer i underklasseene, s_synapse og K_synapse. 	 Gjør at underelement som er sammenkobbla til samme s_[element], veit dette.
 	s_axon* pPreNodeAxon;
	s_dendrite* pPostNodeDendrite; 


	public:
	s_synapse(s_auron*, s_auron*, double dSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~s_synapse();

	friend std::ostream & operator<< (std::ostream & ut, s_axon* ); //XXX gjør om arg: (..., s_axon* => i_axon*) XXX
};



class K_synapse : public i_synapse{
	inline void doTask();
	
	//unsigned forrigeUtregnaPresynPeriode;
	//unsigned uKappa;

	// Overlagrer peikarane, for å få rett preElement og postElement for synapsen (av rett modell). 	 Gjør at underelement som er sammenkobbla til samme s_[element], veit dette.
 	K_axon* pPreNodeAxon;
	K_dendrite* pPostNodeDendrite; 

	// TEST: & : inline unsigned& regnutPresynPeriode();
	// Optimalisering? XXX ha denne i presyn istedenfor. Det er en presyn, men mange synapser..
	// Beste er kanskje å regne ut ved AP, og lagre i axon. Kan hentes av synapsene derretter..
	//inline unsigned regnutPresynPeriode();

	double dPresynPeriodINVERSE;
	const inline double getTransmission(){ 	return dPresynPeriodINVERSE*dSynapticWeight; }

	public:
	K_synapse(K_auron*, K_auron*, unsigned uSynVekt_Arg =1, bool bInhibEffekt_Arg =false) ;
	~K_synapse();

	friend class K_dendrite;
	friend class K_auron;
	friend class K_axon;

	friend int main(int, char**);
};

#endif
// vim:fdm=marker:fmr=//{,//}
