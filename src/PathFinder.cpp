#include "PathFinder.hpp"
#include <stdio.h>

PathFinder::PathFinder() {

    uint32_t aux_LUT_index = 0;

    // Initial Node P1

    // P1 -> P2
    LUT["12"] = 0;
    aux_LUT_index = 0;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P3
    LUT["123"] = 1;
    aux_LUT_index = 1;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P4
    LUT["1234"] = 2;
    aux_LUT_index = 2;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P5
    LUT["12345"] = 3;
    aux_LUT_index = 3;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P1 -> P8 //DIST
    LUT["12348"] = 4;
    aux_LUT_index = 4;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P1 -> P9     //DIST
    LUT["123489"] = 5;
    aux_LUT_index = 5;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P1 -> P7
    LUT["1237"] = 6;
    aux_LUT_index = 6;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P9 //DIST
    LUT["12379"] = 7;
    aux_LUT_index = 7;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].transformation[7] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P6 //DIST
    LUT["126"] = 8;
    aux_LUT_index = 8;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[2] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P9 //DIST
    LUT["1269"] = 9;
    aux_LUT_index = 9;
    Info[aux_LUT_index].transformation[0] = 1;
    Info[aux_LUT_index].transformation[2] = 1;
    Info[aux_LUT_index].transformation[3] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 3;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P3
    LUT["13"] = 10;
    aux_LUT_index = 10;
    Info[aux_LUT_index].transformation[4] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P4
    LUT["134"] = 11;
    aux_LUT_index = 11;
    Info[aux_LUT_index].transformation[4] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P5
    LUT["1345"] = 12;
    aux_LUT_index = 12;
    Info[aux_LUT_index].transformation[4] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P8
    LUT["1348"] = 13;
    aux_LUT_index = 13;
    Info[aux_LUT_index].transformation[4] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P9 //DIST
    LUT["13489"] = 14;
    aux_LUT_index = 14;
    Info[aux_LUT_index].transformation[4] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P1 -> P7 //DIST
    LUT["137"] = 15;
    aux_LUT_index = 15;
    Info[aux_LUT_index].transformation[4] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P9 //DIST
    LUT["1379"] = 16;
    aux_LUT_index = 16;
    Info[aux_LUT_index].transformation[4] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].transformation[7] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 3;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P4
    LUT["14"] = 17;
    aux_LUT_index = 17;
    Info[aux_LUT_index].transformation[8] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P5
    LUT["145"] = 18;
    aux_LUT_index = 18;
    Info[aux_LUT_index].transformation[8] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P8 //DIST
    LUT["148"] = 19;
    aux_LUT_index = 19;
    Info[aux_LUT_index].transformation[8] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P1 -> P8 //DIST
    LUT["1489"] = 20;
    aux_LUT_index = 20;
    Info[aux_LUT_index].transformation[8] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 3;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;


    // Initial Node P2

    // P2 -> P3
    LUT["23"] = 21;
    aux_LUT_index = 21;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P2 -> P4
    LUT["234"] = 22;
    aux_LUT_index = 22;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P2 -> P5
    LUT["2345"] = 23;
    aux_LUT_index = 23;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P2 -> P8
    LUT["2348"] = 24;
    aux_LUT_index = 24;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P2 -> P9     //DIST
    LUT["23489"] = 25;
    aux_LUT_index = 25;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 3;
    Info[aux_LUT_index].path[9] = 1;
    Info[aux_LUT_index].path[10] = 2;
    Info[aux_LUT_index].path[11] = 2;

    // P2 -> P7
    LUT["237"] = 26;
    aux_LUT_index = 26;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P2 -> P9 //DIST
    LUT["2379"] = 27;
    aux_LUT_index = 27;
    Info[aux_LUT_index].transformation[1] = 1;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].transformation[7] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 3;
    Info[aux_LUT_index].path[6] = 1;
    Info[aux_LUT_index].path[7] = 2;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P2 -> P6 
    LUT["26"] = 28;
    aux_LUT_index = 28;
    Info[aux_LUT_index].transformation[2] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P2 -> P9 //DIST
    LUT["269"] = 29;
    aux_LUT_index = 29;
    Info[aux_LUT_index].transformation[2] = 1;
    Info[aux_LUT_index].transformation[3] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;


    // Initial Node P3

    // P3 -> P4
    LUT["34"] = 30;
    aux_LUT_index = 30;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P3 -> P5
    LUT["345"] = 31;
    aux_LUT_index = 31;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P3 -> P8
    LUT["348"] = 32;
    aux_LUT_index = 32;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P3 -> P9 //DIST
    LUT["3489"] = 33;
    aux_LUT_index = 33;
    Info[aux_LUT_index].transformation[5] = 1;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 3;
    Info[aux_LUT_index].path[7] = 1;
    Info[aux_LUT_index].path[8] = 2;
    Info[aux_LUT_index].path[9] = 2;

    // P3 -> P7 
    LUT["37"] = 34;
    aux_LUT_index = 34;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P3 -> P9 //DIST
    LUT["379"] = 35;
    aux_LUT_index = 35;
    Info[aux_LUT_index].transformation[6] = 1;
    Info[aux_LUT_index].transformation[7] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // Initial Node P4

    // P4 -> P5
    LUT["45"] = 36;
    aux_LUT_index = 36;
    Info[aux_LUT_index].transformation[9] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P4 -> P8
    LUT["48"] = 37;
    aux_LUT_index = 37;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // P4 -> P9 //DIST
    LUT["489"] = 38;
    aux_LUT_index = 38;
    Info[aux_LUT_index].transformation[10] = 1;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 2;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // Initial Node P6

    // P6 -> P9 
    LUT["69"] = 39;
    aux_LUT_index = 39;
    Info[aux_LUT_index].transformation[3] = 1;
    Info[aux_LUT_index].machine_transformations[0] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 3;
    Info[aux_LUT_index].path[3] = 1;
    Info[aux_LUT_index].path[4] = 2;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // Initial Node P7

    // P7 -> P9 
    LUT["79"] = 40;
    aux_LUT_index = 40;
    Info[aux_LUT_index].transformation[7] = 1;
    Info[aux_LUT_index].machine_transformations[1] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 3;
    Info[aux_LUT_index].path[4] = 1;
    Info[aux_LUT_index].path[5] = 2;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;

    // Initial Node P8

    // P8 -> P9 
    LUT["89"] = 41;
    aux_LUT_index = 41;
    Info[aux_LUT_index].transformation[11] = 1;
    Info[aux_LUT_index].machine_transformations[2] = 1;
    Info[aux_LUT_index].path[0] = 2;
    Info[aux_LUT_index].path[1] = 2;
    Info[aux_LUT_index].path[2] = 2;
    Info[aux_LUT_index].path[3] = 2;
    Info[aux_LUT_index].path[4] = 3;
    Info[aux_LUT_index].path[5] = 1;
    Info[aux_LUT_index].path[6] = 2;
    Info[aux_LUT_index].path[7] = 2;
    
}


