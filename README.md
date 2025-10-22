# Linked Thermometers - In Progress

A smart linked thermometer system using ESP32 and DHT22 sensors to monitor temperature and humidity data.

## Project Status

🚧 **Active Development** - This project is currently under active development.

## Overview

This project implements a temperature and humidity monitoring system using ESP32 microcontrollers and DHT22 sensors. The current implementation reads sensor data and displays it via serial output. 

## Planned Features

- 🖥️ **OLED Display**: Small OLED screen to display temperature and humidity data locally
- 📡 **WiFi Connectivity**: Connect the board to WiFi for remote data transmission
- 🌐 **API Integration**: Push sensor data to a custom API endpoint
- 🏠 **Smart Home Integration**: Display data from multiple thermometers in a centralized home dashboard
- 🔗 **Linked System**: Create a network of thermometers throughout your home

## Current Features

- ✅ DHT22 sensor data reading
- ✅ Temperature measurement (°C)
- ✅ Humidity measurement (%)
- ✅ Continuous monitoring with 5-second intervals
- ✅ Serial output of readings

## Hardware Requirements

- ESP32 development board
- DHT22 temperature and humidity sensor
- Connecting wires
- USB cable for programming and power

### Wiring

- DHT22 Data Pin → GPIO 4 on ESP32
- DHT22 VCC → 3.3V
- DHT22 GND → GND

## Software Dependencies

- [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) (Espressif IoT Development Framework)
- CMake (version 3.16 or higher)

## Project Structure

```
linked_thermometers/
├── main/
│   ├── thermometer.c          # Main application code
│   ├── CMakeLists.txt         # Main component build configuration
│   └── Kconfig.projbuild      # WiFi configuration options
├── components/
│   └── DHT22/
│       ├── dht22.c            # DHT22 sensor driver implementation
│       ├── include/
│       │   └── dht22.h        # DHT22 sensor driver header
│       └── CMakeLists.txt     # DHT22 component build configuration
├── CMakeLists.txt             # Project-level CMake configuration
└── compile_run                # Build and flash helper script
```

## Getting Started

### Prerequisites

1. Install ESP-IDF by following the [official guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)
2. Set up your ESP-IDF environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```

### Configuration

Before building, configure your WiFi settings (for future features):

```bash
idf.py menuconfig
```

Navigate to "Wi-Fi Configuration" and set:
- Wi-Fi SSID
- Wi-Fi Password

### Building

Build the project:

```bash
idf.py build
```

### Flashing and Monitoring

Flash the firmware to your ESP32 and monitor the output:

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Replace `/dev/ttyUSB0` with your actual serial port (e.g., `/dev/ttyUSB1` on Linux, `COM3` on Windows, `/dev/cu.usbserial-*` on macOS).

Or use the provided helper script:

```bash
./compile_run
```

### Expected Output

Once running, you should see temperature and humidity readings updating every 5 seconds:

```
temp - 23.45°C
humidity - 45.67%
```

## Configuration

The project uses ESP-IDF's Kconfig system for configuration. WiFi credentials can be set via `idf.py menuconfig` under the "Wi-Fi Configuration" menu.

## Development

### Sensor Driver

The DHT22 component provides a simple interface:
- `get_data()`: Reads data from the sensor
- `get_temperature()`: Returns the last temperature reading
- `get_humidity()`: Returns the last humidity reading

### Main Application

The main application (`thermometer.c`) continuously reads sensor data and displays it on the serial console with proper formatting (overwriting previous lines for a clean display).

## Contributing

This is a personal project, but suggestions and contributions are welcome! Feel free to open issues or submit pull requests.

## License

[Choose an appropriate license for your project]

## Author

TattieTech

## Acknowledgments

- ESP-IDF framework by Espressif Systems
- DHT22 sensor protocol implementation

---

*Last updated: October 2025*
