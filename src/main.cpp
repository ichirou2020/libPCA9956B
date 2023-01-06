/**
 * @file main.cpp
 * @author マゼピン
 * @brief 	作ったPCA9956B用のドライバがちゃんと動くかのサンプル
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * @defgroup	testsrc	ドライバテスト用メインソース
 * @{
 * 
 */

#include <Arduino.h>
#include "PCA9956_LEDDrv.h"
#include "testseq.h"

#if 0

//	ここのソースは以下のアドレスのパクリ
//	https://playground.arduino.cc/Main/I2cScanner/
//	i2cのアドレスにちゃんと繋がっているかの確認をするロジック
//	元のソースはSerial.begin(9600);だが、
//	platformio.iniにあわせて → Serial.begin(115200);　に修正
//	0x3f以外に、謎の0x70,0x77が検出される

// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>

void setup()
{
	Wire.begin();
 
	Serial.begin(115200);
	while (!Serial);             // Leonardo: wait for serial monitor
	Serial.println("\nI2C Scanner");
}
 
 
void loop()
{
	byte error, address;
	int nDevices;
 
	Serial.println("Scanning...");
 
	nDevices = 0;
	for(address = 1; address < 127; address++ )
	{
	// The i2c_scanner uses the return value of
	// the Write.endTransmisstion to see if
	// a device did acknowledge to the address.
	Wire.beginTransmission(address);
	error = Wire.endTransmission();
 
	if (error == 0)
	{
		Serial.print("I2C device found at address 0x");
		if (address<16)
		Serial.print("0");
		Serial.print(address,HEX);
		Serial.println("  !");
 
		nDevices++;
	}
	else if (error==4)
	{
		Serial.print("Unknown error at address 0x");
		if (address<16)
		Serial.print("0");
		Serial.println(address,HEX);
	}    
	}
	if (nDevices == 0)
	Serial.println("No I2C devices found\n");
	else
	Serial.println("done\n");
 
	delay(5000);           // wait 5 seconds for next scan
}

#else

// #pragma region 列挙体の計算用のオペレーター各種
// /// @brief 		REGのor計算をするためのオペレーター
// /// @param l 	or計算したいREG 列挙体その1
// /// @param r 	or計算したいREG 列挙体その2
// /// @return 	orの結果
// static REG operator|(REG l, REG r){
// 	return static_cast<REG>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
// }

// /// @brief 		REGに対してのアドレス計算をするためのオペレーター
// /// @param l 	plus計算したいREG列挙体
// /// @param r 	アドレスを加算したい数値
// /// @return 	計算結果
// static REG operator+(REG l, uint8_t r){
// 	return static_cast<REG>(static_cast<uint8_t>(l) + r);
// }
// #pragma endregion

static PCA9956_LEDDrv *_drv = new PCA9956_LEDDrv(0x3f);
T_LEDOrder _order ;

void setup() {
	Serial.begin(115200);
	_drv->start(20);		//テスト的に20mAにしているが5mAで充分明るい

	SetPCA9956Drv(_drv);	//メインソースを綺麗にするために、別のソースでドライバーを制御を
	AllOff();				//一旦全部LEDクリア

	delay(1000);

	//先頭の赤色LEDを50で点灯
	_order.ledno = (uint)0;
	_order.ledgain = 50;
	_drv->led_pwn(_order);

	//最後の青色LEDを255で点灯
	_order.ledno = (uint)(LED_CNT - 1);	
	_order.ledgain = LED_PWM_MAX;
	_drv->led_pwn(_order);
	delay(1000);
}

/// @brief メインループ
void loop() {
	AllOff();		//全部OFF
	delay(1000);
	AllRed();		//赤を徐々に明るく
	AllGreen();		//緑を徐々に明るく
	AllBlue();		//青を徐々に明るく
	AllOff();		//全部OFF
	delay(1000);
	PartRGB();		//デモ用に適当に光らせる
	delay(5000);
	AllOn();
	delay(1000);
}
#endif

//!	@}
