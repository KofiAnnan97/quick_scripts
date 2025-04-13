#ifndef CONVERT_H
#define CONVERT_H

#include <string>

using namespace std;

class Convert {
public:
    static int* HEX2RGB(string hexStr);
    static string RGB2HEX(int r, int g, int b);
};

#endif