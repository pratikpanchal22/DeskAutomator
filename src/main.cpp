#include <Arduino.h>
#include <Wire.h>
#include "SSD1306.h"
#include <WiFiManager.h>   
#include <WiFi.h>
#include "time.h"

// onboard LED
const int LED_BUILTIN = 25;

// onboard OLED display
const int OLED_I2C_ADDRESS = 0x3C;
const int OLED_SDA = 4;
const int OLED_SCL = 15;
const int OLED_RST = 16;

const int DELAY_BETWEEN_DISPLAYS = 1500;

SSD1306 display(OLED_I2C_ADDRESS, OLED_SDA, OLED_SCL); 
int itr;

//time
const char* ntpServer1 = "time.google.com";
const char* ntpServer2 = "utcnist.colorado.edu";
const char* ntpServer3 = "pool.ntp.org";
/*
You need to adjust the UTC offset for your timezone in milliseconds. Refer the list of UTC time offsets. Here are some examples for different timezones:
For UTC -5.00 : -5 * 60 * 60 : -18000
For UTC +1.00 : 1 * 60 * 60 : 3600
For UTC +0.00 : 0 * 60 * 60 : 0
*/
const long  gmtOffset_sec = -25200;  //MST is UTC-07:00 = -7 * 60 * 60 = -25200

/*
Change the Daylight offset in milliseconds. If your country observes Daylight saving time set it to 3600. Otherwise, set it to 0.
*/
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

typedef void (*FuncPtr) (void);

void preDisplaySetup(){
  display.clear();
}

void attachFooterMs(){
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 54, String(millis()));
}

void sendBufferToDisplay(){
  attachFooterMs();
  // write the buffer to the display
  display.display();
  //show the display for some time
  delay(DELAY_BETWEEN_DISPLAYS);
}

void ledBlinkFunction(){
  int offset=10;
  for(int i=0; i<10; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay((i+1)*offset);
    digitalWrite(LED_BUILTIN, LOW);
    delay((i+1)*offset);
  }

  for(int i=9; i>0; i--){
    digitalWrite(LED_BUILTIN, HIGH);
    delay((i+1)*offset);
    digitalWrite(LED_BUILTIN, LOW);
    delay((i+1)*offset);
  }
}

void fontSizeDemoFunction(){
  preDisplaySetup();
  
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello world");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Hello world");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, "Hello world");

  sendBufferToDisplay();
}

void overflowTextDemoFunction(){
  preDisplaySetup();

  //drawTextFlowDemo
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(0, 0, 128,
                             "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );

  sendBufferToDisplay();
}

void textAlignmentDemoFunction(){
  preDisplaySetup();
  
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");

  sendBufferToDisplay();
}

void drawAStarAt(int x, int y){
  int xs, ys;
  int NUMBER_OF_PIXELS_90_DEG = 5;
  int NUMBER_OF_PIXELS_45_DEG = 4;

  //5 pixels N
  xs=x;
  ys=y;
  for(int i=0; i<NUMBER_OF_PIXELS_90_DEG; i++){
    display.setPixel(xs,ys-i);
  }

  //5 pixels NE
  xs=x;
  ys=y;
  for(int i=0; i<NUMBER_OF_PIXELS_45_DEG; i++){
    display.setPixel(xs+i,ys-i);
  }

  //5 pixels E
  xs=x;
  ys=y;
  for(int i=0; i<NUMBER_OF_PIXELS_90_DEG; i++){
    display.setPixel(xs+i,ys);
  }

  //5 pixels SE
  xs=x;
  ys=y;
  for(int i=0; i<4; i++){
    display.setPixel(xs+i,ys+i);
  }

  //5 pixels S
  xs=x;
  ys=y;
  for(int i=0; i<NUMBER_OF_PIXELS_90_DEG; i++){
    display.setPixel(xs,ys+i);
  }

  //5 pixels SW
  xs=x;
  ys=y;
  for(int i=0; i<4; i++){
    display.setPixel(xs-i,ys+i);
  }

  //5 pixels W
  xs=x;
  ys=y;
  for(int i=0; i<NUMBER_OF_PIXELS_90_DEG; i++){
    display.setPixel(xs-i,ys);
  }

  //5 pixels NW
  xs=x;
  ys=y;
  for(int i=0; i<4; i++){
    display.setPixel(xs-i,ys-i);
  }
}

void displayStars(){
  //number of screens that show stars
  int numberOfScreens = (int)random(4,13);
  for(int k=0; k<numberOfScreens; k++){
    preDisplaySetup();
    //number of stars in a screen
    int numberOfStars = (int)random(7,17);
    for(int i=0; i<numberOfStars; i++){
      drawAStarAt((int)random(0,128), (int)random(0,64));
    }

    char buff1[32];
    char buff2[16];
    sprintf(buff1, "THE UNIVERSE: %d of %d",k+1,numberOfScreens);
    sprintf(buff2, "%d STARS",numberOfStars);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 0, buff1);
    display.drawString(0, 54, buff2);
    sendBufferToDisplay();
  }
}

