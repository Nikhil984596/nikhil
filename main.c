/*
 * File:   main.c
 * Author: Sagar A
 *
 * Created on 20 September, 2023, 5:40 PM
 */


#include <xc.h>
#include <string.h>
#include "eeprom.h"
#include"Ext_EEPROM.h"
#include "clcd.h"
#include"uart.h"
#include"i2c.h"
#include"ds1307.h"
#include"adc.h"
#include"matrix_keypad.h"
unsigned char time[9] = "00:00:00";

static void init_config(void) {
    init_clcd();
    init_adc();
    init_i2c();
    init_ds1307();
    init_uart();
    init_matrix_keypad();
    read_password();
    
}
unsigned short adc_reg_val;

unsigned char gear[8][3] = {"ON", "GR", "GN", "G1", "G2", "G3", "G4", "C "};
unsigned char i = 0;
unsigned int SPEED;
unsigned char d[3];
unsigned char eventflag = 1, lap, lapflag;
unsigned char Pass[5]="0011";
unsigned char attempt = 0;
unsigned char ch, dflag, menuflag, logflag;

void main(void) {
    
    init_config();
    unsigned char key = ALL_RELEASED;
    unsigned char log[10];
    while (1) {
        get_time();
        clcd_print("Time     EV   SP", LINE1(0));
        clcd_print(time, LINE2(0));
        adc_reg_val = read_adc(CHANNEL4);
        d[2] = '\0';

        //to scale down the value to 0-99
        SPEED = adc_reg_val / 10.33;
        d[0] = (SPEED / 10) + 48;
        d[1] = (SPEED % 10) + 48;

        clcd_print(d, LINE2(14));
        key = read_switches(STATE_CHANGE);
        //mkp 1 is used to indicate collision
        if (key == MK_SW1) {
            i = 7;
            eventflag = 1;
        }
        //mkp2 and mkp3  is used to go up and down
        else if ((key == MK_SW2 || key == MK_SW3) && i == 7) {
            i = 2;
            eventflag = 1;
        } else if (key == MK_SW2) {
            if (i < 6) {
                i++;
                eventflag = 1;
            }
        } else if (key == MK_SW3) {
            if (i > 1) {
                i--;
                eventflag = 1;
            }
        }
        clcd_print(gear[i], LINE2(9));
        //to save every event eventflag =1 
        if (eventflag == 1) {
            eventflag = 0;
            saveevent(gear[i]);
            lap++;
            if (lap == 10) {
                lap = 0;
                lapflag = 1;
            }

        } else if (key == MK_SW11) {
            dflag = 1;
            read_menu();
        }

    }
}

void saveevent(char event[]) {
    //to save the event in ext eeprom
    char log[] = {time[0], time[1], time[3], time[4], time[6], time[7], event[0], event[1], d[0], d[1]};
    for (unsigned int i = 0; i < 10; i++) {
        write_Ext_EEPROM(lap * 10 + i, log[i]);
    }
}

void read_password() {
    //to save password in eeprom
    unsigned char ch = read_Ext_EEPROM(124);
    if (ch == '#') {
        Pass[0] = read_Ext_EEPROM(120);
        Pass[1] = read_Ext_EEPROM(121);
        Pass[2] = read_Ext_EEPROM(122);
        Pass[3] = read_Ext_EEPROM(123);

    } else {
        write_Ext_EEPROM(120, Pass[0]);
        write_Ext_EEPROM(121, Pass[1]);
        write_Ext_EEPROM(122, Pass[2]);
        write_Ext_EEPROM(123, Pass[3]);
        write_Ext_EEPROM(124, '#');

    }
}

