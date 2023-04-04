#include "snes_emu.h"

unsigned int cpu_get_operand_address(CPU *cpu, enum AddressingMode mode)
{
    return 0;
}

unsigned char cpu_mem_read_u8(CPU *cpu, unsigned int pos)
{
    return cpu->wram[pos];
}
void cpu_mem_write_u8(CPU *cpu, unsigned int pos, unsigned char data)
{
    cpu->wram[pos] = data;
}

unsigned short cpu_mem_read_u16(CPU *cpu, unsigned int pos)
{
    unsigned short  lo = cpu->wram[pos], 
                    hi = cpu->wram[pos + 1];

    return (hi << 8) | lo;
}

void cpu_mem_write_u16(CPU *cpu, unsigned int pos, unsigned short data)
{
    unsigned char   hi = data >> 8, 
                    lo = data & 0xFF;

    cpu->wram[pos] = lo;
    cpu->wram[pos + 1] = hi;
}

void cpu_adc(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  arg =  cpu_mem_read_u16(cpu, addr);

    int             sum = cpu->register_a + arg + (cpu->status & CARRY_FLAG);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char   rega_arg = (unsigned char)cpu->register_a ^ cpu->wram[addr],
                        rega_sum = (unsigned char)cpu->register_a ^ (unsigned char)sum;

        if (~rega_arg & rega_sum & 0x80)
            cpu->status = cpu->status | OVERFLOW_FLAG;
        else 
            cpu->status = cpu->status & 0b10111111;

        cpu->register_a = sum & 0xFF;

        if (sum > 0xFF) cpu_sec(mode, &cpu->status);
        else cpu_clc(mode, &cpu->status);

        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_a);
        cpu_set_zero_flag_u8(&cpu->status, (unsigned char)cpu->register_a);
    }
    else
    {
        if (~(cpu->register_a ^ arg) & (cpu->register_a ^ sum) & 0x8000)
            cpu->status = cpu->status | OVERFLOW_FLAG;
        else 
            cpu->status = cpu->status & 0b10111111;

        cpu->register_a = sum & 0xFFFF;

        if (sum > 0xFFFF) cpu_sec(mode, &cpu->status);
        else cpu_clc(mode, &cpu->status);

        cpu_set_negative_16bit(&cpu->status, cpu->register_a);
        cpu_set_zero_flag_u16(&cpu->status, cpu->register_a);
    }
}
void cpu_sbc(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  arg = cpu->wram[addr] ^ 0xFFFF;

    int             sum = cpu->register_a + arg + (cpu->status & CARRY_FLAG);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char   rega_arg = (unsigned char)cpu->register_a ^ (unsigned char)arg,
                        rega_sum = (unsigned char)cpu->register_a ^ (unsigned char)sum;

        if (~rega_arg & rega_sum & 0x80)
            cpu->status = cpu->status | OVERFLOW_FLAG;
        else 
            cpu->status = cpu->status & 0b10111111;

        cpu->register_a = sum & 0xFF;

        if (sum > 0xFF) cpu_sec(mode, &cpu->status);
        else cpu_clc(mode, &cpu->status);

        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_a);
        cpu_set_zero_flag_u8(&cpu->status, (unsigned char)cpu->register_a);
    }
    else
    {
        if (~(cpu->register_a ^ arg) & (cpu->register_a ^ sum) & 0x8000)
            cpu->status = cpu->status | OVERFLOW_FLAG;
        else 
            cpu->status = cpu->status & 0b10111111;

        cpu->register_a = sum & 0xFFFF;

        if (sum > 0xFFFF) cpu_sec(mode, &cpu->status);
        else cpu_clc(mode, &cpu->status);

        cpu_set_negative_16bit(&cpu->status, cpu->register_a);
        cpu_set_zero_flag_u16(&cpu->status, cpu->register_a);
    }
}

void cpu_cmp(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    unsigned short result;

    if (cpu->emulation_mode)
    {
        if ((unsigned char)cpu->register_a >= cpu->wram[addr])
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = (unsigned char)cpu->register_a - cpu->wram[addr];
    }
    else
    {
        if (cpu->register_a >= cpu_mem_read_u16(cpu, addr))
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = cpu->register_a - cpu_mem_read_u16(cpu, addr);
    }

    cpu_set_mem_zero_flag(&cpu->status, result);
    cpu_set_mem_negative(&cpu->status, result);
}
void cpu_cpx(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    unsigned short result;

    if (cpu->emulation_mode)
    {
        if ((unsigned char)cpu->register_x >= cpu->wram[addr])
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = (unsigned char)cpu->register_x - cpu->wram[addr];
    }
    else
    {
        if (cpu->register_x >= cpu_mem_read_u16(cpu, addr))
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = cpu->register_x - cpu_mem_read_u16(cpu, addr);
    }

    cpu_set_mem_zero_flag(&cpu->status, result);
    cpu_set_mem_negative(&cpu->status, result);
}
void cpu_cpy(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    unsigned short result;

    if (cpu->emulation_mode)
    {
        if ((unsigned char)cpu->register_y >= cpu->wram[addr])
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = (unsigned char)cpu->register_y - cpu->wram[addr];
    }
    else
    {
        if (cpu->register_y >= cpu_mem_read_u16(cpu, addr))
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = cpu->register_y - cpu_mem_read_u16(cpu, addr);
    }

    cpu_set_mem_zero_flag(&cpu->status, result);
    cpu_set_mem_negative(&cpu->status, result);
}

void cpu_dec(enum AddressingMode mode, CPU *cpu)
{
    unsigned short result;

    if (mode == ACCUMULATOR)
    {
        if (cpu->emulation_mode)
            cpu->register_a = (unsigned char)cpu->register_a - 1;
        else
            cpu->register_a -= 1;

        result = cpu->register_a;
    }
    else
    {
        unsigned int addr = cpu_get_operand_address(cpu, mode);

        if (cpu->emulation_mode)
            cpu->wram[addr] -= 1;
        else
            cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) - 1);

        result = cpu_mem_read_u16(cpu, addr);
    }

    cpu_set_mem_zero_flag(&cpu->status, result);
    cpu_set_mem_negative(&cpu->status, result);
}
void cpu_dex(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
        cpu->register_x = (unsigned char)cpu->register_x - 1;
    else
        cpu->register_x -= 1;

    cpu_set_mem_zero_flag(&cpu->status, cpu->register_x);
    cpu_set_idx_negative(&cpu->status, cpu->register_x);
}
void cpu_dey(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
        cpu->register_y = (unsigned char)cpu->register_y - 1;
    else
        cpu->register_y -= 1;

    cpu_set_mem_zero_flag(&cpu->status, cpu->register_y);
    cpu_set_idx_negative(&cpu->status, cpu->register_y);
}

