# Quick Script Repository
A collection of scripts to accomplish various tasks.

### Scripts Directory
| Name | Directory | Programming Language | Description |
|------|-----------|----------------------|-------------|
| [Ping Checker](/Utility/ping_checker/ping_checker.py) | `Utility` | Python3 | Simple tool to ping a list of ip addresses. |
| [DataManager](/PurdueResearch/DataManager.py) | `PurdueResearch` | Python3 |A simple way for converting into a more concise string packets for constrained payloads.  |
| [GPStoCartesian](/PurdueResearch/GPStoCartesian.py) | `PurdueResearch` | Python3 | Convert GPS coordinates to Cartesian using UTM |
| [SerialTest](/PurdueResearch/SerialTest.py) | `PurdueResearch` | Python3 | A quick script to test that the serial communication works |
| [Visualizer](/PurdueResearch/Visualizer.py) | `PurdueResearch` | Python3 | Generate graph in real-time or using a CSV file |
| [PoseWithKalman](/PurdueResearch/PoseWithKalman.py) | `PurdueResearch` | Python3 | Apply acceleration data in a Kalman filter to pose data from CSV |
| [kalman](/PurdueResearch/kalman2.py) | `PurdueResearch` | Python3 | A kalman filter |
| [covariance](/PurdueResearch/covariance.py) | `PurdueResearch` | Python3 | Calculate covariance |
| [LCI Formatter](https://github.com/KofiAnnan97/quick_scripts/tree/master/Recreation/limited_color_img_format) | `Recreation` | C++ | A script that converts between image files and the limited color image format (.lci). This format was created to be a malluable representation of an image for the [`APGB Converter`](https://github.com/KofiAnnan97/apgb_converter) application. |
| [Code Line Count](/Utility/code_lines.sh) | `Utility` | Bash | A simple bash script that counts the approximate number of lines of code that are present within a repository. |

### External Scripts
The following scripts were once included in this repository and have been split off into their own projects.

| Name                  | Description                                                                                                                                                                                                                                                                             | URL                                                       |
|-----------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------------|
| `APGB Converter`      | A GUI for creating Analogue Pocket GameBoy Palettes                                                                                                                                                                                                                                     | [link](https://github.com/KofiAnnan97/apgb_converter)     |
| `Game Sales Scrapper` | The purpose of this script is to scrape the Steam Web API and GOG API to determine whether a game has reached a specified price threshold. If one or more games fall below the user-defined limit an email will be sent containing a list of games along with their respective prices.  | [link](https://github.com/KofiAnnan97/games_sales_scrapper) |                                     