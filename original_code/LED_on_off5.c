#include <SI_EFM8SB1_Register_Enums.h>//4.6秒周期

sbit LED = P0^5;  // P1.4をLEDに設定

void Timer0_Init(void);
void Timer0_Wait_1s(void);

void main(void) {
    XBR2 = 0x40;  // クロスバーを有効化
    PCA0MD &= ~0x40;  // ウォッチドッグタイマーを無効化

    P1MDOUT |= 0x10;  // P1.4をプッシュプル出力に設定
    LED = 0;  // LEDを消灯

    Timer0_Init();  // Timer0を初期化

    while (1) {
        LED = !LED;  // LEDの状態を反転
        Timer0_Wait_1s();  // 1秒待機
    }
}

void Timer0_Init(void) {
    TMOD &= 0xF0;  // Timer0の設定をクリア
    TMOD |= 0x01;  // Timer0をモード1（16ビットタイマモード）に設定
    CKCON0 |= 0x08;  // システムクロックをTimer0に供給
    TH0 = 0x3C;  // 初期値設定 (65536 - 50000) -> 50ms
    TL0 = 0xB0;
    TCON_TR0 = 1;  // Timer0をスタート
}

void Timer0_Wait_1s(void) {
    unsigned int i;
    for (i = 0; i < 20; i++) {  // 50ms * 20 = 1000ms = 1s
        while (!TCON_TF0);  // Timer0のオーバーフローを待機
        TCON_TF0 = 0;  // Timer0のオーバーフローフラグをクリア
        TH0 = 0x3C;  // 50msの初期値設定
        TL0 = 0xB0;
        TCON_TR0 = 1;  // Timer0を再スタート
    }
}