void objectsDemo(){
  preDisplaySetup();

  //drawRectDemo
  // Draw a pixel at given position
  for (int i = 0; i < 10; i++) {
    display.setPixel(i, i);
    display.setPixel(10 - i, i);
  }

  display.drawRect(12, 12, 20, 20);

  // Fill the rectangle
  display.fillRect(14, 14, 17, 17);

  // Draw a line horizontally
  display.drawHorizontalLine(0, 40, 20);

  // Draw a line horizontally
  display.drawVerticalLine(40, 0, 20);

  sendBufferToDisplay();
}

void swipeDisplayMessageDemo(){
  preDisplaySetup();

  /*
  screen size in pixels in 128 x 64
  center pixels are: (63,31) (64,32)
                     (63,31) (64,32)
  
    To draw a rectangle, let's of side 32 x 16,
    starting x value will be 63-16=47
    starting y value will be 31-8=23;

  */

  display.setColor(WHITE);
  int x=63,x1=64;
  while(x>32){
    display.setPixel(x,31);
    display.setPixel(x,32);
    display.setPixel(x1,31);
    display.setPixel(x1,32);
    display.display();
    delay(20);
    x--;
    x1++;
  }

  int xo=32, yo=32, wo=64, ho=0;
  while(xo>=0 || yo>=0){
    display.drawRect(xo, yo, wo, ho);
    display.display();
    delay(20);
    xo--;
    yo--;
    wo+=2;
    ho+=2;
  }

  x=63;
  x1=64;
  while(x>32){
    display.setColor(BLACK);
    display.setPixel(x,31);
    display.setPixel(x,32);
    display.setPixel(x1,31);
    display.setPixel(x1,32);

    display.display();
    delay(20);
    x--;
    x1++;
  }

  xo=32;
  yo=32; 
  wo=64;
  ho=0;
  while(xo>=0 || yo>=0){
    display.setColor(BLACK);
    display.drawRect(xo, yo, wo, ho);

    display.setColor(WHITE);
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(63, 2, "Desk");
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(63, 22, "Automator");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 52, "github.com/pratikpanchal22");

    display.display();
    delay(20);
    xo--;
    yo--;
    wo+=2;
    ho+=2;
  }

  display.setColor(WHITE);
  x=63;
  x1=64;
  while(x>32){
    display.setPixel(x,31);
    display.setPixel(x,32);
    display.setPixel(x1,31);
    display.setPixel(x1,32);
    display.display();
    delay(20);
    x--;
    x1++;
  }

  xo=32; 
  yo=32; 
  wo=64; 
  ho=0;
  while(xo>=0 || yo>=0){
    display.drawRect(xo, yo, wo, ho);
    display.display();
    delay(20);
    xo--;
    yo--;
    wo+=2;
    ho+=2;
  }

  x=63;
  x1=64;
  while(x>32){
    display.setColor(BLACK);
    display.setPixel(x,31);
    display.setPixel(x,32);
    display.setPixel(x1,31);
    display.setPixel(x1,32);

    display.display();
    delay(20);
    x--;
    x1++;
  }

  xo=32;
  yo=32; 
  wo=64;
  ho=0;
  while(xo>=0 || yo>=0){
    display.setColor(BLACK);
    display.drawRect(xo, yo, wo, ho);

    display.setColor(WHITE);
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(63, 2, "Controller");
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(63, 32, "State: Ready");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    //display.drawString(64, 52, "is a chatty baby");

    display.display();
    delay(20);
    xo--;
    yo--;
    wo+=2;
    ho+=2;
  }

  sendBufferToDisplay();
}

