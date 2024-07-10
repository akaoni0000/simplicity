#include <SI_EFM8BB1_Register_Enums.h> // デバイスに依存したレジスタの定義 失敗のコード

#define LED_PIN   P0_B4   // 0.4ピンを使用
#define LED_ON    0       // LEDがオンの時の値
#define LED_OFF   1       // LEDがオフの時の値

void Timer0_Delay_ms(unsigned int ms);

void main(void)
{
    // クロックの初期化など、必要な初期化を行う
    PCA0MD &= ~0x40; // ウォッチドッグタイマーを無効にする
    P0MDOUT |= 0x10; // P0.4をプッシュプル出力に設定
    XBR1 |= 0x40;    // クロスバーを有効にする
    IE_EA = 1;       // 全割り込みを有効にする

    while (1)
    {
        // LEDを点灯
        LED_PIN = LED_ON;
        Timer0_Delay_ms(1000); // 1秒待機

        // LEDを消灯
        LED_PIN = LED_OFF;
        Timer0_Delay_ms(1000); // 1秒待機
    }
}

// タイマー0を使用してms単位の遅延を作成
void Timer0_Delay_ms(unsigned int ms)
{
    unsigned int i;
    for (i = 0; i < ms; i++)
    {
        TH0 = 0xFC; // 1msの遅延を設定 (12MHzクロックの場合)
        TL0 = 0x66;
        TCON_TR0 = 1; // タイマー0を開始
        while (!TCON_TF0); // オーバーフローを待つ
        TCON_TF0 = 0; // オーバーフローをクリア
        TCON_TR0 = 0; // タイマー0を停止
    }
}
