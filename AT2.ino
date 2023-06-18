#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);

// variable
int moisure,temp;
int i = 0, sec = 0, minutes = 0, hours = 0 , day = 0;

ISR(TIMER1_COMPA_vect) // timer1 Interrupt ทุกๆ 0.1 วินาที
{
  i++;
  if(i >= 10){
    sec++;
     i = 0;
}
   if(sec >= 60){
    minutes++;
    sec = 0;
}
   if(minutes >= 60){
      hours++;  
      minutes = 0;
}
    if(hours >= 24){
      day++;
      hours = 0;
}
}

void setup() {
  lcd.begin();
  lcd.backlight();
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 6250; // 0.1 sec.
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12); // 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts
  Serial.begin(9600);
}
void loop() {
  temp = sensors.getTempCByIndex(0);
  moisure = analogRead(A3);
  sensors.requestTemperatures();
  Serial.print("Time:"); Serial.println(moisure);
  lcd_status();
  delay(5000);
  lcd_date();
  delay(5000);
}
void lcd_date(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Already Started");
  lcd.setCursor(0,1);
  lcd.print(day);
  lcd.setCursor(3,1);
  lcd.print("Day");
  lcd.setCursor(7,1);
  lcd.print("H/M");
  lcd.setCursor(11,1);
  lcd.print(hours);
  lcd.setCursor(13,1);
  lcd.print(":");
  lcd.setCursor(14,1);
  lcd.print(minutes);
}
void lcd_status(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp :");
  lcd.setCursor(7,0);
  lcd.print(sensors.getTempCByIndex(0));
  lcd.setCursor(13,0);
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("Moisure:");
  lcd.setCursor(9,1);
  lcd.print(moisure);
}
