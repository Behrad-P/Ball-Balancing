
/**
 * @author  Behrad Pourhadi
 * @email   behradp32@gmail.com
 * @ide     Arduino IDE (2.1.0)
 * @brief   Ball_Balancing (.ino) 
 **
 * @note    An external power supply is required to power the servo motors
 **
 * @connections   pin_9 => x-axis servo motor, pin_10 => y-axis servo motor 
 *                PIN_4 => up-right LED, PIN_5 => up-left LED, PIN_6 => down-right LED, PIN_7 => down-left LED,
*/

#include <Servo.h>

const String LED_OFF_STR = "ledoff";
const int CT_POINT = -1;
const char SEP_CHAR = ',';
const char CMD_NUM = 2;
const char HIGH_CHAR = 'h';
const char LOW_CHAR = 'l';

Servo hServo, vServo;
String inputStr;
int servoValues[2] = {90, 90};
int count = 0;
int setPointLed;

void ledSetupBlink(int blinkDelay, int numOfBlink);
void ledIdleBlink(int blinkDelay);
bool processReceivedCommand(String str);

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(20);

  hServo.attach(9);
  vServo.attach(10);

  hServo.write(90);
  vServo.write(90);

  pinMode(PIN4, OUTPUT);
  pinMode(PIN5, OUTPUT);
  pinMode(PIN6, OUTPUT);
  pinMode(PIN7, OUTPUT);

  ledSetupBlink(250, 3);
}

void loop() {
  
  while(Serial.available() == 0)
  {
    if(count >= 2000)
    {
      hServo.write(90);
      vServo.write(90);
      ledIdleBlink(250);
    }
    else
      count++;

    delay(1);
  }
  count = 0;

  inputStr = Serial.readString(); 

  if(processReceivedCommand(inputStr) == false)
    Serial.println("Can not get values!");

  else if((servoValues[0] > 180 || servoValues[0] < 0) || (servoValues[1] > 180 || servoValues[1] < 0))
    Serial.println("Out of range!"); 

  else
  {
    hServo.write(servoValues[0]);
    vServo.write(servoValues[1]);
    Serial.println("Ok");
  }
}

bool processReceivedCommand(String str)
{
  String temp = "";
  int i = 0;
  int servoIndex = 0;
  int cmdIt = 0;
  bool pinState = 0;

  str += SEP_CHAR;
  str += '\0';

  while(str[i] != '\0')
  {
    while(str[i] != SEP_CHAR)
    {
      temp += str[i];
      i++;
    }

    cmdIt++;

    if(temp == LED_OFF_STR)
    {
      PORTD = 0x00;
      setPointLed = CT_POINT;
      return true; 
    }  

    if(temp[0] == HIGH_CHAR || temp[0] == LOW_CHAR)
    {
      if(str[i + 1] < 52 || str[i + 1] > 55)
        return false;

      if(temp[0] == HIGH_CHAR)
        pinState = 1;

      setPointLed = str[i + 1] - '0';
      digitalWrite(setPointLed, pinState);

      return true;
    }

    if(!isNum(temp))
      return false;
    
    servoValues[servoIndex] = temp.toInt();
    servoIndex++;
    i++;
    temp = "";
  }
  
  if(cmdIt == CMD_NUM)
    return true;
  else
    return false;
}

bool isNum(const String& numStr)
{
  if(numStr == "")
      return false;

  for(char ch : numStr)
  {
      if (!isDigit(ch))
          return false;
  }
  return true;
}

void ledSetupBlink(int blinkDelay, int numOfBlink)
{
  int count = 0;

  while(count++ < numOfBlink)
  {
    PORTD = 0xF0;
    delay(blinkDelay);
    PORTD = 0x00;
    delay(blinkDelay);
  }
}

void ledIdleBlink(int blinkDelay)
{
  int i = 0;

  while(i < 4)
  {
    PORTD = 1 << i + 4;
    delay(blinkDelay); 
    i++;
  }
  if(Serial.available() > 0)
    if(setPointLed != CT_POINT)
      PORTD = 1 << setPointLed;
    else
      PORTD = 0x00;
}









