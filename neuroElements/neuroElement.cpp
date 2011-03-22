#include "synapse.h"


auron::auron(std::string sNavn_Arg ="unnamed") : tidInterface("auron"), ao_AuronetsAktivitet(this), sNavn(sNavn_Arg) {
	cout<<"Lager auron med navn " <<sNavn <<endl;

	pAxon_output = new axon(this); 						// TODO XXX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). XXX TODO
 	pDendrite_input = new dendrite(this); 				// TODO XXX Husk destructor. Husk å også destruere dette axon (fra det frie lageret). XXX TODO
}
	


//synapse::synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, bool bInhibEffekt_Arg =false, float fSynVekt_Arg =1) 
synapse::synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, bool bInhibEffekt_Arg /*=false*/, float fSynVekt_Arg /*=1*/) 
			: 	pPreNode(pPresynAuron_arg), pPostNode(pPostsynAuron_arg), bInhibitorisk_effekt(bInhibEffekt_Arg), fSynaptiskVekt(fSynVekt_Arg)
{	
	cout<<"Kaller synapse::synapse(&" <<pPreNode->sNavn <<", &" <<pPostNode->sNavn <<")\n";

	pPreNode->pAxon_output ->pUtSynapser.push_back(this);
	pPostNode->pDendrite_input ->pInnSynapser.push_back(this);
	

	cout 	<<"\tCONSTRUCTOR : synapse::synapse(a*, a*) \tEtterpå får vi:\n" 
			<<*(pPreNode->pAxon_output) <<endl;

	
	//TODO lag utsynapser. std::list<synapse*> pUtSynapser    (A.pAxon_output)->ny-synapse(&B) {  pUtSynapser.push_back(new synapse( this->pElementAvAuron , &B) }
	// 		OG HUSK DESTRUKTOR!
}
/*
// constructor:
//{ Kommentert ut
synapse::synapse(auron* pPresynAuron, auron* pPostsynAuron, bool inhibitoriskEffekt =false, float vekt =1) :
		bInhibitorisk_effekt(argInhibitorisk_effekt),
	 	fSynaptiskVekt( vekt ),
		pPreNode(pPreN_arg), pPostNode(pPostN_arg)
{	//{
	pPreNode->pUtSynapser.push_back(  this );
	pPostNode->pInnSynapser.push_back(this );
	

	/ * //{ Utskrift til fil for å lage plot i etterkant. (Kommentert ut foreløpig)
	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/synapse_" <<pPreNode->navn <<"-"  <<pPostNode->navn ;
	if(bInhibitorisk_effekt){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );
	// trenger c-style string for open():
	utskriftsFilLogg.open( tempStr.c_str() );
	
	utskriftsFilLogg<<"data=[\n";
	utskriftsFilLogg.flush();
	* / //}
	
}  	//}
//}


Fra gammelt neuronett:
// constructor:
synapse::synapse( neuron* pPreN_arg, neuron* pPostN_arg, bool argInhibitorisk_effekt / *=false* /, float v / *=1* / ) :  // v er oppgitt i promille.
		bInhibitorisk_effekt(argInhibitorisk_effekt),
		ulTimestampForrigeOppdatering( ulTidsiterasjoner ), 			 	??? Ha med?
		ulTimestampForrigeSignal 	(ulTidsiterasjoner ), 					??? Ha med?
		pPreNode(pPreN_arg), pPostNode(pPostN_arg)
{
	pPreNode->pUtSynapser.push_back(  this );
	pPostNode->pInnSynapser.push_back(this );
	

//{	// lag ei .oct - fil, og gjør klar for å kjøres i octave:
	std::ostringstream tempFilAdr;
	tempFilAdr<<"./datafiler_for_utskrift/synapse_" <<pPreNode->navn <<"-"  <<pPostNode->navn ;
	if(bInhibitorisk_effekt){ tempFilAdr<<"_inhi"; }
	else{ 			  tempFilAdr<<"_eksi"; }
	tempFilAdr<<".oct";

	std::string tempStr( tempFilAdr.str() );

	// trenger c-style string for open():
	utskriftsFilLogg.open( tempStr.c_str() );
	
	utskriftsFilLogg<<"data=[\n";
	
	utskriftsFilLogg.flush();

	// Logg for størrelsen av PSP-endring:
	std::ostringstream tempFilAdr2;
	tempFilAdr2<<"./datafiler_for_utskrift/effekt_av_PSP_for_syn_" <<pPreNode->navn <<"-"  <<pPostNode->navn ;
	tempFilAdr2<<".oct";

	tempStr = tempFilAdr2.str() ;

	// trenger c-style string for open():
	PSP_loggFil.open( tempStr.c_str() );
	
	PSP_loggFil<<"data=[\n";
	PSP_loggFil.flush();
//}
}


*************************** OG destructor ************************************************
//{ ~synapse } --DESTRUCTOR  
synapse::~synapse()
{ //{ ... }
	bool bPreOk  = false;
	bool bPostOk = false;


	// Sjekker om der er utsynapser i prenode:
	if( ! (pPreNode->pUtSynapser).empty() ){
		cout<<"\tså skriver eg ut alle utsynapsene fra prenode:\n";
		for( std::vector<synapse*>::iterator iter = pPreNode->pUtSynapser.begin(); iter != pPreNode->pUtSynapser.end() ; iter++ ){
			cout<<"\t\tUtsynapse fra " <<(*iter)->pPreNode->navn <<" til " <<(*iter)->pPostNode->navn <<"\t->\n";
		}
	}else{ 
		cout<<"\tprenode  er Tom for utsynapser.\n"; 
		//bPreOk = true;
	}
	// Sjekker om det er innsynapser i postnode:
	if( ! pPostNode->pInnSynapser.empty() ){
		cout<<"\tså skriver eg ut alle innsynapsene til postnode:\n";
		 
		for( std::vector<synapse*>::iterator iter = pPostNode->pInnSynapser.begin(); iter != pPostNode->pInnSynapser.end() ; iter++ ){
			cout<<"\t\tInnynapse fra " <<(*iter)->pPreNode->navn <<" til " <<(*iter)->pPostNode->navn <<"\t<-\n";
		}
	}else{ 
		cout<<"\tpostnode er Tom for innsynapser.\n";
		//bPostOk = true;
	}


	if( !bPreOk ){
		//fjærner seg sjølv fra prenode:
		for( std::vector<synapse*>::iterator iter = pPreNode->pUtSynapser.begin(); iter != pPreNode->pUtSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t~synapse: Fjærner ut-syn. fra prenode: [" <<pPreNode->navn <<"] "; //Merk: ingen endl;
				(pPreNode->pUtSynapser).erase( iter );
				bPreOk = true;
				break;
			}
		}
	}
	if( !bPostOk ){
		//fjærner seg sjølv fra postnode:
		for( std::vector<synapse*>::iterator iter = pPostNode->pInnSynapser.begin(); iter != pPostNode->pInnSynapser.end() ; iter++ ){
			if( *iter == this ){
				cout<<"\t og videre inn-syn. til postnode [" <<pPostNode->navn <<"]\n";
				(pPostNode->pInnSynapser).erase( iter );
				bPostOk = true;
				break;
			}
		}
	}

	if( (!bPreOk) || (!bPostOk) ){
		/// FEIL:
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i synapse destruksjon. (~synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNode->navn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNode->navn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNode->navn <<" - " <<pPostNode->navn <<endl;
		exit(-9);	
	}
	cout<<"\tO.K.  -  syn. " <<pPreNode->navn <<" til " <<pPostNode->navn <<" er ikkje lenger\n";




	//{ utskriftslogger
	
	if( ulTimestampForrigeOppdatering < 10 ){
	 	utskriftsFilLogg <<"0 0 0 0"; // for å unngå feilmelding om tom vektor, fra octave når det skal plottes..
	}

	// no er data slik: [tid, antall, 		tomme, membran ] i synapse_likevekt..
	utskriftsFilLogg<<"];\n"
				<<"plot( data([1:end-1],1), data([1:end-1],2), \";Frie  S.V.;\",\n\t"
	     				<<"data([1:end-1],1), data([1:end-1],3), \";tomme S.V.;\",\n\t" 
					<<"data([1:end-1],1), data([1:end-1],4), \";mem.  S.V.;\""
			//	<<",\n\tdata([1:end-1],1), data([1:end-1],5), \";raavarer;\"\n\t" //XXX For likevekts_synapse utskrift.
					<<");\n"
			<<"title \"Synaptic vesicles i presyn. terminal i synapse: " <<pPreNode->navn <<" -> " <<pPostNode->navn <<"\"\n"
			<<"xlabel Tid\n" <<"ylabel Antall\n"
			<<"akser=[0 data(end,1) 0 1400 ]; axis(akser);\n"
			<<"print(\'eps_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".eps\', \'-deps\');"
			//<<"print(\'pdf_" <<pPreNode->navn <<"->" <<pPostNode->navn <<".pdf\', \'-dpdf\');"
			<<" sleep(9); ";
	utskriftsFilLogg.close();
	
	// Og PSP_loggFil:
	PSP_loggFil <<"\t]; \nplot(data);\n"
			<<"title \"Effekt ved overfoering i forskjellige tidspunkt i synapse: " <<pPreNode->navn <<" -> " <<pPostNode->navn <<"\"\n"
			<<"xlabel Tid\n" <<"ylabel Potensiell_effekt\n"
			<<"sleep(10);";
	PSP_loggFil.close();
	//}
} //}
*/ //}

	
	
axon::axon(const auron* pAuronArg) : tidInterface("axon"), pElementAvAuron(pAuronArg){ // tanke er at axon må tilhøre eit auron. Difor auronpeiker.
	cout<<"\tlager axon fra auron " <<pAuronArg->sNavn <<endl;		
}

dendrite::dendrite( auron* pPostSynAuron_Arg ) : pElementAvAuron(pPostSynAuron_Arg)
{
	cout<<"\tLager dendrite til auron med navn " <<pElementAvAuron->sNavn <<endl;
}

