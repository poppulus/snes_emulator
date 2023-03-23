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
    }
}
