#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <string>
#include <filesystem>
#include <typeinfo>
//#ifdef _WIN32
//    #include <direct.h> // For Windows
//#else 
//    #include <unistd.h> // For UNIX-like systems
//#endif

using namespace std;
using namespace std::filesystem;
using namespace std::literals::string_literals;

string footer = "8141504742";
string lcdOff = "FFFFFF";

struct APGBPalette{
  string *bg, *obj0, *obj1, *window; 
  int numOfBytes = 56;
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

void displayBuffer(char *buffer, int size){
  cout << "Buffer: ";
  for(int i = 0; i < size; i++) cout << buffer[i] << " ";
  cout << "\n";
}

void clearBuffer(char *buffer, int size){
  for(int j = 0; j < size; j++){
    buffer[j] = 0x00;
  } 
}

int* fromStrToHexStrArr(string hexStr, int numOfBytes){
  int *arr = new int[numOfBytes];
  //if(strcmp(hexStr.substr(0, 2).c_str(), "0x") == 0) hexStr.erase(hexStr.begin(), hexStr.begin()+2);
  for(int i = 0; i < numOfBytes;++i){
    string segment = hexStr.substr(i*2, 2);
    stringstream ss;
    ss << std::hex << segment;
    ss >> arr[i]; 
  }
  return arr;
}

string trim(string data){
  int startIdx = 0;
  int endIdx = data.length()-1;
  while(data[startIdx] == ' ') startIdx++;
  while(data[endIdx] == ' ') endIdx--;
  return data.substr(startIdx, endIdx-startIdx+1);
}

APGBPalette getPaletteFromCSV(string filename){
  APGBPalette ap;
  ap.bg = new string[4];
  ap.obj0 = new string[4];
  ap.obj1 = new string[4];
  ap.window = new string[4];
  path cwd = getWorkingDir();
  string fnWithExtension = filename + ".csv";
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
      while (getline(s, word, ',')){
        string trimmedWord = trim(word);
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
  else cout << "Failed to create file: " << filepath << endl;
  return ap;
}

char* APGBFormat(APGBPalette p, int size){
  string data = "";
  for(int a = 0; a < 4; a++){
    if(strcmp(p.bg[a].substr(0, 2).c_str(), "0x") == 0){
      p.bg[a].erase(p.bg[a].begin(), p.bg[a].begin()+2);
    }
    data += p.bg[a];
  } 
  for(int c = 0; c < 4; c++){
    if(strcmp(p.obj0[c].substr(0, 2).c_str(), "0x") == 0){
      p.obj0[c].erase(p.obj0[c].begin(), p.obj0[c].begin()+2);
    }
    data += p.obj0[c];
  } 
  for(int e = 0; e < 4; e++){
    if(strcmp(p.obj1[e].substr(0, 2).c_str(), "0x") == 0){
      p.obj1[e].erase(p.obj1[e].begin(), p.obj1[e].begin()+2);
    }
    data += p.obj1[e];
  }
  for(int g = 0; g < 4; g++) {
    if(strcmp(p.window[g].substr(0, 2).c_str(), "0x") == 0){
      p.window[g].erase(p.window[g].begin(), p.window[g].begin()+2);
    }
    data += p.window[g];
  }
  data += lcdOff + footer;
  
  int *temp = fromStrToHexStrArr(data, size);
  char *buffer = new char[size];
  clearBuffer(buffer, size);
  for(int i = 0; i < size; i++) buffer[i] = temp[i];
  //displayBuffer(buffer, p.numOfBytes);
  return buffer;
}

void savePalette(string filename, APGBPalette p){
  path cwd = getWorkingDir();
  std::string fnWithExtension = filename + ".pal";
  path filepath = cwd / fnWithExtension;
  std::ofstream df(filepath, std::ios_base::binary | std::ios_base::out);
  if(df.is_open()){    
    char *buffer = APGBFormat(p, p.numOfBytes);
    df.write(buffer, p.numOfBytes);
    df.close();
  }
  else cout << "Failed to create file: " << filepath << endl;
}

int main(){
  string filename = "test";
  APGBPalette pal = getPaletteFromCSV(filename);
  savePalette(filename, pal);
}