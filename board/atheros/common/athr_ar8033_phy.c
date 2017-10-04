/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <config.h>
#include <linux/types.h>
#include <common.h>
#include <miiphy.h>
#include "phy.h"
#include <asm/addrspace.h>
#include <atheros.h>
#include "athrs_ar8033_phy.h"

//add by dragon 2014.12.8
#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA956x)))
#define PHY_ADDR 0x4
#endif

void
athrs_ar8033_mgmt_init(void)
{
//modify by dragon 2014.12.8
#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x)))
	smiWrite(PHY_ADDR, 0x00, 0x9000);		//add by dragon 2014.12.8
#elif (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA956x)))
    smiWrite(PHY_ADDR, 0x00, 0x1000);
#else
    uint32_t rddata;


    rddata = ath_reg_rd(GPIO_IN_ENABLE3_ADDRESS)&
             ~GPIO_IN_ENABLE3_MII_GE1_MDI_MASK;
    rddata |= GPIO_IN_ENABLE3_MII_GE1_MDI_SET(19);
    ath_reg_wr(GPIO_IN_ENABLE3_ADDRESS, rddata);
    
    ath_reg_rmw_clear(GPIO_OE_ADDRESS, ATH_GPIO);

    ath_reg_rmw_clear(GPIO_OE_ADDRESS, ATH_GPIO17);

    
    rddata = ath_reg_rd(GPIO_OUT_FUNCTION4_ADDRESS) & 
             ~ (GPIO_FUNCTION4_MASK);

    rddata |= (GPIO_FUNCTION4_ENABLE);

    ath_reg_wr(GPIO_OUT_FUNCTION4_ADDRESS, rddata);

#ifdef ATH_MDC_GPIO
    rddata = ath_reg_rd(GPIO_OUT_FUNCTION3_ADDRESS) &
           ~ (GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_MASK);

    rddata |= GPIO_OUT_FUNCTION3_ENABLE_GPIO_14_SET(0x21);

    ath_reg_wr(GPIO_OUT_FUNCTION3_ADDRESS, rddata);
#endif
#endif

	return;	//add by dragon 2014.12.8
}

int
athrs_ar8033_phy_setup(void  *arg)
{
#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA956x)))
     smiWrite(PHY_ADDR, 0x00, 0x9000);	//add by dragon 2014.12.8
 #endif
 
    return 0;
}

int
athrs_ar8033_phy_is_fdx(int ethUnit)
{

#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA956x)))
	//add by dragon 2014.12.8
	uint32_t phy_hw_status = 0x0;

    smiRead(PHY_ADDR, 0x11, &phy_hw_status);
    if ((phy_hw_status & (1<<13))>>13 == 1)
    {
        printf("Checking Duplex: Full\n");
        return 1;
    }
    printf("Checking Duplex: Half\n");
#else
   int phy_hw_status = 0x0;

   phy_hw_status = ath_reg_rd(SGMII_MAC_RX_CONFIG_ADDRESS);

   if (SGMII_MAC_RX_CONFIG_DUPLEX_MODE_GET(phy_hw_status) == 1) {
        return 1;
    } else if (SGMII_MAC_RX_CONFIG_DUPLEX_MODE_GET(phy_hw_status) == 0) {
        return 0;
    }
#endif

	
    return 0;
	
}

int
athrs_ar8033_phy_is_link_alive(int phyUnit)
{

#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA956x)))
	//add by dragon 2014.12.8
	uint32_t phy_hw_status = 0x0;

    smiRead(PHY_ADDR, 0x11, &phy_hw_status);
    if ((phy_hw_status & (1<<10))>>10 == 1)
    {
        printf("Checking Link: Up\n");
        return 1;
    }
    printf("Checking Link: Down\n");
	return 0;
#else
   int phy_hw_status = 0x0;

   phy_hw_status = ath_reg_rd(SGMII_MAC_RX_CONFIG_ADDRESS);

   if (SGMII_MAC_RX_CONFIG_LINK_GET(phy_hw_status))
        return 1;
    else
        return 0;
#endif
	
  }

int
athrs_ar8033_phy_is_up(int ethUnit)
{

#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA956x)))
	//add by dragon 2014.12.8
   uint32_t phy_hw_status = 0x0;

    smiRead(PHY_ADDR, 0x11, &phy_hw_status);
    if ((phy_hw_status & (1<<10))>>10 == 1)
    {
        printf("Checking Link: Up\n");
        return 1;
    }
    printf("Checking Link: Down\n");
    return 0;
#else
   int phy_hw_status = 0x0;

   phy_hw_status = ath_reg_rd(SGMII_MAC_RX_CONFIG_ADDRESS);
    if (SGMII_MAC_RX_CONFIG_LINK_GET(phy_hw_status))
        return 1;
    else
        return 0;
#endif
	
}
int
athrs_ar8033_phy_speed(int ethUnit)
{

#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA956x)))
	//add by dragon 2014.12.8
	uint32_t phy_hw_status = 0x0;
    uint32_t speed = 0x0;

    smiRead(PHY_ADDR, 0x11, &phy_hw_status);
    speed = (phy_hw_status & (3<<14)) >> 14;

    switch (speed) {
         case 0:
                 printf("Checking Speed 10BaseT\n");
                 return _10BASET;
                 break;
         case 1:
                 printf("Checking Speed 100BaseT\n");
                 return _100BASET;
                 break;
         case 2:
                 printf("Checking Speed 1000BaseT\n");
                 return _1000BASET;
                 break;
         default:
                 printf("Checking Speed Unknown\n");
                 return -1;
                 break;
    }
#else
   int phy_hw_status = 0x0,speed;

   phy_hw_status = ath_reg_rd(SGMII_MAC_RX_CONFIG_ADDRESS);

   speed = ((phy_hw_status & (3 << 10)) >> 10);

   switch (speed) {
        case 0:
                return _10BASET;
                break;
        case 1:
                return _100BASET;
                break;
        case 2:
                return _1000BASET;
                break;
        default:
                return -1;
                break;
   }

#endif

	

    return -1;
	
}


int 
athrs_ar8033_reg_init(void *arg)
{
//modify by phw
#if (defined(CONFIG_ATHR_8033_PHY) && (defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA956x)))
	athrs_ar8033_mgmt_init();
 #else
 	athrs_ar8033_mgmt_init();
	
	phy_reg_write(0x1,0x5, 0x1f, 0x101);	

	printf("%s: Done %x \n",__func__, phy_reg_read(0x1,0x5,0x1f));
 #endif
 
	return 0;
}