Path *PathFinder::FindPath(Order::BaseOrder &order) {
    Path *path = new Path;

    Order::Piece *piece = order.GetLastPiece();

    uint8_t initial_piece = order.GetInitialPiece();
    uint8_t final_piece = order.GetFinalPiece();
    uint8_t order_type = order.GetType();
    uint8_t destination = final_piece; // isto é redundante. É só para ficar mais claro

    Graph T(string("TransformationsGraph.txt"));
    T.getInfo();
    //////////////////////////////////////////////////// TRANSFORMATION ORDERS //////////////////////////////////////////////
    if (order_type == Order::ORDER_TYPE_TRANSFORMATION) {
        vector<string> shortestPath = T.Dijktras(to_string(initial_piece), to_string(final_piece));
        string _shortestPath;
        for (auto const &s : shortestPath) {
            _shortestPath += s;
        }
        //cout << "Shortest Path: " << _shortestPath << endl;

        //if (&shortestPath == NULL) {meslog(ERROR) << "No path found for transformation order provided." << std::endl;}

        
        mapT::iterator it = LUT.find(_shortestPath);
        uint8_t index_info = 0;
        if (it != LUT.end())
            index_info = it->second;

        bool C1free,C2free,C3free;  //read from OPCUA
        C1free = 1;
        uint8_t path_index = 0;
        uint8_t machine_step = 0;
        // Saida armazem
        if(C1free){
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 0;
            path->moves[3] = 0;
            path->moves[4] = 0;
            path->moves[5] = 0;
            path_index = 2;
            machine_step = 0;
        }

        else if(C2free && !C1free){
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 0;
            path->moves[5] = 0;
            path_index = 4;
            machine_step = 3;
        }

        else if(C3free && !C1free && !C2free){
            path->moves[0] = 1;
            path->moves[1] = 1;
            path->moves[2] = 1;
            path->moves[3] = 1;
            path->moves[4] = 1;
            path->moves[5] = 1;
            path_index = 6;
            machine_step = 6;
        }

        //Info transformations and machines
        
        for (int i = machine_step; i < 9; i++) {
            path->machine_transformations[i] = Info[index_info].machine_transformations[i];
        }

        for (int i = 0; i < 12; i++) {
            path->transformations[i] = Info[index_info].transformation[i];
        }


        //mudar SetPath para usar pointers
        for (int i = 0; i < 59; i++,path_index++) {
            if (Info[index_info].path[i]) {
                path->moves[path_index] = Info[index_info].path[i];
            }
            else {
                break;
            }
        }

        // Entrada armazem
        if(C1free){
            path->moves[path_index+0] = 3;
            path->moves[path_index+1] = 3;
            path->moves[path_index+2] = 0;
            path->moves[path_index+3] = 0;
            path->moves[path_index+4] = 0;
            path->moves[path_index+5] = 0;
        }

        else if(C2free && !C1free){
            path->moves[path_index+0] = 3;
            path->moves[path_index+1] = 3;
            path->moves[path_index+2] = 3;
            path->moves[path_index+3] = 3;
            path->moves[path_index+4] = 0;
            path->moves[path_index+5] = 0;
        }

        else if(C3free && !C1free && !C2free){
            path->moves[path_index+0] = 3;
            path->moves[path_index+1] = 3;
            path->moves[path_index+2] = 3;
            path->moves[path_index+3] = 3;
            path->moves[path_index+4] = 3;
            path->moves[path_index+5] = 3;
        }

       
       // Update of free/blocked cells

    }
    ////////////////////////////////////////////////////// UNLOAD ORDERS ///////////////////////////////////////////////////
    else if (order_type == Order::ORDER_TYPE_UNLOAD) {

        path->moves[0] = 1;
        path->moves[1] = 1;
        path->moves[2] = 1;
        path->moves[3] = 1;
        path->moves[4] = 1;
        path->moves[5] = 1;
        path->moves[6] = 1;
        path->moves[7] = 2;
        path->moves[8] = 2;

        switch (destination) {
            case 1:
                //unload Pusher1
                path->moves[9] = 1;
                path->moves[10] = 0;
                break;

            case 2:
                //unload Pusher2
                path->moves[9] = 2;
                path->moves[10] = 1;
                path->moves[11] = 0;
                break;

            case 3:
                //unload Pusher3
                path->moves[9] = 2;
                path->moves[10] = 2;
                path->moves[11] = 1;
                path->moves[12] = 0;
                break;

            default:
                meslog(ERROR) << "Invalid destination for Unload-type order." << std::endl;
                break;
        }
    }

    return path;
}
