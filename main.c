/******************************************************************************
 * File Name:   main.c
 *
 * Description: This is the source code for the XMC MCU: SPI QSPI Flash
 *              example for ModusToolbox. This example demonstrates how
 *              an external memory can be interfaced via the QSPI
 *              interface.
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

#include "cybsp.h"
#include "cy_utils.h"
#include "xmc_spi.h"
#include "spi_master.h"
#include "N25Q03.h"
#include "retarget_io.h"
#include <stdio.h>

/*******************************************************************************
 * Macros
 *******************************************************************************/
#define DATA_LENGTH              256
#define EXT_MEM_START_ADDR       0x00000000
#define SUCCESS                  1
#define FAILURE                  2
#define WRITE_IN_PROGRESS_MSK    1
#define WRITE_IN_ENABLED         1
#define WRITE_EN_LATCH_MSK       2
#define WRITE_ENABLED            2

/* Declarations for LED toggle timing */
#define TICKS_PER_SECOND   1000
#define TICKS_WAIT_MS      500

/*******************************************************************************
 * Function Prototypes
 ********************************************************************************/
static uint8_t erase_sector();
static uint8_t program_page();
void SysTick_Handler(void);
static void handle_error();

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
/* Create buffer for transmit data */
uint8_t transmit_buffer[DATA_LENGTH];

/* Create Buffers to read data */
uint8_t receive_buffer[DATA_LENGTH];

/*******************************************************************************
 * Function Name: main
 ********************************************************************************
 * Summary:
 *  This is the main function. It initializes the UART for debug information,
 *  a LED for status indication and then performs the following operations:
 *   1. Erasing the external memory
 *   2. Programming data into the external memory
 *   3. Reading data back from external memory
 *
 * Parameters:
 *  none
 *
 * Return:
 *  int
 *
 *******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint8_t status = 0;
    uint32_t tmp;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Turn LED on before flash programming starts */
    XMC_GPIO_SetOutputHigh(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

    /* Initialize retarget-io to use the debug UART port */
    retarget_io_init();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("**************** XMC MCU: SPI QSPI Flash *****************\r\n");

    /* Initialize the SPI Master */
    SPI_MASTER_Init(&SPI_MASTER_0);

    /* Erase Sector */
    printf("\n\nErasing sector...\r\n");
    status = erase_sector();

    if(status == FAILURE)
    {
        printf("Error: Erase sector operation failed!\r\n");
        handle_error();
    }else{
        printf("Sector erase successful!\r\n");
    }

    /* Read the data from SPI flash chip */
    printf("\n\nIssuing read command...\r\n");
    N25Q03_ReadPage(EXT_MEM_START_ADDR, &receive_buffer[0]);

    printf("\r\nData read from external memory:\r\n");

    /* Check all the data received */
    for (tmp = 0; tmp < DATA_LENGTH; tmp++)
    {
        if(tmp % 8 == 0){
            printf("\n\r");
        }

        printf("0x%X ", receive_buffer[tmp]);
    }

    printf("\r\n\nData to be programmed into external memory:\r\n");

    /* Initialize the transmit buffer with 256 bytes of incrementing data */
    for (tmp = 0; tmp < DATA_LENGTH; tmp++)
    {
        transmit_buffer[tmp] = tmp;

        /* Add new line after 8 bytes displayed */
        if(tmp % 8 == 0){
            printf("\n\r");
        }

        /* Print the data being added to the buffer */
        printf("0x%X ", transmit_buffer[tmp]);
    }

    /* Program Page */
    printf("\r\n\nIssuing program command...\r\n");
    status = program_page();

    if(status == FAILURE)
    {
        printf("Error: Program page operation failed!\r\n");
        handle_error();
    }else{
        printf("Programming successful!\r\n");
    }

    /* Read the data from SPI flash chip */
    printf("\r\n\nIssuing read command again...\r\n");
    N25Q03_ReadPage(EXT_MEM_START_ADDR, &receive_buffer[0]);

    printf("\r\nData read back from external memory:\r\n");

    /* Check all the data received */
    for (tmp = 0; tmp < DATA_LENGTH; tmp++)
    {
        if(tmp % 8 == 0){
            printf("\n\r");
        }

        printf("0x%X ", receive_buffer[tmp]);

        /* Validate data */
        if (receive_buffer[tmp] != transmit_buffer[tmp])
        {
            printf("\r\nError: Data mismatch found!\r\n");
            handle_error();
        }
    }

    printf("\r\n\nSuccessfully completed demo!\r\n");

    for (;;)
    {
    }
}

/*******************************************************************************
 * Function Name: erase_sector
 ********************************************************************************
 * Summary:
 *  This function erases 256 bytes of data of the sector with start
 *  address 0x00000000
 *
 * Parameters:
 *  None
 *
 * Return:
 *  uint8_t - Status of operation
 *          - returns 1 on successful operation
 *          - returns 2 on failure
 *
 *******************************************************************************/
static uint8_t erase_sector()
{
    uint8_t tmp;

    /* Send write Enable data */
    N25Q03_WriteEnable();

    /* Read Status registers */
    tmp = N25Q03_StatusRead();

    /* Check if write enable is set */
    if ((tmp & WRITE_EN_LATCH_MSK) != WRITE_ENABLED)
        return FAILURE;

    /* Send sector erase command */
    N25Q03_SectorErase(EXT_MEM_START_ADDR);

    /* Wait till sector erase is completed */
    do
    {
        tmp = N25Q03_StatusRead();
    } while (tmp & WRITE_IN_PROGRESS_MSK);   /* wait until busy = 0 */

    return SUCCESS;
}

/*******************************************************************************
 * Function Name: program_page
 ********************************************************************************
 * Summary:
 *  This function programs 256 bytes of data to the sector with start
 *  address 0x00000000
 *
 * Parameters:
 *  None
 *
 * Return:
 *  uint8_t - Status of operation
 *          - returns 1 on successful operation
 *          - returns 2 on failure
 *
 *******************************************************************************/
static uint8_t program_page()
{
    uint32_t tmp;

    /* Send write enable command for programming page */
    N25Q03_WriteEnable();

    /* Read the status register value of SPI Flash chip */
    tmp = N25Q03_StatusRead();

    /* Check if write enable is set */
    if ((tmp & WRITE_EN_LATCH_MSK) != WRITE_ENABLED)
        return FAILURE;

    /* Program page */
    N25Q03_ProgramPage(EXT_MEM_START_ADDR, &transmit_buffer[0]);

    /* Wait till program page is finished */
    do
    {
        tmp = N25Q03_StatusRead();
    } while ((tmp & WRITE_IN_PROGRESS_MSK));    /* wait until busy = 0 */

    return SUCCESS;
}

/*******************************************************************************
 * Function Name: SysTick_Handler
 ********************************************************************************
 * Summary:
 * Function called by system timer every millisecond.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void SysTick_Handler(void)
{
    static uint32_t ticks = 0;

    ticks++;
    if (TICKS_WAIT_MS == ticks)
    {
        /* Toggle LED */
        XMC_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);
        ticks = 0;
    }
}


/*******************************************************************************
 * Function Name: handle_error
 ********************************************************************************
 * Summary:
 *  This function sets the LED to LOW and puts the application in a forever
 *  loop whenever an error is encountered
 *
 * Parameters:
 *  None
 *
 * Return:
 *  void
 *
 *******************************************************************************/
static void handle_error()
{
    /* Toggle user LED using system timer to indicate failure */
    SysTick_Config(SystemCoreClock / TICKS_PER_SECOND);

    /* Forever loop */
    while(1);
}

/* [] END OF FILE */
