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
#include <sstream> //For skriving til fil: logg.

#include "synapse.h"
#include "auron.h"
#include "../andreKildefiler/main.h"
#include "../andreKildefiler/time.h"

// XXX ulTimestampForrigeFyring brukes ikkje. Kan taes vekk.


std::ostream & operator<< (std::ostream & ut, i_auron* pAuronArg );
//std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );
std::ostream & operator<<(std::ostream& ut, i_auron* pAuronArg );

// Testfunk fra main.cpp
void timeClassTestFunk_som_kjoeres_kvar_tidsIter();



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

/****************************************** Kaller destructor for alle gjenværande udestruerte auron ***************************************/
	// Sletter alle auron i i_auron::pAllAurons
	while( ! i_auron::pAllAurons.empty() )
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 2
		cout<<"Calls destructor for auron " <<i_auron::pAllAurons.front()->sNavn <<endl;
		#endif
		// remove element from pAllAurons.
	 	delete (*i_auron::pAllAurons.begin());
	}
/********************************************************************************************************************************************/

}
void K_auron::callDestructorForAllKappaAurons()
{
/****************************************** Kaller destructor for alle gjenværande udestruerte auron ***************************************/
	// Sletter alle auron i K_auron::pAllKappaAurons
	while( ! K_auron::pAllKappaAurons.empty() )
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 2
		cout<<"Calls destructor for K_auron " <<K_auron::pAllKappaAurons.front()->sNavn <<endl;
		#endif

		// remove element from pAllAurons.
	 	delete (*K_auron::pAllKappaAurons.begin());
	}
/********************************************************************************************************************************************/
}


