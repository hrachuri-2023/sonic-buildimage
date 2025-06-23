/* SPDX-License-Identifier: GPL-2.0
 *
 * Juniper GARNET-64 MFD resources
 *
 * This file is used by jnx-garnet-core.c
 *
 * Copyright (c) 2024, Juniper Networks
 * Author: Arun kumar Alapati (arunkumara@juniper.net)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef __JNX_GARNET64_CORE__
#define __JNX_GARNET64_CORE__

#define GARNET64_PORT_COUNT	66
#define	GARNET64_I2C_STRL_LEN	11

#define GARNET64_FPGA_VER_SPI_LOCK_MAJOR	0x5C
#define GARNET64_FPGA_VER_SPI_LOCK_MINOR	0x01

static struct resource garnet64_resource_gpioslave0_presence[] = {
	{
		.name = "actn-gpioslave0-presence-gmc",
		.start = GARNET64_GPIO_CPLD1_PRESENCE_START,
		.end   = GARNET64_GPIO_CPLD1_PRESENCE_END,
		.flags = IORESOURCE_MEM,
	}
};

#if 1 
static struct resource garnet64_resource_gpioslave1_presence[] = {
	{
		.name = "actn-gpioslave1-presence-gmc",
		.start = GARNET64_GPIO_CPLD2_PRESENCE_START,
		.end   = GARNET64_GPIO_CPLD2_PRESENCE_END,
		.flags = IORESOURCE_MEM,
	}
};
#endif

static struct resource garnet64_resource_gpioslave0_lpmod[] = {
	{
		.name = "actn-gpioslave0-lpmod-gmc",
		.start = GARNET64_GPIO_CPLD1_LPMOD_START,
		.end   = GARNET64_GPIO_CPLD1_LPMOD_END,
		.flags = IORESOURCE_MEM,
	}
};

#if 1
static struct resource garnet64_resource_gpioslave1_lpmod[] = {
	{
		.name = "actn-gpioslave1-lpmod-gmc",
		.start = GARNET64_GPIO_CPLD2_LPMOD_START,
		.end   = GARNET64_GPIO_CPLD2_LPMOD_END,
		.flags = IORESOURCE_MEM,
	}
};
#endif

static struct resource garnet64_resource_gpioslave0_reset[] = {
	{
		.name = "actn-gpioslave0-reset-gmc",
		.start = GARNET64_GPIO_CPLD1_RESET_START,
		.end   = GARNET64_GPIO_CPLD1_RESET_END,
		.flags = IORESOURCE_MEM,
	}
};

#if 1 
static struct resource garnet64_resource_gpioslave1_reset[] = {
	{
		.name = "actn-gpioslave1-reset-gmc",
		.start = GARNET64_GPIO_CPLD2_RESET_START,
		.end   = GARNET64_GPIO_CPLD2_RESET_END,
		.flags = IORESOURCE_MEM,
	}
};
#endif

static struct resource garnet64_resource_gpioslave0_pwr_good[] = {
	{
		.name = "actn-gpioslave0-pwr-good",
		.start = GARNET64_GPIO_CPLD1_PWR_GOOD_START,
		.end   = GARNET64_GPIO_CPLD1_PWR_GOOD_END,
		.flags = IORESOURCE_MEM,
	}
};

#if 1
static struct resource garnet64_resource_gpioslave1_pwr_good[] = {
	{
		.name = "actn-gpioslave1-pwr-good",
		.start = GARNET64_GPIO_CPLD2_PWR_GOOD_START,
		.end   = GARNET64_GPIO_CPLD2_PWR_GOOD_END,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource garnet64_resource_gpioslave1_sfp_pres[] = {
	{
		.name = "actn-gpioslave1-sfp-pres-gmc",
		.start = GARNET64_GPIO_CPLD2_SFP28_PRESENCE_START,
		.end   = GARNET64_GPIO_CPLD2_SFP28_PRESENCE_END,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource garnet64_resource_gpioslave1_sfp_tx_dis[] = {
	{
		.name = "actn-gpioslave1-sfp-tx_dis-gmc",
		.start = GARNET64_GPIO_CPLD2_SFP28_TX_DIS_START,
		.end   = GARNET64_GPIO_CPLD2_SFP28_TX_DIS_END,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource garnet64_resource_gpioslave1_sfp_tx_fault[] = {
	{
		.name = "actn-gpioslave1-sfp-tx_fault-gmc",
		.start = GARNET64_GPIO_CPLD2_SFP28_TX_FAULT_START,
		.end   = GARNET64_GPIO_CPLD2_SFP28_TX_FAULT_END,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource garnet64_resource_gpioslave1_sfp_rx_los[] = {
	{
		.name = "actn-gpioslave1-sfp-rx_los-gmc",
		.start = GARNET64_GPIO_CPLD2_SFP28_RX_LOS_START,
		.end   = GARNET64_GPIO_CPLD2_SFP28_RX_LOS_END,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource garnet64_resource_fpga_system_leds[] = {
	{
		.name = "actn-resource-fpga-system-leds",
		.start = GARNET64_GPIO_FPGA_SYSTEM_LED_START,
		.end   = GARNET64_GPIO_FPGA_SYSTEM_LED_END,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource irq_tmc[] = {
	/* PSU Presence Block */
	{
		.name  = "irq-tmc",
		.start = GARNET64_PSU_START,
		.end   = GARNET64_PSU_END,
		.flags = IORESOURCE_MEM,
	}
};
#endif

