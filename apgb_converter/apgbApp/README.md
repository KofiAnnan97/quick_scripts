# APGB Converter
This script is designed to provied a more convenient way of creating custom GB palattes for the Analogue Pocket.

## Overview
- [Quick Start](#quick-start)
- [Functionality](#functionality)
- [Format](#format)
- [Resources](#resources)

## Quick Start


## Functionality
- **Generate** := Create a pal file (hex-based) for Analogue Pocket using a grahical interface .
![](/apgb_converter/apgbApp/img/editor.png)

- **Import** := Converts data into appropriate format and allows for editting in the graphical interface. 
    - `Supported file types`: **CSV**, **JASC**(Text)
    - for the JASC format the colors are applied in a sequential manner to the palettes and respective intentisities.

- **Convert** := For simple conversions that do not require changes to the colors used.
![](/apgb_converter/apgbApp/img/convert.png)

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