//}2
//{2 *** i_auron   
i_auron::i_auron(std::string sNavn_Arg /*="unnamed"*/, double dStartAktVar /*=0*/) : timeInterface("auron"), dAktivitetsVariabel(dStartAktVar), sNavn(sNavn_Arg)
{ //{3
	#if DEBUG_UTSKRIFTS_NIVAA > 2
	cout<<"CONSTRUCTOR: Lager auron med navn " <<sNavn <<endl;
	#endif


	// Utskrift til logg. LOGG-initiering (lag ei .oct fil som er kjørbar)
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_auron" <<sNavn <<"-depol" <<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	depol_logFile.open( tempStr.c_str() );
	depol_logFile<<"data=[";// Kan lage trøbbel for K_auron:   <<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<";\n";
	depol_logFile.flush();

}  //}3
i_auron::~i_auron()
{ //{3
	cout<<"\tDESTRUCTOR: \ti_auron::~i_auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";

	//fjærner seg fra pAllAurons   Listevariant:
	pAllAurons.remove(this);

	// (( Skriver eit punkt til depol-logg for å få rett lengde på plottet. ))
	// Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
	depol_logFile 	// Lager en prikk som er på Kappa, for K_auron:  <<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"];\n"
				 	//<<time_class::getTid() <<"\t" <<XXX Funker ikkje i i_auron: getDepol() <<"; \t #Last point from destructor\n" 
					<<"];\n\n"
					<<"axis([0, " <<time_class::getTid() <<", 0, " <<FYRINGSTERSKEL*1.3 <<"]);\n"
					<<"plot( data([1:end],1), data([1:end],2), \".;Depolarization;\");\n"

					<<"title \"Depolarization for auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					<<"print(\'./eps/eps_auron" <<sNavn <<"-depol.eps\', \'-deps\');\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); "
					;
	depol_logFile.close();

} //}3
//}2
//{2 *** s_auron
s_auron::s_auron(std::string sNavn_Arg /*="unnamed"*/, int nStartDepol /*=0*/) : i_auron(sNavn_Arg, nStartDepol)
{ //{3
	ulTimestampForrigeFyring = time_class::getTid();
	ulTimestampLastInput  = time_class::getTid();
	// Osv.
	
	// Legger til auron* i std::list<i_auron*> pAllAurons;
	i_auron::pAllAurons.push_back( this );

	// Sørger for ei initiell fyring: (bl.a. for å lage like plott for KN og SN)
	//time_class::addTaskIn_pWorkTaskQue( this );

	//pOutputAxon og pInputDendrite
	pOutputAxon = new s_axon(this);
 	pInputDendrite = new s_dendrite(this);

} //}3 	
s_auron::~s_auron() // Blir ikkje kjørt automagisk i slutten av programmet. Da går i_auron::~i_auron();
{ //{3
	// OBS OBS XXX SJÅ i_auron::~i_auron XXX Ikkje her..
	// Denne destructoren kalles ikkje automatisk. Dersom auronet ikkje blir destruert underveis, destrueres auron som eit i_auron på slutten av programmet.
	// Da vil ikkje denne koden kjøre..

	//Vil bare kalles dersom eit s_auron slettes før avslutting. Ved destruering av alle i_auron::pAllAurons kalles bare ~i_auron.
	cout<<"DESTRUCTOR:\ts_auron::~s_auron()\n";
	
	//Skriver eit punkt til depol-logg for å få rett lengde på plottet:
	 //depol_logFile 	<<time_class::getTid() <<"\t" <<0 <<"; \t #Last point from destructor\n" ;
	 //depol_logFile.flush();

	//i_auron tar seg av dette.. (?) test! XXX
	delete pOutputAxon;
	delete pInputDendrite;

	//Etter ~s_auron() kalles ~i_auron()

} //}3
//}2
//{2 *** K_auron
K_auron::K_auron(std::string sNavn_Arg /*="unnamed"*/, double dStartKappa_arg /*= 0*/, unsigned uStartDepol_prosent /*=0*/) : i_auron(sNavn_Arg), kappaRecalculator(this)
{ //{
	ulTimestampForrigeFyring = time_class::getTid();
	//ulTimestampLastInput  = time_class::getTid();

	ulStartOfTimewindow = time_class::getTid();
	dDepolAtStartOfTimeWindow = uStartDepol_prosent * FYRINGSTERSKEL;


	dChangeInKappa_this_iter = 0;

	dChangeInPeriodINVERSE = 0;
	dPeriodINVERSE = 0;

	// Sjå auron.h
	//bEndraKappaDennePerioden = false;

	// Sjå dokumentasjon for meir om bKappaLargerThanThreshold_lastIter
	//bKappaLargerThanThreshold_lastIter = false;
	
	// Legger til auron* i std::list<i_auron*> pAllAurons;
	pAllAurons.push_back( this ); 		// evt. i_auron::pAllAurons her og bare pAllAurons på neste linje.
	pAllKappaAurons.push_back( this );
	
	// Legger til auron* i std::list<timeInterface*> pPeriodicElements:
	time_class::addElementIn_pPeriodicElements( this );

	//pOutputAxon og pInputDendrite
	pOutputAxon = new K_axon(this);
 	pInputDendrite = new K_dendrite(this); //	Skal ligge i s_auron og k_auron

	// Dersom start-kappa er over fyringsterskel (dette er bare for initiell debugging)
	// Alt taes hand om i changeKappa_derivedArg()
	// (legger til kappa, og legger element inn i pCalculatationTaskQue, slik at .doCalculation() blir kjørt etter iter).
	changeKappa_derivedArg( dStartKappa_arg );
	//doCalculation();




	// START LOGGFIL * * * * * *
	// Utskrift til logg. LOGG-initiering (lag ei .oct fil som er kjørbar)
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_auron" <<sNavn <<"-kappa" <<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	kappa_logFile.open( tempStr.c_str() );
	kappa_logFile<<"data=[" <<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<";\n";
	kappa_logFile.flush();
	// SLULL LOGGFIL * * * * * * 




 	cout 	<<"\nConstructor: K_auron.\n"
			<<"\tKappa: \t\t\t\t" <<dAktivitetsVariabel <<endl
			<<"\tTau:   \t\t\t\t" <<FYRINGSTERSKEL <<endl
			//<<"\tEstimated time to A.P.: \t" <<(1 / ALPHA) * log( (dDepolAtStartOfTimeWindow - dAktivitetsVariabel) / (float)(FYRINGSTERSKEL - dAktivitetsVariabel) ) 
			<<"\n\n";



	// Kjører auronets doCalculation() for å få rett verdier:
	doCalculation();

} //}
K_auron::~K_auron()
{ //{
	

	cout<<"Destructor:\tK_auron::~K_auron()\n";

	// Tar seg bort fra pAllKappaAurons og pAllAurons: LISTEvariant:
	//pAllKappaAurons.remove(this);

	// pAllAurons - element fjærnes i i_auron. Fjærner pAllKappaAurons-element i K_auron::~K_auron() :
	pAllKappaAurons.remove(this);
	



	//i_auron tar seg ikkje av dette.. Sletter axon og dendrite:
	delete pOutputAxon;
	delete pInputDendrite;


	// Kanskje det er bedre å la ~i_auron kjøre? Korleis kan dette garanteres? Gjøres det uansett, kanskje?
	// pAllAurons.re move(this);

	// Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
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
} //}
//}2
//{2 *** K_sensor_auron
K_sensor_auron::K_sensor_auron( std::string sNavn_Arg , double (*pFunk_arg)(void) ) : K_auron(sNavn_Arg)
{
	// Assign the sensor function:
	pSensorFunction = pFunk_arg;
	// Add to pAllSensorAurons list:
	pAllSensorAurons.push_back(this);
	pAllAurons.push_back( this ); 		// evt. i_auron::pAllAurons her og bare pAllAurons på neste linje.

// TODO XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
// ER ikkje heilt sikker på om neste linja skal være her..
	// Legger til auron* i std::list<timeInterface*> pPeriodicElements:
	// Trur kanskje den allerede ligger der, pga. at det arver fra K_auron. Dette er vel isåfall feil?
	//time_class::addElementIn_pPeriodicElements( this );


	dAktivitetsVariabel = (*pSensorFunction)();

	// KANSKEJ EG SKAL TA VEKK DENNE? Ikkje naudsynt å rekalkulere sensor-verdi!
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

	// TODO XXX FRA K_sensor_auron :
 	// ER ikkje heilt sikker på om neste linja skal være her.. ( Legger til auron* i std::list<timeInterface*> pPeriodicElements: )
	// Trur kanskje den allerede ligger der, pga. at det arver fra K_auron. Dette er vel isåfall feil?
	//time_class::addElementIn_pPeriodicElements( this );

	//kappaRecalculator.pKappaAuron_obj = this;

	//recalculateKappa();

}
//}2
//}1 * AURON

//{1 * SYNAPSE
//{2 i_synapse
//i_synapse::i_synapse(i_axon* pPresynAxon_arg, i_dendrite* pPostsynDendrite_arg, unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ ) : timeInterface(sKlasseNavn), bInhibitoryEffect(bInhibEffekt_Arg)
i_synapse::i_synapse(double dSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ ) : timeInterface(sKlasseNavn), bInhibitoryEffect(bInhibEffekt_Arg)
{ //{3
	dSynapticWeight = dSynVekt_Arg;
	dSynapticWeightChange = 0;
/*
	pPreNodeAxon = pPresynAxon_arg;
	pPostNodeDendrite = pPostsynDendrite_arg;
*/
	
	cout<<"\tConstructor :\ti_synapse::i_synapse(unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, string navn);\n";
	
} //}3
//}2
//{2 s_synapse

#if 0
// s_synapse mellom K_auron og s_auron:
s_synapse::s_synapse(K_auron* pPresynAuron_arg, s_auron* pPostsynAuron_arg, double dSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			:  i_synapse(dSynVekt_Arg, bInhibEffekt_Arg, "s_synapse__from_k_auron"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite) 
{
	cerr<<"Presyn. axon: " <<pPreNodeAxon->sClassName ;
	pPreNodeAxon->SLETTtypeId(); 	/// XXX KVA ER DETTE?
	
	cerr<<"Kaller s_synapse::s_synapse(" <<pPreNodeAxon->pElementOfAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementOfAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);

	

	cout 	<<"\tCONSTRUCTOR :\ts_synapse::s_synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;



	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	//{4 Utskrift til logg. LOGG-initiering
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
#endif
// s_synapse mellom s_auron og s_auron:
s_synapse::s_synapse(s_auron* pPresynAuron_arg, s_auron* pPostsynAuron_arg, double dSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			:  i_synapse(dSynVekt_Arg, bInhibEffekt_Arg, "s_synapse"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite) 
{//{3	



	cerr<<"Presyn. axon: " <<pPreNodeAxon->sClassName ;
	pPreNodeAxon->SLETTtypeId();
	
	cerr<<"Kaller s_synapse::s_synapse(" <<pPreNodeAxon->pElementOfAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementOfAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);

	

	cout 	<<"\tCONSTRUCTOR :\ts_synapse::s_synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;



	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	//{4 Utskrift til logg. LOGG-initiering
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

} //}3
s_synapse::~s_synapse()
{ //{3 ... }    --DESTRUCTOR
	bool bPreOk  = false;
	bool bPostOk = false;


	cout<<"\tDESTRUCTOR :\ts_synapse::~s_synapse() : \t";


	//TODO Sjekk ut std::find() istedenfor. Stroustrup anbefaler å bruke dette!.
	if( !bPreOk ){ 	//Redundant test. Kanskje eg skal skrive while(!bPreOk)?
		//fjærner seg sjølv fra prenode:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<s_synapse*>::iterator iter = (pPreNodeAxon->pUtSynapser).begin(); iter != (pPreNodeAxon->pUtSynapser).end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~( [" <<pPreNodeAxon->pElementOfAuron->sNavn <<"] -> "; 	// utskrift del 1
				(pPreNodeAxon->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<s_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
			if( *iter == this ){ 
				cout<<"[" <<pPostNodeDendrite->pElementOfAuron->sNavn <<"] )\t"; 										// utskrift del 2
				(pPostNodeDendrite->pInnSynapser).erase( iter );
				bPostOk = true;
				break;
			}
		}
	}

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

	// no er data slik: [tid, synWeight ] i s_synapse-logg
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

} //}3
//}2
//{2 K_synapse
K_synapse::K_synapse(K_auron* pPresynAuron_arg, K_auron* pPostsynAuron_arg, double dSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/ )
 :  i_synapse(dSynVekt_Arg * FYRINGSTERSKEL      , bInhibEffekt_Arg, "K_synapse") , pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite) 
{ 	//XXX HER: nytt kappa element:
	cout<<"Constructor :\tK_synapse::K_synapse(" <<pPreNodeAxon->pElementOfAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementOfAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	


	// lag ei .oct - fil, og gjør klar for å kjøres i octave:  UTKOMMENTERT (ikkje naudsynt når vi ikkje har syn.plast.)
	// Utskrift til logg. LOGG-initiering 
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
{ // XXX TODO UTESTA! 			HAR IKKEJ SETT GJENNOM, engang. TODAY: 12.04.2011 		TODO TODO
	bool bPreOk  = false;
	bool bPostOk = false;


cout<<"\t\tDESTRUCTOR :\tK_synapse::~K_synapse() : \t";


	//TODO Sjekk ut std::find() istedenfor. Stroustrup anbefaler å bruke dette!.
	if( !bPreOk ){ 	//Redundant test. Kanskje eg skal skrive while(!bPreOk)?
		//fjærner seg sjølv fra prenode:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<K_synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~( [" <<pPreNodeAxon->pElementOfAuron->sNavn <<"] -> "; 	// utskrift del 1
				(pPreNodeAxon->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<K_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){ 		//Synapse_overgang
			if( *iter == this ){ 
				cout<<"[" <<pPostNodeDendrite->pElementOfAuron->sNavn <<"] )\t"; 										// utskrift del 2
				(pPostNodeDendrite->pInnSynapser).erase( iter );
				bPostOk = true;
				break;
			}
		}
	}

	if( (!bPreOk) || (!bPostOk) ){
		/// FEIL:
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i s_synapse destruksjon. (~s_synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNodeAxon->pElementOfAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementOfAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementOfAuron->sNavn <<" - " <<pPostNodeDendrite->pElementOfAuron->sNavn <<endl;
		exit(-9);	
	}
	//cout<<"\tO.K.  -  syn. " <<pPreNodeAxon->pElementOfAuron->sNavn <<" til " <<pPostNodeDendrite->pElementOfAuron->sNavn <<" er ikkje lenger\n";
	cout<<endl;


	// Rett slutt på utskriftsfil-logg:
	// no er data slik: [tid, synWeight ] i s_synapse-logg
	synTransmission_logFile<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \".r;Synaptic transmission;\");\n"
					<<"title \"Synaptic transmission from s_synapse: " <<pPreNodeAxon->pElementOfAuron->sNavn <<" to " <<pPostNodeDendrite->pElementOfAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel Syn.Transmission\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_transmission_" <<pPreNodeAxon->pElementOfAuron->sNavn <<"->" <<pPostNodeDendrite->pElementOfAuron->sNavn <<".eps\', \'-deps\', \'-color\');\"\n"
					<<"print ./eps/eps_transmission_" <<pPreNodeAxon->pElementOfAuron->sNavn <<"->" <<pPostNodeDendrite->pElementOfAuron->sNavn <<".eps -deps -color\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); ";
	synTransmission_logFile.close();
	//}

	 
}
//}2


//}1 * SYNAPSE

//{1 * AXON
//{2 ***  i_axon
i_axon::i_axon( std::string sKlasseNavn ="dendrite") : timeInterface(sKlasseNavn)
{ //{3
} //}3
i_axon::~i_axon()
{ //{
	// pUtSynapser inneholder bare peikere, så pUtSynapser.clear() vil ikkje føre til destruksjon av synapsene.
	// 		Sletter synapsene eksplisitt med delete-operatoren på alle element som list<synapse*> pUtSynapser) peiker på.
	while( !pUtSynapser.empty() ){
		delete (*pUtSynapser.begin()); //Kaller destruktoren til første gjenværende synapse. Dette fører også til at synapsa fjærnes fra pUtSynapser (og dendrite.pInnSynapser)
	}
	//TO DO   TO DO    TO DO Denne skal ligge i i_axon når pUtSynapser er endra fra <s_synapse*> til <i_synapse*>. TODO TODO  TODO

} //}
//}2 *** i_axon
//{2 ***  s_axon
s_axon::s_axon(s_auron* pAuronArg) : i_axon("s_axon"), pElementOfAuron(pAuronArg)
{ //{3 
	cout<<"\tConstructor :\ts_axon::s_axon(s_auron*)\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
} //}3
s_axon::~s_axon()
{ //{3
	cout<<"\tDESTRUCTOR :\ts_axon::~s_axon() for auron \t" <<pElementOfAuron->sNavn <<"\n";

} //}3
//}2
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
{ //{3 
	//pElementOfAuron = pElementOfAuron_arg;
} //}3
i_dendrite::~i_dendrite()
{ //{3
	// Destruerer alle innsynapser.
	while( !pInnSynapser.empty() ){
	 	delete (*pInnSynapser.begin() );
	}
} //}3
//}2
//{2 *** s_dendrite
s_dendrite::s_dendrite( s_auron* pPostSynAuron_Arg ) : i_dendrite("s_dendrite"), pElementOfAuron(pPostSynAuron_Arg)
{ //{3
	cout<<"\tConstructor :\ts_dendrite::s_dendrite( s_auron* )\n";// for \tauron " <<pElementOfAuron->sNavn <<endl;

	bBlockInput_refractionTime = false;
} //}3
s_dendrite::~s_dendrite()
{ //{3
	cout<<"\tDesructor :\ts_dendrite::~s_dendrite() \t for auron \t" <<pElementOfAuron->sNavn <<"\n";
} //}3
//}2 XXX s_dendrite 
//{2 *** K_dendrite
K_dendrite::K_dendrite( K_auron* pPostSynAuron_Arg ) : i_dendrite("K_dendrite" ), pElementOfAuron(pPostSynAuron_Arg)
{
	cout<<"\tConstructor :\tK_dendrite::K_dendrite( K_auron* ) \t\t[TOM]\n";
}
K_dendrite::~K_dendrite()
{
	cout<<"\tDesructor :\tK_dendrite::~K_dendrite() \t\t for auron \t" <<pElementOfAuron->sNavn <<"\t[TOM]\n";
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
	// TODO Endre til å være at arg er verdien, og fikser derivering her.

	//pElementOfAuron->dAktivitetsVariabel += dNewSignal_arg; SKJER I changeKappa_derivedArg(a)
	// TODO FINN UT kva kappa skal endres med. ALPHA*dNewSignal_arg eller bare dNewSignal_arg
	pElementOfAuron->changeKappa_derivedArg( dNewSignal_arg );

} //}2

inline void K_auron::changeKappa_derivedArg( double dInputDerived_arg)//int derivedInput_arg )
{
	/************************
	* Legg arg til i Kappa  *
	************************/

	// Legges til Kappa no, og effektene av endringa kalkuleres i .doCalculation().
	dChangeInKappa_this_iter +=  dInputDerived_arg ;

	// Legger den i pCalculatationTaskQue, slik at effekt av all endring i kappa ila. tidsiterasjonen beregnes etter iterasjonen.
	time_class::addCalculationIn_pCalculatationTaskQue( this );
	// Eller om delay blir stort, kanskje:
	// doCalculation();

	//bEndraKappaDennePerioden = true;

	writeKappaToLog();

	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout<<sNavn <<"\t:\tTid:\t" <<time_class::getTid() <<" ,\tKappa :\t" <<dAktivitetsVariabel <<endl;
	#endif

}


// XXX XXX XXX FARLIG ! Lager føkk når den endrer kappa, og doCalculation(). Kalkulerer v_0 fra gammel tid, men ny kappa. XXX IKKJE BRUK!
inline void K_auron::changeKappa_absArg(double dNewKappa)
{
	cout<<"IKKJE Bruk changeKappa_absArg() ! Den innfører potensil for feil som ødelegger alt. Ikkje ferdig.\nAvlutter."; exit(0);

	// Kanskje ikkje naudsynt, siden vi ikkje legger til element i pCalculatationTaskQue..
	//dChangeInKappa_this_iter = 0; // Hindrer .doCalculation() å endre kappa ytterligare.
	dAktivitetsVariabel = dNewKappa;

	// Foreløpig definerer eg eit sensorauron som en rein sensor (Tar ikkje imot input fra andre auron):
	// I dette tilfellet fjærner eg en potensiell feilkilde:
	dChangeInKappa_this_iter = 0;

	// Legger den i pCalculatationTaskQue, slik at effekt av all endring i kappa ila. tidsiterasjonen beregnes etter iterasjonen.
	time_class::addCalculationIn_pCalculatationTaskQue( this );
	
	writeKappaToLog();
}
//}1

//{1  * SANN

inline void s_dendrite::newInputSignal( double dNewSignal_arg )
{ //{2 .. }

	// Dersom input er blokkert er enten noden i 'refraction period' eller så fyrer det allerede for denne iterasjonen. (trenger ikkje kalkulere meir på depol..)
 	if( bBlockInput_refractionTime ) return;
	#if DEBUG_UTSKRIFTS_NIVAA > 5
	else{ cout<<"forrige fyring på tid: " <<pElementOfAuron->ulTimestampForrigeFyring <<endl; }
	#endif

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
		// Blokkerer videre input (etter at vi allerede har nådd terskel)
		bBlockInput_refractionTime = true;

		//Ekstra time-delay:
		//time_class::addTaskIn_pWorkTaskQue( this );
		time_class::addTaskIn_pWorkTaskQue( pElementOfAuron );
		// Blir kalla fra this->doTask() addTaskIn_pWorkTaskQue( pElementOfAuron );
		
		// for ekstra time delay. Evt kan vi kjøre auron som neste ledd:
		// Legger til neste ledd i signal-path (soma == auron).
	 	//time_class::addTaskIn_pWorkTaskQue( pElementOfAuron );

	}

	// Skriver til log for aktivitetsVar:
	pElementOfAuron->depol_logFile 	<<time_class::getTid() <<"\t" <<pElementOfAuron->dAktivitetsVariabel <<"; \t #Depolarization\n" ;
	pElementOfAuron->depol_logFile.flush();
} //}2

inline void s_dendrite::calculateLeakage()
{ //{2 /** Bare for SANN:  ***/
	/*
	* 	Har testa verdiane i octave. Loggen lå på kurva for [initDepol]*LEKKASJEFAKTOR_FOR_DEPOL^x, selv om eg bare sjekka calculateLeakage() for siste ledd 
	* 		(f.eks. etter 30 iterasjoner uten noko input anna enn init-verdien for depool. for auronet). Hurra!
	*/
	
	//static unsigned long ulTimestampLastUpdate_static_local = 0;
	if( pElementOfAuron->ulTimestampLastInput != time_class::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
		// SKRIVER TIL dAktivitetsVariabel.
	 	pElementOfAuron->dAktivitetsVariabel *= pow( (double)(1-ALPHA), (double)(time_class::getTid() - pElementOfAuron->ulTimestampLastInput) );
		
		// Denne har ikkje mykje med input å gjøre. Men kanskje den variabelen..
		//pElementOfAuron->ulTimestampLastInput = time_class::getTid();
		
		#if DEBUG_UTSKRIFTS_NIVAA > 10 //{
		HUSK DENNE, om man skal ha med utskrift: int slettDebugGammelDepolverdi = pElementOfAuron->dAktivitetsVariabel;

		cout 	<<"\n\n\t\t\t\t\tLEKKASJEfaktor: " <<(double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist ) <<" [gammel => ny depol.]: [" <<slettDebugGammelDepolverdi <<"=>" <<pElementOfAuron->dAktivitetsVariabel
				<<"]. (for auron " <<pElementOfAuron->sNavn <<")\n";
		#endif //}
		
		// Gjøres antagligvis i s_dendrite::newInputSignal() --gidder ikkje sjekke no. Funker.
		//ulTimestampForrigeOppdatering = time_class::getTid(); 
	}
	// TODO LEGG INN FEILMELDING DERSOM : else

	// Dersom den allerede har regna ut lekkasje: ikkje gjør det igjen. Returner.
} //}2

//}1



/*************************************************************
****** 													******
******  		doTask() -- samla på en plass. 			******
****** 													******
*************************************************************/

/*
 //{1   *  i_axon::doTask() KOMMENTERT UT
inline void i_axon::doTask()
{
 	cout<<"i_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementOfAuron->sNavn <<" - - - - - - - - - - - - - - - \n";

	// For meir nøyaktig simulering av tid kan alle synaper få verdi for 'time lag' før fyring. No fokuserer eg heller på effektivitet. 
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
 	for( std::list<i_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i arbeidskø: (FIFO-kø)
		
		//time_class::pWorkTaskQue.push_back( (*iter) );
		// Legger til ut-synapser i time_class::arbeidskø
		time_class::addTaskIn_pWorkTaskQue( *iter );
	}

} //}1
*/
//{1 		* 	SANN
inline void s_auron::doTask()
{ //{ ** AURON

	//Avblokkerer bBlockInput_refractionTime: Kanskje dette skal være her? (1ms refraction time..)
	//pInputDendrite ->bBlockInput_refractionTime = false;




	#if DEBUG_UTSKRIFTS_NIVAA > 0
	cout<<"\tS S " <<sNavn <<" | S | " <<sNavn <<" | S | S | S | | " <<sNavn <<" | S | | " <<sNavn <<" | S | | " <<sNavn <<"| S | S | S | S |\t"
		<<sNavn <<".doTask()\tFYRER neuron " <<sNavn <<".\t\t| S S | \t  | S |  \t  | S |\t\tS | " <<time_class::getTid() <<" |\n"
		<<endl;
	#endif

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output. // bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	time_class::addTaskIn_pWorkTaskQue( pOutputAxon );


	if( ulTimestampForrigeFyring == time_class::getTid() )
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 1
		cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
		#endif
		return;
	}

	// Registrerer fyringstid (for feisjekk (over) osv.) 
	ulTimestampForrigeFyring = time_class::getTid();


	//Resetter depol.verdi 
	dAktivitetsVariabel = 0; 

	writeAPtoLog();

} //}
inline void s_axon::doTask()
{ //{ ** AXON

DEBUG("s_axon::doTask() START");
	// Avblokkerer dendritt. Opner den for meir input. Foreløpig er dette måten 'refraction time' funker på.. (etter 2 ms-dendrite og auron overføring..)
	//static_cast<s_dendrite*>(pElementOfAuron->pInputDendrite)->bBlockInput_refractionTime = false;
	pElementOfAuron->pInputDendrite ->bBlockInput_refractionTime = false;

	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"s_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementOfAuron->sNavn <<" - - - - - - - - - - - - - - - \n";
	#endif

	// Legger til alle utsynapser i pWorkTaskQue:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
 	for( std::list<s_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i time_class::pWorkTaskQue: (FIFO-kø)
		time_class::addTaskIn_pWorkTaskQue( *iter );
	}

	 //Skriver til logg etter refraction-period.
	 pElementOfAuron->writeDepolToLog();
DEBUG("s_axon::doTask() SLUTT");

} //}
inline void s_synapse::doTask()
{ //{2 ** SYNAPSE

	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout<<"Overføring i s_synapse.\n";
	#endif

	// Dersom synapsen har inhibitorisk effekt: send inhibitorisk signal (subtraksjon). Ellers: eksiter postsyn. auron.
	// Dendrite lagrer tidspunk for overføring.
	// Istedenfor sender inn pos. eller neg. signal avhengig av bInhibitoryEffect: [ 1-2*bInhibitoryEffect ]  Gir enten +1 dersom bInhibitoryEffect er false (=0) eller -1 om bInhibitoryEffect er true (=1).

	// TODO TODO TODO TODO Før dette legges inn i rapporten: Endre neste if-setning til
	// (1- 2*bInhibitoryEffect) dSynapticWeight 
 	pPostNodeDendrite->newInputSignal( (1-2*bInhibitoryEffect) * 	( 1000 * dSynapticWeight )   );   //1/a = 250. Ganger dermed med 250 000.
	/*
	if( bInhibitoryEffect ){
 		pPostNodeDendrite->newInputSignal(  1000/ALPHA * dSynapticWeight );   //1/a = 250. Ganger dermed med 250 000.
	}else{
		pPostNodeDendrite->newInputSignal(  1000/ALPHA * dSynapticWeight );
	}*/
//( dendrite legges til i arbeidskø dersom den kommer over fyringsterskel.) --Dette er feilaktig, men meir effektivt.
		


	// XXX Kalkulere syn.p.?



	// Loggfører syn.weight
	synTransmission_logFile 	<<"\t" <<time_class::getTid() <<"\t" <<(1-2*bInhibitoryEffect) * dSynapticWeight
						<<" ;   \t#Synpaptic weight\n" ;
	synTransmission_logFile.flush();

	// Logg for aktivitetsVar for postsyn auron skjer i pPostNodeDendrite->newInputSignal(-);

} //}2
inline void s_dendrite::doTask()
{ //{ DENDRITE
	// Kva skal dendrite::doTask() gjøre? 
	//cout<<pElementOfAuron->sNavn <<"->[dendrite]::doTask(). Postsyn. depol (" <<pElementOfAuron->sNavn <<") etter overføring: " <<pElementOfAuron->nAktivitetsVariabel <<".\n";
	time_class::addTaskIn_pWorkTaskQue( pElementOfAuron );
} //}
//}1            *       slutt SANN

//{1 		* 	KANN


inline void K_auron::doTask()
{ //{ ** AURON

	//{ Kommentert ut
	// Feilsjekk! Sjekker om kappa har vore over Tau, siste 'time window'
	#if 0
	if( !bKappaLargerThanThreshold_lastIter )
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 1
		cout<<"ERROR \t ERROR \t ERROR \t ERROR \t Fyring mens K<T forrige time window: bKappaLargerThanThreshold_lastIter: false" <<endl;
		#endif
		return;
	}
	#endif
	//}

	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout 	<<"K_auron::doTask():\n"
			<<"\tdLastCalculatedPeriod :\t" <<dLastCalculatedPeriod <<endl
			<<"\tdPeriodINVERSE :\t" <<dPeriodINVERSE <<endl;
	#endif


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

	// Setter v_0 til 0 og t_0 til [no]:
	dDepolAtStartOfTimeWindow = 0;
	ulStartOfTimewindow = time_class::getTid();

		// Har testa litt, og det er definitivt best å kjøre neste tre linjene, iforhold til å kjøre doCalculation()!
		//(doCalculation() varianten slutta aldri på 40K tidssteg, neste-tre-linje varianten slutta etter 30 sekund på 1000K tidsiterasjoner..
		//
		// Har testa det for nye varianten av tidsplanlegging (sjekke alle aurons ulEstimatedTaskTime_for_object)
		// Nå er det det samme (1 000 000 iter, to ukobla auron(eit vanlig og eit sensor) : 28,155 sek  eller 29.286  for doCalculation() VS 		29.056 eller  28.828 	for neste 4 linjene

	//Har funnet ut at neste fire linjene er mest effektivt, men bare litt. For eit auron, 300000 tidsiter gjorde det 29/15/12 istedenfor doCalculation() som gjorde 31/17/13
	#if 0
	//{
		// Beregn ny isi-periode^{-1}. Brukes til å beregne syn.overføring seinare i signal-cascade.
		dL astCalculatedPeriod = (- log((dAktivitetsVariabel - FYRINGSTERSKEL) / dAktivitetsVariabel) / ALPHA);
		
		double dPeriodInverse_temp =  (1/d LastCalculatedPeriod);
 		dChangeInPeriodINVERSE = dPeriodInverse_temp - dPeriodINVERSE ;
		dPeriodINVERSE = dPeriodInverse_temp;
	//}
	#else
		//Kjør heller det over enn .doCalculations() ... Der er jo en del meir jobb.. (Krever sattans mykje meir arbeid!)
		doCalculation();
	#endif


	// Oppdaterer ulEstimatedTaskTime_for_object til [no] + dLastCalculatedPeriod:
	ulEstimatedTaskTime_for_object = time_class::getTid() + (unsigned long)dLastCalculatedPeriod;

	// Logger AP (vertikal strek)
	writeAPtoLog();
	
	
} //}
inline void K_axon::doTask()
{ //{ ** AXON
	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"K_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementOfAuron->sNavn <<" - - - - - - - - - - - - - - - \n";
	#endif

	// Refraction time: Setter depol til 0 igjen (dette er 
	// FUNKER IKKJE: (blir bare problemer med jævvla høge verdier (type folde-shit (eller ka-det-ne-neiter) )
	//Ok, no funker det men har ingen effekt. (fjærna +1 på ulStartOfTimewindow=tid +1;
	//pElementOfAuron->dDepolAtStartOfTimeWindow = 0;
	//pElementOfAuron->ulStartOfTimewindow = time_class::getTid() ;

	// Overføring:
	doTransmission();

	// Gjøres i s_auron. Skal eg også gjøre det her (for å poengtere refraction time)?
	//pElementOfAuron->writeDepolToLog();

} //}