void cpu_inc(enum AddressingMode mode, CPU *cpu)
{
    unsigned short result;

    if (mode == ACCUMULATOR)
    {
        if (cpu->emulation_mode)
            cpu->register_a = (unsigned char)cpu->register_a + 1;
        else
            cpu->register_a += 1;

        result = cpu->register_a;
    }
    else
    {
        unsigned int addr = cpu_get_operand_address(cpu, mode);

        if (cpu->emulation_mode)
            cpu->wram[addr] += 1;
        else
            cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) + 1);

        result = cpu_mem_read_u16(cpu, addr);
    }

    cpu_set_mem_zero_flag(&cpu->status, result);
    cpu_set_mem_negative(&cpu->status, result);
}
void cpu_inx(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
        cpu->register_x = (unsigned char)cpu->register_x + 1;
    else
        cpu->register_x -= 1;

    cpu_set_idx_zero_flag(&cpu->status, cpu->register_x);
    cpu_set_idx_negative(&cpu->status, cpu->register_x);
}
void cpu_iny(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
        cpu->register_y = (unsigned char)cpu->register_y + 1;
    else
        cpu->register_y += 1;

    cpu_set_idx_zero_flag(&cpu->status, cpu->register_y);
    cpu_set_idx_negative(&cpu->status, cpu->register_y);
}

void cpu_and(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->emulation_mode)
        cpu->register_a = (cpu->register_a & cpu->wram[addr]);
    else
        cpu->register_a &= cpu_mem_read_u16(cpu, addr);

    cpu_set_mem_zero_flag(&cpu->status, cpu->register_a);
    cpu_set_mem_negative(&cpu->status, cpu->register_a);
}
void cpu_eor(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->emulation_mode)
        cpu->register_a = (unsigned char)cpu->register_a ^ cpu->wram[addr];
    else
        cpu->register_a ^= cpu_mem_read_u16(cpu, addr);

    cpu_set_mem_zero_flag(&cpu->status, cpu->register_a);
    cpu_set_mem_negative(&cpu->status, cpu->register_a);
}
void cpu_ora(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->emulation_mode)
        cpu->register_a = (unsigned char)cpu->register_a | cpu->wram[addr];
    else
        cpu->register_a |= cpu_mem_read_u16(cpu, addr);

    cpu_set_mem_zero_flag(&cpu->status, cpu->register_a);
    cpu_set_mem_negative(&cpu->status, cpu->register_a);
}

void cpu_bit(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode); 
    unsigned short  result;

    if (cpu->status & MEMORY_WIDTH)
    {
        result = (unsigned char)cpu->register_a & cpu->wram[addr];

        switch (mode)
        {
            default: break;
            case DIRECT:
            case ABSOLUTE:
            case DIRECT_X:
            case ABSOLUTE_X:
                cpu_set_negative_8bit(&cpu->status, result);
                cpu_set_overflow_flag_u8(&cpu->status, result);
                break;
        }

        cpu->cycles -= 1;
    }
    else
    {
        result = cpu->register_a & cpu_mem_read_u16(cpu, addr);

        switch (mode)
        {
            default: break;
            case DIRECT:
            case ABSOLUTE:
            case DIRECT_X:
            case ABSOLUTE_X:
                cpu_set_negative_16bit(&cpu->status, result);
                cpu_set_overflow_flag_u16(&cpu->status, result);
                break;
        }
    }

    if (mode == ABSOLUTE_X && cpu->status & INDEX_WIDTH)
        cpu->cycles -= 1;

    cpu_set_zero_flag(&cpu->status, result);
}

void cpu_trb(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  result;

    if (cpu->status & MEMORY_WIDTH)
    {
        result = (unsigned char)cpu->register_a & cpu->wram[addr];
        cpu->wram[addr] &= (unsigned char)~cpu->register_a;
        cpu->cycles -= 2;
    }
    else
    {
        result = cpu->register_a & cpu_mem_read_u16(cpu, addr);
        cpu_mem_write_u16(cpu, addr, cpu->register_a & ~cpu_mem_read_u16(cpu, addr));
    }

    cpu_set_zero_flag(&cpu->status, result);
}
void cpu_tsb(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  result;

    if (cpu->status & MEMORY_WIDTH)
    {
        result = (unsigned char)cpu->register_a & cpu->wram[addr];
        cpu->wram[addr] |= (unsigned char)cpu->register_a;
        cpu->cycles -= 2;
    }
    else
    {
        result = cpu->register_a & cpu_mem_read_u16(cpu, addr);
        cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) | cpu->register_a);
    }

    cpu_set_zero_flag(&cpu->status, result);
}

