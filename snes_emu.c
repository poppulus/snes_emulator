#include "snes_emu.h"


void cpu_adc(enum AddressingMode mode)
{

}
void cpu_sbc(enum AddressingMode mode)
{

}

void cpu_cmp(enum AddressingMode mode)
{

}
void cpu_cpx(enum AddressingMode mode)
{

}
void cpu_cpy(enum AddressingMode mode)
{

}

void cpu_dec(enum AddressingMode mode)
{

}
void cpu_dex(enum AddressingMode mode)
{

}
void cpu_dey(enum AddressingMode mode)
{

}

void cpu_inc(enum AddressingMode mode)
{

}
void cpu_inx(enum AddressingMode mode)
{

}
void cpu_iny(enum AddressingMode mode)
{

}

void cpu_and(enum AddressingMode mode)
{

}
void cpu_eor(enum AddressingMode mode)
{

}
void cpu_ora(enum AddressingMode mode)
{

}

void cpu_bit(enum AddressingMode mode)
{

}

void cpu_trb(enum AddressingMode mode)
{

}
void cpu_tsb(enum AddressingMode mode)
{

}

void cpu_asl(enum AddressingMode mode)
{

}
void cpu_lsr(enum AddressingMode mode)
{
    
}
void cpu_rol(enum AddressingMode mode)
{
    
}
void cpu_ror(enum AddressingMode mode)
{
    
}

void cpu_bcc(enum AddressingMode mode)
{

}
void cpu_bcs(enum AddressingMode mode)
{
    
}
void cpu_beq(enum AddressingMode mode)
{
    
}
void cpu_bmi(enum AddressingMode mode)
{
    
}
void cpu_bne(enum AddressingMode mode)
{
    
}
void cpu_bpl(enum AddressingMode mode)
{
    
}
void cpu_bra(enum AddressingMode mode)
{
    
}
void cpu_bvc(enum AddressingMode mode)
{
    
}
void cpu_bvs(enum AddressingMode mode)
{
    
}

void cpu_brl(enum AddressingMode mode)
{

}

void cpu_jmp(enum AddressingMode mode)
{

}
void cpu_jsl(enum AddressingMode mode)
{

}
void cpu_jsr(enum AddressingMode mode)
{

}

void cpu_rtl(enum AddressingMode mode)
{

}
void cpu_rts(enum AddressingMode mode)
{

}

void cpu_brk(enum AddressingMode mode)
{

}
void cpu_cop(enum AddressingMode mode)
{

}

void cpu_abt(enum AddressingMode mode)
{

}
void cpu_irq(enum AddressingMode mode)
{

}
void cpu_nmi(enum AddressingMode mode)
{

}
void cpu_rst(enum AddressingMode mode)
{

}

void cpu_rti(enum AddressingMode mode)
{

}

void cpu_clc(enum AddressingMode mode)
{

}
void cpu_cld(enum AddressingMode mode)
{
    
}
void cpu_cli(enum AddressingMode mode)
{
    
}
void cpu_clv(enum AddressingMode mode)
{
    
}
void cpu_sec(enum AddressingMode mode)
{
    
}
void cpu_sed(enum AddressingMode mode)
{
    
}
void cpu_sei(enum AddressingMode mode)
{
    
}

void cpu_rep(enum AddressingMode mode)
{

}
void cpu_sep(enum AddressingMode mode)
{

}



void cpu_lda(enum AddressingMode mode)
{

}
void cpu_ldx(enum AddressingMode mode)
{

}
void cpu_ldy(enum AddressingMode mode)
{

}

void cpu_sta(enum AddressingMode mode)
{

}
void cpu_stx(enum AddressingMode mode)
{

}
void cpu_sty(enum AddressingMode mode)
{

}
void cpu_stz(enum AddressingMode mode)
{

}

void cpu_mvn(enum AddressingMode mode)
{

}
void cpu_mvp(enum AddressingMode mode)
{

}

void cpu_nop(enum AddressingMode mode)
{

}
void cpu_wdm(enum AddressingMode mode)
{

}

void cpu_pea(enum AddressingMode mode)
{

}
void cpu_pei(enum AddressingMode mode)
{

}
void cpu_per(enum AddressingMode mode)
{

}

void cpu_pha(enum AddressingMode mode)
{

}
void cpu_phx(enum AddressingMode mode)
{

}
void cpu_phy(enum AddressingMode mode)
{

}
void cpu_pla(enum AddressingMode mode)
{

}
void cpu_plx(enum AddressingMode mode)
{

}
void cpu_ply(enum AddressingMode mode)
{

}

