/**
  ******************************************************************************
  * @file    vofa.c
  * @brief   This file provides code for the configuration
  *          of the VOFA instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
	/* Includes ------------------------------------------------------------------*/
#include "vofa.h"

// VOFA+ JustFloat固定帧尾
static const uint8_t VOFA_TAIL[4] = {0x00, 0x00, 0x80, 0x7f};

/**
 * @brief  发送主函数（必须每1ms调用一次，与ADC采样同步）
 * @param  无
 * @retval 无
 */


void Vofa_JustFloat(float *_data, uint8_t _num)
{
  const uint8_t data_len = _num * 4U;
  const uint8_t total_len = data_len + 4U;
  uint8_t tx_buf[8];

  // 拷贝浮点数组到发送缓冲区
  memcpy(tx_buf, _data, data_len);
  // 拼接帧尾到缓冲区末尾
  memcpy(&tx_buf[data_len], VOFA_TAIL, 4U);

  HAL_UART_Transmit_DMA(&huart1, tx_buf, (_num + 1) * 4);

}

