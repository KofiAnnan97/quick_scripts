# Quick Script Repository
A collection of scripts to accomplish various tasks.

### Scripts Directory
| Name | Directory | Description |
|------|-----------|-------------|
| [DataManager](/PurdueResearch/DataManager.py) | `PurdueResearch` | A simple way for converting into a more concise string packets for constrained payloads.  |
| [GPStoCartesian](/PurdueResearch/GPStoCartesian.py) | `PurdueResearch` |Convert GPS coordinates to Cartesian using UTM |
| [SerialTest](/PurdueResearch/SerialTest.py) | `PurdueResearch` |A quick script to test that the serial communication works |
| [Visualizer](/PurdueResearch/Visualizer.py) | `PurdueResearch` |Generate graph in real-time or using a CSV file |
| [PoseWithKalman](/PurdueResearch/PoseWithKalman.py) | `PurdueResearch` |Apply acceleration data in a Kalman filter to pose data from CSV |
| [kalman](/PurdueResearch/kalman2.py) | `PurdueResearch` |A kalman filter |
| [covariance](/PurdueResearch/covariance.py) | `PurdueResearch` |Calculate covariance |
| [LCI Encoder](/Recreation/limited_color_img_format/src/encoder.cpp) | `Recreation` | A script to convert images into a malluable representation of an image  based on my text file format [`LCI`](/Recreation/limited_color_img_format/README.md) (used to develop dynamic images for `APGB Converter`) |
| [LCI Decoder](/Recreation/limited_color_img_format/src/decoder.cpp) | `Recreation` | A script to convert LCI files into JPEGs |

### External Scripts Directory
The following scripts were once included in this repository or other scripts that I find useful and revamped into their own projects.

| Name | Description | URL |
|------|-------------|-----|
| `Simple CSV Grapher` | A script that generates 2D plots, 3D plots, animated plots and live plots based on a CSV file. | [link](https://github.com/KofiAnnan97/simple_csv_grapher) |
| `APGB Converter` | A GUI for creating Analogue Pocket GameBoy Palettes | [link](https://github.com/KofiAnnan97/apgb_converter)