//////////// Standard Header file for the PIC18F67J94 device ////////////////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996, 2020 Custom Computer Services          ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////
#device PIC18F67J94

#nolist
//////// Program memory: 65528x16  Data RAM: 3578  Stack: 31
//////// I/O: 51   Analog Pins: 16
//////// C Scratch area: 00   ID Location: 0000
//////// Fuses: STVREN,NOSTVREN,XINST,NOXINST,DEBUG,NODEBUG,NOBROWNOUT
//////// Fuses: BROWNOUT_SW,BORV20,BORV18,PROTECT,NOPROTECT,FRC,FRC_PLL,PR
//////// Fuses: PR_PLL,SOSC,LPRC,FRC_DIV_BY_16,FRC_DIV,SOSC_DIG,SOSC_LOW
//////// Fuses: CLOCKOUT,NOCLOCKOUT,IESO,NOIESO,PLL1,PLL2,PLL3,PLL4,PLL5
//////// Fuses: PLL6,PLL10,PLL12,PLL4X,PLL6X,PLL8X,NOPLL,EC,MS,HS,NOPR
//////// Fuses: CKSFSM,CKSNOFSM,NOCKSNOFSM,NOWPFP,WPFP,WPDIS,NOWPDIS,WPBEG
//////// Fuses: WPEND,WPCFG,NOWPCFG,T5G_IS_T3G,T5G_IS_T5G,CINA_ALT
//////// Fuses: CINA_DEFAULT,IOL1WAY,NOIOL1WAY,LS24MHZ,LS48MHZ,MSSP2MSK5
//////// Fuses: MSSP2MSK7,MSSPMSK5,MSSPMSK7,WDTWIN_75%,WDTWIN_50%,WDTWIN_37%
//////// Fuses: WDTWIN_25%,WDTCLK_PERIPHERAL,WDTCLK_SOSC,WDTCLK_LPRC
//////// Fuses: WDTCLK_FRC,WDT1,WDT2,WDT4,WDT8,WDT16,WDT32,WDT64,WDT128
//////// Fuses: WDT256,WDT512,WDT1024,WDT2048,WDT4096,WDT8192,WDT16384
//////// Fuses: WDT32768,NOWDT,WDT_NOSL,WDT_SW,WDT,WINDIS,NOWINDIS,WPRES32
//////// Fuses: WPRES128,VREGSLEEP_SW,NOVREGSLEEP,VBATBOR,NOVBATBOR,DSBOR
//////// Fuses: NODSBOR,NODS,DS_SW,DSWDT_1MS,DSWDT_2MS,DSWDT_4MS,DSWDT_8MS
//////// Fuses: DSWDT_16MS,DSWDT_33MS,DSWDT_66MS,DSWDT_132MS,DSWDT_264MS
//////// Fuses: DSWDT_528MS,DSWDT_1SEC,DSWDT_2SEC,DSWDT_4SEC,DSWDT_8SEC
//////// Fuses: DSWDT_16SEC,DSWDT_33SEC,DSWDT_67SEC,DSWDT_135SEC,DSWDT_4MINS
//////// Fuses: DSWDT_9MINS,DSWDT_18MINS,DSWDT_36MINS,DSWDT_72MINS
//////// Fuses: DSWDT_2HRS,DSWDT_4HRS,DSWDT_9HRS,DSWDT_19HRS,DSWDT_38HRS
//////// Fuses: DSWDT_77HRS,DSWDT_6DAYS,DSWDT_12DAYS,DSWDT_25DAYS,DSWDT
//////// Fuses: NODSWDT,DSWDTOSC_SOSC,DSWDTOSC_INT
//////// 
#if (!defined(__PCH__)||defined(__ISNT_CCS__))
#define _bif
#define int8 char
#define int16 long
#define int32 long long
#define float32 float
#define int1 char

#endif
////////////////////////////////////////////////////////////////// PIN_SELECT
// #pin_select function=pin
// Valid Pins: 
//    PIN_A0,PIN_A1,PIN_A2,PIN_A3,PIN_A4,PIN_A5,PIN_A6,PIN_B3,PIN_B0,PIN_B1,
//    PIN_A7,PIN_C2,PIN_B4,PIN_B5,PIN_B2,PIN_C3,PIN_C5,PIN_C4,PIN_C6,PIN_C7,
//    PIN_D0,PIN_D1,PIN_D2,PIN_D3,PIN_D4,PIN_D5,PIN_D6,PIN_D7,PIN_E0,PIN_E1,
//    PIN_E2,PIN_E7,PIN_E4,PIN_E3,PIN_E6,PIN_F5,PIN_F2,PIN_E5,PIN_F7,PIN_G1,
//    PIN_F6,PIN_F3,PIN_G2,PIN_G3,PIN_G4,PIN_F4,PIN_G0
// Input Functions: 
//    U1RX,U1CK,U2RX,U2CK,U3RX,U3CK,U4RX,U4CK,SCK1IN,SDI1,SS1IN,SCK2IN,SDI2,
//    SS2IN,FLT0,CCP1,CCP2,CCP3,IOC0,IOC1,IOC2,IOC3,IOC4,IOC5,IOC6,IOC7,INT1,
//    INT2,INT3,MDMIN,MDCIN1,MDCIN2,CCP4,CCP5,CCP6,CCP7,CCP8,CCP9,CCP10,T0CK,T1G,
//    T1CK,T3G,T3CK,T5G,T5CK,PBIO0IN,PBIO1IN,PBIO2IN,PBIO3IN,PBIO4IN,PBIO5IN,
//    PBIO6IN,PBIO7IN,RX1,RX2,RX3,RX4,TOCKI,T1CKI,T3CKI,T5CKI
// Output Functions: 
//    NULL,U2BCLK,U3DT,U4DT,SDO2,P1D,P2D,P3B,CTPLS,P5A,P8A,C1OUT,PBIO0OUT,
//    PBIO4OUT,U1BCLK,U3TX,U4TX,SDO1,P1C,P2C,P3C,P7A,P9A,C2OUT,PBIO1OUT,PBIO5OUT,
//    U1TX,U2DT,U3BCLK,U4BCLK,SCK2OUT,P1B,P2B,P3A,P6A,P10A,PBIO2OUT,PBIO6OUT,
//    U1DT,U2TX,SCK1OUT,P1A,P2A,P3D,MDOUT,P4A,C3OUT,PBIO3OUT,PBIO7OUT,TX1,TX2,
//    TX3,TX4,SCK1,SCK2,CCP4OUT,CCP5OUT,CCP6OUT,CCP7OUT,CCP8OUT,CCP9OUT,CCP10OUT
//

////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_x_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT(),
// Discrete I/O Prototypes:
_bif void set_tris_a(int8 value);  
_bif void set_tris_b(int8 value);  
_bif void set_tris_c(int8 value);  
_bif void set_tris_d(int8 value);  
_bif void set_tris_e(int8 value);  
_bif void set_tris_f(int8 value);  
_bif void set_tris_g(int8 value);  
_bif int8 get_tris_a(void);  
_bif int8 get_tris_b(void);  
_bif int8 get_tris_c(void);  
_bif int8 get_tris_d(void);  
_bif int8 get_tris_e(void);  
_bif int8 get_tris_f(void);  
_bif int8 get_tris_g(void);  
_bif void output_a(int8 value);  
_bif void output_b(int8 value);  
_bif void output_c(int8 value);  
_bif void output_d(int8 value);  
_bif void output_e(int8 value);  
_bif void output_f(int8 value);  
_bif void output_g(int8 value);  
_bif int8 input_a(void);  
_bif int8 input_b(void);  
_bif int8 input_c(void);  
_bif int8 input_d(void);  
_bif int8 input_e(void);  
_bif int8 input_f(void);  
_bif int8 input_g(void);  
_bif int8 input_change_a(void);  
_bif int8 input_change_b(void);  
_bif int8 input_change_c(void);  
_bif int8 input_change_d(void);  
_bif int8 input_change_e(void);  
_bif int8 input_change_f(void);  
_bif int8 input_change_g(void);  
_bif void port_a_pullups(int1 value);     
_bif void port_b_pullups(int8 upmask);  
_bif void port_c_pullups(int1 value);   
_bif void port_d_pullups(int1 value);   
_bif void port_e_pullups(int1 value);   
_bif void port_f_pullups(int1 value);   
_bif void port_g_pullups(int1 value);   
_bif void port_j_pullups(int1 value);   
_bif void port_k_pullups(int1 value);   
_bif void port_l_pullups(int1 value);   
_bif int1 input(int16 pin); 
_bif int1 input_state(int16 pin); 
_bif void output_low(int16 pin); 
_bif void output_high(int16 pin); 
_bif void output_toggle(int16 pin); 
_bif void output_bit(int16 pin, int1 level); 
_bif void output_float(int16 pin); 
_bif void output_drive(int16 pin); 

// Constants used to identify pins in the above are:

#define PIN_A0  31744
#define PIN_A1  31745
#define PIN_A2  31746
#define PIN_A3  31747
#define PIN_A4  31748
#define PIN_A5  31749
#define PIN_A6  31750
#define PIN_A7  31751

