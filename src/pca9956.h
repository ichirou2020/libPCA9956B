/**
 * @file pca9956.h
 * @author 石塚一郎
 * @brief LEDドライバー(PCA9956B)
 * @details ライセンスはMITライセンスです
 * @version 0.1
 * @date 2023-01-01
 * 
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <vector>
#include "Arduino.h"
#include "Wire.h"

//  参考資料
//  https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf

/// @brief 関数の戻り値
enum class E_RESULT_9956
{
    OK, ///<    OK
    NG  ///<   NG
};

/// @brief 初期化命令
enum class E_LED_INIT
{
    ON_OFF,             ///<    ONOFF動作
    PWM                 ///<    PWM動作
};

/// @brief LEDに出す命令をパックした構造体
struct T_LEDOrder
{
    uint8_t ledno;      ///<    LED番号
    uint8_t ledgain;    ///<    LEDの明るさ(0=消灯)
};

/// @brief LEDに個別に供給電流を設定する場合
struct T_LEDCurrent
{
    uint8_t ledno;      ///<    LED番号
    uint8_t ledcurrent; //!<    電力(57mAを超えたら最大で固定)
};

/// @brief PCA9956に発行する命令のアドレス
/// @details    ON/OFFは無視する(全てPWNで代用)
enum class REG:uint8_t
{
    MODE1 = 0x00,   //!<    モード設定1
    MODE2,          //!<    モード設定2
    PWM0 = 0x0a,    //!<    PWN出力先0
    PWM1	,       //!<    PWM出力先1
    PWM2	,       //!<    PWM出力先2
    PWM3	,       //!<    PWM出力先3
    PWM4	,       //!<    PWM出力先4
    PWM5	,       //!<    PWM出力先5
    PWM6	,       //!<    PWM出力先6
    PWM7	,       //!<    PWM出力先7
    PWM8	,       //!<    PWM出力先8
    PWM9	,       //!<    PWM出力先9
    PWM10	,       //!<    PWM出力先10
    PWM11	,       //!<    PWM出力先11
    PWM12	,       //!<    PWM出力先12
    PWM13	,       //!<    PWM出力先13
    PWM14	,       //!<    PWM出力先14
    PWM15	,       //!<    PWM出力先15
    PWM16	,       //!<    PWM出力先16
    PWM17	,       //!<    PWM出力先17
    PWM18	,       //!<    PWM出力先18
    PWM19	,       //!<    PWM出力先19
    PWM20	,       //!<    PWM出力先20
    PWM21	,       //!<    PWM出力先21
    PWM22	,       //!<    PWM出力先22
    PWM23	,       //!<    PWM出力先23
    IREF0 = 0x22,   ///<    GAINの出力先(電流出力)0
    IREF1,	        ///<    GAINの出力先(電流出力)1
    IREF2,	        ///<    GAINの出力先(電流出力)2
    IREF3,	        ///<    GAINの出力先(電流出力)3
    IREF4,	        ///<    GAINの出力先(電流出力)4
    IREF5,	        ///<    GAINの出力先(電流出力)5
    IREF6,	        ///<    GAINの出力先(電流出力)6
    IREF7,	        ///<    GAINの出力先(電流出力)7
    IREF8,	        ///<    GAINの出力先(電流出力)8
    IREF9,	        ///<    GAINの出力先(電流出力)9
    IREF10,	        ///<    GAINの出力先(電流出力)10
    IREF11,	        ///<    GAINの出力先(電流出力)11
    IREF12,	        ///<    GAINの出力先(電流出力)12
    IREF13,	        ///<    GAINの出力先(電流出力)13
    IREF14,	        ///<    GAINの出力先(電流出力)14
    IREF15,	        ///<    GAINの出力先(電流出力)15
    IREF16,	        ///<    GAINの出力先(電流出力)16
    IREF17,	        ///<    GAINの出力先(電流出力)17
    IREF18,	        ///<    GAINの出力先(電流出力)18
    IREF19,	        ///<    GAINの出力先(電流出力)19
    IREF20,	        ///<    GAINの出力先(電流出力)20
    IREF21,	        ///<    GAINの出力先(電流出力)21
    IREF22,	        ///<    GAINの出力先(電流出力)22
    IREF23,	        ///<    GAINの出力先(電流出力)23
};

#define LED_CNT  24     //!<    LEDの個数

/** 
 * @brief PCA9956 LEDドライバークラス
 * 
 */
class PCA9956_LEDDrv
{
private:
    /* data */
    TwoWire         *_wire;
    const uint8_t   LED_ICURRENT_MAX = 57;      //!<    LEDの出力は最大57mA
    const uint8_t   LED_I_GAIN = 255;           //!<    LEDの供給電流の粒度
    const uint8_t   LED_PWM_MAX = 255;          //!<    LEDの調光の粒度
    uint8_t         _led_current[LED_CNT] = { 5 };  //!<    5mA程度をデフォルト値にする
                                                //      5mAなら、1005というLEDでも大抵は耐えられる
    uint8_t         _hard_addr = 0;             //!<    ボードのアドレス

    uint8_t         convItoGain(uint8_t current);                           //!<    LEDの電流をPCA9956Bのデータに変換する
    E_RESULT_9956   i2csend(uint8_t adr, uint8_t reg, uint8_t data);                 //!<    データをI2Cポートに送信する
    E_RESULT_9956   i2csend(uint8_t adr, uint8_t reg, std::vector<uint8_t> &data);  //!<    データをI2Cポートに送信する(複数データ)
    
public:
    PCA9956_LEDDrv(uint8_t hard_adr);
    ~PCA9956_LEDDrv();
    E_RESULT_9956 start(E_LED_INIT initorder);                       //!<    ドライバーを初期化する
    E_RESULT_9956 start(E_LED_INIT initorder, uint8_t icurrent);     //!<    ドライバーを初期化する(電流指定あり)
    E_RESULT_9956 led_off(uint8_t ledno);                           //!<    指定のLED番号をOFF
    E_RESULT_9956 led_on(uint8_t ledno);                            //!<    指定のLED番号をON
    E_RESULT_9956 led_pwn(T_LEDOrder &ledorder);                    //!<    指定のLED番号の明るさを指定
    E_RESULT_9956 led_pwn(std::vector<T_LEDOrder> &ledorder_lec);   //!<    指定のLED番号の明るさを指定(一括指定)
    E_RESULT_9956 led_setCurrent(uint8_t current);                  //!<    指定のLED番号の電流を指定
    E_RESULT_9956 led_setCurrent(T_LEDCurrent &current);            //!<    指定のLED番号の電流を指定(一括指定)    
};
