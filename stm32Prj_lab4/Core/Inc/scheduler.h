/*
 * scheduler.h
 *
 *  Created on: Nov 5, 2024
 *      Author: Admin
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct{
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint32_t RunMe;

	uint32_t TaskID;

	//struct sTasks *next;
}sTasks;

#define	SCH_MAX_TASKS 	10

void SCH_Init(void);

//SetTimer
void SCH_Add_Task (	void (*pFunction)(),
					uint32_t DELAY,
					uint32_t PERIOD);

//TimerRun
void SCH_Update(void);

//TimerFlag
void SCH_Dispatch_Tasks(void);

#endif /* INC_SCHEDULER_H_ */
