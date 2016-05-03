#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v05C6p920Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p920Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p9001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p9008d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p900Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p9011d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p9025d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p9046d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1435pD111d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1435pD112d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1435pD116d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "AC3C36AB140223FD24CBD14");
