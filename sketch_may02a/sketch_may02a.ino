#include <Keypad.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte rows = 4; // четыре строки
const byte cols = 4; // четыре столбца
char keys[rows][cols] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'0',',','=','+'}
};
byte rowPins[rows] = {5, 4, 3, 2}; // подключить к выводам строк клавиатуры
byte colPins[cols] = {9, 8, 7, 6};    // подключить к выводам столбцов клавиатуры
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
char symbolArray[16];
char symbolArray2[16];
int i = 0;
double firstNumber = 0;
double result = 0;
int buttonPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  lcd.init();                      // Инициализация дисплея  
  lcd.backlight();                 // Подключение подсветки
  lcd.setCursor(0,0);              // Установка курсора в начало первой строки
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("Calculate me");
  delay(3000);
  lcd.clear();
}

double setNumber(char key) {         // Установить число
  lcd.clear();
  while (key != '/' && key != '*' && key != '-' && key != '+' && key != '=') {
    boolean containDot = false;
    if (key == ',') {
      for (int j = 0; j < 16; j += 1) {
        if (symbolArray[j] == ',') containDot = true;
      } 
    }
    if (containDot == false) {
      symbolArray[i] = key;
      lcd.setCursor(i, 0);
      lcd.print(key);
      i += 1;
      key = keypad.waitForKey();
    }
    else key = keypad.waitForKey();
  }
  i = 0;
  String number;
  for (int j = 0; j < 16; j += 1) {
    number += symbolArray[j];
  }
  memset(symbolArray, 0, 16);
  return number.toInt();
}

double setSecondNumber(char key){
  lcd.clear();
  while (key != '=') {
    while(key == '/' || key == '*' || key == '-' || key == '+'){
      key = keypad.waitForKey();
    }
    boolean containDot = false;
    if (key == ',') {
      for (int j = 0; j < 16; j += 1) {
        if (symbolArray[j] == ',') containDot = true;
      } 
    }
    if (containDot == false) {
      symbolArray[i] = key;
      lcd.setCursor(i, 0);
      lcd.print(key);
      i += 1;
      key = keypad.waitForKey();
    }
    else key = keypad.waitForKey();
  }
  i = 0;
  String number;
  for (int j = 0; j < 16; j += 1) {
    number += symbolArray[j];
  }
  memset(symbolArray, 0, 16);
  return number.toInt();
}

void division(double firstNumber, char key) {          // Деление
  lcd.clear();
  lcd.print('/');
  key = keypad.waitForKey();
  double secondNumber = setSecondNumber(key);
  lcd.clear();
  lcd.print(firstNumber / secondNumber);
  Serial.println(firstNumber / secondNumber);
  result = firstNumber / secondNumber;
}

void multiplication(double firstNumber, char key) {          // Умножение
  lcd.clear();
  lcd.print('*');
  key = keypad.waitForKey();
  double secondNumber = setSecondNumber(key);
  lcd.clear();
  lcd.print(firstNumber * secondNumber);
  Serial.println(firstNumber * secondNumber);
  result = firstNumber * secondNumber;
}

void subtraction(double firstNumber, char key) {          // Вычитание
  lcd.clear();
  lcd.print('-');
  key = keypad.waitForKey();
  double secondNumber = setSecondNumber(key);
  lcd.clear();
  lcd.print(firstNumber - secondNumber);
  Serial.println(firstNumber - secondNumber);
  result = firstNumber - secondNumber;
}

void addition(double firstNumber, char key) {          // Сложение
  lcd.clear();
  lcd.print('+');
  key = keypad.waitForKey();
  double secondNumber = setSecondNumber(key);
  lcd.clear();
  lcd.print(firstNumber + secondNumber);
  Serial.println(firstNumber + secondNumber);
  result = firstNumber + secondNumber;
}

void calculate(double firstNumber) {          // Вычисление
  lcd.clear();
  lcd.print(result);
}

void isButtonPress(boolean val) {
  if(val){
    result = 0;
    firstNumber = 0;
    lcd.clear();
    Serial.println("reset");
  }
}

void loop() {
  bool val = digitalRead(buttonPin);
  isButtonPress(val);
  char key = keypad.getKey();
  if (key) {
      switch(key) {
        case '0' ... '9':
        firstNumber = setNumber(key);
        result = firstNumber;
        break;
        case '/':
        division(result, key);
        break;
        case '*':
        multiplication(result, key);
        break;
        case '-':
        subtraction(result, key);
        break;
        case '+':
        addition(result, key);
        break;
    }
  }
  
  
}
