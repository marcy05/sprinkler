#ifndef LOGGER_H
#define LOGGER_H

#define DEVELOPMENT 1

    #if BUILD_TYPE == DEVELOPMENT
        #define debug(x) Serial.print(x)
        #define debugln(x) Serial.println(x)
    #else
        #define debug(x)
        #define debugln(x)
    #endif


#endif // LOGGER_H