void read_menu() {
    //to read password if pass is correct  logged into menu or else after 3 attempts user has to wait 120 sec 
    unsigned char count = 0;
    unsigned int wait = 0;
    unsigned char attempt = 3;
    char i = 0;
    unsigned char key = ALL_RELEASED;
    char str[5]={0};
    str[4] = '\0';
    CLEAR_DISP_SCREEN;
    clcd_print("ENTER PASSWORD", LINE1(0));

    while (dflag) {
        
        key = read_switches(STATE_CHANGE);
        //to blink underscore
        if (wait++ < 12000) {
            clcd_putch('_', LINE2(i));
        } else if (wait++ < 24000) {
            clcd_putch(' ', LINE2(i));
        } else {
            wait = 0;
            count++;
        }
        if (count == 5) {
            dflag = 0;
            continue;
        }
        if (key == MK_SW11) {

            str[i] = '0';
            clcd_putch('*', LINE2(i));
            i++;
        } else if (key == MK_SW12) {

            str[i] = '1';
            clcd_putch('*', LINE2(i));
            i++;
        }
        if (i == 4) {
            CLEAR_DISP_SCREEN;
            
            if (strcmp(Pass, str) == 0) {
                menuflag = 1;
                menu();
                break;
            }
            else {
                if (attempt) {
                    // attempt--;
                    i = 0;
                    ch = attempt + 48;
                    CLEAR_DISP_SCREEN;
                    clcd_print("TRY AGAIN", LINE1(0));
                    clcd_putch(ch, LINE2(0));
                    clcd_print("ATTEMPTS LEFT ", LINE2(2));
                    for (unsigned long int time = 500000; time--;);
                    CLEAR_DISP_SCREEN;
                    clcd_print("ENTER PASSWORD", LINE1(0));
                    //clcd_print("Enter Password", LINE1(0));
                    attempt--;
                } else {
                    char arr[4];
                    arr[3] = '\0';
                    //attempt = 3;
                    CLEAR_DISP_SCREEN;
                    clcd_print("YOU ARE BLOCKED", LINE1(0));
                    clcd_print("FOR     SECONDS", LINE2(0));
                    for (unsigned char time = 120; time--;) {
                        arr[0] = time / 100 + 48;
                        arr[2] = time % 10 + 48;
                        arr[1] = ((time / 10) % 10) + 48;
                        clcd_print(arr, LINE2(4));
                        for (unsigned int wait = 10; wait--;) {

                            for (unsigned int wait = 65000; wait--;);
                        }
                        attempt = 3;
                    }
                }
            }
        }
    }
}

void menu() {
    //to scroll up and down the menu using mkp11 and mkp12,mkp11 long press to enter a particular event mkp12 long press to come out of menu
    CLEAR_DISP_SCREEN;
    unsigned char key = ALL_RELEASED;
    unsigned int kcount;
    unsigned int kcount1;
    unsigned int i = 0;
    unsigned int star = 0;
    char Menu[][17] = {"View LOG        ", "Download LOG    ", "Clear LOG       ", "Set Time        ", "Change Password "};
    while (menuflag) {
        //CLEAR_DISP_SCREEN;
        clcd_print(Menu[i % 5], LINE1(1));
        clcd_print(Menu[(i + 1) % 5], LINE2(1));
        if (star == 0) {
            clcd_putch('*', LINE1(0));
            clcd_putch(' ', LINE2(0));
        } else {
            clcd_putch('*', LINE2(0));
            clcd_putch(' ', LINE1(0));
        }

        key = read_switches(LEVEL_CHANGE);
        if (key == MK_SW12) {
            kcount++;
            if (kcount == 200) {
                menuflag = 0;
                continue;
            }
        } else if (key == MK_SW11) {
            kcount1++;
            if (kcount1 == 200) {
                if ((i + star) % 5 == 0) {
                    logflag = 1;
                    viewlog();
                } else if ((i + star) % 5 == 1) {
                    downloadlog();
                } else if ((i + star) % 5 == 2) {
                    clearlog();
                } else if ((i + star) % 5 == 3) {
                    settime();
                } else if ((i + star) % 5 == 4) {
                    change_password();
                    kcount = 0;
                }
                //CLEAR_DISP_SCREEN;
            }
        } else {
            if (kcount > 0 && kcount < 200) {
                if (i < 3 && star) {
                    i++;
                }
                star = 1;
            }
            if (kcount1 > 0 && kcount1 < 200) {
                if (i > 0 && !star) {
                    i--;
                }
                star = 0;
            }
            kcount = 0;
            kcount1 = 0;
        }
        //CLEAR_DISP_SCREEN;
    }
    CLEAR_DISP_SCREEN;
}

