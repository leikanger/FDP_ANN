#include "synapse.h"


auron::auron(std::string sNavn_Arg ="unnamed") : tidInterface("auron"),ao_AuronetsAktivitet(this) ,sNavn(sNavn_Arg)
{
	cout<<"Lager auron med navn " <<sNavn <<endl;


	pOutputAxon = new axon(this); 						//  Husk destructor. Husk å også destruere dette axon (fra det frie lageret).
 	pInputDendrite = new dendrite(this); 				//  Husk destructor. Husk å også destruere dette axon (fra det frie lageret). 

 	// Må kanskje flytte ned her. XXX Dersom eg får rar utskrift på kjøringa av kjør.sh: 	ao_AuronetsAktivitet(this)
}

auron::~auron()
{
	cout<<"DESTRUCTOR: auron::~auron() : \t" <<sNavn <<"\n";

	// Legge inn sjekk for om den er sletta allerede? XXX :
	delete pOutputAxon;
	delete pInputDendrite;

	cout<<"FERDIG destruert: auron " <<sNavn <<"\n\n";
}




synapse::synapse(auron* pPresynAuron_arg, auron* pPostsynAuron_arg, bool bInhibEffekt_Arg /*=false*/, float fSynVekt_Arg /*=1*/) 
			: 	pPreNodeAxon(pPresynAuron_arg->pOutputAxon), pPostNodeDendrite(pPostsynAuron_arg->pInputDendrite), bInhibitorisk_effekt(bInhibEffekt_Arg), fSynaptiskVekt(fSynVekt_Arg)
{	
	cout<<"Kaller synapse::synapse(" <<pPreNodeAxon->pElementAvAuron->sNavn <<".pOutputAxon, " <<pPostNodeDendrite->pElementAvAuron->sNavn <<".pInputDendrite, ...)\n";

	pPreNodeAxon->pUtSynapser.push_back(this);
	pPostNodeDendrite->pInnSynapser.push_back(this);
	

	cout 	<<"\tCONSTRUCTOR : synapse::synapse(a*, a*) \tEtterpå får vi:\n" 
			<<(pPreNodeAxon) <<endl;

}

//{ ~synapse } --DESTRUCTOR  	TODO TEST: denne er utesta..
synapse::~synapse()
{ // { ... }
	bool bPreOk  = false;
	bool bPostOk = false;


	cout<<"DESTRUCTOR: synapse::~synapse() : \t";


	//TODO Sjekk ut std::find() istedenfor. Stroustrup anbefaler å bruke dette!.
	if( !bPreOk ){ 	//Redundant test. Kanskje eg skal skrive while(!bPreOk)?
		//fjærner seg sjølv fra prenode:
		for( std::list<synapse*>::iterator iter = pPreNodeAxon->pUtSynapser.begin(); iter != pPreNodeAxon->pUtSynapser.end() ; iter++ ){
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
		for( std::list<synapse*>::iterator iter = pPostNodeDendrite->pInnSynapser.begin(); iter != pPostNodeDendrite->pInnSynapser.end() ; iter++ ){
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
		std::cerr<<"\n\n\n\n\nFEIL FEIL FEIL!\nSjekk neuroEnhet.cpp #asdf250. Feil i synapse destruksjon. (~synapse )\n";
		std::cerr<<"I test om eg vart sletta  fra presyn. neuron og postsyn. neuron: failed\t\t" 
			<<"bPreOk (" <<pPreNodeAxon->pElementAvAuron->sNavn <<"):" <<bPreOk <<"  ->  bPostOk (" <<pPostNodeDendrite->pElementAvAuron->sNavn <<"): " <<bPostOk 
			<<"\n(Eg er ikkje sletta fra det aktuelle neuronet (dei med verdi 0)\n";
		std::cerr<<"Eg ligger mellom (neuron: presyn - postsyn): " <<pPreNodeAxon->pElementAvAuron->sNavn <<" - " <<pPostNodeDendrite->pElementAvAuron->sNavn <<endl;
		exit(-9);	
	}
	//cout<<"\tO.K.  -  syn. " <<pPreNodeAxon->pElementAvAuron->sNavn <<" til " <<pPostNodeDendrite->pElementAvAuron->sNavn <<" er ikkje lenger\n";
	cout<<endl;

}
	




	
axon::axon(const auron* pAuronArg) : tidInterface("axon"), pElementAvAuron(pAuronArg){ // tanke er at axon må tilhøre eit auron. Difor auronpeiker.
	cout<<"\tlager axon\n";//for \tauron " <<pAuronArg->sNavn <<endl;		
}
axon::~axon()
{
	cout<<"axon::~axon() : 	\t\t(tilhører auron " <<pElementAvAuron->sNavn <<")\n";
	//delete pElementAvAuron->pOutputAxon; // XXX LAGER HELVETE ? JEPP

	cout<<"SKRIV først ut axonet: " <<this <<"\n";

	// pUtSynapser inneholder bare peikere, så pUtSynapser.clear() vil ikkje føre til destruksjon av synapsene.
	// 		Sletter synapsene eksplisitt med delete-operatoren på alle element som list<synapse*> pUtSynapser) peiker på.
	while( ! pUtSynapser.empty() ){
		delete (*pUtSynapser.begin()); //Kaller destruktoren til første gjenværende synapse. Dette fører også til at synapsa fjærnes fra pUtSynapser (og dendrite.pInnSynapser)
	}

}

dendrite::dendrite( auron* pPostSynAuron_Arg ) : pElementAvAuron(pPostSynAuron_Arg)
{
	cout<<"\tLager dendrite\n";// for \tauron " <<pElementAvAuron->sNavn <<endl;
}

