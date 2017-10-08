#include <Arduino.h>
#include <ESP8266WiFi.h>

#define VERSION "v0.1alpha"
#define HOSTNAME "SSI-Irrigation-OTA-" ///< Hostename. The setup_OTA_WBServer function adds the Chip ID at the end.
#define SERIAL_OUT Serial


//IGROMETRO
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IGROMETRO A0


//SLOT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SLOT_IGROMETRO          0
#define SLOT_VALVOLA_1          2     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_VALVOLA_2          3     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_VALVOLA_3          4     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_VALVOLA_4          5     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_VALVOLA_5          6     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_VALVOLA_6          7     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_VALVOLA_7          8     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_VALVOLA_8          9     // This is the memory slot used for the execution of the logic in network_address1


//PIN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **** Define here the right pin for your ESP module ****
#define RELE_1     3  
#define RELE_2     5  
#define RELE_3     4  
#define RELE_4     0  
#define RELE_5     10 
#define RELE_6     13 
#define RELE_7     14 
#define RELE_8     16 




