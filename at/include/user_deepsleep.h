#ifndef __USER_DEEPSLEEP_H__
#define __USER_DEEPSLEEP_H__

#define USER_SELLP_WAKEUP_BY_PGIO 1

#define FPM_SELLP_TIME(S) ((S*1000*1000))			// 0s - 268s  /0xFFFFFFF endless

void userEnterLightSleep(uint32 sec);
void userEnterModemSleep(uint32 sec);
void userEnterDeepSleep(uint32 sec);
void userModemSleepWakeUp(void);

#endif

