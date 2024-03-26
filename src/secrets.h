// Use this file to store all of the private credentials
// and connection details

#ifdef WEST

#define SECRET_SSID "SOTON-IoT"                                // replace XXXXXX with your WiFi network name
#define SECRET_PASS "wbuQmgbm76Cr"                            // replace XXXXXX with your WiFi password

#define TX1 D1
#define RX1 D4
#define TX2 D3
#define RX2 D2

#define RELAY_1 D7
#define RELAY_2 D6 

#define SECRET_CH_ID 1916463                                 // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "U3TA3QZVDCDHSPFV"   

#elif defined(EAST)

#define SECRET_SSID "SOTON-IoT"                                // replace XXXXXX with your WiFi network name
#define SECRET_PASS "3a8DH2hhMNDf"  

#define TX1 D6
#define RX1 D5
#define TX2 D7
#define RX2 D8

#define RELAY_1 D3
#define RELAY_2 D4

#define SECRET_CH_ID 1916463                                 // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "U3TA3QZVDCDHSPFV"   

#else
#error "No side selected, please define east or west"
#endif


                        // replace XYZ with your channel write API Key
