#include <Q2HX711.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

int potset = 12;
int zero = 13;
float zerosize = 0;
int state = 1;




const byte hx711_data_pin = 5;
const byte hx711_clock_pin = 4;

float y1 = 20.0; // calibrated mass to be added
long x1 = 0L;
long x0 = 0L;
float avg_size = 10.0; // amount of averages for each mass measurement

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin); // prep hx711




void setup() {
  Serial.begin(9600); // prepare serial port
  delay(1000); // allow load cell and hx711 to settle
  pinMode(potset, INPUT);
  pinMode(zero, INPUT);

  
  
  
  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // tare procedure
  for (int ii=0;ii<int(avg_size);ii++){
    delay(10);
    x0+=hx711.read();
  }
  x0/=long(avg_size);
  //Serial.println("Add Calibrated Mass");
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Add Calibrated ");
  lcd.setCursor(1,1);
  lcd.print("     Mass");
  
  // calibration procedure (mass should be added equal to y1)
  int ii = 1;
  while(true){
    if (hx711.read()<x0+10000){
    } else {
      ii++;
      delay(2000);
      for (int jj=0;jj<int(avg_size);jj++){
        x1+=hx711.read();
      }
      x1/=long(avg_size);
      break;
    }
  }
  lcd.clear();
  lcd.print("      Done");
}


void loop() {
   
   
   int potstate = digitalRead(potset);
   
   
   
   
     
   
   if (potstate == HIGH)
   {
   
   float sensorValue = analogRead(A0);
   float setpoint = sensorValue / 8;
   //Serial.println(setpoint,2);
  
  
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("  ");
  lcd.print("Set Grains");
  lcd.print("                ");
  lcd.setCursor(1,1);
  lcd.print("     ");
  lcd.print(setpoint,1);
  lcd.print("                ");
  

   
   }
   
   
   
   
   if (potstate == LOW)
{
      
   
   
   int sensorValue = analogRead(A0);
   float setpoint = sensorValue / 10;
   //Serial.println(setpoint,2);
     
  // averaging reading
  long reading = 0;
  for (int jj=0;jj<int(avg_size);jj++){
    reading+=hx711.read();
  }
  reading/=long(avg_size);
  // calculating mass based on calibration and linear fit
  float ratio_1 = (float) (reading-x0);
  float ratio_2 = (float) (x1-x0);
  float ratio = ratio_1/ratio_2;
  float mass = y1*ratio;

int zerostate = digitalRead(zero);
  
    if (zerostate == HIGH)
  {
    zerosize = (mass);
    
    
  }
  
float zeromass = mass - zerosize;
String szeromass = String(zeromass);
float fin = szeromass.toFloat();
float grain = fin * 15.43;
Serial.println(grain);
  

  
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Grams : ");
  lcd.print(zeromass,2);
  lcd.print("     ");
  lcd.setCursor(1,1);
  lcd.print("Grains: ");
  lcd.print(grain,2);
  lcd.print("        ");

  int eighty = mass * 0.8;
  int ninety = mass * 0.9;
  
  if (grain < eighty)
  {
  char mystr[5] = "1"; //String data
  Serial.write(mystr,5); //Write the serial data   
  }

  if (grain >= eighty)
    {
        char mystr[5] = "2"; //String data
        Serial.write(mystr,5); //Write the serial data
    }
  
char test[5] = "hi"; //String data
        Serial.write(test,7); //Write the serial data
}
  
  



}
