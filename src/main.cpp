#include <Arduino.h>
#include <HomeSpan.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <qrcode.h>
#include <dev_light.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define pairingCode "11122333"
#define qrID "WRNG"
char* payloadStr;

void displayQRCode(const char* text) {
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData, 3, 0, text);

  int scale = min(u8g2.getWidth() / qrcode.size, u8g2.getHeight() / qrcode.size);
  int offsetX = (u8g2.getWidth() - qrcode.size * scale) / 2;
  int offsetY = (u8g2.getHeight() - qrcode.size * scale) / 2;

  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {
      if (qrcode_getModule(&qrcode, x, y)) {
        u8g2.drawBox(offsetX + x * scale, offsetY + y * scale, scale, scale);
      }
    }
  }
}

class getStatusPayload {
private:
  HapQR hapQR;
  const char* setupID;
  uint32_t setupCode;
  uint8_t category;

public:
  getStatusPayload(const char* setupID, uint32_t setupCode, uint8_t category)
    : setupID(setupID), setupCode(setupCode), category(category) {}

  char* generate() {
    return hapQR.get(setupCode, setupID, category);
  }
};

void statusUpdate(HS_STATUS status){
  u8g2.clearBuffer();
  u8g2.setBitmapMode(1);
  u8g2.setFontMode(1);
  u8g2.setFont(u8g2_font_ncenB08_tr);

  const char* statusStr = homeSpan.statusString(status);
  u8g2.drawUTF8(0, 10, statusStr);

  if (status == HS_PAIRING_NEEDED) {
    getStatusPayload payload(qrID, atoi(pairingCode), 1);
    payloadStr = payload.generate();

    u8g2.sendBuffer();
    delay(1000);
    u8g2.clearBuffer();
    displayQRCode(payloadStr);
    u8g2.sendBuffer();
  } else {
    u8g2.sendBuffer();
  }
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();

  homeSpan.setPairingCode(pairingCode);
  homeSpan.setQRID(qrID);
  // homeSpan.setStatusPin(2);
  homeSpan.setStatusCallback(statusUpdate);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("Desk Lamp");
    new light(2);
}

void loop() {
  homeSpan.poll();
}
