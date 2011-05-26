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



std::ostream & operator<< (std::ostream & ut, i_auron* pAuronArg );
//std::ostream & operator<< (std::ostream & ut, s_axon* pAxonArg );
std::ostream & operator<<(std::ostream& ut, i_auron* pAuronArg );



//Bedre oppdeling: (uferdig (ustarta)

/********************************
** Contructors and destructors **
********************************/
//{1 		* 	interface-class

//{2  ***  i_auron
//}
//{2  ***  i_ axon
//}
//{2  ***  i_synapse
//}
//{2  ***  i_dendrite
//}

//}1
//{1 		* 	SANN

//{2  ***  S_auron
//}
//{2  ***  S_ axon
//}
//{2  ***  S_synapse
//}
//{2  ***  S_dendrite
//}

//}1
//{1 		* 	KANN

//{2  ***  K_auron
//}
//{2  ***  K_ axon
//}
//{2  ***  K_synapse
//}
//{2  ***  K_dendrite
//}


//}1







/*************************************************************
****** 													******
******  		CONTRUCTORS OG DESCTRUCTORS 			******
****** 													******
*************************************************************/

//{1 * AURON
//{2 *** i_auron   
i_auron::i_auron(std::string sNavn_Arg /*="unnamed"*/, double dStartAktVar /*=0*/) : timeInterface("auron"), dAktivitetsVariabel(dStartAktVar), sNavn(sNavn_Arg)
{ //{3
	cout<<"CONSTRUCTOR: Lager auron med navn " <<sNavn <<endl;

	// Gjøres heller i underklassene. K_auron kaller doCalculation() direkte.
	//time_class::pCalculatationTaskQue.push_back(this);

	// lag ei .oct - fil, og gjør klar for å kjøres i octave: TODO må flytte denne inn i s_auron og k_auron for å få med dette i navnet (egene log-navn for s_auron og k_auron)
	//{4 Utskrift til logg. LOGG-initiering
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiles_for_evaluation/log_auron" <<sNavn <<"-activityVar" <<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	depol_logFile.open( tempStr.c_str() );
	depol_logFile<<"data=[" <<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<";\n";
	depol_logFile.flush();

	//}4
}  //}3
i_auron::~i_auron()
{ //{3
	cout<<"\tDESTRUCTOR: \ti_auron::~i_auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";
	
	//fjærner seg fra pAllAurons
	pAllAurons.remove(this);

	//{4 Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
	depol_logFile<<time_class::getTid() <<"\t" <<dAktivitetsVariabel <<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \"@;Activity variable;\");\n"

					<<"title \"Activity variable for auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					<<"print(\'eps_auron" <<sNavn <<".eps\', \'-deps\');\n"
					<<"sleep(9); ";
	depol_logFile.close();
	//}4
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
K_auron::K_auron(std::string sNavn_Arg /*="unnamed"*/, int nStartKappa /*= FYRINGSTERSKEL*/, unsigned uStartDepol_prosent /*=0*/) : i_auron(sNavn_Arg)
{
	ulTimestampForrigeFyring = time_class::getTid();
	//ulTimestampForrigeInput  = time_class::getTid();

	ulStartOfTimewindow = time_class::getTid();
	dDepolAtStartOfTimeWindow = uStartDepol_prosent * FYRINGSTERSKEL;


	// Sjå auron.h
	bEndraKappaDennePerioden = false;
	
	// Legger til auron* i std::list<i_auron*> pAllAurons;
	pAllAurons.push_back( this );
	pAllKappaAurons.push_back( this );
	

	//pOutputAxon og pInputDendrite
	pOutputAxon = new K_axon(this);
 	pInputDendrite = new K_dendrite(this); //	Skal ligge i s_auron og k_auron

	// Dersom start-kappa er over fyringsterskel (dette er bare for initiell debugging)
	// Alt taes hand om i changeKappa()
	changeKappa( nStartKappa );


 	cout 	<<"\nConstructing K_auron.\n"
			<<"\tKappa: \t\t\t\t" <<dAktivitetsVariabel <<endl
			<<"\tTau:   \t\t\t\t" <<FYRINGSTERSKEL <<endl
			//<<"\tEstimated time to A.P.: \t" <<(1 / ALPHA) * log( (dDepolAtStartOfTimeWindow - dAktivitetsVariabel) / (float)(FYRINGSTERSKEL - dAktivitetsVariabel) ) 
			<<"\n\n";



	// Kjører auronets doCalculation() for å få rett verdier:
	doCalculation();

}
K_auron::~K_auron()
{
	// pAllAurons - element fjærnes i i_auron. Fjærner pAllKappaAurons-element i K_auron::~K_auron() :
	//pAllKappaAurons.remove(this);
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
	
	cout<<"\t constructor for i_synapse(unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, string navn);\n";
	
} //}3
//}2
//{2 s_synapse
s_synapse::s_synapse(s_auron* pPresynAuron_arg, s_auron* pPostsynAuron_arg, unsigned uSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			:  i_synapse(uSynVekt_Arg, bInhibEffekt_Arg, "s_synapse"), pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite) 
{//{3	



cerr<<"Presyn. axon: " <<pPreNodeAxon->sClassName ;
pPreNodeAxon->SLETTtypeId();
	
	cerr<<"Kaller s_synapse::s_synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);

	

	cout 	<<"\tCONSTRUCTOR : s_synapse::s_synapse(a*, a*) \tEtterpå får vi:\n" 
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


	cout<<"\t\tDESTRUCTOR: s_synapse::~s_synapse() : \t";


	//TODO Sjekk ut std::find() istedenfor. Stroustrup anbefaler å bruke dette!.
	if( !bPreOk ){ 	//Redundant test. Kanskje eg skal skrive while(!bPreOk)?
		//fjærner seg sjølv fra prenode:
		//			TODO gjør om x++ til ++x, siden ++x slepper å lage en "temporary".
		for( std::list<s_synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
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
	//cout<<"\tO.K.  -  syn. " <<pPreNodeAxon->pElementAvAuron->sNavn <<" til " <<pPostNodeDendrite->pElementAvAuron->sNavn <<" er ikkje lenger\n";
	cout<<endl;


	//{4 Rett slutt på utskriftsfil-logg:

	// no er data slik: [tid, synWeight ] i s_synapse-logg
	synWeight_loggFil<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Synaptic weight;\");\n"

					<<"title \"Synaptic weight for s_synapse: " <<pPreNodeAxon->pElementAvAuron->sNavn <<" -> " <<pPostNodeDendrite->pElementAvAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel syn.w.\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(9); "
					<<"print(\'eps__s_synapse_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');" 	;
	synWeight_loggFil.close();
	//}4

} //}3
//}2
//{2 K_synapse
K_synapse::K_synapse(K_auron* pPresynAuron_arg, K_auron* pPostsynAuron_arg, unsigned uSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/ )
 :  i_synapse(uSynVekt_Arg, bInhibEffekt_Arg, "s_synapse") , pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite), dPresynPeriodINVERSE(0)
{ 	//XXX HER: nytt kappa element:
	cout<<"Kaller K_synapse::K_synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	

//	cout 	<<"\tCONSTRUCTOR : K_synapse::K_synapse(a*, a*) \tEtterpå får vi:\n" 
//			<<(pPreNodeAxon) <<endl;


	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	//{ Utskrift til logg. LOGG-initiering
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

	//}
}
K_synapse::~K_synapse()
{ // XXX TODO UTESTA! 			HAR IKKEJ SETT GJENNOM, engang. TODAY: 12.04.2011 		TODO TODO
	bool bPreOk  = false;
	bool bPostOk = false;


cout<<"\t\tDESTRUCTOR: K_synapse::~<K_synapse() : \t";


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


	//{ Rett slutt på utskriftsfil-logg:

	// no er data slik: [tid, synWeight ] i s_synapse-logg
	synWeight_loggFil<<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \";Synaptic weight;\");\n"

					<<"title \"Synaptic weight for s_synapse: " <<pPreNodeAxon->pElementAvAuron->sNavn <<" -> " <<pPostNodeDendrite->pElementAvAuron->sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel syn.w.\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					//<<"print(\'eps_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');\"\n"
					<<"sleep(9); "
					<<"print(\'eps__s_synapse_" <<pPreNodeAxon->pElementAvAuron->sNavn <<"->" <<pPostNodeDendrite->pElementAvAuron->sNavn <<".eps\', \'-deps\');" 	;
	synWeight_loggFil.close();
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
	cout<<"\tlager axon\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
} //}3
s_axon::~s_axon()
{ //{3
	cout<<"\tDESTRUCTOR:  s_axon::~s_axon() for auron \t" <<pElementAvAuron->sNavn <<"\n";

} //}3
//}2
//{2 ***  i_axon
K_axon::K_axon( K_auron* pAuronArg) : i_axon("K_axon" ), pElementAvAuron(pAuronArg)
{
	cout<<"\tlager axon\n";
}
K_axon::~K_axon()
{
	 
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
	cout<<"\tLager s_dendrite\n";// for \tauron " <<pElementAvAuron->sNavn <<endl;

	bBlockInput_refractionTime = false;
} //}3
s_dendrite::~s_dendrite()
{ //{3
	cout<<"\tDestruerer s_dendrite for auron \t" <<pElementAvAuron->sNavn <<"\n";
} //}3
//}2 XXX s_dendrite 
//{2 *** K_dendrite
K_dendrite::K_dendrite( K_auron* pPostSynAuron_Arg ) : i_dendrite("K_dendrite" ), pElementAvAuron(pPostSynAuron_Arg)
{
	cout<<"\tLager K_dendrite\n";
}
K_dendrite::~K_dendrite()
{
	cout<<"\tDestruerer K_dendrite for auron \t" <<pElementAvAuron->sNavn <<"\n";
}
//}2 XXX K_dendrite
//}1 X DENDRITE



/*************************************************************
****** 													******
******  		ANDRE FUNKSJONER:           			******
****** 													******
*************************************************************/



inline void s_dendrite::newInputSignal( double dNewSignal_arg )
{ //{

	// Dersom input er blokkert er enten noden i 'refraction period' eller så fyrer det allerede for denne iterasjonen. (trenger ikkje kalkulere meir på depol..)
 	if( bBlockInput_refractionTime ) return;
	else{ cout<<"forrige fyring på tid: " <<pElementAvAuron->ulTimestampForrigeFyring <<endl; }

	// beregner lekkasje av depol siden sist:
	calculateLeakage();

	pElementAvAuron->dAktivitetsVariabel += dNewSignal_arg;
	cout<<"s_dendrite::newInputSignal( " <<dNewSignal_arg <<" ); gir depol. :  " <<pElementAvAuron->dAktivitetsVariabel <<"\n";
	
	pElementAvAuron->ulTimestampForrigeInput = time_class::getTid();


	// Dersom auron går over fyringsterskel: fyr A.P.
	if( pElementAvAuron->dAktivitetsVariabel > FYRINGSTERSKEL )
	{
		// Blokkerer videre input (etter at vi allerede har nådd terskel)
		bBlockInput_refractionTime = true;

		//cout<<"Postsyn. kom over FYRINGSTERSKEL.\nLegger til i arbeidskø.\n\n";
		
	 	
		//Ekstra time-delay:
		time_class::addTaskIn_pWorkTaskQue( this );
		// for ekstra time delay. Evt kan vi kjøre auron som neste ledd:
		// Legger til neste ledd i signal-path (soma == auron).
	 	//time_class::addTaskIn_pWorkTaskQue( pElementAvAuron );

	}

	// Skriver til log for aktivitetsVar:
	pElementAvAuron->depol_logFile 	<<time_class::getTid() <<"\t" <<pElementAvAuron->dAktivitetsVariabel <<"; \t #Depolarization\n" ;
	pElementAvAuron->depol_logFile.flush();
} //}
inline void K_dendrite::newInputSignal( double dNewSignal_arg )
{ //{1
	// TODO Sikkert meir effektivt å plassere alt dette i K_auron. Da slepp eg alle peikeroperasjonane.. Kan for eksempel lage K_auron::endreKappa(); og plassere alt der..

	//cout<<"\n\n \t\t\t\tK_dendrite::newInputSignal( " <<dNewSignal_arg <<" )\t--arg er i promille: gir K_ij=" <<(double)dNewSignal_arg/1000 <<endl;

	//HER XxX xXx casting (eksplisitt typekonvertering) fra i_auron til K_auron for å kunne accessere K_auron::bEndraKappaDennePerioden.
	// Ikkje lenger naudsynt:
	//static_cast<K_auron*>(pElementAvAuron)->nAktivitetsVariabel += dNewSignal_arg;
	// (har gjort K_dendrite->pElementAvAuron om fra type i_auron* til type K_auron*.
	pElementAvAuron->dAktivitetsVariabel += dNewSignal_arg;

	pElementAvAuron->changeKappa( dNewSignal_arg );

	// Flytter auron i pEstimatedTaskTime



	// TODO Kanskje skrive for Kappa (aktivitetsVar) også (for feilsjekk / interresse).

	// TODO Legg inn funksjonalitet for å flytte auron* i pEstimatedTaskTime-lista (etter oppdatert fyringstidspunkt etter ny kappa..)
} //}1

inline void K_auron::changeKappa( double dInputDerived_arg)//int derivedInput_arg )
{
	/************************
	* Legg arg til i Kappa  *
	************************/
	dAktivitetsVariabel += dInputDerived_arg;

	time_class::addCalculationIn_pCalculatationTaskQue( this );

	bEndraKappaDennePerioden = true;
	
}

inline void s_dendrite::calculateLeakage()
{ //{1 /** Bare for SANN:  ***/
	/*
	* 	Har testa verdiane i octave. Loggen følgte lå på kurva for [initDepol]*LEKKASJEFAKTOR_FOR_DEPOL^x, selv om eg bare sjekka calculateLeakage() for siste ledd 
	* 		(f.eks. etter 30 iterasjoner uten noko input anna enn init-verdien for depool. for auronet). Hurra!
	*/

	int slettDebugGammelDepolverdi = pElementAvAuron->dAktivitetsVariabel;

	if( pElementAvAuron->ulTimestampForrigeInput != time_class::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
		unsigned long sulTidSidenSist = time_class::getTid()-pElementAvAuron->ulTimestampForrigeInput;

		cout<<"XXX Tid siden sist: " <<sulTidSidenSist <<endl;

		// SKRIVER TIL dAktivitetsVariabel.
	 	pElementAvAuron->dAktivitetsVariabel *= (double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist );
		
		cout 	<<"\n\n\t\t\t\t\tLEKKASJEfaktor: " <<(double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist ) <<" [gammel => ny depol.]: [" <<slettDebugGammelDepolverdi <<"=>" <<pElementAvAuron->dAktivitetsVariabel
				<<"]. (for auron " <<pElementAvAuron->sNavn <<")\n";
		// Bruker heller auron::ulTimestampForrigeInput..
		//ulTimestampForrigeOppdatering = time_class::getTid(); 
	}
	// Dersom den allerede har regna ut lekkasje: ikkje gjør det igjen. Returner.
} //}1




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


	cout<<"\t" <<sNavn <<".doTask()\tFYRER Action Potential for neuron " <<sNavn <<".\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | \ttid: " <<time_class::getTid() <<"\n";

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output. // bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	time_class::addTaskIn_pWorkTaskQue( pOutputAxon );


	if( ulTimestampForrigeFyring == time_class::getTid() )
	{
		cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
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

 	cout<<"s_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";

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

	// FØR  VAR DET :  inline void synapse::transmission()
	cout<<"Overføring i s_synapse.\n";

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

	// Beregn ny isi-periode^{-1}. Brukes til å beregne syn.overføring seinare i signal-cascade.
	uLastCalculatedPeriod = (- log((dAktivitetsVariabel - FYRINGSTERSKEL) / dAktivitetsVariabel) / ALPHA);
	
 	nChangeInPeriodINVERSE = (1/(double)uLastCalculatedPeriod) - dPeriodINVERSE ;
	dPeriodINVERSE = (1/(double)uLastCalculatedPeriod);


cout 	<<"K_auron::doTask()\n"
		<<"uLastCalculatedPeriod :\t" <<uLastCalculatedPeriod <<endl
		<<"dPeriodINVERSE :\t" <<dPeriodINVERSE <<endl;


	// Legger seg selv til i pEstimatedTaskTime om uLastCalculatedPeriod time iterations: (forrige peiker i fra pEstimatedTaskTime til dette K_auron ble fjærnet når peiker til dette obj ble lagt inn i pWorkTaskQue).
	time_class::addTask_in_pEstimatedTaskTime( this,  uLastCalculatedPeriod );
	cout<<"K_auron: legger til [this] peiker til pEstimatedTaskTime om tid:\t" <<uLastCalculatedPeriod <<"\n\n";
	

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output. // bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	time_class::addTaskIn_pWorkTaskQue( pOutputAxon );

	// Resetter bEndraKappaDennePerioden. Blir satt til true når auron får nytt input.
	bEndraKappaDennePerioden = false;
	
	// Setter v_0 til 0 og t_0 til [no]:
	dDepolAtStartOfTimeWindow = 0;
	ulStartOfTimewindow = time_class::getTid();
	
	writeDepolToLog();
	
	//Utskrift til skjerm:
	cout<<"| | " <<sNavn <<" | | | " <<sNavn <<" | | | | " <<sNavn <<" | | | | " <<sNavn <<" | | | | " <<sNavn <<" | | | | " <<sNavn <<"| | | | | | | | |\t"
		<<sNavn <<".doTask()\tFYRER neuron " <<sNavn <<".\t\t| | | |  [periode] = " <<(float)uLastCalculatedPeriod/1000 <<"     | | | | | | | | | | | | | | | | | | \ttid: " <<time_class::getTid() <<"\n\n";


} //}
inline void K_axon::doTask()
{ //{ ** AXON
	// Legg heller til en i pEstimatedTaskTime, og kall i_axon::doTask() direkte fra K_auron.
//	i_axon::doTask();
 	cout<<"K_axon::doTask()\tLegger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";

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
	// 	denne er gitt som Delta K_ij = Delta w_ij / Delta p(K_j)

	// Istedenfor sender inn pos. eller neg. signal avhengig av bInhibitorisk_effekt: [ 1-2*bInhibitorisk_effekt ]  Gir enten +1 dersom bInhibitorisk_effekt er false (=0) eller -1 om bInhibitorisk_effekt er true (=1).
	pPostNodeDendrite->newInputSignal( (1-2*bInhibitorisk_effekt) * dSynapticWeightChange * (pPreNodeAxon->pElementAvAuron)->nChangeInPeriodINVERSE );

	dPresynPeriodINVERSE = (pPreNodeAxon->pElementAvAuron)->dPeriodINVERSE;


 	cout<<"K_synapse::doTask()\tdSynapticWeight: " <<dSynapticWeight 
		<<", preNode-dPeriodINVERSE: " <<(pPreNodeAxon->pElementAvAuron)->dPeriodINVERSE 	<<endl;
} //}
inline void K_dendrite::doTask()
{ //{ XX DENDRITE (ikkje i bruk)
	// TODO Trur ikkje denne skal være med lenger. Legger til brutal feilsjekk: exit(-1);
	cout<<"\n\nBrutal feilsjekk @ K_dendrite::doTask() : exit(-1);\n\n";

	exit(-1);
} //}

//}1            *       KANN slutt


/******************************************************************
****** 														*******
****** 			doCalculation() -- samla på en plass 		*******
****** 														*******
******************************************************************/

void K_auron::doCalculation()
{ //{
	DEBUG("DEBUG: K_auron::doCalculation()");
 	cout<<"dC():\tK_auron " <<sNavn <<".doCalculation()\t\t" <<sNavn <<".doCalculation()\n";

	//**********************************************
	//*  Beregn ny depol og estimert fyringstid:   *
	//**********************************************
	// Beregner ny v_0 : depolarisasjon ved slutt av dette time window (v_0 for neste 'time window'..) 								v_0 = (v_0,forrige - K)e^-at + K
	dDepolAtStartOfTimeWindow = calculateDepol(); 
	//(dDepolAtStartOfTimeWindow - dAktivitetsVariabel)*exp(-(double)ALPHA  * ((time_class::getTid() - ulStartOfTimewindow))) + dAktivitetsVariabel ;
	
	// Skriver ny t_0 (start av 'time window')
	ulStartOfTimewindow = time_class::getTid();

	// if(K>T)
	if( dAktivitetsVariabel > FYRINGSTERSKEL ) // if(kappa>tau)
	{
		
		// Beregner nytt fyringsestimat:
		lEstimatedTaskTime_for_object = ( - log((dAktivitetsVariabel-FYRINGSTERSKEL)/(dAktivitetsVariabel - dDepolAtStartOfTimeWindow)) / ALPHA ); //XXX TODO Trur det skal være med:  * 1000;
		//cout<<"lEstimatedTaskTime_for_object     = \t" << lEstimatedTaskTime_for_object <<endl
		//	<<"dDepolAtStartOfTimeWindow = \t" <<dDepolAtStartOfTimeWindow <<"\n\n\n\n";
	
		// Berenger dPeriodINVERSE og nChangeInPeriodINVERSE:
		unsigned uPeriod_temp  = (- log((dAktivitetsVariabel - FYRINGSTERSKEL) / dAktivitetsVariabel) / ALPHA);
		nChangeInPeriodINVERSE = uPeriod_temp - dPeriodINVERSE;
		uLastCalculatedPeriod  = uPeriod_temp;
	
		/*********************************************************************************************
		** 	Her er det veldig viktig at auronet fyrer når det skal, og ikkje seinare. 				**
		** 		Dersom det sjekkes etter at det skulle fyre, vil verdien wrappe (pga unsigned) 		**
		** 		Case: depol er 999, Kappa endres til høg, => auronet skulle fyrt for "lenge siden". **
		** 		Av denne grunn endrer eg ulEstimatedTimeToFiring til lEstimertTidTilFyring 			**
		** 		(Den får lov å være negativ også. Da kan problemet handteres seinare.. 				**
		*********************************************************************************************/
			
		#if 0 	// DEBUG:
		//{
		cout 	<<"lEstimatedTaskTime_for_object = " <<lEstimatedTaskTime_for_object <<endl <<((dAktivitetsVariabel - FYRINGSTERSKEL)/(dAktivitetsVariabel-dDepolAtStartOfTimeWindow)) <<endl
				<<"lEstimatedTaskTime_for_object = ( - log((dAktivitetsVariabel-FYRINGSTERSKEL)/(dAktivitetsVariabel - dDepolAtStartOfTimeWindow)) / ALPHA ) = "
				<< ( - log((dAktivitetsVariabel-FYRINGSTERSKEL)/(dAktivitetsVariabel - dDepolAtStartOfTimeWindow)) / ALPHA ) <<endl
				<<"\ndAktivitetsVariabel, FYRINGSTERSKEL = " <<dAktivitetsVariabel <<", " <<FYRINGSTERSKEL <<", depol = " <<dDepolAtStartOfTimeWindow <<"\n\n" ;
		//} 
		#endif
		
			
		/**************************************
		* Legg inn auron i pEstimatedTaskTime *
		**************************************/
	
		/*
		* 	- Siden auronet alltid ligger der, skal det bare flyttes.
		* 		( Ved fyring av auronet flyttes det bare til 	[no] + [periode] i pEstimatedTaskTime )
		*		Dette skjer i K_auron.doTask()
		*/
	
		if( (lEstimatedTaskTime_for_object - time_class::getTid() ) > 0 )
		 	time_class::moveTask_in_pEstimatedTaskTime( this, lEstimatedTaskTime_for_object-time_class::getTid() );
		else
			// XXX XXX Ikkje heilt sikker på om det er greitt å flytte denne til element 0. Men eg trur det.. XXX
			time_class::moveTask_in_pEstimatedTaskTime( this, 0);
	} // slutt: if(kappa>tau)
	
	
	// Skriver til log for depol:
	writeDepolToLog();
} //}

// Rekalkulerer feil i Kappa for auronet.
double K_auron::recalculateKappa()
{
	// Plan:
	//  - Rekalkulerer kappa for dendrite.
	// 		- skal hente ut K_ij fra alle innsynapsene. Dette kan den gjøre ved å kalle K_synapse::getWij();
	double dKappa_temp = pInputDendrite->recalculateKappa();
	double dKappaFeil_temp = dAktivitetsVariabel-dKappa_temp;

	dAktivitetsVariabel = dKappa_temp;

	return dKappaFeil_temp;
	
}

void recalcKappaObj::doTask()
{
	// Rekalkuler Kappa.
	// Trenger en funksjon K_auron::recalculateKappa() som
	// 	- rekalkulerer kappa for auronet.
	// 	- returnerer rekalkulert kappa.
	
	pKappaAuron_obj->recalculateKappa();
	// Sjå kva feilen er, og la feilen bestemme kor lenge vi skal vente til neste rekalk. Kanskje også ha en slags FIR-effekt her?
}



// Gjøre denne til mdl.funk. av K_auron?
void loggeFunk_K_auron()
{
		// DEBUG: Skriver depol til log for alle K_auron (ved å kappe changeKappa(0) - denne beregner v og skriver til depol-loggfil).
		for( std::list<K_auron*>::iterator iter = K_auron::pAllKappaAurons.begin(); iter != K_auron::pAllKappaAurons.end(); iter++ )
		{
			(*iter) ->writeDepolToLog();
		}
}



// vim:fdm=marker:fmr=//{,//}
