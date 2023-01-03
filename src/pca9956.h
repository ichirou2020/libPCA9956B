/**
 * @file pca9956.h
 * @author マゼピン
 * @brief LEDドライバー(PCA9956B)
 * @details ライセンスはMITライセンスです
 * @version 0.1
 * @date 2023-01-01
 *
 *
 * @copyright Copyright (c) マゼピン	2023
 * @defgroup pca9956	PCA9956ライブラリ
 * @{
 *
 */

#pragma once

#include <vector>
#include "Arduino.h"
#include "Wire.h"

//  参考資料
//  https://www.nxp.com/docs/en/data-sheet/PCA9956B.pdf

#pragma region 列挙体
///	@brief LEDOUTに設定する値
enum class LEDOUT
{
	DRV_OFF 		= B00,	//!<	LED driver x is off (x = 0 to 23)<br />
							//!<	LED強制OFF?
	DRV_FULLY_ON 	= B01,	//!<	LED driver x is fully on (individual brightness and group dimming/blinking not controlled). The OE pin can be used as external dimming/blinking control in this state<br />
							//!<	LED強制ON?
	DRV_PWM			= B10,	//!<	LED driver x individual brightness can be controlled through its PWMx register (default power-up state) or PWMALL register for all LEDn outputs.<br />
							//!<	LEDはPMWのコントロール可能な状態で、PWMALLでもまとめてコントロールできる?
	DRV_PWM_GRP		= B11	//!<	LED driver x individual brightness and group dimming/blinking can be controlled through its PWMx register and the GRPPWM registers.	<br />
							//!<	LEDはPWMのコントロール可能な状態で、点滅やGRPPWMデモコントロール可能?
};

/// @brief PCA9956に発行する命令のアドレス
enum class REG : uint8_t
{
	MODE1 = 0x00,	//!<	モード設定1
	MODE2,			//!<	モード設定2
	LEDOUT0 = 0x02, //!<	LED設定0(通常のPWMコントロールするだけならデフォルト設定が[10]なので設定の必要が無いかも)
					//!<	@details 7:6 LED3 output state control<br />
					//!<	5:4 LED2 output state control<br />
					//!<	3:2 LED1 output state control<br />
					//!<	1:0 LED0 output state control<br />
					//!<	設定する値は @ref LEDOUT を参照すること(以下LEDOUT5まで同様)
	LEDOUT1,		//!<	LED設定1
					//!<	@details 7:6 LED7 output state control<br />
					//!<	5:4 LED6 output state control<br />
					//!<	3:2 LED5 output state control<br />
					//!<	1:0 LED4 output state control
	LEDOUT2,		//!<	LED設定2
					//!<	@details 7:6 LED11 output state control<br />
					//!<	5:4 LED10 output state control<br />
					//!<	3:2 LED9 output state control<br />
					//!<	1:0 LED8 output state control
	LEDOUT3,		//!<	LED設定3
					//!<	@details 7:6 LED15 output state control<br />
					//!<	5:4 LED14 output state control<br />
					//!<	3:2 LED13 output state control<br />
					//!<	1:0 LED12 output state control
	LEDOUT4,		//!<	LED設定4
					//!<	@details 7:6 LED19 output state control<br />
					//!<	5:4 LED18 output state control<br />
					//!<	3:2 LED17 output state control<br />
					//!<	1:0 LED16 output state control
	LEDOUT5,		//!<	LED設定5
					//!<	@details 7:6 LED23 output state control<br />
					//!<	5:4 LED22 output state control<br />
					//!<	3:2 LED21 output state control<br />
					//!<	1:0 LED20 output state control
	GRPPWM = 0x08,	//!<	group duty cycle control
	GRPFREQ = 0x09,	//!<	group frequency
	PWM0 = 0x0a,	//!<	PWN出力先0
	PWM1,			//!<	PWM出力先1
	PWM2,			//!<	PWM出力先2
	PWM3,			//!<	PWM出力先3
	PWM4,			//!<	PWM出力先4
	PWM5,			//!<	PWM出力先5
	PWM6,			//!<	PWM出力先6
	PWM7,			//!<	PWM出力先7
	PWM8,			//!<	PWM出力先8
	PWM9,			//!<	PWM出力先9
	PWM10,			//!<	PWM出力先10
	PWM11,			//!<	PWM出力先11
	PWM12,			//!<	PWM出力先12
	PWM13,			//!<	PWM出力先13
	PWM14,			//!<	PWM出力先14
	PWM15,			//!<	PWM出力先15
	PWM16,			//!<	PWM出力先16
	PWM17,			//!<	PWM出力先17
	PWM18,			//!<	PWM出力先18
	PWM19,			//!<	PWM出力先19
	PWM20,			//!<	PWM出力先20
	PWM21,			//!<	PWM出力先21
	PWM22,			//!<	PWM出力先22
	PWM23,			//!<	PWM出力先23
	IREF0 = 0x22,	///<	GAINの出力先(電流出力)0
	IREF1,			///<	GAINの出力先(電流出力)1
	IREF2,			///<	GAINの出力先(電流出力)2
	IREF3,			///<	GAINの出力先(電流出力)3
	IREF4,			///<	GAINの出力先(電流出力)4
	IREF5,			///<	GAINの出力先(電流出力)5
	IREF6,			///<	GAINの出力先(電流出力)6
	IREF7,			///<	GAINの出力先(電流出力)7
	IREF8,			///<	GAINの出力先(電流出力)8
	IREF9,			///<	GAINの出力先(電流出力)9
	IREF10,			///<	GAINの出力先(電流出力)10
	IREF11,			///<	GAINの出力先(電流出力)11
	IREF12,			///<	GAINの出力先(電流出力)12
	IREF13,			///<	GAINの出力先(電流出力)13
	IREF14,			///<	GAINの出力先(電流出力)14
	IREF15,			///<	GAINの出力先(電流出力)15
	IREF16,			///<	GAINの出力先(電流出力)16
	IREF17,			///<	GAINの出力先(電流出力)17
	IREF18,			///<	GAINの出力先(電流出力)18
	IREF19,			///<	GAINの出力先(電流出力)19
	IREF20,			///<	GAINの出力先(電流出力)20
	IREF21,			///<	GAINの出力先(電流出力)21
	IREF22,			///<	GAINの出力先(電流出力)22
	IREF23,			///<	GAINの出力先(電流出力)23
	//
	MODEFLAG_INC = B10000000 	//!<	インクリメントモードのフラグ
};

