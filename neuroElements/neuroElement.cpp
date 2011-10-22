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
#include <sstream>

#include "synapse.h"
#include "auron.h"
#include "../andreKildefiler/main.h"
#include "../andreKildefiler/time.h"

std::ostream & operator<< (std::ostream& ut, i_auron* pAuronArg );




/*************************************************************
****** 													******
******  		CONTRUCTORS OG DESCTRUCTORS 			******
****** 													******
*************************************************************/

//{1 * AURON
//{2 *** callDestructorForAllAurons() og callDestructorForAllKappaAurons() 
void i_auron::callDestructorForAllAurons()
{
	// Først kall destructor for alle kappa-auron!
	K_auron::callDestructorForAllKappaAurons();

	// Kaller destructor for alle gjenværende udestruerte auron (alle ikkje-K_auron):
	// Sletter alle auron i i_auron::pAllAurons
	while( ! i_auron::pAllAurons.empty() )
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 2
		cout<<"Calls destructor for auron " <<i_auron::pAllAurons.front()->sNavn <<endl;
		#endif
		// remove element from pAllAurons.
	 	delete (*i_auron::pAllAurons.begin());
	}
}
void K_auron::callDestructorForAllKappaAurons()
{
	// Call destructor for all remaining K_aurons (listed in K_auron::pAllKappaAurons):
	while( ! K_auron::pAllKappaAurons.empty() )
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 2
		cout<<"Calls destructor for K_auron " <<K_auron::pAllKappaAurons.front()->sNavn <<endl;
		#endif

		// remove element from pAllAurons.
	 	delete (*K_auron::pAllKappaAurons.begin());
	}
}

