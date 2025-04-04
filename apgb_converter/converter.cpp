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

/*int* hexAsArray(int hexVal, int bytes){
  int size = bytes/2;
  int *arr = new int[size];
  for(int i = 0; i < size; ++i) arr[i] = (hexVal >> (size-i-1)*8) & 0xFF;
  return arr;
}*/

int* fromStrToHexStrArr(string hexStr, int bytes){
  int size = bytes/2;
  int *arr = new int[size];
  if(strcmp(hexStr.substr(0, 2).c_str(), "0x") == 0){
    hexStr.erase(hexStr.begin(), hexStr.begin()+2);
  }
  for(int i = 0; i < size;++i){
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
      //for(int i = 0; i < 5; i++) cout << row[i] << " ";
      //cout << "\n";
      
      /*string **pt = nullptr;
      //cout << strcmp(row[0].c_str(), "BG") << endl;
      //if(strcmp(row[0].c_str(), "BG") == 0 || strcmp(row[0].c_str(),"bg") == 0){
            if(count == 0){
                pt = &ap.bg;
            //}else if(strcmp(row[0].c_str(), "OBJ0") == 0){
            }else if(count == 1){
                pt = &ap.obj0;
            //}else if(strcmp(row[0].c_str(), "OBJ1") == 0){
            }else if(count == 2){
                pt = &ap.obj1;
            //}else if(strcmp(row[0].c_str(), "WINDOW") == 0){
            }else if(count == 3){    
                pt = &ap.window;
            }
            count++;
            if(pt != nullptr){
                pt[0] = &row[1];
                pt[1] = &row[2];
                pt[2] = &row[3];
                pt[3] = &row[4];
                for(int x = 0; x < 4; x++){
                    cout << pt[x] << " ";
                }
                cout << "\n";
            }*/
      if(strcmp(row[0].c_str(), "BG") == 0 || strcmp(row[0].c_str(),"bg") == 0){
        ap.bg[0] = string(row[1]);
        ap.bg[1] = string(row[2]);
        ap.bg[2] = row[3];
        ap.bg[3] = row[4];
      }else if(strcmp(row[0].c_str(), "OBJ0") == 0 || strcmp(row[0].c_str(),"obj0") == 0){
        ap.obj0[0] = row[1];
        ap.obj0[1] = row[2];
        ap.obj0[2] = row[3];
        ap.obj0[3] = row[4];
      }else if(strcmp(row[0].c_str(), "OBJ1") == 0 || strcmp(row[0].c_str(),"obj1") == 0){
        ap.obj1[0] = row[1];
        ap.obj1[1] = row[2];
        ap.obj1[2] = row[3];
        ap.obj1[3] = row[4];
      }else if(strcmp(row[0].c_str(), "WINDOW") == 0 || strcmp(row[0].c_str(),"window") == 0){
        ap.window[0] = row[1];
        ap.window[1] = row[2];
        ap.window[2] = row[3];
        ap.window[3] = row[4];
      }
    }
  }
  else cout << "Failed to create file: " << filepath << endl;
  return ap;
}

char* APGBFormat(APGBPalette p, int size){
  char *buffer = new char[size];
  for(int j = 0; j < size; j++) buffer[j] = 0x00;

  int idx = 0;
  for(int a = 0; a < 4; a++){
    int *temp = fromStrToHexStrArr(p.bg[a], 6);
    for(int b = 0; b < 3; b++){
      buffer[idx] = temp[b];
      idx++;
    }
  }
  for(int c = 0; c < 4; c++){
    int *temp = fromStrToHexStrArr(p.obj0[c], 6);
    for(int d = 0; d < 3; d++){
      buffer[idx] = temp[d];
      idx++;
    }
  }
  for(int e = 0; e < 4; e++){
    int *temp = fromStrToHexStrArr(p.obj1[e], 6);
    for(int f = 0; f < 3; f++){
      buffer[idx] = temp[f];
      idx++;
    }
  }
  for(int g = 0; g < 4; g++){
    int *temp = fromStrToHexStrArr(p.window[g], 6);
    for(int h = 0; h < 3; h++){
      buffer[idx] = temp[h];
      idx++;
    }
  }
  int *temp = fromStrToHexStrArr(lcdOff, 6);
  for (int i = 0; i < 3; i++){
    buffer[idx] = temp[i];
    idx++;
  }
  temp = fromStrToHexStrArr(footer, 10);
  for (int i = 0; i < 5; i++){
    buffer[idx] = temp[i];
    idx++;
  }
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
  string filename = "test2";
  APGBPalette pal = getPaletteFromCSV(filename);
  savePalette(filename, pal);
}