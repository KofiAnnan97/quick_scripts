#include "format.h"
#include "convert.h"

int* Convert::HEX2RGB(string hexStr){
    int *rgb =  new int[3];
    rgb[0] = (int)strtol(hexStr.substr(0, 2).c_str(), NULL, 16);
    rgb[1] = (int)strtol(hexStr.substr(2, 2).c_str(), NULL, 16);
    rgb[2] = (int)strtol(hexStr.substr(4, 2).c_str(), NULL, 16);
    return rgb;
}

string Convert::RGB2HEX(int r, int g, int b){
    string hexVal = "";
    string data[2];
    LCI lf;
    data[1] = lf.encodeIdx[r%16];
    r /= 16;
    data[0] = lf.encodeIdx[r%16];
    hexVal += data[0] + data[1];
    data[1] = lf.encodeIdx[g%16];
    g /= 16;
    data[0] = lf.encodeIdx[g%16];
    hexVal += data[0] + data[1];
    data[1] = lf.encodeIdx[b%16];
    b /= 16;
    data[0] = lf.encodeIdx[b%16];
    hexVal += data[0] + data[1];
    return hexVal;
}