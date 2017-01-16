#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
#include "mbed.h"

UINT32 g_TestTskHandle;

//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

//Serial pc(SERIAL_TX, SERIAL_RX);

DigitalOut myled(LED1);
DigitalIn dserven(D7);
DigitalIn dfour(D4);

void hardware_init(void)
{
	;
}
// ҵ���߼�
void task1(void)
{
    while(1) {
			// ����
			int i = !dserven;
			// �����
			int j = !dfour;
			// ������ֻ��棬������˸ 3��
      if(i){
				myled = i;
				
				wait(0.2);
				
				myled = 0;
				
				wait(0.2);
				
				myled = i;
				
				wait(0.2);
				
				myled = 0;
				
				wait(0.2);
				
				myled = i;
				
			}
			// �����������������˸ 3��
			if(j){
				myled = j;
				
				wait(1);
				
				myled = 0;
				
				wait(1);
				
				myled = j;
				
				wait(1);
				
				myled = 0;
				
				wait(1);
				
				myled = j;
				
			}
			
			if(!i && !j) {
				myled = 0;
			}
			
			
			wait(0.2);
    }
}
// ����Lite OS ����
UINT32 create_task1(void)
{
	UINT32 uwRet ;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task1";
	// ����ҵ���߼�
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle, &task_init_param);

	return uwRet;
}

// ��������
UINT32 osAppInit(void)
{
	UINT32 uwRet;
	hardware_init();
	uwRet = create_task1();
	if (uwRet != LOS_OK)
	{
		return uwRet;
	}
	return LOS_OK;
}

