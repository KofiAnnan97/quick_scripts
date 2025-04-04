# APGB Converter
This script is designed to provied a more convenient way of creating custom GB palattes for the Analogue Pocket.

## Format
- 56 bytes
- 4 total palettes
- each palette has 4 intensities (from dark to bright)
- each intensity is a 24-bit color
- footer is `0x81` + ASCII for `APGB`

## Example
```
000000 52528C 8C8CDE FFFFFF   ; BG
000000 944A4A FF8484 FFFFFF   ; OBJ0
000000 843100 FFAD63 FFFFFF   ; OBJ1
000000 555555 AAAAAA FFFFFF   ; Window
FFFFFF 81 41504742            ; LCDoff and footer
```

## Resources
[Custom Palettes](https://www.analogue.co/developer/docs/custom-palettes)