#include <sstream>
#include <fstream>

#include "save.h"

FileSave::FileSave() {
    footer = "8141504742";
    lcdOff = "FFFFFF";
}

int* FileSave::fromStrToHexStrArr(string str, int numOfBytes){
    int *hexArr = new int[numOfBytes];
    //if(strcmp(str.substr(0, 2).c_str(), "0x") == 0) str.erase(str.begin(), str.begin()+2);
    for(int i = 0; i < numOfBytes;++i){
        string segment = str.substr(i*2, 2);
        stringstream ss;
        ss << std::hex << segment;
        ss >> hexArr[i];
    }
    return hexArr;
}

void FileSave::clearBuffer(char *buffer, int size){
    for(int j = 0; j < size; j++) buffer[j] = 0x00;
}

char* FileSave::APGBFormat(APGB_Palette p, int size){
    string data = "";
    for(int a = 0; a < 4; a++){
        if(strcmp(p.bg[a].substr(0, 2).c_str(), "0x") == 0){
            p.bg[a].erase(p.bg[a].begin(), p.bg[a].begin()+2);
        }
        else if(strcmp(p.bg[a].substr(0,1).c_str(), "#") == 0){
            p.bg[a].erase(p.bg[a].begin(), p.bg[a].begin()+1);
        }
        data += p.bg[a];
    }
    for(int c = 0; c < 4; c++){
        if(strcmp(p.obj0[c].substr(0, 2).c_str(), "0x") == 0){
            p.obj0[c].erase(p.obj0[c].begin(), p.obj0[c].begin()+2);
        }
        else if(strcmp(p.obj0[c].substr(0,1).c_str(), "#") == 0){
            p.obj0[c].erase(p.obj0[c].begin(), p.obj0[c].begin()+1);
        }
        data += p.obj0[c];
    }
    for(int e = 0; e < 4; e++){
        if(strcmp(p.obj1[e].substr(0, 2).c_str(), "0x") == 0){
            p.obj1[e].erase(p.obj1[e].begin(), p.obj1[e].begin()+2);
        }
        else if(strcmp(p.obj1[e].substr(0,1).c_str(), "#") == 0){
            p.obj1[e].erase(p.obj1[e].begin(), p.obj1[e].begin()+1);
        }
        data += p.obj1[e];
    }
    for(int g = 0; g < 4; g++) {
        if(strcmp(p.window[g].substr(0, 2).c_str(), "0x") == 0){
            p.window[g].erase(p.window[g].begin(), p.window[g].begin()+2);
        }
        else if(strcmp(p.window[g].substr(0,1).c_str(), "#") == 0){
            p.window[g].erase(p.window[g].begin(), p.window[g].begin()+1);
        }
        data += p.window[g];
    }
    data += this->lcdOff + this->footer;
    qDebug() << "Buffer:" << data;

    int *temp = fromStrToHexStrArr(data, size);
    char *buffer = new char[size];
    clearBuffer(buffer, size);
    for(int i = 0; i < size; i++) buffer[i] = temp[i];
    return buffer;
}

APGB_Palette FileSave::getPalette(string *bg, string *obj0, string *obj1, string *window){
    APGB_Palette pal;
    pal.bg = bg;
    pal.obj0 = obj0;
    pal.obj1 = obj1;
    pal.window = window;
    return pal;
}

void FileSave::savePalette(string filename, APGB_Palette p){
    std::ofstream df(filename, std::ios_base::binary | std::ios_base::out);
    if(df.is_open()){
        char *buffer = this->APGBFormat(p, p.numOfBytes);
        df.write(buffer, p.numOfBytes);
        df.close();
    }
    else qDebug() << "Failed to create file: " << filename;
}
