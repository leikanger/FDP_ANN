
#include <sstream> //For skriving til fil: logg.

#include "synapse.h"
#include "auron.h"
#include "../andreKildefiler/main.h"




//Bedre oppdeling: (uferdig (ustarta)
//{1 		* 	interface-class

//{2  ** auron
//}
//{2  ** axon
//}
//{2  ** synapse
//}
//{2  ** dendrite
//}


//}1
//{1 		* 	SANN

//{2  ** auron
//}
//{2  ** axon
//}
//{2  ** synapse
//}
//{2  ** dendrite
//}


//}1
//{1 		* 	KANN

//{2  ** auron
//}
//{2  ** axon
//}
//{2  ** synapse
//}
//{2  ** dendrite
//}


//}1



/*************************************************************
****** 													******
******  		CONTRUCTORS OG DESCTRUCTORS 			******
****** 													******
*************************************************************/

//{1 * AURON
//{2 *** i_auron   
i_auron::i_auron(std::string sNavn_Arg /*="unnamed"*/, int nStartAktVar /*=0*/) : timeInterface("auron"), nAktivitetsVariabel(nStartAktVar), sNavn(sNavn_Arg)
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
	activityVar_logFile.open( tempStr.c_str() );
	activityVar_logFile<<"data=[" <<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<";\n";
	activityVar_logFile.flush();

	//}4
}  //}3
i_auron::~i_auron()
{ //{3
	cout<<"\tDESTRUCTOR: \ti_auron::~i_auron() : \t" <<sNavn <<"\t * * * * * * * * * * * * * * * * * * * * * * * * * \n";
	
	//fjærner seg fra pAllAurons
	pAllAurons.remove(this);

	//{4 Rett slutt på utskriftsfil-logg:
	// no er data slik: [time, synWeight ] i synapse-logg
	activityVar_logFile<<time_class::getTid() <<"\t" <<nAktivitetsVariabel <<"];\n"
					<<"plot( data([1:end],1), data([1:end],2), \"@;Activity variable;\");\n"

					<<"title \"Activity variable for auron " <<sNavn <<"\"\n"
					<<"xlabel Time\n" <<"ylabel \"Activity variable\"\n"
					//<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
					<<"print(\'eps_auron" <<sNavn <<".eps\', \'-deps\');\n"
					<<"sleep(9); ";
	activityVar_logFile.close();
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
 	pInputDendrite = new s_dendrite(this); //	Skal ligge i s_auron og k_auron

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
K_auron::K_auron(std::string sNavn_Arg /*="unnamed"*/, int nStartKappa /*= FYRINGSTERSKEL*/, unsigned uStartDepol_prosent /*=0*/) : i_auron(sNavn_Arg, nStartKappa)
{
	ulTimestampForrigeFyring = time_class::getTid();
	//ulTimestampForrigeInput  = time_class::getTid();

	ulStartOfTimewindow = time_class::getTid();
	nDepolAtStartOfTimeWindow = uStartDepol_prosent * FYRINGSTERSKEL;


	// Sjå auron.h
	bEndraKappaDennePerioden = false;
	
	// Legger til auron* i std::list<i_auron*> pAllAurons;
	pAllAurons.push_back( this );

	

	//pOutputAxon og pInputDendrite
	pOutputAxon = new K_axon(this);
 	pInputDendrite = new K_dendrite(this); //	Skal ligge i s_auron og k_auron

	// Kjører auronets doCalculation() for å få rett verdier:
	doCalculation();
}
K_auron::~K_auron()
{
	
}
//}2
//}1 * AURON

//{1 * SYNAPSE
//{2 i_synapse
i_synapse::i_synapse(i_axon* pPresynAxon_arg, i_dendrite* pPostsynDendrite_arg, unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn /*="synapse"*/ ) : timeInterface(sKlasseNavn), bInhibitorisk_effekt(bInhibEffekt_Arg)
//i_synapse::i_synapse(unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, std::string sKlasseNavn ="synapse") : timeInterface(sKlasseNavn), bInhibitorisk_effekt(bInhibEffekt_Arg)
{ //{3
	uSynaptiskVekt = uSynVekt_Arg;
	pPreNodeAxon = pPresynAxon_arg;
	pPostNodeDendrite = pPostsynDendrite_arg;

	// TODO Kva skal stå her? Har tilegna klassenavn, bInhibitorisk_effekt allerede..
	
	cout<<"\t constructor for i_synapse(unsigned uSynVekt_Arg, bool bInhibEffekt_Arg, string navn);\n";
	
} //}3
//}2
//{2 s_synapse
s_synapse::s_synapse(s_auron* pPresynAuron_arg, s_auron* pPostsynAuron_arg, unsigned uSynVekt_Arg /*=1*/, bool bInhibEffekt_Arg /*=false*/) 
			:  i_synapse(pPresynAuron_arg->pOutputAxon, pPostsynAuron_arg->pInputDendrite , uSynVekt_Arg, bInhibEffekt_Arg, "s_synapse") 
{//{3	

	cout<<"Kaller s_synapse::s_synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

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
		for( std::list<i_synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
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
		for( std::list<i_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
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
 :  i_synapse(pPresynAuron_arg->pOutputAxon, pPostsynAuron_arg->pInputDendrite , uSynVekt_Arg, bInhibEffekt_Arg, "s_synapse") 
{ 	//XXX HER: nytt kappa element:
	cout<<"Kaller K_synapse::K_synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	

	cout 	<<"\tCONSTRUCTOR : K_synapse::K_synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;


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
		for( std::list<i_synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
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
		for( std::list<i_synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
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
i_axon::i_axon( i_auron* pAuronArg, std::string sKlasseNavn ="dendrite") : timeInterface(sKlasseNavn), pElementAvAuron(pAuronArg)
{ //{3
} //}3
i_axon::~i_axon()
{ 
	// pUtSynapser inneholder bare peikere, så pUtSynapser.clear() vil ikkje føre til destruksjon av synapsene.
	// 		Sletter synapsene eksplisitt med delete-operatoren på alle element som list<synapse*> pUtSynapser) peiker på.
	while( !pUtSynapser.empty() ){
		delete (*pUtSynapser.begin()); //Kaller destruktoren til første gjenværende synapse. Dette fører også til at synapsa fjærnes fra pUtSynapser (og dendrite.pInnSynapser)
	}
	//TO DO   TO DO    TO DO Denne skal ligge i i_axon når pUtSynapser er endra fra <s_synapse*> til <i_synapse*>. TODO TODO  TODO

}
//}2 *** i_axon
//{2 ***  s_axon
s_axon::s_axon(s_auron* pAuronArg) : i_axon(pAuronArg, "s_axon")
{ //{3 
	cout<<"\tlager axon\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
} //}3
s_axon::~s_axon()
{ //{3
	cout<<"\tDESTRUCTOR:  s_axon::~s_axon() for auron \t" <<pElementAvAuron->sNavn <<"\n";

} //}3
//}2
//{2 ***  i_axon
K_axon::K_axon( K_auron* pAuronArg) : i_axon(pAuronArg, "K_axon" )
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
i_dendrite::i_dendrite(i_auron* pElementAvAuron_arg, std::string sNavn ="dendrite") : timeInterface(sNavn)
{ //{3 
	pElementAvAuron = pElementAvAuron_arg;
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
s_dendrite::s_dendrite( s_auron* pPostSynAuron_Arg ) : i_dendrite(pPostSynAuron_Arg, "s_dendrite")//, pElementAvAuron(pPostSynAuron_Arg)
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
K_dendrite::K_dendrite( K_auron* pPostSynAuron_Arg ) : i_dendrite(pPostSynAuron_Arg, "K_dendrite" )
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



/******************* SYNAPSE ********************/
//{1
// TEST:XXX:inline unsigned& K_synapse::regnutPresynPeriode() XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
// VEKK VEKK VEKK :
inline unsigned K_synapse::regnutPresynPeriode()
{
	int kappa_pre = pPreNodeAxon->pElementAvAuron->nAktivitetsVariabel;
	forrigeUtregnaPresynPeriode = -1/ALPHA * log( (FYRINGSTERSKEL - kappa_pre ) / kappa_pre );
	cout<<forrigeUtregnaPresynPeriode <<endl;
	return forrigeUtregnaPresynPeriode;
}

//}1

/******************* DENDRITE *******************/
//{1

// TPDP TODO TODO FJÆRN feedbackToDendrite()  (fra i_dendrite..)
// TODO Ta bort feedbackToDendrite(). Har bestemt meg for å heller caste i_auron til K_auron osv. Internt i noden (dendrite, auron, axon)
inline void s_dendrite::feedbackToDendrite() // var: axonTilbakemelding()
{ //{
	 //Tilbakemelding fra axonet forteller oss at refraction time er over.

	 //Skriver til logg etter refraction-period.
	 pElementAvAuron->skrivAktivitetsVarLogg();

	 //Avsetter bBlockInput_refractionTime - flagg:
	 bBlockInput_refractionTime = false;
} //}
inline void K_dendrite::feedbackToDendrite()
{
	pElementAvAuron->skrivAktivitetsVarLogg();
}
inline void s_dendrite::newInputSignal( int nNewSignal_arg )
{ //{

	// Dersom input er blokkert er enten noden i 'refraction period' eller så fyrer det allerede for denne iterasjonen. (trenger ikkje kalkulere meir på depol..)
 	if( bBlockInput_refractionTime ) return;
	else{ cout<<"forrige fyring på tid: " <<pElementAvAuron->ulTimestampForrigeFyring <<endl; }

	// beregner lekkasje av depol siden sist:
	calculateLeakage();

	pElementAvAuron->nAktivitetsVariabel += nNewSignal_arg;
	cout<<"s_dendrite::newInputSignal( " <<nNewSignal_arg <<" ); gir depol. :  " <<pElementAvAuron->nAktivitetsVariabel <<"\n";
	
	pElementAvAuron->ulTimestampForrigeInput = time_class::getTid();


	// Dersom auron går over fyringsterskel: fyr A.P.
	if( pElementAvAuron->nAktivitetsVariabel > FYRINGSTERSKEL )
	{
		// Blokkerer videre input (etter at vi allerede har nådd terskel)
		bBlockInput_refractionTime = true;

		//cout<<"Postsyn. kom over FYRINGSTERSKEL.\nLegger til i arbeidskø.\n\n";
		
	 	
		//Ekstra time-delay:
		time_class::leggTilTask( this );
		// for ekstra time delay. Evt kan vi kjøre auron som neste ledd:
		// Legger til neste ledd i signal-path (soma == auron).
	 	//time_class::leggTilTask( pElementAvAuron );

	}

	// Skriver til log for aktivitetsVar:
	pElementAvAuron->activityVar_logFile 	<<time_class::getTid() <<"\t" <<pElementAvAuron->nAktivitetsVariabel <<"; \t #Depolarization\n" ;
	pElementAvAuron->activityVar_logFile.flush();
} //}
inline void K_dendrite::newInputSignal( int nNewSignal_arg )
{
	//HER XxX xXx casting (eksplisitt typekonvertering) fra i_auron til K_auron for å kunne accessere K_auron::bEndraKappaDennePerioden.
	//static_cast<K_auron*>(pElementAvAuron)->newKappaDueToNew_inputLevel( nNewSignal_arg );
	cout<<"Bla,bla,bla.. K_dendrite::newInputSignal(int), jeah\n"; 
}
/** Bare for SANN:  ***/
inline void s_dendrite::calculateLeakage()
{ //{
	/*
	* 	Har testa verdiane i octave. Loggen følgte lå på kurva for [initDepol]*LEKKASJEFAKTOR_FOR_DEPOL^x, selv om eg bare sjekka calculateLeakage() for siste ledd 
	* 		(f.eks. etter 30 iterasjoner uten noko input anna enn init-verdien for depool. for auronet). Hurra!
	*/

	int slettDebugGammelDepolverdi = pElementAvAuron->nAktivitetsVariabel;

	if( pElementAvAuron->ulTimestampForrigeInput != time_class::getTid() )
	{
		// regner ut, og trekker fra lekkasje av depol til postsyn neuron.
		unsigned long sulTidSidenSist = time_class::getTid()-pElementAvAuron->ulTimestampForrigeInput;

		cout<<"XXX Tid siden sist: " <<sulTidSidenSist <<endl;

		// SKRIVER TIL nAktivitetsVariabel. XXX
	 	pElementAvAuron->nAktivitetsVariabel *= (double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist );
		
		cout 	<<"\n\n\t\t\t\t\tLEKKASJEfaktor: " <<(double)pow( LEKKASJEFAKTOR_FOR_DEPOL, sulTidSidenSist ) <<" [gammel => ny depol.]: [" <<slettDebugGammelDepolverdi <<"=>" <<pElementAvAuron->nAktivitetsVariabel
				<<"]. (for auron " <<pElementAvAuron->sNavn <<")\n";
		// Bruker heller auron::ulTimestampForrigeInput..
		//ulTimestampForrigeOppdatering = time_class::getTid(); 
	}
	// Dersom den allerede har regna ut lekkasje: ikkje gjør det igjen. Returner.
} //}

//}1

/*********************** auron *************************************/
//{1

/*ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT ROT 
*/
void K_auron::newKappaDueToNew_inputLevel(int nChangeInKappaValue_arg )
{
	bEndraKappaDennePerioden = true;
	
	// add change to neurons kappa value: (the input signal is the change of kappa from one synapse).
	nAktivitetsVariabel += nChangeInKappaValue_arg; 

/*
	unsigned long ulEstimatedFiringTime_temp = estimateFiringTime();
*/
	
}
/*ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER |  ROT : TIL HER */ 

//}1



/*************************************************************
****** 													******
******  		doTask() -- samla på en plass. 			******
****** 													******
*************************************************************/

/****************************** auron *******************************************/ //{
inline void s_auron::doTask()
{ //{


	cout<<"\t" <<sNavn <<".doTask()\tFYRER Action Potential for neuron " <<sNavn <<".\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | \ttid: " <<time_class::getTid() <<"\n";

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output. // bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	time_class::leggTilTask( pOutputAxon );


	if( ulTimestampForrigeFyring == time_class::getTid() )
	{
		cout<<"\n\n************************\nFeil?\nTo fyringer på en iterasjon? \nFeilmelding au#103 @ auron.h\n************************\n\n";
		return;
	}

	// Registrerer fyringstid (for feisjekk (over) osv.) 
	ulTimestampForrigeFyring = time_class::getTid();


	//Resetter depol.verdi 
	nAktivitetsVariabel = 0; 
	//loggAktivitetsVar_i_AktivitetsVarLoggFil(); endra navn til:
	skrivAktivitetsVarLogg();

} //}
inline void K_auron::doTask()
{ 
	 
	cout<<"dT():\t" <<sNavn <<".doTask()\tFYRER Action Potential for neuron " <<sNavn <<".\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | \ttid: " <<time_class::getTid() <<"\n";

	// Beregn ny isi-periode^{-1}. Brukes til å beregne syn.overføring seinare i signal-cascade.
	unsigned uNyIPeriode_temp = log((double)nAktivitetsVariabel/((double)nAktivitetsVariabel-FYRINGSTERSKEL)) / ALPHA ;
	 	// Beregn endring i periode ivers, og lagre dette i nChangeInPeriodInverse for seinare bruk i synapsene.
	nChangeInPeriodInverse = uLastCalculatedPeriod_inverse-uNyIPeriode_temp;
	 	// Lagre ny kalkulert periode i uLastCalculatedPeriod_inverse:
	uLastCalculatedPeriod_inverse = uNyIPeriode_temp;

	// Legger seg selv til i pEstimatedTaskTime om uNyIPeriode_temp time iterations:
	time_class::addTaskIn_pEstimatedTaskTime( this, uNyIPeriode_temp );
	cout<<"K_auron: legger til [this] peiker til pEstimatedTaskTime om " <<uNyIPeriode_temp <<"\n\n";
	

	//Axon hillock: send aksjonspotensial 	-- innkapsling gir at a xon skal ta hånd om all output. // bestiller at a xon skal fyre NESTE tidsiterasjon. Simulerer tidsdelay i a xonet.
	time_class::leggTilTask( pOutputAxon );

	// Resetter bEndraKappaDennePerioden. Blir satt til true når auron får nytt input.
	bEndraKappaDennePerioden = false;
	
	skrivAktivitetsVarLogg();

}

//} *************************** slutt auron ***************************************
/****************************** synapse *****************************************/ //{


inline void s_synapse::doTask()
{ //{2 .. }

	// FØR  VAR DET :  inline void synapse::transmission()
	cout<<"Overføring i s_synapse.\n";

	// Dersom synapsen har inhibitorisk effekt: send inhibitorisk signal (subtraksjon). Ellers: eksiter postsyn. auron.
	// Dendrite lagrer tidspunk for overføring.
	if( bInhibitorisk_effekt ){
 		pPostNodeDendrite->newInputSignal(  -uSynaptiskVekt );
	}else{
		pPostNodeDendrite->newInputSignal(   uSynaptiskVekt ); //TODO gjør uSynaptiskVekt om til uSynaptiskVekt
	}
//( dendrite legges til i arbeidskø dersom den kommer over fyringsterskel.) --Dette er feilaktig, men meir effektivt.
		


	// XXX Kalkulere syn.p.?



	// TODO: synaptisk plastisitet: HER.
	//{ Loggfører syn.weight
	synWeight_loggFil 	<<"\t" <<time_class::getTid() <<"\t" <<uSynaptiskVekt
						<<" ;   \t#Synpaptic weight\n" ;
	synWeight_loggFil.flush();
			//} 

	// Logg for aktivitetsVar for postsyn auron skjer i pPostNodeDendrite->newInputSignal(-);

} //}2
inline void K_synapse::doTask()
{
	cout<<"dT():\tsynapse:\toverføring av ny kappa i K_synapse.\n";
/*
	// K_ij = w_ij / p(K_j)

	if(bInhibitorisk_effekt)
	{
		//pPostNodeDendrite->newInputSignal( -uSynaptiskVekt * pPreNodeAxon->pElementAvAuron->getPeriode() );
		pPostNodeDendrite->newInputSignal( -uSynaptiskVekt * regnutPresynPeriode() );

	}else{
		//pPostNodeDendrite->newInputSignal( uSynaptiskVekt * pPreNodeAxon->pElementAvAuron->nAktivitetsVariabel );
		pPostNodeDendrite->newInputSignal( uSynaptiskVekt * regnutPresynPeriode() );
	}
*/	 
}
//} *************************** SLUTT synapse *************************************
/****************************** axon ********************************************/ //{

inline void i_axon::doTask()
{ //{ // initierAksjonspotensial()
	cout<<"Legger inn alle outputsynapser i arbeidskø. Mdl. av auron: " <<pElementAvAuron->sNavn <<" - - - - - - - - - - - - - - - \n";

	// For meir nøyaktig simulering av tid kan alle synaper få verdi for 'time lag' før fyring. No fokuserer eg heller på effektivitet. 
 	for( std::list<i_synapse*>::iterator iter = pUtSynapser.begin(); iter != pUtSynapser.end(); iter++ )
	{ // Legger alle pUtSynapser inn i arbeidskø: (FIFO-kø)
		
		//time_class::pWorkTaskQue.push_back( (*iter) );
		// Legger til ut-synapser i time_class::arbeidskø
		time_class::leggTilTask( *iter );
	}

	//pElementAvAuron->loggAktivitetsVar_i_AktivitetsVarLoggFil(); ENDRA TIL:
	pElementAvAuron->skrivAktivitetsVarLogg();
} //}
inline void s_axon::doTask()
{ //{
	i_axon::doTask();

	// Avblokkerer dendritt. Opner den for meir input. Foreløpig er dette måten 'refraction time' funker på.. (etter 2 ms - dendrite og auron overføring..)
	pElementAvAuron->pInputDendrite->feedbackToDendrite();

} //}
inline void K_axon::doTask()
{
	// Legg heller til en i pEstimatedTaskTime, og kall i_axon::doTask() direkte fra K_auron.
	i_axon::doTask();

	// kanskje:
	// pElementAvAuron->pInputDendrite->feedbackToDendrite();
}
//} ************************** SLUTT axon *****************************************
/****************************** dendrite ****************************************/ //{
inline void s_dendrite::doTask()
{ //{ 
	// Kva skal dendrite::doTask() gjøre? 
	//cout<<pElementAvAuron->sNavn <<"->[dendrite]::doTask(). Postsyn. depol (" <<pElementAvAuron->sNavn <<") etter overføring: " <<pElementAvAuron->nAktivitetsVariabel <<".\n";
	time_class::leggTilTask( pElementAvAuron );
} //}
inline void K_dendrite::doTask()
{
	// TODO Trur ikkje denne skal være med lenger. Legger til brutal feilsjekk: exit(-1);
	cout<<"\n\nBrutal feilsjekk @ K_dendrite::doTask() : exit(-1);\n\n";

	exit(-1);
	time_class::leggTilTask( pElementAvAuron );
}
//} **************************** SLUTT dendrite ***********************************


/******************************************************************
****** 														*******
****** 			doCalculation() -- samla på en plass 		*******
****** 														*******
******************************************************************/

void K_auron::doCalculation()
{
	cout<<"dC():\tK_auron " <<sNavn <<".doCalculation()\t\t" <<sNavn <<".doCalculation()\n";

	// Skal estimere firingTime, og endre oppføringa i pEstimatedTaskTime.
	// Det har blitt ny kappa, så nytt estimat av fyringstid må beregnes:
	nDepolAtStartOfTimeWindow = nAktivitetsVariabel + (nDepolAtStartOfTimeWindow - nAktivitetsVariabel)*exp(-ALPHA*(time_class::getTid()-ulStartOfTimewindow));

 	unsigned long ulEstimatedTimeToAP =  (FAKTOR_FOR_AA_FAA_RETT_PERIODE / ALPHA) * log( (float)(nDepolAtStartOfTimeWindow - nAktivitetsVariabel) / (float)(FYRINGSTERSKEL - nAktivitetsVariabel) );
	//unsigned uNyIPeriode_temp = ALPHA / log((double)nAktivitetsVariabel/((double)nAktivitetsVariabel-FYRINGSTERSKEL));
	// TODO EN AV DESSE ER FEIL: Veit ikkje kva. Denne i doCalculation() eller den i K_auron::doTask()
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 
	// 
	// TRUR DET ER DENNE! FIKS DENNE! (eg sjekka i octave no: 4.3 er rett svar, ikkje 23
	//
	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO 

	//cout<<"K == " <<nAktivitetsVariabel <<", T == " <<FYRINGSTERSKEL <<", alpha == " <<ALPHA <<", V_0 == " <<nDepolAtStartOfTimeWindow <<endl;

	cout<<"ulEstimatedTimeToAP == " <<ulEstimatedTimeToAP <<endl;
	cout<<"[no-tid] + ulEstimatedTimeToAP == " <<ulEstimatedTimeToAP+time_class::getTid() <<endl;

	cout<<"LEGGER til task i pEstimatedTaskTime";
	time_class::addTaskIn_pEstimatedTaskTime( this, ulEstimatedTimeToAP);

}

