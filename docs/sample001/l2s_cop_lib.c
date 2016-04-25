/********************************************************************************
**
**  CONFIDENTIAL - GETRAG Getriebe- und Zahnradfabrik
**                 Hermann Hagenmeyer GmbH & Cie KG
** 
**  this is an unpublished work, which is a trade secret, created in 2013.      
**  GETRAG Getriebe- und Zahnradfabrik Hermann Hagenmeyer GmbH & Cie KG
**  owns all rights to this work and intends to maintain it in confidence
**  to preserve its trade secret status. GETRAG Getriebe- und Zahnradfabrik
**  Hermann Hagenmeyer GmbH & Cie KG reserves the right to protect this work       
**  as an unpublished copyrighted work in the event of an inadvertent or      
**  deliberate unauthorized publication. GETRAG Getriebe- und Zahnradfabrik
**  Hermann Hagenmeyer GmbH & Cie KG also reserves its rights under the copyright
**  laws to protect this work as a published work. Those having access to this
**  work may not copy it, use it or disclose the information contained in it
**  without the written authorization of GETRAG Getriebe- und Zahnradfabrik
**  Hermann Hagenmeyer GmbH & Cie KG.
**
*******************************************************************************/

/***************************************************************************//**
*
*     \file            l2s_cop_lib.c
*
*     \author          Tim Weyres
*
*     \date            03.06.2013
*
*     \b Compiler:     Renesas V9.04
*
*     \b Description:  Level 2 software library functions for Core Platform
*
*                      The library comprise of following functions:          \n
*                      l2s_lib_abs_s16_sat()                                 \n
*                      l2s_lib_s16_sat()                                     \n
*                      l2s_lib_s16_add_s16_s16_sat()                         \n
*                      l2s_lib_s32_add_s32_s32_sat()                         \n
*                      l2s_lib_s16_sub_s16_s16_sat()                         \n
*                      l2s_lib_u16_add_u16_u16_sat()                         \n
*                      l2s_lib_u16_sub_u16_u16_sat()                         \n
*                      l2s_lib_div_u32u32()                                  \n
*                      l2s_lib_div_s32s32()                                  \n
*                      l2s_abs_s32()                                         \n
*                      l2s_lib_idx_brkpt_srch()                              \n
*                      l2s_lib_ipol_brkpt_srch()                             \n
*                      l2s_lib_ipol_dim2_S16_S16()                           \n
*                      l2s_lib_rolav_S16_S16()                               \n
*                      l2s_lib_mul_s16_s16_sr13_sat()                        \n
*                      l2s_lib_maxS16S16S16()                                \n
*                                                                            \n 
*******************************************************************************/


/******************************************************************************
*
* $ProjectName: d:/MKS/Repository/GCG_PS_VP_150_DF/L2S_CORE.pj $
*
* $ProjectRevision: 1.33 $
*
* --------------------------------------------------------------------------
* Modification History:
* --------------------------------------------------------------------------
* $Log: f_03/level2/lib/l2s_cop_lib.c  $
* Revision 1.24 2015/06/19 11:56:32MESZ Harald.Schwindt 
* TASK-920557: fix l2s_lib_rolav_S16_S16 and merge parallel version
* -----------------------------------------------------------------
* --- Added comments ---  Harald.Schwindt [2015/06/19 09:57:55Z]
* Review passed / H.Schwindt, G.Dickers / Ref.: 1.23 & 1.23.1.1
* -------------------------------------------------------------
*    --------------+----------------+-----------------------------------------\n
*    16. Jun 2015  | Guido Dickers  | -l2s_lib_idx_brkpt_srch and             \n
*                  |                |  l2s_lib_ipol_brkpt_srch have a logic   \n
*                  |                |  error within the loop for the index;   \n
*                  |                |  correct it (Knot within brain)         \n
*    --------------+----------------+-----------------------------------------\n
*    13. May 2015  | H.Schwindt     | -justify polyspace warning; TASK-915676 \n
*    --------------+----------------+-----------------------------------------\n
*     8. Apr 2015  | Guido Dickers  | -eleminate MISRA warnings; TASK-915656  \n
*    --------------+----------------+-----------------------------------------\n
*    10. Mar 2015  | Tim Weyres     | -add function l2s_lib_s16_sat           \n
*                  |                | -remove functions l2s_add_circular()    \n
*                  |                |  and l2s_init_circular()                \n
*    --------------+----------------+-----------------------------------------\n
*    21. Nov 2014  | Guido Dickers  | -add requirement links (only comments)  \n
*    --------------+----------------+-----------------------------------------\n
*    21 Nov 2014   | Tim Weyres     | -add requirement links (only comments)  \n
*    --------------+----------------+-----------------------------------------\n
*     7 Nov 2013   | Guido Dickers  | -introduce first order low pass filter  \n
*    --------------+----------------+-----------------------------------------\n
*     9 Jul 2013   | Guido Dickers  | -introduce signed subtraction with      \n
*                  |                |  saturation of S16                      \n
*                  |                | -introduce abs with saturation of S16   \n
*    --------------+----------------+-----------------------------------------\n
*     8 Jul 2013   | Guido Dickers  | -introduce signed addition with         \n
*                  |                |  saturation of S16                      \n
*                  |                | -introduce unsigned addition with       \n
*                  |                |  saturation of U16                      \n
*                  |                | -introduce unsigned subtraction         \n
*                  |                |  with saturation of U16                 \n
*    --------------+----------------+-----------------------------------------\n
*     5 Jul 2013   | Guido Dickers  | -modify breakpoint index search         \n
*                  |                | -extend naming of interpolation         \n
*                  |                |  functions                              \n
*    --------------+----------------+-----------------------------------------\n 
*    21 Jun 2013   | Guido Dickers  | -introduce two dimensional interpolation\n
*                  |                |  function                               \n
*                  |                | -introduce pure breakpoint search       \n
*    --------------+----------------+-----------------------------------------\n
*    03.06.2013    | Tim Weyres     | initial release                         \n
*    --------------+----------------+-----------------------------------------\n

*
*******************************************************************************/