void viewlog() {
    //to view time event and speed in veiw log mkp11 and mkp12 used to scroll up and down
    unsigned char i, key = ALL_RELEASED, No = 0, lapcheck;
    unsigned int kcount = 0, kcount1 = 0;
    //saveevent("VL");
    /*lap++;
    if (lap == 10) {
        lapflag = 1;
        lap = 0;
    }*/
    CLEAR_DISP_SCREEN;
    clcd_print("LOG:", LINE1(0));
    if (lapflag == 0) {
        lapcheck = lap - 1;
        i = 0;
    } else {
        lapcheck = 9;
        i = lap;
    }

    while (logflag) {
        key = read_switches(0);
        if (key == MK_SW11) {
            kcount++;

        } else if (key == MK_SW12) {
            kcount1++;
            if (kcount1 == 1000) {
                logflag = 0;
                continue;
            }
        } else {
            if (kcount > 0 && kcount < 2000) {
                if (i < lapcheck && No < 9) {
                    i++;
                    No++;
                }
            } else if (kcount1 > 0 && kcount1 < 2000) {
                if (i > 0 && No > 0) {
                    i--;
                    No--;
                }
            }
            kcount = 0;
            kcount1 = 0;
        }
        printlog(i, No);
    }
}

void printlog(unsigned char i, unsigned char No) {
    //print the arr in viewlog
    char arr[17] = "    :  :        ";
    arr[0] = No + 48;
    arr[2] = read_Ext_EEPROM(i * 10 + 0);
    arr[3] = read_Ext_EEPROM(i * 10 + 1);
    arr[5] = read_Ext_EEPROM(i * 10 + 2);
    arr[6] = read_Ext_EEPROM(i * 10 + 3);
    arr[8] = read_Ext_EEPROM(i * 10 + 4);
    arr[9] = read_Ext_EEPROM(i * 10 + 5);
    arr[11] = read_Ext_EEPROM(i * 10 + 6);
    arr[12] = read_Ext_EEPROM(i * 10 + 7);
    arr[14] = read_Ext_EEPROM(i * 10 + 8);
    arr[15] = read_Ext_EEPROM(i * 10 + 9);
    clcd_print(arr, LINE2(0));
}

void downloadlog(void) {
    //to display in tera term using uart protocol
    saveevent("DL");
    lap++;
    if (lap == 10) {
        lapflag = 1;
        lap = 0;
    }
    puts("NO  TIME  EV SP\n\r");
    unsigned char i, limit;
    char arr[19] = "                   ";
    arr[16] = '\n';
    arr[17] = '\r';
    arr[18] = '\0';
    if (lapflag) {
        i = lap;
        limit = 10;
    } else {
        i = 0;
        limit = lap;
    }
    for (char No = 0; No < limit; No++) {
        arr[0] = No + 48;
        arr[2] = read_Ext_EEPROM(i * 10 + 0);
        arr[3] = read_Ext_EEPROM(i * 10 + 1);
        arr[5] = read_Ext_EEPROM(i * 10 + 2);
        arr[6] = read_Ext_EEPROM(i * 10 + 3);
        arr[8] = read_Ext_EEPROM(i * 10 + 4);
        arr[9] = read_Ext_EEPROM(i * 10 + 5);
        arr[11] = read_Ext_EEPROM(i * 10 + 6);
        arr[12] = read_Ext_EEPROM(i * 10 + 7);
        arr[14] = read_Ext_EEPROM(i * 10 + 8);
        arr[15] = read_Ext_EEPROM(i * 10 + 9);
        puts(arr);
        if (++i == 10) {
            i = 0;
        }
    }
}

void clearlog(void) {
    //to clear recorded data
    lap = 0;
    lapflag = 0;
    saveevent("CL");
    lap++;
    if (lap == 10) {
        lapflag = 1;
        lap = 0;
    }
}

