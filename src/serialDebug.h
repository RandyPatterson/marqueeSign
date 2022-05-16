
//comment line to disable debug serial output
//#define DEBUG 

#ifdef DEBUG
  #define DEBUG_INIT Serial.begin(115200)
  #define DEBUG_PRINT(str) Serial.print(str)
  #define DEBUG_PRINTLN(str) Serial.println(str)
#else
  #define DEBUG_INIT
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif


