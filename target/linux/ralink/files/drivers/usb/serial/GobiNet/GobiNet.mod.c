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
"depends=usbnet";

MODULE_ALIAS("usb:v1199p68A2d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p68C0d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v05C6p920Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9011d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9013d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9015d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9019d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v03F0p371Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9041d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9051d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9053d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9054d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9055d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9056d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9061d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1199p9057d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0846p68A2d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "6A6440D74067A2AEAC3FA6C");