/*******************************************************************************
*
*     Include Files
*
*******************************************************************************/
#include "getrag_context.h"
#include "l2s_cop_lib.h"

/*******************************************************************************
*
*     Global Constants (const)
*
*******************************************************************************/


/*******************************************************************************
*
*     Global Variables
*
*******************************************************************************/


/*******************************************************************************
*
*     Export Functions
*
*******************************************************************************/


/*******************************************************************************
*
*     Internal Defines and Macros (#define)
*
*******************************************************************************/


/*******************************************************************************
*
*     Internal Constants (static const)
*
*******************************************************************************/
/*
 ******************************************************************************
 * crc8 (J1850) Lookup table
 ******************************************************************************
 */
static const GT_U8 crc8table[256] =
{
   /* [0x00..0x0F] */    0,  29,  58,  39, 116, 105,  78,  83, 232, 245, 210, 207, 156, 129, 166, 187,
   /* [0x10..0x1F] */  205, 208, 247, 234, 185, 164, 131, 158,  37,  56,  31,   2,  81,  76, 107, 118,
   /* [0x20..0x2F] */  135, 154, 189, 160, 243, 238, 201, 212, 111, 114,  85,  72,  27,   6,  33,  60,
   /* [0x30..0x3F] */   74,  87, 112, 109,  62,  35,   4,  25, 162, 191, 152, 133, 214, 203, 236, 241,
   /* [0x40..0x4F] */   19,  14,  41,  52, 103, 122,  93,  64, 251, 230, 193, 220, 143, 146, 181, 168,
   /* [0x50..0x5F] */  222, 195, 228, 249, 170, 183, 144, 141,  54,  43,  12,  17,  66,  95, 120, 101,
   /* [0x60..0x6F] */  148, 137, 174, 179, 224, 253, 218, 199, 124,  97,  70,  91,   8,  21,  50,  47,
   /* [0x70..0x7F] */   89,  68,  99, 126,  45,  48,  23,  10, 177, 172, 139, 150, 197, 216, 255, 226,
   /* [0x80..0x8F] */   38,  59,  28,   1,  82,  79, 104, 117, 206, 211, 244, 233, 186, 167, 128, 157,
   /* [0x90..0x9F] */  235, 246, 209, 204, 159, 130, 165, 184,   3,  30,  57,  36, 119, 106,  77,  80,
   /* [0xA0..0xAF] */  161, 188, 155, 134, 213, 200, 239, 242,  73,  84, 115, 110,  61,  32,   7,  26,
   /* [0xB0..0xBF] */  108, 113,  86,  75,  24,   5,  34,  63, 132, 153, 190, 163, 240, 237, 202, 215,
   /* [0xC0..0xCF] */   53,  40,  15,  18,  65,  92, 123, 102, 221, 192, 231, 250, 169, 180, 147, 142,
   /* [0xD0..0xDF] */  248, 229, 194, 223, 140, 145, 182, 171,  16,  13,  42,  55, 100, 121,  94,  67,
   /* [0xE0..0xEF] */  178, 175, 136, 149, 198, 219, 252, 225,  90,  71,  96, 125,  46,  51,  20,   9,
   /* [0xF0..0xFF] */  127,  98,  69,  88,  11,  22,  49,  44, 151, 138, 173, 176, 227, 254, 217, 196
};

/*
 ******************************************************************************
 * crc16 Lookup table
 ******************************************************************************
 */

