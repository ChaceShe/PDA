/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PDA_H__
#define __PDA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
	
typedef enum {
    DETECT_IDLE,
    DETECT_PULSE,
    DETECT_VALIDATE
} DetectState;

#define BASELINE_UPDATE_CYCLE    500    // 基线更新周期 500ms（适配<1Hz低频噪声）
#define DROP_THRESHOLD           0.4f    // 负向跌落阈值（V）：电压下降超过0.4V判定为触发
#define PULSE_MIN_WIDTH          40     // 最小有效脉冲宽度 40ms（容错）
#define PULSE_MAX_WIDTH          110    // 最大有效脉冲宽度 110ms（容错）
#define DEBOUNCE_TIME            20     // 复位防抖时间 20ms（防止重复计数）

extern float signal_baseline;
extern DetectState detect_state;
extern uint32_t baseline_timer;
extern uint32_t pulse_start_time;
extern uint32_t pulse_end_time;
extern uint16_t falling_object_count;


#ifdef __cplusplus
}
#endif

#endif /* __PDA_H__ */
