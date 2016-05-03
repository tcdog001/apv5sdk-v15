#include <stdarg.h>
#define gApnpLogFileName  "/var/apnp.log"

void ApnpLogInitFile(char * fileName);
void ApnpLogCloseFile();
int checkApnpResetFile();

__inline__ void ApnpVlog(const char *format,va_list args);

__inline__ void ApnpDebugLog(const char *format,...);