#define PIN_B0  31752
#define PIN_B1  31753
#define PIN_B2  31754
#define PIN_B3  31755
#define PIN_B4  31756
#define PIN_B5  31757
#define PIN_B6  31758
#define PIN_B7  31759

#define PIN_C0  31760
#define PIN_C1  31761
#define PIN_C2  31762
#define PIN_C3  31763
#define PIN_C4  31764
#define PIN_C5  31765
#define PIN_C6  31766
#define PIN_C7  31767

#define PIN_D0  31768
#define PIN_D1  31769
#define PIN_D2  31770
#define PIN_D3  31771
#define PIN_D4  31772
#define PIN_D5  31773
#define PIN_D6  31774
#define PIN_D7  31775

#define PIN_E0  31776
#define PIN_E1  31777
#define PIN_E2  31778
#define PIN_E3  31779
#define PIN_E4  31780
#define PIN_E5  31781
#define PIN_E6  31782
#define PIN_E7  31783

#define PIN_F2  31786
#define PIN_F3  31787
#define PIN_F4  31788
#define PIN_F5  31789
#define PIN_F6  31790
#define PIN_F7  31791

#define PIN_G0  31792
#define PIN_G1  31793
#define PIN_G2  31794
#define PIN_G3  31795
#define PIN_G4  31796

////////////////////////////////////////////////////////////////// Useful defines
#define FALSE 0
#define TRUE 1

#define BYTE int8
#define BOOLEAN int1

#define getc getch
#define fgetc getch
#define getchar getch
#define putc putchar
#define fputc putchar
#define fgets gets
#define fputs puts

////////////////////////////////////////////////////////////////// Control
// Control Functions:  RESET_CPU(), SLEEP(), RESTART_CAUSE()
// Prototypes:
_bif int16 restart_cause(void); 
_bif void reset_cpu(void);
_bif void sleep(void);
_bif void sleep(int8 type); 
_bif void release_io(void); 
// Constants returned from RESTART_CAUSE() are:
#define WDT_TIMEOUT       7      
#define MCLR_FROM_SLEEP  11      
#define MCLR_FROM_RUN    15      
#define NORMAL_POWER_UP  12      
#define BROWNOUT_RESTART 14      
#define WDT_FROM_SLEEP    3      
#define RESET_INSTRUCTION 0      

#define POR_FROM_DS      0x0100
#define MCLR_FROM_DS     0x0400
#define RTC_FROM_DS      0x0800
#define DSWDT_FROM_DS    0x1000
#define ULPWU_FROM_DS    0x2000
#define FAULT_FROM_DS    0x8000
#define EXT_FROM_DS      0x0200

//Constants for calls to sleep() to enable deep sleep mode are:
#define DEEP_SLEEP       3
// Any of the following may be OR'ed in with the above using |
#define WAKE_FROM_RTCC   4  // Wake-up from RTCC is enabled
#define WAKE_FROM_ULP    8  // ULPWU module is enabled in Deep Sleep

#byte DSGPR0 = getenv("SFR:DSGPR0")
#byte DSGPR1 = getenv("SFR:DSGPR1")
#byte DSGPR2 = getenv("SFR:DSGPR2") 
#byte DSGPR3 = getenv("SFR:DSGPR3") 

// Constants for calls to sleep() are:
#define REG_LOW_POWER            2
#define REG_NORMAL               0  // default

// Constants for calls to sleep() are:
#define SLEEP_FULL       0  // Default
#define SLEEP_IDLE       1  // Clock and peripherals don't stop

////////////////////////////////////////////////////////////////// Timer 0
// Timer 0 (AKA RTCC)Functions: SETUP_COUNTERS() or SETUP_TIMER_0(),
//                              SET_TIMER0() or SET_RTCC(),
//                              GET_TIMER0() or GET_RTCC()
// Timer 0 Prototypes:
_bif void setup_timer_0(int8 mode);
_bif void set_timer0(int16 value);     
_bif int16 get_timer0(void);           
_bif void setup_counters(int8 mode, int8 prescaler);
_bif void set_rtcc(int16 value);       
_bif int16 get_rtcc(void);             
// Constants used for SETUP_TIMER_0() are:
#define T0_INTOSC     0   
#define T0_INTERNAL   16  
#define T0_EXT_L_TO_H 32
#define T0_EXT_H_TO_L 48

#define T0_DIV_1      8
#define T0_DIV_2      0
#define T0_DIV_4      1
#define T0_DIV_8      2
#define T0_DIV_16     3
#define T0_DIV_32     4
#define T0_DIV_64     5
#define T0_DIV_128    6
#define T0_DIV_256    7

#define T0_OFF        0x80  

#define T0_8_BIT      0x40  

// The following are provided for compatibility
// with older compiler versions
#define RTCC_INTERNAL   16 
#define RTCC_EXT_L_TO_H 32     
#define RTCC_EXT_H_TO_L 48
#define RTCC_DIV_1      8
#define RTCC_DIV_2      0
#define RTCC_DIV_4      1
#define RTCC_DIV_8      2
#define RTCC_DIV_16     3
#define RTCC_DIV_32     4
#define RTCC_DIV_64     5
#define RTCC_DIV_128    6
#define RTCC_DIV_256    7
#define RTCC_OFF        0x80  
#define RTCC_8_BIT      0x40  

// Constants used for SETUP_COUNTERS() are the above
// constants for the 1st parameter and the following for
// the 2nd parameter:

////////////////////////////////////////////////////////////////// WDT
// Watch Dog Timer Functions: SETUP_WDT() or SETUP_COUNTERS() (see above)
//                            RESTART_WDT()
// WDT base is 4ms
// WDT Prototypes:
_bif void setup_wdt(int16 mode);
_bif void restart_wdt(void);
// Constants used for SETUP_WDT() are:
#define WDT_ON        0x100
#define WDT_OFF       0

#define WDT_4MS    0x300  
#define WDT_8MS    0x500  
#define WDT_16MS   0x700  
#define WDT_32MS   0x900  
#define WDT_64MS   0xB00  
#define WDT_128MS  0xD00  
#define WDT_256MS  0xF00  
#define WDT_512MS  0x1100  
#define WDT_1S     0x1300  
#define WDT_2S     0x1500  
#define WDT_4S     0x1700  
#define WDT_8S     0x1900  
#define WDT_16S    0x1B00  
#define WDT_32S    0x1D00  
#define WDT_64S    0x1F00  
#define WDT_128S   0x2100

////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Timer 1 Prototypes:
_bif void setup_timer_1(int16 mode); 
_bif int16 get_timer1(void);
_bif void set_timer1(int16 value);
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_INTERNAL         0x07      
#define T1_EXTERNAL         0x87      
#define T1_EXTERNAL_SYNC    0x83      
#define T1_FOSC             0x47      

#define T1_ENABLE_SOSC      0x08      

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

#define T1_GATE             0x8000
#define T1_GATE_INVERTED    0xC000
#define T1_GATE_TOGGLE      0xA000
#define T1_GATE_SINGLE      0x9000
#define T1_GATE_TIMER2      0x8100         
#define T1_GATE_COMP1       0x8200      
#define T1_GATE_COMP2       0x8300       

////////////////////////////////////////////////////////////////// Timer 2
// Timer 2 Functions: SETUP_TIMER_2, GET_TIMER2, SET_TIMER2
// Timer 2 Prototypes:
_bif void setup_timer_2(int8 mode, int8 period, int8 postscale); 
_bif int8 get_timer2(void);
_bif void set_timer2(int8 value);
// Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED         0
#define T2_DIV_BY_1         4
#define T2_DIV_BY_4         5
#define T2_DIV_BY_16        6 

////////////////////////////////////////////////////////////////// Timer 3
// Timer 3 Functions: SETUP_TIMER_3, GET_TIMER3, SET_TIMER3
// Timer 3 Prototypes:
_bif void setup_timer_3(int16 mode); 
_bif int16 get_timer3(void);
_bif void set_timer3(int16 value);
// Constants used for SETUP_TIMER_3() are:
//      (or (via |) together constants from each group)
#define T3_DISABLED         0
#define T3_INTERNAL         0x07  
#define T3_EXTERNAL         0x87   
#define T3_EXTERNAL_SYNC    0x83   
#define T3_FOSC             0x47  

#define T3_ENABLE_SOSC      0x08   

#define T3_DIV_BY_1         0
#define T3_DIV_BY_2         0x10
#define T3_DIV_BY_4         0x20
#define T3_DIV_BY_8         0x30

#define T3_GATE             0x8000
#define T3_GATE_INVERTED    0xC000
#define T3_GATE_TOGGLE      0xA000
#define T3_GATE_SINGLE      0x9000
#define T3_GATE_TIMER4      0x8100    
#define T3_GATE_COMP1       0x8200  
#define T3_GATE_COMP2       0x8300  

