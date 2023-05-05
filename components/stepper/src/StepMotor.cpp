/**
 * \file StepMotor.cpp
 * \author T_IOT_901_STG_1_MSC_2023 (m.alognikou, d.kalumvuati, r.nguemo, r.mers and a.azzoug)
 * \brief The main purpose of this component is to drive stepper motors motor on M5stack series
 * \date Dec. 2022
 * \copyright This code is under the MIT license but administrated by EPITECH Organization Github Workspace
 */

#include "StepMotor.h"

#define STEPMOTOR_I2C_ADDR 0x70

GRBL _GRBL = GRBL(STEPMOTOR_I2C_ADDR);

void StepMotor::step_motor_configuration() {
    Wire.begin(21, 22);
    _GRBL.Init(&Wire);
    _GRBL.setMode("absolute");
}

String StepMotor::step_motor_status()
{
    return _GRBL.readStatus();
}

void StepMotor::step_motor_run(int axes, int speed) {
    _GRBL.setMotor(axes,axes,axes,speed);
    _GRBL.setMotor(0,0,0,speed);
}

void StepMotor::step_motor_inlock() {
    _GRBL.unLock();
}

void StepMotor::step_motor_unlock() {
    _GRBL.unLock();
}

StepMotor::StepMotor() = default;
