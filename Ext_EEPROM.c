#include <xc.h>
#include "i2c.h"
#include "Ext_EEPROM.h"


/* 
 * DS1307 Slave address
 * D0  -  Write Mode
 * D1  -  Read Mode
 */



void write_Ext_EEPROM(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(SLAVE_EEPROM_WRITE);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(int i =5000;i--;);
}

unsigned char read_Ext_EEPROM(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(SLAVE_EEPROM_WRITE);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(SLAVE_EEPROM_READ);
	data = i2c_read();
	i2c_stop();

	return data;
}
