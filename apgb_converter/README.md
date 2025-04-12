# APGB Converter
A tool designed to provide a convenient way of creating custom GB palettes for the Analogue Pocket or BGB Emulator.

## Overview
- [Quick Start](#quick-start)
- [Functionality](#functionality)
- [Format](#format)
- [Resources](#resources)

## Quick Start
### Pre-requistes
- C++17 or higher
- QT6 (and supported qmake)

### Build and Run
##### Option 1: QT Creator
1. Open project through interface
2. Build project
3. Navigate to executable from `build/` directory 
4. Run application executable
##### Option 2: Command Line
1. Open the directory for this project.
2. Configure QT and Qmake
    ```
    export QT_VERSION_MAJOR=6
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/Qt/<version>/gcc_64/lib
    export QTDIR=/path/to/Qt/<version>/gcc_64
    ```
3. Build Project
     ```
    qmake -project 
    qmake
    make
    ```
3. Run application executable

## Functionality
- **Editor** := Create a pal file (hex-based) for Analogue Pocket using a grahical interface.
![](/apgb_converter/graphics/visual/editor.png)
    - The dynamic image window provides a preview or visual representation of how each palette and their intensities affect the overall look.
    - If you would like to include more test images please use this [script](https://github.com/KofiAnnan97/quick_scripts/tree/master/recreation/limited_color_img_encoder) to encode your image into a suitable format (with `.lci` extension). 

- **Import** := Found in the Editor tab. Allows for the conversion of data into the appropriate format and provides the ability to update the colors via the graphical interface. 
    - `Supported file types`: **CSV**, **PAL** (APGB and JASC format)
    - CSV file structure:
       ```
       BG, 0x000000, 0x52528C, 0x8C8CDE, 0xFFFFFF
       OBJ0, 0x000000, 0x944A4A, 0xFF8484, 0xFFFFFF
       OBJ1, 0x000000, 0x843100, 0xFFAD63, 0xFFFFFF
       WINDOW, 0x000000, 0x555555, 0xAAAAAA, 0xFFFFFF
       ```
       - The order is not strict as long as the keyword is first
       - 24-bit colors can have a leading `0x`, `#`, or nothing
    - JASC format:  The color palettes are applied in a sequential manner for each palette and its respective intentisities as shown [here](#example).
        - only supports 4, 8, or 16 colors

- **Convert** := For simple conversions that do not require changes to the colors used.
![](/apgb_converter/graphics/visual/convert.png)

## Format
- 56 bytes
- 4 total palettes
    - `BG` := background & tile layers
    - `OBJ0` := used on most sprites
    - `OBJ1` := some games use this for certain sprities
    - `Window` := static elements (i.e. status bar); normally the same as te BG palette
- each palette has 4 intensities (from dark to bright)
- each intensity is a 24-bit color
- Footer: character `0x81` + ASCII for `APGB`
    #### Example
    ```
    000000 52528C 8C8CDE FFFFFF   ; BG
    000000 944A4A FF8484 FFFFFF   ; OBJ0
    000000 843100 FFAD63 FFFFFF   ; OBJ1
    000000 555555 AAAAAA FFFFFF   ; Window
    FFFFFF 81 41504742            ; LCDoff and footer
    ```

## Resources
- [Analogue - Custom Palettes](https://www.analogue.co/developer/docs/custom-palettes)
- [JASC Palette Format](https://liero.nl/lierohack/docformats/other-jasc.html)
