#include <MPU6050.h>
#include <Wire.h>
#include <bmp085.h>

MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {

  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  float temp, pres, height;
  Wire.begin();
  bmp085Init(0);
  delay(100);
  bmp085Measure(&temp, &pres, &height);
  delay(100);
  bmp085Init(pres);
  Serial.begin(9600);
  mpu.initialize();
  mpu.setFullScaleAccelRange(3);
  mpu.setFullScaleGyroRange(3);
  mpu.setXGyroOffset(60.5);
  mpu.setYGyroOffset(-3.99);
  mpu.setZGyroOffset(3);
}

int16_t i=0;

float agx,agy,agz;

void loop() {

  // *** OBS OBS OBS *** CHANGE A0 BELOW TO A6 WHEN ON THE REAL FLIGHT COMPUTER!!! *** OBS OBS OBS ***
  
  int vbat = analogRead(A0)*9.765625; // value (0-1023) x 5 volts / 1000 millivolts x 2 (because of the even voltage divider)
  int vcc    = readVcc();                 // Read 5V supply voltage after regulator

  float temp, pres, height;
  bmp085Measure(&temp, &pres, &height);
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  float acc_scale=2048.0;
  float gyr_scale=16.4;
  
  String output = "";
  
  output += i;
  output += ",";
  output += millis();
  output += ",";
  output += vbat;
  output += ",";
  output += vcc;
  output += ",";
  output += temp;
  output += ",";
  output += long(pres);
  output += ",";
  output += height;
  output += ",";
  output += ax/acc_scale;
  output += ",";
  output += ay/acc_scale;
  output += ",";
  output += az/acc_scale;
  output += ",";
  output += gx/gyr_scale;
  output += ",";
  output += gy/gyr_scale;
  output += ",";
  output += gz/gyr_scale;
  Serial.println(output);
  i++;
}