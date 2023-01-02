/**
 * @file pca9956.cpp
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

#include "Arduino.h"
#include "pca9956.h"

/// @brief コンストラクタ
/// @param hard_adr ボードのアドレスを指定する
/// @note   例えば、switch-scienceのPCA9956BTW I2C 24ch 電流源型LEDドライバ基板 であれば<br />初期値を0x7eとする
PCA9956_LEDDrv::PCA9956_LEDDrv(uint8_t hard_adr)
{
    _hard_addr = hard_adr;
    _wire = new TwoWire(hard_adr);
}

PCA9956_LEDDrv::~PCA9956_LEDDrv()
{
}

/// @brief          LEDの電流をPCA9956Bのデータに変換する
/// @param current  電流(57mmが最大値)
/// @return         0-255
/// @details        The output peak current is adjustable with an 8-bit linear DAC from 225 A to 57 mA.
///                 となっているので、実際は57.375mAが最大値になる?
uint8_t PCA9956_LEDDrv::convItoGain(uint8_t current)
{
    uint8_t gain = 0;
    if( current >= LED_ICURRENT_MAX){
        gain = LED_I_GAIN;
    }else{
        gain = (uint8_t)(current * ( LED_ICURRENT_MAX /  LED_I_GAIN));
    }

    return gain;
}

/// @brief              データをI2Cポートに送信する(複数データ)
/// @param reg_adr      データ送信の先頭アドレス
/// @param data         実際のデータ
/// @return             OK/NG
E_RESULT_9956 PCA9956_LEDDrv::i2csend(uint8_t adr, uint8_t reg, uint8_t data)
{
    _wire->beginTransmission(reg);   //アドレス設定

    _wire->write(reg);
    _wire->write(data);

    _wire->endTransmission();

    return E_RESULT_9956::OK;
}

/// @brief              データをI2Cポートに送信する(複数データ)
/// @param reg_adr      データ送信の先頭アドレス
/// @param data         実際のデータ(リスト)
/// @return             OK/NG
E_RESULT_9956 PCA9956_LEDDrv::i2csend(uint8_t adr, uint8_t reg,  std::vector<uint8_t> &data)
{
    int sndcnt = data.size();           //データ個数

    for(int i = 0; i < sndcnt; i++){
        i2csend(adr, reg, data[i]); //実際の送信は1個ずつになる
    }

    return E_RESULT_9956::OK;
}

