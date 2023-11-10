
// Arduino interface code for ARDOP running on a Teensy 3.6

#include "TeensyConfig.h"
#include "TeensyCommon.h"

#ifndef ARDOP
#error("ARDOP not defined in TeensyConfig.h");
#endif


#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

unsigned char RXBUFFER[500];	// Async RX Buffer. Enough for Stuffed Host Mode Frame

extern volatile int RXBPtr;
volatile int flag = 0;
volatile int flag2 = 0;
volatile int lastperiod = 0, lastint = 0;

int SerialHost = TRUE;				// Will eventually allow switching from Serial to I2c without reconfig
int ActivePort = 0;						// Serial port in use
int SerialWatchDog = 0;

extern int VRef;
extern int TXLevel;

void CommonSetup();
void setupPDB(int SampleRate);
void setupDAC();
void setupADC(int Pin);
extern "C" void StartDAC();
extern "C" void StopDAC();
void StartADC();
void setupOLED();
void setupKMR_18();

void setupWDTTFT();

// Arduino is a c++ environment so functions in ardop must be defined as "C"

extern "C"
{
  void WriteDebugLog(int Level, const char * format, ...);
  void InitSound();
  void HostInit();
  void CheckTimers();
  void HostPoll();
  void MainPoll();
  void InitDMA();
  void PlatformSleep(int mS);
  void AdjustTXLevel(int Level);
  void KISSInit();

  void ProcessSCSPacket(unsigned char * rxbuffer, int Length);

#include "../../ARDOPC/ARDOPC.h"

  extern unsigned int tmrPollOBQueue;

  extern int UseKISS;

#define Now getTicks()

#define MEM_LEN 512
  extern uint8_t databuf[MEM_LEN];
  extern volatile int i2cputptr, i2cgetptr;

  void HostPoll()
  {
#ifdef I2CHOST

    while (i2cgetptr != i2cputptr)
    {
      unsigned char c;
      c = databuf[i2cgetptr++];
      i2cgetptr &= 0x1ff;				// 512 cyclic

      RXBUFFER[RXBPtr++] = c;

      if (i2cgetptr == i2cputptr || RXBPtr > 498)
      {
        ProcessSCSPacket(RXBUFFER, RXBPtr);
        return;
      }
    }
#else

#ifdef i2cSlaveSupport
    i2cloop();					// I2C but not for Host
#endif

    // To simplify switching USB / Serial / Bluetooth / ESP we poll all defined ports,
    // and switch to using whichever provides input

    // Both boards could use USB(Serail) or Serial1
    // PIBoard has ESP on Serail3
    // WDT Board has BT on Serial5

    int Avail = HOSTPORT.available();
    int Count;

    if (Avail)
    {
      if (Avail > (499 - RXBPtr))
        Avail = 499 - RXBPtr;

      Count = HOSTPORT.readBytes((char *)&RXBUFFER[RXBPtr], Avail);
      RXBPtr += Count;
      if (ActivePort != 1)
      {
        ActivePort = 1;
        WriteDebugLog(LOGINFO, "Input is on Port %d", ActivePort);
      }

      ProcessSCSPacket(RXBUFFER, RXBPtr);
    }
#ifdef HOSTPORT2
    else
    {
      Avail = HOSTPORT2.available();
      if (Avail)
      {
        if (Avail > (499 - RXBPtr))
          Avail = 499 - RXBPtr;

        Count = HOSTPORT2.readBytes((char *)&RXBUFFER[RXBPtr], Avail);
        RXBPtr += Count;
        if (ActivePort != 2)
        {
          ActivePort = 2;
          WriteDebugLog(LOGINFO, "Input is on Port %d", ActivePort);
        }
        ProcessSCSPacket(RXBUFFER, RXBPtr);
      }
    }
#endif
#ifdef HOSTPORT3
    else
    {
      Avail = HOSTPORT3.available();
      if (Avail)
      {
        if (Avail > (499 - RXBPtr))
          Avail = 499 - RXBPtr;

        Count = HOSTPORT3.readBytes((char *)&RXBUFFER[RXBPtr], Avail);
        RXBPtr += Count;
        if (ActivePort != 3)
        {
          ActivePort = 3;
          WriteDebugLog(LOGINFO, "Input is on Port %d", ActivePort);
        }
        ProcessSCSPacket(RXBUFFER, RXBPtr);
      }
    }
#endif
#endif		// I2CHOST
  }
}

