# APGB Converter
A tool designed to provide a convenient way of creating custom GB palettes for the Analogue Pocket or BGB Emulator.

## Overview
- [Quick Start](#quick-start)
- [Functionality](#functionality)
- [Format](#format)
- [Resources](#resources)

## Quick Start
#### Installation
- QT6 (and supported qmake)

#### Execute
1. Open the directory for this project.
2. Run the following commands:
```
export QT_VERSION_MAJOR=6
qmake -project 
qmake
make
```
This is should generate a executable for the application.
3. Run application

## Functionality
- **Editor** := Create a pal file (hex-based) for Analogue Pocket using a grahical interface .
![](/apgb_converter/img/editor.png)

- **Import** := Found in the Editor tab. Allows for the conversion of data into the appropriate format and provides the ability to update the colors via the graphical interface. 
    - `Supported file types`: **CSV**, **PAL** (APGB and JASC format)
    - CSV file structure:
       ```
       1, BG, 0x000000, 0x52528C, 0x8C8CDE, 0xFFFFFF
       2, OBJ0, 0x000000, 0x944A4A, 0xFF8484, 0xFFFFFF
       3, OBJ1, 0x000000, 0x843100, 0xFFAD63, 0xFFFFFF
       4, WINDOW, 0x000000, 0x555555, 0xAAAAAA, 0xFFFFFF
       ```
       - The order is not strict as long as the keyword is first
       - 24-bit colors can have a leading `0x`, `#`, or nothing
    - JASC format:  The color palettes are applied in a sequential manner for each palette and its respective intentisities as shown [here](#example).
        - only supports 4, 8, or 16 colors

- **Convert** := For simple conversions that do not require changes to the colors used.
![](/apgb_converter/img/convert.png)

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
