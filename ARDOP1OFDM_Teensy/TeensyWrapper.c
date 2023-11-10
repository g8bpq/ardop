//
//	Makes keeping Windows, Linux and Teensy version in step easier

#define TEENSY
#define ARDOP
#define PTC

#include "TeensyConfig.h"

#include "../../ARDOP1OFDM/ofdm.c"
#include "../../ARDOP1OFDM/ARDOPC.c"
#include "../../ARDOP1OFDM/ardopSampleArrays.c"
#include "../../ARDOP1OFDM/ARQ.c"
#include "../../ARDOP1OFDM/berlekamp.c"
#include "../../ARDOP1OFDM/BusyDetect.c"
#include "../../ARDOP1OFDM/FEC.c"
#include "../../ARDOP1OFDM/FFT.c"
#include "../../ARDOP1OFDM/KISSModule.c"
#include "../../ARDOP1OFDM/galois.c"
#include "../../ARDOP1OFDM/HostInterface.c"
#include "../../ARDOP1OFDM/Modulate.c"
#include "../../ARDOP1OFDM/rs.c"
#include "../../ARDOP1OFDM/SCSHostInterface.c"
#include "../../ARDOP1OFDM/SoundInput.c"
#include "../../ARDOP1OFDM/afskModule.c"
#include "../../ARDOP1OFDM/pktARDOP.c"
#include "../../ARDOP1OFDM/pktSession.c"
#include "../../ARDOPCommonCode/ARDOPCommon.c"