// Delte opp for å kunne skille mellom å propagere kappa, og å fyre AP, for å kunne ha refraction period.
inline void K_axon::doTransmission()
{
	// Legg til alle utsynapser i pWorkTaskQue:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
 	for( std::list<K_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i time_class::pWorkTaskQue: (FIFO-kø)
		time_class::addTaskIn_pWorkTaskQue( *iter );
	}
}

inline void K_synapse::doTask()
{ //{ ** SYNAPSE
	// K_ij = w_ij / p(K_j) Differansen Delta K_ij blir sendt som argument til pPostNodeDendrite->newInputSignal( argument );
	// 	denne er gitt som Delta K_ij = dt * w_ij / Delta p(K_j)

	// Istedenfor sender inn pos. eller neg. signal avhengig av bInhibitoryEffect: [ 1-2*bInhibitoryEffect ]  Gir enten +1 dersom bInhibitoryEffect er false (=0) eller -1 om bInhibitoryEffect er true (=1).
	pPostNodeDendrite->newInputSignal( (1-2*bInhibitoryEffect) * dSynapticWeight * (pPreNodeAxon->pElementOfAuron)->dChangeInPeriodINVERSE );

	
	// Funker nesten som en makro. Ta vekk:
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
} //}

inline void K_dendrite::doTask()
{ //{ XX DENDRITE (ikkje i bruk)
	// TODO Trur ikkje denne skal være med lenger. Legger til brutal feilsjekk: exit(-1);
	cout<<"\n\nBrutal feilsjekk @ K_dendrite::doTask() : exit(-1);\n\n";

	exit(-1);
} //}

