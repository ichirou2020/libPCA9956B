/**
 * @file testseq.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-06
 * 
 * @copyright Copyright (c) 2023
 * @addtogroup testsrc
 * @{
 */

#pragma once

#include "PCA9956_LEDDrv.h"

void SetPCA9956Drv(PCA9956_LEDDrv* drv);
void AllOff();
void AllRed();
void AllGreen();
void AllBlue();
void PartRGB();
void AllOn();

//!	@}
