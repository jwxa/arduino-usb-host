#include <hidboot.h>
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include<SoftwareSerial.h>
//新建一个softSerial对象，rx:2,tx:0
SoftwareSerial softSerial1(2,0);

class HIDRelay : public HIDReportParser 
{
    protected:
    virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};


void HIDRelay::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
softSerial1.write(0x0c);  //BYTE1      
softSerial1.write(0x00);  //BYTE2
softSerial1.write(0xA1);  //BYTE3
softSerial1.write(0x01);  //BYTE4 
softSerial1.write(buf[1]);  //BYTE5
softSerial1.write(0x00);  //BYTE6          
softSerial1.write(buf[2]);  //BYTE7
softSerial1.write(buf[3]);  //BYTE8
softSerial1.write(buf[4]);  //BYTE9
softSerial1.write(buf[5]);  //BYTE10          
softSerial1.write(buf[6]);  //BYTE11
softSerial1.write(buf[7]);  //BYTE12
//Serial.println(0x0c,HEX);  //BYTE1      
//Serial.println(0x00,HEX);  //BYTE2
//Serial.println(0xA1,HEX);  //BYTE3
//Serial.println(0x01,HEX);  //BYTE4 
//Serial.println(buf[1],HEX);  //BYTE5
//Serial.println(0x00,HEX);  //BYTE6          Enter Setup Mode
//Serial.println(buf[2],HEX);  //BYTE7
//Serial.println(buf[3],HEX);  //BYTE8
//Serial.println(buf[4],HEX);  //BYTE9
//Serial.println(buf[5],HEX);  //BYTE10          
//Serial.println(buf[6],HEX);  //BYTE11
//Serial.println(buf[7],HEX);  //BYTE12
//Serial.println("----------");
};

class KbdRptParser : public HIDRelay 
{
    void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

void KbdRptParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
//    DEBUG_PRINTLN("Kbd");

    HIDRelay::Parse(hid,is_rpt_id,len,buf);
};

USB     Usb;
//HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);
KbdRptParser KbdPrs;
HIDUniversal Hid(&Usb);


void setup() {
  // 初始化串口通信:
  Serial.begin(115200);
  softSerial1.begin(9600);
  delay( 200 );
  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
  delay( 200 );
  Hid.SetReportParser(0, &KbdPrs);
   //HidKeyboard.SetReportParser(0, &KbdPrs);
}

void loop() {
  Usb.Task();
}
