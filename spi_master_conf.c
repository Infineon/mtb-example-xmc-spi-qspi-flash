/******************************************************************************
 * File Name:   spi_master_conf.c
 *
 * Description: This is the source code for the XMC MCU: SPI QSPI Flash
 *              example for ModusToolbox. This file contains all the
 *              configurations required to interface with the on-board
 *              external memory chip on XMC4700 Relax Kit V1.
 *
 * Related Document: See README.md
 *
 ******************************************************************************
 *
 * Copyright (c) 2015-2021, Infineon Technologies AG
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

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
extern void SPI_MASTER_lTransmitHandler(const SPI_MASTER_t * const handle);
extern void SPI_MASTER_lReceiveHandler(const SPI_MASTER_t * const handle);
static SPI_MASTER_STATUS_t SPI_MASTER_0_lInit(void);

/*******************************************************************************
* Data Structures
*******************************************************************************/
/* QSPI Data IO0 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI0 = 
{
    .port = (XMC_GPIO_PORT_t *)PORT4_BASE,
    .pin  = (uint8_t)7
};         

/* Configuration for QSPI Data IO0 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI0_Config = 
{ 
    .port_config =
    {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
    },
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI Data IO1 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI1 = 
{
    .port = (XMC_GPIO_PORT_t *)PORT4_BASE,
    .pin  = (uint8_t)6
};     

/* Configuration for QSPI Data IO1 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI1_Config = 
{ 
    .port_config =
    {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
    },
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI Data IO2 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI2 = 
{
    .port = (XMC_GPIO_PORT_t *)PORT4_BASE,
    .pin  = (uint8_t)5
};

/* Configuration for QSPI Data IO2 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI2_Config = 
{ 
    .port_config =
    {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
    },
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI Data IO3 pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_MOSI3 = 
{
    .port = (XMC_GPIO_PORT_t *)PORT4_BASE,
    .pin  = (uint8_t)4
};

/* Configuration for QSPI Data IO3 pin */
SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_MOSI3_Config = 
{ 
    .port_config =
    {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
    },
    .hw_control = XMC_GPIO_HWCTRL_PERIPHERAL1
};

/* QSPI SCLK pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_SCLKOUT = 
{
    .port = (XMC_GPIO_PORT_t *)PORT4_BASE,
    .pin  = (uint8_t)2
};     

/* Configuration for QSPI SCLK pin */
const SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_SCLKOUT_Config = 
{ 
    .port_config =
    {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
    }
};

/* QSPI Slave Select pin assignment */
const SPI_MASTER_GPIO_t SPI_MASTER_0_SS_0 =
{
    .port = (XMC_GPIO_PORT_t *)PORT4_BASE,
    .pin  = (uint8_t)3
};

/* Configuration for QSPI Slave Select pin */
const SPI_MASTER_GPIO_CONFIG_t SPI_MASTER_0_SS_0_Config =
{
    .port_config =
    {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_STRONG_MEDIUM_EDGE
    },
    .slave_select_ch = XMC_SPI_CH_SLAVE_SELECT_2
};

/* SPI Master channel configuration */
XMC_SPI_CH_CONFIG_t SPI_MASTER_0_Channel_Config =
{
    .baudrate = BAUD_RATE,
    .bus_mode = (XMC_SPI_CH_BUS_MODE_t)XMC_SPI_CH_BUS_MODE_MASTER,
    .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
    .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};

/* SPI Master driver configuration */
const SPI_MASTER_CONFIG_t SPI_MASTER_0_Config  = 
{
    .channel_config          = &SPI_MASTER_0_Channel_Config,
    .fptr_spi_master_config  = SPI_MASTER_0_lInit,
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

    .tx_data_dummy = false,
    .rx_data_dummy = true,
    .tx_busy = false,
    .rx_busy = false
};

/* SPI Master initialization structure */
SPI_MASTER_t SPI_MASTER_0 =
{
    .channel = XMC_SPI2_CH1, /* USIC channel */
    .config  = &SPI_MASTER_0_Config, /* spi master configuration structure pointer */
    .runtime = &SPI_MASTER_0_runtime,
};

