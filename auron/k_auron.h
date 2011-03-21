


For spiking auron : s_auron 
så er auron::doTask() kobla til lekkasje f0r auronet. For kappa-auron kan denne være kobla til å regne ut ny estimert fyringstid (fra nye kappa).

Dette gjøres i tidsSkilleElement::doTask():
	- går gjennom alle nodene som har oppdatert kappa denne iterasjonen og estimerer ny fyringstid.  (skjer i auron::doTask() )
	(dette impliserer at når eit neuron endrer kappa, skal den legge inn i static std::set<auron*> lAlleNodeneSomSkal_doTask; (std::set er eit set med unike element, der elementet sjølv er `key')
	- kanskje denne skal "fyre" k_aurona også? Nei, auron som skal fyre er ikkje nødvendigvis satt opp i lista over.. Finn på noke anna.
