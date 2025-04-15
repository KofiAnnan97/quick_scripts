# Limited Color Image (LCI) Format
The primary purpose of this script is to allow for dynamic changing of image files as used in [apgb_converter](https://github.com/KofiAnnan97/apgb_converter). It is design to encode an image as a series of characters in which a single character respresents a color. The limited nature comes its intent to be used for old video game consoles with a limited color range that could be displayed such as the original Gameboy. 

## Build
This script relies on the OpenCV C++ libray and uses CMake to compile the code. In the same directory as `CMakeLists.txt` run the following:
```
cmake .
make
```

## Execution
### Encoder
Run the command below to convert an image into a .lci file. The colors will be ordered dynamically based on the sequence in which they first occur. The resulting file will be saved in the same directory as the source image.
```
.\lci-encoder -i \path\to\image.png
``` 

If the colors need to be in a specific sequence use the command below instead. The format of these colors is the same as the example of the `hexColors` values in a text file.
```
.\lci-encoder -i \path\to\image.png -p \path\to\colors.txt
```

### Decoder
Run the command below to convert a LCI file to an image. Like the encoder the resulting output image will be saved in the same directory as the source file. 
```
.\lci-decoder -i \path\to\image.lci
``` 

This decoder can be used to output the data in multiple image formats. Use one of the following flags in the command line to change the image format.
- `--jpg` := Convert LCI file into an JPG image (default)
- `--png` := Convert LCI file into a PNG image

## LCI Format
The limited color image (LCI) format is a text file with the `.lci` extension that uses a singular hexadecimal value to represent a color.
| Value | Description |
|-------|-------------| 
| id | The ASCII representation of `.LCI` |
| width | The width of the image (integer). | 
| height | The height of the image (integer). |
| numOfColors | The total number of colors being used as an integer (up to 36).
| hexColors | A 24-bit representation of a RGB color (the # of colors should equal numOfColors). |
| imageData | The remainder of this file is allocated to the characters used to represent the hexColors above. The dimension should be equivalent to width by height of the image. |

#### Example
```
.LCI
16                  ; width
8                   ; height
4                   ; numOfColors
E2D9E4              ; hexColors
6C9A9A              ; hexColors
52675D              ; hexColors
37403B              ; hexColors
0011223300112233    ; imageData
0011223300112233    ; imageData
0011223300112233    ; imageData
0011223300112233    ; imageData
3300112233001122    ; imageData
3300112233001122    ; imageData
3300112233001122    ; imageData
3300112233001122    ; imageData
```