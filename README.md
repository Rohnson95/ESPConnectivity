﻿# ESPConnectivity
## Setting Up Environment Variables

To run this project, you need to add your WiFi credentials, InfluxDB token, and InfluxDB IP address.

1. **Create a `secrets.h` file** in the project root (same folder as the main `.ino` or `.cpp` file).
2. Add the following content to `secrets.h`:
   ```cpp
   #define WIFI_SSID "your_wifi_ssid"
   #define WIFI_PASSWORD "your_wifi_password"
   #define INFLUXDB_TOKEN "your_influxdb_token"
   #define INFLUXDB_IP "your_influxdb_ip"