static unsigned short crc16table[256] = /* CRC lookup table polynomial 0xA001 */
{
    0x0000, 0xa001, 0xe003, 0x4002, 0x6007, 0xc006, 0x8004, 0x2005,
    0xc00e, 0x600f, 0x200d, 0x800c, 0xa009, 0x0008, 0x400a, 0xe00b,
    0x201d, 0x801c, 0xc01e, 0x601f, 0x401a, 0xe01b, 0xa019, 0x0018,
    0xe013, 0x4012, 0x0010, 0xa011, 0x8014, 0x2015, 0x6017, 0xc016,
    0x403a, 0xe03b, 0xa039, 0x0038, 0x203d, 0x803c, 0xc03e, 0x603f,
    0x8034, 0x2035, 0x6037, 0xc036, 0xe033, 0x4032, 0x0030, 0xa031,
    0x6027, 0xc026, 0x8024, 0x2025, 0x0020, 0xa021, 0xe023, 0x4022,
    0xa029, 0x0028, 0x402a, 0xe02b, 0xc02e, 0x602f, 0x202d, 0x802c,
    0x8074, 0x2075, 0x6077, 0xc076, 0xe073, 0x4072, 0x0070, 0xa071,
    0x407a, 0xe07b, 0xa079, 0x0078, 0x207d, 0x807c, 0xc07e, 0x607f,
    0xa069, 0x0068, 0x406a, 0xe06b, 0xc06e, 0x606f, 0x206d, 0x806c,
    0x6067, 0xc066, 0x8064, 0x2065, 0x0060, 0xa061, 0xe063, 0x4062,
    0xc04e, 0x604f, 0x204d, 0x804c, 0xa049, 0x0048, 0x404a, 0xe04b,
    0x0040, 0xa041, 0xe043, 0x4042, 0x6047, 0xc046, 0x8044, 0x2045,
    0xe053, 0x4052, 0x0050, 0xa051, 0x8054, 0x2055, 0x6057, 0xc056,
    0x205d, 0x805c, 0xc05e, 0x605f, 0x405a, 0xe05b, 0xa059, 0x0058,
    0xa0e9, 0x00e8, 0x40ea, 0xe0eb, 0xc0ee, 0x60ef, 0x20ed, 0x80ec,
    0x60e7, 0xc0e6, 0x80e4, 0x20e5, 0x00e0, 0xa0e1, 0xe0e3, 0x40e2,
    0x80f4, 0x20f5, 0x60f7, 0xc0f6, 0xe0f3, 0x40f2, 0x00f0, 0xa0f1,
    0x40fa, 0xe0fb, 0xa0f9, 0x00f8, 0x20fd, 0x80fc, 0xc0fe, 0x60ff,
    0xe0d3, 0x40d2, 0x00d0, 0xa0d1, 0x80d4, 0x20d5, 0x60d7, 0xc0d6,
    0x20dd, 0x80dc, 0xc0de, 0x60df, 0x40da, 0xe0db, 0xa0d9, 0x00d8,
    0xc0ce, 0x60cf, 0x20cd, 0x80cc, 0xa0c9, 0x00c8, 0x40ca, 0xe0cb,
    0x00c0, 0xa0c1, 0xe0c3, 0x40c2, 0x60c7, 0xc0c6, 0x80c4, 0x20c5,
    0x209d, 0x809c, 0xc09e, 0x609f, 0x409a, 0xe09b, 0xa099, 0x0098,
    0xe093, 0x4092, 0x0090, 0xa091, 0x8094, 0x2095, 0x6097, 0xc096,
    0x0080, 0xa081, 0xe083, 0x4082, 0x6087, 0xc086, 0x8084, 0x2085,
    0xc08e, 0x608f, 0x208d, 0x808c, 0xa089, 0x0088, 0x408a, 0xe08b,
    0x60a7, 0xc0a6, 0x80a4, 0x20a5, 0x00a0, 0xa0a1, 0xe0a3, 0x40a2,
    0xa0a9, 0x00a8, 0x40aa, 0xe0ab, 0xc0ae, 0x60af, 0x20ad, 0x80ac,
    0x40ba, 0xe0bb, 0xa0b9, 0x00b8, 0x20bd, 0x80bc, 0xc0be, 0x60bf,
    0x80b4, 0x20b5, 0x60b7, 0xc0b6, 0xe0b3, 0x40b2, 0x00b0, 0xa0b1
};



/*******************************************************************************
*
*     Internal Variables (static)
*
*******************************************************************************/


/******************************************************************************
*
*     Internal Functions (static)
*
*******************************************************************************/


/*******************************************************************************
*
*     Internal Variables
*
*******************************************************************************/

/***************************************************************************//**
  \brief  <b>crc8 checksum algorithm</b>

  <b>Component Functionality</b>\n
    implementation of table driven crc8 algorithm
    (SAE J1850)
    genpoly: I000III0I = x^8+x^4+x^3+x^2+1 = 1Dh
    initial: FFh
    input not reflected
    result not reflected
    Final XOR: FFh
    Check: 4Bh
    Magic: C4h
    
  <b>Control Flow:</b>

  <b>Calibration Parameters:</b>\n
     none

  <b>Inputs:</b>\n
  start : pointer to start of data section
  size :  byte size of data section in

  <b>Outputs:</b>\n
  crc8 checksum

  \param  none
  \return none

*******************************************************************************/
#ifndef l2s_calc_crc8_MOCK
GT_U8 l2s_calc_crc8(GT_U8* start, GT_U32 size)
{
    GT_U32 i;
    GT_U8 crc = 0xFFU;

    for(i = 0; i < size; i++)
    {
        /* polyspace<RTE: IDP : Not a defect : Justify with annotations > The address-of operator �&� will never return a null pointer and the size-of-operator limits the access to the memory */
        crc = crc8table[crc ^ start[i]];
    }
    /* invert result */
    crc = crc ^ 0xFFU;

    return crc;
}
#endif

/***************************************************************************//**
  \brief  <b>crc16 checksum algorithm</b>

  <b>Component Functionality</b>\n
    implementation of table driven crc16 algorithm
    (?)
    genpoly: x^16+x^15+x^13+1 = A001h
    initial: 0001h
    input not reflected
    result not reflected
    Final XOR: 0000h
    
  <b>Control Flow:</b>

  <b>Calibration Parameters:</b>\n
     none

  <b>Inputs:</b>\n
  start : pointer to start of data section
  size :  byte size of data section in

  <b>Outputs:</b>\n
  crc16 checksum

  \param  none
  \return none

*******************************************************************************/
GT_U16 l2s_calc_crc16(GT_U8* start, GT_U32 size)
{
    GT_U32 i;
    GT_U16 crc = 0x0001;
    GT_U8 index;

    for(i = 0; i < size; i++)
    {
        /* polyspace<RTE: IDP : Not a defect : Justify with annotations > The address-of operator �&� will never return a null pointer and the size-of-operator limits the access to the memory */
        index = (GT_U8)(crc >> 8) ^ start[i];
        crc = crc16table[index] ^ (GT_U16)(crc << 8);
    }
    /* invert result */
    //crc = crc ^ 0xFFFF;

    return crc;
}