void setup()
{
  uint32_t i, sum = 0;

#ifdef I2CHOST
  SerialHost = FALSE;
#endif

  CommonSetup();

#if defined(__IMXRT1062__)
  WriteDebugLog(LOGALERT, "ARDOPC Version %s CPU %d Bus %d FreeRAM %d", ProductVersion, F_CPU_ACTUAL, F_BUS_ACTUAL, FreeRam());
#else
  WriteDebugLog(LOGALERT, "ARDOPC Version %s CPU %d Bus %d FreeRAM %d", ProductVersion, F_CPU, F_BUS, FreeRam());
#endif
blnTimeoutTriggered = FALSE;
  SetARDOPProtocolState(DISC);

  InitSound();
  HostInit();
  tmrPollOBQueue = Now + 10000;
  ProtocolMode = ARQ;

  if (UseKISS)
    KISSInit();

  // Configure the ADC and run at least one software-triggered
  // conversion.  This completes the self calibration stuff and
  // leaves the ADC in a state that's mostly ready to use

  analogReadRes(16);

#if defined(__IMXRT1062__)
#else
  analogReference(DEFAULT); // range 0 to 3.3 volts
#endif
  //analogReadAveraging(8);
  // Actually, do many normal reads, to start with a nice DC level

  for (i = 0; i < 1024; i++)
  {
    sum += analogRead(16);
  }

  WriteDebugLog(LOGDEBUG, "DAC Baseline %d", sum / 1024);

  setupPDB(12000);			// 12K sample rate
  setupDAC();
  setupADC(16);
  StartADC();

#ifdef PIBOARD

  // PI Baord can read Analog out which allows u to Read Vref

  SetPot(1, 256);				// TX Level Gain = 1

  delay(100);

  analogRead(17);

  for (i = 0; i < 100; i++)
  {
    VRef += analogRead(17);
  }
  VRef /= 100;
  analogRead(16);		// Set ADC back to A0

#endif


  WriteDebugLog(0, "VREF %d offset %d", VRef, VRef - 32768);

  AdjustTXLevel(TXLevel);

  print_mac();

#ifdef PLOTCONSTELLATION
#ifdef OLED
  setupOLED();
#endif
#ifdef WDTTFT
  setupWDTTFT();
#endif
#ifdef KMR_18
  setupKMR_18();
#endif

#endif
}

int lastticks = Now;

void loop()
{
  PollReceivedSamples();
  CheckTimers();
  HostPoll();
  MainPoll();

  if (Now - lastticks > 999)
  {
    // Debug 1 sec tick
  }
  PlatformSleep(0);
  //RadioPoll();
  Sleep(1);

#ifdef I2CHOST
  SerialWatchDog++;		// Reset if nothing for 60 secs
#endif

  if (blnClosing || SerialWatchDog > 60000)
  {
    CPU_RESTART // reset processor
  }
}

extern "C"
{
  int OKtoAdjustLevel()
  {
    // Only auto adjust level when disconnected.
    // Level is set at end of each received packet when connected

    return (ProtocolState == DISC);
  }

  void StartCapture()
  {
    Capturing = TRUE;
    DiscardOldSamples();
    ClearAllMixedSamples();
    State = SearchingForLeader;
  }
  void StopCapture()
  {
    Capturing = FALSE;
  }
  void TurnroundLink()
  {
    if (blnEnbARQRpt > 0 || blnDISCRepeating)	// Start Repeat Timer if frame should be repeated
      dttNextPlay = Now + intFrameRepeatInterval;
  }
}



// function from the sdFat library (SdFatUtil.cpp)
// licensed under GPL v3
// Full credit goes to William Greiman.

extern "C" char* sbrk(int incr);

extern char *__brkval;
extern char __bss_end;


int FreeRam()
{
  char top;

  return __brkval ? &top - __brkval : &top - &__bss_end;
}

// Function to get mac/serial number

uint8_t mac[8];

// http://forum.pjrc.com/threads/91-teensy-3-MAC-address

void readserialno(uint8_t *mac)
{
  noInterrupts();

  // With 3.6 have to read a block of 8 bytes at zero
  // With 3.1 two blocks of 4 at e and f

#if defined(__IMXRT1062__)
#else

  FTFL_FCCOB0 = 0x41;             // Selects the READONCE command
#if defined(__MK64FX512__) || defined(__MK66FX1M0__)
  FTFL_FCCOB1 = 0;             // read the given word of read once area
#else
  FTFL_FCCOB1 = 0x0F;          // read the given word of read once area
#endif
  FTFL_FCCOB2 = 0;
  FTFL_FCCOB3 = 0;
  // launch command and wait until complete
  FTFL_FSTAT = FTFL_FSTAT_CCIF;
  while (!(FTFL_FSTAT & FTFL_FSTAT_CCIF));

#if defined(__MK64FX512__) || defined(__MK66FX1M0__)

  // Top 3 bytes (Manufacture ID in bits 8-31

  *(mac++) = FTFL_FCCOB5;
  *(mac++) = FTFL_FCCOB6;
  *(mac++) = FTFL_FCCOB7;

  // Serial No in bits 8-31

  *(mac++) = FTFL_FCCOB9;
  *(mac++) = FTFL_FCCOBA;
  *(mac++) = FTFL_FCCOBB;
#else

  // Could read Manufacture ID from page E but no point as fixed (04:E9:E5)

  *(mac++) = 0x04;
  *(mac++) = 0xE9;
  *(mac++) = 0xE5;
  *(mac++) = FTFL_FCCOB5;
  *(mac++) = FTFL_FCCOB6;
  *(mac++) = FTFL_FCCOB7;


#endif
#endif

  interrupts();
}

void print_mac()
{
  readserialno(mac);
  WriteDebugLog(6, "Hardware Serial No %02X:%02X:%02X:%02X:%02X:%02X",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

extern "C"
{
  // Dummies to satisfy linker

  int _gettimeofday()
  {
    return 0;
  }

  // Stuff to support Common Code for ARDOP and Packet

  void pktProcessNewSamples(short * buf, int count)
  {}

  // Dummies for ARDOP GUI

  void DrawTXFrame(const char * Frame)
  {}

  int SendtoGUI(char Type, unsigned char * Msg, int Len)
  {
    return 0;
  }
}
