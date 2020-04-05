/**************************************************************
BlackBug Engineering
02.04.2020
https://github.com/dbprof/hot-wheels-speed-track
***************************************************************/
// Библиотека сервопривода
#include <Servo.h> 

// Библиотека ЖК-дисплея
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Определение пинов инфракрасных датчиков 
#define IR1_PIN 2
#define IR2_PIN 3

// Определение пина кнопки
#define BTN_PIN 8

// Определение пинов сервоприводов
#define SERVO1_PIN 9
#define SERVO2_PIN 10

// Определение пина пищалки
#define BUZZER_PIN 12

// Определение углов поворота сервоприводов (определяются опытным путем)
// позволяют проводить дополнительную юстировку в случае не одновременного срабатывания
#define L_SERV_ANGLE_CLOS 153 // Угол левого сервопривода в закрытом состоянии
#define R_SERV_ANGLE_CLOS 6   // Угол правого сервопривода в закрытом состоянии
#define L_SERV_ANGLE_OPEN 43  // Угол левого сервопривода в открытом состоянии
#define R_SERV_ANGLE_OPEN 96  // Угол правого сервопривода в открытом состоянии

// Определение пинов светодиодов 
#define RED1 4
#define GREEN1 5
#define RED2 6
#define GREEN2 7

// Объявляем переменные времени
uint32_t uiStartTime, uiFinishTime1, uiFinishTime2;

// Объявляем переменную расстояния и подставляем длину трассы в метрах
double dDistance = 0.5; //метров (определяется замером)

// Объявляем переменные скорости
double dSpeed1, dSpeed2;

// Объявляем переменные сигнала ИК-датчика
byte bIR1Signal, bIR2Signal;

// Объявляем ЖК-дисплей
LiquidCrystal_I2C lcd(0x27,16,2);

// Объявляем сервоприводы
Servo ServoLeft;
Servo ServoRight;

// Объявляем углы поворота сервопривода
int iLeftServoAngle, iRightServoAngle;

// Объявляем признак подключения сервопривода
bool isServoAttached = false;

// Подпрограмма подключения сервопривода
void attachServo() {
  if (!isServoAttached) {
    ServoLeft.attach(SERVO1_PIN);
    ServoRight.attach(SERVO2_PIN);
    isServoAttached = true;
  }
}

// Подпрограмма отключения сервопривода
void detachServo() {
  if (isServoAttached) {
    ServoLeft.detach();
    ServoRight.detach();
    isServoAttached = false;
  }
}

// Подпрограмма закрытия барьера
void closeGate() {
  attachServo();
  ServoLeft.write(L_SERV_ANGLE_CLOS);
  ServoRight.write(R_SERV_ANGLE_CLOS);
  delay(300);
  detachServo();
}

// Подпрограмма открытия барьера
void openGate() {
  attachServo();
  ServoLeft.write(L_SERV_ANGLE_OPEN);
  ServoRight.write(R_SERV_ANGLE_OPEN);
  delay(300);
  detachServo();
}

// Подпрограмма переводящая барьер в положение по умолчанию
void defaultGate() {
  attachServo();
  ServoLeft.write(L_SERV_ANGLE_CLOS);
  ServoRight.write(R_SERV_ANGLE_CLOS);
  delay(300);
  iLeftServoAngle = ServoLeft.read();
  iRightServoAngle = ServoRight.read();
  detachServo();
}

// Функция вывода на ЖК-дисплей
void printLCD(String stVar1, String stVar2){
  lcd.clear();  
  lcd.setCursor(0, 0); //первая строка
  lcd.print(stVar1);
  lcd.setCursor(0, 1); //вторая строка
  lcd.print(stVar2);
}

// Функция обнуления светодиодов - все не светятся
void ledreset() {
  digitalWrite(RED1, 1);
  digitalWrite(GREEN1, 1);
  digitalWrite(RED2, 1);
  digitalWrite(GREEN2, 1);
}

// Функция обнуления переменных
void clearAll(){
  uiStartTime = 0;
  uiFinishTime1 = 0;
  uiFinishTime2 = 0;
  dSpeed1 = 0.0;
  dSpeed2 = 0.0;
  ledreset();
}

void setup() {
  // Установка вывода для тестирования
  //Serial.begin(9600);

  // Установка светодиодов
  pinMode(RED1, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(GREEN2, OUTPUT);

  // Установка инфракрасных датчиков
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);

  // Установка кнопки
  pinMode(BTN_PIN, INPUT);

  // Установка пищалки
  pinMode(BUZZER_PIN,OUTPUT);
  
  // Установка барьера в начальное положение
  defaultGate();
  
  // Инициализация ЖК-дисплея
  lcd.init();      
  lcd.backlight(); //Включаем подсветку
  
  // Обнуляем переменные
  clearAll();
}

void loop() {
  
  // Проверка датчиков
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

      // Открываем трассу
      openGate();
      
      // Засекаем время
      uiStartTime = millis();
      
      // Ждем срабатывания обоих датчиков и расчитываем время
      do {
        bIR1Signal = digitalRead( IR1_PIN );
        bIR2Signal = digitalRead( IR2_PIN );
        if (bIR1Signal == HIGH && uiFinishTime1 == 0) uiFinishTime1 = millis() - uiStartTime;
        if (bIR2Signal == HIGH && uiFinishTime2 == 0) uiFinishTime2 = millis() - uiStartTime;
      }
      while (uiFinishTime1 == 0 || uiFinishTime2 == 0);

      // Играем звук завершения замера
      tone(BUZZER_PIN,3000,500);
      delay(500);
      
      //Закрываем трассу
      closeGate();

      // Расчитываем скорость
      dSpeed1 = dDistance/uiFinishTime1*1000;  //скорость = м/с
      dSpeed2 = dDistance/uiFinishTime2*1000;  //скорость = м/с

      // Выводим значения на дисплей до нажатия кнопки
      printLCD("1:" + String(uiFinishTime1) + "ms " + String(dSpeed1) + "m/s",
               "2:" + String(uiFinishTime2) + "ms " + String(dSpeed2) + "m/s");

      // Зажигаем светодиодные индикаторы в зависимости от результата
      if (dSpeed1 > dSpeed2) {
        digitalWrite(GREEN1, 0);
        digitalWrite(RED2, 0);
      }
      else if (dSpeed1 < dSpeed2){
        digitalWrite(RED1, 0);
        digitalWrite(GREEN2, 0);
      }
      else {
        digitalWrite(GREEN1, 0);
        digitalWrite(GREEN2, 0);
      }

      // Ждем нажатия кнопки для повторного замера
      while (digitalRead(BTN_PIN) == LOW){};
      clearAll();
    }
  }

  // Продолжени проверки датчиков - не работает датчик 2
  else if(bIR1Signal == LOW && bIR2Signal == HIGH){
    printLCD("IR Sensor Error","CHECK SIGNAL 2");
    delay(200);
  }

  // Продолжени проверки датчиков - не работает датчик 1
  else if(bIR1Signal == HIGH && bIR2Signal == LOW){
    printLCD("IR Sensor Error","CHECK SIGNAL 1");
    delay(200);
  }

  // Продолжени проверки датчиков -  - не работают датчики 1 и 2
  else{
    printLCD("IR Sensor Error","CHECK SIGNAL 1,2");
    delay(200);
  }
}