//}1            *       KANN slutt

//{1 		* 	time_class

void time_class::doTask()
{ 	//{ 

	// gjennomfører planlagte kalkulasjoner:
	doCalculation();


	//itererer time:
	ulTime++;

	// utskrift:
	#if DOT_ENTER_UTSKRIFT_AV_TID
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
	* Oppdater alle K_sensor_auron *
	*******************************/
	#if SANN
	s_sensor_auron::updateAllSensorAurons();
	#endif

	
	/*************************************************
	* Flytter planlagde oppgaver over i pWorkTaskQue *
	*************************************************/
	for( std::list<timeInterface*>::iterator pPE_iter = pPeriodicElements.begin() ; pPE_iter != pPeriodicElements.end() ; pPE_iter++ )
	{
		if( (*pPE_iter)->ulEstimatedTaskTime_for_object == ulTime+1 )
		{
			addTaskIn_pWorkTaskQue( (*pPE_iter) );
			// Dette fører til eit kall til eit tidsElements doTask(). Teller antall kall (til rapporten):
			comparisonClass::ulNumberOfCallsTo_doTask ++;
			#if DEBUG_UTSKRIFTS_NIVAA > 2
			cout<<"Telte kall til " <<(*pPE_iter)->sClassName <<".doTask()\n";
			#endif
		}
 	}


	/*XXXXXXXXXXXXXXX
	X 	 TESTING 	X
	XXXXXXXXXXXXXXXX*/
	// Kaller testfunk for å teste K_auron. 
	K_auron::loggeFunk_K_auron();

	// Bare debugging--utskrift:
	timeClassTestFunk_som_kjoeres_kvar_tidsIter();
	
	// Legger til egenpeiker på slutt av pNesteJobb_ArbeidsKoe
	pWorkTaskQue.push_back(this);	
}//}


