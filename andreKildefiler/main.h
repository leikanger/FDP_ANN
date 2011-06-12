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
#include <iostream>
#include <string>
//#include <vector>
#include <list>
#include <map>
#include <vector>
//#include <set>


#include <stdlib.h> 	//- for rand()
#include <cmath> 		//- for pow() funksjoner
//include <algorithm> 	- for_each()
//include <fstream> 	- file streams



//Defines:

// Velger KANN eller SANN for neste compilasjon
#define KANN 1
#define SANN 1

#define DEFAULT_ANTALL_TIDSITERASJONER 1000
//#define MIN_PERIODE_MELLOM_REKALKULERING_AV_KAPPA 100

#define RECALC_c1 100 
#define RECALC_c2 250
#define RECALC_c3 10
#define RECALC_c4 0.5

#define UTSKRIFT_AV_TID 1
#define UTSKRIFT_AV_TID_KVAR_Nte_ITER 1000

#define DEBUG_UTSKRIFTS_NIVAA 2
#define DOT_ENTER_UTSKRIFT_AV_TID 10 // Betyr enter kvar n'te iter. (der n er tall som blir def.). Definer til 0 eller false for å slå av.

#define ALPHA (double)0.004000000
#define LEKKASJEFAKTOR_FOR_DEPOL (double)(1-ALPHA)


#define FYRINGSTERSKEL 1000.00000

#define OCTAVE_SLEEP_ETTER_PLOTTA 0

#define DEBUG_EXIT(tekst) cerr<<"\n\nDEBUG_EXIT :\t" <<tekst <<"\nexit(99);\n\n"; exit(99);
//#define DEBUG(tekst) cerr<<"DEBUG :\t" <<tekst <<"\n";
#define DEBUG(tekst) ;

#ifndef MAIN_H_
#define MAIN_H_

#include "../neuroElements/auron.h"
#include "time.h"

#ifndef SYNAPSE_H_
#include "../neuroElements/synapse.h"
#endif
/*
#ifndef DENDRITE_H_
#include "../neuroElements/dendrite.h"
#endif*/

using std::cerr;
using std::cout;
using std::endl;





// deklarasjoner:

class comparisonClass{
	public:
	static unsigned long ulNumberOfCallsToKappa_doCalculations;
	static unsigned long ulNumberOfCallsTo_doTask;
};






/*
	simulert, diskret TID
*/
//unsigned long ulTidsiterasjoner; // ligg i main.h
// flytta inn som static time_class::ulTidsiterasjoner;







#endif
// vim:fdm=marker:fmr=//{,//}
