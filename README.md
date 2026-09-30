# SeaDrift 海漂計畫

Code for **SeaDrift**, an art-and-technology project exhibited at Ars Electronica Festival 2022 (National Tsing Hua University, Technology and Art). A drifting buoy measures motion, pressure, temperature and position, sends the readings over the Iridium satellite network, and the data is decoded and visualized.

```
Arduino buoy ──Iridium / RockBLOCK──▶ email ──▶ Python decoder ──▶ data list ──▶ p5.js / Processing visuals
 (GY-91, GPS, RTC)                                (IMAP)
```

## Repository layout

| Path | What it is |
| --- | --- |
| `code/arduino/main/` | Buoy firmware: GY-91 (MPU9250 + BMP280), GPS, RTC, buzzer/LED, Iridium send. Packs readings into a bit stream for the RockBLOCK 9603N. |
| `code/arduino/gy91/` | Standalone GY-91 sensor sketch. |
| `code/arduino/powerRTCalarm/` | RTC-alarm power control test. |
| `code/arduino/experiments/` | One-off sensor and module tests (accelerometer, barometer, compass, GPS, Iridium signal, sleep, wave height, …). |
| `code/python/seadrift-data/` | Polls the mailbox that receives RockBLOCK messages, decodes the payload and writes the values to a text file. |
| `code/python/raspberry-pi/` | DHT11 reader for the Raspberry Pi. |
| `code/python/stable-diffusion/` | Stable Diffusion img2img script used to generate creature images. |
| `code/processing/list_test/` | Processing sketch reading the decoded data list. |
| `code/web/seadrift-visualizer/` | p5.js visualizer showing the decoded data as text and dots. |
| `code/web/satellite-dashboard/` | p5.js dashboard predicting satellite passes (OpenProcessing export). |
| `code/hardware/` | Fritzing circuit (`main.fzz`). |

## Credits

- [25qi](https://github.com/25qi)
- 以諾: Python data decoding (`code/python/`) and visualization (`code/web/`, `code/processing/`)

## Version history

Every earlier version of each sketch/script (for example `main_v3` … `main_v25`) is preserved as a commit. Browse it with:

```sh
git log --oneline -- code/arduino/main
git show <commit>:code/arduino/main/main.ino
```

Each commit message records the original folder and date the version came from.

## Payload format

`seadrift_data.py` decodes the hex payload into a bit string. Each field starts with a sign bit:

| Field | Bits (incl. sign) | Scale |
| --- | --- | --- |
| Acceleration x / y / z | 10 each | ÷ 100 |
| Gyroscope x / y / z | 17 each | ÷ 100 |
| Pressure | 25 | ÷ 100 |
| Temperature | 14 | ÷ 100 |
| Height | 18 | ÷ 100 |
| Wave height | 18 | ÷ 100 |
| Longitude | 29 | ÷ 1,000,000 |
| Latitude | 28 | ÷ 1,000,000 |

## Setup

### Python

```sh
cp code/.env.example code/.env   # then fill in the values
pip install -r code/python/seadrift-data/requirements.txt
python code/python/seadrift-data/seadrift_data.py
```

| Variable | Used by |
| --- | --- |
| `SEADRIFT_EMAIL_USER`, `SEADRIFT_EMAIL_PASSWORD` | Mailbox receiving RockBLOCK emails |
| `ROCKBLOCK_IMEI` | Only emails from this device are decoded |
| `SEADRIFT_OUTPUT_FILE` | Where decoded values are written |
| `STABILITY_KEY` | Stable Diffusion script |

The Raspberry Pi script needs `Adafruit_DHT`; the Stable Diffusion script needs `stability-sdk`, `Pillow` and `ipython`.

### Arduino

Third-party libraries are not included in this repository. Install them with the Arduino Library Manager or from their repositories:

- `code/arduino/main`: TinyGPSPlus, AltSoftSerial, IridiumSBDi2c (SparkFun), FaBo 9Axis MPU9250, I2Cdev, I2C-Sensor-Lib (iLib, for `i2c.h` / `i2c_BMP280.h`)
- Other sketches additionally use: MPU6050, MS5611, HMC5883L, DS3232RTC, Adafruit INA219, Enerlib, ESP32 AnalogRead / AnalogWrite

### Web

Open `index.html` through a local web server (for example the VS Code Live Server extension). p5.js and moment.js load from cdnjs; the dashboard's `satellite.js` 1.2.0 and a modified `jspredict` 1.0.2 are kept in `vendor/` because those exact builds are required.