/// @brief オートインクリメントオプション
/// @details MODE1に設定する
///	@note 	Table 6. Auto-Increment options 参照
enum class MODE1_AUTO_INC
{
	NO_AUTO = 	B00000000,	//!<	no Auto-Increment
							//!<	@details 	全データを毎回送信?
	INC_ALL = 	B10000000,	//!<	Auto-Increment for registers (00h to 3Eh). D[6:0] roll over to 00h after the last register 3Eh is accessed
							//!<	@details	全データ領域でインクリメントモード?
	INC_PWM =	B10100000,	//!<	Auto-Increment for individual brightness registers only (0Ah to 21h).D[6:0] roll over to 0Ah after the last register (21h) is accessed
							//!<	@details	PWMの設定領域でインクリメントモード
	INC_IREF=	B11000000,	//!<	Auto-Increment for MODE1 to IREF23 control registers (00h to 39h). D[6:0] roll over to 00h after the last register (39h) is accessed.
							//!<	@details 	MODE1～IREF23(電流設定の最後)までインクリメントモード(最初にこれを投げる?)
	INC_PWM2=	B11100000	//!<	Auto-Increment for global control registers and individual brightness registers (08h to 21h). D[6:0] roll over to 08h after the last register (21h) is accessed.
							//!< 	@details 	GRPPWM,GRPFREQ～PWMの設定領域でインクリメントモード
};

/// @brief 関数の戻り値
enum class E_RESULT_9956
{
	OK, ///<	OK
	NG	///<	NG
};

/// @brief 初期化命令
enum class E_LED_INIT
{
	ON_OFF, ///<	ONOFF動作
	PWM		///<	PWM動作
};
#pragma endregion 列挙体