////////////////////////////////////////////////////////////////// Timer 4
// Timer 4 Functions: SETUP_TIMER_4, GET_TIMER4, SET_TIMER4
// Timer 4 Prototypes:
_bif void setup_timer_4(int16 mode,int8 period, int8 postscaler);
_bif int8 get_timer4(void);
_bif void set_timer4(int8 value);
// Constants used for SETUP_TIMER_4() are:
#define T4_DISABLED         0
#define T4_DIV_BY_1         4
#define T4_DIV_BY_4         5
#define T4_DIV_BY_16        6

////////////////////////////////////////////////////////////////// Timer 5
// Timer 5 Functions: SETUP_TIMER_5, GET_TIMER5, SET_TIMER5
// Timer 5 Prototypes:
_bif void setup_timer_5(int16 mode); 
_bif int16 get_timer5(void);
_bif void set_timer5(int16 value);
// Constants used for SETUP_TIMER_5() are:
//      (or (via |) together constants from each group)
#define T5_DISABLED         0
#define T5_INTERNAL         0x07  
#define T5_EXTERNAL         0x87   
#define T5_EXTERNAL_SYNC    0x83   
#define T5_FOSC             0x47  
#define T5_LFINTRC          0xC7  

#define T5_ENABLE_SOSC      0x08   

#define T5_DIV_BY_1         0
#define T5_DIV_BY_2         0x10
#define T5_DIV_BY_4         0x20
#define T5_DIV_BY_8         0x30

#define T5_GATE             0x8000
#define T5_GATE_INVERTED    0xC000
#define T5_GATE_TOGGLE      0xA000
#define T5_GATE_SINGLE      0x9000
#define T5_GATE_TIMER6      0x8100    
#define T5_GATE_COMP1       0x8200  
#define T5_GATE_COMP2       0x8300  

////////////////////////////////////////////////////////////////// Timer 6
// Timer 6 Functions: SETUP_TIMER_6, GET_TIMER5, SET_TIMER5
// Timer 6 Prototypes:
_bif void setup_timer_6(int16 mode, int8 period, int8 postscaler);
_bif int8 get_timer6(void);
_bif void set_timer6(int8 value);
// Constants used for SETUP_TIMER_6() are:
//      (or (via |) together constants from each group)
#define T6_DISABLED         0
#define T6_DIV_BY_1         4
#define T6_DIV_BY_4         5
#define T6_DIV_BY_16        6

////////////////////////////////////////////////////////////////// Timer 8
// Timer 8 Functions: SETUP_TIMER_8, GET_TIMER8, SET_TIMER8
// Timer 8 Prototypes:
_bif void setup_timer_8(int8 mode, int8 period, int8 postscaler);
_bif int8 get_timer8(void);
_bif void set_timer8(int8 value);
// Constants used for SETUP_TIMER_8() are:
//      (or (via |) together constants from each group)
#define T8_DISABLED         0
#define T8_DIV_BY_1         4
#define T8_DIV_BY_4         5
#define T8_DIV_BY_16        6

////////////////////////////////////////////////////////////////// RTC
// RTC Functions: setup_rtc(), setup_rtc_alarm(), rtc_read(),
//                rtc_write(), rtc_alarm_read(), rtc_alarm_write()
// Structure used in read and write functions (pass pointer):
typedef struct {
   int8 tm_year;
   int8 tm_temp;  // Not used by built in functions, place holder only do not use
   int8 tm_mday;
   int8 tm_mon;
   int8 tm_hour;
   int8 tm_wday;
   int8 tm_sec;
   int8 tm_min;
   int8 tm_isdst; // Not used by built in functions
} rtc_time_t;
// RTC Protoyptes:
_bif void setup_rtc(unsigned int16 mode);
_bif void setup_rtc(unsigned int16 mode, unsigned int8 calibration);
_bif void setup_rtc_alarm(unsigned int16 mode, unsigned int16 mask, unsigned int8 repeat);
_bif void rtc_read(rtc_time_t *time);
_bif void rtc_write(rtc_time_t *time);
_bif void rtc_alarm_read(rtc_time_t *time);
_bif void rtc_alarm_write(rtc_time_t *time);
// Constants used in setup_rtc() are:  // Second parameter is calibration
#define RTC_ENABLE               0x0080
#define RTC_DISABLE              0
#define RTC_CLOCK_EXT_60HZ       0x0C00
#define RTC_CLOCK_EXT_50HZ       0x0800
#define RTC_CLOCK_INT            0x0400
#define RTC_CLOCK_SOSC           0
#define RTC_OUTPUT_PWC           0x0304
#define RTC_OUTPUT_CLOCK         0x0204
#define RTC_OUTPUT_SECONDS       0x0104
#define RTC_OUTPUT_ALARM         0x0004
// OR in any of the following:
#define RTC_PWC_ENABLE           0x8000
#define RTC_PWC_HIGH             0x4000
#define RTC_PWC_LOW              0
#define RTC_PWC_STAB_CLOCK_DIV2  0x2000
#define RTC_PWC_STAB_CLOCK_DIV1  0
#define RTC_PWC_SAMP_CLOCK_DIV2  0x1000
#define RTC_PWC_SAMP_CLOCK_DIV1  0

// Constants used in setup_rtc_alarm() first parameter are:  
#define RTC_ALARM_ENABLE         0x80
#define RTC_ALARM_DISABLE        0
#define RTC_CHIME_ENABLE         0x40
#define RTC_CHIME_DISABLE        0
// Constants used in setup_rtc_alarm() second parameter are:  // Third parameter is repeat#
#define RTC_ALARM_HALFSECOND     0x00
#define RTC_ALARM_SECOND         0x04
#define RTC_ALARM_10_SECONDS     0x08
#define RTC_ALARM_MINUTE         0x0C
#define RTC_ALARM_10_MINUTES     0x10
#define RTC_ALARM_HOUR           0x14
#define RTC_ALARM_DAY            0x18
#define RTC_ALARM_WEEK           0x1C
#define RTC_ALARM_MONTH          0x20
#define RTC_ALARM_YEAR           0x24

////////////////////////////////////////////////////////////////// CCP
// CCP Functions: SETUP_CCPx, SET_PWMx_DUTY 
// CCP Variables: CCP_x, CCP_x_LOW, CCP_x_HIGH
// CCP1 Prototypes:
_bif void setup_ccp1(int32 mode);
_bif void setup_ccp1(int32 mode, int8 pwm); 
_bif void set_pwm1_duty(int8 value); 
_bif void set_pwm1_duty(int16 value); 
// Constants used for SETUP_CCPx() are:
#define CCP_OFF                          0
#define CCP_CAPTURE_FE                   0x04
#define CCP_CAPTURE_RE                   0x05
#define CCP_CAPTURE_DIV_4                0x06
#define CCP_CAPTURE_DIV_16               0x07
#define CCP_COMPARE_SET_ON_MATCH         0x08
#define CCP_COMPARE_CLR_ON_MATCH         0x09
#define CCP_COMPARE_INT                  0x0A               
#define CCP_COMPARE_INT_AND_TOGGLE       0x02      
#define CCP_COMPARE_RESET_TIMER          0x0B              
#define CCP_PWM                          0x0C    
#define CCP_PWM_PLUS_1                   0x1c               
#define CCP_PWM_PLUS_2                   0x2c               
#define CCP_PWM_PLUS_3                   0x3c               
//#define CCP_USE_TIMER3                 0x100  OBSOLETE, SEE TIMER-3          

#word   CCP_1       =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_LOW   =                    getenv("SFR:CCPR1L") 
#byte   CCP_1_HIGH  =                    getenv("SFR:CCPR1H") 

// The following are used to select the Timer source for the CCP/ECCP
// The first timer is the timer used when in CAPTURE or COMPARE mode
// The second timer is the timer used when in PWM mode
#define CCP_USE_TIMER1_AND_TIMER2       0x0000 //used for CCP 1 2 3 4 5 6 7 8 9 10 
#define CCP_USE_TIMER3_AND_TIMER4       0x0100 //used for CCP 1 2 3 4 8 
#define CCP_USE_TIMER3_AND_TIMER6       0x0200 //used for CCP 1 2 3 4 8 
#define CCP_USE_TIMER3_AND_TIMER8       0x0300 //used for CCP 1 2 3
#define CCP_USE_TIMER5_AND_TIMER4       0x0100 //used for CCP 5 7 9 
#define CCP_USE_TIMER5_AND_TIMER2       0x0100 //used for CCP 6 10 
#define CCP_USE_TIMER5_AND_TIMER6       0x0200 //used for CCP 7
#define CCP_USE_TIMER5_AND_TIMER8       0x0300 //used for CCP 7
#define CCP_USE_TIMER1_AND_TIMER6       0x0200 //used for CCP 8 

// The following should be used with the ECCP unit only (or these in)
#define CCP_PWM_H_H                     0x0c
#define CCP_PWM_H_L                     0x0d
#define CCP_PWM_L_H                     0x0e
#define CCP_PWM_L_L                     0x0f

#define CCP_PWM_FULL_BRIDGE             0x40
#define CCP_PWM_FULL_BRIDGE_REV         0xC0
#define CCP_PWM_HALF_BRIDGE             0x80

