#ifndef SAVE_H
#define SAVE_H

#include <string>
#include <QDebug>

#include "palettes.h"

using namespace std;

class FileSave
{
public:
    FileSave();

    // Helper functions
    APGB_Palette getPalette(string *bg, string *obj0, string *obj1, string *window);
    int* fromStrToHexStrArr(string str, int numOfBytes);
    void clearBuffer(char *buffer, int size);

    // Save functions
    char* APGBFormat(APGB_Palette p, int size);
    void savePalette(string filename, APGB_Palette p);

    // Variables
    string footer, lcdOff;
};

#endif // SAVE_H
