
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo servo1, servo2, servo3, servo4;

#define button1 9
#define button2 10
#define button3 11
#define button4 12
#define coinPin 2  // Change this to the actual pin connected to the CH-926 coin signal wire
#define FULL_ROTATION_DELAY 950  // Adjust this value based on your servo's speed

const int COIN_ACCEPTED = 5;
int buttonPressed;
volatile int coinInserted = 0;  // Initialize the coinInserted variable

void coinInsertedInterrupt() {
  coinInserted = 1;
}

void setup() {
  Serial.begin(9600); 
  Serial1.begin(9600);

  lcd.begin();
  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(7);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  pinMode(coinPin, INPUT);  // Set the coin pin as input
  attachInterrupt(digitalPinToInterrupt(coinPin), coinInsertedInterrupt, RISING);  // Attach an interrupt to handle coin insertion
}

void sendSignalToNodeMCU(const char* signal) {
  Serial.println(signal);
}

void sendCoinInsertedSignal() {
  sendSignalToNodeMCU("Coin Inserted");
}

void loop() {
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Insert a coin!");

  // Wait for a coin to be inserted
  while (!coinInserted) {
    // You can add additional code here to display waiting messages or perform other tasks
  }


    Serial1.write(highByte(COIN_ACCEPTED));
    Serial1.write(lowByte(COIN_ACCEPTED));

  // while (true) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select your item");
    lcd.setCursor(0, 1);
    lcd.print(" 1, 2, 3, or 4?");
    
    // Wait until a button is pressed
    while (true) {
      if (digitalRead(button1) == LOW) {
        buttonPressed = 1;
        break;
      }
      if (digitalRead(button2) == LOW) {
        buttonPressed = 2;
        break;
      }
      if (digitalRead(button3) == LOW) {
        buttonPressed = 3;
        break;
      }
      if (digitalRead(button4) == LOW) {
        buttonPressed = 4;
        break;
      }
    }
    
    sendCoinInsertedSignal();

    // Print "Delivering..." 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Delivering...");
    
    // Depending on the pressed button, move the carrier to that position and discharge the selected item 
    switch (buttonPressed) {
      case 1:
    // Rotate the helical coil, discharge the selected item
    for (int i = 0; i < 2; ++i) {
        servo1.writeMicroseconds(2000); // rotate
        delay(FULL_ROTATION_DELAY);
        servo1.writeMicroseconds(1500);  // stop
        delay(500);
    }
    sendSignalToNodeMCU("Button 1 Pressed");
    break;

case 2:
    // Rotate the helix, push the selected item
    for (int i = 0; i < 2; ++i) {
        servo2.writeMicroseconds(2000); // rotate
        delay(FULL_ROTATION_DELAY);
        servo2.writeMicroseconds(1500);  // stop
        delay(500);
    }
    sendSignalToNodeMCU("Button 2 Pressed");
    break;

case 3:
    // Rotate the helix, push the selected item
    for (int i = 0; i < 2; ++i) {
        servo3.writeMicroseconds(2000); // rotate
        delay(FULL_ROTATION_DELAY);
        servo3.writeMicroseconds(1500);  // stop
        delay(500);
    }
    sendSignalToNodeMCU("Button 3 Pressed");
    break;

case 4:
    // Rotate the helix, push the selected item
    for (int i = 0; i < 2; ++i) {
        servo4.writeMicroseconds(2000); // rotate
        delay(FULL_ROTATION_DELAY);
        servo4.writeMicroseconds(1500);  // stop
        delay(500);
    }
    sendSignalToNodeMCU("Button 4 Pressed");
    break;
    }
    
    
    lcd.clear(); // Clears the display
    lcd.setCursor(0, 0);
    lcd.print("Item delivered!"); // Prints on the LCD
    
    Serial1.write(highByte(buttonPressed));
    Serial1.write(lowByte(buttonPressed));
    buttonPressed = 0;
    // Wait for a few seconds before returning to the item selection screen
    delay(2000);
    // Reset the coinInserted flag
    coinInserted = 0;
    // break;
  // }
}