void cpu_phb(enum AddressingMode mode)
{

}
void cpu_phd(enum AddressingMode mode)
{

}
void cpu_phk(enum AddressingMode mode)
{

}
void cpu_php(enum AddressingMode mode)
{

}
void cpu_plb(enum AddressingMode mode)
{

}
void cpu_pld(enum AddressingMode mode)
{

}
void cpu_plp(enum AddressingMode mode)
{
    
}

void cpu_stp(enum AddressingMode mode)
{

}
void cpu_wai(enum AddressingMode mode)
{

}

void cpu_tax(enum AddressingMode mode)
{

}
void cpu_tay(enum AddressingMode mode)
{

}
void cpu_tsx(enum AddressingMode mode)
{

}
void cpu_txa(enum AddressingMode mode)
{

}
void cpu_txs(enum AddressingMode mode)
{

}
void cpu_txy(enum AddressingMode mode)
{

}
void cpu_tya(enum AddressingMode mode)
{

}
void cpu_tyx(enum AddressingMode mode)
{
    
}

void cpu_tcd(enum AddressingMode mode)
{

}
void cpu_tcs(enum AddressingMode mode)
{

}
void cpu_tdc(enum AddressingMode mode)
{

}
void cpu_tsc(enum AddressingMode mode)
{

}

void cpu_xba(enum AddressingMode mode)
{

}

void cpu_xce(enum AddressingMode mode)
{

}



void cpu_set_zero_flag(unsigned char *status, unsigned short result)
{
    if (result == 0)    *status |= ZERO_FLAG;
    else                *status &= 0b11111101;
}

void cpu_set_mem_negative(unsigned char *status, unsigned short result)
{
    if (*status & MEMORY_WIDTH)
    {
        if (result & 0b10000000)
            *status |= NEGATIVE_FLAG;
        else 
            *status &= 0b011111111;
    }
    else
    {
        if (result & 0b1000000000000000)
            *status |= NEGATIVE_FLAG;
        else 
            *status &= 0b011111111;
    }
}

void cpu_set_idx_negative(unsigned char *status, unsigned short result)
{
    if (*status & INDEX_WIDTH)
    {
        if (result & 0b10000000)
            *status |= NEGATIVE_FLAG;
        else 
            *status &= 0b011111111;
    }
    else
    {
        if (result & 0b1000000000000000)
            *status |= NEGATIVE_FLAG;
        else 
            *status &= 0b011111111;
    }
}

void cpu_set_mem_flag(unsigned char *status)
{
    *status |= MEMORY_WIDTH;
}

void cpu_set_idx_flag(unsigned char *status, unsigned short *reg)
{
    *status |= INDEX_WIDTH;
    *reg &= 0xFF;
}

