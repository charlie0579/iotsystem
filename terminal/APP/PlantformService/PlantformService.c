/********************
* File, PlantformService.c
* Description, create mailbox, sem, memory manage...
* Author, William
* Date, 06-01-2016
************************************************/
#include "PlantformService.h"

OS_EVENT      *CommonMsg;

void CreateMailBox(void)
{
	CommonMsg = OSMboxCreate((void *)NULL);	
}
void CreatePlantFormCenter(void)
{
	CreateMailBox();
}