static struct resource garnet64_resource_cpld1_port_leds[] = {
	{
		.name = "actn-resource-cpld1-port-leds",
		.start = GARNET64_GPIO_CPLD1_PORT_LED_START,
		.end   = GARNET64_GPIO_CPLD1_PORT_LED_END,
		.flags = IORESOURCE_MEM,
	}
};
#if 1
static struct resource garnet64_resource_cpld2_port_leds[] = {
	{
		.name = "actn-resource-cpld2-port-leds",
		.start = GARNET64_GPIO_CPLD2_PORT_LED_START,
		.end   = GARNET64_GPIO_CPLD2_PORT_LED_END,
		.flags = IORESOURCE_MEM,
	}
};

static struct resource garnet64_resource_cpld2_sfp_port_leds[] = {
	{
		.name = "actn-resource-cpld2-sfp-port-leds",
		.start = GARNET64_GPIO_CPLD2_SFP_PORT_LED_START,
		.end   = GARNET64_GPIO_CPLD2_SFP_PORT_LED_END,
		.flags = IORESOURCE_MEM,
	}
};
#endif
static struct mfd_cell garnet64_cpld1_mfd_devs[] = {
	{
		.name = "actn-resource-cpld1-port-leds",
		.num_resources = ARRAY_SIZE(garnet64_resource_cpld1_port_leds),
		.resources = &garnet64_resource_cpld1_port_leds[0],
		.of_compatible = "jnx,actn-resource-cpld1-port-leds",
		.id = 0,
	},
	{
		.name = "actn-gpioslave0-presence-gmc",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave0_presence),
		.resources = &garnet64_resource_gpioslave0_presence[0],
		.of_compatible = "jnx,actn-gpioslave0-presence-gmc",
		.id = 1,
	},
	{
		.name = "actn-gpioslave0-lpmod-gmc",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave0_lpmod),
		.resources = &garnet64_resource_gpioslave0_lpmod[0],
		.of_compatible = "jnx,actn-gpioslave0-lpmod-gmc",
		.id = 2,
	},
	{
		.name = "actn-gpioslave0-reset-gmc",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave0_reset),
		.resources = &garnet64_resource_gpioslave0_reset[0],
		.of_compatible = "jnx,actn-gpioslave0-reset-gmc",
		.id = 3,
	},
	{
		.name = "actn-gpioslave0-pwr-good",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave0_pwr_good),
		.resources = &garnet64_resource_gpioslave0_pwr_good[0],
		.of_compatible = "jnx,actn-gpioslave0-pwr-good",
		.id = 4,
	},
};

