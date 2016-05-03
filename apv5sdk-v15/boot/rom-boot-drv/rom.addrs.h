typedef struct {
void (*serial_puts) (const char *s) ;
void (*udelay) (unsigned long usec) ;
void (*simple_mips_cache_reset)(void) ;
void (*call_fw)(u32, u32) ;
} bootrom_fn_t;

extern void (*serial_puts) (const char *s);
extern void (*udelay) (unsigned long usec);
extern void (*simple_mips_cache_reset)(void);
extern void (*call_fw)(u32, u32);

