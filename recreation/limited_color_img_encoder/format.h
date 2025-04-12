#include <string>

using namespace std;

struct Header{
    string type;
    int width, height, numOfColors;
    string * hexColors;
};

struct Data{
    string * rows;
};

struct LCIFormat{
    Header header;
    int maxColors = 36;
    char encodeIdx[36] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z'};
    Data image;
};