#include "control.h"
#include "main.h"

extern Computer_Port_t computer_port[PCPORTCOUNT];

void Relay_Pullin(uint8_t relay_number)
{
    switch (relay_number) {
        case RELAY1:
            HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_RESET);
            break;
        case RELAY2:
            HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_RESET);
            break;
        case RELAY3:
            HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, GPIO_PIN_RESET);
            break;
        case RELAY4:
            HAL_GPIO_WritePin(Relay4_GPIO_Port, Relay4_Pin, GPIO_PIN_RESET);
            break;
        case RELAY5:
            HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, GPIO_PIN_RESET);
            break;
        case RELAY6:
            HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_RESET);
            break;
        case RELAY7:
            HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_RESET);
            break;
        case RELAY8:
            HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_RESET);
            break;
        default:
            HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay4_GPIO_Port, Relay4_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_RESET);
            break;
    }
}

void Relay_Release(uint8_t relay_number)
{
    switch (relay_number) {
        case RELAY1:
            HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
            break;
        case RELAY2:
            HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
            break;
        case RELAY3:
            HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, GPIO_PIN_SET);
            break;
        case RELAY4:
            HAL_GPIO_WritePin(Relay4_GPIO_Port, Relay4_Pin, GPIO_PIN_SET);
            break;
        case RELAY5:
            HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, GPIO_PIN_SET);
            break;
        case RELAY6:
            HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_SET);
            break;
        case RELAY7:
            HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_SET);
            break;
        case RELAY8:
            HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_SET);
            break;
        default:
            HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay4_GPIO_Port, Relay4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_SET);
            break;
    }
}

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

void Computer_Execute(void)
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

void Projector_Execute(void)
{}