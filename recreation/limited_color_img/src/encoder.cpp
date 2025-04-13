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
vector<string> palettes;
vector<string> hexColors;

vector<string> retrievePalettes(string filename){
    vector<string> data;
    fstream pal;
    string line;
    pal.open(filename, ios::in);
    if(pal.is_open()){
        while(getline(pal, line)){
            if(line[0] == '#') line.erase(0, 1);
            data.push_back(line);
        }
        pal.close();
    }
    else if(filename.size() == 0) {}
    else cout << "Cannot open file: " << filename << endl;
    return data;
}

vector<string> getUniquePalettes(string filename){
    vector<string> temp;
    Mat img; 
    img = imread(filename, 1); 
    if (!img.data)  cout << "No image data \n"; 
    else{
        for(int i=0; i<img.rows; i++){
            for(int j=0; j<img.cols; j++){
                Vec3b bgr = img.at<Vec3b>(i, j);
                int b = bgr[0];
                int g = bgr[1];
                int r = bgr[2];
                string pxHex = Convert::RGB2HEX(r, g, b);
                bool colorPresent = false;
                for(auto colorHex: temp){
                    if(pxHex == colorHex) {
                        colorPresent = true;
                        break;
                    }
                }
                if(colorPresent == false and temp.size() < lciFormat.maxColors) temp.push_back(pxHex);
            }
        }
    }
    return temp;
}

vector<string> convertJPGToLCI(string filename, vector<string> palettes){
    vector<string> temp;
    Mat img; 
    img = imread(filename, 1); 
    if (!img.data)  cout << "No image data \n"; 
    else{
        lciFormat.width = img.cols;
        lciFormat.height = img.rows;
        for(int i=0; i<img.rows; i++){
            string line = "";
            for(int j=0; j<img.cols; j++){
                Vec3b bgr = img.at<Vec3b>(i, j);
                int b = bgr[0];
                int g = bgr[1];
                int r = bgr[2];
                string pxHex = Convert::RGB2HEX(r, g, b);
                for(int k = 0; k < palettes.size(); k++){
                    if(pxHex == palettes[k]){
                        line += lciFormat.encodeIdx[k];
                        break;
                    } 
                }
            } 
            temp.push_back(line);
        }
    }    
    return temp;
}
int getExtensionIdx(string filename){
    int idx = -1;
    if (filename.size() == 0) return idx;
    for(idx = filename.size(); idx >= 0; idx--){
       if(filename[idx] == '.') return idx;
    }
    return -1;
}

void write(string filename, vector<string> lciData, vector<string> palettes){
    ofstream encFile(filename);
    if(encFile.is_open()){
        encFile << lciFormat.id << endl;
        encFile << lciFormat.width << endl;
        encFile << lciFormat.height << endl;
        encFile << lciFormat.numOfColors << endl;
        for(int i = 0; i < palettes.size(); i++) encFile << palettes[i] << endl;
        for(int j = 0; j < lciData.size(); j++)  encFile << lciData[j] << endl;
        encFile.close();
    }
}

int main(int argc, char* argv[]){
    string imgFile, paletteFile;
    for (int i = 0; i < argc;) {
        if(strcmp(argv[i], "-i") == 0){
            if(i+1 >= argc) cout << "Missing image file (jpg)" << endl;
            else imgFile = argv[i+1];
            i+=2;
        }
        else if(strcmp(argv[i], "-p") == 0){
            if(i+1 >= argc) cout << "Missing palette file (txt)" << endl;
            else paletteFile = argv[i+1];
            i+=2;
        }
        else i++;
    }

    if(!imgFile.empty()){
        int extIdx = getExtensionIdx(imgFile);
        string encodedFile = imgFile.substr(0,extIdx) + ".lci";
        palettes = retrievePalettes(paletteFile);
        if(palettes.size() == 0) palettes = getUniquePalettes(imgFile);
        vector<string> lciData = convertJPGToLCI(imgFile, palettes);
        lciFormat.numOfColors = palettes.size();
        write(encodedFile, lciData, palettes);
    } 
}