#define WRAM_SIZE 131072

#define APUIO0      0x2140
#define APUIO1      0x2141
#define APUIO2      0x2142
#define APUIO3      0x2143

#define WMDATA      0x2180
#define WMADDL      0x2181
#define WMADDM      0x2182
#define WMADDH      0x2183
#define WRIO        0x4201
#define WRMPYA      0x4202
#define WRMPYB      0x4203
#define WRDIVL      0x4204
#define WRDIVH      0x4205
#define WRDIVB      0x4206

#define RDNMI       0x4210
#define RDIO        0x4213
#define RDDIVL      0x4214
#define RDDIVH      0x4215
#define RDMPYL      0x4216
#define RDMPYH      0x4217

#define NMITIMEN    0x4200

#define HTIMEL      0x4207
#define HTIMEH      0x4208

#define VTIMEL      0x4209
#define VTIMEH      0x420A

#define MDMAEN      0x420B
#define HDMAEN      0x420C

#define MEMSEL      0x420D

#define TIMEUP      0x4211

#define HVBJOY      0x4212

#define JOYOUT      0x4016
#define JOYSER0     0x4016
#define JOYSER1     0x4017
#define JOY1L       0x4218
#define JOY1H       0x4219
#define JOY2L       0x421A
#define JOY2H       0x421B
#define JOY3L       0x421C
#define JOY3H       0x421D
#define JOY4L       0x421E
#define JOY4H       0x421F

enum AddressingMode
{
    ACCUMULATOR,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    ABSOLUTE_JMP,
    ABSOLUTE_JMP_LONG,
    ABSOLUTE_JMP_X,
    DIRECT,
    DIRECT_X,
    DIRECT_Y,
    DIRECT_SHORT,
    DIRECT_SHORT_X,
    DIRECT_SHORT_Y,
    DIRECT_LONG,
    DIRECT_LONG_Y,
    IMMEDIATE,
    IMPLIED,
    LONG,
    LONG_X,
    RELATIVE_8,
    RELATIVE_16,
    SOURCE_DEST,
    STACK_S,
    STACK_S_Y,
};

enum ProcessorStatus
{
    EMULATION_MODE  = 0b00000001,
    CARRY_FLAG      = 0b00000001,
    ZERO_FLAG       = 0b00000010,
    INTERRUPT_FLAG  = 0b00000100,
    DECIMAL_MODE    = 0b00001000,
    INDEX_WIDTH     = 0b00010000,
    BREAK_FLAG      = 0b00010000,
    MEMORY_WIDTH    = 0b00100000,
    OVERFLOW_FLAG   = 0b01000000,
    NEGATIVE_FLAG   = 0b10000000
};

typedef struct
{
    unsigned char lo, mi, hi;
} WMADD;

typedef struct
{
    unsigned char _a, _b;
} WRMPY;

typedef struct
{
    unsigned char lo, hi;
} WRDIV;

typedef struct
{
    unsigned char lo, hi;
} RDMPY;

typedef struct 
{
    unsigned short btn_status;
    unsigned char index;
    unsigned char strobe:1;
} JOYPAD;

typedef struct
{
    JOYPAD joypad[4];
} BUS;

typedef struct 
{
    unsigned int    cycles;

    unsigned short  register_a, register_x, register_y, 
                    register_direct, program_counter, stack_pointer,
                    vtimer, htimer;

    unsigned char   wram[WRAM_SIZE], data_bank, program_bank, status, nmitimen, divisor;

    unsigned char   emulation_mode:1, interrupt:1, stop:1, wait:1;

    WMADD address;
    WRMPY multiply;
    WRDIV dividend;
    RDMPY prodrem;

    BUS bus;
} CPU;

extern void bus_increment_waddr();
extern unsigned int bus_addr_get(WMADD);

extern unsigned char bus_joypad_read(JOYPAD*);
extern void bus_joypad_write(JOYPAD*, unsigned char data);

extern unsigned char bus_mem_read(CPU*, unsigned int pos);

extern unsigned int cpu_get_operand_address(CPU *cpu, enum AddressingMode mode);

extern unsigned char cpu_mem_read_u8(CPU*, unsigned int pos);
extern void cpu_mem_write_u8(CPU*, unsigned int pos, unsigned char data);

extern unsigned short cpu_mem_read_u16(CPU*, unsigned int pos);
extern void cpu_mem_write_u16(CPU*, unsigned int pos, unsigned short data);

extern unsigned int cpu_mem_read_u24(CPU*, unsigned int pos);

extern void cpu_adc(enum AddressingMode, CPU*);
extern void cpu_sbc(enum AddressingMode, CPU*);

extern void cpu_cmp(enum AddressingMode, CPU*);
extern void cpu_cpx(enum AddressingMode, CPU*);
extern void cpu_cpy(enum AddressingMode, CPU*);

extern void cpu_dec(enum AddressingMode, CPU*);
extern void cpu_dex(enum AddressingMode, CPU*);
extern void cpu_dey(enum AddressingMode, CPU*);

extern void cpu_inc(enum AddressingMode, CPU*);
extern void cpu_inx(enum AddressingMode, CPU*);
extern void cpu_iny(enum AddressingMode, CPU*);

extern void cpu_and(enum AddressingMode, CPU*);
extern void cpu_eor(enum AddressingMode, CPU*);
extern void cpu_ora(enum AddressingMode, CPU*);

extern void cpu_bit(enum AddressingMode, CPU*);

extern void cpu_trb(enum AddressingMode, CPU*);
extern void cpu_tsb(enum AddressingMode, CPU*);

