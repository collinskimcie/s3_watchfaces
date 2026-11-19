# Nightwatch on the VIEWE UEDX24320028E-WB-A

This repository now includes a PlatformIO firmware target for the 240×320 VIEWE ESP32-S3 display. It uses the manufacturer's supported board definition and LVGL v8. The LVGL Editor XML remains in `screens/digital_nightwatch/`; PlatformIO compiles the corresponding native LVGL UI in `src/main.cpp` (the XML is not compiled directly).

The small battery, step, voltage, and compass values are explicitly demo placeholders; this board has no built-in sensors for those values. The clock starts at 12:45 and advances using elapsed uptime, so it is a UI test rather than a real-time clock.

## Build and upload

From the root of this repository, after pulling the branch:

```bash
pio run
pio device list
pio run -t upload --upload-port /dev/ttyACM0
pio device monitor --port /dev/ttyACM0 --baud 115200
```

Use the port shown by `pio device list` if it differs. First build may take a while while PlatformIO downloads the board platform and libraries. The project selects the exact `BOARD_VIEWE_UEDX24320028E_WB_A` environment by default. If upload cannot enter download mode, hold BOOT while starting the upload, then release when writing begins.

The official board support and pin mapping are provided by [VIEWESMART's board repository](https://github.com/VIEWESMART/UEDX24320028ESP32-3.5inch-320_480-Display). See `platformio.ini` for dependencies and target settings.