/* Configure the port registers and data input registers of SPI channel */
static SPI_MASTER_STATUS_t SPI_MASTER_0_lInit(void)
{
    SPI_MASTER_STATUS_t status;
    status = SPI_MASTER_STATUS_SUCCESS;
    /* LLD initialization */
    XMC_SPI_CH_Init(XMC_SPI2_CH1, &SPI_MASTER_0_Channel_Config);

    XMC_SPI_CH_SetBitOrderMsbFirst(XMC_SPI2_CH1);

    XMC_SPI_CH_SetWordLength(XMC_SPI2_CH1, (uint8_t)8);

    XMC_SPI_CH_SetFrameLength(XMC_SPI2_CH1, (uint8_t)64);

    /* Configure the clock polarity and clock delay */
    XMC_SPI_CH_ConfigureShiftClockOutput(XMC_SPI2_CH1,
            XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_DISABLED,
            XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);

    /* Configure Leading/Trailing delay */
    XMC_SPI_CH_SetSlaveSelectDelay(XMC_SPI2_CH1, 2U);

    /* Configure the data input line in loop back mode */
    XMC_SPI_CH_SetInputSource(XMC_SPI2_CH1, XMC_SPI_CH_INPUT_DIN0, (uint8_t)SPI_MASTER_INPUT_G);
    XMC_SPI_CH_SetInputSource(XMC_SPI2_CH1, XMC_SPI_CH_INPUT_DIN1, (uint8_t)SPI_MASTER_INPUT_G);
    XMC_SPI_CH_SetInputSource(XMC_SPI2_CH1, XMC_SPI_CH_INPUT_DIN2, (uint8_t)SPI_MASTER_INPUT_G);
    XMC_SPI_CH_SetInputSource(XMC_SPI2_CH1, XMC_SPI_CH_INPUT_DIN3, (uint8_t)SPI_MASTER_INPUT_G);

    /* Start the SPI_Channel */
    XMC_SPI_CH_Start(XMC_SPI2_CH1);

    /* Configure the pin properties */
    XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)7, &SPI_MASTER_0_MOSI0_Config.port_config);
    XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)6, &SPI_MASTER_0_MOSI1_Config.port_config);
    XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)5, &SPI_MASTER_0_MOSI2_Config.port_config);
    XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)4, &SPI_MASTER_0_MOSI3_Config.port_config);

    /* Configure the Hardware control mode selected for the pin */
    XMC_GPIO_SetHardwareControl((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)7, XMC_GPIO_HWCTRL_PERIPHERAL1);
    XMC_GPIO_SetHardwareControl((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)6, XMC_GPIO_HWCTRL_PERIPHERAL1);
    XMC_GPIO_SetHardwareControl((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)5, XMC_GPIO_HWCTRL_PERIPHERAL1);
    XMC_GPIO_SetHardwareControl((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)4, XMC_GPIO_HWCTRL_PERIPHERAL1);

    /* Initialize SPI SCLK out pin */
    XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)2, &SPI_MASTER_0_SCLKOUT_Config.port_config);

    /* Configure the pin properties */
    XMC_GPIO_Init((XMC_GPIO_PORT_t *)PORT4_BASE, (uint8_t)3, &SPI_MASTER_0_SS_0_Config.port_config);

    XMC_SPI_CH_EnableSlaveSelect(XMC_SPI2_CH1, XMC_SPI_CH_SLAVE_SELECT_2);

    XMC_USIC_CH_SetInterruptNodePointer(XMC_SPI2_CH1,
            XMC_USIC_CH_INTERRUPT_NODE_POINTER_PROTOCOL,
            (uint32_t)SPI_MASTER_SR_ID_2);

    /* Configure transmit FIFO settings */
    XMC_USIC_CH_TXFIFO_Configure(XMC_SPI2_CH1,
            32U,
            (XMC_USIC_CH_FIFO_SIZE_t)XMC_USIC_CH_FIFO_SIZE_32WORDS,
            1U);

    /* Configure the service interrupt nodes for standard transmit FIFO events */
    XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(XMC_SPI2_CH1,
            XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
            (uint32_t)SPI_MASTER_SR_ID_1);

    /* Configure receive FIFO settings */
    XMC_USIC_CH_RXFIFO_Configure(XMC_SPI2_CH1,
            0U,
            (XMC_USIC_CH_FIFO_SIZE_t)XMC_USIC_CH_FIFO_SIZE_32WORDS,
            0U);

    XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_SPI2_CH1,
            XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD,
            (uint32_t)SPI_MASTER_SR_ID_0);

    XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_SPI2_CH1,
            XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE,
            (uint32_t)SPI_MASTER_SR_ID_0);

    /* Set priority of the Transmit interrupt */
    NVIC_SetPriority((IRQn_Type)USIC2_1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 63U, 0U));

    /* Enable Transmit interrupt */
    NVIC_EnableIRQ((IRQn_Type)USIC2_1_IRQn);

    /* Set priority of the Receive interrupt */
    NVIC_SetPriority((IRQn_Type)USIC2_0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 62U, 0U));

    /* Enable Receive interrupt */
    NVIC_EnableIRQ((IRQn_Type)USIC2_0_IRQn);

    return status;
}

/* Transmit ISR */
void SPI_MASTER_0_tx_handler()
{
    SPI_MASTER_lTransmitHandler(&SPI_MASTER_0);
}

/* Receive ISR */
void SPI_MASTER_0_rx_handler()
{
    SPI_MASTER_lReceiveHandler(&SPI_MASTER_0);
}

/* [] END OF FILE */