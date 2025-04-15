#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <opencv2/opencv.hpp>

#include "format.h"
#include "convert.h"

using namespace cv;

LCI lciFormat;

LCI getData(string lciFile){
    LCI lf;
    fstream input;
    input.open(lciFile, ios::in);
    if(input.is_open()){
        string line;
        getline(input, line);
        lf.id = line;
        getline(input, line);
        lf.width = stoi(line);
        getline(input, line);
        lf.height = stoi(line);
        getline(input, line);
        lf.numOfColors = stoi(line);
        lf.hexColors = new string[lf.numOfColors];
        for(int i = 0; i < lf.numOfColors; i++){
            getline(input, line);
            if(line[0] == '#') line.erase(0, 1);
            lf.hexColors[i] = line;
        }
        lf.imageData = new string[lf.height];
        int hIdx = 0;
        while(getline(input, line)){
            if(hIdx >= lf.height) break;
            else if(line.size() < lf.width){
                int diff = lf.width - line.size();
                for (int j = 0; j < diff; j++) line += ' ';
                lf.imageData[hIdx] = line; 
            }
            else lf.imageData[hIdx] = line.substr(0, lf.width);
            hIdx++;
        }
    }
    return lf;
}

string decodeColor(LCI lf, char cc){
    int idx = -1;
    if(cc >= '0' && cc <= '9') idx = (cc - '0');
    else if(cc >= 'A' && cc <= 'Z') idx = 10 + (cc - 'A');
    else if(cc >= 'a' && cc <= 'z') idx = 10 + (cc - 'a');
    string hexStr = "";
    if(idx == -1) return hexStr;
    else hexStr = lf.hexColors[idx];
    return hexStr;
}

void printLCIData(LCI lf){
    cout << "Header:\n\tType: " << lf.id << endl;
    cout << "\tDimension: (" << lf.width << ", " << lf.height << ")\n"; 
    cout << "\tColors[" << lf.numOfColors << "] => [ ";
    for(int i = 0; i < lf.numOfColors; i++) cout << lf.hexColors[i] << " ";
    cout << "]\nData:\n";
    for(int j =0; j < lf.height; j++) cout << "\t" << lf.imageData[j] << endl;
}


void write(string outputFile, string imgType, LCI lf){
    Mat img(lf.height, lf.width, CV_8UC3, Scalar(0, 0, 0)); 
    for(int row = 0; row < img.rows; row++){
        string colorRow = lf.imageData[row];
        for(int col = 0; col < img.cols; col++){
            string hexStr = decodeColor(lciFormat, colorRow[col]);
            if(!hexStr.empty()){
                int *color = Convert::HEX2RGB(hexStr);
                img.at<Vec3b>(Point(col, row)) = Vec3b(color[2], color[1], color[0]);
            }
        }
    }
    String extension = ".jpg";
    if(imgType == "JPG") {}
    else if(imgType == "PNG") extension = ".png";
    imwrite(outputFile+extension, img);
}


int getExtensionIdx(string filename){
    int idx = -1;
    if (filename.size() == 0) return idx;
    for(idx = filename.size(); idx >= 0; idx--){
       if(filename[idx] == '.') return idx;
    }
    return -1;
}

int main(int argc, char* argv[]){
    string imgFile, imgType = "JPG";
    for (int i = 0; i < argc;i++) {
        if(strcmp(argv[i], "-i") == 0){
            if(i+1 >= argc) cout << "Missing image file (jpg)" << endl;
            else imgFile = argv[i+1];
            i++;
        }
        else if(strcmp(argv[i], "--jpg") == 0) imgType = "JPG";
        else if(strcmp(argv[i], "--png") == 0) imgType = "PNG";
    }

    if(!imgFile.empty()){
        int extIdx = getExtensionIdx(imgFile);
        string outputFile = imgFile.substr(0, extIdx);
        lciFormat = getData(imgFile);
        //printLCIData(lciFormat);
        write(outputFile, imgType, lciFormat);
    }
}
