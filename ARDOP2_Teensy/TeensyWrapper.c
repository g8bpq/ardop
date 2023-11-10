//
//	Makes keeping Windows, Linux and Teensy version in step easier

#define TEENSY
#define ARDOP
#define PTC

#include "TeensyConfig.h"

#include "../../ARDOP2/ARDOPC.c"
#include "../../ARDOP2/ardopSampleArrays.c"
#include "../../ARDOP2/ARQ.c"
#include "../../ARDOP2/berlekamp.c"
#include "../../ARDOP2/BusyDetect.c"
#include "../../ARDOP2/FEC.c"
#include "../../ARDOP2/FFT.c"
#include "../../ARDOP2/KISSModule.c"
#include "../../ARDOP2/galois.c"
#include "../../ARDOP2/HostInterface.c"
#include "../../ARDOP2/Modulate.c"
#include "../../ARDOP2/rs.c"
#include "../../ARDOP2/SCSHostInterface.c"
#include "../../ARDOP2/SoundInput.c"
//#include "../../ARDOP2/direwolf/demod_afsk.c"
//#include "../../ARDOP2/direwolf/dsp.c"
//#include "../../ARDOP2/direwolf/hdlc_rec.c"
#include "../../ARDOP2/afskModule.c"
//#include "../../ARDOPC/costab.c"
//#include "../../ARDOPC/modem.c"
#include "../../ARDOP2/pktARDOP.c"
#include "../../ARDOP2/pktSession.c"
#include "../../ARDOPCommonCode/ARDOPCommon.c"
