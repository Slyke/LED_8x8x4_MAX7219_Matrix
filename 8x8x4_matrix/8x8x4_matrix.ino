// MAX7219 OP codes (https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf)
#define OP_NOOP         0x00
#define OP_ROW_1        0x01
#define OP_ROW_2        0x02
#define OP_ROW_3        0x03
#define OP_ROW_4        0x04
#define OP_ROW_5        0x05
#define OP_ROW_6        0x06
#define OP_ROW_7        0x07
#define OP_ROW_8        0x08
#define OP_DECODE_MODE  0x09
#define OP_LED_BRIGHT   0x0A
#define OP_SCAN_LIMIT   0x0B
#define OP_SHUTDOWN     0x0C
#define OP_DISPLAY_TEST 0x0F

// Pins
#define CLK_PIN 13
#define DTA_PIN 11
#define CHP_PIN 7

// Chip Stuff
#define CHIP_COUNT 4
#define COLUMN_COUNT 8

void clearDisplay(int chipAddress) {
    int offset;

    offset = chipAddress * COLUMN_COUNT;
    for(int i = 0; i < COLUMN_COUNT; i++) {
      maxTransfer(chipAddress, i + 1, B00000000);
    }
}

void maxTransfer(int chipAddress, byte opCode, byte data) {

  int offset = chipAddress * 2;
  int maxBytes = CHIP_COUNT * 2;

  byte spiData[maxBytes] = {};

  for(int i = 0; i < maxBytes; i++) {
    spiData[i] = (byte)0;
  }

  spiData[offset] = data;
  spiData[offset + 1] = opCode;
  
  digitalWrite(CHP_PIN, LOW);

  for(int i = maxBytes; i > 0; i--) {
    shiftOut(DTA_PIN, CLK_PIN, MSBFIRST, spiData[i - 1]);
//    if (spiData[i-1] < 16) { Serial.print("0"); }
//    Serial.print(spiData[i - 1], HEX);
//    Serial.print(" ");
  }
//  Serial.println("");
  
  digitalWrite(CHP_PIN, HIGH);
}

void setup() {
//  Serial.begin(115200);
  pinMode(CHP_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(DTA_PIN, OUTPUT);

  digitalWrite(CHP_PIN, HIGH);

  maxTransfer(0, OP_DISPLAY_TEST, 0x00);
  maxTransfer(1, OP_DISPLAY_TEST, 0x00);
  maxTransfer(2, OP_DISPLAY_TEST, 0x00);
  maxTransfer(3, OP_DISPLAY_TEST, 0x00);

  maxTransfer(0, OP_SCAN_LIMIT, 0x07);
  maxTransfer(1, OP_SCAN_LIMIT, 0x07);
  maxTransfer(2, OP_SCAN_LIMIT, 0x07);
  maxTransfer(3, OP_SCAN_LIMIT, 0x07);

  maxTransfer(0, OP_LED_BRIGHT, 0x09);
  maxTransfer(1, OP_LED_BRIGHT, 0x09);
  maxTransfer(2, OP_LED_BRIGHT, 0x09);
  maxTransfer(3, OP_LED_BRIGHT, 0x09);

  maxTransfer(0, OP_DECODE_MODE, 0x00);
  maxTransfer(1, OP_DECODE_MODE, 0x00);
  maxTransfer(2, OP_DECODE_MODE, 0x00);
  maxTransfer(3, OP_DECODE_MODE, 0x00);

  maxTransfer(0, OP_SHUTDOWN, 0x01);
  maxTransfer(1, OP_SHUTDOWN, 0x01);
  maxTransfer(2, OP_SHUTDOWN, 0x01);
  maxTransfer(3, OP_SHUTDOWN, 0x01);

//  clearDisplay(0);
//  clearDisplay(1);
//  clearDisplay(2);
//  clearDisplay(3);

}

void loop() {

  int delayTime = 1500;

  delay(delayTime);
  
  clearDisplay(0);
  clearDisplay(1);
  clearDisplay(2);
  clearDisplay(3);
  
  delay(delayTime);

  // Binary Count
  for (uint8_t k = 0; k < 0x04; k++) { // 8x8 Screen
    for (byte j = 1; j <= 0x08; j++) { // Row
      for (byte i = 0; i <= 0xFE; i++) { // Column array
        maxTransfer(k, j, i+1);
//        delay(1);
      }
    }
  }
}
