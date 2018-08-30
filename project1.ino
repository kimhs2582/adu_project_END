#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#define DHT_TYPE DHT11

int i=0;
int pin_DHT=2;
int pin_DHT_VCC=3;
int pin_RECV_VCC=4;
int pin_RECV=5;
int pin_PEN_VCC=A0;
int pin_PEN_GND=A1;
int pin_LCD_CTRL[3] = {13, 12, 11};
int pin_LCD_DATA[8] = {10, 9, 8, 7};

DHT dht(pin_DHT, DHT_TYPE);
LiquidCrystal lcd(13, 12, 11, 10, 9, 8, 7);
IRrecv irrecv(pin_RECV);
decode_results results;

void setup() 
{
   dht.begin();
   pinMode(pin_DHT_VCC,OUTPUT);
   digitalWrite(pin_DHT_VCC,HIGH);
   pinMode(pin_RECV_VCC,OUTPUT);
   digitalWrite(pin_RECV_VCC,HIGH);
   pinMode(pin_PEN_VCC,OUTPUT);
   pinMode(pin_PEN_GND,OUTPUT);
   digitalWrite(pin_PEN_GND,LOW);
   lcd.begin(16,2);
   Serial.begin(9600);
   irrecv.enableIRIn();
}
void Lcd(float t,float h)
{
   lcd.print("Temp = ");
   lcd.print(t);
   lcd.setCursor(0,1);  
   lcd.print("Hum = ");
   lcd.print(h);
}
void Pen(float t,float h)
{
   if(irrecv.decode(&results)) 
   {
        i= ~i;
        irrecv.resume();
   }
   if(i==0)
   {
      if(t>24 || h>=60)
         digitalWrite(pin_PEN_VCC,HIGH);
      else 
         digitalWrite(pin_PEN_VCC,LOW);
   }
   if(i==-1)
         digitalWrite(pin_PEN_VCC,HIGH);   
}
void loop() 
{
  lcd.clear();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if(isnan(t) || isnan(h))
      return;
  Lcd(t,h); 
  delay(2000);
  Pen(t,h);  
  
}
    
