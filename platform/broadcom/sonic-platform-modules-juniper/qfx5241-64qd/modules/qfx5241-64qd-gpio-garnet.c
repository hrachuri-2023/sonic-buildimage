/*
 * Juniper GARNET GPIO driver
 *
 * drivers/gpio/gpio-garnet.c
 *
 * This driver is being adpoted from supercon FPGA driver.
 *
 * Copyright (C) 2023 Juniper Networks
 * Author: Arun kumar <arunkumara@juniper.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include "qfx5241-64qd-jnx-garnet.h"
#include <linux/pci.h>
#include <linux/mfd/core.h>

#define GARNET_GPIO_MAX_BITS_PER_REG		32
#define GARNET_GPIO_MAX_NGPIO_PER_GROUP		128

struct garnet_gpio_info {
	int (*get)(struct gpio_chip *chip, unsigned int gpionum);
	void (*set)(struct gpio_chip *chip, unsigned int gpionum, int value);
	int (*dirin)(struct gpio_chip *chip, unsigned int gpionum);
	int (*dirout)(struct gpio_chip *chip, unsigned int gpionum, int value);
	void (*set_multiple)(struct gpio_chip *chip,
			     unsigned long *mask,
			     unsigned long *bits);
};

struct garnet_gpio_chip {
	void __iomem *fpga_membase;
	const struct garnet_gpio_info *info;
	void __iomem *base;
	struct device *dev;
	struct gpio_chip gpio;
	int ngpio;
	int spi_busy_bit;
	struct garnet_fpga_data *pdriver_data;
};

#define to_garnet_chip(chip) \
	container_of((chip), struct garnet_gpio_chip, gpio)

/*
 * garnet_gpio_bytewise_bitop - Generic GARNET GPIO bit operation
 */
static void garnet_gpio_bytewise_bitop(struct garnet_gpio_chip *chip,
				unsigned int gpiono, unsigned int bit, bool set)
{
	u32 gpio_state;
	//unsigned long flags;
	void __iomem *iobase;
	struct garnet_fpga_data *pdriver_data = chip->pdriver_data;

	iobase = chip->base + (gpiono / 8);

	dev_info(chip->dev, "GARNET GPIO bitop %u, %u, %u\n",
		    gpiono, bit, set);

	mutex_lock(&pdriver_data->lock[chip->spi_busy_bit]);

	CHECK_SPI_BUSY(chip, chip->spi_busy_bit)
	gpio_state = ioread8(iobase);
	CHECK_SPI_BUSY(chip, chip->spi_busy_bit)

	if (set)
		gpio_state |= BIT(bit);
	else
		gpio_state &= ~BIT(bit);

	CHECK_SPI_BUSY(chip, chip->spi_busy_bit)
	iowrite8(gpio_state, iobase);
	CHECK_SPI_BUSY(chip, chip->spi_busy_bit)

	mutex_unlock(&pdriver_data->lock[chip->spi_busy_bit]);

	return;
};


/*
 * garnet_gpio_bytewise_get - Read the specified signal of the GPIO device.
 */
static int garnet_gpio_bytewise_get(struct gpio_chip *gc, unsigned int gpio)
{
	struct garnet_gpio_chip *chip = to_garnet_chip(gc);
	unsigned int bit   = gpio % 8;
	int val;
	struct garnet_fpga_data *pdriver_data = chip->pdriver_data;

	mutex_lock(&pdriver_data->lock[chip->spi_busy_bit]);

	CHECK_SPI_BUSY(chip, chip->spi_busy_bit)
	val = !!(ioread8(chip->base + (gpio / 8)) & BIT(bit));
	CHECK_SPI_BUSY(chip, chip->spi_busy_bit)

	mutex_unlock(&pdriver_data->lock[chip->spi_busy_bit]);

	return val;
}

/*
 * garnet_gpio_set - Write the specified signal of the GPIO device.
 */
static void garnet_gpio_bytewise_set(struct gpio_chip *gc, unsigned int gpio,
					int val)
{
	struct garnet_gpio_chip *chip = to_garnet_chip(gc);
	unsigned int bit   = gpio % 8;

	garnet_gpio_bytewise_bitop(chip, gpio, bit, val);
}

static struct garnet_gpio_info garnet_gpios[] = {
	{
	    .get = garnet_gpio_bytewise_get,
	    .set = garnet_gpio_bytewise_set,
	    .dirin = NULL,
	    .dirout = NULL,
	    .set_multiple = NULL,
	},
};