/***************************************************************************//** 
  \brief  <b>16 bit absolute value with saturation</b>
  
  <b>Component Functionality</b>                                                             \n
    This library function calculates the absolute value and performs                         \n
    a saturation to the max value of data type S16.                                          \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_abs_s16_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

    \param  in_value_abs_sat            Input signal
    \return Output value

*******************************************************************************/
GT_S16 l2s_lib_abs_s16_sat(GT_S16 in_value_abs_sat)
{
    /* Define local variables */
    GT_S16 out_value_abs_sat;

    /* Input < 0 */
    if (in_value_abs_sat < 0)
    {
        /* Input <= S16_MIN */
        if (in_value_abs_sat <= GT_S16_MIN)
        {
            /* Set output S16_MAX */
            out_value_abs_sat = (GT_S16)GT_S16_MAX;
        }
        else
        {
            /* Set output to input * (-1) */
            out_value_abs_sat = (-1) * in_value_abs_sat;
        }
    }
    else
    {
        /* Set output to input */
        out_value_abs_sat = in_value_abs_sat;
    }

    /* Return output value */
    return(out_value_abs_sat);
}


/***************************************************************************//** 
  \brief  <b>Change data type to 16 bit with saturation</b>
  
  <b>Component Functionality</b>                                                             \n
    This library function changes data type from S32 to S16 and performes                    \n
    a saturation to min and max value of data type S16.                                      \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_s16_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

    \param  in_value            Input signal
    \return Output value

*******************************************************************************/
GT_S16 l2s_lib_s16_sat(GT_S32 in_value)
{
    /* Define local variables */
    GT_S16 out_value_sat;

    /* Input > S16 max */
    if (in_value > GT_S16_MAX)
    {
        /* Set output to GT_S16_MAX */
        out_value_sat = GT_S16_MAX;
    }
    else
    {
        /* Input < S16_MIN */
        if (in_value < GT_S16_MIN)
        {
            /* Set output to GT_S16_MIN */
            out_value_sat = (GT_S16)GT_S16_MIN;
        }
        else
        {
            /* Set output to input */
            out_value_sat = (GT_S16)in_value;
        }
    }

    /* Return output value */
    return(out_value_sat);
}

/***************************************************************************//** 
  \brief  <b>16 bit signed addition with upper and lower saturation</b>
  
  <b>Component Functionality</b>                                                             \n
    This library function performs a addition of the two input signals with                  \n
    saturation to the minimum and maximum of signed 16 bit data type.                        \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b>
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_s16_add_s16_s16_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

    \param  in_value1            First input value of data type S16
    \param  in_value2            Second input value of data type S16
    \return Saturated result value

*******************************************************************************/
GT_S16 l2s_lib_s16_add_s16_s16_sat(GT_S16 in_value1, GT_S16 in_value2)
{
    /* Define local variables \n\n*/
    GT_S16 ret_value;
  
    /* Calculate sum of input 1 and input 2 */
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */
    ret_value = in_value1 + in_value2;

    /* Input 1 < 0 AND
       input 2 < 0 AND
       calculated sum >= 0? */
    if ( (in_value1 < 0) && (in_value2 < 0) && (ret_value >= 0) )
    {
        /* Underflow detected:
           Set output to data type S16 minimum */
        ret_value = (GT_S16)GT_S16_MIN;
    }
    else
    {
        /* Input 1 > 0 AND
           input 2 > 0 AND
           calculated sum <= 0? */
        if ( (in_value1 > 0) && (in_value2 > 0) && (ret_value <= 0) )
        {
        /* Overflow detected:
           Set output to data type S16 maximum */
            ret_value = (GT_S16)GT_S16_MAX;
        }
    }

    /* Return result */
    return(ret_value);
}

/***************************************************************************//** 
  \brief  <b>32 bit signed addition with upper and lower saturation</b>
  
  <b>Component Functionality</b>                                                             \n
    This library function performs a addition of the two input signals with                  \n
	saturation to the minimum and maximum of signed 32 bit data type.                        \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b>
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_s32_add_s32_s32_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

    \param  in_value1            First input value of data type S32
    \param  in_value2            Second input value of data type S32
    \return Saturated result value

*******************************************************************************/
GT_S32 l2s_lib_s32_add_s32_s32_sat(GT_S32 in_value1, GT_S32 in_value2)
{
    /* Define local variables \n\n*/
    GT_S32 ret_value;
  
    /* Calculate sum of input 1 and input 2 */
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */
    ret_value = in_value1 + in_value2;

    /* Input 1 < 0 AND
       input 2 < 0 AND
       calculated sum >= 0? */
    if ( (in_value1 < 0) && (in_value2 < 0) && (ret_value >= 0) )
    {
        /* Underflow detected:
           Set output to data type S32 minimum */
        ret_value = (GT_S32)GT_S32_MIN;
    }
    else
    {
        /* Input 1 > 0 AND
           input 2 > 0 AND
           calculated sum <= 0? */
        if ( (in_value1 > 0) && (in_value2 > 0) && (ret_value <= 0) )
        {
            /* Overflow detected:
               Set output to data type S16 maximum */
            ret_value = (GT_S32)GT_S32_MAX;
        }
    }

    /* Return result */
    return(ret_value);
}

