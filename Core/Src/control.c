#include "control.h"
#include "main.h"
#include "relay.h"

extern Computer_Port_t computer_port[PCPORTCOUNT];

void Computer_CtrlPort_Init(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (((PCPORT >> i) & 0x01) == 1) {
            computer_port[count].relay_number = 0x01 << i;
            computer_port[count].command = CMD_NULL;
            computer_port[count].port_status = PORT_POWEROFF;
            computer_port[count].exec_result = EXEC_UNREADY;
            count++;
        }
    }
}

void Projector_CtrlPort_Init(void)
{}

void Computer_PowerChange(void)
{
    for (int i = 0; i < PCPORTCOUNT; i++) {
        switch (computer_port[i].command) {
            case CMD_ON:
                if ((computer_port[i].port_status == PORT_POWEROFF)) {
                    Relay_Pullin(computer_port[i].relay_number);
                    computer_port[i].port_status = PORT_RELAYPULLIN;
                    computer_port[i].exec_result = EXEC_AFTER_ON;
                    computer_port[i].relay_PULLIN_tim = HAL_GetTick();
                }
                if ((computer_port[i].port_status == PORT_RELAYPULLIN) \
                    && ((HAL_GetTick() - computer_port[i].relay_PULLIN_tim) > POWER_PC_TIM)) {
                    Relay_Release(computer_port[i].relay_number);
                    computer_port[i].command = CMD_NULL;
                    computer_port[i].port_status = PORT_POWERON;
                    computer_port[i].exec_result = EXEC_UNREADY;
                    computer_port[i].relay_PULLIN_tim = 0;
                }
                break;
            case CMD_OFF:
                if (computer_port[i].port_status == PORT_POWERON) {
                    Relay_Pullin(computer_port[i].relay_number);
                    computer_port[i].port_status = PORT_RELAYPULLIN;
                    computer_port[i].exec_result = EXEC_AFTER_OFF;
                    computer_port[i].relay_PULLIN_tim = HAL_GetTick();
                }
                if ((computer_port[i].port_status == PORT_RELAYPULLIN) \
                    && ((HAL_GetTick() - computer_port[i].relay_PULLIN_tim) > POWER_PC_TIM)) {
                    Relay_Release(computer_port[i].relay_number);
                    computer_port[i].command = CMD_NULL;
                    computer_port[i].port_status = PORT_POWEROFF;
                    computer_port[i].exec_result = EXEC_UNREADY;
                    computer_port[i].relay_PULLIN_tim = 0;
                }
                break;
            default:
                break;
        }
    }
}

void Projector_PowerChange(void)
{}