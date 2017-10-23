# souliss-smart-irrigation-WiFi
ESP8266 WiFi Smart Irrigation System based on Souliss IoT Framework

# Bill of Material:

- n.1 PCB
- n.1 Esp-12 E/F 
- n.1 resistor 1k 
- n.1 cap 1000mf 
- n.1 2/4/8 relay board (optoisolated) 
- n.1 Soil Hygrometer Humidity Detection Module (optional)
- n.1 LM2596 step down dc-dc board
- n.1 AMS1117 3.3v
- n.2/4/8 12v valve like FPD-270A
- n.1 12v power supply (2/4A)
- n.1 enclosure ip68

# How to connect your system:

- use lm2596 to step down from 12v to 5v and power on relay board
- use ams1117 to step down from 5v to 3.3v and power on esp12
- Use 1k res between VCC and CHPD
- Use cap between esp gnd and vcc
- connect Hygrometer Analogic pin to ESP A0
- connect Relay1 to GPIO 3
- connect Relay2 to GPIO 5
- connect Relay3 to GPIO 4
- connect Relay4 to GPIO 0
- connect Relay5 to GPIO 10
- connect Relay6 to GPIO 13
- connect Relay7 to GPIO 14
- connect Relay8 to GPIO 16
- Connect 12v to any single input of relay board 
- connect output of any single relay to valve +

That's all folks!!!

# Know bugs:
- Programs from 23.59 to 00.00 doesn't work 
- Program with overlapping valves pheraps can't work well in the minute of ending active program

