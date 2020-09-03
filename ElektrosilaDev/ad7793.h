#ifndef AD7793_H
#define AD7793_H

typedef struct {
    unsigned : 2; //0
    unsigned CREAD : 1; //0
    unsigned RS : 3; //0
    unsigned R_nW : 1; //0
    unsigned nWEN : 1; //0
} COMMUNICATIONS_t; //0x00

enum RS {
    Communications_Register = 0, // 8-bit During a Write Operation
    Status_Register = 0, //         8-bit During a Read Operation
    Mode_Register, //               16-bit
    Configuration_Register, //      16-bit
    Data_Register, //               16-bit (AD7792) / 24-bit (AD7793)
    ID_Register, //                 8-bit
    IO_Register, //                 8-bit
    Offset_Register, //             16-bit (AD7792) / 24-bit (AD7793)
    Full_Scale_Register, //         16-bit (AD7792) / 24-bit (AD7793)
};

typedef struct {
    unsigned CH0 : 1; //0
    unsigned CH1 : 1; //0
    unsigned CH2 : 1; //0
    unsigned TYPE : 1; //0 (AD7792) / 1 (AD7793)
    unsigned : 2; //0
    unsigned ERR : 1; //0
    unsigned nRDY : 1; //1
} STATUS_t; //0x80

typedef struct {
    unsigned FS : 4; //10
    unsigned : 2; //0
    unsigned CLK : 2; //0
    unsigned : 5; //0
    unsigned MD : 3; //0
} MODE_t; //0x000A

enum CLK {
    CLK_0, // Internal 64 kHz Clock. Internal clock is not available at the CLK pin.
    CLK_1, // Internal 64 kHz Clock. This clock is made available at the CLK pin.
    CLK_2, // External 64 kHz Clock Used. An external clock gives better 50 Hz/60 Hz rejection. See specifications for external clock.
    CLK_3, // External Clock Used. The external clock is divided by 2 within the AD7792/AD7793.
};
enum MD {
    MD_0, // Continuous Conversion Mode (Default).
    MD_1, // Single Conversion Mode.
    MD_2, // Idle Mode.
    MD_3, // Power-Down Mode.
    MD_4, // Internal Zero-Scale Calibration.
    MD_5, // Internal Full-Scale Calibration.
    MD_6, // System Zero-Scale Calibration.
    MD_7, // System Full-Scale Calibration.
};
enum FS { // fADC(Hz)   tSETTLE(ms)     Rejection @ 50 Hz/60 Hz (Internal Clock)
    FS_0, // x          x
    FS_1, // 470        4
    FS_2, // 242        8
    FS_3, // 123        16
    FS_4, // 62         32
    FS_5, // 50         40
    FS_6, // 39         48
    FS_7, // 33.2       60
    FS_8, // 19.6       101             90 dB (60 Hz only)
    FS_9, // 16.7       120             80 dB (50 Hz only)
    FS_10, // 16.7      120             65 dB (50 Hz and 60 Hz)
    FS_11, // 12.5      160             66 dB (50 Hz and 60 Hz)
    FS_12, // 10        200             69 dB (50 Hz and 60 Hz)
    FS_13, // 8.33      240             70 dB (50 Hz and 60 Hz)
    FS_14, // 6.25      320             72 dB (50 Hz and 60 Hz)
    FS_15, // 4.17      480             74 dB (50 Hz and 60 Hz)
};

typedef struct {
    unsigned CHANNEL : 3; //0
    unsigned : 1; //0
    unsigned BUF : 1; //1
    unsigned : 2; //0
    unsigned REFSEL : 1; //0
    unsigned GAIN : 3; //7
    unsigned BOOST : 1; //0
    unsigned U_nB : 1; //0
    unsigned BO : 1; //0
    unsigned VBIAS : 2; //0
} CONFIGURATION_t; //0x0710

enum VBIAS {
    VBIAS_0, // Bias voltage generator disabled
    VBIAS_1, // Bias voltage connected to AIN1(−)
    VBIAS_3, // Bias voltage connected to AIN2(−)
    VBIAS_4, // Reserved
};
enum GAIN { // Gain     ADC Input Range (2.5 V Reference)
    GAIN_0, // 1        (In-amp not used) 2.5 V
    GAIN_1, // 2        (In-amp not used) 1.25 V
    GAIN_2, // 4        625 mV
    GAIN_3, // 8        312.5 mV
    GAIN_4, // 16       156.2 mV
    GAIN_5, // 32       78.125 mV
    GAIN_6, // 64       39.06 mV
    GAIN_7, // 128      19.53 mV
};

enum REFSEL { // Reference Source
    REFSEL_0, // External Reference Applied between REFIN(+) and REFIN(–).
    REFSEL_1, // Internal Reference Selected.
};
enum CHANNEL { // Channel           | Calibration Pair
    CHANNEL_0, // AIN1(+) – AIN1(–) | 0
    CHANNEL_1, // AIN2(+) – AIN2(–) | 1
    CHANNEL_2, // AIN3(+) – AIN3(–) | 2
    CHANNEL_3, // AIN1(–) – AIN1(–) | 0
    CHANNEL_4, // Reserved          |
    CHANNEL_5, // Reserved          |
    CHANNEL_6, // Temp Sensor       | Automatically selects gain = 1 and internal reference
    CHANNEL_7, // AVDD Monitor      | Automatically selects gain = 1/6 and 1.17 V reference
};

typedef struct {
    unsigned ID : 4;
    unsigned : 4;
} ID_t; //Power-On/Reset = 0xXA (AD7792)/0xXB (AD7793)

enum ID {
    AD7792 = 0xA,
    AD7793 = 0xB,
};

typedef struct {
    unsigned IEXCEN : 2; //0
    unsigned IEXCDIR : 2; //0
    unsigned : 4; //0
} IO_t; //0x00

enum IEXCDIR { // Current Source Direction
    IEXCDIR_0, // Current Source IEXC1 connected to Pin IOUT1, Current Source IEXC2 connected to Pin IOUT2.
    IEXCDIR_1, // Current Source IEXC1 connected to Pin IOUT2, Current Source IEXC2 connected to Pin IOUT1.
    IEXCDIR_2, // Both current sources connected to Pin IOUT1. Permitted when the current sources are set to 10 μA or 210 μA only.
    IEXCDIR_3, // Both current sources connected to Pin IOUT2. Permitted when the current sources are set to 10 μA or 210 μA only.
};

enum IEXCEN { // Current Source Value
    IEXCEN_0, // Excitation Current Disabled.
    IEXCEN_1, // 10 μA
    IEXCEN_2, // 210 μA
    IEXCEN_3, // 1 mA
};

class ad7793 {
public:
    ad7793();
};

#endif // AD7793_H