void cpu_interpret(CPU *cpu)
{
    unsigned char operand = 0;


    switch (operand)
    {
        case 0x00:
            cpu_brk(IMPLIED);
            break;
        case 0x01:
            cpu_ora(DIRECT_SHORT_X);
            break;
        case 0x02:
            cpu_cop(IMMEDIATE);
            break;
        case 0x03:
            cpu_ora(STACK_S);
            break;
        case 0x04:
            cpu_trb(DIRECT);
            break;
        case 0x05:
            cpu_ora(DIRECT);
            break;
        case 0x06:
            cpu_asl(DIRECT);
            break;
        case 0x07:
            cpu_ora(DIRECT_LONG);
            break;
        case 0x08:
            cpu_php(IMPLIED);
            break;
        case 0x09:
            cpu_ora(IMMEDIATE);
            break;
        case 0x0A:
            cpu_asl(ACCUMULATOR);
            break;
        case 0x0B:
            cpu_phd(IMPLIED);
            break;
        case 0x0C:
            cpu_tsb(ABSOLUTE);
            break;
        case 0x0D:
            cpu_ora(ABSOLUTE);
            break;
        case 0x0F:
            cpu_ora(LONG);
            break;
        case 0x10:
            cpu_bpl(RELATIVE_8);
            break;
        case 0x11:
            cpu_ora(DIRECT_SHORT_Y);
            break;
        case 0x12:
            cpu_ora(DIRECT_SHORT);
            break;
        case 0x13:
            cpu_ora(STACK_S_Y);
            break;
        case 0x14:
            cpu_trb(DIRECT);
            break;
        case 0x15:
            cpu_ora(DIRECT_X);
            break;
        case 0x16:
            
            break;
        case 0x17:
            cpu_ora(DIRECT_LONG_Y);
            break;
        case 0x18:
            cpu_clc(IMPLIED);
            break;
        case 0x19:
            cpu_ora(ABSOLUTE_Y);
            break;
        case 0x1A:
            cpu_inc(ACCUMULATOR);
            break;
        case 0x1B:
            cpu_tcs(IMPLIED);
            break;
        case 0x1C:
            cpu_trb(ABSOLUTE);
            break;
        case 0x1D:
            cpu_ora(ABSOLUTE_X);
            break;
        case 0x1E:
            cpu_asl(ABSOLUTE_X);
            break;
        case 0x1F:
            cpu_ora(LONG_X);
            break;
        case 0x20:
            cpu_jsr(ABSOLUTE);
            break;
        case 0x21:
            cpu_and(DIRECT_SHORT_X);
            break;
        case 0x22:
            cpu_jsl(LONG);
            break;
        case 0x23:
            cpu_and(STACK_S);
            break;
        case 0x24:
            cpu_bit(DIRECT);
            break;
        case 0x25:
            cpu_and(DIRECT);
            break;
        case 0x26:
            cpu_rol(DIRECT);
            break;
        case 0x27:
            cpu_and(DIRECT_LONG);
            break;
        case 0x28:
            cpu_plp(IMPLIED);
            break;
        case 0x29:
            cpu_and(IMMEDIATE);
            break;
        case 0x2A:
            cpu_rol(ACCUMULATOR);
            break;
        case 0x2B:
            cpu_pld(IMPLIED);
            break;
        case 0x2C:
            cpu_bit(ABSOLUTE);
            break;
        case 0x2D:
            cpu_and(ABSOLUTE);
            break;
        case 0x2E:
            cpu_rol(ABSOLUTE);
            break;
        case 0x2F:
            cpu_and(LONG);
            break;
        case 0x30:
            cpu_bmi(RELATIVE_8);
            break;
        case 0x31:
            cpu_and(DIRECT_SHORT_Y);
            break;
        case 0x32:
            cpu_and(DIRECT_SHORT);
            break;
        case 0x33:
            cpu_and(STACK_S_Y);
            break;
        case 0x34:
            cpu_bit(DIRECT_X);
            break;
        case 0x35:
            cpu_and(DIRECT_X);
            break;
        case 0x36:
            cpu_rol(DIRECT_X);
            break;
        case 0x37:
            cpu_and(DIRECT_LONG_Y);
            break;
        case 0x38:
            cpu_sec(IMPLIED);
            break;
        case 0x39:
            cpu_and(ABSOLUTE_Y);
            break;
        case 0x3A:
            cpu_dec(ACCUMULATOR);
            break;
        case 0x3B:
            cpu_tsc(IMPLIED);
            break;
        case 0x3C:
            cpu_bit(ABSOLUTE_X);
            break;
        case 0x3D:
            cpu_and(ABSOLUTE_X);
            break;
        case 0x3E:
            cpu_rol(ABSOLUTE_X);
            break;
        case 0x3F:
            cpu_and(LONG_X);
            break;
        case 0x40:
            cpu_rti(IMPLIED);
            break;
        case 0x41:
            cpu_eor(DIRECT_SHORT_X);
            break;
        case 0x42:
            cpu_wdm(IMMEDIATE);
            break;
        case 0x43:
            cpu_eor(STACK_S);
            break;
        case 0x44:
            cpu_mvp(SOURCE_DEST);
            break;
        case 0x45:
            cpu_eor(DIRECT);
            break;
        case 0x46:
            cpu_lsr(DIRECT);
            break;
        case 0x47:
            cpu_eor(DIRECT_LONG);
            break;
        case 0x48:
            cpu_pha(IMPLIED);
            break;
        case 0x49:
            cpu_eor(IMMEDIATE);
            break;
        case 0x4A:
            cpu_lsr(ACCUMULATOR);
            break;
        case 0x4B:
            cpu_phk(IMPLIED);
            break;
        case 0x4C:
            cpu_jmp(ABSOLUTE);
            break;
        case 0x4D:
            cpu_eor(ABSOLUTE);
            break;
        case 0x4E:
            cpu_lsr(ABSOLUTE);
            break;
        case 0x4F:
            cpu_eor(LONG);
            break;
        case 0x50:
            cpu_bvc(RELATIVE_8);
            break;
        case 0x51:
            cpu_eor(DIRECT_SHORT_Y);
            break;
        case 0x52:
            cpu_eor(DIRECT_SHORT);
            break;
        case 0x53:
            cpu_eor(STACK_S_Y);
            break;
        case 0x54:
            cpu_mvn(SOURCE_DEST);
            break;
        case 0x55:
            cpu_eor(DIRECT_X);
            break;
        case 0x56:
            cpu_lsr(DIRECT_X);
            break;
        case 0x57:
            cpu_eor(DIRECT_LONG_Y);
            break;
        case 0x58:
            cpu_cli(IMPLIED);
            break;
        case 0x59:
            cpu_eor(ABSOLUTE_Y);
            break;
        case 0x5A:
            cpu_phy(IMPLIED);
            break;
        case 0x5B:
            cpu_tcd(IMPLIED);
            break;
        case 0x5C:
            cpu_jmp(LONG);
            break;
        case 0x5D:
            cpu_eor(ABSOLUTE_X);
            break;
        case 0x5E:
            cpu_lsr(ABSOLUTE_X);
            break;
        case 0x5F:
            cpu_eor(LONG_X);
            break;
        case 0x60:
            cpu_rts(IMPLIED);
            break;
        case 0x61:
            cpu_adc(DIRECT_SHORT_X);
            break;
        case 0x62:
            cpu_per(IMMEDIATE);
            break;
        case 0x63:
            cpu_adc(STACK_S);
            break;
        case 0x64:
            cpu_stz(DIRECT);
            break;
        case 0x65:
            cpu_adc(DIRECT);
            break;
        case 0x66:
            cpu_ror(DIRECT);
            break;
        case 0x67:
            cpu_adc(DIRECT_LONG);
            break;
        case 0x68:
            cpu_pla(IMPLIED);
            break;
        case 0x69:
            cpu_adc(IMMEDIATE);
            break;
        case 0x6A:
            cpu_ror(ACCUMULATOR);
            break;
        case 0x6B:
            cpu_rtl(IMPLIED);
            break;
        case 0x6C:
            cpu_jmp(ABSOLUTE_JMP_LONG);
            break;
        case 0x6D:
            cpu_adc(ABSOLUTE);
            break;
        case 0x6E:
            cpu_ror(ABSOLUTE);
            break;
        case 0x6F:
            cpu_adc(LONG);
            break;
        case 0x70:
            cpu_bvs(RELATIVE_8);
            break;
        case 0x71:
            cpu_adc(DIRECT_SHORT_Y);
            break;
        case 0x72:
            cpu_adc(DIRECT_SHORT);
            break;
        case 0x73:
            cpu_adc(STACK_S_Y);
            break;
        case 0x74:
            cpu_stz(DIRECT_X);
            break;
        case 0x75:
            cpu_adc(DIRECT_X);
            break;
        case 0x76:
            cpu_ror(DIRECT_X);
            break;
        case 0x77:
            cpu_adc(DIRECT_LONG_Y);
            break;
        case 0x78:
            cpu_sei(IMPLIED);
            break;
        case 0x79:
            cpu_adc(ABSOLUTE_Y);
            break;
        case 0x7A:
            cpu_ply(IMPLIED);
            break;
        case 0x7B:
            cpu_tdc(IMPLIED);
            break;
        case 0x7C:
            cpu_jmp(ABSOLUTE_JMP_X);
            break;
        case 0x7D:
            cpu_adc(ABSOLUTE_X);
            break;
        case 0x7E:
            cpu_ror(ABSOLUTE_X);
            break;
        case 0x7F:
            cpu_adc(LONG_X);
            break;
        case 0x80:
            cpu_bra(RELATIVE_8);
            break;
        case 0x81:
            cpu_sta(DIRECT_SHORT_X);
            break;
        case 0x82:
            cpu_brl(RELATIVE_16);
            break;
        case 0x83:
            cpu_sta(STACK_S);
            break;
        case 0x84:
            cpu_sty(DIRECT);
            break;
        case 0x85:
            cpu_sta(DIRECT);
            break;
        case 0x86:
            cpu_stx(DIRECT);
            break;
        case 0x87:
            cpu_sta(DIRECT_LONG);
            break;
        case 0x88:
            cpu_dey(IMPLIED);
            break;
        case 0x89:
            cpu_bit(IMMEDIATE);
            break;
        case 0x8A:
            cpu_txa(IMPLIED);
            break;
        case 0x8B:
            cpu_phb(IMPLIED);
            break;
        case 0x8C:
            cpu_sty(ABSOLUTE);
            break;
        case 0x8D:
            cpu_sta(ABSOLUTE);
            break;
        case 0x8E:
            cpu_stx(ABSOLUTE);
            break;
        case 0x8F:
            cpu_sta(LONG);
            break;
        case 0x90:
            cpu_bcc(RELATIVE_8);
            break;
        case 0x91:
            cpu_sta(DIRECT_SHORT_Y);
            break;
        case 0x92:
            cpu_sta(DIRECT_SHORT);
            break;
        case 0x93:
            cpu_sta(STACK_S_Y);
            break;
        case 0x94:
            cpu_sty(DIRECT_X);
            break;
        case 0x95:
            cpu_sta(DIRECT_X);
            break;
        case 0x96:
            cpu_stx(DIRECT_Y);
            break;
        case 0x97:
            cpu_sta(DIRECT_LONG_Y);
            break;
        case 0x98:
            cpu_tya(IMPLIED);
            break;
        case 0x99:
            cpu_sta(ABSOLUTE_Y);
            break;
        case 0x9A:
            cpu_txs(IMPLIED);
            break;
        case 0x9B:
            cpu_txy(IMPLIED);
            break;
        case 0x9C:
            cpu_stz(ABSOLUTE);
            break;
        case 0x9D:
            cpu_sta(ABSOLUTE_X);
            break;
        case 0x9E:
            cpu_stz(ABSOLUTE_X);
            break;
        case 0x9F:
            cpu_sta(LONG_X);
            break;
        case 0xA0:
            cpu_ldy(IMMEDIATE);
            break;
        case 0xA1:
            cpu_lda(DIRECT_SHORT_X);
            break;
        case 0xA2:
            cpu_ldx(IMMEDIATE);
            break;
        case 0xA3:
            cpu_lda(STACK_S);
            break;
        case 0xA4:
            cpu_ldy(DIRECT);
            break;
        case 0xA5:
            cpu_lda(DIRECT);
            break;
        case 0xA6:
            cpu_ldx(DIRECT);
            break;
        case 0xA7:
            cpu_lda(DIRECT_LONG);
            break;
        case 0xA8:
            cpu_tay(IMPLIED);
            break;
        case 0xA9:
            cpu_lda(IMMEDIATE);
            break;
        case 0xAA:
            cpu_tax(IMPLIED);
            break;
        case 0xAB:
            cpu_plb(IMPLIED);
            break;
        case 0xAC:
            cpu_ldy(ABSOLUTE);
            break;
        case 0xAD:
            cpu_lda(ABSOLUTE);
            break;
        case 0xAE:
            cpu_ldx(ABSOLUTE);
            break;
        case 0xAF:
            cpu_lda(LONG);
            break;
        case 0xB0:
            cpu_bcs(RELATIVE_8);
            break;
        case 0xB1:
            cpu_lda(DIRECT_SHORT_Y);
            break;
        case 0xB2:
            cpu_lda(DIRECT_SHORT);
            break;
        case 0xB3:
            cpu_lda(STACK_S_Y);
            break;
        case 0xB4:
            cpu_ldy(DIRECT_X);
            break;
        case 0xB5:
            cpu_lda(DIRECT_X);
            break;
        case 0xB6:
            cpu_ldx(DIRECT_Y);
            break;
        case 0xB7:
            cpu_lda(DIRECT_LONG_Y);
            break;
        case 0xB8:
            cpu_clv(IMPLIED);
            break;
        case 0xB9:
            cpu_lda(ABSOLUTE_Y);
            break;
        case 0xBA:
            cpu_tsx(IMPLIED);
            break;
        case 0xBB:
            cpu_tyx(IMPLIED);
            break;
        case 0xBC:
            cpu_ldy(ABSOLUTE_X);
            break;
        case 0xBD:
            cpu_lda(ABSOLUTE_X);
            break;
        case 0xBE:
            cpu_ldx(ABSOLUTE_Y);
            break;
        case 0xBF:
            cpu_lda(LONG_X);
            break;
        case 0xC0:
            cpu_cpy(IMMEDIATE);
            break;
        case 0xC1:
            cpu_cmp(DIRECT_SHORT_X);
            break;
        case 0xC2:
            
            break;
        case 0xC3:
            cpu_cmp(STACK_S);
            break;
        case 0xC4:

            break;
        case 0xC5:
            cpu_cmp(DIRECT);
            break;
        case 0xC6:

            break;
        case 0xC7:
            cpu_cmp(DIRECT_LONG);
            break;
        case 0xC8:

            break;
        case 0xC9:
            cpu_cmp(IMMEDIATE);
            break;
        case 0xCA:

            break;
        case 0xCB:

            break;
        case 0xCC:

            break;
        case 0xCD:
            cpu_cmp(ABSOLUTE);
            break;
        case 0xCE:

            break;
        case 0xCF:
            cpu_cmp(LONG);
            break;
        case 0xD0:

            break;
    }
}
