/* 
 * File:   eeprom.h
 * Author: Sagar A
 *
 * Created on 21 September, 2023, 8:39 PM
 */
#ifndef EEPROM_H
#define EEPROM_H

void write_internal_eeprom(unsigned char address, unsigned char data); 
unsigned char read_internal_eeprom(unsigned char address);
void saveevent(char event[]);
void read_menu();
void read_password();
void menu();
void viewlog();
void printlog(unsigned char,unsigned char);
void downloadlog();
void clearlog();
void change_password();
void settime();
void save_time(char,char,char);
#endif