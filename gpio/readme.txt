GPIO tools for raspberry pi projects.

NOTE: gpio.h currently includes a #define directive for PI2 (#DEFINE PI2).  This switch
will cause the code to use the correct Broadcom base address for GPIO stuff.  If you are using
a Raspberry Pi v1, you will need to comment this line out as follows in gpio.h:

// Comment out this line for Raspberry Pi 1!!!
// #DEFINE PI2
