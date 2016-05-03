

#include <stdio.h>
#include <stdlib.h>

#include "Field.h"
#include "ParameterSelect.h"
#include "Card.h"

#include "wlantype.h"

#ifndef MDK_AP
#include "osdep.h"
#endif

#include "mEepStruct9300.h"
#include "Ar9300CalibrationApply.h"


//
// hal header files
//
#include "ah.h"
#include "ah_internal.h"
#include "ar9300eep.h"


#define MCHAIN 3



//
// Stuff calibration number into chip
//
int nartAr9300PowerControlOverride(int frequency, int *correction, int *voltage, int *temperature)
{
	ar9300PowerControlOverride(AH, frequency, correction, voltage, temperature);
    return 0;
}



