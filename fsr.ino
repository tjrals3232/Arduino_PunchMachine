#include <LiquidCrystal_I2C.h>
#include "pitches.h"

LiquidCrystal_I2C lcd(0x27,20,4);
// 필요한 변수 정의
int value = 0;
int bestRecord = 0;
int worstRecord = 0;
int count = 0;
int num[6];
unsigned long before = 0;
unsigned long now = 0;

// 부저 핀 번호
int buzzerPin = 2;
int buttonPin = 4;

const char *number[]={"0 1 2","1 2 3","2 3 4","3 4 5","4 5 6","5 6 7","6 7 8","7 8 9","8 9 0","9 0 1"};
const char *words[] = {
  "Know your worth", "Set yourself free", "Be wild for a while", "It's not your fault",
  "Follow your heart", "Patience is power", "Turn pain to power",
  "Stay Strong", "Inner peace", "Be still, steal", "You are beautiful", "No rain, No flower"
};

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  lcd.init();
  lcd.backlight();
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("The Amazing");
  lcd.setCursor(0,2);
  lcd.print("Digital Fortune");
  lcd.setCursor(0,3);
  lcd.print("COOKEY");
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  delay(3000);
}

void loop() {
  value = analogRead(A0);
  //int index = 0;
  if(count==0 && value>4){ // 초기 worstRecord 기록용
    worstRecord = value;
  }
  
  if(digitalRead(buttonPin) == LOW){
    before = millis();
    delay(200);
    lcd.print("Keep press to reset.");
    delay(500);
    
    while(digitalRead(buttonPin) != HIGH){
      now = millis();
      
      if((now-before)>=3000){
        lcd.clear();
        lcd.print("Score reset");
        
        for(int i=0;i<3;i++){
          lcd.print(".");
          delay(1000);
        }
        bestRecord = 0;
        worstRecord = 0;
        count=0;
        lcd.setCursor(0,2);
        lcd.print("Complete!");
        delay(1000);
        break;
      }
    }
  }
  lcd.clear();

  // 오차 이상의 압력값 받을시 진행
  if(value > 10){ 
    for(int i=0;i<6;i++){ // 점수 잭팟 머신처럼 움직이기
      lcd.clear();
      
      for(int k=0;k<sizeof(number)/sizeof(number[0]);k++){
        lcd.setCursor(8,1);
        lcd.print(number[k]);
        tone(buzzerPin,1000,20);
        delay(20);
      }
    }
    lcd.clear();
    tone(buzzerPin,value+1000,500);
    
    if(value > 730 && value < 790){
      lcd.clear();
      lcd.print("U MAKE LUCKY NUMBER!");
      lcd.setCursor(0,2);
      lcd.print("Fortune favors");
      lcd.setCursor(0,3);
      lcd.print("the brave.");
      playHappyBirthday(); // 축하 멜로디 출력
      delay(1000);
      lcd.clear();

      // 로또번호
      lcd.print("LOTTERY NUMBER");
      lcd.setCursor(0,2);
      for(int i=0;i<6;i++){
        num[i] = genLotNum(num,i);
        lcd.print(num[i]);
        lcd.print(" ");
        tone(buzzerPin,5*num[i]+1000,100);
        delay(500);
      }
      delay(2000);
      lcd.clear();
    }

    // 최고기록 달성시 표시
    if(bestRecord < value){ 
      for(int j=0;j<3;j++){
        for(int i=0;i<5;i++){
          lcd.print("!!NEW HIGH SCORE!!");
          tone(buzzerPin,value+1000,50);
          delay(60);
        }
      }

      bestRecord = value; // 최고기록 저장 및 출력
      lcd.clear();
      lcd.print("CONGRATULATIONS!");
      lcd.setCursor(0,2);
      lcd.print("Your record: ");
      lcd.print(value);
      lcd.setCursor(0,3);
      lcd.print("Best record: ");        
      lcd.print(bestRecord);
      count++;

      playMario();
      printWiseSaying();
    }

    // 최고 기록 아닌 경우
    else{ 
      lcd.print("Try more...");
      delay(400);
      lcd.setCursor(0,2);
      lcd.print("Your record: ");
      lcd.print(value);
      lcd.setCursor(0,3);
      lcd.print("Best record: ");
      lcd.print(bestRecord);
      delay(1500);
      
      // 최악기록 표시
      if(value < worstRecord){ 
        lcd.clear();
        lcd.print("Worst record ever!");
        worstRecord = value;
        lcd.setCursor(0,2);
        lcd.print("Worst record: ");
        lcd.print(value);
        count++;
        delay(1500);
      }
      printWiseSaying();
    }
  }

  // 펀치 기록이 없거나 미세오차인 경우
  else {
    lcd.print("Best record: ");
    lcd.print(bestRecord);
    
    if(count > 1 && worstRecord > 10){
      lcd.setCursor(0,2);
      lcd.print("Worst record: ");
      lcd.print(worstRecord);
    }
    
    before = millis(); // 백라이트 off를 위한 시간 측정
    
    while(true){
      delay(10);
      now = millis();
      
      if((now-before>=5000)) lcd.noBacklight(); // 5초 경과시 백라이트 끄기
      
      if(analogRead(A0) > 3 || digitalRead(buttonPin) == LOW){ // 버튼 눌리거나 압력 가해질 시 처음으로 이동
        delay(100);
        lcd.clear();
        lcd.backlight();
        break;
      }
    }
  }
}

