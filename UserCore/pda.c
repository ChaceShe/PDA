/**
  ******************************************************************************
  * @file    pda.c
  * @brief   This file provides code for the configuration
  *          of the PDA instances.
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
#include "pda.h"



float signal_baseline = 5.5;       // 动态基线电压（初始值：无物体中位值）
uint32_t baseline_timer = 0;       // 基线更新计时器
uint32_t pulse_start_time = 0;     // 脉冲起始时间戳
uint32_t pulse_end_time = 0;       // 脉冲结束防抖计时器
uint16_t falling_object_count = 0; // 落物总计数
DetectState detect_state = DETECT_IDLE;          // 检测状态机 0=空闲 1=脉冲触发中 2=复位防抖

// 外部输入变量
extern float PDA_V;                      // 实时ADC采样电压
extern uint32_t time_tick_100us;           // 系统1ms时钟

/**
 * @brief  落物检测主函数（必须每1ms调用一次，与ADC采样同步）
 * @param  无
 * @retval 无
 */
void FallingObject_Detect(void)
{
    // ====================== 步骤1：动态基线校准（抵消低频噪声）======================
    if ((time_tick_100us - baseline_timer) >= BASELINE_UPDATE_CYCLE)
    {
        // 仅在空闲状态更新基线，避免脉冲信号干扰
//        if (detect_state == 0)
//        {
            // 一阶低通滤波，缓慢更新基线（抗干扰）
            signal_baseline = signal_baseline * 0.9f + PDA_V * 0.1f;
//        }
        baseline_timer = time_tick_100us;
    }

    // ====================== 步骤2：状态机检测落物脉冲 ======================
    switch (detect_state)
    {
        case DETECT_IDLE: // 【空闲状态】等待有效负向脉冲
        {
            // 判定条件：电压低于基线 - 跌落阈值（落物负向脉冲触发）
            if (PDA_V < (signal_baseline - DROP_THRESHOLD))
            {
                pulse_start_time = time_tick_100us;  // 记录脉冲起始时间
                detect_state = DETECT_PULSE;                 // 切换到脉冲检测状态
            }
            break;
        }

        case DETECT_PULSE: // 【脉冲检测中】验证脉冲宽度是否有效
        {
            uint32_t pulse_width = time_tick_100us - pulse_start_time;

            // 条件1：脉冲超时（超过110ms，判定为干扰，复位）
            if (pulse_width > PULSE_MAX_WIDTH)
            {
                detect_state = DETECT_IDLE;
            }
            // 条件2：脉冲结束 + 宽度在有效范围内（50±10ms）→ 判定为落物
            else if ((PDA_V >= (signal_baseline - DROP_THRESHOLD)) && 
                     (pulse_width >= PULSE_MIN_WIDTH))
            {
                falling_object_count++;  // 落物计数+1
                pulse_end_time = time_tick_100us;
                detect_state = DETECT_VALIDATE;       // 切换到防抖状态
            }
            break;
        }

        case DETECT_VALIDATE: // 【复位防抖】防止短时间重复计数
        {
            if ((time_tick_100us - pulse_end_time) >= DEBOUNCE_TIME)
            {
                detect_state = DETECT_IDLE;  // 防抖结束，回到空闲状态
            }
            break;
        }

        default:
            detect_state = DETECT_IDLE;
            break;
    }
}