void cpu_asl(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char old_bits, result;

        if (mode == ACCUMULATOR)
        {
            old_bits = (unsigned char)cpu->register_a;
            cpu->register_a = (unsigned char)cpu->register_a << 1;
            result = (unsigned char)cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);

            old_bits = cpu->wram[addr];
            cpu->wram[addr] = cpu->wram[addr] << 1;
            result = cpu->wram[addr];

            cpu->cycles -= 2;
        }

        if ((old_bits & 0b10000000) != 0)
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u8(&cpu->status, result);
        cpu_set_negative_8bit(&cpu->status, result);
    }
    else
    {
        unsigned short old_bits, result;

        if (mode == ACCUMULATOR)
        {
            old_bits = cpu->register_a;
            cpu->register_a <<= 1;
            result = cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);

            old_bits = cpu_mem_read_u16(cpu, addr);
            cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) << 1);
            result = cpu_mem_read_u16(cpu, addr);
        }

        if ((old_bits & 0x8000) != 0)
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u16(&cpu->status, result);
        cpu_set_negative_16bit(&cpu->status, result);
    }
}
void cpu_lsr(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char old_bits, result;

        if (mode == ACCUMULATOR)
        {
            old_bits = (unsigned char)cpu->register_a;
            cpu->register_a = (unsigned char)cpu->register_a >> 1;
            cpu->register_a = (unsigned char)cpu->register_a & 0b01111111;
            result = (unsigned char)cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);

            old_bits = cpu->wram[addr];
            cpu->wram[addr] >>= 1;
            cpu->wram[addr] &= 0b01111111;
            result = cpu->wram[addr];

            cpu->cycles -= 2;
        }

        if ((old_bits & 0b10000000) != 0)
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u8(&cpu->status, result);
        cpu_set_negative_8bit(&cpu->status, result);
    }
    else
    {
        unsigned short old_bits, result;

        if (mode == ACCUMULATOR)
        {
            old_bits = cpu->register_a;
            cpu->register_a >>= 1;
            cpu->register_a &= 0x7FFF;
            result = cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);
            
            old_bits = cpu_mem_read_u16(cpu, addr);
            cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) >> 1);
            cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) & 0x7FFF);
            result = cpu_mem_read_u16(cpu, addr);
        }

        if ((old_bits & 0x8000) != 0)
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u16(&cpu->status, result);
        cpu_set_negative_16bit(&cpu->status, result);
    }
}
void cpu_rol(enum AddressingMode mode, CPU *cpu)
{
    unsigned char carry = cpu->status & CARRY_FLAG;

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char old_bits, shift, rotate, *result;

        if (mode == ACCUMULATOR)
        {
            old_bits = (unsigned char)cpu->register_a;
            shift = (unsigned char)cpu->register_a << 1;
            rotate = (unsigned char)cpu->register_a >> 7;

            cpu->register_a = (unsigned char)(shift | rotate);
            result = (unsigned char*)&cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);

            old_bits = cpu->wram[addr];
            shift = cpu->wram[addr] << 1;
            rotate = cpu->wram[addr] >> 7;

            cpu->wram[addr] = (unsigned char)(shift | rotate);
            result = &cpu->wram[addr];

            cpu->cycles -= 2;
        }

        if (carry)  
            *result = (unsigned char)(*result | CARRY_FLAG);
        else        
            *result = (unsigned char)(*result & 0b11111110);

        if (old_bits & 0b10000000) 
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u8(&cpu->status, *result);
        cpu_set_negative_8bit(&cpu->status, *result);
    }
    else
    {
        unsigned short old_bits, shift, rotate, result;

        if (mode == ACCUMULATOR)
        {
            old_bits = cpu->register_a;
            shift = cpu->register_a << 1;
            rotate = cpu->register_a >> 15;

            cpu->register_a = shift | rotate;

            if (carry)
                cpu->register_a |= CARRY_FLAG;
            else
                cpu->register_a &= 0xFFFE;

            result = cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);
            
            old_bits = cpu_mem_read_u16(cpu, addr);
            shift = cpu_mem_read_u16(cpu, addr) << 1;
            rotate = cpu_mem_read_u16(cpu, addr) >> 15;

            cpu_mem_write_u16(cpu, addr, shift | rotate);

            if (carry)
                cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) | 0x0001);
            else
                cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) & 0xFFFE);

            result = cpu_mem_read_u16(cpu, addr);
        }

        if (old_bits & 0x7FFF)  cpu->status = cpu->status | CARRY_FLAG;
        else                    cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u16(&cpu->status, result);
        cpu_set_negative_16bit(&cpu->status, result);
    }
}
void cpu_ror(enum AddressingMode mode, CPU *cpu)
{
    unsigned char carry = cpu->status & CARRY_FLAG;

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char old_bits, shift, rotate, result;

        if (mode == ACCUMULATOR)
        {
            unsigned char low_acc = cpu->register_a & 0xFF;

            old_bits = low_acc;
            shift = low_acc >> 1;
            rotate = low_acc << 7;

            cpu->register_a = (unsigned char)(shift | rotate);
            result = (unsigned char)cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);

            old_bits = cpu->wram[addr];
            shift = cpu->wram[addr] >> 1;
            rotate = cpu->wram[addr] << 7;

            cpu->wram[addr] = shift | rotate;
            result = cpu->wram[addr];

            cpu->cycles -= 2;
        }

        if (carry)  result |= 0b10000000;
        else        result &= 0b01111111;

        if (old_bits & 0b00000001) 
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u8(&cpu->status, result);
        cpu_set_negative_8bit(&cpu->status, result);
    }
    else
    {
        unsigned short old_bits, shift, rotate, result;

        if (mode == ACCUMULATOR)
        {
            old_bits = cpu->register_a;
            shift = cpu->register_a >> 1;
            rotate = cpu->register_a << 15;

            cpu->register_a = shift | rotate;

            if (carry)
                cpu->register_a |= 0x8000;
            else
                cpu->register_a &= 0x7FFF;

            result = cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);
            
            old_bits = cpu_mem_read_u16(cpu, addr);
            shift = cpu_mem_read_u16(cpu, addr) >> 1;
            rotate = cpu_mem_read_u16(cpu, addr) << 15;

            cpu_mem_write_u16(cpu, addr, shift | rotate);

            if (carry)
                cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) | 0x8000);
            else
                cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) & 0x7FFF);

            result = cpu_mem_read_u16(cpu, addr);
        }

        if (old_bits & 0x0001)  cpu->status = cpu->status | CARRY_FLAG;
        else                    cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag_u16(&cpu->status, result);
        cpu_set_negative_16bit(&cpu->status, result);
    }
}

void cpu_bcc(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & CARRY_FLAG) == 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}
void cpu_bcs(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & CARRY_FLAG) != 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}
void cpu_beq(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & ZERO_FLAG) != 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}
void cpu_bmi(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & NEGATIVE_FLAG) != 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}
void cpu_bne(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & ZERO_FLAG) == 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}
void cpu_bpl(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & NEGATIVE_FLAG) == 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}
void cpu_bra(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    unsigned short old = cpu->program_counter + 1;

    cpu->program_counter += (char)cpu->wram[addr];

    if (cpu->emulation_mode)
    {
        if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
            cpu->cycles += 1;
    }
}
void cpu_bvc(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & OVERFLOW_FLAG) == 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}
void cpu_bvs(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & OVERFLOW_FLAG) != 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr];
        cpu->cycles += 1;

        if (cpu->emulation_mode)
        {
            if ((old >> 8) != ((cpu->program_counter + 1) >> 8))
                cpu->cycles += 1;
        }
    }
}

void cpu_brl(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    cpu->program_counter += (short)cpu_mem_read_u16(cpu, addr);
}

void cpu_jmp(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    cpu->program_counter = (unsigned short)addr;
}
void cpu_jsl(enum AddressingMode mode, CPU *cpu)
{
    cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->program_bank);
    cpu->stack_pointer -= 1;
    cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 3);
    cpu->stack_pointer -= 2;
    cpu->program_counter = cpu_get_operand_address(cpu, mode);
}
void cpu_jsr(enum AddressingMode mode, CPU *cpu)
{
    cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 2);
    cpu->stack_pointer -= 2;
    cpu->program_counter = cpu_get_operand_address(cpu, mode);
}