/***************************************************************************//** 
  \brief  <b>16 bit signed subtraction with upper and lower saturation</b>
  
  <b>Component Functionality</b>                                                             \n
    This library function performs a subtraction of the two input signals with               \n
	saturation to the minimum and maximum of signed 16 bit data type.                        \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b>
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_s16_sub_s16_s16_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

    \param  in_value1            First input value of data type S16
    \param  in_value2            Second input value of data type S16
    \return Saturated result value

*******************************************************************************/
GT_S16 l2s_lib_s16_sub_s16_s16_sat(GT_S16 in_value1, GT_S16 in_value2)
{
    /* Define local variables \n\n*/
    GT_S16 ret_value;

    /* Calculate difference of input 1 and input 2 */
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */
    ret_value = in_value1 - in_value2;

    /* Input 1 < 0 AND
       input 2 > 0 AND
       calculated difference >= 0? */
    if ( (in_value1 < 0) && (in_value2 > 0) && (ret_value >= 0) )
    {
        /* Underflow detected:
           Set output to data type S16 minimum */
        ret_value = (GT_S16)GT_S16_MIN;
    }
    else
    {
        /* Input 1 >= 0 AND
           input 2 < 0 AND
           calculated difference <= 0? */
        if ( (in_value1 >= 0) && (in_value2 < 0) && (ret_value <= 0) )
        {
            /* Overflow detected:
               Set output to data type S16 maximum */
            ret_value = (GT_S16)GT_S16_MAX;
        }
    }

    /* Return result */
    return(ret_value);
}

/***************************************************************************//** 
  \brief  <b>16 bit unsigned addition with saturation</b>                   
  
  <b>Component Functionality</b>                                                             \n
    This library function performs a addition of the two input signals with                  \n
	saturation to the maximum of unsigned 16 bit data type.                                  \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b>
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_u16_add_u16_u16_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

    \param  in_value1            First input value of data type U16
    \param  in_value2            Second input value of data type U16
    \return Saturated result value

*******************************************************************************/
GT_U16 l2s_lib_u16_add_u16_u16_sat(GT_U16 in_value1, GT_U16 in_value2)
{
    /* Define local variables \n\n*/
    GT_U16 tmp_result;
  
    /* Calculate sum of input 1 and input 2 */
    tmp_result = in_value1 + in_value2;

    /*input 1 > result*/
    if (in_value1 > tmp_result)
    {
        /* overflow detected:
           Set output to data type U16 maximum */
        tmp_result = (GT_U16)GT_U16_MAX;
    }

    /* Return result */
    return(tmp_result);
}

/***************************************************************************//** 
  \brief  <b>16 bit unsigned subtraction with saturation</b>                   
  
  <b>Component Functionality</b>                                                             \n
    This library function performs a subtraction of input 1 minus input 2 with               \n
	saturation to the minimum of unsigned 16 bit data type.                                  \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b>
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_u16_sub_u16_u16_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

      \param  in_value1            First input value of data type U16
      \param  in_value2            Second input value of data type U16
      \return Saturated result value

*******************************************************************************/
GT_U16 l2s_lib_u16_sub_u16_u16_sat(GT_U16 in_value1, GT_U16 in_value2)
{
    /* Define local variables \n\n*/
    GT_U16 tmp_result;
  
    /* Calculate sum of input 1 and input 2 */
    tmp_result = in_value1 - in_value2;

    /* input 1 < input 2*/
    if ((in_value1 < in_value2))
    {
        /* underflow detected:
           Set output to data type U16 minmum */
        tmp_result = (GT_U16)GT_U16_MIN;
    }

    /* Return result */
    return(tmp_result);
}

/***************************************************************************//**
  
  \brief  <b>Unsigned 32 bit division</b>
  
  <b>Component Functionality</b>\n
  Integer Division U32 = div(U32,U32) with rounding to nearest value

  \req SC_9993 DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_div_u32u32.png"></p>
 
  <b>Calibration Parameters:</b>\n
     none
 
  \param  z:   numerator [U32]
  \param  n:   denominator [U32]
  \return result of z/n [U32]
  
*******************************************************************************/
GT_U32 l2s_lib_div_u32u32(GT_U32 z, GT_U32 n)
{
    /* Define local variables \n\n*/
    GT_U32 result;
    GT_U32 rest;

    /* Divisor is zero? */
    if(n == 0U)
    {
        /* Set output to max value */
        result = GT_U32_MAX;
    }
    else
    {
        result = z/n;
        rest = z%n;
        
        if((rest > 2147483647U) || (rest<<1) >= n)
        {
            /* round to next higher value */
            result++;
        }
    }

    /* Return result*/
    return(result);
}

