//SDA pin: A4 pinine
//SCL pin: A5 pinine
//GND pin: Gnd pinine
//VIN pin: 5V pinine
//başlamak için seri port ekranına 8 yazarak gönder butonuna basınız.
//Durdurmak için seri port ekranına 8 yazarak gönder butonuna basınız.

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <Servo.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>
 
Servo ust_servo;
Servo alt_servo;
int renk = 0;
int data, durum = false;

 #define Bbuton 6
 #define Dbuton 7
 #define yesilLed 4
 #define kirmiziLed 5



Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

//------------------------------------------------------------ VOİD SETUP --------------------------------------------------------------------------------------------
void setup() {

lcd.init(); lcd.backlight();

Serial.begin(9600);
Serial.println("başlamak için seri port ekranına 1 yazarak gönder butonuna basınız.");
Serial.println("Durdurmak için seri port ekranına 2 yazarak gönder butonuna basınız.");

lcd.setCursor(0,0); lcd.print("===> M.H.O. <===");
lcd.setCursor(0,1); lcd.print("  MAKINE HAZIR  ");



if (tcs.begin()) {
} else {
 
while (1); // Dur!
}
 
ust_servo.attach(9);
alt_servo.attach(8);
 
ust_servo.write(75);
delay(10);

pinMode(Bbuton , INPUT);
pinMode(Dbuton , INPUT);
pinMode(yesilLed , OUTPUT);
pinMode(kirmiziLed , OUTPUT);
}
//------------------------------------------------------------ VOİD LOOP ----------------------------------------------------------------------------------
void loop() {
 
data = Serial.read();
delay(10);
if ( data == '1' || digitalRead(Bbuton) == 1)
{
durum = true;
  Serial.println("MAKİNE BAŞLATILDI.");
  lcd.setCursor(0,1); lcd.print(" MAKINE BASLADI ");
  
 
}
 
if ( data =='2'|| digitalRead(Dbuton) == 1)
{
durum = false;
  Serial.println("MAKİNE DURDURULDU.");
  lcd.setCursor(0,0); lcd.print("===> M.H.O. <===");
  lcd.setCursor(0,1); lcd.print("  MAKINE DURDU  ");
}
 
if ( durum == true)
{
makine_baslat();
}
if ( durum == false)
{
makine_durdur();
}
}


//----------------------------------------------------- FONKSİYONLAR ----------------------------------------------------------------------------
 
void makine_baslat()
{
  digitalWrite(yesilLed , HIGH);
  digitalWrite(kirmiziLed , LOW);
delay(500);
for(int i = 96; i > 40; i--) {
ust_servo.write(i);
delay(5);
}
delay(1500);
 
renk = renk_oku();
Serial.print("*********renk: ");
Serial.println(renk);
delay(50);
switch (renk) {
case 1:
alt_servo.write(40);
 
Serial.println("*************Kırmızı");
lcd.setCursor(0,0); lcd.print("  OKUNAN RENK   ");
lcd.setCursor(0,1); lcd.print("--> KIRMIZI <---");
break;
case 2:
alt_servo.write(125);
Serial.println("**********Yesil");
lcd.setCursor(0,0); lcd.print("  OKUNAN RENK   ");
lcd.setCursor(0,1); lcd.print("---> YESIL <----");
break;
case 3:
alt_servo.write(150);
Serial.println("***********Mor");
lcd.setCursor(0,0); lcd.print("  OKUNAN RENK   ");
lcd.setCursor(0,1); lcd.print("----> MOR <-----");
break;
case 4:
alt_servo.write(95);
Serial.println("***********Sari");
lcd.setCursor(0,0); lcd.print("  OKUNAN RENK   ");
lcd.setCursor(0,1); lcd.print("----> SARI <----");
break;
case 5:
alt_servo.write(63);
Serial.println("**********Pembe");
lcd.setCursor(0,0); lcd.print("  OKUNAN RENK   ");
lcd.setCursor(0,1); lcd.print("---> PEMBE <----");
break;
case 6:
alt_servo.write(150);
Serial.println("*******Gri");
lcd.setCursor(0,0); lcd.print("  OKUNAN RENK   ");
lcd.setCursor(0,1); lcd.print("----> GRI <-----");
break;
default:
Serial.println(0);
break;
}
delay(300);
 
 
for(int i = 40; i > 5; i--) {
ust_servo.write(i);
delay(5);
}
delay(600);
 
for(int i = 5; i < 96; i++) {
ust_servo.write(i);
delay(5);
}
renk=0;
}
 
void makine_durdur()
{
ust_servo.write(ust_servo.read());
alt_servo.write(alt_servo.read());
digitalWrite(kirmiziLed , HIGH);
digitalWrite(yesilLed , LOW);
}
 
// renk okuma fonksiyonu
int renk_oku() {
uint16_t renksiz, kirmizi,yesil,mavi;
float  k,y,m;
delay(100);
tcs.getRawData(&kirmizi,&yesil,&mavi,&renksiz);
 
  k=kirmizi;
  y=yesil;
  m=mavi;
  
Serial.print("Renksiz: "); Serial.println(renksiz);
 Serial.print("Kirmzi: "); Serial.println(k);
 Serial.print("Yesil: "); Serial.println(y);
 Serial.print("Mavi: "); Serial.println(m); 
delay(50);
 
if((k <= 255.00 && k >= 150.00)&&(y <= 110.00 && y >= 0.00)&&(m <= 100.00 && m >= 0.00)){
  Serial.println("--------Kirmizi ");
renk = 1;

}
else if((k <= 150.00 && k >= 55.00)&&(y <= 255.00 && y >= 120.00)&&(m <= 140.00 && m >= 55.00)){
  Serial.println("------------Yesil ");
renk = 2;
 
}
else if((k <= 150.00 && k >= 55.00)&&(y <= 160.00 && y >= 55.00)&&(m <= 255.00 && m >= 85.00)){
  Serial.println("-----------Mor ");
renk = 3;
 
}

else if((k <= 300.00 && k >= 215.00)&&(y <= 255.00 && y >= 150.00)&&(m <= 132.50 && m >= 100.00)){
  Serial.println("------------Sarı");
renk = 4;
 
}
else if((k <= 255.00 && k >= 150.00)&&(y <= 200.00&& y >= 50.00)&&(m <= 170.00 && m >= 50.00)){
  Serial.println("----------Pembe");
renk = 5;
 
}
else if((k <= 215 && k >= 150)&&(y <= 150 && y >= 0)&&(m <= 150 && m >= 0)){
  Serial.println("------------Gri ");
renk = 6;
 
}
return renk;
}
