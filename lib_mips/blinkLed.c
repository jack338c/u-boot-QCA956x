/**
*@file		blinkLed.c
*@brief		blink leds when power on
*@author		chendezhi
*@date		2015/08/05
*/

#include <common.h>
#include <exports.h>
#include "../include/atheros.h"
#include "../include/config.h"

#ifndef GPIO_LED_PORT_WIFI_2G_AP
# define GPIO_LED_PORT_WIFI_2G_AP -1
#endif

#ifndef GPIO_LED_PORT_WIFI_5G_AP
# define GPIO_LED_PORT_WIFI_5G_AP -1
#endif

#ifndef GPIO_LED_PORT_ETHERNET
# define GPIO_LED_PORT_ETHERNET -1
#endif

#ifndef GPIO_LED_PORT_SYS
# define GPIO_LED_PORT_SYS -1
#endif

#ifndef GPIO_SOCKET_LED
# define GPIO_SOCKET_LED -1
#endif

#ifndef GPIO_LED_PORT_LED_CONTROL
# define GPIO_LED_PORT_LED_CONTROL -1
#endif

static void gpioOutputEnable(int gpio)
{
	//if don't have such a gpio
	if(gpio < 0)
	{
		return;
	}
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << gpio));
}

static void gpioOutputClear(int gpio)
{
	if(gpio < 0)
	{
		return;
	}
	ath_reg_rmw_clear(GPIO_OUT_ADDRESS, (1 << gpio));
}

static void gpioOutputSet(int gpio)
{
	if(gpio < 0)
	{
		return;
	}
	ath_reg_rmw_set(GPIO_OUT_ADDRESS, (1 << gpio));
}

static void initLeds(void)
{
 	ath_reg_rmw_set(GPIO_FUNCTION_ADDRESS, GPIO_FUNCTION_DISABLE_JTAG_SET(0x1));

	if(-1!=GPIO_LED_PORT_WIFI_2G_AP)
		gpioOutputEnable(GPIO_LED_PORT_WIFI_2G_AP);

	if(-1!=GPIO_LED_PORT_WIFI_5G_AP)
		gpioOutputEnable(GPIO_LED_PORT_WIFI_5G_AP);

	if(-1!=GPIO_LED_PORT_ETHERNET)
		gpioOutputEnable(GPIO_LED_PORT_ETHERNET);

	if(-1!=GPIO_LED_PORT_SYS)
		gpioOutputEnable(GPIO_LED_PORT_SYS);

#ifdef HAVE_SOCKET
	gpioOutputEnable(GPIO_SOCKET_LED);
#endif

	//leds' power
	if(-1!=GPIO_LED_PORT_LED_CONTROL)
		gpioOutputEnable(GPIO_LED_PORT_LED_CONTROL);
}

static void turnOffLeds(void)
{
	if(-1!=GPIO_LED_PORT_WIFI_2G_AP)
		gpioOutputClear(GPIO_LED_PORT_WIFI_2G_AP);
	
	if(-1!=GPIO_LED_PORT_WIFI_5G_AP)
		gpioOutputClear(GPIO_LED_PORT_WIFI_5G_AP);

	if(-1!=GPIO_LED_PORT_ETHERNET)
		gpioOutputClear(GPIO_LED_PORT_ETHERNET);

	if(-1!=GPIO_LED_PORT_SYS)
		gpioOutputClear(GPIO_LED_PORT_SYS);

#ifdef HAVE_SOCKET
	gpioOutputClear(GPIO_SOCKET_LED);
#endif
}

static void turnOnLeds(void)
{
	//write only register can not be read!
	//ath_reg_rd(GPIO_SET_ADDRESS, (1 << GPIO_LED_POWER));
	if(-1!=GPIO_LED_PORT_WIFI_2G_AP)
		gpioOutputSet(GPIO_LED_PORT_WIFI_2G_AP);

	if(-1!=GPIO_LED_PORT_WIFI_5G_AP)
		gpioOutputSet(GPIO_LED_PORT_WIFI_5G_AP);

	if(-1!=GPIO_LED_PORT_ETHERNET)
		gpioOutputSet(GPIO_LED_PORT_ETHERNET);

	if(-1!=GPIO_LED_PORT_SYS)
		gpioOutputSet(GPIO_LED_PORT_SYS);

#ifdef HAVE_SOCKET
	gpioOutputSet(GPIO_SOCKET_LED);
#endif
	//ath_reg_rmw_set(GPIO_OUT_ADDRESS, (1 << GPIO_LED_POWER));

	if(-1!=GPIO_LED_PORT_LED_CONTROL)
		gpioOutputSet(GPIO_LED_PORT_LED_CONTROL);

}

#ifdef CFG_DOUBLE_BOOT_FACTORY
void fwrecovery_gpio_init()
{
    initLeds();
}

int fwrecovery_led_on()
{
    turnOnLeds();
    return 1;
}

int fwrecovery_led_off()
{
    turnOffLeds();
    return 0;
}
#endif

void blinkLeds(void)
{
	/* add by phw, 2016-10-27, first turn off leds */
#if !defined(CFG_DOUBLE_BOOT_SECOND)
	initLeds();
	turnOffLeds();
#endif
// delay a while to sync with plc LED
#if  (defined CONFIG_WPA8630_V1) || (defined CONFIG_WPA8630P_V1)
	udelay(1100*1000);
#endif

// added by YXF 2016.10.13, delay a while to sync plc LED
#if (defined CONFIG_WPA8630_V2) || (defined CONFIG_WPA8630P_V2) || (defined CONFIG_WPA8730_V2)
	udelay(1500*1000);
#endif

#if (defined CONFIG_WPA8730_V1)
	udelay(750*1000); 
#endif

/*added by ZQQ 2015.11.23, delay a while to sync with plc LED*/
#if (defined CONFIG_WPA4230P_V1)
	/*fix bug 115146*/
	udelay(410*1000);
#endif
/*end added.*/
#if !defined(CFG_DOUBLE_BOOT_SECOND)
	turnOnLeds();
	udelay(500*1000);
	turnOffLeds();
	udelay(500*1000);
#endif	
	//turn on power led
	if(-1!=GPIO_LED_PORT_SYS)
		gpioOutputSet(GPIO_LED_PORT_SYS);
	
}
