/*

    defines for all the SNES memory registers

*/

#define PPU_START   0x2100
#define PPU_END     0x213F

#define DMA_START   0x4300
#define DMA_END     0x437F

#define INIDISP     0x2100
#define OBJSEL      0x2101
#define OAMADDL     0x2102
#define OAMADDH     0x2103
#define OAMDATA     0x2104

#define BGMODE      0x2105
#define MOSAIC      0x2106

#define BG1SC       0x2107
#define BG2SC       0x2108
#define BG3SC       0x2109
#define BG4SC       0x210A

#define BG12NBA     0x210B
#define BG34NBA     0x210C

#define M7HOFS      0x210D
#define M7VOFS      0x210E

#define BG1HOFS     0x210D
#define BG1VOFS     0x210E
#define BG2HOFS     0x210F
#define BG2VOFS     0x2110
#define BG3HOFS     0x2111
#define BG3VOFS     0x2112
#define BG4HOFS     0x2113
#define BG4VOFS     0x2114

#define VMAIN       0x2115

#define VMADDL      0x2116
#define VMADDH      0x2117

#define VMDATAL     0x2118
#define VMDATAH     0x2119

#define M7SEL       0x211A
#define M7A         0x211B
#define M7B         0x211C
#define M7C         0x211D
#define M7D         0x211E
#define M7X         0x211F
#define M7Y         0x2120

#define CGADD       0x2121
#define CGDATA      0x2122

#define W12SEL      0x2123
#define W34SEL      0x2124
#define WOBJSEL     0x2125

#define WH0         0x2126
#define WH1         0x2127
#define WH2         0x2128
#define WH3         0x2129

#define WBGLOG      0x212A
#define WOBJGLOG    0x212B

#define TM          0x212C
#define TS          0x212D
#define TMW         0x212E
#define TSW         0x212F

#define CGWSEL      0x2130
#define CGADSUB     0x2131

#define COLDATA     0x2132

#define SETINI      0x2133

#define MPYL        0x2134
#define MPYM        0x2135
#define MPYH        0x2136

#define SLHV        0x2137

#define OAMDATAREAD 0x2138

#define VMDATALREAD 0x2139
#define VMDATAHREAD 0x213A

#define CGDATAREAD  0x213B

#define OPHCT       0x213C
#define OPVCT       0x213D

#define STAT77      0x213E
#define STAT78      0x213F

#define APUIO0      0x2140
#define APUIO1      0x2141
#define APUIO2      0x2142
#define APUIO3      0x2143

#define WMDATA      0x2180
#define WMADDL      0x2181
#define WMADDM      0x2182
#define WMADDH      0x2183

#define JOYOUT      0x4016
#define JOYSER0     0x4016
#define JOYSER1     0x4017

#define NMITIMEN    0x4200

#define WRIO        0x4201
#define WRMPYA      0x4202
#define WRMPYB      0x4203
#define WRDIVL      0x4204
#define WRDIVH      0x4205
#define WRDIVB      0x4206

#define HTIMEL      0x4207
#define HTIMEH      0x4208

#define VTIMEL      0x4209
#define VTIMEH      0x420A

#define MDMAEN      0x420B
#define HDMAEN      0x420C

#define MEMSEL      0x420D

#define RDNMI       0x4210

#define TIMEUP      0x4211

#define HVBJOY      0x4212

#define RDIO        0x4213
#define RDDIVL      0x4214
#define RDDIVH      0x4215
#define RDMPYL      0x4216
#define RDMPYH      0x4217

#define JOY1L       0x4218
#define JOY1H       0x4219
#define JOY2L       0x421A
#define JOY2H       0x421B
#define JOY3L       0x421C
#define JOY3H       0x421D
#define JOY4L       0x421E
#define JOY4H       0x421F

#define DMAPn       0x4300
#define BBADn       0x4301
#define A1TnL       0x4302
#define A1TnH       0x4303
#define A1Bn        0x4304
#define DASnL       0x4305
#define DASnH       0x4306
#define DASBn       0x4307
#define A2AnL       0x4308
#define A2AnH       0x4309
#define NLTRn       0x430A

#define UNUSEDn     0x430B
#define UNUSEDn     0x430F
