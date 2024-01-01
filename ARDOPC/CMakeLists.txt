cmake_minimum_required(VERSION 3.10)

project(ardopc)

set(SOURCES
    ../ARDOPCommonCode/ARDOPCommon.c
    ../ARDOPCommonCode/LinSerial.c
    KISSModule.c
    pktARDOP.c
    pktSession.c
    BusyDetect.c
    i2cDisplay.c
    ../ARDOPCommonCode/ALSASound.c
    ARDOPC.c
    ardopSampleArrays.c
    ARQ.c
    FFT.c
    FEC.c
    HostInterface.c
    Modulate.c
    rs.c
    berlekamp.c
    galois.c
    SoundInput.c
    TCPHostInterface.c
    SCSHostInterface.c
)

add_executable(ardopc ${SOURCES})

# Configuration options
target_compile_options(ardopc PRIVATE -DLINBPQ -MMD -g)

# Linker options
target_link_libraries(ardopc PRIVATE rt m pthread asound)
