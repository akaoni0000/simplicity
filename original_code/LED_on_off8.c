#include <SI_EFM8SB1_Register_Enums.h>

sbit LED = P0^5;  // LEDの接続ピン
int ms;
int i;

void Timer0_Init(void) {
    CKCON0 = 0x04;
    TMOD = 0x01;
}

void Timer0_Delay(ms) {//for文1回で1ms   2435カウントする F67C
    for (i = 0; i < ms; i++) {
        TH0 = 0xF6;
        TL0 = 0x7C;
        TCON_TR0 = 1;  // Timer0をスタート
        while (!TCON_TF0);  // Timer0のオーバーフローを待機 オーバーフローすると1になる
        TCON_TF0 = 0;  // Timer0のオーバーフローフラグをクリア
    }
}

void main(void) {
    XBR2 = 0x40;  // クロスバーを有効化
    PCA0MD = 0x00;  // ウォッチドッグタイマーを無効化
    P0MDOUT = 0x10;  // P0.4をプッシュプル出力に設定

    Timer0_Init();  // Timer0を初期化
    LED = 0;  // LEDを消灯

    while (1) {
        LED = !LED;  // LEDの状態を反転
        Timer0_Delay(10000);  // 1秒待機
    }
}