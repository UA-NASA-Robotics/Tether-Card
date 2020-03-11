#include "HandleComms.h"
#include "CommsIDs.h"
#include "mcc_generated_files/can1.h"
#include "uart1_config.h"
#include "FastTransfer_CAN.h"
#include "FastTransfer/ft.h"

FT_t uart_handle;
FTC_t can_handle;

void InitComms(void) {
    FT_Init(&uart_handle, TETHER_CARD, uart1_put_c, uart1_get, uart1_tx_empty);
    FTC_Init(&can_handle, TETHER_CARD, 1, CAN1_Initialize, CAN1_transmit, CAN1_receive);
}

void CommunicationsHandle(void) {
    /*
    if (FT_Receive(&uart_handle))
    {

    }
    if (ReceiveDataCAN(&can_handle, FT_LOCAL))
    {

    }
    if (ReceiveDataCAN(&can_handle, FT_GLOBAL))
    {

    }
    */
}