void cpu_rtl(enum AddressingMode mode, CPU *cpu)
{
    cpu->stack_pointer += 2;
    cpu->program_counter = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1) + 1;
    cpu->stack_pointer += 1;
    cpu->program_bank = cpu_mem_read_u8(cpu, cpu->stack_pointer);
}
void cpu_rts(enum AddressingMode mode, CPU *cpu)
{
    cpu->stack_pointer += 2;
    cpu->program_counter = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1) + 1;
}

void cpu_brk(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu_mem_write_u16(cpu, (cpu->stack_pointer - 1) % 0x0100, cpu->program_counter + 1);
        cpu->stack_pointer = (cpu->stack_pointer - 2) % 0x0100;
        cpu->status |= BREAK_FLAG;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer = (cpu->stack_pointer - 1) % 0x0100;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0xFFFE);
        cpu->cycles -= 1;
    }
    else
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->program_bank);
        cpu->stack_pointer -= 1;
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFE6);
    }

    // turn on interrupt flag
    cpu->status |= 0b00000100;

    // turn off decimal flag
    cpu->status &= 0b11110111;
}
void cpu_cop(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu_mem_write_u16(cpu, (cpu->stack_pointer - 1) % 0x0100, cpu->program_counter + 1);
        cpu->stack_pointer = (cpu->stack_pointer - 2) % 0x0100;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer = (cpu->stack_pointer - 1) % 0x0100;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0xFFF4);
        cpu->cycles -= 1;
    }
    else
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->program_bank);
        cpu->stack_pointer -= 1;
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFE4);
    }

    // turn on interrupt flag
    cpu->status |= 0b00000100;

    // turn off decimal flag
    cpu->status &= 0b11110111;
}

void cpu_abt(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFF8);
    }
    else
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->program_bank);
        cpu->stack_pointer -= 1;
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFE8);
    }
}
void cpu_irq(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFFE);
    }
    else
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->program_bank);
        cpu->stack_pointer -= 1;
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFEE);
    }
}
void cpu_nmi(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFFA);
    }
    else
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->program_bank);
        cpu->stack_pointer -= 1;
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFEA);
    }
}
void cpu_rst(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 1);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFFC);
    }
}

void cpu_rti(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        // In emulation mode, the P register is pulled, then the 16-bit program counter is pulled (again low byte first, then high byte). 
        cpu->stack_pointer += 1;
        cpu->status = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu->stack_pointer += 2;
        cpu->program_counter = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu->cycles -= 1;
    }
    else
    {
        // In native mode, the P register is pulled, then the 16-bit program counter is pulled (low byte first, then high byte), 
        //then the K register (i.e. program bank register) is pulled. 
        cpu->stack_pointer += 1;
        cpu->status = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu->stack_pointer += 2;
        cpu->program_counter = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu->stack_pointer += 1;
        cpu->program_bank = cpu_mem_read_u8(cpu, cpu->stack_pointer);
    }
}

void cpu_clc(enum AddressingMode mode, unsigned char *status)
{
    *status &= 0b11111110;
}
void cpu_cld(enum AddressingMode mode, unsigned char *status)
{
    *status &= 0b11110111;
}
void cpu_cli(enum AddressingMode mode, unsigned char *status)
{
    *status &= 0b11111011;
}
void cpu_clv(enum AddressingMode mode, unsigned char *status)
{
    *status &= 0b10111111;
}
void cpu_sec(enum AddressingMode mode, unsigned char *status)
{
    *status |= CARRY_FLAG;
}
void cpu_sed(enum AddressingMode mode, unsigned char *status)
{
    *status |= DECIMAL_MODE;
}
void cpu_sei(enum AddressingMode mode, unsigned char *status)
{
    *status |= INTERRUPT_FLAG;
}

void cpu_rep(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->emulation_mode)
    {
        cpu->status = cpu->status & ~cpu->wram[addr];
        cpu->status |= 0b00110000; // turn on memory width and index width
    }
    else
    {
        cpu->status = cpu->status & ~cpu->wram[addr];
    }
}
void cpu_sep(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->emulation_mode)
    {
        cpu->status |= cpu->wram[addr];
        cpu->status |= 0b00110000; // turn on memory width and index width
    }
    else
    {
        cpu->status |= cpu->wram[addr];
    }
}

void cpu_lda(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & MEMORY_WIDTH)
    {
        cpu->register_a = cpu_mem_read_u8(cpu, addr);
        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;

        if (mode == ABSOLUTE_X || mode == ABSOLUTE_Y)
            cpu->cycles -= 1;
    }
    else
    {
        cpu->register_a = cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_16bit(&cpu->status, cpu->register_a);
    }
    
    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}
void cpu_ldx(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_x = cpu_mem_read_u8(cpu, addr);
        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_x);
        cpu->cycles -= mode == ABSOLUTE_Y ? 2 : 1;
    }
    else
    {
        cpu->register_x = cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_16bit(&cpu->status, cpu->register_x);
    }
    
    cpu_set_zero_flag(&cpu->status, cpu->register_x);
}
void cpu_ldy(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_y = cpu_mem_read_u8(cpu, addr);
        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_y);
        cpu->cycles -= mode == ABSOLUTE_X ? 2 : 1;
    }
    else
    {
        cpu->register_y = cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_16bit(&cpu->status, cpu->register_y);
    }
    
    cpu_set_zero_flag(&cpu->status, cpu->register_y);
}

void cpu_sta(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & MEMORY_WIDTH)
    {
        cpu_mem_write_u8(cpu, addr, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;
    }
    else cpu_mem_write_u16(cpu, addr, cpu->register_a);
}
void cpu_stx(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    
    if (cpu->status & INDEX_WIDTH)
    {
        cpu_mem_write_u8(cpu, addr, (unsigned char)cpu->register_x);
        cpu->cycles -= 1;
    }
    else cpu_mem_write_u16(cpu, addr, cpu->register_x);
}
void cpu_sty(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    
    if (cpu->status & INDEX_WIDTH)
    {
        cpu_mem_write_u8(cpu, addr, (unsigned char)cpu->register_y);
        cpu->cycles -= 1;
    }
    else cpu_mem_write_u16(cpu, addr, cpu->register_y);
}
void cpu_stz(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & MEMORY_WIDTH)
    {
        cpu_mem_write_u8(cpu, addr, cpu_mem_read_u8(cpu, 0));
        cpu->cycles -= 1;
    }
    else cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, 0));
}