void circleDemo(){
  preDisplaySetup();

  //drawCircleDemo
  for (int i = 1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(96, 32, i * 3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(32, 32, 32-i * 3);
  }

  sendBufferToDisplay();
}

void staticProgressBarDemo(){
  preDisplaySetup();

  //drawProgressBarDemo
  int progress = itr % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");

  sendBufferToDisplay();
}

void animatedProgressBarDemo(){
  preDisplaySetup();

  //draw moving Progress Bar Demo
  int j=0;
  while(j++<100){
    preDisplaySetup();
    display.drawProgressBar(0, 32, 120, 10, j);
    attachFooterMs();
    display.display();
    delay(50);
  }

  // draw the percentage as String
  // display.setTextAlignment(TEXT_ALIGN_CENTER);
  // display.drawString(64, 15, String(progress) + "%");

  sendBufferToDisplay();
}

void initializerDemo(){
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Initializing");
  //draw moving Progress Bar
  int j=0;
  while(j++<100){
    display.drawProgressBar(0, 32, 120, 10, j);
    display.display();
    delay(15);
  }
}

void printTimeToDisplay(){
  preDisplaySetup();

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  char day_buff[32];
  strftime(day_buff, 31, "%a, %h %d %Y", &timeinfo);
  display.drawString(64, 8, day_buff);

  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  char day_buff1[16];
  strftime(day_buff1, 15, "%H:%M:%S", &timeinfo);
  display.drawString(64, 26, day_buff1);


  delay(2000);
  sendBufferToDisplay();
}

void reFetchTimeFromNtpServer(){

  preDisplaySetup();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 2, "Atomic Time");
  display.drawString(64, 16, "Servers");
  
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(6, 28, ntpServer1);
  display.drawString(6, 38, ntpServer2);
  display.drawString(6, 48, ntpServer3);

  sendBufferToDisplay();

}

#define NUMBER_OF_DEMO_FUNCTIONS 22
FuncPtr funcPtrs[NUMBER_OF_DEMO_FUNCTIONS] = {ledBlinkFunction,
                                              reFetchTimeFromNtpServer,
                                              printTimeToDisplay,
                                              printLocalTime,
                                              printTimeToDisplay,
                                              fontSizeDemoFunction,
                                              printTimeToDisplay,
                                              overflowTextDemoFunction,
                                              printTimeToDisplay,
                                              textAlignmentDemoFunction,
                                              printTimeToDisplay,
                                              displayStars,
                                              printTimeToDisplay,
                                              objectsDemo,
                                              printTimeToDisplay,
                                              swipeDisplayMessageDemo,
                                              printTimeToDisplay,
                                              staticProgressBarDemo,
                                              printTimeToDisplay,
                                              circleDemo,
                                              printTimeToDisplay,
                                              animatedProgressBarDemo};


#define MAXIMUM_LENGTH_OF_SSID                                     24

void setup() {

  Serial.begin(115200);

  //initialize LED
  pinMode(LED_BUILTIN, OUTPUT);

  //WiFiManager
	//Local intialization. Once its business is done, there is no need to keep it around
	WiFiManager wifiManager;
  char apSsidCharArray[MAXIMUM_LENGTH_OF_SSID];
  uint64_t macAddress = ESP.getEfuseMac();
  uint64_t macAddress_truncated = macAddress << 40;
  sprintf(apSsidCharArray, "%s %" PRIu64, "DeskAutomator", (macAddress_truncated>>40));
  wifiManager.autoConnect(apSsidCharArray);
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
  Serial.println(apSsidCharArray);
  yield();

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
  printLocalTime();
  yield();



  

  //initialize display
  pinMode(OLED_RST, OUTPUT);
  //reset display
  digitalWrite(OLED_RST, 0);
  //delay(100);
  //enable display
  digitalWrite(OLED_RST, 1);
  yield();

  
  display.init();
  initializerDemo();
}

void loop() {

  itr++;
  Serial.println(itr);

  if(itr%2==0){
    display.invertDisplay();
  }
  else {
    display.normalDisplay();
  }

  

  //Execute each demo function one at a time
  funcPtrs[itr%NUMBER_OF_DEMO_FUNCTIONS]();
}