/***************************************************************************//**
  
  \brief  <b>Signed 32 bit division</b>
  
  <b>Component Functionality</b>\n
  Integer Division S32 = div(S32,S32) with rounding to nearest value and
  negative infinity

  \req SC_9993 DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_div_s32s32.png"></p>
 
  <b>Calibration Parameters:</b>\n
     none
  
  \param  z:   numerator [S32]
  \param  n:   denominator [S32]
  \return result of z/n [S32]
  
*******************************************************************************/
GT_S32 l2s_lib_div_s32s32(GT_S32 z, GT_S32 n)
{
  GT_S32 result;
  GT_U32 abs_z, abs_n, rest;
  GT_BOOL pos;
  if(n == 0)
  {
      if(z >= 0)
      {
          result = GT_S32_MAX;
      }
      else
      {
          result = GT_S32_MIN;
      }
  }
  else
  {
      abs_z = (GT_U32)(z >=0) ? z : (-z);
      abs_n = (GT_U32)(n >=0) ? n : (-n);

      result = abs_z/abs_n;
      rest = abs_z%abs_n;

      if((rest<<1) >= abs_n)
      {
          /* aufrunden */
          result++;
      }

      pos =((n<0) == (z<0));
      
      if(!pos)
      {
        result = -result; 
      }
  }
  return result;
}


/***************************************************************************//**
 
  \brief  <b>32 bit absolute value with saturation</b>
  
  <b>Component Functionality</b>\n
    This library function calculates the absolute value and performs                         \n
    a saturation to the max value of data type S16.                                          \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_abs_s32.png"></p>
 
  <b>Calibration Parameters:</b>\n
     none
 
    \param  i            Input signal  [s32]
    \return Output value               [s32]  
  
*******************************************************************************/
GT_S32 l2s_abs_s32(GT_S32 i)
{
    GT_S32 res;

    if (i == GT_S32_MIN)
    {
        res = GT_S32_MAX;
    }
    else
    {
        res = i < 0 ? -i : i;
    }

    return res;
}

/***************************************************************************//** 
  \brief  <b>Index search routine</b>
  
  <b>Component Functionality</b>\n
    The utility calculates the index of the breakpoint immedeatly below real      
    input    

  \req SC_6529
  \n DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src=""></p>

  <b>Calibration Parameters:</b>\n
     none

  <b>Inputs:</b>\n
     none
     
  <b>Outputs:</b>\n
none 

  \param  x_list  ->  pointer to input breakpoint list, the x values [S16 | - | - ]
  \param  nr      ->  number of breakpoints [S8 | En0 | - ]
  \param  x_in    ->  real input value [S16 | - | - ]
  \return brkpt   ->  breakpoint index [U8 | En0 | - ]
          
  

*******************************************************************************/
GT_U8 l2s_lib_idx_brkpt_srch(const volatile GT_S16 *x_list, GT_S8 nr, GT_S16 x_in)
{
  /*define locals*/
  GT_S8  i;
  GT_U8  tmp_brkpt;

  /*make amount of breakpoints to index*/
  nr--;
    
  /*_OSCA: that was meant-> for(i=nr; ((x_in < x_list[i]) && (i >=0)); i--)*/
  /*_OSCA:                  {*/
  /*_OSCA:                  }*/
  
/*result in i is next breakpoint no. below real input x, lowest value is -1*/  
  for (i=nr; i >=0; i--)
  {
    /*real input >= breakpoint?*/
    /* polyspace<RTE: IDP : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */
    if(x_in >= x_list[i])
    {
        /*breakpoint found,
          leave loop*/
        break;
    }
  }
  	  
  /* x below lowest breakpoint?*/
  if(i< 0)          
  { 
    /*set to lowest breakpoint nr =0*/
    tmp_brkpt= 0;
  }
  else
  {
    /*deliver breakpoint index*/
    tmp_brkpt= (GT_U8)(i); 
  }
  
  /*return index*/
  return(tmp_brkpt);
}



/***************************************************************************//** 
  \brief  <b>Breakpoint search routine for function and map interpolation</b>
                                   
  <b>Component Functionality</b>\n
    The utility calculates the index of the breakpoint immedeatly below real      
    input and the interval ratio which is calculated with                       \n
    
    r = (x_in - X_i)/(X_(i+1)- X_i)                                             \n
          
    respectively r = u / d                                                      \n
                                     
    The following figure shows the circumstances
    <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_ipol_brkpt_srch.png"></p>
    
*******************************************************************************/  
  
/*    
      |<------------- d ------------>|                                       
      |                              |                         
      |<--- u -->|                   |                                          
      |          |                   |                         
      |          |                   |                         
  ----|----------|-------------------|---------------> x       
  ---- x_in ---->|                   |                          
      |<-- r --->|                   |                         
      |<------------ "1" ----------->|                         
      X_i                            X_(i+1)                   
*/    
    
