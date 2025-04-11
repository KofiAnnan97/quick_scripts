#ifndef IMAGEIMPORTER_H
#define IMAGEIMPORTER_H

#include <QGraphicsView>
#include <QDebug>
#include <QString>
#include <vector>
#include <QDir>
#include <QImage>
#include <QColor>
#include <QRgb>
#include <QDebug>
#include <QStringList>

using namespace std;

class ImageImporter
{

public:
    ImageImporter();
    void setImgDirectory(QString filepath);
    void changeFile(int direction, QImage *image, vector<QString> palettes);
    vector<QColor> setImageColors(vector<QString> hexColorIdx);
    int getIdxFromHexChar(char hexChar);
    void decodeImageTxt(string filename, QImage *image, vector<QString> palettes);

    int width, height, multiplier, fileIdx;
    QDir imgDir;
    QColor emptyColor = Qt::black;
};

#endif // IMAGEIMPORTER_H
