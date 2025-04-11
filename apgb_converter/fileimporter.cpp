#include <fstream>
#include <sstream>

#include "fileimporter.h"

FileImporter::FileImporter() {
    black = "#000000";
    dark = "#1a1a1a";
    light = "#828483";
    white = "#FFFFFF";
}

bool FileImporter::isKeyword(string word){
    vector<string> keywords = {"BG", "OBJ0", "OBJ1", "WINDOW", "bg", "obj0", "obj1", "window"};
    for(int i = 0; i < keywords.size(); i++){
        if(word == keywords[i]) return true;
    }
    return false;
}

string FileImporter::trim(string data){
    int startIdx = 0;
    int endIdx = data.length()-1;
    while(data[startIdx] == ' ') startIdx++;
    while(data[endIdx] == ' ') endIdx--;
    return data.substr(startIdx, endIdx-startIdx+1);
}

string FileImporter::fromHexToStr(string hexStr){
    char hexIdx[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                       'A', 'B', 'C', 'D', 'E', 'F'};
    int hexVals[2];
    string str;
    for (size_t i = 0; i < hexStr.length(); i += 1) {
        string byte = hexStr.substr(i, 1);
        unsigned temp = (unsigned char)byte.c_str()[0];
        hexVals[1] = temp%16;
        temp /= 16;
        hexVals[0] = temp%16;
        str.push_back(hexIdx[hexVals[0]]);
        str.push_back(hexIdx[hexVals[1]]);
    }
    return str;
}

bool FileImporter::isJASCFormat(string filename){
    fstream pal;
    string jascId;
    bool isFormat = false;
    pal.open(filename, ios::binary | ios::in);
    if(pal.is_open()){
        getline(pal, jascId);
        jascId.erase(jascId.size()-1, 1);
        if(jascId == "JASC-PAL") isFormat = true;
    }
    pal.close();
    return isFormat;
}

bool FileImporter::isAPGBFormat(string filename){
    fstream pal;
    string line;
    bool isFormat = false;
    pal.open(filename, ios::binary | ios::in);
    if(pal.is_open()){
        getline(pal, line);
        if(strcmp(line.substr(52, 4).c_str(),"APGB") == 0) isFormat = true;
    }
    pal.close();
    return isFormat;
}

APGB_Palette FileImporter::importPaletteFromCSV(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    fstream csv;
    csv.open(filename, ios::in);
    if(csv.is_open()){
        vector<string> row;
        string line, word, temp;
        while(csv >> temp){
            row.clear();
            getline(csv, line);
            stringstream s(line);
            while (getline(s, word, ',')){
                string trimmedWord = this->trim(word);
                if(!this->isKeyword(trimmedWord) && trimmedWord.substr(0, 2) == "0x"){
                    trimmedWord.erase(0, 2);
                    trimmedWord = "#" + trimmedWord;
                }
                else if(!this->isKeyword(trimmedWord) && trimmedWord[0] != '#'){
                    trimmedWord = "#" + trimmedWord;
                }
                row.push_back(trimmedWord);
            }
            string *pt;
            auto pType = row[0].c_str();
            if(strcmp(pType , "BG") == 0 || strcmp(pType ,"bg") == 0)              pt = ap.bg;
            else if(strcmp(pType , "OBJ0") == 0 || strcmp(pType ,"obj0") == 0)     pt = ap.obj0;
            else if(strcmp(pType , "OBJ1") == 0 || strcmp(pType ,"obj1") == 0)     pt = ap.obj1;
            else if(strcmp(pType , "WINDOW") == 0 || strcmp(pType ,"window") == 0) pt = ap.window;
            pt[0] = row[1];
            pt[1] = row[2];
            pt[2] = row[3];
            pt[3] = row[4];
        }
    }
    return ap;
}

APGB_Palette FileImporter::importPaletteFromAPGB(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    ifstream pal;
    pal.open(filename, ios::binary | ios::in);
    if(pal.is_open()){
        string line, word, temp;
        while(pal >> temp){
            stringstream ss;
            ss << std::hex << temp;
            ss >> line;
            if(line.size() == 56){
                line.erase(48,8);
                ap.bg[0] = "#"+this->fromHexToStr(line.substr(0, 3));
                ap.bg[1] = "#"+this->fromHexToStr(line.substr(3, 3));
                ap.bg[2] = "#"+this->fromHexToStr(line.substr(6, 3));
                ap.bg[3] = "#"+this->fromHexToStr(line.substr(9, 3));
                ap.obj0[0] = "#"+this->fromHexToStr(line.substr(12,3));
                ap.obj0[1] = "#"+this->fromHexToStr(line.substr(15,3));
                ap.obj0[2] = "#"+this->fromHexToStr(line.substr(18,3));
                ap.obj0[3] = "#"+this->fromHexToStr(line.substr(21,3));
                ap.obj1[0] = "#"+this->fromHexToStr(line.substr(24,3));
                ap.obj1[1] = "#"+this->fromHexToStr(line.substr(27,3));
                ap.obj1[2] = "#"+this->fromHexToStr(line.substr(30,3));
                ap.obj1[3] = "#"+this->fromHexToStr(line.substr(33,3));
                ap.window[0] = "#"+this->fromHexToStr(line.substr(36,3));
                ap.window[1] = "#"+this->fromHexToStr(line.substr(39,3));
                ap.window[2] = "#"+this->fromHexToStr(line.substr(42,3));
                ap.window[3] = "#"+this->fromHexToStr(line.substr(45,3));
                break;
            }
        }
        pal.close();
    }
    return ap;
}

