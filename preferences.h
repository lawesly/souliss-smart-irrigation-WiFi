#include <Arduino.h>

//CLOCK
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CLOCK 1
#define TIME_ZONE 1  // Central European Time
#define DAYLIGHTSAVINGTIME 0 // Ora legale


//SYSTEM AT BOOT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SYSTEM_START_ON 1 if SST start at power on
//SYSTEM_START_ON 0 if SST start at power off.
#define SYSTEM_START_ON 1
#define PROGRAMMI 1 // Programmi attivi
#define SETPOINT 50
#define SENSOREATTIVO 1
#define NUMEROVALVOLE 2



//WIFI CONNECTION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//0 Static Connection
//1 Dynamic Connection
#define DYNAMIC_CONNECTION 0

//DHCP
//1 Use DHCP IP
//0 Use Static IP Address
//0 Only for DYNAMIC_CONNECTION = 0
#define DHCP_OPTION 0

//STATIC IP
// use commas between number
#define STATIC_IP 192,168,0,80
#define STATIC_SUBNET 255,255,255,0
#define STATIC_IP_GW 192,168,0,1

// **** Define the WiFi name and password ****
//Only for DYNAMIC_CONNECTION = 0
#define WIFICONF_INSKETCH
#define WiFi_SSID               "Serenario-Desk"
#define WiFi_Password           "0xsersgheps05"   
#define http_username           "admin"
#define http_password           "gronsuca"

// Define the network configuration according to your router settingsuration according to your router settings
// and the other on the wireless oneless one
//Only for DYNAMIC_CONNECTION = 0
#define peer_address  0xAB15
#define myvNet_subnet 0xFF00
#define wifi_bridge_address    0xAB01 //gateway