#define CCP_SHUTDOWN_ON_COMP1           0x100000
#define CCP_SHUTDOWN_ON_COMP2           0x200000
#define CCP_SHUTDOWN_ON_COMP            0x300000
#define CCP_SHUTDOWN_ON_INT0            0x400000
#define CCP_SHUTDOWN_ON_COMP1_INT0      0x500000
#define CCP_SHUTDOWN_ON_COMP2_INT0      0x600000
#define CCP_SHUTDOWN_ON_COMP_INT0       0x700000

#define CCP_SHUTDOWN_AC_L               0x000000
#define CCP_SHUTDOWN_AC_H               0x040000
#define CCP_SHUTDOWN_AC_F               0x080000

#define CCP_SHUTDOWN_BD_L               0x000000
#define CCP_SHUTDOWN_BD_H               0x010000
#define CCP_SHUTDOWN_BD_F               0x020000

#define CCP_SHUTDOWN_RESTART            0x80000000  

#define CCP_PULSE_STEERING_A            0x01000000
#define CCP_PULSE_STEERING_B            0x02000000
#define CCP_PULSE_STEERING_C            0x04000000
#define CCP_PULSE_STEERING_D            0x08000000
#define CCP_PULSE_STEERING_SYNC         0x10000000

// CCP2 Prototypes:
_bif void setup_ccp2(int32 mode);
_bif void setup_ccp2(int32 mode, int8 pwm); 
_bif void set_pwm2_duty(int8 value); 
_bif void set_pwm2_duty(int16 value); 
#word   CCP_2       =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_LOW   =                    getenv("SFR:CCPR2L") 
#byte   CCP_2_HIGH  =                    getenv("SFR:CCPR2H") 

// CCP3 Prototypes:
_bif void setup_ccp3(int32 mode);
_bif void setup_ccp3(int32 mode, int8 pwm);
_bif void set_pwm3_duty(int8 value);
_bif void set_pwm3_duty(int16 value);
#word   CCP_3       =                    getenv("SFR:CCPR3L")
#byte   CCP_3_LOW   =                    getenv("SFR:CCPR3L")
#byte   CCP_3_HIGH  =                    getenv("SFR:CCPR3H")

// CCP4 Prototypes:
_bif void setup_ccp4(int32 mode);
_bif void setup_ccp4(int32 mode, int8 pwm);
_bif void set_pwm4_duty(int8 value);
_bif void set_pwm4_duty(int16 value);
#word   CCP_4       =                    getenv("SFR:CCPR4L")
#byte   CCP_4_LOW   =                    getenv("SFR:CCPR4L")
#byte   CCP_4_HIGH  =                    getenv("SFR:CCPR4H")

// CCP5 Prototypes:
_bif void setup_ccp5(int32 mode);
_bif void setup_ccp5(int32 mode, int8 pwm);
_bif void set_pwm5_duty(int8 value);
_bif void set_pwm5_duty(int16 value);
#word   CCP_5       =                    getenv("SFR:CCPR5L")
#byte   CCP_5_LOW   =                    getenv("SFR:CCPR5L")
#byte   CCP_5_HIGH  =                    getenv("SFR:CCPR5H")

// CCP6 Prototypes:
_bif void setup_ccp6(int32 mode);
_bif void setup_ccp6(int32 mode, int8 pwm);
_bif void set_pwm6_duty(int8 value);
_bif void set_pwm6_duty(int16 value);
#word   CCP_6       =                    getenv("SFR:CCPR6L")
#byte   CCP_6_LOW   =                    getenv("SFR:CCPR6L")
#byte   CCP_6_HIGH  =                    getenv("SFR:CCPR6H")

// CCP7 Prototypes:
_bif void setup_ccp7(int32 mode);
_bif void setup_ccp7(int32 mode, int8 pwm);
_bif void set_pwm7_duty(int8 value);
_bif void set_pwm7_duty(int16 value);
#word   CCP_7       =                    getenv("SFR:CCPR7L")
#byte   CCP_7_LOW   =                    getenv("SFR:CCPR7L")
#byte   CCP_7_HIGH  =                    getenv("SFR:CCPR7H")

// CCP8 Prototypes:
_bif void setup_ccp8(int32 mode);
_bif void setup_ccp8(int32 mode, int8 pwm);
_bif void set_pwm8_duty(int8 value);
_bif void set_pwm8_duty(int16 value);
#word   CCP_8       =                    getenv("SFR:CCPR8L")
#byte   CCP_8_LOW   =                    getenv("SFR:CCPR8L")
#byte   CCP_8_HIGH  =                    getenv("SFR:CCPR8H")

// CCP9 Prototypes:
_bif void setup_ccp9(int32 mode);
_bif void setup_ccp9(int32 mode, int8 pwm);
_bif void set_pwm9_duty(int8 value);
_bif void set_pwm9_duty(int16 value);
#word   CCP_9       =                    getenv("SFR:CCPR9L")
#byte   CCP_9_LOW   =                    getenv("SFR:CCPR9L")
#byte   CCP_9_HIGH  =                    getenv("SFR:CCPR9H")

// CCP10 Prototypes:
_bif void setup_ccp10(int32 mode);
_bif void setup_ccp10(int32 mode, int8 pwm);
_bif void set_pwm10_duty(int8 value);
_bif void set_pwm10_duty(int16 value);
#word   CCP_10      =                    getenv("SFR:CCPR10L")
#byte   CCP_10_LOW  =                    getenv("SFR:CCPR10L")
#byte   CCP_10_HIGH =                    getenv("SFR:CCPR10H")

////////////////////////////////////////////////////////////////// PSP
// PSP Functions: SETUP_PSP, PSP_INPUT_FULL(), PSP_OUTPUT_FULL(),
//                PSP_OVERFLOW(), INPUT_D(), OUTPUT_D()
// PSP Variables: PSP_DATA
// PSP Prototypes:
_bif void setup_psp(int8 mode);
_bif int1 psp_output_full(void);
_bif int1 psp_input_full(void);
_bif int1 psp_overflow(void);
// Constants used in SETUP_PSP() are:
#define PSP_ENABLED    0x10
#define PSP_DISABLED   0

#byte  PSP_DATA = 0xF83 

////////////////////////////////////////////////////////////////// SPI
// SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IS_IN 
// SPI Prototypes:
_bif void setup_spi(int32 mode);
_bif void spi_write(int8 value);
_bif void spi_write(int1 wait, int8 value);
_bif int8 spi_read(void);
_bif int8 spi_read(int8 value);
_bif int1 spi_data_is_in(void);
_bif void setup_spi2(int32 mode);  
_bif void spi_write2(int8 value);  
_bif int8 spi_read2(void);  
_bif int8 spi_read2(int8 value);  
_bif int1 spi_data_is_in2(void);  
// Constants used in SETUP_SPI() are: 
#define SPI_DISABLED             0x00 
#define SPI_MASTER               0x20 
#define SPI_SLAVE                0x24 
#define SPI_SCK_IDLE_HIGH        0x10 
#define SPI_SCK_IDLE_LOW         0x00 
#define SPI_CLK_DIV_4            0x00 
#define SPI_CLK_DIV_16           0x01 
#define SPI_CLK_DIV_64           0x02 
#define SPI_CLK_T2               0x03 
#define SPI_SS_DISABLED          0x01 

#define SPI_XMIT_L_TO_H          0x4000
#define SPI_XMIT_H_TO_L          0x0000

#define SPI_SAMPLE_AT_MIDDLE     0x0000
#define SPI_SAMPLE_AT_END        0x8000

//The following are provided for compatibility
#define SPI_L_TO_H       SPI_SCK_IDLE_LOW
#define SPI_H_TO_L       SPI_SCK_IDLE_HIGH

////////////////////////////////////////////////////////////////// UART
// UART Prototypes:
_bif void setup_uart(int32 baud);
_bif void setup_uart(int32 baud, int8 stream);
_bif void setup_uart(int32 baud, int8 stream, int32 clock);
_bif void set_uart_speed(int32 baud);
_bif void set_uart_speed(int32 baud, int8 stream);
_bif void set_uart_speed(int32 baud, int8 stream, int32 clock);
// Constants used in setup_uart() are:
// FALSE - Turn UART off
// TRUE  - Turn UART on
#define UART_ADDRESS           2
#define UART_DATA              4
#define UART_AUTODETECT        8
#define UART_AUTODETECT_NOWAIT 9
#define UART_WAKEUP_ON_RDA     10
#define UART_SEND_BREAK        13

////////////////////////////////////////////////////////////////// COMP
// Comparator Variables: C1OUT, C2OUT, C3OUT
// Comparator Prototypes:
_bif void setup_comparator(int32 mode);
// Constants used in setup_comparator() are:
//
#define NC_NC           0x00

//Pick one constant for COMP1
#define CP1_F2_F6       0x00000080
#define CP1_F5_F6       0x00000082
#define CP1_VBG_F6      0x00000083
#define CP1_F2_A5       0x01000080
#define CP1_F5_A5       0x01000082
#define CP1_VBG_A5      0x01000083
#define CP1_F2_VREF     0x00000084
#define CP1_F5_VREF     0x00000086
#define CP1_VBG_VREF    0x00000087
//Optionally OR any of the following with the above
#define CP1_INT_L2H     0x00000008
#define CP1_INT_H2L     0x00000010
#define CP1_INVERT      0x00000020
#define CP1_OUTPUT      0x00000040

