
#include <iostream>
#include <string>
//#include <vector>
#include <list>

#include <stdlib.h> 	//- for rand()
//include <cmath> 		//- for pow() funksjoner
//include <algorithm> 	- for_each()
//include <fstream> 	- file streams



//Defines:
#define DEFAULT_ANTALL_TIDSITERASJONER 10

#define alpha 0.96
#define LEKKASJEFAKTOR_FOR_DEPOL alpha





#ifndef MAIN_H_
#define MAIN_H_

#include "../auron/auron.h"
#include "tid.h"
#include "aktivitetsObj.h"

#ifndef SYNAPSE_H_
#include "../synapse/synapse.h"
#endif

//using std::list;
using std::cout;
using std::endl;
//using std::vector;





// deklarasjoner:







/*
	simulert, diskret TID
*/
//unsigned long ulTidsiterasjoner; // ligg i main.h
// flytta inn som static tid::ulTidsiterasjoner;







#endif
