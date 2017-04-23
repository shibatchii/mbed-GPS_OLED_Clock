//
// GPS Clock. Display OLED.
//  GPS : Akizuki AE-GYSFDMAXB(Taiyo Yuuden) MT3339(MediaTek)
//  OLED: Akizuki SO1602AWWB-UC-WB-U(Sunlike Display Tech. Corp.) 
//  CPU : mbed NUCLEO-F401RE(ST)
//
// PinConnect
//  GPS-Board : TXD-PA_10(Serial1 RX) RXD-PA_9(Serial1 TX) 1PBS-PA_8(Digital In)
//  OLED-Board: SCL-PB_8(I2C1 SCL)(10kOhmPullup) SDA-PB_9(I2C1 SDA)(10kOhmPullup)
//
// 2017/04/22
// @shibatchii
// shibatchii@gmail.com
//

#include "mbed.h"
#include "rtos.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SO1602A.h" // Use SO1602A Librarly (Akinori Hashmoto 2015/Nov/06)

I2C i2c(I2C_SDA, I2C_SCL);
SO1602A oled(i2c);

int main()
{
    char *point;
    int ch; // GPS 1Char Data
    char st[100]={'\0'}; // GPS 1Line Data
    char hh[3]={'\0'}; // Hour
    char mm[3]={'\0'}; // Minitus
    char ss[3]={'\0'}; // Second
    char dd[3]={'\0'}; // Day
    char mi[3]={'\0'}; // Month
    char yy[5]={'\0'}; // Year
    struct tm tm; // GPS time (UTC)
    struct tm *tmj; // GPS time (JST)
    time_t time; // Time temp
    char bufj[30]={'\0'}; // YYYY/MM/DD H:MM:SS WWW
    char ymd[20]={'\0'}; // YYYY/MM/DD(WWW)
    char hms[20]={'\0'}; // HH:MM:SS(JST)

    Serial pc(SERIAL_TX, SERIAL_RX);
    Serial gps(PA_9, PA_10);
    DigitalOut myled(LED1);
    DigitalIn gps1s(PA_8);

    oled.init(); // Init
    oled.setContrast(0x10); // OLED Contrast 0x00-0xFF
    oled.setDispFlag(true, false, false); // Display On/Off, Cursor On/Off, Blink On/Off

    while(1){
        myled=gps1s; // GPS 1Sec signal to Board LED
        // PC Serial Buffer not Empty ?
        if(pc.readable() == 1){
            gps.putc(pc.getc());
        }
        // GPS Serial Buffer not Empty ?
        if(gps.readable() == 1){
            ch=gps.getc();
            strcat(st,(char *)&ch);
            // 1 Line bufferd ?
            if(ch=='\n'){
                point=strstr(st,"$GPZDA"); // Time Date Line Check?
                // Time Date Line ?
                if(point!=NULL){
                    // Cut data
                    strncpy(hh,&st[7],2);
                    strncpy(mi,&st[9],2);
                    strncpy(ss,&st[11],2);
                    strncpy(dd,&st[18],2);
                    strncpy(mm,&st[21],2);
                    strncpy(yy,&st[24],4);
                    // Set Time Struct for UTC -> JST
                    tm.tm_sec  = atoi(ss);
                    tm.tm_min  = atoi(mi);
                    tm.tm_hour = atoi(hh);
                    tm.tm_mday = atoi(dd);
                    tm.tm_mon  = atoi(mm)-1; // Month is 0 origin
                    tm.tm_year = atoi(yy)-1900; // Year is 1900 origin
                    tm.tm_wday = 0; // May be not use
                    tm.tm_yday = 0; // May be not use
                    tm.tm_isdst = 0; // Not summer time
                    time = mktime(&tm);
                    time = time + (time_t) (60*60*9); // JST = UTC + 9 Hour
                    tmj=localtime(&time); // JST
                    strftime(bufj, 30, "%Y/%m/%d %H:%M:%S %a\n", tmj); // Date Time Fromated
                    bufj[24]='\0'; // Char end
                    printf("%s\n",bufj); // Serial Out to PC
                    strftime(ymd, 20, "%Y/%m/%d(%a)", tmj); // Date Formated
                    strftime(hms, 20, "%H:%M:%S(JST)", tmj); // Time Formated
                    ymd[15]='\0'; // Char end
                    hms[13]='\0'; // Char end
                    oled.printf("%s\n",ymd); // I2C Out to OLED
                    oled.printf("  %s\n",hms); // I2C Out to OLED
                    st[0]='\0'; // Line buffer Initialize
                }else{
                    st[0]='\0'; // Line buffer Initialize
                }
            }
        }
    }
}
// End of program.
