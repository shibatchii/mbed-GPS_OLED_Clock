# mbed-GPS_OLED_Clock

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

