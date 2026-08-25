
#ifndef COMPLIANCE_ETH_H
#define COMPLIANCE_ETH_H

#include "ZephyrEthernet.h"
#include <ZephyrClient.h>

#define LOG_ETH_TASK
void init_eth();
void eth_task();
#endif
