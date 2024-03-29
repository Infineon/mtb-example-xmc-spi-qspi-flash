/******************************************************************************
 * File Name:   spi_master_conf.c
 *
 * Description: This is the source code for the XMC MCU: SPI QSPI Flash
 *              example for ModusToolbox. This file contains all the
 *              configurations required to interface with the on-board
 *              external memory chip on all supported Kits.
 *
 * Related Document: See README.md
 *
 ******************************************************************************
 *
 * Copyright (c) 2015-2022, Infineon Technologies AG
 * All rights reserved.
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

/*******************************************************************************
* Include header files
*******************************************************************************/
#include "spi_master.h"
#include "cycfg_peripherals.h"
#include "cycfg_pins.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
extern void SPI_MASTER_lTransmitHandler(const SPI_MASTER_t * const handle);
extern void SPI_MASTER_lReceiveHandler(const SPI_MASTER_t * const handle);

/*******************************************************************************
* Data Structures
*******************************************************************************/
/* QSPI Data IO0 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI0 =
{
    .port = CYBSP_QSPI_D0_PORT,
    .pin  = CYBSP_QSPI_D0_PIN
};

/* Configuration for QSPI Data IO0 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI0_Config =
{
    .port_config =&CYBSP_QSPI_D0_config,
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI Data IO1 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI1 =
{
    .port = CYBSP_QSPI_D1_PORT,
    .pin  = CYBSP_QSPI_D1_PIN
};

/* Configuration for QSPI Data IO1 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI1_Config =
{
    .port_config =&CYBSP_QSPI_D1_config,
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI Data IO2 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI2 =
{
    .port = CYBSP_QSPI_D2_PORT,
    .pin  = CYBSP_QSPI_D2_PIN
};

/* Configuration for QSPI Data IO2 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI2_Config =
{
    .port_config =&CYBSP_QSPI_D2_config,
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI Data IO3 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI3 =
{
    .port = CYBSP_QSPI_D3_PORT,
    .pin  = CYBSP_QSPI_D3_PIN
};

/* Configuration for QSPI Data IO3 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI3_Config =
{
    .port_config =&CYBSP_QSPI_D3_config,
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI SCLK pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_SCLKOUT =
{
    .port = CYBSP_QSPI_SCK_PORT,
    .pin  = CYBSP_QSPI_SCK_PIN
};

/* Configuration for QSPI SCLK pin */
const SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_SCLKOUT_Config =
{
    .port_config = &CYBSP_QSPI_SCK_config
};

/* QSPI Slave Select pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_SS_0 =
{
    .port = CYBSP_QSPI_SS_PORT,
    .pin  = CYBSP_QSPI_SS_PIN
};

/* Configuration for QSPI Slave Select pin */
const SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_SS_0_Config =
{
    .port_config = &CYBSP_QSPI_SS_config,
    .slave_select_ch = SPI_CHANNEL_SS_CHANNEL
};

