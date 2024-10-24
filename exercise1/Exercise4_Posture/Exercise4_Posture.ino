#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>
#include <MadgwickAHRS.h>

Madgwick filter;
float accelScale, gyroScale;
int counter;

void setup()
{
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  Serial.begin(1200);

  while (!Serial)
    ;

  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.println();

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println("Hz");
  Serial.println();
}

void loop()
{
  float aix, aiy, aiz;
  float gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;
  int temperature;
  bool alert = false;

  // read raw data from IMU
  IMU.readAcceleration(aix, aiy, aiz);
  IMU.readGyroscope(gix, giy, giz);

  // convert from raw data to gravity and degrees/second units
  ax = convertRawAcceleration(aix);
  ay = convertRawAcceleration(aiy);
  az = convertRawAcceleration(aiz);
  gx = convertRawGyro(gix);
  gy = convertRawGyro(giy);
  gz = convertRawGyro(giz);

  // update the filter, which computes orientation
  filter.updateIMU(gx, gy, gz, ax, ay, az);

  // Read the temperature from the IMU sensor
  if (IMU.temperatureAvailable())
  {
    IMU.readTemperature(temperature);
  }

  // Control RGB LED based on the temperature
  if (temperature < 20 || temperature > 32)
  {
    alert = true;
  }

  // print the heading, pitch and roll
  roll = filter.getRoll();
  pitch = filter.getPitch();
  heading = filter.getYaw();
  if (counter % 500 == 0)
  {
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
    // Print the temperature to the serial monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    counter = 0;
  }
  if (pitch < -100 || pitch > -80)
  {
    alert = true;
  }

  if (alert)
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
  }
  else
  {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
  }
  counter++;
}

float convertRawAcceleration(int aRaw)
{
  // since we are using 2 g range
  // -2 g maps to a raw value of -32768
  // +2 g maps to a raw value of 32767

  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw)
{
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767

  float g = (gRaw * 250.0) / 32768.0;
  return g;
}