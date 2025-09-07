/* 
 * File:   eeprom.h
 * Author: MrChunckuee
 *
 * Created on 30 de agosto de 2025, 11:46 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
    uint8_t EEPROMInternal_Read(uint8_t EEDIR);
    void EEPROMInternal_Write(uint8_t EEDIR, uint8_t EEVAL);
    uint16_t EEPROMInternal_IntRead(uint8_t EEDIR);
    void EEPROMInternal_IntWrite(uint8_t EEDIR, uint16_t AARG);
    void EEPROMInternal_FloatWrite(uint8_t EEDIR, float AARG);
    void EEPROMInternal_FloatRead(uint8_t EEDIR, float *AARG);
    void EEPROMInternal_WriteBuffer(uint8_t EEDIR, uint8_t *EEBUFFER, uint8_t LEN);
    void EEPROMInternal_ReadBuffer(uint8_t EEDIR, uint8_t *EEBUFFER, uint8_t LEN);

    
#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

