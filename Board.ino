#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins 
#define MOSI_PIN 30
#define SCLK_PIN 31
#define CS_PIN   32
#define DC_PIN   33
#define RST_PIN  34

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

//RGB565 Colors for 16bit color display
uint16_t colors[] = {0x0000,0x4228, 0x6B6D, 0xA534, 0xDEDB, 0xFFFF, 0xE800, 0xE381, 0xFD83, 0xFFE0, 0xBFE0, 0x67E0, 0x0400, 0x57ED, 0x57ED, 0x5DBE, 0x093F, 0x2813,  0x6013, 0xF85F, 0xF015, 0xF00D};
int colorIndex = 0, numColors = 22;

// Option 1: use any pins but a little slower
Adafruit_SSD1351 oled = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);  

int col[] = {8,9,10,11,12,13};
int row[] = {A0,A1,A2};
int c = 6, r =3;
String str = "";
//String chars[3][6] = {{"7", "8", "9", "BACK", "ENTER", "ADD"}, {"4","5","6","0","UP","SUBTRACT"}, {"1","2","3", "LEFT", "DOWN", "RIGHT"}};

String saveA = "";
String saveB = "";
int x = 0, y = 0;
boolean paintCur = true;
boolean initialized = false;
int rotB = A5, rotG = A6, rotR = A7, rotSW = 3;

void setup() {
  pinMode(7, INPUT);//Flight Switch
  pinMode(rotB, OUTPUT);//BLUE RGB
  pinMode(rotG, OUTPUT);//GREEN RGB
  pinMode(rotR, OUTPUT);//RED RGB
  pinMode(rotSW, INPUT);//Rotary Switch

  digitalWrite(7, HIGH);
  digitalWrite(rotSW, HIGH);

  for(int k =0; k< c; k++)
    pinMode(col[k], OUTPUT);
  for(int k =0; k< r; k++)
    pinMode(row[k], INPUT);
  Serial.begin(9600);
  oled.begin();
  oled.setRotation(2);
  x=64;
  y=64;
  oled.fillRect(0,0,128,128, BLACK);
  if(digitalRead(7) == LOW){
    oled.fillRect(0,0,128,128, CYAN);
    delay(50);
    oled.fillRect(0,0,128,128, RED);
    delay(50);
    oled.fillRect(0,0,128,128, BLACK);
    oled.setCursor(x/8,y);  
    oled.setTextSize(2);
    oled.setTextColor(CYAN);
    oled.println("Paintuino");
    colorIndex = 6;    
    delay(1500);
    oled.fillRect(0,0,128,128, BLACK);
    oled.fillRect(0,4,3,3,colors[colorIndex]); 
    oled.fillRect(0,0,3,3,GREEN); 
    initialized = true;
  }
}
void drawCross(int size, int color){
  oled.drawLine(x-size,y,x+size,y,color);
  oled.drawLine(x,y+size,x,y-size, color);
}