APGB_Palette FileImporter::importPaletteJASC(string filename){
    APGB_Palette ap;
    ap.bg = new string[4];
    ap.obj0 = new string[4];
    ap.obj1 = new string[4];
    ap.window = new string[4];
    vector<QString> colorVals;
    ifstream jasc;
    string jascId, jascVersion,line;
    unsigned long numOfPalettes;
    jasc.open(filename);
    if(jasc.is_open()){
        getline(jasc, jascId);
        getline(jasc, jascVersion);
        getline(jasc, line);
        numOfPalettes = stoi(line);
        if(numOfPalettes != 4 && numOfPalettes != 8 && numOfPalettes != 16){
         //"This script only supports 4, 8 or 16 color palettes for the JASC PAL format.";
        }
        else{
            while(getline(jasc, line)){
                line.erase(line.size()-1, 1);
                stringstream ss(line);
                string val;
                getline(ss, val, ' ');
                int r = stoi(val);
                getline(ss, val, ' ');
                int g = stoi(val);
                getline(ss, val, ' ');
                int b = stoi(val);
                colorVals.push_back(QColor(r, g, b).name());
            }
            if(numOfPalettes != colorVals.size()){
                //qDebug() << "The number of color palettes are configured incorrectly. Please check the file.";
            }
            else if(numOfPalettes == 4){
                ap.bg[0] = colorVals[0].toStdString();
                ap.bg[1] = colorVals[1].toStdString();
                ap.bg[2] = colorVals[2].toStdString();
                ap.bg[3] = colorVals[3].toStdString();
                ap.obj0[0] = colorVals[0].toStdString();
                ap.obj0[1] = colorVals[1].toStdString();
                ap.obj0[2] = colorVals[2].toStdString();
                ap.obj0[3] = colorVals[3].toStdString();
                ap.obj1[0] = colorVals[0].toStdString();
                ap.obj1[1] = colorVals[1].toStdString();
                ap.obj1[2] = colorVals[2].toStdString();
                ap.obj1[3] = colorVals[3].toStdString();
                ap.window[0] = colorVals[0].toStdString();
                ap.window[1] = colorVals[1].toStdString();
                ap.window[2] = colorVals[2].toStdString();
                ap.window[3] = colorVals[3].toStdString();
            }
            else if(numOfPalettes == 8){
                ap.bg[0] = colorVals[0].toStdString();
                ap.bg[1] = colorVals[1].toStdString();
                ap.bg[2] = colorVals[2].toStdString();
                ap.bg[3] = colorVals[3].toStdString();
                ap.obj0[0] = colorVals[4].toStdString();
                ap.obj0[1] = colorVals[5].toStdString();
                ap.obj0[2] = colorVals[6].toStdString();
                ap.obj0[3] = colorVals[7].toStdString();
                ap.obj1[0] = colorVals[4].toStdString();
                ap.obj1[1] = colorVals[5].toStdString();
                ap.obj1[2] = colorVals[6].toStdString();
                ap.obj1[3] = colorVals[7].toStdString();
                ap.window[0] = colorVals[0].toStdString();
                ap.window[1] = colorVals[1].toStdString();
                ap.window[2] = colorVals[2].toStdString();
                ap.window[3] = colorVals[3].toStdString();
            }
            else if(numOfPalettes == 16){
                ap.bg[0] = colorVals[0].toStdString();
                ap.bg[1] = colorVals[1].toStdString();
                ap.bg[2] = colorVals[2].toStdString();
                ap.bg[3] = colorVals[3].toStdString();
                ap.obj0[0] = colorVals[4].toStdString();
                ap.obj0[1] = colorVals[5].toStdString();
                ap.obj0[2] = colorVals[6].toStdString();
                ap.obj0[3] = colorVals[7].toStdString();
                ap.obj1[0] = colorVals[8].toStdString();
                ap.obj1[1] = colorVals[9].toStdString();
                ap.obj1[2] = colorVals[10].toStdString();
                ap.obj1[3] = colorVals[11].toStdString();
                ap.window[0] = colorVals[12].toStdString();
                ap.window[1] = colorVals[13].toStdString();
                ap.window[2] = colorVals[14].toStdString();
                ap.window[3] = colorVals[15].toStdString();
            }
        }
        jasc.close();
    }
    //else qDebug() << "Failed to find file: " << filename;
    return ap;
}
