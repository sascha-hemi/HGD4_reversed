#include "main.h"

SparkFun_Ambient_Light light(AL_ADDR);

ENS210 ens210;
LPS22HBSensor lps22hb(&Wire);

Adafruit_LIS3DH acc1 = Adafruit_LIS3DH(ACCL1_CS);
Adafruit_LIS3DH acc2 = Adafruit_LIS3DH(ACCL2_CS);

void setup() {
  gpioinit();
}

void loop() {
  Serial.println();
  Serial.println("|---------Starting loop---------|");
  //powerupesp();
  //powerupmodem();
  measuretemp();
  measurepressure();
  lightsense();
  delay(1000);
}

void wd_handler() {
  digitalWrite(DONE,HIGH);
  delay(1);
  digitalWrite(DONE,LOW);
}


void gpioinit(){
  //latch on the main power
  pinMode(PWR_LATCH , OUTPUT);
  digitalWrite(PWR_LATCH , HIGH);
  //setup watchdog feeding
  pinMode(WAKE, INPUT);
  pinMode(DONE, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(WAKE), wd_handler, RISING);
  //setup Serial
  Serial.begin(115200);
  //setup I2C bus
  Wire.begin();
  //set i2c pins 
  pinMode(I2C_SDA, OUTPUT);
  //needed for some boards
  wd_handler();
  //set up leds
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  digitalWrite(RED_LED,HIGH);
  digitalWrite(GREEN_LED,HIGH);
  //set all other pins
  pinMode(MODEM_ESP_PWR,OUTPUT);
  digitalWrite(MODEM_ESP_PWR,LOW);
  pinMode(ESP_PWR,OUTPUT);
  digitalWrite(ESP_PWR,LOW);
  pinMode(MODEM_PWRKEY,OUTPUT);
  digitalWrite(MODEM_PWRKEY,LOW);
}

void powerupesp(){
  Serial.println("Powering up esp");
  digitalWrite(MODEM_ESP_PWR,HIGH);
  digitalWrite(ESP_PWR,HIGH);
  delay(500);
}

void powerupmodem(){
  Serial.println("powering up modem");
  digitalWrite(MODEM_ESP_PWR,HIGH);
  delay(600);
  digitalWrite(MODEM_PWRKEY,HIGH);
  delay(600);
  digitalWrite(MODEM_PWRKEY,LOW);
  delay(600);
}

void lightsense(){
float gain = .25;
int time = 100;
long luxVal = 0; 
  if(!light.begin())
    Serial.println("Could not communicate with the light sensor!");
  light.setGain(gain);
  light.setIntegTime(time);

  Serial.println("|-----------------------------|");
  Serial.println("| Sensor: VEML6035            |");
  Serial.println("| Gain    | Integration Time  |");
  Serial.print("| ");
  Serial.print(gain, 3);
  Serial.print("   | ");
  Serial.print(time);
  Serial.println("               |");
  
  luxVal = light.readLight();
  
  Serial.println("|-----------------------------|");
  Serial.println("| Ambient Light Reading       |");
  Serial.print("| ");
  Serial.print(luxVal);
  Serial.println(" Lux");
  Serial.println("|-----------------------------|");  
}

void measuretemp(){
  ens210.begin();
  int t_data, t_status, h_data, h_status;
  ens210.measure(&t_data, &t_status, &h_data, &h_status );

  Serial.println("|-------------------------------|");
  Serial.println("| Sensor: ENS210                |");
  Serial.println("| Temperature  | Humidity       |");
  Serial.print("| ");
  Serial.print(ens210.toCelsius(t_data, 10) / 10.0, 1);
  Serial.print(" C       | ");
  Serial.print(ens210.toPercentageH(h_data, 1));
  Serial.println(" %RH         |");
  Serial.println("|-------------------------------|");
}

void measurepressure(){
  lps22hb.begin();
  lps22hb.Enable();
  float pressure, temperature;
  lps22hb.GetPressure(&pressure);
  lps22hb.GetTemperature(&temperature);

  Serial.println("|----------------------------------|");
  Serial.println("| Sensor: LPS22HB                  |");
  Serial.println("| Pressure[hPa]  | Temperature[C]  |");
  Serial.print("| ");
  Serial.print(pressure, 2);
  Serial.print("        | ");
  Serial.print(temperature, 2);
  Serial.println("           |");
  Serial.println("|----------------------------------|");
}