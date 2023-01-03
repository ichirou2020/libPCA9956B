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

/// @brief デストラクタ
/// @details 電源オフで止めてしまうのでデストラクタはいらない
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

/// @brief 			データをI2Cポートに送信するが、連続したデータとして送信する
/// @param adr 
/// @param reg 
/// @param data 
/// @return 
E_RESULT_9956 PCA9956_LEDDrv::i2csend_serial(REG reg, std::vector<uint8_t> &data)
{
	_wire->beginTransmission((uint8_t)_hard_addr);   //アドレス設定

	size_t dtsz = data.size();

    _wire->write((uint8_t)reg);
    _wire->write(data.data(), dtsz);

    _wire->endTransmission();

    return E_RESULT_9956::OK;	
}

/// @brief              データをI2Cポートに送信する(複数データ)
/// @param reg      	データ送信の先頭アドレス
/// @param data         実際のデータ
/// @return             OK/NG
E_RESULT_9956 PCA9956_LEDDrv::i2csend(REG reg, uint8_t data)
{
    _wire->beginTransmission((uint8_t)_hard_addr);   //アドレス設定

    _wire->write((uint8_t)reg);
    _wire->write(data);

    _wire->endTransmission();

    return E_RESULT_9956::OK;
}

/// @brief              データをI2Cポートに送信する(複数データ)
/// @param reg      	データ送信の先頭アドレス
/// @param data         実際のデータ(リスト)
/// @return             OK/NG
E_RESULT_9956 PCA9956_LEDDrv::i2csend(REG reg,  std::vector<uint8_t> &data)
{
    int sndcnt = data.size();           //データ個数

    for(int i = 0; i < sndcnt; i++){
        i2csend(reg, data[i]); //実際の送信は1個ずつになる(連続で送りたいが、模型で使うには多分バラバラに適当に送りつけるので連続は無理)
    }

    return E_RESULT_9956::OK;
}

/// @brief 				ドライバーを初期化する
/// @param initorder 	初期化指示
/// @return 			OK/NG
E_RESULT_9956 PCA9956_LEDDrv::start(E_LED_INIT initorder)
{
	start(initorder, LED_DEFAULT_CURRENT);

	return E_RESULT_9956::OK;
}

/// @brief 				ドライバーを初期化する(電流指定あり)
/// @param initorder 	初期化指示
/// @param icurrent 	電流
/// @return 			OK/NG
E_RESULT_9956 PCA9956_LEDDrv::start(E_LED_INIT initorder, uint8_t icurrent)
{

	uint8_t data1 = (uint8_t)MODE1_AUTO_INC::INC_IREF;
	uint8_t data2 = 0;
	std::vector<uint8_t> modedata;

	//モード設定
	modedata.push_back(data1);
	modedata.push_back(data2);
	i2csend_serial( REG::MODE1, modedata);

	//電流設定
	uint8_t current_gain = convItoGain(icurrent);	//0-57mA → 0-255
	std::vector<uint8_t> gainRec ;
	for(int i = 0; i < LED_CNT; i++){
		gainRec.push_back(current_gain);			//データ送信用に24個分のデータを作成する
	}
	i2csend_serial(REG::IREF0 | REG::MODEFLAG_INC, gainRec);

	return E_RESULT_9956::OK;
}

/// @brief 				指定のLED番号の明るさを指定
/// @param ledorder 	LEDの明度指定
/// @return 			OK/NG
E_RESULT_9956 PCA9956_LEDDrv::led_pwn(T_LEDOrder &ledorder)
{
	REG reg_adr = REG::PWM0 + ledorder.ledno;
	return i2csend(reg_adr | REG::MODEFLAG_INC, ledorder.ledgain);
}

/// @brief 				指定のLED番号の明るさを指定(複数一括指定)
/// @param ledorder_lec LEDの明度指定(複数レコード)
/// @return 			OK/NG
E_RESULT_9956 PCA9956_LEDDrv::led_pwn(std::vector<T_LEDOrder> &ledorder_lec)
{
	int cnt = ledorder_lec.size();
	for(int i = 0; i < cnt; i++){
		led_pwn(ledorder_lec[i]);
	}

	return E_RESULT_9956::OK;
}