/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include "definitions.h"                // SYS function prototypes
#include "config/default/bsp/bsp.h"
#include "../libraries/microchip-LoRa-semtech/src/LoRa.h"

void init(void);
void _SS_CSN_LoRa_Clear(void);
void _SS_CSN_LoRa_Set(void);
void _RESET_LoRa_Clear(void);
void _RESET_LoRa_Set(void);

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    init();

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        LED_On();

        // send LoRa packet
        // send packet
        uint32_t bw = LoRaGetSignalBandwidth();
        LoRaBeginPacket(false);
        char buf[16];
        sprintf(buf+2, "hello %lu\r\n", bw);
        buf[0] = 0x00;
        buf[1] = 0x14; // LoRa address
        LoRaWrite((uint8_t*)buf, strlen(buf) + 1);
        LoRaEndPacket(false);

        LED_Off();
        SYSTICK_DelayMs(5000);
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

void init(void) {
    LED_On();
    SYSTICK_TimerStart();
    // LoRa
    LoRaInitDriver(
            SERCOM6_SPI_WriteRead,
            SERCOM6_SPI_Write,
            SERCOM6_SPI_Read,
            SERCOM6_SPI_IsBusy,
            SERCOM6_SPI_IsTransmitterBusy,
            _SS_CSN_LoRa_Clear,
            _SS_CSN_LoRa_Set,
            _RESET_LoRa_Clear,
            _RESET_LoRa_Set,
            SYSTICK_DelayMs
    );
    LoRaBegin(434E6);
    //    LoRaSetSpreadingFactor(12);

    // register interrupt callbacks


    LED_Off();
};

void _SS_CSN_LoRa_Clear(void) {SS_LORA_Clear();};
void _SS_CSN_LoRa_Set(void) {SS_LORA_Set();};
void _RESET_LoRa_Clear(void) {RESET_LORA_Clear();};
void _RESET_LoRa_Set(void) {RESET_LORA_Set();};

/*******************************************************************************
 End of File
*/

