#include <QImage>
#include <QColor>
#include <QRgb>
#include <QDebug>
#include <fstream>
#include <sstream>

#include "imageimporter.h"

ImageImporter::ImageImporter() {
    multiplier = 2;
    width  = multiplier * 160;  // GameBoy screen width:  160
    height = multiplier * 144;  // GameBoy screen height: 144
}

vector<QColor> ImageImporter::setImageColors(vector<QString> hexColorIdx){
    vector<QColor> temp;
    for(int i = 0; i < hexColorIdx.size(); i++){
        QString hexColor = hexColorIdx[i];
        if(QColor(hexColor).isValid()) temp.push_back(QColor(hexColor));
        else temp.push_back(Qt::black);
    }
    return temp;
}

int ImageImporter::getIdxFromHexChar(char hexChar){
    if(hexChar >= '0' && hexChar <= '9') return hexChar - '0';
    else if(hexChar >= 'A' && hexChar <= 'F') return 10 + (hexChar - 'A');
    else if(hexChar >= 'a' && hexChar <= 'f') return 10 + (hexChar - 'a');
    else return -1;
}

void ImageImporter::decodeImageTxt(string filename, QImage *image, vector<QString> palettes){
    fstream decode;
    string line;
    int row = 0;
    int col = 0;
    decode.open(filename, ios::in);
    if(decode.is_open()){
        vector<QColor> colorIdxs = this->setImageColors(palettes);
        while(getline(decode, line)){
            if(image->size().isEmpty()) break;
            for(auto c: line){
                int hexIdx = this->getIdxFromHexChar(c);
                //qDebug() << c << "=>" << hexIdx;
                if(hexIdx != -1){
                    image->setPixelColor(col, row, colorIdxs[hexIdx]);
                    image->setPixelColor(col+1, row, colorIdxs[hexIdx]);
                    image->setPixelColor(col, row+1, colorIdxs[hexIdx]);
                    image->setPixelColor(col+1, row+1, colorIdxs[hexIdx]);
                }
                col+=this->multiplier;
            }
            row+=this->multiplier;
            col = 0;
        }
    }
    else qDebug() << "Could not find file: " + filename;
}
