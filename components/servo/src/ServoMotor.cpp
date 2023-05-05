/**
 * \file ServoMotor.cpp
 * \author T_IOT_901_STG_1_MSC_2023 (m.alognikou, d.kalumvuati, r.nguemo, r.mers and a.azzoug)
 * \brief The main purpose of this component is to drive servo motors motor on M5stack series
 * \date Dec. 2022
 * \copyright This code is under the MIT license but administrated by EPITECH Organization Github Workspace
 */

#include "ServoMotor.h"

GoPlus2 goPlus;

void ServoMotor::servo_motor_configuration()
{
    goPlus.begin();
}

void ServoMotor::servo_motor_move_arm(uint16_t angle)
{
    goPlus.Servo_write_plusewidth(SERVO_NUM0_PW, angle);
}

ServoMotor::ServoMotor() = default;