#if 1 
static struct mfd_cell garnet64_cpld2_mfd_devs[] = {
	{
		.name = "actn-resource-cpld2-port-leds",
		.num_resources = ARRAY_SIZE(garnet64_resource_cpld2_port_leds),
		.resources = &garnet64_resource_cpld2_port_leds[0],
		.of_compatible = "jnx,actn-resource-cpld2-port-leds",
		.id = 0,
	},
	{
		.name = "actn-resource-cpld2-sfp-port-leds",
		.num_resources = ARRAY_SIZE(garnet64_resource_cpld2_sfp_port_leds),
		.resources = &garnet64_resource_cpld2_sfp_port_leds[0],
		.of_compatible = "jnx,actn-resource-cpld2-sfp-port-leds",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-presence-gmc",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave1_presence),
		.resources = &garnet64_resource_gpioslave1_presence[0],
		.of_compatible = "jnx,actn-gpioslave1-presence-gmc",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-reset-gmc",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave1_reset),
		.resources = &garnet64_resource_gpioslave1_reset[0],
		.of_compatible = "jnx,actn-gpioslave1-reset-gmc",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-pwr-good",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave1_pwr_good),
		.resources = &garnet64_resource_gpioslave1_pwr_good[0],
		.of_compatible = "jnx,actn-gpioslave1-pwr-good",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-sfp-pres-gmc",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave1_sfp_pres),
		.resources = &garnet64_resource_gpioslave1_sfp_pres[0],
		.of_compatible = "jnx,actn-gpioslave1-sfp-pres-gmc",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-sfp-tx_dis-gmc",
		.num_resources =
			ARRAY_SIZE(garnet64_resource_gpioslave1_sfp_tx_dis),
		.resources = &garnet64_resource_gpioslave1_sfp_tx_dis[0],
		.of_compatible = "jnx,actn-gpioslave1-sfp-tx_dis-gmc",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-sfp-tx_fault-gmc",
		.num_resources =
			ARRAY_SIZE(garnet64_resource_gpioslave1_sfp_tx_fault),
		.resources = &garnet64_resource_gpioslave1_sfp_tx_fault[0],
		.of_compatible = "jnx,actn-gpioslave1-sfp-tx_fault-gmc",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-sfp-rx_los-gmc",
		.num_resources =
			ARRAY_SIZE(garnet64_resource_gpioslave1_sfp_rx_los),
		.resources = &garnet64_resource_gpioslave1_sfp_rx_los[0],
		.of_compatible = "jnx,actn-gpioslave1-sfp-rx_los-gmc",
		.id = 0,
	},
	{
		.name = "actn-gpioslave1-lpmod-gmc",
		.num_resources = ARRAY_SIZE(garnet64_resource_gpioslave1_lpmod),
		.resources = &garnet64_resource_gpioslave1_lpmod[0],
		.of_compatible = "jnx,actn-gpioslave1-lpmod-gmc",
		.id = 0,
	},
};

static struct mfd_cell garnet64_fpga_mfd_devs[] = {
	{
		.name = "actn-resource-fpga-system-leds",
		.num_resources = ARRAY_SIZE(garnet64_resource_fpga_system_leds),
		.resources = &garnet64_resource_fpga_system_leds[0],
		.of_compatible = "jnx,actn-resource-fpga-system-leds",
		.id = 0,
	},
	{
		.name = "irq-tmc",
		.num_resources = ARRAY_SIZE(irq_tmc),
		.resources = &irq_tmc[0],
		.of_compatible = "jnx,irq-tmc",
		.id = 0,
	},

};
#endif
static struct mfd_cell garnet64_cpld1_mfd_i2c_devs[32];
static struct mfd_cell garnet64_cpld2_mfd_i2c_devs[34];
static struct resource garnet64_resource_i2c[GARNET64_PORT_COUNT];
static char garnet64_i2c_dev_str[GARNET64_PORT_COUNT][GARNET64_I2C_STRL_LEN];
#endif
