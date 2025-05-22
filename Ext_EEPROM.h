/* 
 * File:   Ext_EEPROM.h
 * Author: Sagar A
 *
 * Created on 9 October, 2023, 3:23 PM
 */

#ifndef Ext_EEPROM
#define  Ext_EEPROM

#define SLAVE_EEPROM_READ		0xA1
#define SLAVE_EEPROM_WRITE		0xA0



//void init_ds1307(void);
void write_Ext_EEPROM(unsigned char address,  unsigned char data);
unsigned char read_Ext_EEPROM(unsigned char address);

#endif