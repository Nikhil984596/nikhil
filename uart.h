/* 
 * File:   uart.h
 * Author: Sagar A
 *
 * Created on 4 October, 2023, 5:28 PM
 */

#ifndef SCI_H
#define SCI_H

#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);
int puts(const char *s);
unsigned char getch(void);
unsigned char getch_with_timeout(unsigned short max_time);
unsigned char getche(void);

#endif
