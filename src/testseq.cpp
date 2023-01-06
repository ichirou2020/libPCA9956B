/**
 * @file testseq.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "testseq.h"

static PCA9956_LEDDrv *_drv = nullptr;

/// @brief 		PCA9956B用ドライバーを使えるようにする
/// @param drv 	ドライバーオブジェクト
void SetPCA9956Drv(PCA9956_LEDDrv* drv)
{
	_drv = drv;
}

/// @brief 全消灯
void AllOff()
{
	//LEDを全部消灯
	for(int i = 0 ; i < LED_CNT; i++){
		_drv->led_off((uint8_t)i);
	}
}

/// @brief 赤を徐々に明るく
void AllRed()
{
	T_LEDOrder order;

	for (int gain = 0; gain <= LED_PWM_MAX; gain++)
	{
		for (int i = 0; i < LED_CNT; i+=3)
		{
			// 赤だけ徐々に明るくする
			order.ledno = (uint)i;
			order.ledgain = gain;
			_drv->led_pwn(order);
		}
		delay(10);
	}
}

/// @brief 緑を徐々に明るく
void AllGreen()
{
	T_LEDOrder order;

	for (int gain = 0; gain <= LED_PWM_MAX; gain++)
	{
		for (int i = 1; i < LED_CNT; i+=3)
		{
			// 緑だけ徐々に明るくする
			order.ledno = (uint)i;
			order.ledgain = gain;
			_drv->led_pwn(order);
		}
		delay(10);
	}	
}

/// @brief 青を徐々に明るく
void AllBlue()
{
	T_LEDOrder order;

	for (int gain = 0; gain <= LED_PWM_MAX; gain++)
	{
		for (int i = 2; i < LED_CNT; i+=3)
		{
			// 青だけ徐々に明るくする
			order.ledno = (uint)i;
			order.ledgain = gain;
			_drv->led_pwn(order);
		}
		delay(10);
	}	
}

/// @brief デモ用に適当に光らせる
void PartRGB()
{
	T_LEDOrder order[] = {{0,100},{4,200},{8,50},{11,200},{13,100},{15,40}
			,{18,100},{19,100},{20,100}
			,{21,100},{22,200},{23,20}};
	std::vector<T_LEDOrder> stdOrder;
	for(int i = 0; i < 12; i++){
		stdOrder.push_back(order[i]);
	}
	_drv->led_pwn(stdOrder);
}

/// @brief 一気に全部フル点灯
void AllOn()
{
	for(int i = 0; i < LED_CNT; i++){
		_drv->led_on((uint8_t)i);
	}
}