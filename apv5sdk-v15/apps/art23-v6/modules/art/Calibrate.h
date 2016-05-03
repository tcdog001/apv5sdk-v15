

//
// save any information required to support calibration
//
extern int CalibrateRecord(int client, int frequency, int txchain, int txgain, double power);


//
// Save the calibration data in the internal data structure 
//
extern int CalibrateSave();



//
// Clear all of the saved calibration data
//
extern int CalibrateClear();

extern void CalibrateStatsHeader(int client);
