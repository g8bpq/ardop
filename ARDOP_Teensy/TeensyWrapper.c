//
//	Makes keeping Windows, Linux and Teensy version in step easier

#define TEENSY
#define ARDOP
#define PTC

#include "TeensyConfig.h"

#include "../../ARDOPC/ARDOPC.c"
#include "../../ARDOPC/ardopSampleArrays.c"
#include "../../ARDOPC/ARQ.c"
#include "../../ARDOPC/berlekamp.c"
#include "../../ARDOPC/BusyDetect.c"
#include "../../ARDOPC/FEC.c"
#include "../../ARDOPC/FFT.c"
#include "../../ARDOPC/KISSModule.c"
#include "../../ARDOPC/galois.c"
#include "../../ARDOPC/HostInterface.c"
#include "../../ARDOPC/Modulate.c"
#include "../../ARDOPC/rs.c"
#include "../../ARDOPC/SCSHostInterface.c"
#include "../../ARDOPC/SoundInput.c"
//#include "../../ARDOPC/direwolf/demod_afsk.c"
//#include "../../ARDOPC/direwolf/dsp.c"
//#include "../../ARDOPC/direwolf/hdlc_rec.c"
#include "../../ARDOPC/afskModule.c"
//#include "../../ARDOPC/costab.c"
//#include "../../ARDOPC/modem.c"
#include "../../ARDOPC/pktARDOP.c"
#include "../../ARDOPC/pktSession.c"
#include "../../ARDOPCommonCode/ARDOPCommon.c"
