

#include <stdio.h>
#include <stdlib.h>


#include "wlantype.h"
#include "smatch.h"
#include "TimeMillisecond.h"
#include "CommandParse.h"
#include "NewArt.h"
#include "MyDelay.h"
#include "ParameterSelect.h"
#include "Card.h"
#include "Field.h"

#include "Device.h"

#include "Calibrate.h"

#include "ErrorPrint.h"
#include "NartError.h"


#define MBUFFER 1024

#define MMEASUREMENT 200

#define MCHAIN 3

struct _CalibrateMeasurement
{
    int frequency;
    int txchain;
    int txgain;
    double power;
    int correction;
    int p1;
    int p2;
    int temperature;
    int voltage;
};

extern int temperature_before_reading;

static struct _CalibrateMeasurement CalibrateMeasurement[MMEASUREMENT];

static int CalibrateMeasurementMany=0;

static int Round(double value)
{
	int ivalue;

	ivalue=(int)value;
	if(value-ivalue>=0.5)
	{
		ivalue++;
	}
	if(value-ivalue<=-0.5)
	{
		ivalue--;
	}
	return ivalue;
}

static int MeasurementFind(int frequency, int txchain)
{
    int it;

    for(it=0; it<CalibrateMeasurementMany; it++)
    {
        if(frequency==CalibrateMeasurement[it].frequency &&
            txchain==CalibrateMeasurement[it].txchain)
        {
            return it;
        }
    }
    return -1;
}


static double GainTableOffset=0.0;

void CalibrateStatsHeader(int client)
{
	ErrorPrint(NartDataHeader,"|cal|frequency|txchain|txgain||power|pcorr|voltage|temp|");
}

static void CalibrateStatsReturn(int client, int frequency, int txchain, int txgain, double power, int pcorr, int voltage, int temp)
{
    char buffer[MBUFFER];

    SformatOutput(buffer,MBUFFER-1,"|cal|%d|%d|%d||%.1lf|%d|%d|%d|",
        frequency,txchain,txgain,power,pcorr,voltage,temp);
    ErrorPrint(NartData,buffer);
}

//
// record any information required to support calibration
//
int CalibrateRecord(int client, int frequency, int txchain, int txgain, double power)
{
    int it;
    int correction;
    int temperature;
    int voltage;
    //
	// use temperature from after the reading as being most accurate
	//
    temperature=DeviceTemperatureGet(1);
    voltage=DeviceVoltageGet();
    it=MeasurementFind(frequency,txchain);
    if(it<0)
    {
        if(CalibrateMeasurementMany<MMEASUREMENT)
        {
            it=CalibrateMeasurementMany;
            CalibrateMeasurementMany++;
        }
    }
    if(it>=0)
    {
        CalibrateMeasurement[it].frequency=frequency;
        CalibrateMeasurement[it].txchain=txchain;
        CalibrateMeasurement[it].txgain=txgain;
        CalibrateMeasurement[it].power=power;
        correction=(int)Round(2.0*(power-GainTableOffset)-txgain+14.0);
        CalibrateMeasurement[it].correction=correction;
        CalibrateMeasurement[it].temperature = temperature;
        CalibrateMeasurement[it].voltage = voltage;
        CalibrateStatsReturn(client, frequency, txchain, txgain, power, correction, voltage, temperature);

        return 0;
    }

    return -1;
}


//
// Save the calibration data in the internal configuration data structure
//
int CalibrateSave()
{
    int ic,it;
    int chmask;
    int pier[2];
	int mode;
    //
    // first we update the calibration data in the internal memory structure
    //
    // do we need to sort it?
    // NEED STUFF FROM FIONA HERE
    for(ic=0; ic<MCHAIN; ic++)
    {
        chmask=(1<<ic);
        pier[0]=0;
		pier[1]=0;
        for(it=0; it<CalibrateMeasurementMany; it++)
        {
            if(CalibrateMeasurement[it].txchain==chmask)
            {
                // NEED TO SORT BY FREQUENCY
				mode=(CalibrateMeasurement[it].frequency>4000);
                DeviceCalibrationPierSet(pier[mode],CalibrateMeasurement[it].frequency,ic,CalibrateMeasurement[it].correction,CalibrateMeasurement[it].voltage,CalibrateMeasurement[it].temperature);
                pier[mode]++;
            }
        }
    }
   return 0;
}


//
// Clear all of the saved calibration data
//
int CalibrateClear()
{
    CalibrateMeasurementMany=0;

    return 0;
}


static int ChainIdentify(unsigned int chmask)
{
    int it;
    int good;

    good= -1;
    for(it=0; it<MCHAIN; it++)
    {
        if((chmask>>it)&0x1)
        {
            if(good>=0)
            {
                return -1;
            }
            good=it;
        }
    }
    return good;
}