#pragma region 列挙体の計算用のオペレーター各種

/// @brief 		REGのor計算をするためのオペレーター
/// @param l 	or計算したいREG 列挙体その1
/// @param r 	or計算したいREG 列挙体その2
/// @return 	orの結果
REG operator|(REG l, REG r){
	return static_cast<REG>(static_cast<uint8_t>(l) | static_cast<uint8_t>(r));
}

/// @brief 		REGに対してのアドレス計算をするためのオペレーター
/// @param l 	plus計算したいREG列挙体
/// @param r 	アドレスを加算したい数値
/// @return 	計算結果
REG operator+(REG l, uint8_t r){
	return static_cast<REG>(static_cast<uint8_t>(l) + r);
}

#pragma endregion

#pragma region 構造体
/// @brief LEDに出す命令をパックした構造体
struct T_LEDOrder
{
	uint8_t ledno;	 ///<	LED番号
	uint8_t ledgain; ///<	LEDの明るさ(0=消灯)
};

/// @brief LEDに個別に供給電流を設定する場合
struct T_LEDCurrent
{
	uint8_t ledno;		///<	LED番号
	uint8_t ledcurrent; //!<	電力(57mAを超えたら最大で固定)
};

#pragma endregion 構造体

//	------------------------------------	定数
#define LED_CNT 24 //!<	LEDの個数

/**
 * @brief PCA9956 LEDドライバークラス
 *
 */
class PCA9956_LEDDrv
{
private:
#pragma region	プライベート
	/* data */
	TwoWire *_wire;							//!<	I2Cライブラリ
	const uint8_t LED_ICURRENT_MAX = 57; 	//!<	LEDの出力は最大57mA
	const uint8_t LED_I_GAIN = 255;		 	//!<	LEDの供給電流の粒度
	const uint8_t LED_PWM_MAX = 255;	 	//!<	LEDの調光の粒度
	const uint8_t LED_DEFAULT_CURRENT = 5;	//!<	5mA程度をデフォルト値にする<br />
											//!<	5mAなら、1005というSMD LEDでも大抵は耐えられる
	//uint8_t _led_current[LED_CNT] = {5}; 	//!<	5mA程度をデフォルト値にする<br />
	//									 	//!<  	5mAなら、1005というSMD LEDでも大抵は耐えられる
	uint8_t _hard_addr = 0;				 	//!<	ボードのアドレス

	uint8_t convItoGain(uint8_t current);							//!<	LEDの電流をPCA9956Bのデータに変換する
	E_RESULT_9956 i2csend(REG reg, uint8_t data);				 	//!<	データをI2Cポートに送信する
	E_RESULT_9956 i2csend(REG reg, std::vector<uint8_t> &data); 	//!<	データをI2Cポートに送信する(複数データ)
	E_RESULT_9956 i2csend_serial(REG reg, std::vector<uint8_t> &data);	//!<	データをI2Cポートに送信するが、連続したデータとして送信する

	//オペレーター


#pragma endregion
public:
	PCA9956_LEDDrv(uint8_t hard_adr);
	~PCA9956_LEDDrv();
	E_RESULT_9956 start(E_LED_INIT initorder);					  //!<	ドライバーを初期化する
	E_RESULT_9956 start(E_LED_INIT initorder, uint8_t icurrent);  //!<	ドライバーを初期化する(電流指定あり)
	E_RESULT_9956 led_off(uint8_t ledno);						  //!<	指定のLED番号をOFF
	E_RESULT_9956 led_on(uint8_t ledno);						  //!<	指定のLED番号をON
	E_RESULT_9956 led_pwn(T_LEDOrder &ledorder);				  //!<	指定のLED番号の明るさを指定
	E_RESULT_9956 led_pwn(std::vector<T_LEDOrder> &ledorder_lec); //!<	指定のLED番号の明るさを指定(複数一括指定)
	E_RESULT_9956 led_setCurrent(uint8_t current);				  //!<	指定のLED番号の電流を指定
	E_RESULT_9956 led_setCurrent(T_LEDCurrent &current);		  //!<	指定のLED番号の電流を指定(一括指定)
};

//!	@}
