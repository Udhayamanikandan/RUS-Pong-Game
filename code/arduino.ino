#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int leftJoystick = A0;
const int rightJoystick = A1;
const int PADDLE_HEIGHT = 12;
const int BALL_SIZE = 4;
const int WIN_SCORE = 15;
const float MAX_SPEED = 3.0;
int leftScore = 0;
int rightScore = 0;
int ballX = 64, ballY = 32;
int prevBallX = 64, prevBallY = 32;
float ballSpeedX = 1.0;
float ballSpeedY = 1.0;
int leftPaddle = 26;
int rightPaddle = 26;
unsigned long gameStartTime;
float speedMultiplier = 1.0;
bool isPaused = false;
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  showWelcome();
  countdown();
  gameStartTime = millis();
}
void loop() {
  if (analogRead(leftJoystick) > 1000 && analogRead(rightJoystick) > 1000) {
    isPaused = !isPaused;
    display.clearDisplay();
    centerText("GAME PAUSED", 30);
    display.display();
    Page 13 of 20
    delay(500);
  }
  if (isPaused) return;
  if (leftScore >= WIN_SCORE || rightScore >= WIN_SCORE) {
    gameOver();
    return;
  }
  speedMultiplier = 1.0 + (millis() - gameStartTime) / 30000.0;
  if (speedMultiplier > MAX_SPEED) speedMultiplier = MAX_SPEED;
  movePaddles();
  updateBall();
  drawCourt();
  drawScore();
  drawSpeed();
  drawTimer();
  drawPaddles();
  drawBall();
  if (leftScore == WIN_SCORE - 1 || rightScore == WIN_SCORE - 1) {
    centerText("MATCH POINT!", 52);
  }
  display.display();
  delay(30);
}
void movePaddles() {
  int leftInput = analogRead(leftJoystick);
  if (leftInput < 400) leftPaddle -= 3;
  if (leftInput > 600) leftPaddle += 3;
  int rightInput = analogRead(rightJoystick);
  if (rightInput < 400) rightPaddle -= 3;
  if (rightInput > 600) rightPaddle += 3;
  leftPaddle = constrain(leftPaddle, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
  rightPaddle = constrain(rightPaddle, 0, SCREEN_HEIGHT - PADDLE_HEIGHT);
}
void updateBall() {
  prevBallX = ballX;
  prevBallY = ballY;
  Page 14 of 20
  ballX += ballSpeedX * speedMultiplier;
  ballY += ballSpeedY * speedMultiplier;
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT - BALL_SIZE) ballSpeedY = -ballSpeedY;
  if (ballX <= 5 && ballY + BALL_SIZE >= leftPaddle && ballY <= leftPaddle + PADDLE_HEIGHT) {
    ballSpeedX = abs(ballSpeedX) * 1.05;
    float hitPos = (ballY + BALL_SIZE / 2) - (leftPaddle + PADDLE_HEIGHT / 2);
    ballSpeedY = hitPos / 4.0;
    ballX = 6;
  }
  if (ballX + BALL_SIZE >= SCREEN_WIDTH - 5 && ballY + BALL_SIZE >= rightPaddle && ballY <= rightPaddle +
    PADDLE_HEIGHT) {
    ballSpeedX = -abs(ballSpeedX) * 1.05;
  float hitPos = (ballY + BALL_SIZE / 2) - (rightPaddle + PADDLE_HEIGHT / 2);
  ballSpeedY = hitPos / 4.0;
  ballX = SCREEN_WIDTH - 5 - BALL_SIZE;
    }
    if (ballX < 0) {
      rightScore++;
      resetBall();
      delay(500);
    }
    if (ballX > SCREEN_WIDTH) {
      leftScore++;
      resetBall();
      delay(500);
    }
}
void resetBall() {
  ballX = SCREEN_WIDTH / 2;
  ballY = random(20, SCREEN_HEIGHT - 20);
  ballSpeedX = random(0, 2) ? 1.0 : -1.0;
  ballSpeedY = random(-2, 2);
}
void drawCourt() {
  display.clearDisplay();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  for (int y = 0; y < SCREEN_HEIGHT; y += 6) {
    display.drawFastVLine(SCREEN_WIDTH / 2, y, 3, WHITE);
  }
}
Page 15 of 20
void drawScore() {
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print("KD");
  display.setCursor(2, 10);
  display.print(leftScore);
  display.setCursor(SCREEN_WIDTH - 18, 2);
  display.print("UD");
  display.setCursor(SCREEN_WIDTH - 18, 10);
  display.print(rightScore);
}
void drawSpeed() {
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH / 2) - 15, 2);
  display.print("x");
  display.print(speedMultiplier, 1);
}
void drawTimer() {
  display.setTextSize(1);
  unsigned long elapsed = (millis() - gameStartTime) / 1000;
  display.setCursor((SCREEN_WIDTH / 2) - 12, SCREEN_HEIGHT - 8);
  display.print("T:");
  display.print(elapsed);
  display.print("s");
}
void drawPaddles() {
  display.fillRect(2, leftPaddle, 3, PADDLE_HEIGHT, WHITE);
  display.fillRect(SCREEN_WIDTH - 5, rightPaddle, 3, PADDLE_HEIGHT, WHITE);
}
void drawBall() {
  display.fillRect(prevBallX, prevBallY, BALL_SIZE, BALL_SIZE, BLACK);
  display.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, WHITE);
}
void showWelcome() {
  display.clearDisplay();
  display.setTextSize(1);
  centerText("Welcome to", 10);
  centerText("RUS PONG!", 20);
  centerText("KD vs UD", 35);