extern void cpu_asl(enum AddressingMode, CPU*);
extern void cpu_lsr(enum AddressingMode, CPU*);
extern void cpu_rol(enum AddressingMode, CPU*);
extern void cpu_ror(enum AddressingMode, CPU*);

extern void cpu_bcc(enum AddressingMode, CPU*);
extern void cpu_bcs(enum AddressingMode, CPU*);
extern void cpu_beq(enum AddressingMode, CPU*);
extern void cpu_bmi(enum AddressingMode, CPU*);
extern void cpu_bne(enum AddressingMode, CPU*);
extern void cpu_bpl(enum AddressingMode, CPU*);
extern void cpu_bra(enum AddressingMode, CPU*);
extern void cpu_bvc(enum AddressingMode, CPU*);
extern void cpu_bvs(enum AddressingMode, CPU*);

extern void cpu_brl(enum AddressingMode, CPU*);

extern void cpu_jmp(enum AddressingMode, CPU*);
extern void cpu_jsl(enum AddressingMode, CPU*);
extern void cpu_jsr(enum AddressingMode, CPU*);

extern void cpu_rtl(enum AddressingMode, CPU*);
extern void cpu_rts(enum AddressingMode, CPU*);

extern void cpu_brk(enum AddressingMode, CPU*);
extern void cpu_cop(enum AddressingMode, CPU*);

extern void cpu_abt(enum AddressingMode, CPU*);
extern void cpu_irq(enum AddressingMode, CPU*);
extern void cpu_nmi(enum AddressingMode, CPU*);
extern void cpu_rst(enum AddressingMode, CPU*);

extern void cpu_rti(enum AddressingMode, CPU*);

extern void cpu_clc(enum AddressingMode, unsigned char*);
extern void cpu_cld(enum AddressingMode, unsigned char*);
extern void cpu_cli(enum AddressingMode, unsigned char*);
extern void cpu_clv(enum AddressingMode, unsigned char*);
extern void cpu_sec(enum AddressingMode, unsigned char*);
extern void cpu_sed(enum AddressingMode, unsigned char*);
extern void cpu_sei(enum AddressingMode, unsigned char*);

extern void cpu_rep(enum AddressingMode, CPU*);
extern void cpu_sep(enum AddressingMode, CPU*);

extern void cpu_lda(enum AddressingMode, CPU*);
extern void cpu_ldx(enum AddressingMode, CPU*);
extern void cpu_ldy(enum AddressingMode, CPU*);

extern void cpu_sta(enum AddressingMode, CPU*);
extern void cpu_stx(enum AddressingMode, CPU*);
extern void cpu_sty(enum AddressingMode, CPU*);
extern void cpu_stz(enum AddressingMode, CPU*);

extern void cpu_mvn(enum AddressingMode, CPU*);
extern void cpu_mvp(enum AddressingMode, CPU*);

extern void cpu_nop(enum AddressingMode);
extern void cpu_wdm(enum AddressingMode);

extern void cpu_pea(enum AddressingMode, CPU*);
extern void cpu_pei(enum AddressingMode, CPU*);
extern void cpu_per(enum AddressingMode, CPU*);

extern void cpu_pha(enum AddressingMode, CPU*);
extern void cpu_phx(enum AddressingMode, CPU*);
extern void cpu_phy(enum AddressingMode, CPU*);
extern void cpu_pla(enum AddressingMode, CPU*);
extern void cpu_plx(enum AddressingMode, CPU*);
extern void cpu_ply(enum AddressingMode, CPU*);

extern void cpu_phb(enum AddressingMode, CPU*);
extern void cpu_phd(enum AddressingMode, CPU*);
extern void cpu_phk(enum AddressingMode, CPU*);
extern void cpu_php(enum AddressingMode, CPU*);
extern void cpu_plb(enum AddressingMode, CPU*);
extern void cpu_pld(enum AddressingMode, CPU*);
extern void cpu_plp(enum AddressingMode, CPU*);

extern void cpu_stp(enum AddressingMode, CPU*);
extern void cpu_wai(enum AddressingMode, CPU*);

extern void cpu_tax(enum AddressingMode, CPU*);
extern void cpu_tay(enum AddressingMode, CPU*);
extern void cpu_tsx(enum AddressingMode, CPU*);
extern void cpu_txa(enum AddressingMode, CPU*);
extern void cpu_txs(enum AddressingMode, CPU*);
extern void cpu_txy(enum AddressingMode, CPU*);
extern void cpu_tya(enum AddressingMode, CPU*);
extern void cpu_tyx(enum AddressingMode, CPU*);

extern void cpu_tcd(enum AddressingMode, CPU*);
extern void cpu_tcs(enum AddressingMode, CPU*);
extern void cpu_tdc(enum AddressingMode, CPU*);
extern void cpu_tsc(enum AddressingMode, CPU*);

extern void cpu_xba(enum AddressingMode, CPU*);

extern void cpu_xce(enum AddressingMode, CPU*);

extern void cpu_set_overflow_flag_u8(unsigned char *status, unsigned char reg, 
                                    unsigned char arg, unsigned char result);

extern void cpu_set_overflow_flag_u16(unsigned char *status, unsigned short reg, 
                                    unsigned short arg, unsigned short result);

extern void cpu_set_zero_flag(unsigned char *status, unsigned short result);

extern void cpu_set_negative_u8(unsigned char *status, unsigned char result);
extern void cpu_set_negative_u16(unsigned char *status, unsigned short result);

extern void cpu_interpret(CPU*);
