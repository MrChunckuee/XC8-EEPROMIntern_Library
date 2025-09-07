/*******************************************************************************
 *
 *                 Internal EEPROM Library
 *
 *******************************************************************************
 * FileName:        eeprom.c
 * Complier:        XC8 v2.36
 * Author:          Pedro Sánchez Ramírez (MrChunckuee)
 * Blog:            http://mrchunckuee.blogspot.com/
 * Email:           mrchunckuee.psr@gmail.com
 * Description:     Library for access to internal EEPROM for PIC
 *******************************************************************************
 *                  MIT License
 * 
 * Copyright (c) 2016 Pedro Sánchez Ramírez
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************
 * Rev.         Date            Comment
 *  v1.0.0      30/08/2025      Created library.
 ******************************************************************************/
#include <xc.h>

/*******************************************************************************
 * Function:        uint8_t EEPROMInternal_Read(uint8_t EEDIR)
 * Description:     Lee el contenido de la EEPROM en la direccion indicada
 * Precondition:    None
 * Parameters:      EEDIR Direccion a leer
 * Return Values:   EEDATA Value
 * Remarks:         None
 ******************************************************************************/
uint8_t EEPROMInternal_Read(uint8_t EEDIR){
    EEADR = EEDIR;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;
}

/*******************************************************************************
 * Function:        void EEPROMInternal_Write(uint8_t EEDIR, uint8_t EEVAL)
 * Description:     Escribe un valor en una direccion especifica de la EEPROM.
 * Precondition:    None
 * Parameters:      EEVAL Valor a escribir
 *                  EEDIR Direccion a escribir
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void EEPROMInternal_Write(uint8_t EEDIR, uint8_t EEVAL){
    uint8_t GIEBitStatus = INTCONbits.GIE;
    EEADR = EEDIR;
    EEDATA = EEVAL;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    while(!PIR2bits.EEIF);
    PIR2bits.EEIF = 0;
    EECON1bits.WREN = 0;
    INTCONbits.GIE = GIEBitStatus;
}

/*******************************************************************************
 * Function:        uint16_t EEPROMInternal_IntRead(uint8_t EEDIR)
 * Description:     Lee el valor tipo int que se encuentra almacenado en la dirección EEDIR
 * Precondition:    None
 * Parameters:      EEDIR Direccion a leer
 * Return Values:   AARG Valor a leer tipo Int
 * Remarks:         None
 ******************************************************************************/
uint16_t EEPROMInternal_IntRead(uint8_t EEDIR){
    uint16_t AARG;
    // Asumiendo Big-endian
    uint8_t MSB_INT = EEPROMInternal_Read(EEDIR);        
    uint8_t LSB_INT = EEPROMInternal_Read(EEDIR+1);  
    AARG = (MSB_INT<<8)|LSB_INT;
    return AARG; 
}

/*******************************************************************************
 * Function:        void EEPROMInternal_IntWrite(uint8_t EEDIR, uint16_t AARG)
 * Description:     Escribe un valor tipo int AARG en la dirección EEDIR
 * Precondition:    None
 * Parameters:      AARG Valor a escribir tipo Int
 *                  EEDIR Direccion a escribir
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void EEPROMInternal_IntWrite(uint8_t EEDIR, uint16_t AARG){
    uint8_t LSB_INT = (uint8_t)(AARG);        
    uint8_t MSB_INT = (uint8_t)(AARG >> 8);  
    // Asumiendo Big-endian
    EEPROMInternal_Write(EEDIR, MSB_INT);
    EEPROMInternal_Write(EEDIR+1, LSB_INT);
}

/*******************************************************************************
 * Function:        void EEPROMInternal_WriteBuffer(uint8_t EEDIR, uint8_t *EEBUFFER, uint8_t LEN)
 * Description:     Escribe un BUFFER en la direccion EEDIR con una longitud LEN
 * Precondition:    None
 * Parameters:      EEBUFFER Buffer a escribir
 *                  EEDIR Direccion a escribir
 *                  LEN Longitud del buffer
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void EEPROMInternal_WriteBuffer(uint8_t EEDIR, uint8_t *EEBUFFER, uint8_t LEN){
    volatile uint8_t BUFF_COUNT;
    for(BUFF_COUNT=0; BUFF_COUNT<LEN; BUFF_COUNT++)
        EEPROMInternal_Write(EEDIR+BUFF_COUNT, EEBUFFER[BUFF_COUNT]);
}

/*******************************************************************************
 * Function:        void EEPROMInternal_ReadBuffer(uint8_t EEDIR, uint8_t *EEBUFFER, uint8_t LEN)
 * Description:     Lee un BUFFER de la direccion EEDIR con una longitud LEN
 * Precondition:    None
 * Parameters:      EEBUFFER Buffer a leer
 *                  EEDIR Direccion a leer
 *                  LEN Longitud del buffer
 * Return Values:   None
 * Remarks:         None
 ******************************************************************************/
void EEPROMInternal_ReadBuffer(uint8_t EEDIR, uint8_t *EEBUFFER, uint8_t LEN){
    uint8_t BUFF_COUNT;
    for(BUFF_COUNT=0; BUFF_COUNT<LEN; BUFF_COUNT++)
        *(EEBUFFER+BUFF_COUNT) = EEPROMInternal_Read(EEDIR+BUFF_COUNT);
}
