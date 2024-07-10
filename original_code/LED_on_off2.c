#include <SI_EFM8SB1_Register_Enums.h>

// LEDが接続されているポートとピンを定義
//#define LED_PIN P1_B2

sbit LED_PIN = P0^4;

void initOscillator(void) {
  // 内部オシレーターを設定 (24.5 MHz)
  HFO0CN = 0x03;
}

void initPorts(void) {
  // P1.2をデジタル出力に設定
  P1MDOUT |= 0x04;
  XBR2 |= 0x40;  // クロスバーを有効化
}

void delay(uint16_t time) {
  // 簡単なディレイループ
  uint16_t i, j;
  for (i = 0; i < time; i++) {
    for (j = 0; j < 500; j++) {
      // 何もしない
    }
  }
}

void main(void) {
  // 初期設定
  initOscillator();
  initPorts();
  //PCA0MD &= ~0x60;

  while (1) {
    // LEDを点灯
    LED_PIN = 0;
    delay(1000);  // 1000msディレイ

    // LEDを消灯
    LED_PIN = 1;
    delay(1000);  // 1000msディレイ
  }
}

