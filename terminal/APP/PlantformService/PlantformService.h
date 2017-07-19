/********************
* File, PlantformService.h
* Description, create mailbox, sem, memory manage...
* Author, William
* Date, 06-01-2016
************************************************/
#ifndef _PLANTFORM_SERVICE_
#define _PLANTFORM_SERVICE_

#include "includes.h"
#include "stm32f10x.h"

extern OS_EVENT *CommonMsg;
void CreatePlantFormCenter(void);
void CreateMailBox(void);

#endif