void loop() {
  if(digitalRead(7) == HIGH){
    oled.fillRect(0,0,128,128, BLACK);
    initialized = false;
  }
  else{
    if(digitalRead(7) == LOW && initialized == false){
      oled.fillRect(0,0,128,128, CYAN);
      delay(50);
      oled.fillRect(0,0,128,128, RED);
      delay(50);
      oled.fillRect(0,0,128,128, BLACK);
      oled.setCursor(x/8,y);  
      oled.setTextSize(2);
      oled.setTextColor(CYAN);
      oled.println("Paintuino");
      colorIndex = 6;    
      delay(1500);
      oled.fillRect(0,0,128,128, BLACK);
      oled.fillRect(0,0,3,3,GREEN);
      oled.fillRect(0,4,3,3,colors[colorIndex]);  
      initialized = true;
    }


    analogWrite(rotB, 200);
    analogWrite(rotG, 200);
    analogWrite(rotR, 200);
    Serial.println((String)"ROTSWITCH: " + digitalRead(rotSW));
    Serial.println((String)"BLUE: " + analogRead(A7));
    if(digitalRead(rotSW) == LOW)
      drawCross(2,colors[colorIndex]);
    delay(100);


    for(int k = 0; k < c; k++){
      digitalWrite(col[k], HIGH);
      int x0=x,y0=y;
      if(!paintCur){
        oled.drawLine(x,y,x,y,MAGENTA);
      }    
      if(k == 0 && analogRead(row[0]) > 275){//7          
        k7();
        //Serial.println((String) "RESISTOR 7: " + analogRead(row[0]));
      }
      else if(k == 1 && analogRead(row[0]) > 275){//8
        k8();        
        //Serial.println((String) "RESISTOR 8: " + analogRead(row[0]));
      }
      else if(k == 2 && analogRead(row[0]) > 275){//9
        k9();
        //Serial.println((String) "RESISTOR 9: " + analogRead(row[0]));
      }
      else if(k == 0 && analogRead(row[1]) > 275){//4
        k4();
        //Serial.println((String) "RESISTOR 4: " + analogRead(row[1]));
      }
      else if(k == 1 && analogRead(row[1]) > 275){//5
        k5();
        //Serial.println((String) "RESISTOR 5: " + analogRead(row[1]));
      }
      else if(k == 2 && analogRead(row[1]) > 275){//6      
        k6();
        //Serial.println((String) "RESISTOR 6: " + analogRead(row[1]));
      }
      else if(k == 0 && analogRead(row[2]) > 275){//1
        k1();
        //Serial.println((String) "RESISTOR 1: " + analogRead(row[2]));
      }
      else if(k == 1 && analogRead(row[2]) > 275){//2
        k2();
        //Serial.println((String) "RESISTOR 2: " + analogRead(row[2]));
      }
      else if(k == 2 && analogRead(row[2]) > 275){//3
        k3();
        //Serial.println((String) "RESISTOR 3: " + analogRead(row[2]));
      }
      
      if(k==3 && analogRead(row[0]) > 275){//clear screen and center cursor
          oled.fillRect(0,0,128,128,BLACK);
          x=64;y=64;
          if(paintCur){
            paintCur = false;
            oled.fillRect(0,0,3,3,RED);
          }
          else
          {
            paintCur = true;
            oled.fillRect(0,0,3,3,GREEN);
          }
          oled.fillRect(0,4,3,3,colors[colorIndex]);  
          delay(500);
      }
      if(k==3 && analogRead(row[1]) > 275){//Paint vs Move
          if(paintCur){
            paintCur = false;
            oled.fillRect(0,0,3,3,RED);
          }
          else
          {
            paintCur = true;
            oled.fillRect(0,0,3,3,GREEN);
          }   
          delay(500);     
      }
      if(k == 3 && analogRead(row[2] > 275)){//change color back
        colorIndex--;
        if(colorIndex < 0){
          colorIndex = numColors-1;
        }
        oled.fillRect(0,4,3,3,colors[colorIndex]);  
        delay(500);
      }
      if(k == 5 && analogRead(row[2] > 275)){//change color forward
        colorIndex++;
        if(colorIndex > numColors-1){
          colorIndex = 0;
        }
        oled.fillRect(0,4,3,3,colors[colorIndex]);  
        delay(500);
      }
      if(paintCur){
        oled.drawLine(x0,y0,x,y,colors[colorIndex]);      
      }
      else{
        oled.drawLine(x0,y0,x0,y0,BLACK);
      }
      digitalWrite(col[k], LOW);    
    }
  delay(50);
  }
}




void k7(){
  x-=1;
  y-=1;  
}
void k8(){
  y-=1;  
}
void k9(){
  x+=1;
  y-=1; 
}
void k4(){
  x-=1;
}
void k5(){
  drawCross(2,MAGENTA); 
}
void k6(){
  x+=1; 
}
void k1(){
  x-=1;
  y+=1;  
}
void k2(){
  y+=1;  
}
void k3(){
  x+=1;
  y+=1;  
}

