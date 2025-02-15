
#include <TinyWireM.h>

#define TINY4KOLED_QUICK_BEGIN

#include <Tiny4kOLED.h>

const uint8_t snakeBody[] PROGMEM = {
    0x00,
    0x7e,
    0x7e,
    0x7e,
    0x7e,
    0x7e,
    0x7e,
    0x00,
};

byte snakeCords[128] = {
    0b00000001, 0b00000011, 0b00000101, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000

};

int temp = 0;
byte binaryInput;
byte snakeDirection = 1;
byte lastDigit;
byte firstPart;
byte secondPart;
byte byteWrite;
byte previousPosY;
byte previousPosX;
byte holdingPosY;
byte holdingPosX;
void setup()
{

  oled.begin(128, 64, sizeof(tiny4koled_init_128x64r), tiny4koled_init_128x64r);
  pinMode(3, OUTPUT);
  oled.clear();
  oled.setFont(FONT8X16);
  oled.setInverse(true);
  oled.on();

  oled.bitmap(firstPart * 8, secondPart, 8 + firstPart * 8, 1 + secondPart, snakeBody);
}

void loop()
{

  // oled.setCursor(40, 1);oled.print(strela);oled.setCursor(40, 1);oled.print("  ");

  temp = analogRead(A2);


  for (int i = 0; i < 128; i++)
  {
    firstPart = (snakeCords[i] >> 4) & 0x0F;
    secondPart = (snakeCords[i] >> 1) & 0x07; // Extract next 3 bits
    lastDigit = snakeCords[i] & 0x01;
    if (lastDigit == 0 || firstPart >= 15 || secondPart >= 7)
    {
      break;
    }
    else
    {
      holdingPosY = firstPart;
      holdingPosX = secondPart;
      if (i == 0)
      {
        switch (snakeDirection)
        {
        case 0:
          break;

        case 1:

          break;

        case 2:
          // Action for case 2
          break;

        case 3:
          // Action for case 3
          break;

        default:
          // Default action (optional)
          break;
        }
      }
      else
      {
        firstPart = previousPosY;
        secondPart = previousPosX;
      }
      previousPosY = holdingPosY;
      previousPosX = holdingPosX;
      byteWrite = (firstPart & 0x0F) << 4;
      byteWrite |= (secondPart & 0x07) << 1; // Store next 3 bits
      byteWrite |= (lastDigit & 0x01);       // Store last bit
      snakeCords[i] = byteWrite;             // Store first 4 bits
    }
  }

  for (int i = 0; i < 128; i++)
  {
    binaryInput = snakeCords[i];
    firstPart = (binaryInput >> 4) & 0x0F;  // Extract first 4 bits
    secondPart = (binaryInput >> 1) & 0x07; // Extract next 3 bits
    lastDigit = binaryInput & 0x01;
    oled.setFont(FONT6X8);
    oled.setCursor(40, 6);
    oled.print(secondPart);
    if (lastDigit == 0)
    {
      oled.setFont(FONT6X8);
      oled.setCursor(20, 6);
      oled.print(i);
      oled.setFont(FONT6X8);
      oled.setCursor(30, 6);
      oled.print(firstPart);
      break;
    }
    oled.setFont(FONT6X8);
    oled.setCursor(10, 6);
    oled.print(firstPart);
    oled.bitmap(firstPart * 8, secondPart, 8 + firstPart * 8, 1 + secondPart, snakeBody);
  }

  // LEVE TLACITKO
  if (temp > 830 && temp < 845)
  {
    snakeDirection = 1;
  }
  // LEVE TLACITKO

  if (temp > 505 && temp < 515)
  {
    snakeDirection = 1;
  }

  // STREDNI TLACITKO
  if (temp > 695 && temp < 705)
  {
    snakeDirection = 0;
  }
  delay(500);
  oled.clear();
}
