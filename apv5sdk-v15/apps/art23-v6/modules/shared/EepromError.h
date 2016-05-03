

enum
{
    EepromNoRoom=8000,
	EepromVerify,
	EepromAlgorithm,
	EepromTooMany,
	EepromWrite,

	EepromFatal,
	EepromWontFit,
	EepromNotEmpty,

	PcieVerify=8100,
	PcieTooMany,
	PcieWrite,
	PcieFatal,

	PcieWontFit,
};

#define EepromNoRoomFormat "Compressed block is too big."
#define EepromVerifyFormat "Calibration memory verify error at 0x%x: 0x%x != 0x%x."
#define EepromAlgorithmFormat "Writing calibration memory using algorithm=%d reference=%d size=%d address=0x%x."
#define EepromTooManyFormat "Too many calibration memory write errors."
#define EepromWriteFormat "Can't write calibration memory."
#define EepromFatalFormat "Fatal calibration memory error. Bad chip."
#define EepromWontFitFormat "Calibration data won't fit. Want 0x%x to 0x%x. Low limit is 0x%x."
#define EepromNotEmptyFormat "Memory not empty before calibration structure write. 0x%x: 0x%x."

#define EepromNoRoomFormatInput EepromNoRoomFormat
#define EepromVerifyFormatInput EepromVerifyFormat
#define EepromAlgorithmFormatInput EepromAlgorithmFormat
#define EepromTooManyFormatInput EepromTooManyFormat
#define EepromWriteFormatInput EepromWriteFormat
#define EepromFatalFormatInput EepromFatalFormat
#define EepromWontFitFormatInput EepromWontFitFormat
#define EepromNotEmptyFormatInput EepromNotEmptyFormat

#define PcieVerifyFormat "Chip initialization space verify error at 0x%x: 0x%x != 0x%x."
#define PcieTooManyFormat "Too many chip initialization space write errors."
#define PcieWriteFormat "Can't write chip initialization memory."
#define PcieFatalFormat "Fatal chip initialization memory error. Bad chip."
#define PcieWontFitFormat "Chip initialization data won't fit. Want 0x%x. Limit is 0x%x."

#define PcieNoRoomFormatInput PcieNoRoomFormat
#define PcieVerifyFormatInput PcieVerifyFormat
#define PcieTooManyFormatInput PcieTooManyFormat
#define PcieWriteFormatInput PcieWriteFormat
#define PcieFatalFormatInput PcieFatalFormat
#define PcieWontFitFormatInput PcieWontFitFormat

extern void EepromErrorInit(void);