static void garnet_gpio_setup(struct garnet_gpio_chip *ggc)
{
	struct gpio_chip *chip = &ggc->gpio;
	const struct garnet_gpio_info *info = ggc->info;

	chip->get		= info->get;
	chip->set		= info->set;
	chip->dbg_show		= NULL;
	chip->can_sleep		= 0;
	chip->base		= -1;
	chip->ngpio		= ggc->ngpio;
	chip->label		= dev_name(ggc->dev);
	chip->parent		= ggc->dev;
	chip->of_node		= ggc->dev->of_node;
	chip->owner		= THIS_MODULE;
	chip->direction_input	= info->dirin;
	chip->direction_output	= info->dirout;
	chip->set_multiple 	= info->set_multiple;
}

static const struct of_device_id garnet_gpio_ids[] = {
	{ .compatible = "jnx,actn-gpioslave0-presence-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-presence-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave0-lpmod-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-lpmod-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave0-reset-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-reset-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave0-pwr-good", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-pwr-good", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-sfp-pres-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-sfp-tx_dis-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-sfp-tx_fault-gmc", .data = &garnet_gpios[0] },
	{ .compatible = "jnx,actn-gpioslave1-sfp-rx_los-gmc", .data = &garnet_gpios[0] },
	{ },
};
MODULE_DEVICE_TABLE(of, garnet_gpio_ids);

static int garnet_gpio_of_init(struct device *dev,
				struct garnet_gpio_chip *chip, int cell_id)
{
	int err;
	u32 val;
	const struct of_device_id *of_id;

	if (!dev->of_node) {
		dev_err(dev, "No device node\n");
		return -ENODEV;
	}

	of_id = of_match_device(garnet_gpio_ids, dev);
	if (!of_id) {
		dev_err(dev, "GARNET GPIO: Failure to match DTB compatibles\n");
		return -ENODEV;
	}

	chip->info = of_id->data;

	switch (cell_id) {
	case 4:
	    val = 40;
	    break;
	default:
	    val = 32;
	    break;
	}

		chip->ngpio = val;

	return err;
}

static int garnet_gpio_probe(struct platform_device *pdev)
{
	struct garnet_fpga_data *pdriver_data;
	struct device *dev = &pdev->dev;
	struct garnet_gpio_chip *chip;
	struct resource *res;
	int ret;

	const struct mfd_cell *cell = mfd_get_cell(pdev);

	dev_info(dev, "GARNET GPIO probe\n");

	chip = devm_kzalloc(dev, sizeof(*chip), GFP_KERNEL);
	if (!chip) {
		dev_err(dev, "GARNET GPIO: malloc failed for gpio_chip\n");
		return -ENOMEM;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "GARNET GPIO: Failed to get mem resource\n");
		return -ENODEV;
	}

	dev_info(dev, "GARNET GPIO resource 0x%llx, %llu\n",
		 res->start, resource_size(res));

	chip->base = devm_ioremap(dev, res->start, resource_size(res));
	if (!chip->base) {
		dev_err(dev, "GARNET GPIO: ioremap of mem resource failed\n");
		return -ENOMEM;
	}

	pdriver_data = (struct garnet_fpga_data *)
			(pdev->dev.parent->driver_data);
	chip->pdriver_data = pdriver_data;
	chip->fpga_membase = pcim_iomap_table(pdriver_data->pdev)[0];

	if ((res->start % 0x10000) < 0x3000)
		chip->spi_busy_bit = 0;
	else
		chip->spi_busy_bit = 1;

	ret = garnet_gpio_of_init(dev, chip, cell->id);
	if (ret) {
		dev_err(dev, "GARNET GPIO: Of Init Failed\n");
		return ret;
	}

	chip->dev = dev;
	garnet_gpio_setup(chip);

	ret = gpiochip_add(&chip->gpio);
	if (ret) {
		dev_err(dev, "Failed to register GARNET gpiochip : %d\n", ret);
		return ret;
	}

	platform_set_drvdata(pdev, chip);
	dev_info(dev, "GPIO registered at 0x%lx, gpiobase: %d spi_busy_bit: %d\n",
			(unsigned long)chip->base, chip->gpio.base,
			chip->spi_busy_bit);

	return 0;
}

static int garnet_gpio_remove(struct platform_device *pdev)
{
	struct garnet_gpio_chip *chip = platform_get_drvdata(pdev);

	gpiochip_remove(&chip->gpio);

	return 0;
}

static struct platform_driver garnet_gpio_driver = {
	.driver = {
		.name = "gpio-garnet",
		.owner  = THIS_MODULE,
		.of_match_table = garnet_gpio_ids,
	},
	.probe = garnet_gpio_probe,
	.remove = garnet_gpio_remove,
};

module_platform_driver(garnet_gpio_driver);

MODULE_DESCRIPTION("Juniper GARNET FPGA GPIO driver");
MODULE_AUTHOR("Arun kumar <arunkumara@juniper.net>");
MODULE_LICENSE("GPL");
