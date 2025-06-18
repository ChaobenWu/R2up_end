#ifndef __SYSTEM_MONITOR_H__
#define __SYSTEM_MONITOR_H__

#include "types.h"
void System_Monitor(ST_SYSTEM_MONITOR *monitor);
void Monitor(uint16_t rps,uint16_t *error,uint16_t decision);

#endif //__SYSTEM_MONITOR_H__