//}2
//{2 *** i_auron   
i_auron::i_auron(std::string sNavn_Arg /*="unnamed"*/, double dStartAktVar /*=0*/) : timeInterface("auron"), dAktivitetsVariabel(dStartAktVar), sNavn(sNavn_Arg)
{

	// Printing to log file. Initiation of file stream and creation of a .oct file that is executable in octave.
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_auron" <<sNavn <<"-depol" <<".oct";

	std::string tempStr( tempFilAdr.str() );

	// need c-style string for open() function:
	depol_logFile.open( tempStr.c_str() );
	depol_logFile<<"data=[";
	depol_logFile.flush();

}
i_auron::~i_auron()
{
	cout<<"\tDESTRUCTOR: \ti_auron::~i_auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";

	// remove auron from pAllAurons:
	pAllAurons.remove(this);

	// Finalize octave script to make it executable:
	depol_logFile 	<<"];\n\n"
					<<"axis([0, " <<time_class::getTid() <<", 0, " <<FYRINGSTERSKEL*1.3 <<"]);\n"
					<<"plot( data([1:end],1), data([1:end],2), \".;Depolarization;\");\n"
					<<"title \"Depolarization for auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					<<"print(\'./eps/eps_auron" <<sNavn <<"-depol.eps\', \'-deps\');\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); "
					;
	depol_logFile.close();
}
//}2
//{2 *** s_auron
s_auron::s_auron(std::string sNavn_Arg /*="unnamed"*/, int nStartDepol /*=0*/) : i_auron(sNavn_Arg, nStartDepol)
{
	ulTimestampForrigeFyring = time_class::getTid();
	ulTimestampLastInput  = time_class::getTid();
	
	// Legger til auron* i std::list<i_auron*> pAllAurons;
	i_auron::pAllAurons.push_back( this );

	//pOutputAxon og pInputDendrite
	pOutputAxon = new s_axon(this);
 	pInputDendrite = new s_dendrite(this);
}
s_auron::~s_auron() // Blir ikkje kjørt automagisk i slutten av programmet. Da går i_auron::~i_auron();
{
	//Vil bare kalles dersom eit s_auron slettes før avslutting. Ved destruering av alle i_auron::pAllAurons kalles bare ~i_auron.
	cout<<"DESTRUCTOR:\ts_auron::~s_auron()\n";
	
	//i_auron tar seg av dette.. (???)
	delete pOutputAxon;
	delete pInputDendrite;

	//Etter ~s_auron() kalles ~i_auron()
} 
//}2
//{2 *** K_auron
K_auron::K_auron(std::string sNavn_Arg /*="unnamed"*/, double dStartKappa_arg /*= 0*/, unsigned uStartDepol_prosent /*=0*/) : i_auron(sNavn_Arg), kappaRecalculator(this)
{
	ulTimestampForrigeFyring = time_class::getTid();

	// Initierer første 'time window':
	ulStartOfTimewindow = time_class::getTid();
	dDepolAtStartOfTimeWindow = uStartDepol_prosent * FYRINGSTERSKEL;

	// Initierer andre medlemsvariabler:
	dChangeInKappa_this_iter = 0;
	dChangeInPeriodINVERSE = 0;
	dPeriodINVERSE = 0;

	// Legger til auron* i std::list<i_auron*> pAllAurons;
	pAllAurons.push_back( this );
	pAllKappaAurons.push_back( this );
	
	// Legger til auron* i std::list<timeInterface*> pPeriodicElements:
	time_class::addElementIn_pPeriodicElements( this );

//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
// XXX XXX AXON OG DENDRITE SKAL VEKK! XXX XXX
//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
	//pOutputAxon og pInputDendrite
	pOutputAxon = new K_axon(this);
 	pInputDendrite = new K_dendrite(this);

	// Initialiserer aktivitetsvariablene kappa til å være verdien dStartKappa_arg:
	changeKappa_derivedArg( dStartKappa_arg );

	// Utskrift til logg. LOGG-initiering (lag ei .oct fil som er kjørbar)
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_auron" <<sNavn <<"-kappa" <<".oct";
	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	kappa_logFile.open( tempStr.c_str() );
	kappa_logFile<<"data=[" <<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<";\n";
	kappa_logFile.flush();


 	cout 	<<"\nConstructor: K_auron.\n"
			<<"\tKappa: \t\t\t\t" <<dAktivitetsVariabel <<endl
			<<"\tTau:   \t\t\t\t" <<FYRINGSTERSKEL <<endl
			<<"\n\n";

	// Kjører auronets doCalculation() for å få rett verdier:
	doCalculation();

} 
K_auron::~K_auron()
{
	cout<<"Destructor:\tK_auron::~K_auron()\n";

	// pAllAurons - element fjærnes i i_auron. 
	// Fjærner pAllKappaAurons-element i K_auron::~K_auron() :
	pAllKappaAurons.remove(this);


//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
// XXX XXX AXON OG DENDRITE SKAL VEKK! XXX XXX
//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
	//i_auron tar seg ikkje av dette.. Sletter axon og dendrite:
	delete pOutputAxon;
	delete pInputDendrite;


	// Rett slutt på utskriftsfil-logg:
	kappa_logFile<<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"];\n"
					<<"axis([0," <<time_class::getTid() <<"])\n"
					<<"plot( data([1:end],1), data([1:end],2), \".;Kappa;\");\n"

					<<"title \"Activity variable for K-auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					<<"print(\'./eps/eps_auron" <<sNavn <<"-kappa.eps\', \'-deps\');\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); "
					;
	kappa_logFile.close();
}
//}2
//{2 *** K_sensor_auron
K_sensor_auron::K_sensor_auron( std::string sNavn_Arg , double (*pFunk_arg)(void) ) : K_auron(sNavn_Arg)
{
	// Assign the sensor function:
	pSensorFunction = pFunk_arg;
	// Add to pAllSensorAurons list:
	pAllSensorAurons.push_back(this);
	pAllAurons.push_back( this ); 

	// Tar første sample av pSensorFunction:
	dAktivitetsVariabel = (*pSensorFunction)();

	// Initialiserer kappaRecalculator i tilfelle sensorauron seinere skal få mulighet for input-synapser:
	kappaRecalculator.pKappaAuron_obj = this;

	recalculateKappa();
}
//}2
//{2 *** s_sensor_auron
s_sensor_auron::s_sensor_auron( std::string sNavn_Arg , double (*pFunk_arg)(void) ) : s_auron(sNavn_Arg)
{
	// Assign the sensor function:
	pSensorFunction = pFunk_arg;
	// Add to pAllSensorAurons list:
	pAllSensorAurons.push_back(this);
}
//}2
//}1 * AURON

//{1 * SYNAPSE
//{2 i_synapse
i_synapse::i_synapse(double dSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ ) : timeInterface(sKlasseNavn), bInhibitoryEffect(bInhibEffekt_Arg)
{
	dSynapticWeight = dSynVekt_Arg;
	dSynapticWeightChange = 0;
	
	cout<<"\tConstructor :\ti_synapse::i_synapse(unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, string navn);\n";
}
//}2
//{2 s_synapse
s_synapse::s_synapse(s_auron* pPresynAuron_arg, s_auron* pPostsynAuron_arg, double dSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			:  i_synapse(dSynVekt_Arg, bInhibEffekt_Arg, "s_synapse"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite) 
{

	cerr<<"Kaller s_synapse::s_synapse(" <<pPreNodeAxon->pElementOfAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementOfAuron->sNavn <<".pInputDendrite, ...)\n";

	// Legger til synapse som utsyn. i presyn. axon og innsyn. i postsyn. dendrite:
	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	
	// Initialisering av synapsens logg-fil:
	//{4 lag ei .oct - fil, og gjør klar for å kjøres i octave:
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_s_synapse_" <<pPresynAuron_arg->sNavn <<"-"  <<pPostsynAuron_arg->sNavn ;
	if(bInhibitoryEffect){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	synTransmission_logFile.open( tempStr.c_str() );
	synTransmission_logFile<<"data=[0 0;\n";
	synTransmission_logFile.flush();
	//}4

}
s_synapse::~s_synapse()
{
	bool bPreOk  = false;
	bool bPostOk = false;

	cout<<"\tDESTRUCTOR :\ts_synapse::~s_synapse() : \t";


	if( !bPreOk ){ 
		//fjærner seg sjølv fra prenode:
		for( std::list<s_synapse*>::iterator iter = (pPreNodeAxon->pUtSynapser).begin(); iter != (pPreNodeAxon->pUtSynapser).end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~( [" <<pPreNodeAxon->pElementOfAuron->sNavn <<"] -> "; 						// utskrift del 1
				(pPreNodeAxon->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		for( std::list<s_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
			if( *iter == this ){ 
				cout<<"[" <<pPostNodeDendrite->pElementOfAuron->sNavn <<"] )\t"; 						// utskrift del 2
				(pPostNodeDendrite->pInnSynapser).erase( iter );
				bPostOk = true;
				break;
			}
		}
	}

	// Vil aldri skje: Redundant feiltest for sikkerhetskkuld:
	if( (!bPreOk) || (!bPostOk) ){
		/// FEIL:
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i s_synapse destruksjon. (~s_synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNodeAxon->pElementOfAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementOfAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementOfAuron->sNavn <<" - " <<pPostNodeDendrite->pElementOfAuron->sNavn <<endl;
		exit(-9);	
	}
	cout<<endl;


	//{4 Rett slutt på utskriftsfil-logg:
	synTransmission_logFile<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Synaptic weight;\");\n"

					<<"title \"Synaptic weight for s_synapse: " <<pPreNodeAxon->pElementOfAuron->sNavn <<" -> " <<pPostNodeDendrite->pElementOfAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel syn.w.\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementOfAuron->sNavn <<"->" <<pPostNodeDendrite->pElementOfAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); "
					<<"print(\'eps__s_synapse_" <<pPreNodeAxon->pElementOfAuron->sNavn <<"->" <<pPostNodeDendrite->pElementOfAuron->sNavn <<".eps\', \'-deps\');" 	;
	synTransmission_logFile.close();
	//}4

}
//}2
//{2 K_synapse
K_synapse::K_synapse(K_auron* pPresynAuron_arg, K_auron* pPostsynAuron_arg, double dSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/ )
 :  i_synapse(dSynVekt_Arg * FYRINGSTERSKEL      , bInhibEffekt_Arg, "K_synapse") , pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite) 
{
	cout<<"Constructor :\tK_synapse::K_synapse(" <<pPreNodeAxon->pElementOfAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementOfAuron->sNavn <<".pInputDendrite, ...)\n";

// XXX XXX  TA VEKK axon OG dendrite XXX XXX
	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);


	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_transmission_K_synapse_" <<pPresynAuron_arg->sNavn <<"-"  <<pPostsynAuron_arg->sNavn ;
	if(bInhibitoryEffect){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );
	// trenger c-style string for open():
	synTransmission_logFile.open( tempStr.c_str() );
	synTransmission_logFile<<"data=[0 0;\n";
	synTransmission_logFile.flush();


	// Initierer overføring:
	doTask();
}

K_synapse::~K_synapse()
{
	bool bPreOk  = false;
	bool bPostOk = false;


	cout<<"\t\tDESTRUCTOR :\tK_synapse::~K_synapse() : \t";


	if( !bPreOk ){
		//fjærner seg sjølv fra prenode:
		for( std::list<K_synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~( [" <<pPreNodeAxon->pElementOfAuron->sNavn <<"] -> "; 				// utskrift del 1
				(pPreNodeAxon->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		for( std::list<K_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
			if( *iter == this ){ 
				cout<<"[" <<pPostNodeDendrite->pElementOfAuron->sNavn <<"] )\t"; 				// utskrift del 2
				(pPostNodeDendrite->pInnSynapser).erase( iter );
				bPostOk = true;
				break;
			}
		}
	}

	// Redundant test: Vil "aldri" skje. 
	if( (!bPreOk) || (!bPostOk) ){
		/// FEIL:
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i s_synapse destruksjon. (~s_synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNodeAxon->pElementOfAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementOfAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementOfAuron->sNavn <<" - " <<pPostNodeDendrite->pElementOfAuron->sNavn <<endl;
		exit(-9);	
	}
	cout<<endl;


	// Rett slutt på utskriftsfil-logg:
	synTransmission_logFile<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \".r;Synaptic transmission;\");\n"
					<<"title \"Synaptic transmission from s_synapse: " <<pPreNodeAxon->pElementOfAuron->sNavn <<" to " <<pPostNodeDendrite->pElementOfAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel Syn.Transmission\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_transmission_" <<pPreNodeAxon->pElementOfAuron->sNavn <<"->" <<pPostNodeDendrite->pElementOfAuron->sNavn <<".eps\', \'-deps\', \'-color\');\"\n"
					<<"print ./eps/eps_transmission_" <<pPreNodeAxon->pElementOfAuron->sNavn <<"->" <<pPostNodeDendrite->pElementOfAuron->sNavn <<".eps -deps -color\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); ";
	synTransmission_logFile.close();

	 
}
//}2
//}1 * SYNAPSE

//{1 * AXON
//{2 ***  i_axon
i_axon::i_axon( std::string sKlasseNavn ="dendrite") : timeInterface(sKlasseNavn)
{
}
i_axon::~i_axon()
{
	// pUtSynapser inneholder bare peikere, så pUtSynapser.clear() vil ikkje føre til destruksjon av synapsene. Kaller destruksjonen eksplisitt:
	while( !pUtSynapser.empty() ){
		// Kaller destruktor for første gjenværende synapse ved å frigjør det elementet som ligg i det frie lageret:
		// 		- Dette fører også til at synapsa fjærnes fra pUtSynapser (og dendrite.pInnSynapser)
		delete (*pUtSynapser.begin()); 
	}
}
//}2 *** i_axon
//{2 ***  s_axon
s_axon::s_axon(s_auron* pAuronArg) : i_axon("s_axon"), pElementOfAuron(pAuronArg)
{
	cout<<"\tConstructor :\ts_axon::s_axon(s_auron*)\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
}
s_axon::~s_axon()
{
	cout<<"\tDESTRUCTOR :\ts_axon::~s_axon() for auron \t" <<pElementOfAuron->sNavn <<"\n";
}
//}2
// TODO SKAL VEKK:
//{2 ***  K_axon
K_axon::K_axon( K_auron* pAuronArg) : i_axon("K_axon" ), pElementOfAuron(pAuronArg)
{
	cout<<"\tConstructor :\tK_axon::K_axon( K_axon*)\n";
}
K_axon::~K_axon()
{
	// Trenger å slette de her, siden de er overlagra her av typen K_axon (som har K_synapser, som har egen destructor som er viktig for avsluttinga av transmissionLog_K_synapse..
	while( !pUtSynapser.empty() ){
		delete (*pUtSynapser.begin()); //Kaller destruktoren til første gjenværende synapse. Dette fører også til at synapsa fjærnes fra pUtSynapser (og dendrite.pInnSynapser)
	}
}
//}2
//}1

//{1 * DENDRITE
//{2 *** i_dendrite
i_dendrite::i_dendrite(/*i_auron* pElementOfAuron_arg,*/ std::string sNavn ="dendrite") : timeInterface(sNavn)
{ 
	//pElementOfAuron = pElementOfAuron_arg;
}
// i_dendrite skal vekk: Innholdet skal over i s_dendrite::~s_dendrite og K_auron::~K_auron!
i_dendrite::~i_dendrite()
{
	// Destruerer alle innsynapser.
	while( !pInnSynapser.empty() ){
	 	delete (*pInnSynapser.begin() );
	}
}
//}2
// Skal gjøres om: ikkje lenger arve fra i_dendrite (?)
//{2 *** s_dendrite
s_dendrite::s_dendrite( s_auron* pPostSynAuron_Arg ) : i_dendrite("s_dendrite"), pElementOfAuron(pPostSynAuron_Arg)
{
	cout<<"\tConstructor :\ts_dendrite::s_dendrite( s_auron* )\n";

	bBlockInput_refractionTime = false;
}
s_dendrite::~s_dendrite()
{
	cout<<"\tDesructor :\ts_dendrite::~s_dendrite() \t for auron \t" <<pElementOfAuron->sNavn <<"\n";
}
//}2 s_dendrite 
// SKAL VEKK: (Ta inn i K_auron::K_auron)
//{2 *** K_dendrite
K_dendrite::K_dendrite( K_auron* pPostSynAuron_Arg ) : i_dendrite("K_dendrite" ), pElementOfAuron(pPostSynAuron_Arg)
{
	cout<<"\tConstructor :\tK_dendrite::K_dendrite( K_auron* ) \t\t[TOM]\n";
}
K_dendrite::~K_dendrite()
{
	cout<<"\tDesructor :\tK_dendrite::~K_dendrite() \t\t for auron \t" <<pElementOfAuron->sNavn <<"\n";
	// Destruerer her først: fordi synapser må avslutte logg..
	while( !pInnSynapser.empty() ){
	 	delete (*pInnSynapser.begin() );
	}
}
//}2 XXX K_dendrite
//}1 X DENDRITE



/*************************************************************
****** 													******
******  		ANDRE FUNKSJONER:           			******
****** 													******
*************************************************************/

//{1  * KANN
inline void K_dendrite::newInputSignal( double dNewSignal_arg )
{ //{2
	// TODO Legg inn spatiotemporal differensiering for ulike synapser. Dette gjør at eg må gjøre om heile strukturen til dette opplegget.
	pElementOfAuron->changeKappa_derivedArg( dNewSignal_arg );
} //}2

inline void K_auron::changeKappa_derivedArg( double dInputDerived_arg)//int derivedInput_arg )
{
	// Arg legges til Kappa no, og effektene av endringa kalkuleres i .doCalculation().
	dChangeInKappa_this_iter +=  dInputDerived_arg ;

	// Legger den i pCalculatationTaskQue, slik at effekt av all endring i kappa ila. tidsiterasjonen beregnes etter iterasjonen.
	time_class::addCalculationIn_pCalculatationTaskQue( this );

	writeKappaToLog();

	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout<<sNavn <<"\t:\tTid:\t" <<time_class::getTid() <<" ,\tKappa :\t" <<dAktivitetsVariabel <<endl;
	#endif
}


// XXX XXX XXX FARLIG ! Lager føkk når den endrer kappa, og doCalculation(). Kalkulerer v_0 fra gammel tid, men ny kappa. XXX IKKJE BRUK!
inline void K_auron::changeKappa_absArg(double dNewKappa)
{ //{
	cout<<"IKKJE Bruk changeKappa_absArg() ! Den innfører potensial for feil som ødelegger alt. Ikkje ferdig.\nAvlutter."; exit(-9);

	// Kanskje ikkje naudsynt, siden vi ikkje legger til element i pCalculatationTaskQue..
	//dChangeInKappa_this_iter = 0; // Hindrer .doCalculation() å endre kappa ytterligare.
	dAktivitetsVariabel = dNewKappa;

	// Foreløpig definerer eg eit sensorauron som en rein sensor (Tar ikkje imot input fra andre auron):
	// I dette tilfellet fjærner eg en potensiell feilkilde:
	dChangeInKappa_this_iter = 0;

	// Legger den i pCalculatationTaskQue, slik at effekt av all endring i kappa ila. tidsiterasjonen beregnes etter iterasjonen.
	time_class::addCalculationIn_pCalculatationTaskQue( this );
	
	writeKappaToLog();
} //}
//}1

//{1  * SANN

inline void s_dendrite::newInputSignal( double dNewSignal_arg )
{ //{2 .. }

	// Sjekker om input er blokkert grunnet 'refraction period':
 	if( bBlockInput_refractionTime ) return;

	// beregner lekkasje av depol siden sist:
	calculateLeakage();

	// Bli heilt sikker på ALPHA: XXX XXX XXX ALPHA XXX XXX xxx XXX
	pElementOfAuron->dAktivitetsVariabel +=  dNewSignal_arg * ALPHA;

	#if DEBUG_UTSKRIFTS_NIVAA > 2
	cout<<time_class::getTid() <<"\ts_dendrite::newInputSignal( " <<dNewSignal_arg <<" ); \t\tgir depol. :  " <<pElementOfAuron->dAktivitetsVariabel <<"\n";
	#endif
	
	pElementOfAuron->ulTimestampLastInput = time_class::getTid();


	// Dersom auron går over fyringsterskel: fyr A.P.
	if( pElementOfAuron->dAktivitetsVariabel > FYRINGSTERSKEL )
	{
		// Blokkerer videre input grunnet 'refraction time':
		bBlockInput_refractionTime = true;

		// Spatioteporal delay from AP initialization at axon hillock:
		time_class::addTaskIn_pWorkTaskQue( pElementOfAuron );
	}

	// Skriver til log for aktivitetsVar:
	pElementOfAuron->depol_logFile 	<<time_class::getTid() <<"\t" <<pElementOfAuron->dAktivitetsVariabel <<"; \t #Depolarization\n" ;
	pElementOfAuron->depol_logFile.flush();
} //}2

inline void s_dendrite::calculateLeakage()
{ //{2 /** Bare for SANN:  ***/

	if( pElementOfAuron->ulTimestampLastInput != time_class::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
	 	pElementOfAuron->dAktivitetsVariabel *= pow( (double)(1-ALPHA), (double)(time_class::getTid() - pElementOfAuron->ulTimestampLastInput) );
	
		// Gjøres i s_dendrite::newInputSignal(): 
		//ulTimestampLastInput = time_class::getTid(); 
	}
} //}2

//}1



/*************************************************************
****** 													******
******  		doTask() -- samla på en plass. 			******
****** 													******
*************************************************************/

//  		* 	SANN

/* 	s_auron::doTask() 	:  		Fyrer A.P. */
inline void s_auron::doTask()
{ //{ ** AURON

	if( ulTimestampForrigeFyring == time_class::getTid() )
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 1
		cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
		#endif
		return;
	}

	// FYRER A.P.:
	cout<<"\tS S " <<sNavn <<" | S | " <<sNavn <<" | S | S | S | | " <<sNavn <<" | S | | " <<sNavn <<" | S | | " <<sNavn <<"| S | S | S | S |\t"
		<<sNavn <<".doTask()\tFYRER neuron " <<sNavn <<".\t\t| S S | \t  | S |  \t  | S |\t\tS | " <<time_class::getTid() <<" |\n"
		<<endl;

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output:
	// bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	time_class::addTaskIn_pWorkTaskQue( pOutputAxon );


	// Registrerer fyringstid (for feisjekk (over) osv.) 
	ulTimestampForrigeFyring = time_class::getTid();


	//Resetter depol.verdi 
	dAktivitetsVariabel = 0; 

	writeAPtoLog();

} //}
/* 	s_axon::doTask()   	:		Simulerer delay i axonet 	*/
inline void s_axon::doTask()
{ //{ ** AXON

	// Avblokkerer dendritt. Opner den for meir input. Foreløpig er dette måten 'refraction time' funker på.. (etter 2 ms-dendrite og auron overføring..)
	pElementOfAuron->pInputDendrite ->bBlockInput_refractionTime = false;

	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"s_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementOfAuron->sNavn <<" - - - - - - - - - - - - - - - \n";
	#endif

	// Legger til alle utsynapser i pWorkTaskQue:
 	for( std::list<s_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i time_class::pWorkTaskQue: (FIFO-kø)
		time_class::addTaskIn_pWorkTaskQue( *iter );
	}

	 //Skriver til logg etter refraction-period.
	 pElementOfAuron->writeDepolToLog();
} //}
/*  s_synapse::doTask() : 		Simulerer overføring i synapse */
inline void s_synapse::doTask()
{ //{2 ** SYNAPSE

	// Dersom synapsen har inhibitorisk effekt: send inhibitorisk signal (subtraksjon). Ellers: eksiter postsyn. auron.
 	pPostNodeDendrite->newInputSignal( (1-2*bInhibitoryEffect) * 	( 1000 * dSynapticWeight )   );

	// XXX Kalkulere syn.p.?

	// Loggfører syn.weight
	synTransmission_logFile 	<<"\t" <<time_class::getTid() <<"\t" <<(1-2*bInhibitoryEffect) * dSynapticWeight
						<<" ;   \t#Synpaptic weight\n" ;
	synTransmission_logFile.flush();

} //}2
/* s_dendrite::doTask() : 		Simulerer delay for input ved dendrite */
inline void s_dendrite::doTask()
{ //{1 DENDRITE
	// s_dendrite::doTask() er en metode for å få inn spatiotemporal delay for neuronet (simulere ikkje-instant overføring)
	time_class::addTaskIn_pWorkTaskQue( pElementOfAuron );
} //}1

//			* 	KANN

/* K_auron::doTask() 	: 		Fyrer A.P. */
inline void K_auron::doTask()
{ //{1 ** AURON

	//Utskrift til skjerm:
	#if DEBUG_UTSKRIFTS_NIVAA > 0
	cout<<"\tK K " <<sNavn <<" | K | " <<sNavn <<" | K | K | K | | " <<sNavn <<" | K | | " <<sNavn <<" | K | | " <<sNavn <<"| K | K | K | K |\t"
		<<sNavn <<".doTask()\tFYRER neuron " <<sNavn <<".\t\t| K | |  [periode] = " <<dLastCalculatedPeriod/1000 <<"     | K | \tK | " <<time_class::getTid() <<" |\n"
		<<"\t| | Kappa:" <<dAktivitetsVariabel <<"\tForrige periode:" <<dLastCalculatedPeriod <<"\tDepol før fyring:" <<dDepolAtStartOfTimeWindow <<"\n"
		<<endl;
	#endif

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output. // bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	// TODO Dette kan gjøres direkte, for å slippe litt jobb: Dersom vi legger til 1 i fyringsestimatet, er ikkje axon-delay naudsynt.. TODO
	time_class::addTaskIn_pWorkTaskQue( pOutputAxon );

	// Initialiserer nytt 'time window':
	dDepolAtStartOfTimeWindow = 0;
	ulStartOfTimewindow = time_class::getTid();

	// Gjør kalkuleringer for å planlegge neste fyring.
	doCalculation();

	// Oppdaterer ulEstimatedTaskTime til [no] + dLastCalculatedPeriod:
	ulEstimatedTaskTime = time_class::getTid() + (unsigned long)dLastCalculatedPeriod;

	// Logger AP (vertikal strek)
	writeAPtoLog();
} //}1
// TODO SKAL VEKK:
inline void K_axon::doTask()
{ //{1 ** AXON
	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"K_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementOfAuron->sNavn <<" - - - - - - - - - - - - - - - \n";
	#endif

	// Overføring:
	doTransmission();
} //}1

// Delte opp for å kunne skille mellom å propagere kappa, og å fyre AP, for å kunne ha refraction period.
inline void K_axon::doTransmission()
{
	// Legg til alle utsynapser i pWorkTaskQue:
 	for( std::list<K_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i time_class::pWorkTaskQue: (FIFO-kø)
		time_class::addTaskIn_pWorkTaskQue( *iter );
	}
}
/* K_synapse::doTask() 	: 		Simulerer overføring i synapsen */
inline void K_synapse::doTask()
{ //{1 ** SYNAPSE

	// Istedenfor å sende inn pos. eller neg. signal avhengig av bInhibitoryEffect: [ 1-2*bInhibitoryEffect ]  Gir enten +1 eller -1. 
	pPostNodeDendrite->newInputSignal( (1-2*bInhibitoryEffect) * dSynapticWeight * (pPreNodeAxon->pElementOfAuron)->dChangeInPeriodINVERSE );

	
	double dPresynDeltaPeriodeINVERSE_temp =  (pPreNodeAxon->pElementOfAuron)->dChangeInPeriodINVERSE;
	#if DEBUG_UTSKRIFTS_NIVAA > 2
 	cout<<"K_synapse::doTask()\tdSynapticWeight: " <<dSynapticWeight 
		<<", preNode->dChangeInPeriodINVERSE: " <<dPresynDeltaPeriodeINVERSE_temp <<"\tOverføring: "
		<<(1-2*bInhibitoryEffect) * dSynapticWeight * dPresynDeltaPeriodeINVERSE_temp
		<<endl;
	#endif

	synTransmission_logFile
		<<time_class::getTid() <<"  \t"
		<<(1-2*bInhibitoryEffect) * dSynapticWeight * dPresynDeltaPeriodeINVERSE_temp
		<<" ; \n";
	synTransmission_logFile.flush();
} //}1

// TODO SKAL VEKK: 
inline void K_dendrite::doTask()
{ // DENDRITE (ikkje i bruk)
	// TODO Trur ikkje denne skal være med lenger. Legger til brutal feilsjekk: exit(-1);
	cout<<"\n\nBrutal feilsjekk @ K_dendrite::doTask() : exit(-1);\n\n";

	exit(-1);
} 

//}1            *       KANN slutt

// 		* 	time_class
/* time_class::doTask() 	: 		Organiserer tid: doTask() itererer tid og holder pWorkTaskQue og tid i orden etter definerte regler */
void time_class::doTask()
{ 	//{1 

	// gjennomfører planlagte kalkulasjoner:
	doCalculation();

	//itererer time:
	ulTime++;

	// utskrift:
	#if DOT_ENTER_UTSKRIFT_AV_TID // Lager fin .-linje for tid, og enter hver DOT_ENTER_UTSKRIFT_AV_TID'te punktum.
	cout<<".";
	if(ulTime % DOT_ENTER_UTSKRIFT_AV_TID == 0)
		cout<<endl;
	#endif

	#if UTSKRIFT_AV_TID
	if(ulTime % UTSKRIFT_AV_TID_KVAR_Nte_ITER  == 0)		
		cout<<"\t* * * * TID: \t  =  " <<ulTime <<" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * = "
			<<ulTime <<"\n";
	#endif


	/*******************************
	* Oppdater alle K_sensor_auron *
	*******************************/
	#if KANN
	K_sensor_auron::updateAllSensorAurons();
	#endif

	/*******************************
	* Oppdater alle s_sensor_auron *
	*******************************/
	#if SANN
	s_sensor_auron::updateAllSensorAurons();
	#endif

	
	/*************************************************
	* Flytter planlagde oppgaver over i pWorkTaskQue *
	*************************************************/
	for( std::list<timeInterface*>::iterator pPE_iter = pPeriodicElements.begin() ; pPE_iter != pPeriodicElements.end() ; pPE_iter++ )
	{
		if( (*pPE_iter)->ulEstimatedTaskTime == ulTime+1 )
		{
			addTaskIn_pWorkTaskQue( (*pPE_iter) );
			// Dette fører til eit kall til eit tidsElements doTask(). Teller antall kall (til rapporten):
			#if DEBUG_UTSKRIFTS_NIVAA > 2
			cout<<"Telte kall til " <<(*pPE_iter)->sClassName <<".doTask()\n";
			#endif
		}
 	}


	// Kaller loggeFunk for å teste K_auron. 
	K_auron::loggeFunk_K_auron();


	// Legger til egenpeiker på slutt av pNesteJobb_ArbeidsKoe
	pWorkTaskQue.push_back(this);	
}//}1





/******************************************************************
****** 														*******
****** 			doCalculation() -- samla på en plass 		*******
****** 														*******
******************************************************************/

void K_auron::doCalculation()
{ //{
	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"K_auron " <<sNavn <<".doCalculation()\t\t" <<sNavn <<".doCalculation()\t\tTid: " <<time_class::getTid() <<"\n";
	#endif

	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"[K, T] = " <<dAktivitetsVariabel <<", " <<FYRINGSTERSKEL <<endl;
	#endif

	// Viktig å kalkulere depol med GAMMEL Kappa! Ellers får vi hopp i depol!
	// Lagrer v_0 og t_0 for neste 'time window':
	dDepolAtStartOfTimeWindow = getCalculateDepol();
	ulStartOfTimewindow = time_class::getTid();
	
	// Oppdaterer Kappa
	dAktivitetsVariabel += dChangeInKappa_this_iter;
	dChangeInKappa_this_iter = 0;

	//***********************************************
	//*  Beregn estimert fyringstid:   				*
	//***********************************************
	if( dAktivitetsVariabel > FYRINGSTERSKEL){
		static double dPeriodInverse_static_local = 0;
	
		// Berenger dPeriodINVERSE og dChangeInPeriodINVERSE:
		dLastCalculatedPeriod  = (( log( dAktivitetsVariabel / (dAktivitetsVariabel - FYRINGSTERSKEL) ) / ALPHA) +0.5) 									+1 		; // +1 for å gi delay til dendrite (GJØRE TIL +2?)XXX
		dPeriodInverse_static_local = 1/dLastCalculatedPeriod;
		dChangeInPeriodINVERSE = dPeriodInverse_static_local - dPeriodINVERSE;
		dPeriodINVERSE = dPeriodInverse_static_local;

		#if DEBUG_UTSKRIFTS_NIVAA > 5
		cout<<"Beregner periode, osv:\tdPeriodINVERSE:\t" <<dPeriodINVERSE <<"\t dChangeInPeriodINVERSE: \t" <<dChangeInPeriodINVERSE <<endl;
		cerr<<dLastCalculatedPeriod <<", "
			<<dPeriodInverse_static_local <<", "
			<<dChangeInPeriodINVERSE <<", ";
		#endif


		// 	For K_auron trenger vi ikkje legge til elementet i [liste som skal sjekkes]. pAllKappaAurons sjekkes alltid.. 												+0.5 for rett avrunding.
		ulEstimatedTaskTime =( ( time_class::getTid() + log( (dAktivitetsVariabel-dDepolAtStartOfTimeWindow)/(dAktivitetsVariabel-FYRINGSTERSKEL) )   /  ALPHA )+0.5)  		+2 ; //+1 for å få delay i dendrite. 
																																												 //+1 for å også få delay i axon.
		/*
		* 	Her er eit problem:
		* 		Refraction time: Dersom vi bare legger til en, så vil oppladning starte umiddelbart. Når kappa blir endra, så vil oppladninga (som starter for tidlig) være det einaste som er viktig. Dette skaper problemer.
		* 		XXX XXX XXX
		*       ? ? ? ? ? ? 
		*/

	
	
	
		/* Propagerer resultatet:
		time_class::addTaskIn_pWorkTaskQue( pOutputAxon ); 
	 	 Er visst i neste section:
		 */

		// XXX
		// KJører synaptisk overføring kvar gang Kappa endres. Dette er ikkje optimalt. Men det vil nok bli rett. Mens eg utvikler Kappa-mekanismer..
		
		// Propagerer aktivitetsnivå. Gjør umiddelbart: (Dette må gjøres om dersom spatiotemporale effekter skal simuleres)
		pOutputAxon->doTransmission();

		// For else-biten når det skjer (sjå ned)
		bAuronHarPropagertAtDenErInaktiv = false;

	}else{
		// setter planlagt task time til no, slik at den aldri vil fyre pga. ulEstimatedTaskTime. (når den sjekker nest gang, så vil [no] være i fortida..)
		ulEstimatedTaskTime = time_class::getTid();
		
		// Setter dLastCalculatedPeriod, dChangeInPeriodINVERSE, dPeriodINVERSE.
		dLastCalculatedPeriod = 0; 	// Er dette greit?    SKUMMELT! (men funker).
		dChangeInPeriodINVERSE = -dPeriodINVERSE; 
		dPeriodINVERSE = 0;
		
		#if DEBUG_UTSKRIFTS_NIVAA > 4
			cout<<"Kappa er mindre enn Tau. Setter ulEstimatedTaskTime = [no] (vil ikkje ha noko å sei for fyringa).\n";
			cerr<<"Setter [dPeriodINVERSE, dPeriodInverse_static_local, dChangeInPeriodINVERSE, dLastCalculatedPeriod] til [ "
			 	<<dPeriodINVERSE <<","
				<<dChangeInPeriodINVERSE <<", "
				<<dLastCalculatedPeriod <<" ]\n ";
		#endif
		
		if( !bAuronHarPropagertAtDenErInaktiv )
		{
			// Propagerer resultatet:
			time_class::addTaskIn_pWorkTaskQue( pOutputAxon ); 
	 		
			bAuronHarPropagertAtDenErInaktiv = true;
		}
	}


	// Dersom Kappa endres slik at node vil fyre neste iter (sjeldent), så fanges dette opp her:
	if( ulEstimatedTaskTime == time_class::getTid()+1)
		time_class::addTaskIn_pWorkTaskQue( this );

	// Skriver til log for depol:
	writeDepolToLog();
} //}
	
/**************************************************************
****** 			K_sensor_auron - senseFunksjoner        *******
**************************************************************/
inline void K_sensor_auron::updateAllSensorAurons()
{ //{

	// Itererer gjennom lista pAllSensorAurons, og kaller updateSensorValue() for de.
	for( std::list<K_sensor_auron*>::iterator sensorIter = pAllSensorAurons.begin() 	; 	sensorIter != pAllSensorAurons.end() ; sensorIter++)
	{
		(*sensorIter)->updateSensorValue();
	}
} //}1

inline void K_sensor_auron::updateSensorValue()
{ //{
	#if DEBUG_UTSKRIFTS_NIVAA > 2
	DEBUG("K_sensor_auron::updateSensorValue()");
	#endif

	// To variabler for å finne deriverte. Denne skal bestemme ny kappa..
	dLastSensedValue = dSensedValue;
	dSensedValue = /* ALPHA*  */ (*pSensorFunction)(); //TODO Finn ut om denne er gange ALPHA.

	if( dSensedValue != dLastSensedValue){
		//changeKappa_absArg( dSensedValue ); XXX FARLIG! IKKJE BRUK changeKappa_absArg() !
		changeKappa_derivedArg( dSensedValue-dLastSensedValue );
	}

	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout<<"Kappa for K_sensor_auron: " <<dAktivitetsVariabel <<"\n\n";
	#endif
} //}
	

/**************************************************************
****** 			s_sensor_auron - senseFunksjoner        *******
**************************************************************/
inline void s_sensor_auron::updateAllSensorAurons()
{ //{

	// Itererer gjennom lista pAllSensorAurons, og kaller updateSensorValue() for de.
	for( std::list<s_sensor_auron*>::iterator sensorIter = pAllSensorAurons.begin() 	; 	sensorIter != pAllSensorAurons.end() ; sensorIter++)
	{
		(*sensorIter)->updateSensorValue();
	}
} //}


inline void s_sensor_auron::updateSensorValue()
{ //{
	static double sdLastValue = 0;
	static double sdValue = 0;
	sdLastValue = sdValue;
	sdValue = (*pSensorFunction)();
	pInputDendrite->newInputSignal( (  sdValue )); // XXX OPPMERKSOMHET!    Sender inn umiddelbart sensa signal, ikkje den deriverte.  (Sjekk dette).
} //}


/******************************
**   Recalculate Kappa :     **
******************************/
void recalcKappaClass::doTask()
{ //{

	// Rekalkuler Kappa.
	// Trenger en funksjon K_auron::recalculateKappa() som
	// 	- rekalkulerer kappa for auronet.
	// 	- returnerer rekalkulert kappa.
	double dFeil;

	dFeil = pKappaAuron_obj->recalculateKappa();

	static double sdSecondLastValue =0;
	static double sdLastValue 		=0;
	static double sdValue 			=0;
	static double sdTemp;

	// Beregner når neste oppdatering av recalcKappaClass-kjøring:
	// Med sigmoid-kurve (beskrevet i FDP-rapport, med 3-ledds FIR-effekt)
	sdTemp = sdValue;
	sdValue = ( (RECALC_c1+RECALC_c2) - (RECALC_c2 / ( 1+exp(-(RECALC_c4*dFeil - RECALC_c3)) )) 		+ 	sdLastValue + sdSecondLastValue )  / 3 ;
	sdSecondLastValue = sdLastValue;
	sdLastValue = sdTemp;
	
	#if DEBUG_UTSKRIFTS_NIVAA > 2
	cout<<pKappaAuron_obj->sNavn <<" - recalcKappaClass::doTask() :\tny periode for recalc kappa: \t" <<sdValue <<"\t\tfor feil: " <<dFeil <<"\t\tTid:\t" <<time_class::getTid() <<endl;
	#endif

	// Schedule recalcKappaClass til å kjøre på kalkulert tid:
	ulEstimatedTaskTime = time_class::getTid() + sdValue;

} //}

inline double K_auron::recalculateKappa()
{ //{
	// Plan:
	//  - Rekalkulerer kappa for dendrite.
	// 		- skal hente ut K_ij fra alle innsynapsene. Dette kan den gjøre ved å kalle K_synapse::getWij();
	double dKappa_temp = pInputDendrite->recalculateKappa();
	double dKappaFeil_temp = dAktivitetsVariabel - dKappa_temp;

	// TESTER: Blir bare bullshit! XXX
	// Test på nytt! (skrevet: 22.10.2011)
	double dKappa_derived_temp = 0;
 	for( std::list<K_synapse*>::iterator iter = pInputDendrite->pInnSynapser.begin() ; iter!=pInputDendrite->pInnSynapser.end() ; iter++)
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 2
		cout<<"Derived Transmission: " <<(*iter)->getDerivedTransmission() <<endl;
		#endif
 		dKappa_derived_temp += (*iter)->getDerivedTransmission();
	}
	// TIL her.

	#if DEBUG_UTSKRIFTS_NIVAA > 2
	cout<<"[Kappa, dKappa_temp, dKappaFeil_temp] : " <<dAktivitetsVariabel <<", " <<dKappa_temp <<", " <<dKappaFeil_temp <<"\tderived-transmission: " <<dKappa_derived_temp
		<<" => Kappa+transmission = " <<dAktivitetsVariabel+dKappa_derived_temp
		<<endl;
	#endif
	dAktivitetsVariabel = dKappa_temp;


	return dKappaFeil_temp;	
} //}

inline double K_sensor_auron::recalculateKappa()
{
	// TODO No er dette bare en sensor (Har ikkje muligheten for å få input fra andre neuron. Dette kan eg kanskje implementere om eg har tid..)
	#if 0
	static double dOldActivityVariable;
	dOldActivityVariable = dAktivitetsVariabel;
	updateSensorValue();
	// Er dette rett :
	return dAktivitetsVariabel-dOldActivityVariable;
	#endif
	// Gjør det lett: (sensorauron har ikkje muligheten til å få input (enda))
	return 0;
}
// SKAL VEKK:
inline double K_dendrite::recalculateKappa()
{ //{
	double dKappa_temp = 0;
 	for( std::list<K_synapse*>::iterator iter = pInnSynapser.begin() ; iter!=pInnSynapser.end() ; iter++)
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 4
		cout<<"Total Transmission: " <<(*iter)->getTotalTransmission() <<endl;
		#endif
 		dKappa_temp += (*iter)->getTotalTransmission();
	}
	return dKappa_temp;
} //}


/*******************
**     ANNA:      **
*******************/

const double K_synapse::getDerivedTransmission()
{
	return ((pPreNodeAxon->pElementOfAuron)->dChangeInPeriodINVERSE)*dSynapticWeight;
}
const double K_synapse::getTotalTransmission()
{
	return (1-2*bInhibitoryEffect)*((pPreNodeAxon->pElementOfAuron)->dPeriodINVERSE)*dSynapticWeight;
}

inline void K_synapse::skrivUt()
{ //{
 	cout<<"Synapse mellom " <<(pPreNodeAxon->pElementOfAuron)->sNavn <<" og " <<(pPostNodeDendrite->pElementOfAuron)->sNavn 
		<<" med W_ij = " <<dSynapticWeight 
		<<" og presyn. periodeINVERSE: "  <<(pPreNodeAxon->pElementOfAuron)->dChangeInPeriodINVERSE <<" \t Gir (total) overføring: " <<getTotalTransmission() 
		<<endl;

} //}


// vim:fdm=marker:fmr=//{,//}