/***************************************************************************//** 
  \req SC_6529
  \n DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src=""></p>

  <b>Calibration Parameters:</b>\n
     none

  <b>Inputs:</b>\n
     none
     
  <b>Outputs:</b>\n
     none  

  \param  x_list  ->  pointer to input breakpoint list, the x values [S16 | - | - ]                                                                         
  \param  nr      ->  number of breakpoints [S8 | En0 | - ]
  \param  x_in    ->  real input value [S16 | - | - ]                              
                                                                                   
  \return 
          structure which contains the breakpoint index and and the interval ratio \n
          .idx        ->  breakpoint index [S8 | En0 | - ]                         \n
          .ival_ratio ->  interval ratio [S16 | En14 | - ]
          
*******************************************************************************/
l2s_lib_ipol_brkpt_type l2s_lib_ipol_brkpt_srch(const volatile GT_S16 *x_list, GT_S8 nr, GT_S16 x_in)
{
  /*define locals*/
  GT_S8  i;
  GT_S16 tmp_u;
  GT_S16 tmp_d;
  GT_S16 tmp_r; /*_OSCA: ratio [S16 | En14 | - ]*/
  
  /*define local breakpoint structure/n*/
  l2s_lib_ipol_brkpt_type result; 
  
  /*breakpoint search routine for x- or y- input*/
  
  /*make amount of break points to index*/
  nr--;

  
  /*_OSCA: that was meant-> for(i=nr; ((x_in < x_list[i]) && (i >=0)); i--)*/
  /*_OSCA:                  {*/
  /*_OSCA:                  }*/

  /*result in i is next breakpoint no. below real input x, lowest value is -1*/
  for (i=nr; i >=0; i--)
  {
    /*real input >= breakpoint?*/
    if(x_in >= x_list[i])
    {
        /*breakpoint found,
          leave loop*/
        break;
    }
  }
  	  
  /* is x below lowest breakpoint?*/
  if(i< 0)          
  { 
    /*set index i to lowest breakpoint nr =0 and set*/
    /*ratio r to 0 for map/function calculation*/
    i = 0 ;
    tmp_r = 0 ;  
  }                
  else 
  { 
    /*is x above highest breakpoint?*/
    if(i == nr)    
  	{ 
  	  /*set ratio r to 0 for map/function calculation*/
  	  tmp_r = 0;       
  	}
    else  		      
    {                                                                         
      /*here: x_in is in between an intervall X_i and X_(i+1)/n*/
      /*u = x_in - X_i*/
      /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */
      tmp_u = x_in - x_list[i];
      
      /*d = X_(i+1) - X_i*/
      /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */      
      tmp_d = x_list[i+1] - x_list[i];
      
      /*r = (u<<14)/d*/
      /* polyspace<RTE: OVFL, ZDV : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */      
      tmp_r = (GT_S16)((((GT_S32)(tmp_u))*(1*GT_BIN_EN14))/((GT_S32)(tmp_d)));  
    };                                                                        
   }
  
  /*load interval ratio and index into return structure*/
  result.ival_ratio = tmp_r;
  result.idx = i;

  return result;    
}



/***************************************************************************//** 
  \brief  <b>Two dimensional (x/y) linear interpolation</b>
  
  <b>Component Functionality</b>\n
    this utility does a two dimensional (x/y function) interpolation.           \n                         
    The output y is calculated to:                                              \n
                                                                   
    y  =  Y_i * ( 1 - r_x )  +  Y_(i+1) * r_x                                   \n                  
                                                                   
    r_x is the interval ratio which is calculated in the breakpoint
    search uitility.                                                            \n
    
    The figure shows the circumstances for an interval                          \n                                                  
    <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_ipol_dim2_S16_S16.png"></p>
    
*******************************************************************************/
/*    
                                                .....       
                                           .....            
                                      ... *                 
                                   ...    | Y_(i+1)         
                                ...       |                 
                             ...          |                 
                          ...             |                 
                       ...                |                 
                      *                   |                 
                  ... | y                 |                 
               ...    |                   |                 
             ...      |                   |                 
           * Y_i      |                   |                 
     ..... |          |                   |                 
  ....     |          |                   |                 
   --------|----------|-------------------|--------------> x
   -------- x_in ---->|                                     
           |<- r_x  ->|                   |                 
           |<------------ "1" ----------->|                 
           X_i                            X_(i+1)           
*/

/***************************************************************************//**     
  \req SC_6529
  \n DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src=""></p>

  <b>Calibration Parameters:</b>\n
     none

  <b>Inputs:</b>\n
     none
     
  <b>Outputs:</b>\n
     none  


  \param  ptr_dim2  -> pointer to the data input structure which 
                       contains pointers to the x and y values as
                       well as the amount of breakpoints         
                    
  \param  x_in      -> real input value [S16 | - | - ]




  \return y         -> the interpolated value [S16 | - | - ]

*******************************************************************************/
GT_S16 l2s_lib_ipol_dim2_S16_S16(l2s_lib_ipol_dim2_type *ptr_dim2, GT_S16 x_in)
{
    /*define locals*/
    GT_S8  tmp_idx; 
    GT_S16 tmp_r;   /*ratio [S16 | En14 | - ] /n/n*/ 
    GT_S16 y;       
    
    /*local breakpoint structure/n/n*/
    l2s_lib_ipol_brkpt_type tmp_brkpt; 
    
    /*breakpoint evaluation in x-direction/n*/
    tmp_brkpt= l2s_lib_ipol_brkpt_srch(ptr_dim2->x_list, ptr_dim2->no_brk_pts, x_in);
    
    /*load interval ratio and index no. into internal values/n*/
    tmp_idx= tmp_brkpt.idx; 
    tmp_r= tmp_brkpt.ival_ratio;
    
    
    /*calculate output value y = function(x) with the equation
    y= (Y(i) * (1-ratio)) + (Y(i+1) * ratio)*/
    
                                  /*_OSCA:         Y(i) * (1-ratio)         */
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */                              
    y= (GT_S16)(((((GT_S32)(ptr_dim2->y_list[tmp_idx]))*((GT_S32)(((GT_S32)(1*GT_BIN_EN14))-(GT_S32)(tmp_r))))
    
                                  /*_OSCA:         (Y(i+1) * ratio)       */
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */                              
                 +(((GT_S32)(ptr_dim2->y_list[tmp_idx+1]))* ((GT_S32)(tmp_r))))/(1*GT_BIN_EN14)); 
    	                                       
    /*return y*/
    return y ;
    
}



