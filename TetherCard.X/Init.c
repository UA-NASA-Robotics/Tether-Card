#include "Init.h"
#include "timers.h"
#include "uart1_config.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/can1.h"

void init(void) {
    SYSTEM_Initialize();
    timer1_init();
    uart1_init();
//    TRISBbits.TRISB10 = 0;
    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
}