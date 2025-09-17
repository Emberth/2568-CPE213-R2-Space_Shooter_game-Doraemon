#include <Arduino.h>
#include <stdlib.h>

// === Pin Definitions ===
const int potPin = 34;      
const int buttonPin = 25;   
const int ledGreen = 26;    
const int ledYellow = 27;   
const int ledRed = 14;      

// Ultrasonic Sensor 1
const int trigPin1 = 32;
const int echoPin1 = 33;

// Ultrasonic Sensor 2
const int trigPin2 = 4;
const int echoPin2 = 5;

// === Game Constants ===
const int SCREEN_WIDTH = 20;
const int SCREEN_HEIGHT = 10;
const int GAME_UPDATE_RATE = 500;     
const int BULLET_UPDATE_RATE = 100;   
const int DISPLAY_UPDATE_RATE = 100;  
const int SHIP_MOVE_RATE = 5;        
const int DETECT_DISTANCE = 10;       

// === Game Variables ===
int shipPosition = SCREEN_WIDTH / 2;
int bulletY = -1;
int bulletX = 0;
int asteroidX = 0;
int asteroidY = 0;
int lives = 3;
int score = 0;
bool gameOver = false;
bool gameOverPrinted = false;   // <--- flag ใหม่
unsigned long lastUpdate = 0;
unsigned long lastShipUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastBulletUpdate = 0;
int targetShipPosition = SCREEN_WIDTH / 2;

// === Functions for Ultrasonic ===
long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = duration * 0.034 / 2;
  return distance;
}

long stableReadUltrasonic(int trigPin, int echoPin) {
  long sum = 0;
  int count = 5;
  for (int i = 0; i < count; i++) {
    sum += readUltrasonic(trigPin, echoPin);
    delay(5);
  }
  return sum / count;
}

bool isPersonDetected() {
  long dist1 = stableReadUltrasonic(trigPin1, echoPin1);
  long dist2 = stableReadUltrasonic(trigPin2, echoPin2);
  return (dist1 > 0 && dist1 < DETECT_DISTANCE) ||
         (dist2 > 0 && dist2 < DETECT_DISTANCE);
}

// === Game Support Functions ===
void updateLEDs() {
  digitalWrite(ledGreen, lives >= 3);
  digitalWrite(ledYellow, lives >= 2);
  digitalWrite(ledRed, lives >= 1);
}

void drawGame() {
  for(int i = 0; i < 16; i++) Serial.println("                      ");
  Serial.print("+--------------------+\n");
  
  for(int y = 0; y < SCREEN_HEIGHT - 1; y++) {
    Serial.print("|");
    for(int x = 0; x < SCREEN_WIDTH; x++) {
      if(y == asteroidY && x == asteroidX) {
        Serial.print("*");
      } else if(y == bulletY && x == bulletX) {
        Serial.print("^");
      } else {
        Serial.print(" ");
      }
    }
    Serial.print("|\n");
  }
  
  Serial.print("|");
  for(int x = 0; x < SCREEN_WIDTH; x++) {
    Serial.print(x == shipPosition ? "W" : " ");
  }
  Serial.print("|\n");
  
  Serial.print("+--------------------+\n");
  Serial.print("Lives: ");
  Serial.print(lives);
  Serial.print(" Score: ");
  Serial.print(score);
  Serial.print("\n");
}

void setup() {
  Serial.begin(921600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  Serial.println("\n\n=== Space Shooter Game ===");
  Serial.println("Use potentiometer to move left/right");
  Serial.println("Press button to shoot");
  Serial.println("Game will start when someone is detected within 10 cm...");
}

void loop() {
  if (!isPersonDetected()) {
    Serial.println("⏸ ไม่มีคนในระยะ 10cm - เกมหยุดชั่วคราว");
    return;
  }

  if(gameOver) {
    if(!gameOverPrinted) {  // <--- เช็ค flag
      Serial.println("\nGAME OVER - Final Score: " + String(score));
      gameOverPrinted = true; // พิมพ์ครั้งเดียว
    }
    return; // ไม่ทำ logic ต่อ
  }

  unsigned long currentTime = millis();

  // Ship movement
  if(currentTime - lastShipUpdate >= SHIP_MOVE_RATE) {
    lastShipUpdate = currentTime;
    int potValue = analogRead(potPin);
    targetShipPosition = map(potValue, 0, 4095, 0, SCREEN_WIDTH - 1);
    if(shipPosition < targetShipPosition) shipPosition++;
    else if(shipPosition > targetShipPosition) shipPosition--;
  }

  // Bullet update
  if(currentTime - lastBulletUpdate >= BULLET_UPDATE_RATE) {
    lastBulletUpdate = currentTime;
    if(bulletY >= 0) {
      int prevBulletY = bulletY;
      bulletY--;
      if(bulletX == asteroidX && 
         ((prevBulletY >= asteroidY && bulletY <= asteroidY) || 
          (bulletY == asteroidY))) {
        score += 10;
        asteroidY = 0;
        asteroidX = random(0, SCREEN_WIDTH);
        bulletY = -1;
      }
    }
  }

  // Asteroid update
  if(currentTime - lastUpdate >= GAME_UPDATE_RATE) {
    lastUpdate = currentTime;
    if(digitalRead(buttonPin) == LOW && bulletY < 0) {
      bulletY = SCREEN_HEIGHT - 2;
      bulletX = shipPosition;
    }
    asteroidY++;
    if(asteroidY >= SCREEN_HEIGHT - 1) {
      if(abs(asteroidX - shipPosition) <= 1) {
        lives--;
      }
      asteroidY = 0;
      asteroidX = random(0, SCREEN_WIDTH);
    }
    updateLEDs();
    if(lives <= 0) gameOver = true;
  }

  // Display update
  if(currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_RATE) {
    lastDisplayUpdate = currentTime;
    drawGame();
  }
}