//}1


/******************************************************************
****** 														*******
****** 			doCalculation() -- samla på en plass 		*******
****** 														*******
******************************************************************/

void K_auron::doCalculation()
{ //{
	comparisonClass::ulNumberOfCallsToKappa_doCalculations++;

	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"K_auron " <<sNavn <<".doCalculation()\t\t" <<sNavn <<".doCalculation()\t\tTid: " <<time_class::getTid() <<"\n";
	#endif

	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"[K, T] = " <<dAktivitetsVariabel <<", " <<FYRINGSTERSKEL <<endl;
	#endif



	/*********************************************************
	* Legg dChangeInKappa_this_iter til dAktivitetsVariabel  *
	*********************************************************/
	
	// doCalculation() blir kalla etter enten changeKappa_derivedArg() eller doTask(). Begge desse skriver dDepolAtStartOfTimeWindow (så trenger ikkje gjøre dette her).

	// Viktig å kalkulere depol med GAMMEL Kappa! Ellers får vi hopp i depol!
	// Veldig viktig å hugse å kalkulere dDepolAtStartOfTimeWindow på slutten av forrige time window! (og lagre tidspunkt)
	dDepolAtStartOfTimeWindow = getCalculateDepol();
	ulStartOfTimewindow = time_class::getTid();
	
	// Oppdaterer Kappa
	dAktivitetsVariabel += dChangeInKappa_this_iter;
	dChangeInKappa_this_iter = 0;

	//**********************************************
	//*  Beregn estimert fyringstid:   *
	//**********************************************

	/******************************************************************
	*  Kjører beregning av dPeriodINVERSE og dChangeInPeriodINVERSE:  *
	******************************************************************/
	if( dAktivitetsVariabel > FYRINGSTERSKEL){
		static double dPeriodInverse_static_local = 0;
	
		// Berenger dPeriodINVERSE og dChangeInPeriodINVERSE:
	// XXX Ikkje nødvendig, trur eg. Test bedre! (Kommentert ut neste linje, men ikkje testa det så godt) :
	// Blir vel gjordt i changeKappa_derivedArg()  			- testa med å kommentere ut og sjekke plott. Så bra ut. TODO test bedre
	// Joo, har ikkje funnet det nokon annan plass.. Hmm..
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
		ulEstimatedTaskTime_for_object =( ( time_class::getTid() + log( (dAktivitetsVariabel-dDepolAtStartOfTimeWindow)/(dAktivitetsVariabel-FYRINGSTERSKEL) )   /  ALPHA )+0.5)  		+2 ; //+1 for å få delay i dendrite. 
																													// TODO TODO TODO før på +2 istadenfor +1 for å også få delay i axon. XXX XXX TODO TODO TODO
		/*
		* 	Her er eit problem:
		* 		Refraction time: Dersom vi bare legger til en, så vil oppladning starte umiddelbart. Når kappa blir endra, så vil oppladninga (som starter for tidlig) være det einaste som er viktig. Dette skaper problemer.
		* 		XXX XXX XXX
		*/

	
	
	
		/* Propagerer resultatet:
		time_class::addTaskIn_pWorkTaskQue( pOutputAxon ); 
	 	 Er visst i neste section:
		 */

		// TODO TODO TODO TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO 
		// KJører synaptisk overføring kvar gang Kappa endres. Dette er ikkje optimalt. Men det vil nok bli rett. Mens eg utvikler Kappa-mekanismer..
		// Dersom eg vil ha fin plot for transmission_synapse, bør denne flyttes inn i if-setninga under (eller fjærnes). Da forsvinner 0-overføringene...
		
		// Bestiller doTask() for å propagere endring i aktivitetsnivå:
		//time_class::addTaskIn_pWorkTaskQue( pOutputAxon );	
		// Propagerer heller ved å kalle K_axon::doTransmission() direkte. Dette gjør at vi ikkje får time-delay, men whatdefuck. Har for lite tid no..
		pOutputAxon->doTransmission(); 	//			XXX SKAL være med: propager kappa kvar gang den endres! (eventuelt gjøres dette bare ved spiking av noden).
		//VAR: pOutputAxon->doTask();
		bAuronHarPropagertAtDenErInaktiv = false;
		//XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 


	}else{
		// setter planlagt task time til no, slik at den aldri vil fyre pga. ulEstimatedTaskTime_for_object. (når den sjekker nest gang, så vil [no] være i fortida..)
		ulEstimatedTaskTime_for_object = time_class::getTid();
		
		// Setter dLastCalculatedPeriod, dChangeInPeriodINVERSE, dPeriodINVERSE.
		dLastCalculatedPeriod = 0; 	// Er dette greit?
		dChangeInPeriodINVERSE = -dPeriodINVERSE; 
		dPeriodINVERSE = 0;
		
		#if DEBUG_UTSKRIFTS_NIVAA > 4
			cout<<"Kappa er mindre enn Tau. Setter ulEstimatedTaskTime_for_object = [no] (vil ikkje ha noko å sei for fyringa).\n";
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




	// Propagerer resultatet:
	// flytta inn i if-testen:  time_class::addTaskIn_pWorkTaskQue( pOutputAxon ); 


	// TODO Legger dersom element skal fyre neste iter: Legger den inn i pWorkTaskQue:
	// Den er med bare for sikkerhetsskuld ? (i tilfelle kappa endres, slik at node vil fyre neste iter (sjeldent) )
	if( ulEstimatedTaskTime_for_object == time_class::getTid()+1)
		time_class::addTaskIn_pWorkTaskQue( this );

	// Skriver til log for depol:
	writeDepolToLog();
} //}

	
	
	
// Kvar enkelt K_sensor_auron har en funksjonspeiker som peiker på dens sense-funksjon. Denne skal være av type [ double F(void ].
// 	K_sensor_auron har en static funksjon som beregner ny kappa for alle senseauron, kalla fra time_class:: doTask()

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
	dSensedValue = /* XXX ALPHA*  */ (*pSensorFunction)(); //TODO Finn ut om denne er gange ALPHA.

	// Er ikkje heilt sikker på kven eg vil bruke:  		De virker heilt ekvivalente. Akkurat no prøver eg med abs-arg for å fjærne en mulig feil. TODO Ta tilbake før rapport!
	if( dSensedValue != dLastSensedValue){
		//changeKappa_absArg( dSensedValue ); XXX FARLIG! IKKJE BRUK changeKappa_absArg() !
		changeKappa_derivedArg( dSensedValue-dLastSensedValue );
	}

	// Veit ikkje om den er naudsynt (legger til i calculation kø, inne i changeKappa()
	// Dette er litt teit, men ikkje veldig siden sensor ikkje endres fleire ganger per iter :   Å kalle neste direkte:
	//doCalculation();
	
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


// TODO TODO TODO TODO TODO  	Trur sendng til dendrite blir feil.		TODO TODO TODO TODO TODO 
inline void s_sensor_auron::updateSensorValue()
{ //{
	static double sdLastValue = 0;
	static double sdValue = 0;
	sdLastValue = sdValue;
	sdValue = (*pSensorFunction)();
	pInputDendrite->newInputSignal( (  sdValue )); // TODO TODO OPPMERKSOMHET!    Dette er feil. Sender inn umiddelbart sensa signal, ikkje den deriverte. 

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

	

#define FOELSOMHET_FOR_FEIL_RECALC_KAPPA 5
// TODO DENNE ER DÅRLIG: Dersom feil er veldig liten vil periode til neste rekalk være stor. For stor (1/x^2 går mot uendelig når x går mot null..)
// TODO Finn bedre funksjon. Kansje med en form for soft-maks og min-verdi? sigmoid function?

	static double sdSecondLastValue =0;
	static double sdLastValue 		=0;
	static double sdValue 			=0;
	static double sdTemp;

	sdTemp = sdValue;
	sdValue = ( (RECALC_c1+RECALC_c2) - (RECALC_c2 / ( 1+exp(-(RECALC_c4*dFeil - RECALC_c3)) )) 		+ 	sdLastValue + sdSecondLastValue )  / 3 ;
	sdSecondLastValue = sdLastValue;
	sdLastValue = sdTemp;
	
	#if DEBUG_UTSKRIFTS_NIVAA > 2
	cout<<pKappaAuron_obj->sNavn <<" - recalcKappaClass::doTask() :\tny periode for recalc kappa: \t" <<sdValue <<"\t\tfor feil: " <<dFeil <<"\t\tTid:\t" <<time_class::getTid() <<endl;
	#endif

	ulEstimatedTaskTime_for_object = time_class::getTid() + sdValue;
	// TODO TODO TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO  TODO 
	// Sjå kva feilen er, og la feilen bestemme kor lenge vi skal vente til neste rekalk. Kanskje også ha en slags FIR-effekt her?

} //}

inline double K_auron::recalculateKappa()
{ //{
	// Plan:
	//  - Rekalkulerer kappa for dendrite.
	// 		- skal hente ut K_ij fra alle innsynapsene. Dette kan den gjøre ved å kalle K_synapse::getWij();
	double dKappa_temp = pInputDendrite->recalculateKappa();
	double dKappaFeil_temp = dAktivitetsVariabel - dKappa_temp;

	// TESTER: Blir bare bullshit! XXX
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
	/*
	static double dOldActivityVariable;
	dOldActivityVariable = dAktivitetsVariabel;
	updateSensorValue();
	// Er dette rett :
	return dAktivitetsVariabel-dOldActivityVariable;
	*/ // Gjør det lett: (sensorauron har ikkje muligheten til å få input (enda))
	return 0;
}

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

void neuroElement_syn_testFunk(K_synapse* pK_syn_arg)
{
	pK_syn_arg->skrivUt();
}
void neuroElement_testFunk(K_auron* pK_arg)
{
	pK_arg->recalculateKappa();
}

// Ubrukt funk. Skal bare teste noke, no..
inline const double s_auron::getCalculateDepol()
{
	pInputDendrite->calculateLeakage();
	return dAktivitetsVariabel;
}


void timeClassTestFunk_som_kjoeres_kvar_tidsIter()
{

}

// vim:fdm=marker:fmr=//{,//}