void change_password() {
    //to change pass and write it in eeprom

    char str[5] = {}, str1[5] = {};
    unsigned char i = 0, j = 0, key = ALL_RELEASED;
    unsigned int cpflag = 1;
    str[4] = '\0';
    unsigned int wait = 0;
    str1[4] = '\0';
    CLEAR_DISP_SCREEN;
    while (cpflag) {

        key = read_switches(1);

        if (i < 4) {
            clcd_print("ENTER PASSWORD", LINE1(0));
            if (wait++ < 1500) {
                clcd_putch('_', LINE2(i));
            } else if (wait++ < 3000) {
                clcd_putch(' ', LINE2(i));
            } else {
                wait = 0;
            }
            if (key == MK_SW11) {

                str[i] = '0';
                clcd_putch('*', LINE2(i));
                i++;
            } else if (key == MK_SW12) {

                str[i] = '1';
                clcd_putch('*', LINE2(i));
                i++;
            }
        } else if (i == 4) {
            i++;
            CLEAR_DISP_SCREEN;
        } else {
            clcd_print("RE-ENTER PASS", LINE1(0));
            if (wait++ < 1500) {
                clcd_putch('_', LINE2(j));
            } else if (wait++ < 3000) {
                clcd_putch(' ', LINE2(j));
            } else {
                wait = 0;
            }
            if (key == MK_SW11) {

                str1[j] = '0';
                clcd_putch('*', LINE2(j));
                j++;
            } else if (key == MK_SW12) {

                str1[j] = '1';
                clcd_putch('*', LINE2(j));
                j++;
            }
            if (j == 4) {
                if (strcmp(str, str1) == 0) {
                    write_Ext_EEPROM(120, str[0]);
                    write_Ext_EEPROM(121, str[1]);
                    write_Ext_EEPROM(122, str[2]);
                    write_Ext_EEPROM(123, str[3]);
                    write_Ext_EEPROM(124, '#');
                    saveevent("CP");
                    lap++;
                    if (lap == 10) {
                        lap = 0;
                        lapflag = 1;
                    }
                    CLEAR_DISP_SCREEN;
                    clcd_print("PASS CHANGED", LINE1(0));
                    for (unsigned long time = 400000; time--;);                
                    cpflag = 0;

                } else {
                    cpflag = 0;
                    // break;
                }
            }
        }
    }
}

void settime() {
    //to set the time and change time for a particular field
    char hr = (time[0] - 48)*10 + (time[1] - 48);
    char min = (time[3] - 48)*10 + (time[4] - 48);
    char sec = (time[6] - 48)*10 + (time[7] - 48);
    char blink = 3;
    char clock[9];
    for (char i = 0; i <= 8; i++) {
        clock[i] = time[i];
    }
    unsigned char key = ALL_RELEASED;
    CLEAR_DISP_SCREEN;
    clcd_print("HH:MM:SS", LINE1(0));
    unsigned int j = 0, count = 0, count1 = 0, flag = 1;
    while (flag) {
        key = read_switches(0);
        if (key == MK_SW11) {
            count++;
            if (count == 2000) {
                save_time(hr, min, sec);
                flag = 0;
                continue;
            }
        } else if (key == MK_SW12) {
            count1++;
            if (count1 == 2000) {
                flag = 0;
                continue;
            }
        } else {
            if (count > 0 && count1 < 2000) {
                if (blink == 1) {
                    if (hr < 23) {
                        hr++;
                    } else {
                        hr = 0;
                    }
                } else if (blink == 2) {
                    if (min < 59) {
                        min++;
                    } else {
                        min = 0;
                    }
                } else if (blink == 3) {
                    if (sec < 59) {
                        sec++;
                    } else {
                        sec = 0;
                    }
                }
            } else if (count1 > 0 && count < 2000) {
                if (blink > 1) {
                    blink--;
                } else {
                    blink = 3;
                }
            }
            count = 0;
            count1 = 0;
        }
        clock[0] = hr / 10 + 48;
        clock[1] = hr % 10 + 48;
        clock[3] = min / 10 + 48;
        clock[4] = min % 10 + 48;
        clock[6] = sec / 10 + 48;
        clock[7] = sec % 10 + 48;
        if (j++ < 500) {
            if (blink == 1) {
                clock[0] = ' ';
                clock[1] = ' ';
            } else if (blink == 2) {
                clock[3] = ' ';
                clock[4] = ' ';
            } else if (blink == 3) {
                clock[6] = ' ';
                clock[7] = ' ';
            }
        } else {
            if (j == 1000) {
                j = 0;
            }

        }
        clcd_print(clock, LINE2(0));
    }
}

void save_time(char hr, char min, char sec) {
    //to save time in reg
    char flag;
    flag = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, flag | 0x80);
    flag = read_ds1307(HOUR_ADDR);
    flag = (flag & 0xB0) | ((hr / 10) << 4) | (hr % 10);
    write_ds1307(HOUR_ADDR, flag);
    flag = read_ds1307(MIN_ADDR);
    flag = (flag & 0x80) | ((min / 10) << 4) | (min % 10);
    write_ds1307(MIN_ADDR, flag);
    flag = read_ds1307(SEC_ADDR);
    flag = (flag & 0x80) | ((sec / 10) << 4) | (sec % 10);
    write_ds1307(SEC_ADDR, flag);
    flag = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, flag & 0x7F);
}






