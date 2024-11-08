/*
 * scheduler.c
 *
 *  Created on: Nov 5, 2024
 *      Author: Admin
 */

#include "scheduler.h"

struct node{
	sTasks task;
    struct node *next;
    struct node *prev;
};
typedef struct node node;

struct LIST{
    node *head;
    node *tail;
};

typedef struct LIST LIST;

LIST SCH_Tasks_G = {NULL, NULL};
uint8_t current_index_task = 0;


void SCH_Delete_Task();

sTasks makeTask(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	sTasks newTask;
	newTask.pTask = pFunction;
	newTask.Delay = DELAY;
	newTask.Period = PERIOD;

	newTask.RunMe = 0;
	current_index_task++;
	return newTask;
}

void SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	node *newNode = (node*)malloc(sizeof(node));
	newNode->task = makeTask(pFunction, DELAY, PERIOD);
	newNode->next = NULL;
	newNode->prev = NULL;

	if(SCH_Tasks_G.head == NULL && SCH_Tasks_G.tail == NULL){
		SCH_Tasks_G.head = SCH_Tasks_G.tail = newNode;
		return;
	}

	if(newNode->task.Delay <= SCH_Tasks_G.head->task.Delay){
		//Update Delay
		SCH_Tasks_G.head->task.Delay -= newNode->task.Delay;
		//Insert First
		newNode->next = SCH_Tasks_G.head;
		SCH_Tasks_G.head->prev = newNode;
		SCH_Tasks_G.head = newNode;
	}else{
		node* tmp = SCH_Tasks_G.head;
		//update new value for Delay
		while(tmp != NULL	&&	newNode->task.Delay > tmp->task.Delay){
			newNode->task.Delay -= tmp->task.Delay;
			tmp = tmp->next;
		}

		if(tmp != NULL){
			//Insert Middle
			newNode->prev = tmp->prev;
			newNode->next = tmp;
			tmp->prev->next = newNode;
			tmp->prev = newNode;

			//Minus the following task
			tmp->task.Delay -= newNode->task.Delay;
		}else{
			//Insert Last
			SCH_Tasks_G.tail->next = newNode;
			newNode->prev = SCH_Tasks_G.tail;
			SCH_Tasks_G.tail = newNode;
		}
	}
}

void SCH_Update(void){
	if(SCH_Tasks_G.head->task.Delay > 0){
		SCH_Tasks_G.head->task.Delay--;
	}else{
		SCH_Tasks_G.head->task.RunMe = 1;
	}
}

void SCH_Dispatch_Tasks(void){
	if(SCH_Tasks_G.head->task.RunMe > 0){
		SCH_Tasks_G.head->task.RunMe--;
		(*SCH_Tasks_G.head->task.pTask)();

		node *copyNode = (node*)malloc(sizeof(node));
		copyNode->task = makeTask(SCH_Tasks_G.head->task.pTask, SCH_Tasks_G.head->task.Period, SCH_Tasks_G.head->task.Period);
		copyNode->next = NULL;
		copyNode->prev = NULL;
		SCH_Delete_Task();

		if(copyNode->task.Period > 0){
			//ADD TASK AGAIN
			if(SCH_Tasks_G.head == NULL && SCH_Tasks_G.tail == NULL){
				SCH_Tasks_G.head = SCH_Tasks_G.tail = copyNode;
				return;
			}

			if(copyNode->task.Delay <= SCH_Tasks_G.head->task.Delay){
				//Update Delay
				SCH_Tasks_G.head->task.Delay -= copyNode->task.Delay;
				//Insert First
				copyNode->next = SCH_Tasks_G.head;
				SCH_Tasks_G.head->prev = copyNode;
				SCH_Tasks_G.head = copyNode;
			}else{
				node* tmp = SCH_Tasks_G.head;
				//update new value for Delay
				while(tmp != NULL	&&	copyNode->task.Delay > tmp->task.Delay){
					copyNode->task.Delay -= tmp->task.Delay;
					tmp = tmp->next;
				}

				if(tmp != NULL){
					//Insert Middle
					copyNode->prev = tmp->prev;
					copyNode->next = tmp;
					tmp->prev->next = copyNode;
					tmp->prev = copyNode;

					//Minus the following task
					tmp->task.Delay -= copyNode->task.Delay;
				}else{
					//Insert Last
					SCH_Tasks_G.tail->next = copyNode;
					copyNode->prev = SCH_Tasks_G.tail;
					SCH_Tasks_G.tail = copyNode;
				}
			}
		}
	}
}

void SCH_Delete_Task(){
	if(SCH_Tasks_G.head == NULL){
		return;
	}
	node* tmp = SCH_Tasks_G.head;
	SCH_Tasks_G.head = SCH_Tasks_G.head->next;
	if(SCH_Tasks_G.head){
		SCH_Tasks_G.head->prev = NULL;
	}
	free(tmp);
}
//sTasks SCH_Tasks_G[SCH_MAX_TASKS];
//uint8_t current_index_task = 0;

//void SCH_Init(void){
//	current_index_task = 0;
//}
//
//void SCH_Add_Task (	void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
//	if(current_index_task < SCH_MAX_TASKS){
//		SCH_Tasks_G[current_index_task].pTask = pFunction;
//		SCH_Tasks_G[current_index_task].Delay = DELAY;
//		SCH_Tasks_G[current_index_task].Period = PERIOD;
//		SCH_Tasks_G[current_index_task].RunMe = 0;
//
//		SCH_Tasks_G[current_index_task].TaskID = current_index_task;
//
//		current_index_task++;
//	}
//}
//
//void SCH_Update(void){
//	for(int i = 0; i < current_index_task; i++){
//		if(SCH_Tasks_G[i].Delay > 0){
//			SCH_Tasks_G[i].Delay--;
//		}else{
//			SCH_Tasks_G[i].Delay = SCH_Tasks_G[i].Period;
//			SCH_Tasks_G[i].RunMe = 1;
//		}
//	}
//}
//
//void SCH_Dispatch_Tasks(void){
//	for(int i = 0; i < current_index_task; i++){
//		if(SCH_Tasks_G[i].RunMe > 0){
//			SCH_Tasks_G[i].RunMe--;
//			(*SCH_Tasks_G[i].pTask)();
//		}
//	}
//}
