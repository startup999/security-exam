Project programming:
#include "esp_camera.h"
#include <stdio.h>
#include <WiFi.h>
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module, // or another board which has PSRAM enabled
// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_M5STACK_PSRAM
#define CAMERA_MODEL_AI_THINKER
const char* ssid = "DESKTOP-PUN0C8C 7689";
const char* password = "12345678";
//const char* ssid = "DESKTOP-HL2UTFG 6328";
//const char* password = "12345678";
int FACE_ID;
int face_id;
int y;
String student, student1, student2;
const char *server = "maker.ifttt.com";
const char* resource = "/trigger/attendance/with/key/bmZdv6VCLeFYFg0YjRYYpr"; #if defined(CAMERA_MODEL_WROVER_KIT)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1 #define XCLK_GPIO_NUM 21
#define SIOD_GPIO_NUM
#define SIOC_GPIO_NUM
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 19
#define Y4_GPIO_NUM 18
#define Y3_GPIO_NUM 5
#define Y2_GPIO_NUM 4
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22
#elif defined(CAMERA_MODEL_M5STACK_PSRAM) #define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM 15
#define XCLK_GPIO_NUM #define SIOD_GPIO_NUM #define SIOC_GPIO_NUM #define Y9_GPIO_NUM 19 #define Y8_GPIO_NUM 36
#define Y7_GPIO_NUM 18
#define Y6_GPIO_NUM 39
#define Y5_GPIO_NUM 5
#define Y4_GPIO_NUM 34
#define Y3_GPIO_NUM 35
#define Y2_GPIO_NUM 32
#define VSYNC_GPIO_NUM 22
#define HREF_GPIO_NUM 26
#define PCLK_GPIO_NUM 21
#elif defined(CAMERA_MODEL_AI_THINKER) #define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
0 26 27
#define XCLK_GPIO_NUM #define SIOD_GPIO_NUM #define SIOC_GPIO_NUM #define Y9_GPIO_NUM 35 #define Y8_GPIO_NUM 34 #define Y7_GPIO_NUM 39 #define Y6_GPIO_NUM 36 #define Y5_GPIO_NUM 21 #define Y4_GPIO_NUM 19 #define Y3_GPIO_NUM 18 #define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25 #define HREF_GPIO_NUM 23 #define PCLK_GPIO_NUM 22 #else
#error "Camera model not selected" #endif
void startCameraServer();
void setup() {
Serial.begin(115200); Serial.setDebugOutput(true);
Serial.println();
camera_config_t config;
config.ledc_channel = LEDC_CHANNEL_0; config.ledc_timer = LEDC_TIMER_0; config.pin_d0 = Y2_GPIO_NUM; config.pin_d1 = Y3_GPIO_NUM; config.pin_d2 = Y4_GPIO_NUM; config.pin_d3 = Y5_GPIO_NUM; config.pin_d4 = Y6_GPIO_NUM; config.pin_d5 = Y7_GPIO_NUM; config.pin_d6 = Y8_GPIO_NUM; config.pin_d7 = Y9_GPIO_NUM; config.pin_xclk = XCLK_GPIO_NUM;
config.pin_pclk = PCLK_GPIO_NUM; config.pin_vsync = VSYNC_GPIO_NUM; config.pin_href = HREF_GPIO_NUM; config.pin_sscb_sda = SIOD_GPIO_NUM; config.pin_sscb_scl = SIOC_GPIO_NUM; config.pin_pwdn = PWDN_GPIO_NUM; config.pin_reset = RESET_GPIO_NUM; config.xclk_freq_hz = 20000000; config.pixel_format = PIXFORMAT_JPEG; //init with high specs to pre-allocate larger buffers if(psramFound()){
config.frame_size = FRAMESIZE_UXGA; config.jpeg_quality = 10;
config.fb_count = 2;
} else {
config.frame_size = FRAMESIZE_SVGA; config.jpeg_quality = 12;
config.fb_count = 1;
}
// camera init
esp_err_t err = esp_camera_init(&config); if (err != ESP_OK) {
Serial.printf("Camera init failed with error 0x%x", err);
return; }
//drop down frame size for higher initial frame rate sensor_t * s = esp_camera_sensor_get(); s->set_framesize(s, FRAMESIZE_QVGA); WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) { delay(500);
Serial.print(".");
}
Serial.println(""); Serial.println("WiFi connected");
// while (WiFi.status() == WL_CONNECTED) {
// digitalWrite(WiFi_led, HIGH);
// Serial.print("LED High"); // }
startCameraServer();
Serial.print("Camera Ready! Use 'http://"); Serial.print(WiFi.localIP()); Serial.println("' to connect");
}
void loop() {
// // put your main code here, to run repeatedly:
Serial.print("face iddddddddd:'%d'\n "); Serial.print(y);
if (y == 0){ student = "nobody";
makeIFTTTRequest();
delay(10000); }
if (y == 1){
student = "Amna"; makeIFTTTRequest(); delay(10000);
}
if (y == 2){
student = "Alanood"; makeIFTTTRequest(); delay(10000);
}
if (y == 3){
student = "Fidaa"; makeIFTTTRequest(); delay(10000);
} }
void makeIFTTTRequest() { Serial.print("Connecting to "); Serial.print(server); WiFiClient client;
int retries = 5;
while(!!!client.connect(server, 80) && (retries-- > 0)) {
Serial.print("."); }
Serial.println(); if(!!!client.connected()) {
Serial.println("Failed to connect..."); }
Serial.print("Request resource: "); Serial.println(resource);
String jsonObject = String("{\"value1\":\"") + student + "\"}";
//String jsonObject = String("{\"value1\":\"") + student + "\",\"value2\":\"" + student1
+ "\",\"value3\":\"" + student2 + "\"}";
// Comment the previous line and uncomment the next line to publish temperature readings in Fahrenheit
/*String jsonObject = String("{\"value1\":\"") + (1.8 * bme.readTemperature() + 32) + "\",\"value2\":\""
"\"}";*/
+ (bme.readPressure()/100.0F) + "\",\"value3\":\"" + bme.readHumidity() +
client.println(String("POST ") + resource + " HTTP/1.1"); client.println(String("Host: ") + server);
client.println("Connection: close\r\nContent-Type: application/json"); client.print("Content-Length: ");
client.println(jsonObject.length()); client.println();
client.println(jsonObject);
int timeout = 5 * 10; // 5 seconds while(!!!client.available() && (timeout-- > 0)){
delay(100); }
if(!!!client.available()) { Serial.println("No response...");
} while(client.available()){
Serial.write(client.read()); }
Serial.println("\nclosing connection");
client.stop(); }
