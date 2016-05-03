

#define MALLOWED 100


enum
{
	TemplatePreference=1000,
	TemplateAllowed,
	TemplateMemory,
	TemplateSize,
	TemplateCompress,
	TemplateOverwrite,
	TemplateInstall,
};

static int TemplateMemoryDefault=CalibrationDataEeprom;
static int TemplateMemoryDefaultRead=CalibrationDataNone;
static int TemplateMemoryMinimum=CalibrationDataNone;
static int TemplateMemoryMaximum=CalibrationDataOtp;

static struct _ParameterList TemplateMemoryParameter[]=
{
	{CalibrationDataDontLoad,{"none",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{CalibrationDataNone,{"automatic",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{CalibrationDataFlash,{"flash",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{CalibrationDataEeprom,{"eeprom",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{CalibrationDataOtp,{"otp",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int TemplateSizeDefault=1024;
static int TemplateSizeDefaultRead=0;
static int TemplateSizeMinimum=0;
static int TemplateSizeMaximum=8192;

static struct _ParameterList TemplateSizeParameter[]=
{
	{0,{"automatic",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1024,{"1K",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{2048,{"2K",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{4096,{"4K",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{8192,{"8K",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int TemplatePreferenceDefault=Ar9300EepromTemplateDefault;
static int TemplatePreferenceMinimum=Ar9300EepromTemplateDefault;

static struct _ParameterList TemplatePreferenceParameter[]=
{
	{Ar9300EepromTemplateGeneric,{"ar938x",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateGeneric,{"ar939x",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateHB112,{"hb112",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateHB116,{"hb116",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateXB112,{"xb112",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateXB113,{"xb113",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateXB114,{"xb114",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateTB417,{"tb417",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateAP111,{"ap111",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateAP121,{"ap121",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{Ar9300EepromTemplateHornetGeneric,{"ar9330",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int TemplateCompressionDefault=0;
static int TemplateOverwriteDefault=1;

static struct _ParameterList TemplateLogicalParameter[]=
{
	{0,{"no",0,0},0,0,0,0,0,0,0,0,0,0,0},
	{1,{"yes",0,0},0,0,0,0,0,0,0,0,0,0,0},
};

static int TemplateInstallDefault=0;

static struct _ParameterList TemplateInstallParameter[]=
{
	{0,{"no",0,0},"never install the tempalte",0,0,0,0,0,0,0,0,0,0},
	{1,{"yes",0,0},"always install the template",0,0,0,0,0,0,0,0,0,0},
	{2,{"blank",0,0},"install on a blank card",0,0,0,0,0,0,0,0,0,0},
};

#define TEMPLATE_PREFERENCE	{TemplatePreference,{"preference","default",0},"the prefered starting template",'d',0,1,1,1,&TemplatePreferenceMinimum,0,&TemplatePreferenceDefault,	\
	    sizeof(TemplatePreferenceParameter)/sizeof(TemplatePreferenceParameter[0]),TemplatePreferenceParameter}

#define TEMPLATE_ALLOWED {TemplateAllowed,{"allow",0,0},"which templates may be used",'d',0,MALLOWED,1,1,&TemplatePreferenceMinimum,0,&TemplatePreferenceDefault,	\
	    sizeof(TemplatePreferenceParameter)/sizeof(TemplatePreferenceParameter[0]),TemplatePreferenceParameter}

#define TEMPLATE_MEMORY {TemplateMemory,{"memory","caldata",0},"memory type used for calibration data",'z',0,1,1,1,&TemplateMemoryMinimum,&TemplateMemoryMaximum,&TemplateMemoryDefault,	\
	    sizeof(TemplateMemoryParameter)/sizeof(TemplateMemoryParameter[0]),TemplateMemoryParameter}

#define TEMPLATE_MEMORY_READ {TemplateMemory,{"memory","caldata",0},"memory type used for calibration data",'z',0,1,1,1,&TemplateMemoryMinimum,&TemplateMemoryMaximum,&TemplateMemoryDefaultRead,	\
	    sizeof(TemplateMemoryParameter)/sizeof(TemplateMemoryParameter[0]),TemplateMemoryParameter}

#define TEMPLATE_SIZE {TemplateSize,{"size",0,0},"memory size used for calibration data",'z',0,1,1,1,&TemplateSizeMinimum,&TemplateSizeMaximum,&TemplateSizeDefault,	\
	    sizeof(TemplateSizeParameter)/sizeof(TemplateSizeParameter[0]),TemplateSizeParameter}

#define TEMPLATE_SIZE_READ {TemplateSize,{"size",0,0},"memory size used for calibration data",'z',0,1,1,1,&TemplateSizeMinimum,&TemplateSizeMaximum,&TemplateSizeDefaultRead,	\
	    sizeof(TemplateSizeParameter)/sizeof(TemplateSizeParameter[0]),TemplateSizeParameter}

#define TEMPLATE_COMPRESS {TemplateCompress,{"compress",0,0},"use compression?",'z',0,1,1,1,0,0,&TemplateCompressionDefault,	\
	    sizeof(TemplateLogicalParameter)/sizeof(TemplateLogicalParameter[0]),TemplateLogicalParameter}

#define TEMPLATE_OVERWRITE {TemplateOverwrite,{"overwrite",0,0},"overwrite existing data?",'z',0,1,1,1,0,0,&TemplateOverwriteDefault,	\
	    sizeof(TemplateLogicalParameter)/sizeof(TemplateLogicalParameter[0]),TemplateLogicalParameter}

#define TEMPLATE_INSTALL {TemplateInstall,{"install",0,0},"install tempalte?",'z',0,1,1,1,0,0,&TemplateInstallDefault,	\
	    sizeof(TemplateInstallParameter)/sizeof(TemplateInstallParameter[0]),TemplateInstallParameter}
