// Служебные библиотеки
#include <SPI.h>
#include <Wire.h>

// Библиотеки для работы с OLED дисплеем
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Библиотека для работы с датчиком BMP
#include <Adafruit_BMP085.h>
// Библиотека для работы с часами реального времени
#include <iarduino_RTC.h>
// Библиотека для работы с датчиками DHT
#include <DHT.h>


// Объект управляющий датчиком BMP180
static Adafruit_BMP085 sensorBMP180;

// Объект управляющий датчиком DHT11
static DHT sensorDHT11(3, DHT11);

// Объект управляющий датчиком DHT21
static DHT sensorDHT21(4, DHT21);

// Объект управляющий OLED дисплеем
static Adafruit_SSD1306 oledDisplay(128, 64, &Wire, 4);

// Часы реального времени
static iarduino_RTC realTimeSensor(RTC_DS1302,11, 12, 13);


// Характеристики дисплея
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 
#define OLED_ADDR   0x3C
// Пины датчиков температуры
#define DHT11_PIN      3
#define DHT21_PIN      4


// Переменные для значений
float valueTempIn;
float valueTempOut;
float valueHumidity;
int valuePressure;



void setup() 
{
  Serial.begin(115200);

  // Инициализация дисплея
  oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oledDisplay.clearDisplay();
  oledDisplay.setTextSize(1);
  oledDisplay.setTextColor(WHITE);

  // Инициализация часов
  realTimeSensor.begin();
  
  // Установка времени, после первичной установки следует закомментировать
  realTimeSensor.settime(1, 35, 14, 29, 6, 21, 2);

  // Инициализация дисплея
  oledDisplay.display();

  // Иниицализация датчиков
  sensorDHT11.begin();
  sensorDHT21.begin();
  sensorBMP180.begin();
}



void loop()
{
  delay(800);

  
  oledDisplay.clearDisplay();
  oledDisplay.setCursor(0, 0);

  // Вывод времени
  oledDisplay.print(realTimeSensor.gettime("d-m-Y H:i:s"));


  // Отправка значений времени
  Serial.println(String(realTimeSensor.gettime("H:i:s")) +  "|" + String(realTimeSensor.gettime("d-m-Y")) +  "|" + String(realTimeSensor.gettime("D")));


  oledDisplay.setCursor(0, 16);

  // Температура внутри
  valueTempIn = sensorDHT11.readTemperature();
  oledDisplay.print("In:" + String(valueTempIn) + (char)247 + "  ");

  // Температура снаружи
  valueTempOut = sensorDHT21.readTemperature();
  oledDisplay.print("Out:" + String(valueTempOut) + (char)247);

  // Влажность
  valueHumidity = sensorDHT21.readHumidity();
  oledDisplay.setCursor(0, 32);
  oledDisplay.print("Humidity : " + String(valueHumidity) + '%');

  // Давление
  valuePressure = sensorBMP180.readPressure() / 133;
  oledDisplay.setCursor(0, 48);
  oledDisplay.print("Pressure : " + String(valuePressure) + "mmHg");


  // Отправка значений температуры, влажности и давления
  Serial.println( String(valueTempIn) + "|" + String(valueTempOut) + "|" + String(valueHumidity) + "|" + String(valuePressure));

  oledDisplay.display();


}
