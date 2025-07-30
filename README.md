# Internet of Things-based Pest-Free Plant Monitoring System 


This project is an IoT-based solution for real-time plant health and pest prevention monitoring. It integrates various sensors, microcontrollers, and software platforms to automate data collection, analysis, and visualization for maintaining a pest-free environment for plants.

# System Components
## Sensors

DHT11 — Measures temperature and humidity.

Ultrasonic Sensor — Detects object movement to trigger pest deterrence.

Water Level Sensor — Monitors the water level in the irrigation system.

## Actuators

Micro Servo — Moves in response to ultrasonic sensor detection.

Water Pump — Controls irrigation based on sensor data.

Relay 2 Channel — Controls power delivery to actuators.

## Endpoint Controller (ESP32)

Reads environmental data (temperature, humidity, water level) and sends it to an I2C display and a central database.

Detects motion via the ultrasonic sensor and activates the micro servo.

Sends water level data to the database for monitoring and irrigation control.

Uses relay timers to control the water pump.

## Connectivity

ESP32 connects to Wi-Fi using configured SSID and password.

Sends sensor data to a Flask API server for processing and storage.

## Server and Database

Flask API receives data from ESP32 devices.

MySQL database stores temperature, humidity, and water level readings.

## Application Platform

Streamlit-based dashboard for real-time data visualization.

Displays sensor readings and system status using HTTP-based communication.
