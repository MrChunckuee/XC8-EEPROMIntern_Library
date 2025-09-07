/*******************************************************************************
 *
 *      EEPROM Interna
 *
 *******************************************************************************
 * FileName:        main.c
 * Processor:       PIC18F25K22
 * Complier:        XC8 v2.36
 * Author:          Pedro Sanchez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.electronics@gmail.com
 * Description:     Libreria para control de la memoria EEPROM interna
 *
 *******************************************************************************
 * Rev.         Date            Comment
 *   v1.0.0     10/04/2025      - Creacion y prueba de funcionamiento de la libreria 
 *                                con el PIC18F25K22.
 ******************************************************************************/
#include <xc.h>
#include "FUSES_Config.h"
#include "eeprom.h"

#define _XTAL_FREQ  64000000

void MCU_Init(void);
void USER_EEPROMTest(void);

void main(void){
    MCU_Init();
    USER_EEPROMTest();
    while(1){   
    }
}

void MCU_Init(void){
    // Pines de entrada digitles
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    //Configura oscilador interno a 16MHz
    OSCCONbits.IRCF = 0b111; //El ciclo de instruccion es de 0.25 us
    //PLL FOSCx4 64MHz, el ciclo de instruccion es de .0625 us
    OSCTUNEbits.PLLEN = 1;
    
    //Configura entras/salidas
    TRISA = 0b00000000;
    TRISB = 0b00000000;
    TRISC = 0b00000000;
  
    //Limpiamos los puertos
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;   
}

void USER_EEPROMTest(void){
    uint8_t rwEEPROM_temp;
    uint16_t rwEEPROM_tempINT = 0;
    uint8_t wEEPROM_BUFF[5] = {0x0A, 0x0B, 0x0C, 
                               0x0D, 0x0E};
    uint8_t rEEPROM_BUFF[5];
    
    rwEEPROM_temp = EEPROMInternal_Read(0x00);
    if(rwEEPROM_temp == 0xFF)
        EEPROMInternal_Write(0x00, 0xAA);
    else EEPROMInternal_Write(0x00, 0xFF); 
    __delay_ms(50);
    
    EEPROMInternal_IntWrite(0x10, 0x2A3B);
    rwEEPROM_tempINT = EEPROMInternal_IntRead(0x10);
    EEPROMInternal_IntWrite(0x16, rwEEPROM_tempINT);
    __delay_ms(50);
    
    EEPROMInternal_WriteBuffer(0x20, wEEPROM_BUFF, 5);
    EEPROMInternal_ReadBuffer(0x20, rEEPROM_BUFF, 5);
    EEPROMInternal_WriteBuffer(0x26, rEEPROM_BUFF, 5);
    __delay_ms(50);
}