/***************************************************************************//** 
  \brief  <b>Rolling Average Routine</b>
  
  <b>Component Functionality</b>\n
    The utility calculates a first order low pass filter.
    The filter is calculated as follows:
    
    filtered_value=                                          
    input-((input-old_filtered_value)*(1-filter_coefficient))
                                                             
    For a negative residual a rounding towards 0 is done.    
    This rounding leads the filtered value exact onto the    
    input.                                                   
    

  \req SC_3690
  \n DOORS Baseline 4.46

  <b>Control Flow:</b> 
  <p><img src=""></p>

  <b>Calibration Parameters:</b>\n
     none

  <b>Inputs:</b>\n
     none
     
  <b>Outputs:</b>\n
     none 

  \param   input        ->  input value [S16 | - | - ]
  \param   old_result   ->  last filtered value [S16 | - | - ]                 
  \param   filt_coef    ->  filter coefficient [S16 | En14 | - ]
  
  \return  result       ->  filtered input [S16 | - | - ]

*******************************************************************************/
GT_S16 l2s_lib_rolav_S16_S16(GT_S16 input, GT_S16 old_result, GT_S16 filt_coef)
{
    /*define locals*/
    GT_S16 tmp;
    GT_S16 result;
    GT_S32 tmp_residual; /*the residual [S32 | En14 | - ]*/
    
     
    /*calculate the residual*/
    /*residual=               (input - old_result)            *        (1 - filt_coef)*/
    
    /*_OSCA: original statement before MISRA check*/
    /*tmp_residual= (((GT_S32)(input)) - ((GT_S32)(old_result)))*((GT_S32)(((GT_S16)(1*GT_BIN_EN14))-filt_coef));*/
    
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */
    tmp_residual= ((((GT_S32)(input)) - ((GT_S32)(old_result))) * (((GT_S32)(1*GT_BIN_EN14))-((GT_S32)(filt_coef))));

    
    /*bring it back to BIN_EN0*/
                  /*_OSCA:        >>14*/
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */              
    tmp= (GT_S16)(tmp_residual/(GT_BIN_EN14));
    
    /*round a negative value towards 0*/
    if((tmp_residual<0)&&((((GT_U32)(tmp_residual))&0x00003fffU)!= 0U))
    {
     tmp++;
    }
    
    /* polyspace<RTE: OVFL : Not a defect : Justify with annotations > see l2s_polyspace_orange_items_gd2.doc */
    result= input - tmp;
    
  return(result);    
}

/***************************************************************************//**
  \brief  <b>Fixed point multiplication with saturation</b>
  
  <b>Component Functionality</b>                                                             \n
    This software unit performs a multiplication of the two input signals with               \n
	saturation to the minimum and maximum of signed 16 bit data type.                        \n

  \req SC_9992 DOORS Baseline 4.46

  <b>Control Flow:</b> 
     <p><img src="../control_flow_diagrams/l2s_cop_lib.c-l2s_lib_mul_s16_s16_sr13_sat.png"></p>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

      \param  in_value1            First input value  [S16 | En8 | n/a]
      \param  in_value2            Second input value [S16 | En9 | n/a]
      \return Saturated result value [S16 | En4 | n/a]

*******************************************************************************/
GT_S16 l2s_lib_mul_s16_s16_sr13_sat(GT_S16 in_value1, GT_S16 in_value2)
{
  /* Define local variables \n\n*/
  GT_S32 tmp_prod;
  GT_S16 tmp_return_val;

  /* Calculate product of input 1 and input 2 with cast to S32 \n\n*/
  tmp_prod = ((GT_S32)in_value1 * (GT_S32)in_value2);

  /* Take scaling of En14 into account */
  tmp_prod = tmp_prod / GT_BIN_EN13;

  /* Calculated product >
     S16 data type maximum? */
  if (tmp_prod > GT_S16_MAX)
  {
    /* Set output to S16 data type maximum */
    tmp_return_val = (GT_S16)GT_S16_MAX;
  }
  else
  {
    /* Calculated product <=
       S16 data type minimum? */
    if ( tmp_prod <= ((GT_S32)GT_S16_MIN) )
    {
      /* Set output to data type S16 minimum */
      tmp_return_val = (GT_S16)GT_S16_MIN;
    }
    else
    {
      /* Cast calculated product
         to S16 data type */
      tmp_return_val = (GT_S16)tmp_prod;
    }
  }

  /* Return result */
  return (tmp_return_val);
}

/***************************************************************************//**
  \brief  <b>S16 max(S16, S16)</b>

  <b>Component Functionality</b>                                                             \n
     return maximum of two S16

  <b>Control Flow:</b>

  <b>Calibration Parameters:</b>                                                             \n
    - none                                                                                   \n

      \param  in_value1            First input value  [S16 | n/a | n/a]
      \param  in_value2            Second input value [S16 | n/a | n/a]
      \return result value                            [S16 | n/a | n/a]

*******************************************************************************/
GT_S16 l2s_lib_maxS16S16S16(GT_S16 a, GT_S16 b)
{
	return a > b ? a : b;
}






        


  














