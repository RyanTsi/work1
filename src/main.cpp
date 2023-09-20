#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // 屏幕宽度
#define SCREEN_HEIGHT 64 // 屏幕高度

#define OLED_MOSI  12   // D1
#define OLED_CLK   14   // D0
#define OLED_DC    0
#define OLED_CS    4
#define OLED_RESET 2

#define DHTTYPE DHT11   // DHT 11 (AM2302)
#define DHTPIN 5        // DHT 引脚

// 温湿度传感器实例
DHT_Unified dht(DHTPIN, DHTTYPE);

// 创建 SSD1306 实例
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

uint32_t delayMS = 5000; // 循环时间

float Temperature, Humidity;                            // 当前环境的温湿度

// 初始化串口
void init_UART() {
    Serial.begin(115200);
}

// 初始化 SSD1306
void init_SSD1306() {
    if (!display.begin(SSD1306_SWITCHCAPVCC)) {
        Serial.println("failed");
        for (;;);
    } else {
        Serial.print("ssd1306 is ok");
    }
}

// 初始化温湿度传感器
void init_dht() {
    dht.begin();
    Serial.println(F("DHT11 is ok"));
}

// 打印温湿度传感器信息
void print_info_dht() {
    
// 打印温度传感器的信息
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F(" C"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F(" C"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F(" C"));
    Serial.println(F("------------------------------------"));
// 打印湿度传感器的信息
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));

}

void setup() {

// 一些初始化设置
    init_UART();
    init_SSD1306();
    init_dht();
    print_info_dht();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.clearDisplay();
}

// 得到温湿度值并打印到串口和屏幕
void get_temperature_humidity() {

// 得到温度并输出值
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        Serial.println(F("Error reading temperature!"));
    } else {
        Temperature = event.temperature;
        Serial.print(F("Temperature:  "));
        Serial.print(Temperature);
        Serial.println(F(" C\n"));
    }

// 得到湿度并输出值
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        Serial.println(F("Error reading humidity!"));
    } else {
        Humidity = event.relative_humidity;
        Serial.print(F("Humidity:     "));
        Serial.print(Humidity);
        Serial.println(F("%"));
    }

// ---------------oled 屏幕输出温湿度信息-------------
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print(F("Temperature: "));
    display.print(Temperature);
    display.println(F(" C\n"));
    display.print(F("Humidity:    "));
    display.print(Humidity);
    display.println(F(" %"));
    display.display();
// ---------------------------------------------------
}

void loop() {
    
// 得到当前环境的温湿度
    get_temperature_humidity();

    delay(delayMS);
}