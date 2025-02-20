
#include <TinyWireM.h>

#define TINY4KOLED_QUICK_BEGIN

#include <Tiny4kOLED.h>

const uint8_t snakeBody[] PROGMEM = {
    0x00,
    0x3C,
    0x7e,
    0x7e,
    0x7e,
    0x7e,
    0x7e,
    0x3C,
};
const uint8_t blank[] PROGMEM = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};
const uint8_t apple[] PROGMEM = {
    0x00,
    0x00,
    0x3C,
    0x3C,
    0x3C,
    0x3C,
    0x00,
    0x00,
};
// First four bytes are for Y coordinate, next three are for the X coordinate and the last bit of information is for checking if there is another part of the body
byte snakeCords[128] = {
    0b00000101, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
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
byte snakeDirection = 0;
byte lastDigit;
byte firstPart;
byte secondPart;
byte byteWrite;
byte previousPosY;
byte previousPosX;
byte holdingPosY;
byte holdingPosX;
byte running = 1;
byte wantDirection = 0;
bool pressingButtons = 0;
long previousMillis = 0;
byte appleCords[3] = {4, 6, 2};
void setup()
{

  oled.begin(128, 64, sizeof(tiny4koled_init_128x64r), tiny4koled_init_128x64r);
  pinMode(3, OUTPUT);
  oled.clear();
  //oled.setFont(FONT8X16);
  //oled.setInverse(false);
  oled.on();

  // Show apple to the screen
  oled.bitmap(appleCords[0] * 8, appleCords[1], 8 + appleCords[0] * 8, 1 + appleCords[1], apple);
}

void loop()
{
  
  // Code for Debugging
  //  oled.setFont(FONT8X16);oled.setCursor(40, 1);oled.print(strela);oled.setCursor(40, 1);oled.print("  ");
  //oled.setCursor(40, 1);oled.print(previousMillis);
  temp = analogRead(A2);
  
  // Moving the snake and rotating it
  for (int i = 0; i < 128 && running == 1; i++)
  {
    // Extracting the coordinates from the snakeCords array
    firstPart = (snakeCords[i] >> 4) & 0x0F;
    secondPart = (snakeCords[i] >> 1) & 0x07;
    lastDigit = snakeCords[i] & 0x01;
    // check if the head is within boundaries
    if (firstPart >= 15 || secondPart >= 7 || lastDigit == 0)
    {
      break;
    }
    else
    {
      holdingPosY = firstPart;
      holdingPosX = secondPart;
      if (i == 0)
      {
        // Rotating the snake
        switch (snakeDirection)
        {
        case 0:
          firstPart++;
          break;

        case 1:
          secondPart++;
          break;

        case 2:
          firstPart--;
          break;

        case 3:
          secondPart--;
          break;

        default:
          break;
        }
      }
      else
      {
        firstPart = previousPosY;
        secondPart = previousPosX;
      }
      // Writing moved body to the array
      previousPosY = holdingPosY;
      previousPosX = holdingPosX;
      byteWrite = (firstPart & 0x0F) << 4;
      byteWrite |= (secondPart & 0x07) << 1; // Store next 3 bits
      byteWrite |= (lastDigit & 0x01);       // Store last bit
      snakeCords[i] = byteWrite;             // Store first 4 bits
    }
  }

  // Displaying the head on the screen 
  binaryInput = snakeCords[0];
  firstPart = (binaryInput >> 4) & 0x0F;  // Extract first 4 bits
  secondPart = (binaryInput >> 1) & 0x07; // Extract next 3 bits
  lastDigit = binaryInput & 0x01;
  oled.bitmap(firstPart * 8, secondPart, 8 + firstPart * 8, 1 + secondPart, snakeBody);

  // Removing last part of the snake
  oled.bitmap(holdingPosY * 8, holdingPosX, 8 + holdingPosY * 8, 1 + holdingPosX, blank);

  // Checking if the head coordinates are matching apple coordinates
  // If true generate new coorinates and add a body paart to the snake
  if (firstPart == appleCords[0] && secondPart == appleCords[1])
  {
    byteWrite = (holdingPosY & 0x0F) << 4;
    byteWrite |= (holdingPosX & 0x07) << 1;
    byteWrite |= (1 & 0x01);
    snakeCords[appleCords[2]] = byteWrite;
    appleCords[0] = random(0, 15);
    appleCords[1] = random(0, 7);
    appleCords[2]++;
  }
    oled.bitmap(appleCords[0] * 8, appleCords[1], 8 + appleCords[0] * 8, 1 + appleCords[1], apple);
  // LEVE TLACITKO
  if (temp > 830 && temp < 845 && pressingButtons == 0)
  {
    wantDirection = 1;
    
  }else{
    
  }

  // PRAVE TLACITKO
  if (temp > 505 && temp < 515 && pressingButtons == 0)
  {
    wantDirection = 2;
    
  }

   if ((temp > 505 && temp < 515) || (temp > 830 && temp < 845))
  {
    pressingButtons = 1;
  }else{
    pressingButtons = 0;
  }

  // STREDNI TLACITKO
  if (temp > 695 && temp < 705)
  {
  }

  if (millis() - previousMillis >= 500)
  {
    if (wantDirection == 1)
    {
      if (snakeDirection == 0)
      {

        
          snakeDirection = 3;
        
      }
      else
      {
        snakeDirection--;
      }
    }
    if (wantDirection == 2)
    {

      if (snakeDirection == 3)
      {
        snakeDirection = 0;
      }
      else
      {
        snakeDirection++;
      }
    }
    wantDirection = 0;
    previousMillis = millis();
    running = 1;
    
  }
  else
  {
    running = 0;
  }
}
