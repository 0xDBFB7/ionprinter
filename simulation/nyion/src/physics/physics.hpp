#ifndef PHYSICS_H
#define PHYSICS_H

#include <math.h>

float norm3(float x, float y, float z);

const int MAX_ELECTRODE_NUM = 10;

struct electrodes{
    float voltage[MAX_ELECTRODE_NUM];
    float capacitance[MAX_ELECTRODE_NUM];//?
}
