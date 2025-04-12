# Limited Color Image Encoder
The primary purpose of this script is to allow for dynamic changing of image files as used in [apgb_converter](https://github.com/KofiAnnan97/apgb_converter). It is design to encode an image as a series of characters in which a single character respresents a color. The limited nature comes its intent to be used for old video game consoles with a limited color range that could be displayed such as the original Gameboy. 

## Build
This script relies on the OpenCV C++ libray and uses CMake to compile the code. In the same directory as `CMakeLists.txt` run the following:
```
cmake .
make
```

## Execution
Run this command to convert an image into a .lci file:
```
.\lci-encoder -i \path\to\image.png
``` 
The resulting file with be saved in the current working directory.

---------------------------
If the colors need to be in a specific sequence use the following command:
```
.\lci-encoder -i \path\to\image.png -p \path\to\colorSequence.txt
```
The format of these colors is the same as the example of the `hexColor` values but in a text file.

## LCI Format
The limited color image (LCI) format is a text file with the `.lci` extension that uses a singular hexadecimal value to represent a color.
| Value | Description |
|-------|-------------| 
| id | The ASCII representation of `.LCI` |
| width | The width of the image (integer). | 
| height | The height of the image (integer). |
| numOfColors | The total number of colors being used as an integer (up to 36).
| hexColor | A 24-bit representation of a RGB color (the # of colors should equal numOfColors). |
| imageData | The remainder of this file is allocated to the characters used to represent the hexColors above. The dimension should be equivalent to width by height of the image. |

#### Example
```
.LCI
16                  ; width
8                   ; height
4                   ; numOfColors
E2D9E4              ; hexColor
6C9A9A              ; hexColor
52675D              ; hexColor
37403B              ; hexColor
0011223300112233    ; imageData
0011223300112233    ; imageData
0011223300112233    ; imageData
0011223300112233    ; imageData
3300112233001122    ; imageData
3300112233001122    ; imageData
3300112233001122    ; imageData
3300112233001122    ; imageData
```