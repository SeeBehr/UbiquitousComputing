#include <Arduino_LSM6DSOX.h>
#include <WiFiNINA.h>


void setup() {
  // Set the RGB LED pins as OUTPUT
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  // Turn off all LED colors at the beginning
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
  // Initialize serial communication at 9600 baud rate
  Serial.begin(1200);
  while (!Serial) {
    ;
  }

  // Initialize IMU sensor
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU initialized.");

}

void loop() {
  int temperature = 0;
  // Read the temperature from the IMU sensor
  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(temperature);

    // Print the temperature to the serial monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Control RGB LED based on the temperature
    if (temperature > 32) {
      analogWrite(LEDR, 255);
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 0);
    } else if (temperature >= 20 && temperature <= 32) {
      analogWrite(LEDR, 0);
      analogWrite(LEDG, 255);
      analogWrite(LEDB, 0);
    } else if (temperature < 25) {
      analogWrite(LEDR, 0);
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 255);
    }
  }

  // Wait for 1 second before taking another reading
  delay(1000);
}