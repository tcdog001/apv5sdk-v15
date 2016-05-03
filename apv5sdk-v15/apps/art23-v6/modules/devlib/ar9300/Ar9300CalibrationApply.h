


//
// Apply the recorded correction values.
//
//extern int Ar9300CalibrationApply(int frequency);


//
// Stuff calibration number into chip
//
extern int nartAr9300PowerControlOverride(int frequency, int *correction, int *voltage, int *temperature);

