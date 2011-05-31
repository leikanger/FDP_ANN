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



std::ostream & operator<< (std::ostream & ut, i_auron* pAuronArg );
//std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );
std::ostream & operator<<(std::ostream& ut, i_auron* pAuronArg );





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
	
	//Finner seg selv i pAllKappaAurons
	#if 0
	for( std::list<i_auron*>::iterator iter = pAllAurons.begin() ; iter != pAllAurons.end() ; iter++ )
	{
		static int i=0; cout<<"Her: " <<i++ <<endl;
		if( (*iter) == this )
		{
			//cout<<"sletter " <<(*iter)->sNavn <<"\n";
			pAllAurons.erase(iter);
			break; // Farlig farlig. Før eg satt inn break fekk eg segfault. 	Yess! Eg har det! : Fordi da blir iterator ugyldig!
		}
	}
	#else
	//fjærner seg fra pAllAurons   Listevariant:
	pAllAurons.remove(this);
	#endif

	// Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
	depol_logFile 	// Lager en prikk som er på Kappa, for K_auron:  <<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"];\n"
					<<"];\n\n"
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
	ulTimestampForrigeInput  = time_class::getTid();
	// Osv.
	
	// Legger til auron* i std::list<i_auron*> pAllAurons;
	pAllAurons.push_back( this );

	//pOutputAxon og pInputDendrite
	pOutputAxon = new s_axon(this);
 	pInputDendrite = new s_dendrite(this);

} //}3 	
s_auron::~s_auron()
{ //{3
	//Vil bare kalles dersom eit s_auron slettes før avslutting. Ved destruering av alle i_auron::pAllAurons kalles bare ~i_auron.
	cout<<"\n\nDESTRUCTOR: s_auron::~s_auron() : \t\t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";
	

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
	//ulTimestampForrigeInput  = time_class::getTid();

	ulStartOfTimewindow = time_class::getTid();
	dDepolAtStartOfTimeWindow = uStartDepol_prosent * FYRINGSTERSKEL;


	// Sjå auron.h
	//bEndraKappaDennePerioden = false;

	// Sjå dokumentasjon for meir om bKappaLargerThanThreshold_lastIter
	//bKappaLargerThanThreshold_lastIter = false;
	
	// Legger til auron* i std::list<i_auron*> pAllAurons;
	pAllAurons.push_back( this ); 		// evt. i_auron::pAllAurons her og bare pAllAurons på neste linje.
	pAllKappaAurons.push_back( this );
	

	//pOutputAxon og pInputDendrite
	pOutputAxon = new K_axon(this);
 	pInputDendrite = new K_dendrite(this); //	Skal ligge i s_auron og k_auron

	// Dersom start-kappa er over fyringsterskel (dette er bare for initiell debugging)
	// Alt taes hand om i changeKappa()
	changeKappa( dStartKappa_arg );




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
{
	

	cout<<"Destructor:\tK_auron::~K_auron()\n";

	// Tar seg bort fra pAllKappaAurons og pAllAurons: LISTEvariant:
	//pAllKappaAurons.remove(this);

	//Finner seg selv i pAllKappaAurons (vector-variant):
	#if 0
	for( std::vector<K_auron*>::iterator iter = pAllKappaAurons.begin() ; iter != pAllKappaAurons.end() ; iter++ )
	{
		if( (*iter) == this )
		{
			//cout<<"sletter " <<(*iter)->sNavn <<"\n";
			pAllKappaAurons.erase(iter);
			break; // Farlig farlig! Før eg satt inn break fekk eg segfault. Yess! Fordi da blir iterator ugyldig!
		}
	}
	#else
		// pAllAurons - element fjærnes i i_auron. Fjærner pAllKappaAurons-element i K_auron::~K_auron() :
		pAllKappaAurons.remove(this);
	#endif

	// Kanskje det er bedre å la ~i_auron kjøre? Korleis kan dette garanteres? Gjøres det uansett, kanskje?
	// pAllAurons.re move(this);

	// Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
	kappa_logFile<<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \"@;Kappa;\");\n"

					<<"title \"Activity variable for K_auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					<<"print(\'./eps/eps_auron" <<sNavn <<"-kappa.eps\', \'-deps\');\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); "
					;
	kappa_logFile.close();
}
//}2
//{2 *** K_sensor_auron
K_sensor_auron::K_sensor_auron( double (*pFunk_arg)(void) , std::string sNavn_Arg /*="K_sensor_auron" */) : K_auron(sNavn_Arg)
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
//i_synapse::i_synapse(i_axon* pPresynAxon_arg, i_dendrite* pPostsynDendrite_arg, unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ ) : timeInterface(sKlasseNavn), bInhibitorisk_effekt(bInhibEffekt_Arg)
i_synapse::i_synapse(double dSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ ) : timeInterface(sKlasseNavn), bInhibitorisk_effekt(bInhibEffekt_Arg)
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
s_synapse::s_synapse(s_auron* pPresynAuron_arg, s_auron* pPostsynAuron_arg, double dSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			:  i_synapse(dSynVekt_Arg, bInhibEffekt_Arg, "s_synapse"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite) 
{//{3	



	cerr<<"Presyn. axon: " <<pPreNodeAxon->sClassName ;
	pPreNodeAxon->SLETTtypeId();
	
	cerr<<"Kaller s_synapse::s_synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);

	

	cout 	<<"\tCONSTRUCTOR :\ts_synapse::s_synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;



	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	//{4 Utskrift til logg. LOGG-initiering
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_s_synapse_" <<pPresynAuron_arg->sNavn <<"-"  <<pPostsynAuron_arg->sNavn ;
	if(bInhibitorisk_effekt){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	synWeight_loggFil.open( tempStr.c_str() );
	synWeight_loggFil<<"data=[\n";
	synWeight_loggFil.flush();

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
				cout<<"\t~( [" <<pPreNodeAxon->pElementAvAuron->sNavn <<"] -> "; 	// utskrift del 1
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
				cout<<"[" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"] )\t"; 										// utskrift del 2
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
			<<"bPreOk (" <<pPreNodeAxon->pElementAvAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementAvAuron->sNavn <<" - " <<pPostNodeDendrite->pElementAvAuron->sNavn <<endl;
		exit(-9);	
	}
	cout<<endl;


	//{4 Rett slutt på utskriftsfil-logg:

	// no er data slik: [tid, synWeight ] i s_synapse-logg
	synWeight_loggFil<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Synaptic weight;\");\n"

					<<"title \"Synaptic weight for s_synapse: " <<pPreNodeAxon->pElementAvAuron->sNavn <<" -> " <<pPostNodeDendrite->pElementAvAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel syn.w.\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); "
					<<"print(\'eps__s_synapse_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');" 	;
	synWeight_loggFil.close();
	//}4

} //}3
//}2
//{2 K_synapse
K_synapse::K_synapse(K_auron* pPresynAuron_arg, K_auron* pPostsynAuron_arg, double dSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/ )
 :  i_synapse(dSynVekt_Arg, bInhibEffekt_Arg, "s_synapse") , pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite), dPresynPeriodINVERSE(0)
{ 	//XXX HER: nytt kappa element:
	cout<<"Constructor :\tK_synapse::K_synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	


	// lag ei .oct - fil, og gjør klar for å kjøres i octave: 
	//{ Utskrift til logg. LOGG-initiering UTKOMMENTERT
	#if 0
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_K_synapse_" <<pPresynAuron_arg->sNavn <<"-"  <<pPostsynAuron_arg->sNavn ;
	if(bInhibitorisk_effekt){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	synWeight_loggFil.open( tempStr.c_str() );
	synWeight_loggFil<<"data=[\n";
	synWeight_loggFil.flush();
	#endif
	//}
}
K_synapse::~K_synapse()
{ // XXX TODO UTESTA! 			HAR IKKEJ SETT GJENNOM, engang. TODAY: 12.04.2011 		TODO TODO
	bool bPreOk  = false;
	bool bPostOk = false;


cout<<"\t\tDESTRUCTOR :\tK_synapse::~<K_synapse() : \t";


	//TODO Sjekk ut std::find() istedenfor. Stroustrup anbefaler å bruke dette!.
	if( !bPreOk ){ 	//Redundant test. Kanskje eg skal skrive while(!bPreOk)?
		//fjærner seg sjølv fra prenode:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<K_synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~( [" <<pPreNodeAxon->pElementAvAuron->sNavn <<"] -> "; 	// utskrift del 1
				(pPreNodeAxon->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<K_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
			if( *iter == this ){ 
				cout<<"[" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"] )\t"; 										// utskrift del 2
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
			<<"bPreOk (" <<pPreNodeAxon->pElementAvAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementAvAuron->sNavn <<" - " <<pPostNodeDendrite->pElementAvAuron->sNavn <<endl;
		exit(-9);	
	}
	//cout<<"\tO.K.  -  syn. " <<pPreNodeAxon->pElementAvAuron->sNavn <<" til " <<pPostNodeDendrite->pElementAvAuron->sNavn <<" er ikkje lenger\n";
	cout<<endl;


	//{ Rett slutt på utskriftsfil-logg: UTKOMMENTERT
	#if 0
	// no er data slik: [tid, synWeight ] i s_synapse-logg
	synWeight_loggFil<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Synaptic weight;\");\n"

					<<"title \"Synaptic weight for s_synapse: " <<pPreNodeAxon->pElementAvAuron->sNavn <<" -> " <<pPostNodeDendrite->pElementAvAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel syn.w.\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(" <<OCTAVE_SLEEP_ETTER_PLOTTA <<"); "
					<<"print(\'eps__s_synapse_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');" 	;
	synWeight_loggFil.close();
	#endif
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
s_axon::s_axon(s_auron* pAuronArg) : i_axon("s_axon"), pElementAvAuron(pAuronArg)
{ //{3 
	cout<<"\tConstructor :\ts_axon::s_axon(s_auron*)\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
} //}3
s_axon::~s_axon()
{ //{3
	cout<<"\tDESTRUCTOR :\ts_axon::~s_axon() for auron \t" <<pElementAvAuron->sNavn <<"\n";

} //}3
//}2
//{2 ***  i_axon
K_axon::K_axon( K_auron* pAuronArg) : i_axon("K_axon" ), pElementAvAuron(pAuronArg)
{
	cout<<"\tConstructor :\tK_axon::K_axon( K_axon*)\n";
}
K_axon::~K_axon()
{
	//TOM XXX TOM
}
//}2
//}1

//{1 * DENDRITE
//{2 *** i_dendrite
i_dendrite::i_dendrite(/*i_auron* pElementAvAuron_arg,*/ std::string sNavn ="dendrite") : timeInterface(sNavn)
{ //{3 
	//pElementAvAuron = pElementAvAuron_arg;
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
s_dendrite::s_dendrite( s_auron* pPostSynAuron_Arg ) : i_dendrite("s_dendrite"), pElementAvAuron(pPostSynAuron_Arg)
{ //{3
	cout<<"\tConstructor :\ts_dendrite::s_dendrite( s_auron* )\n";// for \tauron " <<pElementAvAuron->sNavn <<endl;

	bBlockInput_refractionTime = false;
} //}3
s_dendrite::~s_dendrite()
{ //{3
	cout<<"\tDesructor :\ts_dendrite::~s_dendrite() \t for auron \t" <<pElementAvAuron->sNavn <<"\n";
} //}3
//}2 XXX s_dendrite 
//{2 *** K_dendrite
K_dendrite::K_dendrite( K_auron* pPostSynAuron_Arg ) : i_dendrite("K_dendrite" ), pElementAvAuron(pPostSynAuron_Arg)
{
	cout<<"\tConstructor :\tK_dendrite::K_dendrite( K_auron* ) \t\t[TOM]\n";
}
K_dendrite::~K_dendrite()
{
	cout<<"\tDesructor :\tK_dendrite::~K_dendrite() \t\t for auron \t" <<pElementAvAuron->sNavn <<"\t[TOM]\n";
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
	//pElementAvAuron->dAktivitetsVariabel += dNewSignal_arg; SKJER I changeKappa(a)
	pElementAvAuron->changeKappa( dNewSignal_arg );
} //}2

inline void K_auron::changeKappa( double dInputDerived_arg)//int derivedInput_arg )
{
	/************************
	* Legg arg til i Kappa  *
	************************/

	// Viktig å kalkulere depol med GAMMEL Kappa! Ellers får vi hopp i depol!
	dDepolAtStartOfTimeWindow = calculateDepol();
	ulStartOfTimewindow = time_class::getTid();
	
	// Oppdaterer Kappa
	dAktivitetsVariabel += dInputDerived_arg;

	time_class::addCalculationIn_pCalculatationTaskQue( this );

	//bEndraKappaDennePerioden = true;

	writeKappaToLog();
	
}
//}1

//{1  * SANN

inline void s_dendrite::newInputSignal( double dNewSignal_arg )
{ //{2 .. }

	// Dersom input er blokkert er enten noden i 'refraction period' eller så fyrer det allerede for denne iterasjonen. (trenger ikkje kalkulere meir på depol..)
 	if( bBlockInput_refractionTime ) return;
	#if DEBUG_UTSKRIFTS_NIVAA > 5
	else{ cout<<"forrige fyring på tid: " <<pElementAvAuron->ulTimestampForrigeFyring <<endl; }
	#endif

	// beregner lekkasje av depol siden sist:
	calculateLeakage();

	pElementAvAuron->dAktivitetsVariabel += dNewSignal_arg;
	#if DEBUG_UTSKRIFTS_NIVAA > 2
	cout<<"s_dendrite::newInputSignal( " <<dNewSignal_arg <<" ); gir depol. :  " <<pElementAvAuron->dAktivitetsVariabel <<"\n";
	#endif
	
	pElementAvAuron->ulTimestampForrigeInput = time_class::getTid();


	// Dersom auron går over fyringsterskel: fyr A.P.
	if( pElementAvAuron->dAktivitetsVariabel > FYRINGSTERSKEL )
	{
		// Blokkerer videre input (etter at vi allerede har nådd terskel)
		bBlockInput_refractionTime = true;

		//Ekstra time-delay:
		time_class::addTaskIn_pWorkTaskQue( this );
		// for ekstra time delay. Evt kan vi kjøre auron som neste ledd:
		// Legger til neste ledd i signal-path (soma == auron).
	 	//time_class::addTaskIn_pWorkTaskQue( pElementAvAuron );

	}

	// Skriver til log for aktivitetsVar:
	pElementAvAuron->depol_logFile 	<<time_class::getTid() <<"\t" <<pElementAvAuron->dAktivitetsVariabel <<"; \t #Depolarization\n" ;
	pElementAvAuron->depol_logFile.flush();
} //}2

inline void s_dendrite::calculateLeakage()
{ //{2 /** Bare for SANN:  ***/
	/*
	* 	Har testa verdiane i octave. Loggen følgte lå på kurva for [initDepol]*LEKKASJEFAKTOR_FOR_DEPOL^x, selv om eg bare sjekka calculateLeakage() for siste ledd 
	* 		(f.eks. etter 30 iterasjoner uten noko input anna enn init-verdien for depool. for auronet). Hurra!
	*/

	int slettDebugGammelDepolverdi = pElementAvAuron->dAktivitetsVariabel;

	if( pElementAvAuron->ulTimestampForrigeInput != time_class::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
		unsigned long sulTidSidenSist = time_class::getTid()-pElementAvAuron->ulTimestampForrigeInput;

		// SKRIVER TIL dAktivitetsVariabel.
	 	pElementAvAuron->dAktivitetsVariabel *= (double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist );
		
		#if DEBUG_UTSKRIFTS_NIVAA > 3
		cout 	<<"\n\n\t\t\t\t\tLEKKASJEfaktor: " <<(double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist ) <<" [gammel => ny depol.]: [" <<slettDebugGammelDepolverdi <<"=>" <<pElementAvAuron->dAktivitetsVariabel
				<<"]. (for auron " <<pElementAvAuron->sNavn <<")\n";
		#endif
		// Bruker heller auron::ulTimestampForrigeInput..
		//ulTimestampForrigeOppdatering = time_class::getTid(); 
	}
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
 	cout<<"i_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";

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

	#if DEBUG_UTSKRIFTS_NIVAA > 2
	cout<<"\t" <<sNavn <<".doTask()\tFYRER Action Potential for neuron " <<sNavn <<".\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | \ttid: " <<time_class::getTid() <<"\n";
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

	writeDepolToLog();

} //}
inline void s_axon::doTask()
{ //{ ** AXON

DEBUG("s_axon::doTask() START");
	// Avblokkerer dendritt. Opner den for meir input. Foreløpig er dette måten 'refraction time' funker på.. (etter 2 ms-dendrite og auron overføring..)
	//static_cast<s_dendrite*>(pElementAvAuron->pInputDendrite)->bBlockInput_refractionTime = false;
	pElementAvAuron->pInputDendrite ->bBlockInput_refractionTime = false;

	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"s_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";
	#endif

	// Legger til alle utsynapser i pWorkTaskQue:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
 	for( std::list<s_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i time_class::pWorkTaskQue: (FIFO-kø)
		time_class::addTaskIn_pWorkTaskQue( *iter );
	}

	 //Skriver til logg etter refraction-period.
	 pElementAvAuron->writeDepolToLog();
DEBUG("s_axon::doTask() SLUTT");

} //}
inline void s_synapse::doTask()
{ //{2 ** SYNAPSE

	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout<<"Overføring i s_synapse.\n";
	#endif

	// Dersom synapsen har inhibitorisk effekt: send inhibitorisk signal (subtraksjon). Ellers: eksiter postsyn. auron.
	// Dendrite lagrer tidspunk for overføring.
	// Istedenfor sender inn pos. eller neg. signal avhengig av bInhibitorisk_effekt: [ 1-2*bInhibitorisk_effekt ]  Gir enten +1 dersom bInhibitorisk_effekt er false (=0) eller -1 om bInhibitorisk_effekt er true (=1).
	if( bInhibitorisk_effekt ){
 		pPostNodeDendrite->newInputSignal(  -dSynapticWeight );
	}else{
		pPostNodeDendrite->newInputSignal(   dSynapticWeight );
	}
//( dendrite legges til i arbeidskø dersom den kommer over fyringsterskel.) --Dette er feilaktig, men meir effektivt.
		


	// XXX Kalkulere syn.p.?



	// TODO: synaptisk plastisitet: HER.
	//{ Loggfører syn.weight
	synWeight_loggFil 	<<"\t" <<time_class::getTid() <<"\t" <<dSynapticWeight
						<<" ;   \t#Synpaptic weight\n" ;
	synWeight_loggFil.flush();
			//} 

	// Logg for aktivitetsVar for postsyn auron skjer i pPostNodeDendrite->newInputSignal(-);

} //}2
inline void s_dendrite::doTask()
{ //{ DENDRITE
	// Kva skal dendrite::doTask() gjøre? 
	//cout<<pElementAvAuron->sNavn <<"->[dendrite]::doTask(). Postsyn. depol (" <<pElementAvAuron->sNavn <<") etter overføring: " <<pElementAvAuron->nAktivitetsVariabel <<".\n";
	time_class::addTaskIn_pWorkTaskQue( pElementAvAuron );
} //}
//}1            *       slutt SANN

//{1 		* 	KANN


inline void K_auron::doTask()
{ //{ ** AURON

	// Feilsjekk! Sjekker om kappa har vore over Tau, siste 'time window'
	#if 0
	if( !bKappaLargerThanThreshold_lastIter )
	{
		// VeittaFAEN kvifor!
		#if DEBUG_UTSKRIFTS_NIVAA > 1
		cout<<"ERROR \t ERROR \t ERROR \t ERROR \t Fyring mens K<T forrige time window: bKappaLargerThanThreshold_lastIter: false" <<endl;
		#endif
		return;
	}
	#endif

	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout 	<<"K_auron::doTask():\n"
			<<"\tdLastCalculatedPeriod :\t" <<dLastCalculatedPeriod <<endl
			<<"\tdPeriodINVERSE :\t" <<dPeriodINVERSE <<endl;
	#endif


	//Utskrift til skjerm:
	#if DEBUG_UTSKRIFTS_NIVAA > 1
	cout<<"\t| | " <<sNavn <<" | | | " <<sNavn <<" | | | | " <<sNavn <<" | | | | " <<sNavn <<" | | | | " <<sNavn <<" | | | | " <<sNavn <<"| | | | | | | | |\t"
		<<sNavn <<".doTask()\tFYRER neuron " <<sNavn <<".\t\t| | | |  [periode] = " <<dLastCalculatedPeriod/1000 <<"     | | | | | | | | | | | | | | | | | | \ttid: " <<time_class::getTid() <<"\n"
		
		<<"\t| | Kappa:" <<dAktivitetsVariabel <<"\tForrige periode:" <<dLastCalculatedPeriod <<"\tDepol før fyring:" <<dDepolAtStartOfTimeWindow <<"\n"
		<<endl;
	#endif

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output. // bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	// TODO Dette kan gjøres direkte, for å slippe litt jobb: Dersom vi legger til 1 i fyringsestimatet, er ikkje axon-delay naudsynt.. TODO
	time_class::addTaskIn_pWorkTaskQue( pOutputAxon );

	// Resetter bEndraKappaDennePerioden. Blir satt til true når auron får nytt input.
	//bEndraKappaDennePerioden = false;
	
	// Setter v_0 til 0 og t_0 til [no]:
	dDepolAtStartOfTimeWindow = 0;
	ulStartOfTimewindow = time_class::getTid();

	// Oppdaterer lEstimatedTaskTime_for_object til [no] + dLastCalculatedPeriod:
	lEstimatedTaskTime_for_object = time_class::getTid() + (unsigned)dLastCalculatedPeriod;

	//if( bKappaLargerThanThreshold_lastIter ){
		// Har testa litt, og det er definitivt best å kjøre neste tre linjene, iforhold til å kjøre doCalculation()!
		//(doCalculation() varianten slutta aldri på 40K tidssteg, neste-tre-linje varianten slutta etter 30 sekund på 1000K tidsiterasjoner..
		#if 1
		// Beregn ny isi-periode^{-1}. Brukes til å beregne syn.overføring seinare i signal-cascade.
		dLastCalculatedPeriod = (- log((dAktivitetsVariabel - FYRINGSTERSKEL) / dAktivitetsVariabel) / ALPHA);
		
		double dPeriodInverse_temp =  (1/dLastCalculatedPeriod);
 		dChangeInPeriodINVERSE = dPeriodInverse_temp - dPeriodINVERSE ;
		dPeriodINVERSE = dPeriodInverse_temp;
		#else
		//Kjør heller det over enn .doCalculations() ... Der er jo en del meir jobb.. (Krever sattans mykje meir arbeid!)
		//doCalculation();
		#endif

		// Legger seg selv til i pEstimatedTaskTime om dLastCalculatedPeriod time iterations: (forrige peiker i fra pEstimatedTaskTime til dette K_auron ble fjærnet når peiker til dette obj ble lagt inn i pWorkTaskQue).
		#if ! KOMMENTER_UT_pEstimatedTaskTime
		time_class::addTask_in_pEstimatedTaskTime_list( this,  dLastCalculatedPeriod );
		#endif
		// evt TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	/*	
	}else{
		// For å få den vekk fra planlagte tasks (legges til som [no]+[periode], og sjekkes om element ligger på plass [no]+1 i time_class ::doTask()..
		dLastCalculatedPeriod = 0;
		dPeriodINVERSE = 0;
		dChangeInPeriodINVERSE = 0; //??? XXX

		cout<<"Eg forstår ikkje kvifor den fyrer igjen etter dette. Skriver ut litt variabler:\n"
			<<"v_0:\t" <<dDepolAtStartOfTimeWindow <<"\nEstimert fyretid om :\t" <<dLastCalculatedPeriod <<"\nlEstimatedTaskTime_for_object:\t" <<lEstimatedTaskTime_for_object <<endl;
	}
*/
	// Logger AP (vertikal strek)
	writeAPtoLog();
	
	//	time_class::TEST_skrivUt_pEstimatedTaskTime();
	
} //}
inline void K_axon::doTask()
{ //{ ** AXON
	// Legg heller til en i pEstimatedTaskTime, og kall i_axon::doTask() direkte fra K_auron.
//	i_axon::doTask();
	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"K_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";
	#endif

	// Legg til alle utsynapser i pWorkTaskQue:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
 	for( std::list<K_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i time_class::pWorkTaskQue: (FIFO-kø)
		time_class::addTaskIn_pWorkTaskQue( *iter );
	}

	
	// Gjøres i K_auron. Skal eg også gjøre det her (for å poengtere refraction time)?
	pElementAvAuron->writeDepolToLog();
} //}

inline void K_synapse::doTask()
{ //{ ** SYNAPSE
	// K_ij = w_ij / p(K_j) Differansen Delta K_ij blir sendt som argument til pPostNodeDendrite->newInputSignal( argument );
	// 	denne er gitt som Delta K_ij = dt * w_ij / Delta p(K_j)

	// Istedenfor sender inn pos. eller neg. signal avhengig av bInhibitorisk_effekt: [ 1-2*bInhibitorisk_effekt ]  Gir enten +1 dersom bInhibitorisk_effekt er false (=0) eller -1 om bInhibitorisk_effekt er true (=1).
	pPostNodeDendrite->newInputSignal( (1-2*bInhibitorisk_effekt) * dSynapticWeight * (pPreNodeAxon->pElementAvAuron)->dChangeInPeriodINVERSE );

	dPresynPeriodINVERSE = (pPreNodeAxon->pElementAvAuron)->dPeriodINVERSE;

	
	#if DEBUG_UTSKRIFTS_NIVAA > 6
 	cout<<"K_synapse::doTask()\tdSynapticWeight: " <<dSynapticWeight 
		<<", preNode-dPeriodINVERSE: " <<(pPreNodeAxon->pElementAvAuron)->dPeriodINVERSE 	<<endl;
	#endif
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
	// Bare debugging--utskrift:
	//if(p CalculatationTaskQue.empty()) c out<<"TOM p CalculatationTaskQue\n"; // BARE debugging! XXX Fjærn tilslutt..

	// gjennomfører planlagte kalkulasjoner:
	doCalculation();


	#if ! KOMMENTER_UT_pEstimatedTaskTime //{
		// pEstimatedTaskTime opplegg ************* 	****** 		********* 		*************  ************************************
	
		// Undersøker om lengden på lista er kortere enn MIN_LENGDE_PAA_pEstimatedTaskTime:
		if(pEstimatedTaskTime.size() < MIN_LENGDE_PAA_pEstimatedTaskTime )
		{ //{
			//cerr<<"Legger inn manglende element for MIN_LENGDE_PAA_pEstimatedTaskTime i pEstimatedTaskTime\n";
	
				// Legger inn MIN_LENGDE_PAA_pEstimatedTaskTime antall element.
			for( int i=0; i<MIN_LENGDE_PAA_pEstimatedTaskTime; i++)
				pEstimatedTaskTime.push_back( new std::list<timeInterface*> );
		} //}
		// Greit. pEstimatedTaskTime er minimum MIN_LENGDE_PAA_pEstimatedTaskTime lang.
	
		#if DEBUG_UTSKRIFTS_NIVAA > 3
		if( ! pEstimatedTaskTime.front()->empty() ){
			cout<<"* * * * * * * * * * * * * * * * * * *\n* *  Planlagt oppgave i tid: " <<time_class::getTid() <<"  * *\n* * * * * * * * * * * * * * * * * * * \n\n";
		}
		#endif
		
		timeInterface* pTI_temp;
		while( ! (pEstimatedTaskTime.front())->empty() ) // Kjør alle element i FØRSTE element av pEstimatedTaskTime.
		{
			// pTI_temp peiker får verdien til første timeInterface*-element i pEstimatedTaskTime.front()->front():
			pTI_temp = (pEstimatedTaskTime.front())->front();
	
			#if DEBUG_UTSKRIFTS_NIVAA > 2
			cout<<"pEstimatedTaskTime fører til at eg legger til " <<pTI_temp->sClassName <<" i pWorkTaskQue\n";
			#endif
	
			pWorkTaskQue.push_back( pTI_temp ); //Legger til første element av første liste i pWorkTaskQue
			// legg elementet i pEstimatedTaskTime om [periode] tid
				
			// fjærner element fra { pEstimatedTaskTime.[dette tidssteget] }
			(pEstimatedTaskTime.front()) ->pop_front(); 			//Fjærn første element (av første lista).
	
			// Legger til element om [periode] tidsiterasjoner. (Skjer i [element].doTask()
	
		}
		
		// Organiserer pEstimatedTaskTime. Fjærner første ledd:
			// free minnet fra førte element (ligger nemmelig i frie lageret)
		delete pEstimatedTaskTime.front();  // XXX XXX XXX Blir dette rett? XXX (laga den med new std::list<timeInterface*> (den er bare en peiker til list<> i frie lageret..))
			// pop element (ta vekk fra liste)
		pEstimatedTaskTime.pop_front(); //Fjærn heile lista med estimerte oppgaver for neste time_iteration.
			
	#endif //}
	// Ferdig: pEstimatedTaskTime opplegg ************* 	****** 		********* 		*************  ************************************


	//itererer time:
	ulTidsiterasjoner++;

	// utskrift:
	#if UTSKRIFT_AV_TID
	//if(ulTidsiterasjoner%100 == 0)		
		cout<<"\t* * * * TID: \t  =  " <<ulTidsiterasjoner <<" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * = "
			<<ulTidsiterasjoner <<"\n";
	#endif


	/*******************************
	* Oppdater alle K_sensor_auron *
	*******************************/
	K_sensor_auron::updateAllSensorAurons();

	
	/*************************************************
	* Flytter planlagde oppgaver over i pWorkTaskQue *
	*************************************************/
	// Sjekker alle K_auron:
	for( std::list<K_auron*>::iterator K_iter = K_auron::pAllKappaAurons.begin() ; K_iter != K_auron::pAllKappaAurons.end() ; K_iter++ )
	{
		if( (*K_iter)->lEstimatedTaskTime_for_object == ulTidsiterasjoner+1 )
		{
			addTaskIn_pWorkTaskQue( (*K_iter) );
		}
	
		// Sjekk om kappa skal rekalkuleres (TODO dette skal være ei egen liste. Sjå under)
		// TODO IKKJE SLIK:
	//	if( (*K_iter)->kappaRecalculator.lEstimatedTaskTime_for_object == ulTidsiterasjoner+1 ) 
	//	{
	//		addTaskIn_pWorkTaskQue( &((*K_iter)->kappaRecalculator) );
	//	}
 	}

	// Lag ei tilsvarende liste for recalcKappaClass.
			//(*K_iter)->kappaRecalculator . doTask();

	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	// HUGS å legge inn ei ekstra liste for tidselement (element som kan planlegges) som ikkje er K_auron.
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 




	/*XXXXXXXXXXXXXXX
	X 	 TESTING 	X
	XXXXXXXXXXXXXXXX*/
	// Kaller testfunk for å teste K_auron. 
	K_auron::loggeFunk_K_auron();

	
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
	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"K_auron " <<sNavn <<".doCalculation()\t\t" <<sNavn <<".doCalculation()\t\tTid: " <<time_class::getTid() <<"\n";
	#endif

	#if DEBUG_UTSKRIFTS_NIVAA > 3
 	cout<<"[K, T] = " <<dAktivitetsVariabel <<", " <<FYRINGSTERSKEL <<endl;
	#endif


	//**********************************************
	//*  Beregn ny depol og estimert fyringstid:   *
	//**********************************************
	// Beregner ny v_0 : depolarisasjon ved slutt av dette time window (v_0 for neste 'time window'..) 								v_0 = (v_0,forrige - K)e^-at + K

	// Veldig viktig å hugse å kalkulere dDepolAtStartOfTimeWindow på slutten av forrige time window! (og lagre tidspunkt)

	// TODO :  Følgende er utesta (element med bKappaLargerThanThreshold_lastIter)

	/******************************************************************
	*  Kjører beregning av dPeriodINVERSE og dChangeInPeriodINVERSE:  *
	******************************************************************/
	static double dPeriodInverse_static_local;
		
	// Berenger dPeriodINVERSE og dChangeInPeriodINVERSE:
	dLastCalculatedPeriod  = ( log( dAktivitetsVariabel / (dAktivitetsVariabel - FYRINGSTERSKEL) ) / ALPHA);
	dPeriodInverse_static_local = 1/dLastCalculatedPeriod;
	dChangeInPeriodINVERSE = dPeriodInverse_static_local - dPeriodINVERSE; //  		Her var det kjørt med uPeriod_temp. Endra til dPeriod_temp. UTESTA
	dPeriodINVERSE = dPeriodInverse_static_local;

#if 0 //{
	// if(K>T)
	if( dAktivitetsVariabel > FYRINGSTERSKEL ) // if(kappa>tau)
	{
		#if DEBUG_UTSKRIFTS_NIVAA > 3
			cout<<"K<T : [ " <<dAktivitetsVariabel <<">" <<FYRINGSTERSKEL <<" ]\n";
		#endif 
		
		// lEstimatedTaskTime_for_object skal ikkje settes her! Dette skal gjøres i moveTask!
		// Beregner nytt fyringsestimat:      [no] 			 +  	[estimert tid til fyring]
		//lEstimatedTaskTime_for_object = time_class::getTid() + ( log( (dAktivitetsVariabel-dDepolAtStartOfTimeWindow)/(dAktivitetsVariabel-FYRINGSTERSKEL) )   /  ALPHA );

		#if DEBUG_UTSKRIFTS_NIVAA > 3
/*XXX*/	cout<<"lEstimatedTaskTime_for_object     = \t" <<( time_class::getTid() + log( (dAktivitetsVariabel-dDepolAtStartOfTimeWindow)/(dAktivitetsVariabel-FYRINGSTERSKEL) )   /  ALPHA ) <<"\n\n\n\n";
		#endif
	



		// TEST var naudsynt for pEstimatedTaskTime-opplegget (deprecated..)
		#if ! KOMMENTER_UT_pEstimatedTaskTime
		if( bKappaLargerThanThreshold_lastIter )
		{
			
			#if DEBUG_UTSKRIFTS_NIVAA > 3
				cout<<"K>T, forrige iter og no: lEstimatedTaskTime_for_object = " <<lEstimatedTaskTime_for_object <<endl;
			#endif
		 	time_class::moveTask_in_pEstimatedTaskTime_list( this, ( log( (dAktivitetsVariabel-dDepolAtStartOfTimeWindow)/(dAktivitetsVariabel-FYRINGSTERSKEL) )   /  ALPHA ) );
		}else{ // Forrige iter: K var mindre enn Tau.
			#if DEBUG_UTSKRIFTS_NIVAA > 1
				cout<<"K>T no, men ikkje forrige iter: lEstimatedTaskTime_for_object = " <<lEstimatedTaskTime_for_object <<endl;
			#endif
		}
		#endif



		bKappaLargerThanThreshold_lastIter=true;
		
	} // slutt: if(kappa>tau)
	else{ // Kappa < Tau
DEBUG("HERHER HER HER HER HER\n\nHERN HER\n\n");
		if( bKappaLargerThanThreshold_lastIter ){
			#if ! KOMMENTER_UT_pEstimatedTaskTime
			time_class::removeTask_in_pEstimatedTaskTime_list( this );
			#endif
		}
		bKappaLargerThanThreshold_lastIter = false;
	}
#endif //}
// Husk å enten fjærne bKappaLargerThanThreshold_lastIter, eller skrive denne.



	// Hugs å sette lEstimatedTaskTime_for_object ! 
	// 	For K_auron trenger vi ikkje legge til elementet i [liste som skal sjekkes]. pAllKappaAurons sjekkes alltid..
	#if KOMMENTER_UT_pEstimatedTaskTime
	lEstimatedTaskTime_for_object = ( time_class::getTid() + log( (dAktivitetsVariabel-dDepolAtStartOfTimeWindow)/(dAktivitetsVariabel-FYRINGSTERSKEL) )   /  ALPHA ) ;
	#endif

	
	// Skriver til log for depol:
	writeDepolToLog();
} //}

/**************************************************************
****** 			K_sensor_auron - senseFunksjoner        *******
**************************************************************/
//{1
	// Kvar enkelt K_sensor_auron har en funksjonspeiker som peiker på dens sense-funksjon. Denne skal være av type [ double F(void ].
	// 	K_sensor_auron har en static funksjon som beregner ny kappa for alle senseauron, kalla fra time_class:: doTask()
inline void K_sensor_auron::updateAllSensorAurons()
{

	// Itererer gjennom lista pAllSensorAurons, og kaller updateSensorValue() for de.
	for( std::list<K_sensor_auron*>::iterator sensorIter = pAllSensorAurons.begin() 	; 	sensorIter != pAllSensorAurons.end() ; sensorIter++)
	{
		(*sensorIter)->updateSensorValue();
	}
}

inline void K_sensor_auron::updateSensorValue()
{
	#if DEBUG_UTSKRIFTS_NIVAA > 2
	DEBUG("K_sensor_auron::updateSensorValue()");
	#endif

	// To variabler for å finne deriverte. Denne skal bestemme ny kappa..
	dLastSensedValue = dSensedValue;
	dSensedValue = (*pSensorFunction)();

	changeKappa( dSensedValue-dLastSensedValue );
	
	#if DEBUG_UTSKRIFTS_NIVAA > 3
	cout<<"Kappa for K_sensor_auron: " <<dAktivitetsVariabel <<"\n\n";
	#endif
}
	
//}1



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

	cout<<"Rekalkulerer kappa: recalculateKappa::doTask() for auron " <<pKappaAuron_obj->sNavn 
		<<"\nFeil: " <<dFeil <<endl;

	// Sørge for å legge til planlagt rekalkulering om DEFAULT_PERIODE_MELLOM_RECALC_KAPPA tid:
	lEstimatedTaskTime_for_object = time_class::getTid()+DEFAULT_PERIODE_MELLOM_RECALC_KAPPA;

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

	cout<<"[Kappa, dKappa_temp, dKappaFeil_temp] : " <<dAktivitetsVariabel <<", " <<dKappa_temp <<", " <<dKappaFeil_temp <<endl;
	dAktivitetsVariabel = dKappa_temp;


	return dKappaFeil_temp;	
} //}


inline double K_dendrite::recalculateKappa()
{ //{
	double dKappa_temp = 0;
 	for( std::list<K_synapse*>::iterator iter = pInnSynapser.begin() ; iter!=pInnSynapser.end() ; iter++)
	{
		cout<<"Transmission: " <<(*iter)->getTransmission() <<endl;
 		dKappa_temp += (*iter)->getTransmission();
	}
	return dKappa_temp;
} //}


/*******************
**     ANNA:      **
*******************/




void neuroElement_testFunk(K_auron* pK_arg)
{
	pK_arg->recalculateKappa();
}


// vim:fdm=marker:fmr=//{,//}