void cpu_mvn(enum AddressingMode mode, CPU *cpu)
{
    //unsigned int addr = cpu_get_operand_address(cpu, mode);
    for (; cpu->register_a >= 0; cpu->register_a--)
    {
        cpu_mem_write_u8(cpu, cpu->register_y, cpu_mem_read_u8(cpu, cpu->register_x));

        cpu->register_x++;
        cpu->register_y++;
        cpu->cycles += 7;

        if (cpu->interrupt) return;
    }
}
void cpu_mvp(enum AddressingMode mode, CPU *cpu)
{
    //unsigned int addr = cpu_get_operand_address(cpu, mode);
    for (; cpu->register_a >= 0; cpu->register_a--)
    {
        cpu_mem_write_u8(cpu, cpu->register_y, cpu_mem_read_u8(cpu, cpu->register_x));

        cpu->register_x--;
        cpu->register_y--;
        cpu->cycles += 7;

        if (cpu->interrupt) return;
    }
}

void cpu_nop(enum AddressingMode mode)
{
    // do nothing
}
void cpu_wdm(enum AddressingMode mode)
{
    // do nothing, skip first byte
}

void cpu_pea(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu_mem_read_u16(cpu, addr));
    cpu->stack_pointer -= 2;
}
void cpu_pei(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu_mem_read_u16(cpu, addr));
    cpu->stack_pointer -= 2;
}
void cpu_per(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu_mem_read_u16(cpu, addr));
    cpu->stack_pointer -= 2;
}

void cpu_pha(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->register_a);
        cpu->stack_pointer -= 1;
    }
    else
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->register_a);
        cpu->stack_pointer -= 2;
    }
}
void cpu_phx(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->register_x);
        cpu->stack_pointer -= 1;
    }
    else
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->register_x);
        cpu->stack_pointer -= 2;
    }
}
void cpu_phy(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->register_y);
        cpu->stack_pointer -= 1;
    }
    else
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->register_y);
        cpu->stack_pointer -= 2;
    }
}
void cpu_pla(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        cpu->stack_pointer += 1;
        cpu->register_a = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;
    }
    else
    {
        cpu->stack_pointer += 2;
        cpu->register_a = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu_set_negative_16bit(&cpu->status, cpu->register_a);
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}
void cpu_plx(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->stack_pointer += 1;
        cpu->register_x = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_x);
        cpu->cycles -= 1;
    }
    else
    {
        cpu->stack_pointer += 2;
        cpu->register_x = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu_set_negative_16bit(&cpu->status, cpu->register_x);
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_x);
}
void cpu_ply(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        cpu->stack_pointer += 1;
        cpu->register_y = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu_set_negative_8bit(&cpu->status, (unsigned char)cpu->register_y);
        cpu->cycles -= 1;
    }
    else
    {
        cpu->stack_pointer += 2;
        cpu->register_y = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu_set_negative_16bit(&cpu->status, cpu->register_y);
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_y);
}

void cpu_phb(enum AddressingMode mode, CPU *cpu)
{
    cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->data_bank);
    cpu->stack_pointer -= 1;
}
void cpu_phd(enum AddressingMode mode, CPU *cpu)
{
    cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->register_direct);
    cpu->stack_pointer -= 2;
}
void cpu_phk(enum AddressingMode mode, CPU *cpu)
{
    cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->program_bank);
    cpu->stack_pointer -= 1;
}
void cpu_php(enum AddressingMode mode, CPU *cpu)
{
    cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
    cpu->stack_pointer -= 1;
}
void cpu_plb(enum AddressingMode mode, CPU *cpu)
{
    cpu->stack_pointer += 1;
    cpu->program_bank = cpu_mem_read_u8(cpu, cpu->stack_pointer);
    cpu_set_negative_8bit(&cpu->status, cpu->program_bank);
}
void cpu_pld(enum AddressingMode mode, CPU *cpu)
{
    cpu->stack_pointer += 2;
    cpu->register_direct = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
    cpu_set_negative_16bit(&cpu->status, cpu->register_direct);
}
void cpu_plp(enum AddressingMode mode, CPU *cpu)
{
    cpu->stack_pointer += 1;
    cpu->status = cpu_mem_read_u8(cpu, cpu->stack_pointer);
    if (cpu->emulation_mode) cpu->status |= 0b00110000;
}

void cpu_stp(enum AddressingMode mode, CPU *cpu)
{
    cpu->stop = 1;
}
void cpu_wai(enum AddressingMode mode, CPU *cpu)
{
    cpu->wait = 1;
}