/* SPI Master driver configuration */
const SPI_MASTER_CONFIG_t SPI_MASTER_0_Config  =
{
    .channel_config          = &SPI_CHANNEL_config,
    /* FIFO configuration */
    .tx_fifo_size            = (XMC_USIC_CH_FIFO_SIZE_t)XMC_USIC_CH_FIFO_SIZE_32WORDS,
    .rx_fifo_size            = (XMC_USIC_CH_FIFO_SIZE_t)XMC_USIC_CH_FIFO_SIZE_32WORDS,
    /* Clock Settings */
    .shift_clk_passive_level = XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
    .slave_select_lines      = (uint8_t)1,
    .leading_trailing_delay  = (uint8_t)2,
    .spi_master_config_mode  = XMC_SPI_CH_MODE_QUAD,
    .transmit_mode           = SPI_MASTER_TRANSFER_MODE_INTERRUPT,
    .receive_mode            = SPI_MASTER_TRANSFER_MODE_INTERRUPT,
    .tx_cbhandler            = NULL,
    .rx_cbhandler            = NULL,
    .parity_cbhandler        = NULL,
    .mosi_0_pin              = &SPI_MASTER_0_MOSI0, /*!< mosi0 pin pointer*/
    .mosi_0_pin_config       = &SPI_MASTER_0_MOSI0_Config,
    .mosi_1_pin              = &SPI_MASTER_0_MOSI1,
    .mosi_1_pin_config       = &SPI_MASTER_0_MOSI1_Config,
    .mosi_2_pin              = &SPI_MASTER_0_MOSI2,
    .mosi_2_pin_config       = &SPI_MASTER_0_MOSI2_Config,
    .mosi_3_pin              = &SPI_MASTER_0_MOSI3,
    .mosi_3_pin_config       = &SPI_MASTER_0_MOSI3_Config,
    .sclk_out_pin_config     = &SPI_MASTER_0_SCLKOUT_Config,
    .sclk_out_pin            = &SPI_MASTER_0_SCLKOUT,
    .slave_select_pin        = {&SPI_MASTER_0_SS_0, NULL,
                                NULL, NULL,
                                NULL, NULL,
                                NULL, NULL
                                },
    .slave_select_pin_config = {&SPI_MASTER_0_SS_0_Config, NULL,
                                NULL, NULL,
                                NULL, NULL,
                                NULL, NULL
                                },
    .tx_sr   = (SPI_MASTER_SR_ID_t)SPI_MASTER_SR_ID_1,
    .rx_sr   = (SPI_MASTER_SR_ID_t)SPI_MASTER_SR_ID_0,
};

/* SPI Master runtime configuration */
SPI_MASTER_RUNTIME_t SPI_MASTER_0_runtime =
{
    .spi_master_mode = XMC_SPI_CH_MODE_QUAD, /* spi master transmission mode */
    .word_length     = 8U,

#if (UC_SERIES == XMC44) || (UC_SERIES == XMC45)
#ifdef USIC0_C0_DX0_P1_4
    .dx0_input = (SPI_MASTER_INPUT_t)USIC0_C0_DX0_P1_4,
#else
    .dx0_input = SPI_MASTER_INPUT_INVALID,
#endif

#ifdef USIC0_C0_DX0_P1_4
    .dx0_input_half_duplex = (SPI_MASTER_INPUT_t)USIC0_C0_DX0_P1_4,
#else
    .dx0_input_half_duplex = SPI_MASTER_INPUT_INVALID,
#endif
#endif

#if (UC_SERIES == XMC47) || (UC_SERIES == XMC48)
#ifdef USIC2_C1_DX0_P4_6
    .dx0_input = (SPI_MASTER_INPUT_t)USIC2_C1_DX0_P4_6,
#else
    .dx0_input = SPI_MASTER_INPUT_INVALID,
#endif

#ifdef USIC2_C1_DX0_P4_6
    .dx0_input_half_duplex = (SPI_MASTER_INPUT_t)USIC2_C1_DX0_P4_6,
#else
    .dx0_input_half_duplex = SPI_MASTER_INPUT_INVALID,
#endif
#endif

    .tx_data_dummy = false,
    .rx_data_dummy = true,
    .tx_busy = false,
    .rx_busy = false
};

/* SPI Master initialization structure */
SPI_MASTER_t SPI_MASTER_0 =
{
    .channel = SPI_CHANNEL_HW, /* USIC channel */
    .config  = &SPI_MASTER_0_Config, /* spi master configuration structure pointer */
    .runtime = &SPI_MASTER_0_runtime,
};

/* Transmit ISR */
void SPI_CHANNEL_TRANSMIT_BUFFER_STANDARD_EVENT_HANDLER()
{
    SPI_MASTER_lTransmitHandler(&SPI_MASTER_0);
}

/* Receive ISR */
void SPI_CHANNEL_RECEIVE_BUFFER_STANDARD_EVENT_HANDLER()
{
    SPI_MASTER_lReceiveHandler(&SPI_MASTER_0);
}

/* [] END OF FILE */