//Pick one constant for COMP2
#define CP2_F2_A5       0x00008200
#define CP2_VBG_A5      0x00008300
#define CP2_F2_VREF     0x00008600
#define CP2_VBG_VREF    0x00008700
//Optionally OR any of the following with the above
#define CP2_INT_L2H     0x00000800
#define CP2_INT_H2L     0x00001000
#define CP2_INVERT      0x00002000
#define CP2_OUTPUT      0x00004000

//Pick one constant for COMP3
#define CP3_F2_G2       0x00800000
#define CP3_G4_G2       0x00810000
#define CP3_G3_G2       0x00820000
#define CP3_VBG_G2      0x00830000
#define CP3_F2_A5       0x04800000
#define CP3_G4_A5       0x04810000
#define CP3_G3_A5       0x04820000
#define CP3_VBG_A5      0x04830000
#define CP3_F2_VREF     0x00840000
#define CP3_G4_VREF     0x00850000
#define CP3_G3_VREF     0x00860000
#define CP3_VBG_VREF    0x00870000
//Optionally OR any of the following with the above
#define CP3_INT_L2H     0x00080000
#define CP3_INT_H2L     0x00100000
#define CP3_INVERT      0x00200000
#define CP3_OUTPUT      0x00400000

#bit C1OUT = getenv("SFR:CMSTAT").0
#bit C2OUT = getenv("SFR:CMSTAT").1
#bit C3OUT = getenv("SFR:CMSTAT").2

////////////////////////////////////////////////////////////////// VREF
// VREF Prototypes:
_bif void setup_vref(int16 mode);
// Constants used in setup_vref() are:
#define VREF_OFF       0
#define VREF_ON        0x8000
#define VREF_OUT_F5    0x4000
#define VREF_VSS_VDD   0       //CVref Positive Source Vdd, Negative Source Vss
#define VREF_VSS_VREF  0x1000  //CVref Positive Source Vref+, Negative Source Vss
#define VREF_VSS_VBG   0x2000  //CVref Positive Source VBG, Negative Source Vss
#define VREF_VREF_VDD  0x0100  //CVref Positive Source Vdd, Negative Source Vref-
#define VREF_VREF_VREF 0x1100  //CVref Positive Source Vref+, Negative Source Vref-
#define VREF_VREF_VBG  0x2100  //CVref Positive Source VBG, Negative Source Vref-
// OR (with |) the above with a number 0-31

////////////////////////////////////////////////////////////////// LVD
// LVD Prototypes:
_bif void setup_low_volt_detect(int8 mode);
// Constants used in setup_low_volt_detect() are:
#define LVD_LVDIN           0x1F
#define LVD_34              0x1E
#define LVD_33              0x1D
#define LVD_30              0x1C
#define LVD_28              0x1B
#define LVD_27              0x1A
#define LVD_25              0x19
#define LVD_24              0x18
#define LVD_23              0x17
#define LVD_22              0x16
#define LVD_21              0x15
#define LVD_20              0x14

#define LVD_TRIGGER_BELOW   0
#define LVD_TRIGGER_ABOVE   0x80

////////////////////////////////////////////////////////////////// DSM
// Data Signal Modulator Functions: SETUP_DSM()  
// DSM Prototypes:
_bif void setup_dsm(int1 enable);
_bif void setup_dsm(unsigned int8 mode, unsigned int8 source);      
_bif void setup_dsm(unsigned int8 mode, unsigned int8 source, unsigned int16 carrier);      
// Constants used in SETUP_DSM() first parameter are:  
#define DSM_ENABLED                     0x80
#define DSM_DISABLED                    0x00
// Any of the following may be or'ed in with the above using |
#define DSM_OUTPUT_ENABLED              0x40       
#define DSM_SLEW_RATE_LIMITING_ENABLED  0x20       
#define DSM_OUTPUT_INVERTED             0x10
//Constants used as the second parameter are:
#define DSM_SOURCE_MANUAL               0x00       
#define DSM_SOURCE_MDMIN                0x01       
#define DSM_SOURCE_SPI1                 0x02         
#define DSM_SOURCE_SPI2                 0x03         
#define DSM_SOURCE_UART1                0x04         
#define DSM_SOURCE_UART2                0x05         
#define DSM_SOURCE_UART3                0x06         
#define DSM_SOURCE_UART4                0x07         
#define DSM_SOURCE_CCP1                 0x08         
#define DSM_SOURCE_CCP2                 0x09         
#define DSM_SOURCE_CCP3                 0x0A         
#define DSM_SOURCE_CCP4                 0x0B         
#define DSM_SOURCE_CCP5                 0x0C         
#define DSM_SOURCE_CCP6                 0x0D         
#define DSM_SOURCE_CCP7                 0x0E         
#define DSM_SOURCE_CCP8                 0x0F         
#define DSM_DISABLE_SOURCE              0x80        
//Constants used as the third parameter are:
#define DSM_CARRIER_LOW_VSS             0x0000      
#define DSM_CARRIER_LOW_MDCIN1          0x0001      
#define DSM_CARRIER_LOW_MDCIN2          0x0002        
#define DSM_CARRIER_LOW_REFO1           0x0003        
#define DSM_CARRIER_LOW_CCP1            0x0004   
#define DSM_CARRIER_LOW_CCP2            0x0005     
#define DSM_CARRIER_LOW_CCP3            0x0006      
#define DSM_CARRIER_LOW_CCP4            0x0007      
#define DSM_CARRIER_LOW_CCP5            0x0008       
#define DSM_CARRIER_LOW_CCP6            0x0009        
#define DSM_CARRIER_LOW_CCP7            0x000A        
#define DSM_CARRIER_LOW_CCP8            0x000B        
#define DSM_CARRIER_LOW_CCP9            0x000C        
#define DSM_CARRIER_LOW_CCP10           0x000D        
#define DSM_CARRIER_LOW_SYSTEM_CLOCK    0x000E        
#define DSM_CARRIER_LOW_REFO2           0x000F        
#define DSM_SYNC_CARRIER_LOW            0x0020      
#define DSM_INVERT_CARRIER_LOW          0x0040      
#define DSM_DISABLE_CARRIER_LOW         0x0080       
#define DSM_CARRIER_HIGH_VSS            0x0000      
#define DSM_CARRIER_HIGH_MDCIN1         0x0100     
#define DSM_CARRIER_HIGH_MDCIN2         0x0200     
#define DSM_CARRIER_HIGH_CCP1           0x0400   
#define DSM_CARRIER_HIGH_CCP2           0x0500     
#define DSM_CARRIER_HIGH_CCP3           0x0600      
#define DSM_CARRIER_HIGH_CCP4           0x0700       
#define DSM_CARRIER_HIGH_CCP5           0x0800       
#define DSM_CARRIER_HIGH_CCP6           0x0900        
#define DSM_CARRIER_HIGH_CCP7           0x0A00        
#define DSM_CARRIER_HIGH_CCP8           0x0B00        
#define DSM_CARRIER_HIGH_CCP9           0x0C00        
#define DSM_CARRIER_HIGH_CCP10          0x0D00        
#define DSM_CARRIER_HIGH_SYSTEM_CLOCK   0x0E00        
#define DSM_CARRIER_HIGH_REFO2          0x0F00        
#define DSM_SYNC_CARRIER_HIGH           0x2000      
#define DSM_INVERT_CARRIER_HIGH         0x4000      
#define DSM_DISABLE_CARRIER_HIGH        0x8000       

#bit MDBIT = getenv("SFR:MDCON").0      
#bit MDOUT = getenv("SFR:MDCON").3      

////////////////////////////////////////////////////////////////// LCD
// LCD Functions: SETUP_LCD, LCD_SYMBOL, LCD_LOAD, LCD_CONTRAST,
//                SETUP_LCD_REGULATOR
// LCD Prototypes:
_bif void setup_lcd(int32 mode, int8 prescale, int32 segments0_31, int32 segments32_63);
_bif void setup_lcd_regulator(int8 mode);
_bif void lcd_load(int8* buffer_pointer, int8 offset, int8 length);
_bif void lcd_symbol(int16 symbol, int8 b1_addr, ...);
_bif void lcd_contrast(int8 contrast);
// Constants used in setup_lcd() are:

// LCD Multiplex Mode (first parameter)
#define LCD_STATIC         0x00000080
#define LCD_MUX12          0x00000081
#define LCD_MUX13          0x00000082
#define LCD_MUX14          0x00000083
#define LCD_MUX15          0x00000084
#define LCD_MUX16          0x00000085
#define LCD_MUX17          0x00000086
#define LCD_MUX18          0x00000087
#define LCD_DISABLED       0x00000000

//      Any of the following may be or'ed in with the first parameter
// LCD Sleep Mode
#define LCD_HALT_IDLE      0x00000040
#define LCD_CONTINUE_IDLE  0x00000000   //default

