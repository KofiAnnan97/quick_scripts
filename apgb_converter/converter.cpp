#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <filesystem>
#ifdef _WIN32
    #include <direct.h> // For Windows
#else 
    #include <unistd.h> // For UNIX-like systems
#endif

using namespace std;
using namespace std::filesystem;
using namespace std::literals::string_literals;

int separator = 0x81;
int footer = 0x41504742;
int lcdOff = 0xFFFFFF;

struct APGBPalette{
    int *bg, *obj0, *obj1, *window; 
    int bitSize = 56;
};

path getWorkingDir(){
    //char cwd[1024];
    //#ifdef _WIN32
    //    return _getcwd(cwd, sizeof(cwd));
    //#else
    //    return getcwd(cwd, sizeof(cwd));
    //#endif
    return current_path(); 
}

int* hexAsArray(int hexVal, int bits){
    cout << std::hex << hexVal << endl;
    int size  = bits/2;
    int *arr = new int[size];
    for(int i = 0; i < size; ++i) arr[i] = (hexVal >> (size-i-1)*8) & 0xFF;
    return arr;
}

/*int* intAsHexArray(int hexInt, int bits){

}*/

char* APGBFormat(APGBPalette p, int size){
    char *buffer = new char[size];
    for(int j = 0; j < size; j++) buffer[j] = 0x00;

    int idx = 0;
    for(int a = 0; a < 4; a++){
        int *temp = hexAsArray(p.bg[a], 6);
        for(int b = 0; b < 3; b++){
            buffer[idx] = temp[b];
            cout << std::hex << buffer[idx] << endl;
            idx++;
        }
    }
    for(int c = 0; c < 4; c++){
        int *temp = hexAsArray(p.obj0[c], 6);
        for(int d = 0; d < 3; d++){
            buffer[idx] = temp[d];
            idx++;
        }
    }
    for(int e = 0; e < 4; e++){
        int *temp = hexAsArray(p.obj1[e], 6);
        for(int f = 0; f < 3; f++){
            buffer[idx] = temp[f];
            idx++;
        }
    }
    for(int g = 0; g < 4; g++){
        int *temp = hexAsArray(p.window[g], 6);
        for(int h = 0; h < 3; h++){
            buffer[idx] = temp[h];
            idx++;
        }
    }
    int *temp = hexAsArray(lcdOff, 6);
    for (int i = 0; i < 3; i++){
        buffer[idx] = temp[i];
        idx++;
    }
    buffer[idx] = separator;
    idx++;
    temp = hexAsArray(footer, 8);
    for(int j = 0; j < 4; j++){
        buffer[idx] = temp[j];
        idx++;
    }
    return buffer;
}

APGBPalette getPaletteFromCSV(string filename){
    APGBPalette ap;
    ap.bg = new int[4];
    ap.obj0 = new int[4];
    ap.obj1 = new int[4];
    ap.window = new int[4];
    path cwd = getWorkingDir();
    std::string fnWithExtension = filename + ".csv";
    path filepath = cwd / fnWithExtension;
    fstream csv;
    csv.open(filepath, ios::in);
    if(csv.is_open()){
        vector<string> row;
        string line, word, temp;
        while(csv >> temp){
            row.clear();
            getline(csv, line);
            stringstream s(line);
            while (getline(s, word, ',')) row.push_back(word);
            for(int i = 0; i < 4; i++){
                cout << std::hex << std::stoul(row[i], nullptr, 16) << " ";
            }
            cout << "\n";
            if(row[0] == "BG" || row[0] == "bg"){
                ap.bg[0] = std::stoul(row[1], nullptr, 16);
                ap.bg[1] = std::stoul(row[2], nullptr, 16);
                ap.bg[2] = std::stoul(row[3], nullptr, 16);
                ap.bg[3] = std::stoul(row[4], nullptr, 16);
            }
            else if(row[0] == "OBJ0" || row[0] == "obj0"){
                ap.obj0[0] = std::stoul(row[1], nullptr, 16);
                ap.obj0[1] = std::stoul(row[2], nullptr, 16);
                ap.obj0[2] = std::stoul(row[3], nullptr, 16);
                ap.obj0[3] = std::stoul(row[4], nullptr, 16);
            }
            else if(row[0] == "OBJ1" || row[0] == "obj1"){
                ap.obj1[0] = std::stoul(row[1], nullptr, 16);
                ap.obj1[1] = std::stoul(row[2], nullptr, 16);
                ap.obj1[2] = std::stoul(row[3], nullptr, 16);
                ap.obj1[3] = std::stoul(row[4], nullptr, 16);
            }
            else if(row[0] == "Window" || row[0] == "window"){
                ap.window[0] = std::stoul(row[1], nullptr, 16);
                ap.window[1] = std::stoul(row[2], nullptr, 16);
                ap.window[2] = std::stoul(row[3], nullptr, 16);
                ap.window[3] = std::stoul(row[4], nullptr, 16);
            }
        }
    }
    else cout << "Failed to create file: " << filepath << endl;
    return ap;
}

void savePalette(std::string filename, APGBPalette p){
    path cwd = getWorkingDir();
    std::string fnWithExtension = filename + ".pal";
    path filepath = cwd / fnWithExtension;
    std::ofstream df(filepath, std::ios_base::binary | std::ios_base::out);
    if(df.is_open()){    
        char *buffer = APGBFormat(p, p.bitSize);
        df.write(buffer, p.bitSize);
        df.close();
    }
    else cout << "Failed to create file: " << filepath << endl;
}

int main(){
    std::string filename = "test2";
    APGBPalette pal; //= getPaletteFromCSV(filename);
    pal.bg = new int[4]{0x00000, 0x52518C, 0x8C8EDE, lcdOff};
    pal.obj0 = new int[4]{0x00000, 0x943839, 0xFF8684, lcdOff};
    pal.obj1 = new int[4]{0x00000, 0x943839, 0xFF8684, lcdOff};
    pal.window = new int[4]{0x00000, 0x943839, 0xFF8684, lcdOff};
    savePalette(filename, pal);
}