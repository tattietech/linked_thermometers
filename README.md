# Linked Thermometers

A smart linked thermometer system using ESP32 and DHT22 sensors to monitor temperature and humidity data, with WiFi connectivity and API integration for remote monitoring.

## Project Status

**Active Development** - Core features are implemented and functional. Additional features and improvements are ongoing.

## Overview

This project implements a temperature and humidity monitoring system using ESP32 microcontrollers and DHT22 sensors. The system connects to WiFi and transmits sensor data to a cloud API endpoint via HTTPS, enabling remote monitoring of temperature and humidity readings from multiple devices. 

## Planned Features

- **OLED Display**: Small OLED screen to display temperature and humidity data locally
- **Smart Home Integration**: Display data from multiple thermometers in a centralized home dashboard
- **Enhanced Network Features**: Mesh networking between devices

## Current Features

- DHT22 sensor data reading with checksum validation
- Temperature measurement (°C)
- Humidity measurement (%)
- Continuous monitoring with 60-second intervals
- WiFi connectivity (WPA/WPA2)
- HTTPS API integration for data transmission
- MAC address-based device identification
- JSON-formatted data payloads
- Optimized data transmission (only sends when values change)
- Automatic WiFi reconnection on disconnection
- Configurable API endpoint and credentials

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
│   ├── main.c                  # Main application code with WiFi and API integration
│   ├── CMakeLists.txt          # Main component build configuration
│   └── Kconfig.projbuild       # WiFi and API configuration options
├── components/
│   ├── DHT22/
│   │   ├── dht22.c             # DHT22 sensor driver with checksum validation
│   │   ├── include/
│   │   │   └── dht22.h         # DHT22 sensor driver header
│   │   └── CMakeLists.txt      # DHT22 component build configuration
│   ├── wifi/
│   │   ├── wifi.c              # WiFi connection and management
│   │   ├── include/
│   │   │   └── wifi.h          # WiFi header
│   │   └── CMakeLists.txt      # WiFi component build configuration
│   └── http_client/
│       ├── http_client.c       # HTTPS client for API communication
│       ├── cert.pem            # SSL certificate for HTTPS
│       ├── include/
│       │   └── http_client.h   # HTTP client header
│       └── CMakeLists.txt      # HTTP client component build configuration
├── CMakeLists.txt              # Project-level CMake configuration
└── compile_run                 # Build and flash helper script
```

## Getting Started

### Prerequisites

1. Install ESP-IDF by following the [official guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)
2. Set up your ESP-IDF environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```

### Configuration

Before building, configure your WiFi and API settings:

```bash
idf.py menuconfig
```

Navigate to the appropriate menu and configure:

**Wi-Fi Configuration:**
- Wi-Fi SSID: Your network SSID
- Wi-Fi Password: Your network password

**Server Configuration:**
- API Endpoint: Your API server hostname (e.g., api.example.com)
- API Key: Your authentication key for the API

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

Once running, the device will:
1. Connect to the configured WiFi network
2. Read temperature and humidity from the DHT22 sensor every 60 seconds
3. Send data to the API endpoint when values change

The serial monitor will show connection status and transmission logs. The device identifies itself using its unique MAC address, and sensor data is transmitted as JSON:

```json
{
  "deviceId": "aabbccddeeff",
  "temperature": 23.45,
  "humidity": 45.67
}
```

## Configuration

The project uses ESP-IDF's Kconfig system for configuration. All settings can be configured via `idf.py menuconfig`:

- **WiFi credentials**: Required for network connectivity
- **API endpoint**: The hostname of your data collection server
- **API key**: Authentication key for secure API access

### SSL/TLS Certificate

The project includes HTTPS support for secure data transmission. Update `components/http_client/cert.pem` with your server's SSL certificate if using a custom API endpoint.

## Development

### DHT22 Sensor Driver

The DHT22 component provides a reliable interface with checksum validation:
- `get_data()`: Reads data from the sensor and validates checksum
- `get_temperature()`: Returns the last validated temperature reading
- `get_humidity()`: Returns the last validated humidity reading

Checksum validation ensures data integrity and prevents incorrect readings from being transmitted to the API.

### WiFi Component

The WiFi component handles network connectivity with automatic retry logic:
- Connects to WPA/WPA2 protected networks
- Automatically retries connection up to 3 times on failure
- Maintains connection and handles disconnections

### HTTP Client Component

The HTTP client provides secure data transmission:
- HTTPS POST requests to configurable API endpoint
- JSON payload formatting
- API key authentication via headers
- SSL/TLS certificate validation

### Main Application

The main application (`main.c`) orchestrates all components:
- Initializes WiFi and waits for connection
- Retrieves device MAC address for unique identification
- Continuously reads sensor data at 60-second intervals
- Optimizes API calls by only transmitting when values change
- Formats data as JSON with device ID, temperature, and humidity

## Contributing

This is a personal project, but suggestions and contributions are welcome! Feel free to open issues or submit pull requests.

## License

[Choose an appropriate license for your project]

## Author

TattieTech

## Acknowledgments

- ESP-IDF framework by Espressif Systems
- DHT22 sensor protocol implementation with checksum validation
- Community contributions and feedback

---

*Last updated: November 2025*