void cpu_tax(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_x = (unsigned char)cpu->register_a;

        if ((unsigned char)cpu->register_x & 0b10000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if ((unsigned char)cpu->register_x == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
    else
    {
        cpu->register_x = cpu->register_a;

        if (cpu->register_x & 0b1000000000000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if (cpu->register_x == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
}
void cpu_tay(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_y = (unsigned char)cpu->register_a;

        if ((unsigned char)cpu->register_y & 0b10000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if ((unsigned char)cpu->register_y == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
    else
    {
        cpu->register_y = cpu->register_a;

        if (cpu->register_y & 0b1000000000000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if (cpu->register_y == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
}
void cpu_tsx(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_x = (unsigned char)cpu->stack_pointer;

        if ((unsigned char)cpu->register_x & 0b10000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if ((unsigned char)cpu->register_x == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
    else
    {
        cpu->register_x = cpu->stack_pointer;

        if (cpu->register_x & 0b1000000000000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if (cpu->register_x == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
}
void cpu_txa(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        cpu->register_a = (unsigned char)cpu->register_x;

        if ((unsigned char)cpu->register_a & 0b10000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if ((unsigned char)cpu->register_a == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
    else
    {
        cpu->register_a = cpu->register_x;

        if (cpu->register_a & 0b1000000000000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if (cpu->register_a == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
}
void cpu_txs(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu->stack_pointer = 0x0100;
        cpu->stack_pointer |= (unsigned char)cpu->register_x;
    }
    else
    {
        if (cpu->status & INDEX_WIDTH)
        {
            cpu->register_x &= 0xFF;
            cpu->stack_pointer = cpu->register_x;
        }
        else
        {
            cpu->stack_pointer = cpu->register_x;
        }
        
    }
}
void cpu_txy(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_y = (unsigned char)cpu->register_x;

        if ((unsigned char)cpu->register_y & 0b10000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if ((unsigned char)cpu->register_y == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
    else
    {
        cpu->register_y = cpu->register_x;

        if (cpu->register_y & 0b1000000000000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if (cpu->register_y == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
}
void cpu_tya(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        cpu->register_a |= (unsigned char)cpu->register_y;

        if ((unsigned char)cpu->register_a & 0b10000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if ((unsigned char)cpu->register_a == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
    else
    {
        cpu->register_a = cpu->register_y;

        if (cpu->register_a & 0b1000000000000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if (cpu->register_a == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
}
void cpu_tyx(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_x = (unsigned char)cpu->register_y;

        if ((unsigned char)cpu->register_x & 0b10000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if ((unsigned char)cpu->register_x == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
    else
    {
        cpu->register_x = cpu->register_y;

        if (cpu->register_x & 0b1000000000000000)
            cpu->status |= NEGATIVE_FLAG;
        else 
            cpu->status &= 0b011111111;

        if (cpu->register_x == 0)
            cpu->status |= ZERO_FLAG;
        else
            cpu->status &= 0b11111101;
    }
}

void cpu_tcd(enum AddressingMode mode, CPU *cpu)
{
    cpu->register_direct = cpu->register_a;

    cpu_set_zero_flag_u16(&cpu->status, cpu->register_direct);
    cpu_set_negative_16bit(&cpu->status, cpu->register_direct);
}
void cpu_tcs(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
        cpu->stack_pointer |= (unsigned char)cpu->register_a;
    else
        cpu->stack_pointer = cpu->register_a;
}
void cpu_tdc(enum AddressingMode mode, CPU *cpu)
{
    cpu->register_a = cpu->register_direct;

    cpu_set_zero_flag_u16(&cpu->status, cpu->register_a);
    cpu_set_negative_16bit(&cpu->status, cpu->register_a);
}
void cpu_tsc(enum AddressingMode mode, CPU *cpu)
{
    cpu->register_a = cpu->stack_pointer;

    cpu_set_zero_flag_u16(&cpu->status, cpu->register_a);
    cpu_set_negative_16bit(&cpu->status, cpu->register_a);
}

void cpu_xba(enum AddressingMode mode, CPU *cpu)
{
    unsigned char   reg_h = cpu->register_a >> 8,
                    reg_l = cpu->register_a & 0xFF;

    cpu->register_a = (unsigned short)reg_h >> 8 | (unsigned short)reg_l << 8;

    cpu_set_zero_flag_u16(&cpu->status, cpu->register_a);
    cpu_set_negative_16bit(&cpu->status, cpu->register_a);
}

void cpu_xce(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & CARRY_FLAG)
    {
        cpu->emulation_mode = 1;

        // turn off carry flag and turn on memory and index width flags
        cpu->status &= 0b11111110;
        cpu->status |= 0b00110000;

        // force high byte of x and y registers to zero
        cpu->register_x &= 0x00FF;
        cpu->register_y &= 0x00FF;

        // force high byte of stack pointer to 01
        cpu->stack_pointer &= 0x00FF;
        cpu->stack_pointer |= 0x0100;
    }
    else
    {
        cpu->status &= 0b11001111;
        cpu->status |= CARRY_FLAG;
        cpu->emulation_mode = 0;
    }
}

void cpu_set_overflow_flag_u8(unsigned char *status, unsigned char result)
{
    /*
    if (~(cpu->register_a ^ arg) & (cpu->register_a ^ sum) & 0x80)
        cpu->status = cpu->status | Overflow_Flag;
    else 
        cpu->status = cpu->status & 0b10111111;
    */

    // this is absolutely not right(?)

    if (result & 0b01000000)    *status |= OVERFLOW_FLAG;
    else                        *status &= 0b10111111;
}
void cpu_set_overflow_flag_u16(unsigned char *status, unsigned short result)
{
    // this is absolutely not right(?)
    
    if (result & 0x4000)        *status |= OVERFLOW_FLAG;
    else                        *status &= 0b10111111;
}

void cpu_set_zero_flag(unsigned char *status, unsigned short result)
{
    if (result == 0)    *status |= ZERO_FLAG;
    else                *status &= 0b11111101;
}

void cpu_set_zero_flag_u8(unsigned char *status, unsigned char result)
{
    if (result == 0)    *status |= ZERO_FLAG;
    else                *status &= 0b11111101;
}

void cpu_set_zero_flag_u16(unsigned char *status, unsigned short result)
{
    if (result == 0)    *status |= ZERO_FLAG;
    else                *status &= 0b11111101;
}

void cpu_set_idx_zero_flag(unsigned char *status, unsigned short result)
{
    if (*status & INDEX_WIDTH)
    {
        if ((unsigned char)result == 0)
            *status |= ZERO_FLAG;
        else
            *status &= 0b11111101;
    }
    else
    {
        if (result == 0)
            *status |= ZERO_FLAG;
        else
            *status &= 0b11111101;
    }
}

void cpu_set_mem_zero_flag(unsigned char *status, unsigned short result)
{
    if (*status & MEMORY_WIDTH)
    {
        if ((unsigned char)result == 0) 
            *status |= ZERO_FLAG;
        else
            *status &= 0b11111101;
    }
    else
    {
        if (result == 0)
            *status |= ZERO_FLAG;
        else
            *status &= 0b11111101;
    }
}

void cpu_set_negative_8bit(unsigned char *status, unsigned char result)
{
    if (result & 0x80)      *status |= NEGATIVE_FLAG;
    else                    *status &= 0b011111111;
}

void cpu_set_negative_16bit(unsigned char *status, unsigned short result)
{
    if (result & 0x8000)    *status |= NEGATIVE_FLAG;
    else                    *status &= 0b011111111;
}

void cpu_set_mem_negative(unsigned char *status, unsigned short result)
{
    if (*status & MEMORY_WIDTH)
    {
        if ((unsigned char)result & 0b10000000)
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
        if ((unsigned char)result & 0b10000000)
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
    if (cpu->stop) return;

    if (cpu->wait)
    {
        if (!(cpu->status & INTERRUPT_FLAG))
        {
            if (cpu->interrupt)
            {
                // do the interrupt = push states and jump to vector

                cpu->wait = 0;
            }
            else return;
        }
    }

    unsigned char operand = 0;

    switch (operand)
    {
        case 0x00:
            cpu_brk(IMPLIED, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 1;
            break;
        case 0x01:
            cpu_ora(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x02:
            cpu_cop(IMMEDIATE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x03:
            cpu_ora(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x04:
            cpu_tsb(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x05:
            cpu_ora(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x06:
            cpu_asl(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x07:
            cpu_ora(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x08:
            cpu_php(IMPLIED, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 1;
            break;
        case 0x09:
            cpu_ora(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0x0A:
            cpu_asl(ACCUMULATOR, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x0B:
            cpu_phd(IMPLIED, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 1;
            break;
        case 0x0C:
            cpu_tsb(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0x0D:
            cpu_ora(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x0E:
            cpu_asl(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0x0F:
            cpu_ora(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x10:
            cpu_bpl(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0x11:
            cpu_ora(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x12:
            cpu_ora(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0x13:
            cpu_ora(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x14:
            cpu_trb(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x15:
            cpu_ora(DIRECT_X, cpu);
            cpu->cycles +=5;
            cpu->program_counter += 2;
            break;
        case 0x16:
            cpu_asl(DIRECT_X, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x17:
            cpu_ora(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x18:
            cpu_clc(IMPLIED, &cpu->status);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x19:
            cpu_ora(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x1A:
            cpu_inc(ACCUMULATOR, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x1B:
            cpu_tcs(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x1C:
            cpu_trb(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0x1D:
            cpu_ora(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x1E:
            cpu_asl(ABSOLUTE_X, cpu);
            cpu->cycles += 9;
            cpu->program_counter += 3;
            break;
        case 0x1F:
            cpu_ora(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x20:
            cpu_jsr(ABSOLUTE, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x21:
            cpu_and(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x22:
            cpu_jsl(LONG, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 4;
            break;
        case 0x23:
            cpu_and(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x24:
            cpu_bit(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x25:
            cpu_and(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x26:
            cpu_rol(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x27:
            cpu_and(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x28:
            cpu_plp(IMPLIED, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 1;
            break;
        case 0x29:
            cpu_and(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0x2A:
            cpu_rol(ACCUMULATOR, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x2B:
            cpu_pld(IMPLIED, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 1;
            break;
        case 0x2C:
            cpu_bit(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x2D:
            cpu_and(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x2E:
            cpu_rol(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0x2F:
            cpu_and(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x30:
            cpu_bmi(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0x31:
            cpu_and(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x32:
            cpu_and(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0x33:
            cpu_and(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x34:
            cpu_bit(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x35:
            cpu_and(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x36:
            cpu_rol(DIRECT_X, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x37:
            cpu_and(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x38:
            cpu_sec(IMPLIED, &cpu->status);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x39:
            cpu_and(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x3A:
            cpu_dec(ACCUMULATOR, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x3B:
            cpu_tsc(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x3C:
            cpu_bit(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x3D:
            cpu_and(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x3E:
            cpu_rol(ABSOLUTE_X, cpu);
            cpu->cycles += 9;
            cpu->program_counter += 3;
            break;
        case 0x3F:
            cpu_and(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x40:
            cpu_rti(IMPLIED, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 1;
            break;
        case 0x41:
            cpu_eor(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x42:
            cpu_wdm(IMMEDIATE);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0x43:
            cpu_eor(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x44:
            cpu_mvp(SOURCE_DEST, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 3;
            break;
        case 0x45:
            cpu_eor(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x46:
            cpu_lsr(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x47:
            cpu_eor(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x48:
            cpu_pha(IMPLIED, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 1;
            break;
        case 0x49:
            cpu_eor(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0x4A:
            cpu_lsr(ACCUMULATOR, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x4B:
            cpu_phk(IMPLIED, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 1;
            break;
        case 0x4C:
            cpu_jmp(ABSOLUTE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0x4D:
            cpu_eor(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x4E:
            cpu_lsr(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0x4F:
            cpu_eor(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x50:
            cpu_bvc(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0x51:
            cpu_eor(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x52:
            cpu_eor(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0x53:
            cpu_eor(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x54:
            cpu_mvn(SOURCE_DEST, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 3;
            break;
        case 0x55:
            cpu_eor(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x56:
            cpu_lsr(DIRECT_X, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x57:
            cpu_eor(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x58:
            cpu_cli(IMPLIED, &cpu->status);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x59:
            cpu_eor(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x5A:
            cpu_phy(IMPLIED, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 1;
            break;
        case 0x5B:
            cpu_tcd(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x5C:
            cpu_jmp(LONG, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 4;
            break;
        case 0x5D:
            cpu_eor(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x5E:
            cpu_lsr(ABSOLUTE_X, cpu);
            cpu->cycles += 9;
            cpu->program_counter += 3;
            break;
        case 0x5F:
            cpu_eor(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x60:
            cpu_rts(IMPLIED, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 1;
            break;
        case 0x61:
            cpu_adc(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x62:
            cpu_per(IMMEDIATE, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x63:
            cpu_adc(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x64:
            cpu_stz(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x65:
            cpu_adc(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x66:
            cpu_ror(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x67:
            cpu_adc(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x68:
            cpu_pla(IMPLIED, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 1;
            break;
        case 0x69:
            cpu_adc(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0x6A:
            cpu_ror(ACCUMULATOR, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x6B:
            cpu_rtl(IMPLIED, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 1;
            break;
        case 0x6C:
            cpu_jmp(ABSOLUTE_JMP_LONG, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x6D:
            cpu_adc(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x6E:
            cpu_ror(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0x6F:
            cpu_adc(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x70:
            cpu_bvs(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0x71:
            cpu_adc(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x72:
            cpu_adc(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0x73:
            cpu_adc(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x74:
            cpu_stz(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x75:
            cpu_adc(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x76:
            cpu_ror(DIRECT_X, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x77:
            cpu_adc(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x78:
            cpu_sei(IMPLIED, &cpu->status);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x79:
            cpu_adc(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x7A:
            cpu_ply(IMPLIED, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 1;
            break;
        case 0x7B:
            cpu_tdc(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x7C:
            cpu_jmp(ABSOLUTE_JMP_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x7D:
            cpu_adc(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x7E:
            cpu_ror(ABSOLUTE_X, cpu);
            cpu->cycles += 9;
            cpu->program_counter += 3;
            break;
        case 0x7F:
            cpu_adc(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x80:
            cpu_bra(RELATIVE_8, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 2;
            break;
        case 0x81:
            cpu_sta(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x82:
            cpu_brl(RELATIVE_16, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 3;
            break;
        case 0x83:
            cpu_sta(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x84:
            cpu_sty(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x85:
            cpu_sta(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x86:
            cpu_stx(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0x87:
            cpu_sta(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x88:
            cpu_dey(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x89:
            cpu_bit(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0x8A:
            cpu_txa(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x8B:
            cpu_phb(IMPLIED, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 1;
            break;
        case 0x8C:
            cpu_sty(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x8D:
            cpu_sta(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x8E:
            cpu_stx(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x8F:
            cpu_sta(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0x90:
            cpu_bcc(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0x91:
            cpu_sta(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x92:
            cpu_sta(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0x93:
            cpu_sta(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0x94:
            cpu_sty(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x95:
            cpu_sta(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x96:
            cpu_stx(DIRECT_Y, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0x97:
            cpu_sta(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x98:
            cpu_tya(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x99:
            cpu_sta(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x9A:
            cpu_txs(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x9B:
            cpu_txy(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0x9C:
            cpu_stz(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0x9D:
            cpu_sta(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x9E:
            cpu_stz(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0x9F:
            cpu_sta(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0xA0:
            cpu_ldy(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0xA1:
            cpu_lda(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xA2:
            cpu_ldx(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0xA3:
            cpu_lda(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xA4:
            cpu_ldy(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0xA5:
            cpu_lda(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0xA6:
            cpu_ldx(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0xA7:
            cpu_lda(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xA8:
            cpu_tay(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xA9:
            cpu_lda(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0xAA:
            cpu_tax(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xAB:
            cpu_plb(IMPLIED, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 1;
            break;
        case 0xAC:
            cpu_ldy(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xAD:
            cpu_lda(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xAE:
            cpu_ldx(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xAF:
            cpu_lda(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0xB0:
            cpu_bcs(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0xB1:
            cpu_lda(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xB2:
            cpu_lda(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0xB3:
            cpu_lda(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0xB4:
            cpu_ldy(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xB5:
            cpu_lda(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xB6:
            cpu_ldx(DIRECT_Y, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xB7:
            cpu_lda(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xB8:
            cpu_clv(IMPLIED, &cpu->status);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xB9:
            cpu_lda(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xBA:
            cpu_tsx(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xBB:
            cpu_tyx(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xBC:
            cpu_ldy(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xBD:
            cpu_lda(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xBE:
            cpu_ldx(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xBF:
            cpu_lda(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0xC0:
            cpu_cpy(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0xC1:
            cpu_cmp(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xC2:
            cpu_rep(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 2;
            break;
        case 0xC3:
            cpu_cmp(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xC4:
            cpu_cpy(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0xC5:
            cpu_cmp(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0xC6:
            cpu_dec(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xC7:
            cpu_cmp(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xC8:
            cpu_iny(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xC9:
            cpu_cmp(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0xCA:
            cpu_dex(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xCB:
            cpu_wai(IMPLIED, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 1;
            break;
        case 0xCC:
            cpu_cpy(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xCD:
            cpu_cmp(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xCE:
            cpu_dec(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0xCF:
            cpu_cmp(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0xD0:
            cpu_bne(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0xD1:
            cpu_cmp(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xD2:
            cpu_cmp(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0xD3:
            cpu_cmp(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0xD4:
            cpu_pei(DIRECT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0xD5:
            cpu_cmp(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xD6:
            cpu_dec(DIRECT_X, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0xD7:
            cpu_cmp(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xD8:
            cpu_cld(IMPLIED, &cpu->status);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xD9:
            cpu_cmp(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xDA:
            cpu_phx(IMPLIED, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 1;
            break;
        case 0xDB:
            cpu_stp(IMPLIED, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 1;
            break;
        case 0xDC:
            cpu_jmp(ABSOLUTE_JMP_LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xDD:
            cpu_cmp(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xDE:
            cpu_dec(ABSOLUTE_X, cpu);
            cpu->cycles += 9;
            cpu->program_counter += 3;
            break;
        case 0xDF:
            cpu_cmp(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0xE0:
            cpu_cpx(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0xE1:
            cpu_sbc(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xE2:
            cpu_sep(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 2;
            break;
        case 0xE3:
            cpu_sbc(STACK_S, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xE4:
            cpu_cpx(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0xE5:
            cpu_sbc(DIRECT, cpu);
            cpu->cycles += 4;
            cpu->program_counter += 2;
            break;
        case 0xE6:
            cpu_inc(DIRECT, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xE7:
            cpu_sbc(DIRECT_LONG, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xE8:
            cpu_inx(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xE9:
            cpu_sbc(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3;
            break;
        case 0xEA:
            cpu_nop(IMPLIED);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xEB:
            cpu_xba(IMPLIED, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 1;
            break;
        case 0xEC:
            cpu_cpx(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xED:
            cpu_sbc(ABSOLUTE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xEE:
            cpu_inc(ABSOLUTE, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0xEF:
            cpu_sbc(LONG, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
        case 0xF0:
            cpu_beq(RELATIVE_8, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 2;
            break;
        case 0xF1:
            cpu_sbc(DIRECT_SHORT_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xF2:
            cpu_sbc(DIRECT_SHORT, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 2;
            break;
        case 0xF3:
            cpu_sbc(STACK_S_Y, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0xF4:
            cpu_pea(IMMEDIATE, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 3;
            break;
        case 0xF5:
            cpu_sbc(DIRECT_X, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 2;
            break;
        case 0xF6:
            cpu_inc(DIRECT_X, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 2;
            break;
        case 0xF7:
            cpu_sbc(DIRECT_LONG_Y, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xF8:
            cpu_sed(IMPLIED, &cpu->status);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xF9:
            cpu_sbc(ABSOLUTE_Y, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xFA:
            cpu_plx(IMPLIED, cpu);
            cpu->cycles += 5;
            cpu->program_counter += 1;
            break;
        case 0xFB:
            cpu_xce(IMPLIED, cpu);
            cpu->cycles += 2;
            cpu->program_counter += 1;
            break;
        case 0xFC:
            cpu_jsr(ABSOLUTE_JMP_X, cpu);
            cpu->cycles += 8;
            cpu->program_counter += 3;
            break;
        case 0xFD:
            cpu_sbc(ABSOLUTE_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 3;
            break;
        case 0xFE:
            cpu_inc(ABSOLUTE_X, cpu);
            cpu->cycles += 9;
            cpu->program_counter += 3;
            break;
        case 0xFF:
            cpu_sbc(LONG_X, cpu);
            cpu->cycles += 6;
            cpu->program_counter += 4;
            break;
    }
}
