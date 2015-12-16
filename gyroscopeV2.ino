#include <Wire.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23

int Addr = 105;                 // I2C address of gyro
int x, y, z;
boolean Horizontal = false;
boolean Vertical = false;
boolean Swingforward = false;


void setup() {
  Wire.begin();
  // Serial.begin(9600);
  writeI2C(CTRL_REG1, 0x1F);    // Turn on all axes, disable power down
  writeI2C(CTRL_REG3, 0x08);    // Enable control ready signal
  writeI2C(CTRL_REG4, 0x80);    // Set scale (500 deg/sec)
  delay(100);                   // Wait to synchronize
  //Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  //lcd.print("test");
  getGyroValues();              // Get new values
  x = x / 114;
  y = y / 114;
  z = z / 114;

  // In following Dividing by 114 reduces noise
  lcd.setCursor(0, 0);
  lcd.print("X:");
  lcd.setCursor(2, 0);
  lcd.print(x);
  lcd.setCursor(6, 0);
  lcd.print("Z:");
  lcd.setCursor(8, 0);
  lcd.print(z);


  if (x > 100) {
    Vertical = true;
    Horizontal = false;
    Swingforward =false;
    lcd.setCursor(0, 1);
    lcd.print("Vertical");
  }
  if (x < -100) {
    Vertical = false;
    Horizontal = true;
    lcd.setCursor(0, 1);
    lcd.print("Horizontal");
  }

  if (z > 100) {
    Swingforward = true;
  }

  if (z < -100) {
    Swingforward = false;
  }

  if (Horizontal == true && Swingforward == true) {
    lcd.setCursor(0, 1);
    lcd.print("Swing");
    delay(1000);
  }


  delay(250);
  lcd.clear();
}
void getGyroValues () {
  byte MSB, LSB;

  MSB = readI2C(0x29);
  LSB = readI2C(0x28);
  x = ((MSB << 8) | LSB);

  MSB = readI2C(0x2B);
  LSB = readI2C(0x2A);
  y = ((MSB << 8) | LSB);

  MSB = readI2C(0x2D);
  LSB = readI2C(0x2C);
  z = ((MSB << 8) | LSB);
}

int readI2C (byte regAddr) {
  Wire.beginTransmission(Addr);
  Wire.write(regAddr);                // Register address to read
  Wire.endTransmission();             // Terminate request
  Wire.requestFrom(Addr, 1);          // Read a byte
  while (!Wire.available()) { };      // Wait for receipt
  return (Wire.read());               // Get result
}

void writeI2C (byte regAddr, byte val) {
  Wire.beginTransmission(Addr);
  Wire.write(regAddr);
  Wire.write(val);
  Wire.endTransmission();


}