// LCD Clock Source
#define LCD_FRC            0x00000000   //default
#define LCD_SOSC           0x00000008
#define LCD_LFINTOSC       0x00000010

// LCD Waveform Type
#define LCD_TYPE_B         0x00008000
#define LCD_TYPE_A         0x00000000  //default

// LCD Bias Mode (MUX12 & MUX13 Only)
#define LCD_BIAS12         0x00004000
#define LCD_BIAS13         0x00000000  //default

// LCD Internal Reference Enable
#define LCD_REF_ENABLED    0x00800000
#define LCD_REF_DISABLED   0x00000000  //default

// LCD Voltage Pin Enable
#define LCD_VOLTAGE3       0x00040000
#define LCD_VOLTAGE2       0x00020000
#define LCD_VOLTAGE1       0x00010000
#define LCD_NO_VOLTAGE     0x00000000 //default

// LCD Ladder A Controls
#define LCD_A_NO_POWER     0x00000000 //default
#define LCD_A_LOW_POWER    0x40000000
#define LCD_A_MED_POWER    0x80000000
#define LCD_A_HIGH_POWER   0xC0000000

// LCD Ladder B Controls
#define LCD_B_NO_POWER     0x00000000 //default
#define LCD_B_LOW_POWER    0x10000000
#define LCD_B_MED_POWER    0x20000000
#define LCD_B_HIGH_POWER   0x30000000

// LCD Ladder A Time Interval
#define LCD_B_ONLY         0x00000000  //default
#define LCD_A_CLOCK1       0x01000000
#define LCD_A_CLOCK2       0x02000000
#define LCD_A_CLOCK3       0x03000000
#define LCD_A_CLOCK4       0x04000000
#define LCD_A_CLOCK5       0x05000000
#define LCD_A_CLOCK6       0x06000000
#define LCD_A_CLOCK7       0x07000000

// LCD Regulator Clock
#define LCD_REG_DISABLED   0x00000000
#define LCD_REG_CLK_SOSC   0x00000100
#define LCD_REG_CLK_FRC    0x00000200
#define LCD_REG_CLK_LPRC   0x00000300

// LCD Regulator Output Bias Support
#define LCD_OUTPUT_STATIC  0x00000000
#define LCD_OUTPUT_BIAS13  0x00000400

// LCD Regulator Bias Voltage
#define LCD_BIAS_2v69      0x00000000
#define LCD_BIAS_2v82      0x00000800
#define LCD_BIAS_2v95      0x00001000
#define LCD_BIAS_3v08      0x00001800
#define LCD_BIAS_3v21      0x00002000
#define LCD_BIAS_3v34      0x00002800
#define LCD_BIAS_3v47      0x00003000
#define LCD_BIAS_3v60      0x00003800

// LCD Regulator Charge Pump
#define LCD_CHARGE_PUMP    0x00080000

// Constants used in lcd_symbol() are:
#define COM0 0
#define COM1 64
#define COM2 128
#define COM3 192
#define COM4 256
#define COM5 320
#define COM6 384
#define COM7 448


////////////////////////////////////////////////////////////////// INTERNAL RC
// Oscillator Prototypes
_bif int8 setup_oscillator(int32 mode);
_bif int8 setup_oscillator(int32 mode, signed int8 tune);
_bif void setup_act(int8 mode);
_bif int8 setup_act(void);
_bif int8 setup_act(int8 mode);
_bif int8 act_status(void);
// Constants used in setup_oscillator() are:
// First parameter:
#define OSC_31000         0x00000005
#define OSC_31250         0x00070000
#define OSC_125KHZ        0x00060000
#define OSC_250KHZ        0x00050000
#define OSC_500KHZ        0x00040000
#define OSC_1MHZ          0x00030000
#define OSC_2MHZ          0x00020000
#define OSC_4MHZ          0x00010000
#define OSC_8MHZ          0x00000000
// The following may be OR'ed in with the above using |
#define OSC_INTRC         0x00000007  //FRC Oscillator is System Clock
#define OSC_LPRC          0x00000005  //LPRC Oscillator is System Clock
#define OSC_SOSC          0x00000004  //Secondary Oscillator is System Clock
#define OSC_PRIMARY_PLL   0x00000003  //Primary Oscillator (MS, HS, EC) with PLL is System Clock
#define OSC_PRIMARY       0x00000002  //Primary Oscillator (MS, HS, EC) is System Clock
#define OSC_INTRC_PLL     0x00000001  //FRC Oscillator with PLL is System Clock
// The following may be OR'ed in with the above using |
#define OSC_SOSC_ENABLED  0x00000200  //SOSC always enabled
#define OSC_PRIMARY_NOSL  0x00000400  //Primary Oscillator enabled during Sleep mode
#define OSC_PLL_ON        0x20000000  //PLL is always enabled
// The following may be OR'ed in with the above using | 
#define OSC_CPDIV_1       0x00000000  //Need when using OSC_PRIMARY_PLL or OSC_INTRC_PLL and 96MHz PLL Block (64MHz System Clock)
#define OSC_CPDIV_2       0x40000000  //Need when using OSC_PRIMARY_PLL or OSC_INTRC_PLL and 96MHz PLL Block (32MHz System Clock)
#define OSC_CPDIV_4       0x80000000  //Need when using OSC_PRIMARY_PLL or OSC_INTRC_PLL and 96MHz PLL Block (16MHz System Clock)
#define OSC_CPDIV_8       0xC0000000  //Need when using OSC_PRIMARY_PLL or OSC_INTRC_PLL and 96MHz PLL Block (8MHz System Clock)
// A second optional parameter may be used with this part to fine
// tune the speed (signed int,-32 to 31)
// Result may be (ignore all other bits)
#define OSC_PLL_READY     0x20
#define OSC_CLOCK_FAILED  0x08

// Constants used in setup_act() are:
#define ACT_DISABLED              0
#define ACT_ENABLED               0x80
#define ACT_HALT_IDLE             0x20
#define ACT_TUNED_TO_SOSC         0
#define ACT_TUNED_TO_USB          0x10
#define ACT_INT_WHEN_LOCKED       0
#define ACT_INT_WHEN_NOT_LOCKED   0x04
#define ACT_INT_WHEN_NOT_IN_RANGE 0
#define ACT_INT_WHEN_IN_RANGE     0x01
// Result for setup_act() and act_status() may be (ignore all other bits)
#define ACT_CLOCK_LOCKED          0x08  //internal oscillator is within ± 0.20%
#define ACT_OUT_OF_RANGE          0x02  //oscillator frequency is outside of the OSCTUNE range


////////////////////////////////////////////////////////////////// ADC
// ADC Functions: SETUP_ADC(), SETUP_ADC_PORTS() (aka SETUP_PORT_A),
//                SET_ADC_CHANNEL(), READ_ADC(), ADC_DONE()
// ADC Prototypes:
_bif void setup_adc(int32 mode);
_bif int8 read_adc(void);
_bif int8 read_adc(int8 mode);
_bif int16 read_adc(void);
_bif int16 read_adc(int8 mode);
_bif int1 adc_done(void);
//Constants used for SETUP_ADC() are:
#define ADC_OFF           0x10000
#define ADC_CLOCK          0x0000
#define ADC_CLOCK_DIV_2    0x0001
#define ADC_CLOCK_DIV_4    0x0003
#define ADC_CLOCK_DIV_8    0x0007
#define ADC_CLOCK_DIV_16   0x000F
#define ADC_CLOCK_DIV_32   0x001F
#define ADC_CLOCK_DIV_64   0x003F
#define ADC_CLOCK_INTERNAL 0x8000
// The following may be OR'ed in with the above using |
#define ADC_TAD_MUL_0      0x1F00
#define ADC_TAD_MUL_2      0x1D00
#define ADC_TAD_MUL_4      0x1B00
#define ADC_TAD_MUL_8      0x1700
#define ADC_TAD_MUL_16     0x0F00
#define ADC_TAD_MUL_31     0x0000
// The following may be OR'ed in with the above using |
#define ADC_SIGNED_RESULT 0x20000

//ADC Prototypes:
_bif void setup_adc_ports(int32 pins);
_bif void setup_adc_ports(int32 pins, int32 reference);
_bif void set_analog_pins(int32 pins);
_bif void set_analog_pins(int32 pins, int32 reference);
_bif void set_adc_channel(int8 channel);
// Constants used is SETUP_ADC_PORTS() and SET_ANALOG_PINS() are:
// First argument:
// OR together desired pins
#define sAN0            0x00000100  //| A0
#define sAN1            0x00000200  //| A1
#define sAN2            0x00000400  //| A2
#define sAN3            0x00000800  //| A3
#define sAN4            0x00002000  //| A5
#define sAN5            0x00080000  //| F7
#define sAN6            0x00001000  //| A4
#define sAN7            0x00004000  //| F2
#define sAN8            0x00008000  //| G0
#define sAN9            0x00010000  //| C2
#define sAN10           0x00020000  //| F5
#define sAN11           0x00040000  //| F6
#define sAN16           0x00800000  //| G4
#define sAN17           0x00400000  //| G3
#define sAN18           0x00200000  //| G2
#define sAN19           0x00100000  //| G1
#define NO_ANALOGS               0  // None
#define ALL_ANALOG      0X00FFFF00  // A0 A1 A2 A3 A5 F7 A4 F2 G0 C2 F5 F6 G4 G3 G2 G1  

