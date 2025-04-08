#ifndef FILEIMPORTER_H
#define FILEIMPORTER_H

#include <QString>
#include <QDebug>
#include <QColor>

#include "palettes.h"

using namespace std;

class FileImporter
{
public:
    FileImporter();

    // Helper functions
    bool isKeyword(string word);
    string trim(string data);
    string fromHexToStr(string hexStr);

    // Import functions
    APGB_Palette importPaletteFromCSV(string filename);
    APGB_Palette importPaletteFromAPGB(string filename);
    APGB_Palette importPaletteJASC(string filename);

    // Variables
    QString black, white;
};

#endif // FILEIMPORTER_H
