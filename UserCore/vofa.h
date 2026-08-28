/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VOFA_H__
#define __VOFA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
	
typedef union
{
    float Fdata;         //以浮点数形式读取变量
    
}Vofa_Type;

extern float signal_baseline;
extern uint32_t baseline_timer;
extern uint32_t pulse_start_time;
extern uint32_t pulse_end_time;
extern uint16_t falling_object_count;


#ifdef __cplusplus
}
#endif

#endif /* __VOFA_H__ */