//Optional Second argument:
#define VSS_VDD         0x00000000  //| Range 0 to Vdd
#define VSS_VREF        0x00000040  //| Range 0 to Vref+
#define VREF_VDD        0x00000020  //| Range Vref- to Vdd
#define VREF_VREF       0x00000060  //| Range Vref- to Vref+

// Constants used for first parameter of SET_ADC_CHANNEL() are:
// either use the channel number or one of the following
#define CTMU_TEMP_SENSOR          0x18
#define CTMU_CHANNEL              0x19
#define VBG_DIV_6_CHANNEL         0x1A
#define VBG_DIV_2_CHANNEL         0x1B
#define VBG_CHANNEL               0x1C
#define AVSS_CHANNEL              0x1D
#define AVDD_CHANNEL              0x1E
#define VBAT_DIV_2_CHANNEL        0x1F
// Constants used for second parameter of SET_ADC_CHANNEL() are:
#define ADC_NEG_REFERENCE         0x00  // Analog Negative Channel connected to ADC Negative Reference
#define ADC_NEG_AN1               0x40  // Analog Negative Channel connected to AN1- (sAN0)

// Constants used in READ_ADC() are:
#define ADC_START_AND_READ     7   // This is the default if nothing is specified
#define ADC_START_ONLY         1
#define ADC_READ_ONLY          6

////////////////////////////////////////////////////////////////// BIF
// Built In Functions Prototypes
//
// Math Prototypes:
_bif signed int8 abs(signed int8 x);
_bif signed int16 abs(signed int16 x);
_bif signed int32 abs(signed int32 x);
_bif float32 abs(float32 x);
_bif unsigned int16 _mul(unsigned int8, unsigned int8);
_bif signed int16 _mul(signed int8, signed int8);
_bif unsigned int32 _mul(unsigned int16, unsigned int16);
_bif signed int32 _mul(signed int16, signed int16);

// Memory Manipulation Prototypes:
_bif int8 read_bank(int8 bank, int8 offset);
_bif void write_bank(int8 bank, int8 offset, int8 value);
_bif void strcpy(char* dest, char* src);
_bif void memset(unsigned int8* destination, unsigned int8 value, unsigned int16 num);
_bif void memcpy(unsigned int8* destination, unsigned int8* source, unsigned int16 num);

// String Prototypes:
_bif char toupper(char cvalue);
_bif char tolower(char cvalue);
_bif void sprintf(char* string, char* cstring, ...);

// Data Manipulators Prototypes:
_bif int1 shift_left(unsigned int8* address, unsigned int8 bytes, int1 value);
_bif int1 shift_right(unsigned int8* address, unsigned int8 bytes, int1 value);
_bif void rotate_left(unsigned int8* address, unsigned int8 bytes);
_bif void rotate_right(unsigned int8* address, unsigned int8 bytes);
_bif void swap(unsigned int8 value);
_bif unsigned int8 make8(unsigned int16 var, unsigned int8 offset);
_bif unsigned int8 make8(unsigned int32 var, unsigned int8 offset);
_bif unsigned int16 make16(unsigned int8 varhigh, unsigned int8 varlow);
_bif unsigned int32 make32(unsigned int16 var1);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int16 var2);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int8 var2);
_bif unsigned int32 make32(unsigned int16 var1, unsigned int8 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int8 var3, unsigned int8 var4);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int16 var2);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int16 var2, unsigned int8 var3);
_bif unsigned int32 make32(unsigned int8 var1, unsigned int8 var2, unsigned int16 var3);
_bif void bit_set(unsigned int8 var, unsigned int8 bit);
_bif void bit_set(unsigned int16 var, unsigned int8 bit);
_bif void bit_set(unsigned int32 var, unsigned int8 bit);
_bif void bit_clear(unsigned int8 var, unsigned int8 bit);
_bif void bit_clear(unsigned int16 var, unsigned int8 bit);
_bif void bit_clear(unsigned int32 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int8 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int16 var, unsigned int8 bit);
_bif int1 bit_test(unsigned int32 var, unsigned int8 bit);

// #use delay() Prototypes:
_bif void delay_cycles(unsigned int8 count);
_bif void delay_ms(unsigned int16 time);
_bif void delay_us(unsigned int16 time);

