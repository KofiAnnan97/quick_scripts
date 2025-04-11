#ifndef PALETTES_H
#define PALETTES_H

#include <string>

using namespace std;

struct APGB_Palette{
    string *bg = nullptr, *obj0 = nullptr, *obj1 = nullptr, *window = nullptr;
    int numOfBytes = 56;
};

#endif // PALETTES_H
