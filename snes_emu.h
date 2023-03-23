
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
    BREAK_FLAG      = 0b00010000,
    CARRY_FLAG      = 0b00000001,
    DECIMAL_MODE    = 0b00001000,
    EMULATION_MODE  = 0b00000001,
    INTERRUPT_FLAG  = 0b00000100,
    MEMORY_WIDTH    = 0b00100000,
    NEGATIVE_FLAG   = 0b10000000,
    OVERFLOW_FLAG   = 0b01000000,
    INDEX_WIDTH     = 0b00010000,
    ZERO_FLAG       = 0b00000010
};

typedef struct 
{
    unsigned int    cycles;

    unsigned short  register_a, register_x, register_y, 
                    register_direct, program_counter, stack_pointer;

    unsigned char   data_bank, program_bank, status;

    unsigned char   emulation_mode:1;
} CPU;

extern void cpu_adc(enum AddressingMode);
extern void cpu_sbc(enum AddressingMode);

extern void cpu_cmp(enum AddressingMode);
extern void cpu_cpx(enum AddressingMode);
extern void cpu_cpy(enum AddressingMode);

extern void cpu_dec(enum AddressingMode);
extern void cpu_dex(enum AddressingMode);
extern void cpu_dey(enum AddressingMode);

extern void cpu_inc(enum AddressingMode);
extern void cpu_inx(enum AddressingMode);
extern void cpu_iny(enum AddressingMode);

extern void cpu_and(enum AddressingMode);
extern void cpu_eor(enum AddressingMode);
extern void cpu_ora(enum AddressingMode);

extern void cpu_bit(enum AddressingMode);

extern void cpu_trb(enum AddressingMode);
extern void cpu_tsb(enum AddressingMode);

extern void cpu_asl(enum AddressingMode);
extern void cpu_lsr(enum AddressingMode);
extern void cpu_rol(enum AddressingMode);
extern void cpu_ror(enum AddressingMode);

extern void cpu_bcc(enum AddressingMode);
extern void cpu_bcs(enum AddressingMode);
extern void cpu_beq(enum AddressingMode);
extern void cpu_bmi(enum AddressingMode);
extern void cpu_bne(enum AddressingMode);
extern void cpu_bpl(enum AddressingMode);
extern void cpu_bra(enum AddressingMode);
extern void cpu_bvc(enum AddressingMode);
extern void cpu_bvs(enum AddressingMode);

extern void cpu_brl(enum AddressingMode);

extern void cpu_jmp(enum AddressingMode);
extern void cpu_jsl(enum AddressingMode);
extern void cpu_jsr(enum AddressingMode);

extern void cpu_rtl(enum AddressingMode);
extern void cpu_rts(enum AddressingMode);

extern void cpu_brk(enum AddressingMode);
extern void cpu_cop(enum AddressingMode);

extern void cpu_abt(enum AddressingMode);
extern void cpu_irq(enum AddressingMode);
extern void cpu_nmi(enum AddressingMode);
extern void cpu_rst(enum AddressingMode);

extern void cpu_rti(enum AddressingMode);

extern void cpu_clc(enum AddressingMode);
extern void cpu_cld(enum AddressingMode);
extern void cpu_cli(enum AddressingMode);
extern void cpu_clv(enum AddressingMode);
extern void cpu_sec(enum AddressingMode);
extern void cpu_sed(enum AddressingMode);
extern void cpu_sei(enum AddressingMode);

extern void cpu_rep(enum AddressingMode);
extern void cpu_sep(enum AddressingMode);

extern void cpu_lda(enum AddressingMode);
extern void cpu_ldx(enum AddressingMode);
extern void cpu_ldy(enum AddressingMode);

extern void cpu_sta(enum AddressingMode);
extern void cpu_stx(enum AddressingMode);
extern void cpu_sty(enum AddressingMode);
extern void cpu_stz(enum AddressingMode);

extern void cpu_mvn(enum AddressingMode);
extern void cpu_mvp(enum AddressingMode);

extern void cpu_nop(enum AddressingMode);
extern void cpu_wdm(enum AddressingMode);

extern void cpu_pea(enum AddressingMode);
extern void cpu_pei(enum AddressingMode);
extern void cpu_per(enum AddressingMode);

extern void cpu_pha(enum AddressingMode);
extern void cpu_phx(enum AddressingMode);
extern void cpu_phy(enum AddressingMode);
extern void cpu_pla(enum AddressingMode);
extern void cpu_plx(enum AddressingMode);
extern void cpu_ply(enum AddressingMode);

extern void cpu_phb(enum AddressingMode);
extern void cpu_phd(enum AddressingMode);
extern void cpu_phk(enum AddressingMode);
extern void cpu_php(enum AddressingMode);
extern void cpu_plb(enum AddressingMode);
extern void cpu_pld(enum AddressingMode);
extern void cpu_plp(enum AddressingMode);

extern void cpu_stp(enum AddressingMode);
extern void cpu_wai(enum AddressingMode);

extern void cpu_tax(enum AddressingMode);
extern void cpu_tay(enum AddressingMode);
extern void cpu_tsx(enum AddressingMode);
extern void cpu_txa(enum AddressingMode);
extern void cpu_txs(enum AddressingMode);
extern void cpu_txy(enum AddressingMode);
extern void cpu_tya(enum AddressingMode);
extern void cpu_tyx(enum AddressingMode);

extern void cpu_tcd(enum AddressingMode);
extern void cpu_tcs(enum AddressingMode);
extern void cpu_tdc(enum AddressingMode);
extern void cpu_tsc(enum AddressingMode);

extern void cpu_xba(enum AddressingMode);

extern void cpu_xce(enum AddressingMode);

extern void cpu_set_zero_flag(unsigned char *status, unsigned short result);

extern void cpu_set_mem_negative(unsigned char *status, unsigned short result);
extern void cpu_set_idx_negative(unsigned char *status, unsigned short result);

extern void cpu_set_mem_flag(unsigned char *status);
extern void cpu_set_idx_flag(unsigned char *status, unsigned short *reg);

extern void cpu_interpret(CPU *cpu);