// #use rs232() Prototypes:
_bif void putchar(char cdata);
_bif void putchar(char cdata, unsigned int8 stream);
_bif void puts(char* string);
_bif void puts(char* string, unsigned int8 stream);
_bif char getch(void);
_bif char getch(unsigned int8 stream);
_bif void gets(char* string);
_bif void gets(char* string, unsigned int8 stream);
_bif int1 kbhit(void);
_bif int1 kbhit(unsigned int8 stream);
_bif void printf(char* string, ...);
_bif void fprintf(unsigned int8 stream, char* string, ...);
_bif void putc_send(void);
_bif void fputc_send(unsigned int8 stream);
_bif int1 rcv_buffer_full(void);
_bif int1 rcv_buffer_full(unsigned int8 stream);
_bif unsigned int16 rcv_buffer_bytes(void);
_bif unsigned int16 rcv_buffer_bytes(unsigned int8 stream);
_bif int1 tx_buffer_full(void);
_bif int1 tx_buffer_full(unsigned int8 stream);
_bif unsigned int16 tx_buffer_bytes(void);
_bif unsigned int16 tx_buffer_bytes(unsigned int8 stream);
_bif unsigned int8 linbus_header(unsigned int8 identifier);
_bif unsigned int8 linbus_header(unsigned int8 stream, unsigned int8 identifier);
_bif int1 linbus_header_hit(void);
_bif int1 linbus_header_hit(unsigned int8 stream);
_bif unsigned int8 linbus_header_get(void);
_bif unsigned int8 linbus_header_get(unsigned int8 stream);
_bif unsigned int8 linbus_rx_response(unsigned int8 *DataPtr, unsigned int8 Count);
_bif unsigned int8 linbus_rx_response(unsigned int8 stream, unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_tx_response(unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_tx_response(unsigned int8 stream, unsigned int8 *DataPtr, unsigned int8 Count);
_bif void linbus_checksum_type(unsigned int8 type);
_bif void linbus_checksum_type(unsigned int8 stream, unsigned int8 type);

// #use i2c() Prototypes:
_bif unsigned int8 i2c_read(void);
_bif unsigned int8 i2c_read(unsigned int8 ack);
_bif unsigned int8 i2c_read(unsigned int8 stream, unsigned int8 ack);
_bif int1 i2c_write(unsigned int8 data);
_bif int1 i2c_write(unsigned int8 stream, unsigned int8 data);
_bif void i2c_start(void);
_bif void i2c_start(unsigned int8 stream);
_bif void i2c_start(unsigned int8 stream, unsigned int8 restart);
_bif void i2c_stop(void);
_bif void i2c_stop(unsigned int8 stream);
_bif int8 i2c_isr_state(void);
_bif void i2c_slaveaddr(unsigned int8 addr);
_bif void i2c_slaveaddr(unsigned int8 stream, unsigned int8 addr);
_bif int1 i2c_poll(void);
_bif int1 i2c_poll(unsigned int8 stream);
_bif void i2c_init(unsigned int32 baud);
_bif void i2c_init(unsigned int8 stream, unsigned int32 baud);
_bif int8 i2c_transfer(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount, unsigned int8 *rData, unsigned int8 *rCount);
_bif int8 i2c_transfer(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount, unsigned int8 *rData, unsigned int8 *rCount);
_bif int8 i2c_transfer_out(unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer_out(unsigned int8 stream, unsigned int8 Address, unsigned int8 *wData, unsigned int8 wCount);
_bif int8 i2c_transfer_in(unsigned int8 Address, unsigned int8 *rData, unsigned int8 rCount);
_bif int8 i2c_transfer_in(unsigned int8 stream, unsigned int8 Address, unsigned int8 *rData, unsigned int8 rCount);

// #use spi() Prototypes:
_bif unsigned int8 spi_xfer(void);
_bif unsigned int16 spi_xfer(void);
_bif unsigned int32 spi_xfer(void);
_bif unsigned int8 spi_xfer(unsigned int8 data);
_bif unsigned int16 spi_xfer(unsigned int16 data);
_bif unsigned int32 spi_xfer(unsigned int32 data);
_bif unsigned int8 spi_xfer(unsigned int8 stream, unsigned int8 data);
_bif unsigned int16 spi_xfer(unsigned int8 stream, unsigned int16 data);
_bif unsigned int32 spi_xfer(unsigned int8 stream, unsigned int32 data);
_bif unsigned int8 spi_xfer(unsigned int8 stream, unsigned int8 data, unsigned int8 bits);
_bif unsigned int16 spi_xfer(unsigned int8 stream, unsigned int16 data, unsigned int8 bits);
_bif unsigned int32 spi_xfer(unsigned int8 stream, unsigned int32 data, unsigned int8 bits);
_bif void spi_init(unsigned int32 baud);
_bif void spi_init(unsigned int8 stream, unsigned int32 baud);
_bif void spi_speed(unsigned int32 baud);
_bif void spi_speed(unsigned int8 stream, unsigned int32 baud);
_bif void spi_speed(unsigned int8 stream, unsigned int32 baud, unsigned int32 clock);
_bif void spi_prewrite(unsigned int8 data);
_bif void spi_prewrite(unsigned int16 data);
_bif void spi_prewrite(unsigned int32 data);
_bif void spi_prewrite(unsigned int8, unsigned int8 data);
_bif void spi_prewrite(unsigned int8, unsigned int16 data);
_bif void spi_prewrite(unsigned int8, unsigned int32 data);
_bif unsigned int8 spi_xfer_in(void);
_bif unsigned int16 spi_xfer_in(void);
_bif unsigned int32 spi_xfer_in(void);
_bif unsigned int8 spi_xfer_in(unsigned int8 bits);
_bif unsigned int16 spi_xfer_in(unsigned int8 bits);
_bif unsigned int32 spi_xfer_in(unsigned int8 bits);
_bif unsigned int8 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif unsigned int16 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif unsigned int32 spi_xfer_in(unsigned int8 stream, unsigned int8 bits);
_bif void spi_transfer(unsigned int8 *wData, unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer(unsigned int8 stream, unsigned int8 *wData, unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer_write(unsigned int8 *wData, unsigned int16 Count);
_bif void spi_transfer_write(unsigned int8 stream, unsigned int8 *wData, unsigned int16 Count);
_bif void spi_transfer_read(unsigned int8 *rData, unsigned int16 Count);
_bif void spi_transfer_read(unsigned int8 stream, unsigned int8 *rData, unsigned int16 Count);

// #use rtos() Prototypes:
_bif void rtos_run(void);
_bif void rtos_yield(void);
_bif void rtos_enable(unsigned int8 task);
_bif void rtos_disable(unsigned int8 task);
_bif void rtos_terminate(void);
_bif void rtos_await(int1 flag);
_bif void rtos_wait(unsigned int8 sem);
_bif void rtos_signal(unsigned int8 sem);
_bif void rtos_msg_send(unsigned int8 task, unsigned int8 msg);
_bif unsigned int8 rtos_msg_read(void);
_bif unsigned int8 rtos_msg_poll(void);
_bif int1 rtos_overrun(unsigned int8 task);
_bif void rtos_stats(unsigned int8 task, unsigned int8* stat);

// #use timer() Prototypes:
_bif unsigned int8 get_ticks(void);
_bif unsigned int16 get_ticks(void);
_bif unsigned int32 get_ticks(void);
_bif unsigned int8 get_ticks(unsigned int8 stream);
_bif unsigned int16 get_ticks(unsigned int8 stream);
_bif unsigned int32 get_ticks(unsigned int8 stream);
_bif void set_ticks(unsigned int8 value);
_bif void set_ticks(unsigned int16 value);
_bif void set_ticks(unsigned int32 value);
_bif void set_ticks(unsigned int8 stream, unsigned int8 value);
_bif void set_ticks(unsigned int8 stream, unsigned int16 value);
_bif void set_ticks(unsigned int8 stream, unsigned int32 value);

// #use pwm() Prototypes:
_bif void pwm_on(void);
_bif void pwm_on(unsigned int8 stream);
_bif void pwm_off(void);
_bif void pwm_off(unsigned int8 stream);
_bif void pwm_set_duty(unsigned int16 duty);
_bif void pwm_set_duty(unsigned int8 stream, unsigned int16 duty);
_bif void pwm_set_duty_percent(unsigned int16 percent);
_bif void pwm_set_duty_percent(unsigned int8 stream, unsigned int16 percent);
_bif void pwm_set_frequency(unsigned int32 frequency);
_bif void pwm_set_frequency(unsigned int8 stream, unsigned int32 frequency);
_bif unsigned int16 pwm_get_duty_period(void);
_bif unsigned int16 pwm_get_duty_period(unsigned int8 stream);

// #use capture() Prototypes:
_bif unsigned int16 get_capture_time(void);
_bif unsigned int16 get_capture_time(unsigned int8 stream);
_bif int1 get_capture_event(void);
_bif int1 get_capture_event(unsigned int8 stream);

// Enviroment Prototypes:
//_bif unsigned int8 getenv(char* cstring);

// Address Prototypes:
#ifndef __ADDRESS__
 #if defined(__PCM__) || defined(__PCB__)
  #define __ADDRESS__ unsigned int16
 #else
  #define __ADDRESS__ unsigned int32
 #endif
#endif
_bif void goto_address(__ADDRESS__ address);
_bif __ADDRESS__ label_address(__ADDRESS__ label);

// Program Memory Read Prototypes:
_bif void read_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif unsigned int16 read_program_eeprom(__ADDRESS__ address);

// Program Memory Write Prototypes:
_bif void erase_program_eeprom(__ADDRESS__ address); 
_bif void write_program_memory(__ADDRESS__ address, unsigned int8* dataptr, unsigned int16 count);
_bif void write_program_eeprom(__ADDRESS__ address, unsigned int16 data);

// #use touchpad() Prototypes:
_bif void touchpad_state(unsigned int8 state);
_bif char touchpad_getc(void);
_bif int1 touchpad_hit(void);
////////////////////////////////////////////////////////////////// INT
// Interrupt Functions: ENABLE_INTERRUPTS(), DISABLE_INTERRUPTS(),
//                      CLEAR_INTERRUPT(), INTERRUPT_ACTIVE(),  
//                      EXT_INT_EDGE()  
// INT Prototypes:
_bif void enable_interrupts(int32 interrupt);
_bif void disable_interrupts(int32 interrupt);
_bif void clear_interrupt(int32 interrupt);
_bif int1 interrupt_active(int32 interrupt);
_bif int1 interrupt_enabled(int32 interrupt);
_bif void ext_int_edge(int8 source, int8 edge);  
_bif void jump_to_isr(int16 address);
// Constants used in EXT_INT_EDGE() are:  
#define L_TO_H              0x40  
#define H_TO_L                 0  
// Constants used in ENABLE/DISABLE_INTERRUPTS() are:
#define GLOBAL                    0xF2C0
#define PERIPH                    0xF240
#define INT_IOC_PIN               0xE0000000
#define INT_IOC_PIN_L2H           0xA0000000
#define INT_IOC_PIN_H2L           0xC0000000
#define INT_RTCC                  0x00F220
#define INT_TIMER0                0x00F220
#define INT_TIMER1                0x009D01
#define INT_TIMER2                0x009D02
#define INT_TIMER3                0x00A002
#define INT_EXT_L2H               0x5000F210
#define INT_EXT_H2L               0x6000F210
#define INT_EXT                   0x00F210
#define INT_EXT1_L2H              0x5001F008
#define INT_EXT1_H2L              0x6001F008
#define INT_EXT1                  0x00F008
#define INT_EXT2_L2H              0x5002F010
#define INT_EXT2_H2L              0x6002F010
#define INT_EXT2                  0x00F010
#define INT_EXT3_L2H              0x5003F020
#define INT_EXT3_H2L              0x6003F020
#define INT_EXT3                  0x00F020
#define INT_PSP                   0x009D80
#define INT_AD                    0x009D40
#define INT_RDA                   0x009D20
#define INT_TBE                   0x009D10
#define INT_SSP                   0x009D08
#define INT_BUSCOL                0x00A008
#define INT_LOWVOLT               0x00A004
#define INT_RDA2                  0x00A320
#define INT_TBE2                  0x00A310
#define INT_OSCF                  0x00A080
#define INT_LCD                   0x00A340
#define INT_CCP1                  0x00A302
#define INT_CCP2                  0x00A304
#define INT_USB                   0x00A010
#define INT_RTC                   0x00A301
#define INT_CTMU                  0x00A308
#define INT_TMR1G                 0x009D04
#define INT_TMR3G                 0x00A001
#define INT_TMR5G                 0x00A380
#define INT_BUSCOL2               0x00A120
#define INT_SSP2                  0x00A140
#define INT_CCP3                  0x00B401
#define INT_CCP4                  0x00B402
#define INT_CCP5                  0x00B404
#define INT_CCP6                  0x00B408
#define INT_CCP7                  0x00B410
#define INT_CCP8                  0x00B420
#define INT_CCP9                  0x00B440
#define INT_CCP10                 0x00B480
#define INT_TIMER4                0x00B701
#define INT_TIMER5                0x00B702
#define INT_TIMER6                0x00B704
#define INT_TIMER8                0x00B710
#define INT_ACTLOCK               0x00B720
#define INT_ACTOR                 0x00B740
#define INT_COMP1                 0x006101
#define INT_COMP2                 0x006102
#define INT_COMP3                 0x006104
#define INT_TBE3                  0x006110
#define INT_RDA3                  0x006120
#define INT_TBE4                  0x006140
#define INT_RDA4                  0x006180
#define INT_IOC                   0x0FFFF208

#list
