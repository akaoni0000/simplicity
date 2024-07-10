#include <SI_EFM8SB1_Register_Enums.h>

sbit LED = P0^5;  // LEDの接続ピン

void Timer0_Init(void);
void Timer0_Delay_ms(unsigned int ms);
void Timer0_Delay_ms2(unsigned int ms);

void main(void) {
    XBR2 = 0x40;  // クロスバーを有効化
    PCA0MD &= ~0x40;  // ウォッチドッグタイマーを無効化

    P0MDOUT |= 0x10;  // P0.4をプッシュプル出力に設定
    LED = 0;  // LEDを消灯

    Timer0_Init();  // Timer0を初期化

    while (1) {
        LED = !LED;  // LEDの状態を反転
        Timer0_Delay_ms(1000);  // 1秒待機
    }
}

void Timer0_Init(void) {
    //TMOD &= 0xF0;  // Timer0の設定をクリア
    //TMOD |= 0x01;  // Timer1をモード1（16ビットタイマモード）に設定

    //CKCON0 |= 0x08;  // システムクロックをTimer1に供給
    CKCON0 = 0x04;
    TMOD = 0x01;
}

void Timer0_Delay_ms(unsigned int ms) {//for文1回で1ms   2535カウントする
    unsigned int i;
    for (i = 0; i < ms; i++) {
        TH0 = 0xFC;  // 初期値設定 (65535 - 1000) -> 1ms
        TL0 = 0x17;
        TCON_TR0 = 1;  // Timer0をスタート
        while (!TCON_TF0);  // Timer1のオーバーフローを待機
        TCON_TF0 = 0;  // Timer1のオーバーフローフラグをクリア
    }
}

void Timer0_Delay_ms2(unsigned int ms) {//for文1回で1ms   2535カウントする
    unsigned int i;
    for (i = 0; i < ms; i++) {
        TH0 = 0xFF;  // 初期値設定 (65535 - 1000) -> 1ms
        TL0 = 0x9B;
        TCON_TR0 = 1;  // Timer0をスタート
        while (!TCON_TF0);  // Timer1のオーバーフローを待機
        TCON_TF0 = 0;  // Timer1のオーバーフローフラグをクリア
    }
}