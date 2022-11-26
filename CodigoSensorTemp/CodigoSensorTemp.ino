#include <Adafruit_I2CDevice.h>
#include <Adafruit_MLX90614.h>


Adafruit_MLX90614 sensor = Adafruit_MLX90614(); //declaracao do objeto sensor
double temp_obj = 0.0;

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor de Temperatura para monitoramento do ESC");

  sensor.begin(); //inicializando o sensor

}

void funcaoSensor() {  //realiza a leitura e imprime no monitor serial
  temp_obj = sensor.readObjectTempC();

  Serial.print("Temperatura do ESC = ");
  Serial.print(temp_obj);
  Serial.println("*C");
  delay(1000);
  
}

void loop() {
  funcaoSensor();
}