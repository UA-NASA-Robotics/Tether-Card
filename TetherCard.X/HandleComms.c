#include "HandleComms.h"
#include "CommsIDs.h"
#include "mcc_generated_files/can1.h"
#include "uart1_config.h"
#include "FastTransfer_CAN.h"
#include "FastTransfer/ft.h"
#include "timers.h "

//CAN INDEXs
#define CAN_COMMAND_INDEX       8
#define MACRO_COMMAND_INDEX   CAN_COMMAND_INDEX
#define CAN_COMMAND_DATA_INDEX  9
// starting index of the reserved portion of the local FT CAN rx array for manual commands
#define MANUAL_COMMAND_START_INDEX 4


FT_t uart_handle;
FTC_t can_handle;
timer_t can_timer;

void InitComms(void) {
    FT_Init(&uart_handle, TETHER_CARD, uart1_put_c, uart1_get, uart1_tx_empty);
    FTC_Init(&can_handle, TETHER_CARD, 1, CAN1_Initialize, CAN1_transmit, CAN1_receive);
    setTimerInterval(&can_timer, 200);
}

void CommunicationsHandle(void) {
    // if Tether-Card is addressed directly on the primary robot's CAN bus (FT_LOCAL),
    // a manual command is being sent: forward it to secondary robot's router card
    if (ReceiveDataCAN(&can_handle, FT_LOCAL))
    {
        int i = 0;
        for (i = MANUAL_COMMAND_START_INDEX; i < CAN_COMMAND_DATA_INDEX; i++) {
            FT_ToSend(&uart_handle, i, getCANFTdata(&can_handle, i));
        }
        FT_Send(&uart_handle, ROUTER_CARD);
    }
    // get all macros on the primary robot's global CAN bus partition and foward it to secondary robot's router card
    if (ReceiveDataCAN(&can_handle, FT_GLOBAL))
    {
        int tx_macro = 0x0000;
        // Set the corresponding macro bit to 1 if active for all devices on the GBL bus
        int i;
        for(i = 1; i<= GLOBAL_DEVICES+1; i++)
        {
            if(getGBL_CANFTdata(&can_handle, getGBL_MACRO_INDEX(i))) {
                tx_macro |= getGBL_CANFTdata(&can_handle, getGBL_MACRO_INDEX(i));
            }
        }
        FT_ToSend(&uart_handle, MACRO_COMMAND_INDEX, tx_macro);
        FT_Send(&uart_handle, ROUTER_CARD);  
    }
    // if data was sent from the secondary robot, publish it on the CAN bus global partition on the primary robot
    if (FT_Receive(&uart_handle))
    {
        // send macro command on a timer to prevent flooding the global CAN bus
        if (timerDone(&can_timer, 1))
        {
            FTC_ToSend(&can_handle, getGBL_MACRO_INDEX(TETHER_CARD), FT_Read(&uart_handle, MACRO_COMMAND_INDEX));
            FTC_ToSend(&can_handle, getGBL_MACRO_INDEX(TETHER_CARD) + 1, FT_Read(&uart_handle, CAN_COMMAND_DATA_INDEX));
            FTC_Send(&can_handle, GLOBAL_ADDRESS);
        }
    }
}