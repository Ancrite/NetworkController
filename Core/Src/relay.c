#include "relay.h"
#include "main.h"
#include "gpio.h"

void Relay_Pullin(uint8_t relay_number)
{
    switch (relay_number)
    {
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
        // case RELAY6:
        //     HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_RESET);
        //     break;
        // case RELAY7:
        //     HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_RESET);
        //     break;
        // case RELAY8:
        //     HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_RESET);
        //     break;
        default:
            HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay4_GPIO_Port, Relay4_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, GPIO_PIN_RESET);
            // HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_RESET);
            // HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_RESET);
            // HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_RESET);
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
        // case RELAY6:
        //     HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_SET);
        //     break;
        // case RELAY7:
        //     HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_SET);
        //     break;
        // case RELAY8:
        //     HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_SET);
        //     break;
        default:
            HAL_GPIO_WritePin(Relay1_GPIO_Port, Relay1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay2_GPIO_Port, Relay2_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay3_GPIO_Port, Relay3_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay4_GPIO_Port, Relay4_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(Relay5_GPIO_Port, Relay5_Pin, GPIO_PIN_SET);
            // HAL_GPIO_WritePin(Relay6_GPIO_Port, Relay6_Pin, GPIO_PIN_SET);
            // HAL_GPIO_WritePin(Relay7_GPIO_Port, Relay7_Pin, GPIO_PIN_SET);
            // HAL_GPIO_WritePin(Relay8_GPIO_Port, Relay8_Pin, GPIO_PIN_SET);
            break;
    }
}