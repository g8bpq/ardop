//
//	Makes keeping Windows, Linux and Teensy version in step easier

#define TEENSY
#define ARDOP
#define PTC

#include "TeensyConfig.h"

#include "../../ARDOPOFDM/ofdm.c"
#include "../../ARDOPOFDM/ARDOPC.c"
#include "../../ARDOPOFDM/ardopSampleArrays.c"
#include "../../ARDOPOFDM/ARQ.c"
#include "../../ARDOPOFDM/berlekamp.c"
#include "../../ARDOPOFDM/BusyDetect.c"
#include "../../ARDOPOFDM/FEC.c"
#include "../../ARDOPOFDM/FFT.c"
#include "../../ARDOPOFDM/KISSModule.c"
#include "../../ARDOPOFDM/galois.c"
#include "../../ARDOPOFDM/HostInterface.c"
#include "../../ARDOPOFDM/Modulate.c"
#include "../../ARDOPOFDM/rs.c"
#include "../../ARDOPOFDM/SCSHostInterface.c"
#include "../../ARDOPOFDM/SoundInput.c"
//#include "../../ARDOPOFDM/direwolf/demod_afsk.c"
//#include "../../ARDOPOFDM/direwolf/dsp.c"
//#include "../../ARDOPOFDM/direwolf/hdlc_rec.c"
#include "../../ARDOPOFDM/afskModule.c"
//#include "../../ARDOPC/costab.c"
//#include "../../ARDOPC/modem.c"
#include "../../ARDOPOFDM/pktARDOP.c"
#include "../../ARDOPOFDM/pktSession.c"
#include "../../ARDOPCommonCode/ARDOPCommon.c"
