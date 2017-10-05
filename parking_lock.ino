// Include libraries
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(8, 6, 5, 4, 3, 2);

const byte buttonPin1 = 11;
const byte buttonPin2 = 10;
const byte buttonPin3 = 9;

const byte ledPin1 = 12;
const byte ledPin2 = 7;

// Arrays of chars to compare. Total chars will be 6 if we count the null terminated chars.
char codeNumbers[6] = "1212";
char attemptNumbers[6];
byte numberPos; // To determine position in attemptNumbers array
bool lockOpened;
// Used for timer
unsigned long previousTime = millis();

Servo lock;

void setup()
{
  // Note the use of internal pull up resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  digitalWrite(ledPin1, HIGH); 
  
  lock.attach(A5);
  closeLock();
  
  lcd.begin(16, 2);
  lcd.print("Welcome back!");
  delay(1500);
  lcd.clear();
  lcd.print("Enter Password:");  
  
};

void loop()
{ 
  // Make numberPos between a value of 0 and 3 (which means the position won't '++' further than 4 digits)
  numberPos = constrain(numberPos, 0, 3);
  
  byte inputButtonState1 = digitalRead(buttonPin1);
  byte inputButtonState2 = digitalRead(buttonPin2);
  
  byte submitButtonState = digitalRead(buttonPin3);
  
  // Add number based on button press
  if(inputButtonState1 == LOW) addNumber('1');
  if(inputButtonState2 == LOW) addNumber('2'); 
  
  if(submitButtonState == LOW) 
  {
    lcd.clear();
    
    if(lockOpened)
    {
      lcd.print("Closing...");
      closeLock();
      delay(1500);
      setHome();
    }
    else 
    {
      processAttempt();
    }
    // Prevent multiple LOW reads
    delay(200);
  } 
  lcd.setCursor(0,1);
  lcd.print(attemptNumbers); // Print attempt numbers if enter button not pressed
  
  if(lockOpened) 
  {
    unsigned long currentTime = millis();
    unsigned long interval = 5000;
    
    if(currentTime - previousTime >= interval) 
    {
      closeLock();
      setHome();
    }
  }
};

void addNumber(char number)
{
  attemptNumbers[numberPos] = number;
    numberPos++;
    // Next char will be null terminator. 
    // This is to know where chars array (string) ends in case of checking and clearing
    attemptNumbers[numberPos] = '\0';
    // Delay to make sure to move position only once
    delay(250);
};

void clearAttempt() 
{
  numberPos = 0;
  // This makes the string terminate at the beginning of the chars array so it's "cleared"
  attemptNumbers[numberPos] = '\0';

};

void closeLock()
{
  lock.write(150);
  lockOpened = false;
  turnOnLed(ledPin1);
  turnOffLed(ledPin2);
}

void openLock() 
{
  lockOpened = true;
  lock.write(50);
  turnOnLed(ledPin2);
  turnOffLed(ledPin1);
}

void processAttempt() 
{
  if(strcmp (attemptNumbers,codeNumbers) == 0) 
  {
    lcd.print("Correct password!");
    blinkLed(ledPin2, 3);  
    lcd.clear();
    openLock();
    previousTime = millis();
  }
  else
  {
    lcd.print("Wrong password!");
    blinkLed(ledPin1, 3);
    setHome();
  }
  // Only clear attempt AFTER being checked
  clearAttempt();
}

void setHome()
{
  lcd.clear();
  lcd.print("Enter password:");
  lcd.setCursor(0,1);
}

void turnOnLed(byte led)
{
  digitalWrite(led, HIGH);
}

void turnOffLed(byte led)
{
  digitalWrite(led, LOW);
}

void blinkLed(byte led, int count) 
{
  switch (digitalRead(led))
  {
    case HIGH:
    for (int i=0; i < count; i++)
    {
      digitalWrite(led, LOW);   
      delay(500);                       
      digitalWrite(led, HIGH);    
      delay(500); 
    }
    break;
    case LOW:
    for (int i=0; i < count; i++) 
    {
      digitalWrite(led, HIGH);   
      delay(500);                       
      digitalWrite(led, LOW);    
      delay(500); 
    }
    break;
  }
}






