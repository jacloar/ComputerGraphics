// Include our custom library
#include "../include/PPM.h"
#include "ppm.cpp"
#include <iostream>

int main(){

    PPM myPPM("../textures/test1.ppm");
    myPPM.darken();
    myPPM.savePPM("../textures/test1_darken.ppm");

    return 0;
}
