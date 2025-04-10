#ifndef IMAGEIMPORTER_H
#define IMAGEIMPORTER_H

#include <QGraphicsView>
#include <QDebug>
#include <QString>
#include <vector>

using namespace std;

class ImageImporter
{

public:
    ImageImporter();
    vector<QColor> setImageColors(vector<QString> hexColorIdx);
    int getIdxFromHexChar(char hexChar);
    void decodeImageTxt(string filename, QImage image, vector<QString> palettes);

    int width, height, multiplier;
};

#endif // IMAGEIMPORTER_H