int genLotNum(int num[], int index){
  int randNum;
  bool duplicate;
  do {
    randNum = int(random(1, 46)); // 1부터 45까지의 랜덤번호 생성
    duplicate = false;
    // 배열에 중복된 숫자가 있는지 확인
    for (int i = 0; i < index; ++i) {
      if (num[i] == randNum) {
        duplicate = true;
        break;
      }
    }
  } while (duplicate); // 중복이면 다시 랜덤번호 생성
  return randNum;
}

void printWiseSaying(void){
  lcd.clear();
  int index = random(0,sizeof(words)/sizeof(words[0])-1);
  lcd.print(words[index]);
  delay(3000);
}

void playMario(void){

  int melody[] = {
    NOTE_E7, NOTE_E7, 0, NOTE_E7,
    0, NOTE_C7, NOTE_E7, 0,
    NOTE_G7, 0, 0, 0,
    NOTE_G6, 0, 0, 0,

    NOTE_C7, 0, 0, NOTE_G6,
    0, 0, NOTE_E6, 0,
    0, NOTE_A6, 0, NOTE_B6,
    0, NOTE_AS6, NOTE_A6, 0,

    NOTE_G6, NOTE_E7, NOTE_G7,
    NOTE_A7, 0, NOTE_F7, NOTE_G7,
    0, NOTE_E7, 0, NOTE_C7, NOTE_D7, NOTE_B6,
    0, 0, NOTE_C7, 0, 0,
  };

  int noteDurations[] = {
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    12, 12, 12, 12,
    
    9, 9, 9,
    12, 12, 12, 12,
    12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12,
  };
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
        int noteDuration = 1000 / noteDurations[i];
        tone(buzzerPin, melody[i], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30; // 음악 재생 시간만큼 대기
        delay(pauseBetweenNotes);
  }
}

void playHappyBirthday(void) {
  int melody[] = {
  NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5,
  NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_C5,
  NOTE_C6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_G6, NOTE_E6,
  
  NOTE_B4, NOTE_B5, NOTE_FS5, NOTE_DS5, NOTE_B5,
  NOTE_FS5, NOTE_DS5, NOTE_DS5, NOTE_E5, NOTE_F5,
  NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_B5
  };

  int noteDurations[] = {
    16, 16, 16, 16,
    32, 16, 8, 16,
    16, 16, 16, 32, 16, 8,
    
    16, 16, 16, 16, 32,
    16, 8, 32, 32, 32,
    32, 32, 32, 32, 32, 16, 8
  };
  
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    int noteDuration = 1000 / noteDurations[i];
    tone(buzzerPin, melody[i], noteDuration);
    int pauseBetweenNotes = noteDuration * 2.3;
    delay(pauseBetweenNotes);
    //noTone(BUZZER_PIN);
  }
}