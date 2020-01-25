
/**************************************************************
BlackBug Engineering
25.01.2019
https://github.com/dbprof/hot-wheels-speed-track
***************************************************************/
//Библиотека сервопривода
#include <Servo.h> 

//Библиотека ЖК-дисплея
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define IR1_PIN 2
#define IR2_PIN 3
#define BTN_PIN 8
#define SERVO1_PIN 9
#define SERVO2_PIN 10
#define BUZZER_PIN 12

//Объявляем переменные времени
uint32_t uiStartTime, uiFinishTime1, uiFinishTime2;

//Объявляем переменную расстояния и подставляем длину трассы в метрах
double dDistance = 0.5; //метров

//Объявляем переменную скорости
double dSpeed1, dSpeed2;

//Объявляем переменную сигнала ИК-датчика
byte bIR1Signal, bIR2Signal;

//Объявляем ЖК-дисплей
LiquidCrystal_I2C lcd(0x27,16,2);

//Объявляем сервоприводы
Servo ServoLeft;
Servo ServoRight;

//Объявляем признак подключения сервопривода
bool isServoAttached = false;

//Подпрограмма подключения сервопривода
void attachServo() {
  if (!isServoAttached) {
    ServoLeft.attach(SERVO1_PIN);
    ServoRight.attach(SERVO2_PIN);
    isServoAttached = true;
  }
}
//Подпрограмма отключения сервопривода
void detachServo() {
  if (isServoAttached) {
    ServoLeft.detach();
    ServoRight.detach();
    isServoAttached = false;
  }
}
//Подпрограмма закрытия барьера
void closeGate() {
  attachServo();
  ServoLeft.write(155);
  ServoRight.write(10);
  delay(300);
  detachServo();
}
//Подпрограмма открытия барьера
void openGate() {
  attachServo();
  ServoLeft.write(35);
  ServoRight.write(130);
  delay(300);
  detachServo();
}

//Функция вывода на ЖК-дисплей
void printLCD(String stVar1, String stVar2){
  lcd.clear();  
  lcd.setCursor(0, 0); //первая строка
  lcd.print(stVar1);
  lcd.setCursor(0, 1); //вторая строка
  lcd.print(stVar2);
}

void clearAll(){
  uiStartTime = 0;
  uiFinishTime1 = 0;
  uiFinishTime2 = 0;
  dSpeed1 = 0.0;
  dSpeed2 = 0.0;
}


void setup() {
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(BTN_PIN, INPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  
  closeGate();
  
  //Инициализация ЖК-дисплея
  lcd.init();
  //Включаем подсветку                     
  lcd.backlight();
  //Обнуляем переменные
  clearAll();
}

void loop() {
  
  //Проверка датчиков
  bIR1Signal = digitalRead( IR1_PIN );
  bIR2Signal = digitalRead( IR2_PIN );
  if (bIR1Signal == LOW && bIR2Signal == LOW){
    printLCD("Hot Wheels Track","PRESS THE BUTTON");
    delay(200);
    if( digitalRead(BTN_PIN) == HIGH ){
      printLCD("Ready!","");
      tone(BUZZER_PIN,500,300);
      delay(1000);
      printLCD("Steady!","");
      tone(BUZZER_PIN,510,300);
      delay(1000);
      printLCD("GO!!!","");
      tone(BUZZER_PIN,1500,300);

      //Открываем трассу
      openGate();
      //Засекаем время
      uiStartTime = millis();
      
      //Ждем срабатывания обоих датчиков и расчитываем время
      do {
        bIR1Signal = digitalRead( IR1_PIN );
        bIR2Signal = digitalRead( IR2_PIN );
        if (bIR1Signal == HIGH && uiFinishTime1 == 0) uiFinishTime1 = millis() - uiStartTime;
        if (bIR2Signal == HIGH && uiFinishTime2 == 0) uiFinishTime2 = millis() - uiStartTime;
      }
      while (uiFinishTime1 == 0 || uiFinishTime2 == 0);
    
      tone(BUZZER_PIN,3000,500);
      delay(500);
      //Закрываем трассу
      closeGate();

      //Расчитываем скорость
      dSpeed1 = dDistance/uiFinishTime1*1000;  //скорость = м/с
      dSpeed2 = dDistance/uiFinishTime2*1000;  //скорость = м/с

      //Выводим значения на дисплей до нажатия кнопки
      printLCD(String(uiFinishTime1) + " ms " + String(dSpeed1) + " m/s",
               String(uiFinishTime2) + " ms " + String(dSpeed2) + " m/s");
      while (digitalRead(BTN_PIN) == LOW){};
      clearAll();
    }
  }
  else if(bIR1Signal == LOW && bIR2Signal == HIGH){
    printLCD("IR Sensor Error","CHECK SIGNAL 2");
    delay(200);
  }
  else if(bIR1Signal == HIGH && bIR2Signal == LOW){
    printLCD("IR Sensor Error","CHECK SIGNAL 1");
    delay(200);
  }
  else{
    printLCD("IR Sensor Error","CHECK SIGNAL 1,2");
    delay(200);
  }
}
