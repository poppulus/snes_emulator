#include "snes_emu.h"

int rom_validate(unsigned char buf[])
{
    unsigned short checksum, compliment;

    // look for LoROM header

    checksum = (unsigned short)buf[ROM_LO_CHECKSUM] << 8 | buf[ROM_LO_CHECKSUM + 1];
    compliment = (unsigned short)buf[ROM_LO_COMPLIMENT] << 8 | buf[ROM_LO_COMPLIMENT + 1];

    if (checksum + compliment == 0xFFFF)
    {
        return 1;
    }

    // look for HiROM header

    checksum = (unsigned short)buf[ROM_HI_CHECKSUM] << 8 | buf[ROM_HI_CHECKSUM + 1];
    compliment = (unsigned short)buf[ROM_HI_COMPLIMENT] << 8 | buf[ROM_HI_COMPLIMENT + 1];

    if (checksum + compliment == 0xFFFF)
    {
        return 2;
    }

    // look for ExHiROM header

    checksum = (unsigned short)buf[ROM_EX_CHECKSUM] << 8 | buf[ROM_EX_CHECKSUM + 1];
    compliment = (unsigned short)buf[ROM_EX_COMPLIMENT] << 8 | buf[ROM_EX_COMPLIMENT + 1];

    if (checksum + compliment == 0xFFFF)
    {
        return 3;
    }

    return 0;
}

void rom_load(BUS *bus)
{
    unsigned char buf[0x60FFFF];
    FILE *f;

    if (!(f = fopen("mario.sfc", "r"))) 
    {
        printf("error opening ROM file ...\n");
        return;
    }

    int i = 0;
    char c;

    while(1)
    {
        c = fgetc(f);
        if (feof(f)) break;
        buf[i++] = c;
        if (i >= 0x40FFFF) break;
    }

    fclose(f);

    if ((i % 1024) == 512)
    {
        printf("probably a headered ROM with an extra 512 bytes of padding\n");
    }

    bus->rom_type = rom_validate(buf);

    // make sure ROM is of the right type

    switch (bus->rom_type)
    {
        default: break;
        case 0:
            printf("error validating ROM file!\n");
            break;
        case 1:
            if (buf[0x7FD5] == 0x20)
            {
                printf("ROM file probably of LoROM type\n");
                int rom_size = ((1 << buf[0x7FD7]) * 1024) - 1;
                bus->rom_buffer = malloc(rom_size); 
                
                for (i = 0; i < rom_size; i++)
                    bus->rom_buffer[i] = buf[i];
            }
            break;
        case 2:
            if (buf[0xFFD5] == 0x21)
            {
                printf("ROM file probably of HiROM type\n");
                int rom_size = ((1 << buf[0xFFD7]) * 1024) - 1;
                bus->rom_buffer = malloc(rom_size); 
                
                for (i = 0; i < rom_size; i++)
                    bus->rom_buffer[i] = buf[i];
            }
            break;
        case 3:
            if (buf[0x40FFD5] == 0x25)
            {
                printf("ROM file probably of ExHiROM type\n");
                int rom_size = ((1 << buf[0x40FFD7]) * 1024) - 1;
                bus->rom_buffer = malloc(rom_size); 

                for (i = 0; i < rom_size; i++)
                    bus->rom_buffer[i] = buf[i];
            }
            break;
    }
}

unsigned char dma_mem_read(BUS *bus, unsigned int addr)
{
    unsigned char data = 0;

    switch (addr)
    {
        default: break;
        case DMAP0:
            data = bus->ppu.dmapn[0];
            break;
        case 0x4310:
            data = bus->ppu.dmapn[1];
            break;
        case 0x4320:
            data = bus->ppu.dmapn[2];
            break;
        case 0x4330:
            data = bus->ppu.dmapn[3];
            break;
        case 0x4340:
            data = bus->ppu.dmapn[4];
            break;
        case 0x4350:
            data = bus->ppu.dmapn[5];
            break;
        case 0x4360:
            data = bus->ppu.dmapn[6];
            break;
        case 0x4370:
            data = bus->ppu.dmapn[7];
            break;
        case BBAD0:
            data = bus->ppu.bbadn[0];
            break;
        case 0x4311:
            data = bus->ppu.bbadn[1];
            break;
        case 0x4321:
            data = bus->ppu.bbadn[2];
            break;
        case 0x4331:
            data = bus->ppu.bbadn[3];
            break;
        case 0x4341:
            data = bus->ppu.bbadn[4];
            break;
        case 0x4351:
            data = bus->ppu.bbadn[5];
            break;
        case 0x4361:
            data = bus->ppu.bbadn[6];
            break;
        case 0x4371:
            data = bus->ppu.bbadn[7];
            break;
        case A1T0L:
            data = bus->ppu.a1tnl[0];
            break;
        case 0x4312:
            data = bus->ppu.a1tnl[1];
            break;
        case 0x4322:
            data = bus->ppu.a1tnl[2];
            break;
        case 0x4332:
            data = bus->ppu.a1tnl[3];
            break;
        case 0x4342:
            data = bus->ppu.a1tnl[4];
            break;
        case 0x4352:
            data = bus->ppu.a1tnl[5];
            break;
        case 0x4362:
            data = bus->ppu.a1tnl[6];
            break;
        case 0x4372:
            data = bus->ppu.a1tnl[7];
            break;
        case A1T0H:
            data = bus->ppu.a1tnh[0];
            break;
        case 0x4313:
            data = bus->ppu.a1tnh[1];
            break;
        case 0x4323:
            data = bus->ppu.a1tnh[2];
            break;
        case 0x4333:
            data = bus->ppu.a1tnh[3];
            break;
        case 0x4343:
            data = bus->ppu.a1tnh[4];
            break;
        case 0x4353:
            data = bus->ppu.a1tnh[5];
            break;
        case 0x4363:
            data = bus->ppu.a1tnh[6];
            break;
        case 0x4373:
            data = bus->ppu.a1tnh[7];
            break;
        case A1B0:
            data = bus->ppu.a1tnb[0];
            break;
        case 0x4314:
            data = bus->ppu.a1tnb[1];
            break;
        case 0x4324:
            data = bus->ppu.a1tnb[2];
            break;
        case 0x4334:
            data = bus->ppu.a1tnb[3];
            break;
        case 0x4344:
            data = bus->ppu.a1tnb[4];
            break;
        case 0x4354:
            data = bus->ppu.a1tnb[5];
            break;
        case 0x4364:
            data = bus->ppu.a1tnb[6];
            break;
        case 0x4374:
            data = bus->ppu.a1tnb[7];
            break;
        case DAS0L:
            data = bus->ppu.dasnl[0];
            break;
        case 0x4315:
            data = bus->ppu.dasnl[1];
            break;
        case 0x4325:
            data = bus->ppu.dasnl[2];
            break;
        case 0x4335:
            data = bus->ppu.dasnl[3];
            break;
        case 0x4345:
            data = bus->ppu.dasnl[4];
            break;
        case 0x4355:
            data = bus->ppu.dasnl[5];
            break;
        case 0x4365:
            data = bus->ppu.dasnl[6];
            break;
        case 0x4375:
            data = bus->ppu.dasnl[7];
            break;
        case DAS0H:
            data = bus->ppu.dasnh[0];
            break;
        case 0x4316:
            data = bus->ppu.dasnh[1];
            break;
        case 0x4326:
            data = bus->ppu.dasnh[2];
            break;
        case 0x4336:
            data = bus->ppu.dasnh[3];
            break;
        case 0x4346:
            data = bus->ppu.dasnh[4];
            break;
        case 0x4356:
            data = bus->ppu.dasnh[5];
            break;
        case 0x4366:
            data = bus->ppu.dasnh[6];
            break;
        case 0x4376:
            data = bus->ppu.dasnh[7];
            break;
        case DASB0:
            data = bus->ppu.dasbn[0];
            break;
        case 0x4317:
            data = bus->ppu.dasbn[1];
            break;
        case 0x4327:
            data = bus->ppu.dasbn[2];
            break;
        case 0x4337:
            data = bus->ppu.dasbn[3];
            break;
        case 0x4347:
            data = bus->ppu.dasbn[4];
            break;
        case 0x4357:
            data = bus->ppu.dasbn[5];
            break;
        case 0x4367:
            data = bus->ppu.dasbn[6];
            break;
        case 0x4377:
            data = bus->ppu.dasbn[7];
            break;
        case A2A0L:
            data = bus->ppu.a2anl[0];
            break;
        case 0x4318:
            data = bus->ppu.a2anl[1];
            break;
        case 0x4328:
            data = bus->ppu.a2anl[2];
            break;
        case 0x4338:
            data = bus->ppu.a2anl[3];
            break;
        case 0x4348:
            data = bus->ppu.a2anl[4];
            break;
        case 0x4358:
            data = bus->ppu.a2anl[5];
            break;
        case 0x4368:
            data = bus->ppu.a2anl[6];
            break;
        case 0x4378:
            data = bus->ppu.a2anl[7];
            break;
        case A2A0H:
            data = bus->ppu.a2anh[0];
            break;
        case 0x4319:
            data = bus->ppu.a2anh[1];
            break;
        case 0x4329:
            data = bus->ppu.a2anh[2];
            break;
        case 0x4339:
            data = bus->ppu.a2anh[3];
            break;
        case 0x4349:
            data = bus->ppu.a2anh[4];
            break;
        case 0x4359:
            data = bus->ppu.a2anh[5];
            break;
        case 0x4369:
            data = bus->ppu.a2anh[6];
            break;
        case 0x4379:
            data = bus->ppu.a2anh[7];
            break;
        case NLTR0:
            data = bus->ppu.nltrn[0];
            break;
        case 0x431A:
            data = bus->ppu.nltrn[1];
            break;
        case 0x432A:
            data = bus->ppu.nltrn[2];
            break;
        case 0x433A:
            data = bus->ppu.nltrn[3];
            break;
        case 0x434A:
            data = bus->ppu.nltrn[4];
            break;
        case 0x435A:
            data = bus->ppu.nltrn[5];
            break;
        case 0x436A:
            data = bus->ppu.nltrn[6];
            break;
        case 0x437A:
            data = bus->ppu.nltrn[7];
            break;
        case UNUSEDn1:
            break;
        case 0x431B:
            break;
        case 0x432B:
            break;
        case 0x433B:
            break;
        case 0x434B:
            break;
        case 0x435B:
            break;
        case 0x436B:
            break;
        case 0x437B:
            break;
        case UNUSEDn2:
            break;
        case 0x431F:
            break;
        case 0x432F:
            break;
        case 0x433F:
            break;
        case 0x434F:
            break;
        case 0x435F:
            break;
        case 0x436F:
            break;
        case 0x437F:
            break;
    }

    return data;
}

void dma_mem_write(BUS *bus, unsigned int addr, unsigned char data)
{
    switch (addr)
    {
        default: break;
        case DMAP0:
            bus->ppu.dmapn[0] = data;
            break;
        case 0x4310:
            bus->ppu.dmapn[1] = data;
            break;
        case 0x4320:
            bus->ppu.dmapn[2] = data;
            break;
        case 0x4330:
            bus->ppu.dmapn[3] = data;
            break;
        case 0x4340:
            bus->ppu.dmapn[4] = data;
            break;
        case 0x4350:
            bus->ppu.dmapn[5] = data;
            break;
        case 0x4360:
            bus->ppu.dmapn[6] = data;
            break;
        case 0x4370:
            bus->ppu.dmapn[7] = data;
            break;
        case BBAD0:
            bus->ppu.bbadn[0] = data;
            break;
        case 0x4311:
            bus->ppu.bbadn[1] = data;
            break;
        case 0x4321:
            bus->ppu.bbadn[2] = data;
            break;
        case 0x4331:
            bus->ppu.bbadn[3] = data;
            break;
        case 0x4341:
            bus->ppu.bbadn[4] = data;
            break;
        case 0x4351:
            bus->ppu.bbadn[5] = data;
            break;
        case 0x4361:
            bus->ppu.bbadn[6] = data;
            break;
        case 0x4371:
            bus->ppu.bbadn[7] = data;
            break;
        case A1T0L:
            bus->ppu.a1tnl[0] = data;
            break;
        case 0x4312:
            bus->ppu.a1tnl[1] = data;
            break;
        case 0x4322:
            bus->ppu.a1tnl[2] = data;
            break;
        case 0x4332:
            bus->ppu.a1tnl[3] = data;
            break;
        case 0x4342:
            bus->ppu.a1tnl[4] = data;
            break;
        case 0x4352:
            bus->ppu.a1tnl[5] = data;
            break;
        case 0x4362:
            bus->ppu.a1tnl[6] = data;
            break;
        case 0x4372:
            bus->ppu.a1tnl[7] = data;
            break;
        case A1T0H:
            bus->ppu.a1tnh[0] = data;
            break;
        case 0x4313:
            bus->ppu.a1tnh[1] = data;
            break;
        case 0x4323:
            bus->ppu.a1tnh[2] = data;
            break;
        case 0x4333:
            bus->ppu.a1tnh[3] = data;
            break;
        case 0x4343:
            bus->ppu.a1tnh[4] = data;
            break;
        case 0x4353:
            bus->ppu.a1tnh[5] = data;
            break;
        case 0x4363:
            bus->ppu.a1tnh[6] = data;
            break;
        case 0x4373:
            bus->ppu.a1tnh[7] = data;
            break;
        case A1B0:
            bus->ppu.a1tnb[0] = data;
            break;
        case 0x4314:
            bus->ppu.a1tnb[1] = data;
            break;
        case 0x4324:
            bus->ppu.a1tnb[2] = data;
            break;
        case 0x4334:
            bus->ppu.a1tnb[3] = data;
            break;
        case 0x4344:
            bus->ppu.a1tnb[4] = data;
            break;
        case 0x4354:
            bus->ppu.a1tnb[5] = data;
            break;
        case 0x4364:
            bus->ppu.a1tnb[6] = data;
            break;
        case 0x4374:
            bus->ppu.a1tnb[7] = data;
            break;
        case DAS0L:
            bus->ppu.dasnl[0] = data;
            break;
        case 0x4315:
            bus->ppu.dasnl[1] = data;
            break;
        case 0x4325:
            bus->ppu.dasnl[2] = data;
            break;
        case 0x4335:
            bus->ppu.dasnl[3] = data;
            break;
        case 0x4345:
            bus->ppu.dasnl[4] = data;
            break;
        case 0x4355:
            bus->ppu.dasnl[5] = data;
            break;
        case 0x4365:
            bus->ppu.dasnl[6] = data;
            break;
        case 0x4375:
            bus->ppu.dasnl[7] = data;
            break;
        case DAS0H:
            bus->ppu.dasnh[0] = data;
            break;
        case 0x4316:
            bus->ppu.dasnh[1] = data;
            break;
        case 0x4326:
            bus->ppu.dasnh[2] = data;
            break;
        case 0x4336:
            bus->ppu.dasnh[3] = data;
            break;
        case 0x4346:
            bus->ppu.dasnh[4] = data;
            break;
        case 0x4356:
            bus->ppu.dasnh[5] = data;
            break;
        case 0x4366:
            bus->ppu.dasnh[6] = data;
            break;
        case 0x4376:
            bus->ppu.dasnh[7] = data;
            break;
        case DASB0:
            bus->ppu.dasbn[0] = data;
            break;
        case 0x4317:
            bus->ppu.dasbn[1] = data;
            break;
        case 0x4327:
            bus->ppu.dasbn[2] = data;
            break;
        case 0x4337:
            bus->ppu.dasbn[3] = data;
            break;
        case 0x4347:
            bus->ppu.dasbn[4] = data;
            break;
        case 0x4357:
            bus->ppu.dasbn[5] = data;
            break;
        case 0x4367:
            bus->ppu.dasbn[6] = data;
            break;
        case 0x4377:
            bus->ppu.dasbn[7] = data;
            break;
        case A2A0L:
            bus->ppu.a2anl[0] = data;
            break;
        case 0x4318:
            bus->ppu.a2anl[1] = data;
            break;
        case 0x4328:
            bus->ppu.a2anl[2] = data;
            break;
        case 0x4338:
            bus->ppu.a2anl[3] = data;
            break;
        case 0x4348:
            bus->ppu.a2anl[4] = data;
            break;
        case 0x4358:
            bus->ppu.a2anl[5] = data;
            break;
        case 0x4368:
            bus->ppu.a2anl[6] = data;
            break;
        case 0x4378:
            bus->ppu.a2anl[7] = data;
            break;
        case A2A0H:
            bus->ppu.a2anh[0] = data;
            break;
        case 0x4319:
            bus->ppu.a2anh[1] = data;
            break;
        case 0x4329:
            bus->ppu.a2anh[2] = data;
            break;
        case 0x4339:
            bus->ppu.a2anh[3] = data;
            break;
        case 0x4349:
            bus->ppu.a2anh[4] = data;
            break;
        case 0x4359:
            bus->ppu.a2anh[5] = data;
            break;
        case 0x4369:
            bus->ppu.a2anh[6] = data;
            break;
        case 0x4379:
            bus->ppu.a2anh[7] = data;
            break;
        case NLTR0:
            bus->ppu.nltrn[0] = data;
            break;
        case 0x431A:
            bus->ppu.nltrn[1] = data;
            break;
        case 0x432A:
            bus->ppu.nltrn[2] = data;
            break;
        case 0x433A:
            bus->ppu.nltrn[3] = data;
            break;
        case 0x434A:
            bus->ppu.nltrn[4] = data;
            break;
        case 0x435A:
            bus->ppu.nltrn[5] = data;
            break;
        case 0x436A:
            bus->ppu.nltrn[6] = data;
            break;
        case 0x437A:
            bus->ppu.nltrn[7] = data;
            break;
    }
}

void dma_dma_write(CPU *cpu, unsigned char channel)
{
    unsigned char   count       = 0,
                    pattern     = cpu->bus.ppu.dmapn[channel] & 0b111,
                    addr_adjust = (cpu->bus.ppu.dmapn[channel] >> 3) & 0b11;

    unsigned int    a_bus       = (cpu->bus.ppu.a1tnb[channel] << 16) | (cpu->bus.ppu.a1tnh[channel] << 8) | cpu->bus.ppu.a1tnl[channel];

    unsigned short  byte_count  = (cpu->bus.ppu.dasnh[channel] << 8) | cpu->bus.ppu.dasnl[channel],
                    b_bus       = 0x2100 | cpu->bus.ppu.bbadn[channel];

    printf("DMA %X %X %d\n", a_bus, b_bus, pattern);

    if (cpu->bus.ppu.dmapn[0] & 0b10000000)
    {
        // copy from bus B to bus A
        for (int i = 0; i < (byte_count == 0 ? 0xFFFF + 1 : byte_count); i++)
        {
            switch (pattern)
            {
                case 0:
                case 2:
                case 6:
                    bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus));
                    break;
                case 1:
                case 5:
                    if (count == 0 && count == 2)
                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus));
                    else if (count == 1 && count == 3)
                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, (b_bus + 1) % 0x2200));
                    break;
                case 3:
                case 7:
                    if (count == 0 && count == 1)
                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus));
                    else if (count == 2 && count == 3)
                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus + 1 % 0x2200));
                    break;
                case 4:
                    bus_mem_write(cpu, a_bus, bus_mem_read(cpu, (b_bus + count) % 0x2200));
                    break;
            }

            if (++count > 3) count = 0;

            if (addr_adjust == 0)
                a_bus += 1;
            else if (addr_adjust == 2)
                a_bus -= 1;
        }
    }
    else
    {
        // copy from bus A to bus B
        for (int i = 0; i < (byte_count == 0 ? 0xFFFF + 1 : byte_count); i++)
        {
            switch (pattern)
            {
                case 0:
                case 2:
                case 6:
                    bus_mem_write(cpu, b_bus, bus_mem_read(cpu, a_bus));
                    break;
                case 1:
                case 5:
                    if (count == 0 && count == 2)
                        bus_mem_write(cpu, b_bus, bus_mem_read(cpu, a_bus));
                    else if (count == 1 && count == 3)
                        bus_mem_write(cpu, (b_bus + 1) % 0x2200, bus_mem_read(cpu, a_bus));
                    break;
                case 3:
                case 7:
                    if (count == 0 && count == 1)
                        bus_mem_write(cpu, b_bus, bus_mem_read(cpu, a_bus));
                    else if (count == 2 && count == 3)
                        bus_mem_write(cpu, (b_bus + 1) % 0x2200, bus_mem_read(cpu, a_bus));
                    break;
                case 4:
                    bus_mem_write(cpu, (b_bus + count) % 0x2200, bus_mem_read(cpu, a_bus));
                    break;
            }

            if (++count > 3) count = 0;

            if (addr_adjust == 0)
                a_bus += 1;
            else if (addr_adjust == 2)
                a_bus -= 1;
        }
    }

    cpu->bus.ppu.dasnl[channel] = 0;
    cpu->bus.ppu.dasnh[channel] = 0;
}
void dma_hdma_write(CPU *cpu, unsigned char channel)
{
    unsigned char   count       = 0,
                    pattern     = cpu->bus.ppu.dmapn[channel] & 0b111,
                    addr_adjust = (cpu->bus.ppu.dmapn[channel] >> 3) & 0b11,
                    line_count  = cpu->bus.ppu.nltrn[channel];

    unsigned int    a_bus       = (cpu->bus.ppu.a1tnb[channel] << 16) | (cpu->bus.ppu.a1tnh[channel] << 8) | cpu->bus.ppu.a1tnl[channel];

    unsigned short  b_bus       = 0x2100 + cpu->bus.ppu.bbadn[channel];

    //printf("HDMA %X %X %d\n", a_bus, b_bus, channel);

    cpu->bus.ppu.a2anl[channel] = cpu->bus.ppu.a1tnl[channel];
    cpu->bus.ppu.a2anh[channel] = cpu->bus.ppu.a1tnh[channel];

    if (cpu->bus.ppu.dmapn[channel] & 0b01000000)
    {
        // indirect address
        unsigned int ind_addr = (cpu->bus.ppu.dasbn[channel] << 16) | (cpu->bus.ppu.dasnh[channel] << 8) | cpu->bus.ppu.dasnl[channel];

        if (cpu->bus.ppu.dmapn[0] & 0b10000000)
        {
            // copy from bus B to bus A

        }
        else
        {
            // copy from bus A to bus B

        }
    }
    else
    {
        // direct address
        if (cpu->bus.ppu.dmapn[0] & 0b10000000)
        {
            // copy from bus B to bus A
            switch (pattern)
            {
                case 0:
                case 2:
                case 6:
                    if (cpu->bus.ppu.nltrn[0] & 0b10000000)
                    {
                        for (int i = 0; i < (line_count & 0b01111111); i++)
                        {
                            bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus));

                            if (addr_adjust == 0)
                                a_bus += 1;
                            else if (addr_adjust == 2)
                                a_bus -= 1;
                        }
                    }
                    else
                    {
                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus));
                    }
                    break;
                case 1:
                case 5:
                    if (cpu->bus.ppu.nltrn[0] & 0b10000000)
                    {
                        for (int i = 0; i < (line_count & 0b01111111) << 1; i++)
                        {
                            bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus + (i % 2) % 0x2200));

                            if (addr_adjust == 0)
                                a_bus += 1;
                            else if (addr_adjust == 2)
                                a_bus -= 1;
                        }
                    }
                    else
                    {
                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, b_bus));

                        if (addr_adjust == 0)
                            a_bus += 1;
                        else if (addr_adjust == 2)
                            a_bus -= 1;

                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, (b_bus + 1) % 0x2200));
                    }
                    break;
                case 3:
                case 7:
                    unsigned char pos = 0;

                    if (cpu->bus.ppu.nltrn[0] & 0b10000000)
                    {
                        for (int i = 0; i < (line_count & 0b01111111) << 2; i++)
                        {
                            if (i % 4 == 0 && i % 4 == 1) pos = 0;
                            else if (i % 4 == 2 && i % 4 == 3) pos = 1;

                            bus_mem_write(cpu, a_bus, bus_mem_read(cpu, (b_bus + pos) % 0x2200));

                            if (addr_adjust == 0)
                                a_bus += 1;
                            else if (addr_adjust == 2)
                                a_bus -= 1;
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if (i == 0 && i == 1) pos = 0;
                            else if (i == 2 && i == 3) pos = 1;

                            bus_mem_write(cpu, a_bus, bus_mem_read(cpu, (b_bus + pos) % 0x2200));

                            if (addr_adjust == 0)
                                a_bus += 1;
                            else if (addr_adjust == 2)
                                a_bus -= 1;
                        }
                    }
                    
                    break;
                case 4:
                    for (int i = 0; i < 4; i++)
                    {
                        bus_mem_write(cpu, a_bus, bus_mem_read(cpu, (b_bus + i) % 0x2200));

                        if (addr_adjust == 0)
                            a_bus += 1;
                        else if (addr_adjust == 2)
                            a_bus -= 1;
                    }
                    break;
            }
        }
        else
        {
            // copy from bus A to bus B
            switch (pattern)
            {
                case 0:
                case 2:
                case 6:
                    bus_mem_write(cpu, b_bus, bus_mem_read(cpu, a_bus));
                    break;
                case 1:
                case 5:
                    bus_mem_write(cpu, b_bus, bus_mem_read(cpu, a_bus));

                    if (addr_adjust == 0)
                        a_bus += 1;
                    else if (addr_adjust == 2)
                        a_bus -= 1;

                    bus_mem_write(cpu, (b_bus + 1) % 0x2200, bus_mem_read(cpu, a_bus));
                    break;
                case 3:
                case 7:
                    bus_mem_write(cpu, b_bus, bus_mem_read(cpu, a_bus));
                    
                    if (addr_adjust == 0)
                        a_bus += 1;
                    else if (addr_adjust == 2)
                        a_bus -= 1;

                    bus_mem_write(cpu, (b_bus + 1) % 0x2200, bus_mem_read(cpu, a_bus));
                    break;
                case 4:
                    bus_mem_write(cpu, (b_bus + count) % 0x2200, bus_mem_read(cpu, a_bus));
                    break;
            }
        }
    }
}

void frame_set_pixel(unsigned char frame_data[], short x, short y, unsigned char rgb[3])
{
    int base = (y * 3 * FRAME_WIDTH) + (x * 3);

    if ((base + 2) < FRAME_LENGTH)
    {
        frame_data[base] = rgb[0];
        frame_data[base + 1] = rgb[1];
        frame_data[base + 2] = rgb[2];
    }
}

void ppu_increment_vmadd(VMADD *addr, unsigned char inc)
{
    unsigned char lo = addr->lo;

    switch (inc & 0b11)
    {
        case 0:
            addr->lo += 1;
            break;
        case 1:
            addr->lo += 32;
            break;
        case 2:
        case 3:
            addr->lo += 128;
            break;
    }

    if (addr->lo < lo)
    {
        switch (inc & 0b11)
        {
            case 0:
                addr->hi += 1;
                break;
            case 1:
                addr->hi += 32;
                break;
            case 2:
            case 3:
                addr->hi += 128;
                break;
        }
        
        // need more?
    }
}

unsigned short ppu_get_vmadd(VMADD addr)
{
    return (unsigned short)addr.hi | (unsigned short)addr.lo;
}

void ppu_render_bg(PPU *ppu, unsigned short *tilemap, unsigned short chr_addr, unsigned char cgram)
{
    for (int i = 0; i < 0x800; i++)
    {
        unsigned short  tile_index  = tilemap[i] & 0b1111111111;

        unsigned short  *tile       = &tilemap[chr_addr + tile_index];

        unsigned char   palette     = (tilemap[i] & 0b1110000000000) >> 10,
                        tile_column = i % 32,
                        tile_row    = i / 32;

        for (int y = 0; y < 8; y++)
        {
            unsigned char   plane_0 = tile[y] & 0xFF, 
                            plane_1 = (tile[y] && 0xFF00) >> 8,
                            plane_2 = tile[y + 8] & 0xFF, 
                            plane_3 = (tile[y + 8] && 0xFF00) >> 8;

            for (int x = 7; x >= 0; x--)
            {
                unsigned char value = (1 & plane_0) << 1 | (1 & plane_1) | (1 & plane_2) << 1 | (1 & plane_3);

                //printf("palette value: %d\n", value);

                plane_0 >>= 1;
                plane_1 >>= 1;
                plane_2 >>= 1;
                plane_3 >>= 1;

                unsigned char   pixel_x     = (tile_column << 3) + x,
                                pixel_y     = (tile_row << 3) + y;
                            
                unsigned short  rgb         = ppu->cgram[cgram + (palette * value)];

                unsigned char   rgb_r       = (rgb & 0b11111) << 3,
                                rgb_g       = ((rgb >> 5) & 0b11111) << 3,
                                rgb_b       = ((rgb >> 10) & 0b11111) << 3;

                unsigned char   rgb_arr[3]  = {rgb_r, rgb_g, rgb_b};

                frame_set_pixel(ppu->frame_data, pixel_x, pixel_y, rgb_arr);
            }
        }
    }
}

void ppu_render_mode(PPU *ppu)
{
    unsigned char   mode = ppu->bgmode & 0b111,

                    bg1_vram_addr = (ppu->bg1sc & 0b11111100) >> 2,
                    bg2_vram_addr = (ppu->bg2sc & 0b11111100) >> 2,
                    bg3_vram_addr = (ppu->bg3sc & 0b11111100) >> 2,
                    bg4_vram_addr = (ppu->bg4sc & 0b11111100) >> 2;

    unsigned short  bg1_chr_addr =  (ppu->bg12nba & 0b1111) << 12,
                    bg2_chr_addr =  (ppu->bg12nba & 0b11110000) << 8,
                    bg3_chr_addr =  (ppu->bg34nba & 0b1111) << 12,
                    bg4_chr_addr =  (ppu->bg34nba & 0b11110000) << 8;

    unsigned short  *bg1_nametable = (unsigned short*)&ppu->vram[bg1_vram_addr << 10],
                    *bg2_nametable = (unsigned short*)&ppu->vram[bg2_vram_addr << 10],
                    *bg3_nametable = (unsigned short*)&ppu->vram[bg3_vram_addr << 10],
                    *bg4_nametable = (unsigned short*)&ppu->vram[bg4_vram_addr << 10];

    switch (mode)
    {
        case 0:
            ppu_render_bg(ppu, bg4_nametable, bg4_chr_addr, 48);
            ppu_render_bg(ppu, bg3_nametable, bg3_chr_addr, 32);
            ppu_render_bg(ppu, bg2_nametable, bg2_chr_addr, 16);
            ppu_render_bg(ppu, bg1_nametable, bg1_chr_addr, 0);
            break;
        case 1:
            ppu_render_bg(ppu, bg3_nametable, bg3_chr_addr, 0);
            ppu_render_bg(ppu, bg2_nametable, bg2_chr_addr, 0);
            ppu_render_bg(ppu, bg1_nametable, bg1_chr_addr, 0);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
    }
}

void ppu_render_oam(PPU *ppu)
{
    for (int i = 0; i < 256; i+=4)
    {
        unsigned char   tile_x      = ppu->oam[i],
                        tile_y      = ppu->oam[i + 1],
                        tile_pal    = (ppu->oam[i + 3] >> 1) & 0b111,
                        tile_prio   = (ppu->oam[i + 3] >> 4) & 0b11;

        unsigned char   tile_flip_h = ppu->oam[i + 3] & 0b01000000,
                        tile_flip_v = ppu->oam[i + 3] & 0b10000000;

        unsigned short  tile_select = ppu->oam[i + 2] | (ppu->oam[i + 3] & 1);
    }
}

void ppu_render(PPU *ppu)
{
    ppu_render_mode(ppu);
    //ppu_render_oam(ppu);
}

unsigned char ppu_mem_read(BUS *bus, unsigned int addr)
{
    unsigned char data = 0;

    switch (addr)
    {
        default: break;
        case MPYL:
            data = bus->ppu.m7_multi_result & 0xFF;
            break;
        case MPYM:
            data = (bus->ppu.m7_multi_result & 0xFF00) >> 8;
            break;
        case MPYH:
            data = (bus->ppu.m7_multi_result & 0xFF0000) >> 16;
            break;
        case SLHV:
            bus->ppu.counter_latch = 1;
            break;
        case OAMDATAREAD:
            data = bus->ppu.oam[bus->ppu.oamaddr];
            bus->ppu.oamaddr += 1;
            break;
        case VMDATALREAD:
            data = bus->ppu.vram_latch & 0xFF;
            
            if (!(bus->ppu.vmain & 0b10000000))
            {
                bus->ppu.vram_latch = bus->ppu.vram[ppu_get_vmadd(bus->ppu.vmadd)];
                ppu_increment_vmadd(&bus->ppu.vmadd, bus->ppu.vmain);
            }
            break;
        case VMDATAHREAD:
            data = (bus->ppu.vram_latch & 0xFF00) >> 8;

            if (bus->ppu.vmain & 0b10000000)
            {
                bus->ppu.vram_latch = bus->ppu.vram[ppu_get_vmadd(bus->ppu.vmadd)];
                ppu_increment_vmadd(&bus->ppu.vmadd, bus->ppu.vmain);
            }
            break;
        case CGDATAREAD:
            if (bus->ppu.cgram_byte == 0)
                data = bus->ppu.cgram[bus->ppu.cgadd] & 0xFF;
            else if (bus->ppu.cgram_byte == 1)
                data = (bus->ppu.cgram[bus->ppu.cgadd] & 0xFF00) >> 8;

            bus->ppu.cgram_byte = ~bus->ppu.cgram_byte;
            break;
        case OPHCT:
            if (bus->ppu.ophct_byte == 0) 
                data = bus->ppu.ophct & 0xFF;
            else if (bus->ppu.ophct_byte == 1)
                data = (bus->ppu.ophct & 0xFF00) >> 8;
            
            bus->ppu.ophct_byte = ~bus->ppu.ophct_byte;
            break;
        case OPVCT:
            if (bus->ppu.opvct_byte == 0)
                data = bus->ppu.opvct & 0xFF;
            else if (bus->ppu.opvct_byte == 1)
                data = (bus->ppu.opvct & 0xFF00) >> 8;
            
            bus->ppu.opvct_byte = ~bus->ppu.opvct_byte;
            break;
        case STAT77:
            data = bus->ppu.stat77;
            break;
        case STAT78:
            data = bus->ppu.stat78;
            bus->ppu.ophct_byte = 0;
            bus->ppu.opvct_byte = 0;
            bus->ppu.counter_latch = 0;
            break;
    }

    return data;
}

void ppu_mem_write(BUS *bus, unsigned int addr, unsigned char data)
{
    switch (addr)
    {
        default: break;
        case INIDISP:
            bus->ppu.inidisp = data;
            break;
        case OBJSEL:
            bus->ppu.objsel = data;
            break;
        case OAMADDL:
            bus->ppu.oamaddr |= data;
            bus->ppu.oamaddr = (bus->ppu.oamaddr & 0x1FF) << 1;
            break;
        case OAMADDH:
            unsigned char hi = data & 0b10000001;
            bus->ppu.oamaddr |= (unsigned short)hi << 8;
            bus->ppu.oamaddr = (bus->ppu.oamaddr & 0x1FF) << 1;
            break;
        case OAMDATA:
            if ((bus->ppu.oamaddr & 1) == 0)
                bus->ppu.oam_latch = data;

            if (bus->ppu.oamaddr < 0x200 && bus->ppu.oamaddr & 0b1)
            {
                bus->ppu.oam[bus->ppu.oamaddr - 1] = bus->ppu.oam_latch;
                bus->ppu.oam[bus->ppu.oamaddr] = data;
            }

            if (bus->ppu.oamaddr >= 0x200)
                bus->ppu.oam[bus->ppu.oamaddr] = data;

            bus->ppu.oamaddr += 1;
            break;
        case BGMODE:
            bus->ppu.bgmode = data;
            break;
        case MOSAIC:
            bus->ppu.mosaic = data;
            break;
        case BG1SC:
            bus->ppu.bg1sc = data;
            break;
        case BG2SC:
            bus->ppu.bg2sc = data;
            break;
        case BG3SC:
            bus->ppu.bg3sc = data;
            break;
        case BG4SC:
            bus->ppu.bg4sc = data;
            break;
        case BG12NBA:
            bus->ppu.bg12nba = data;
            break;
        case BG34NBA:
            bus->ppu.bg34nba = data;
            break;
        case BG1HOFS: // M7HOFS
            bus->ppu.m7hofs = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;

            bus->ppu.bg1hofs = (unsigned short)(data << 8) | (bus->ppu.bgofs_latch & ~7) | (bus->ppu.bgofs_latch & 7);
            bus->ppu.bgofs_latch = data;
            bus->ppu.bghofs_latch = data;
            break;
        case BG1VOFS: // M7VOFS
            bus->ppu.m7vofs = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;

            bus->ppu.bg1vofs = (unsigned short)(data << 8) | bus->ppu.bgofs_latch;
            bus->ppu.bgofs_latch = data;
            break;
        case BG2HOFS:
            bus->ppu.bg2hofs = (unsigned short)(data << 8) | (bus->ppu.bgofs_latch & ~7) | (bus->ppu.bgofs_latch & 7);
            bus->ppu.bgofs_latch = data;
            bus->ppu.bghofs_latch = data;
            break;
        case BG2VOFS:
            bus->ppu.bg2vofs = (unsigned short)(data << 8) | bus->ppu.bgofs_latch;
            bus->ppu.bgofs_latch = data;
            break;
        case BG3HOFS:
            bus->ppu.bg3hofs = (unsigned short)(data << 8) | (bus->ppu.bgofs_latch & ~7) | (bus->ppu.bgofs_latch & 7);
            bus->ppu.bgofs_latch = data;
            bus->ppu.bghofs_latch = data;
            break;
        case BG3VOFS:
            bus->ppu.bg3vofs = (unsigned short)(data << 8) | bus->ppu.bgofs_latch;
            bus->ppu.bgofs_latch = data;
            break;
        case BG4HOFS:
            bus->ppu.bg4hofs = (unsigned short)(data << 8) | (bus->ppu.bgofs_latch & ~7) | (bus->ppu.bgofs_latch & 7);
            bus->ppu.bgofs_latch = data;
            bus->ppu.bghofs_latch = data;
            break;
        case BG4VOFS:
            bus->ppu.bg4vofs = (unsigned short)(data << 8) | bus->ppu.bgofs_latch;
            bus->ppu.bgofs_latch = data;
            break;
        case VMAIN:
            bus->ppu.vmain = data;
            break;
        case VMADDL:
            bus->ppu.vmadd.lo = data;
            bus->ppu.vram_latch = bus->ppu.vram[ppu_get_vmadd(bus->ppu.vmadd)];
            break;
        case VMADDH:
            bus->ppu.vmadd.hi = data;
            bus->ppu.vram_latch = bus->ppu.vram[ppu_get_vmadd(bus->ppu.vmadd)];
            break;
        case VMDATAL:
            bus->ppu.vmdata.lo = data;
            if (!(bus->ppu.vmain & 0b10000000))
                ppu_increment_vmadd(&bus->ppu.vmadd, bus->ppu.vmain);
            break;
        case VMDATAH:
            bus->ppu.vmdata.hi = data;
            if (bus->ppu.vmain & 0b10000000)
                ppu_increment_vmadd(&bus->ppu.vmadd, bus->ppu.vmain);
            break;
        case M7SEL:
            bus->ppu.m7sel = data;
            break;
        case M7A:
            bus->ppu.m7a = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;

            bus->ppu.m7_multi_result = bus->ppu.m7a * (bus->ppu.m7b & 0xFF);
            break;
        case M7B:
            bus->ppu.m7b = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;

            bus->ppu.m7_multi_result = bus->ppu.m7a * (bus->ppu.m7b & 0xFF);
            break;
        case M7C:
            bus->ppu.m7c = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;
            break;
        case M7D:
            bus->ppu.m7d = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;
            break;
        case M7X:
            bus->ppu.m7x = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;
            break;
        case M7Y:
            bus->ppu.m7y = (data << 8) | bus->ppu.mode7_latch;
            bus->ppu.mode7_latch = data;
            break;
        case CGADD:
            bus->ppu.cgadd = data;
            bus->ppu.cgram_byte = 0;
            break;
        case CGDATA:
            if (bus->ppu.cgram_byte == 0)
                bus->ppu.cgram_latch = data;
            else if (bus->ppu.cgram_byte == 1)
                bus->ppu.cgram[bus->ppu.cgadd++] = (unsigned short)(data << 8) | bus->ppu.cgram_latch;

            bus->ppu.cgram_byte = ~bus->ppu.cgram_byte;
            break;
        case W12SEL:
            bus->ppu.w12sel = data;
            break;
        case W34SEL:
            bus->ppu.w34sel = data;
            break;
        case WOBJSEL:
            bus->ppu.wobjsel = data;
            break;
        case WH0:
            bus->ppu.wh0 = data;
            break;
        case WH1:
            bus->ppu.wh1 = data;
            break;
        case WH2:
            bus->ppu.wh2 = data;
            break;
        case WH3:
            bus->ppu.wh3 = data;
            break;
        case WBGLOG:
            bus->ppu.wbglog  = data;
            break;
        case WOBJGLOG:
            bus->ppu.wobjlog  = data;
            break;
        case TM:
            bus->ppu.tm = data;
            break;
        case TS:
            bus->ppu.ts = data;
            break;
        case TMW:
            bus->ppu.tmw  = data;
            break;
        case TSW:
            bus->ppu.tsw  = data;
            break;
        case CGWSEL:
            bus->ppu.cgwsel  = data;
            break;
        case CGADSUB:
            bus->ppu.cgadsub = data;
            break;
        case COLDATA:
            bus->ppu.coldata = data;
            break;
        case SETINI:
            bus->ppu.setini = data;
            break;
    }
}

void ppu_init(PPU *ppu)
{
    ppu->nmi_vblank = 0;
    ppu->nmi_write = 0;
}

void bus_addr_set(WMADD *addr, unsigned int data)
{
    addr->hi = (unsigned char)(data >> 16);
    addr->mi = (unsigned char)(data >> 8);
    addr->lo = (unsigned char)(data & 0xFF);
}

unsigned int bus_addr_get(WMADD addr)
{
    return (unsigned int)(addr.hi << 16) | (unsigned short)addr.mi << 8 | addr.lo;
}

void bus_increment_wmadd(WMADD *addr)
{
    unsigned char   lo = addr->lo, 
                    mi = addr->mi;

    if (++addr->lo < lo)
    {
        if (++addr->mi < mi)
            addr->hi = 1;
    }
}

unsigned char bus_joypad_read(JOYPAD *joypad, unsigned char hi)
{
    unsigned char result;

    if (joypad->index > (hi ? 15 : 7))
        return 1;

    result = (joypad->btn_status & (1 << joypad->index)) >> joypad->index;

    if (!joypad->strobe && joypad->index <= (hi ? 15 : 7))
        joypad->index += 1;

    return result;
}

void bus_joypad_write(JOYPAD joypad[], unsigned char data)
{
    for (int i = 0; i < 4; i++)
    {
        joypad[i].strobe = data & 1;

        if (joypad[i].strobe)
            joypad[i].index = 0;
    }
}

unsigned char bus_mem_read(CPU *cpu, unsigned int pos)
{
    unsigned char data = 0;

    switch (pos)
    {
        case 0x0000 ... 0x2000:
            data = cpu->wram[pos];
            break;
        case PPU_START ... PPU_END:
            data = ppu_mem_read(&cpu->bus, pos);
            break;
        case APUIO0:
            break;
        case APUIO1:
            break;
        case APUIO2:
            break;
        case APUIO3:
            break;
        case WMDATA:
            data = cpu->wram[bus_addr_get(cpu->address) % 0x1FFFF];
            break;
        case RDNMI:
            data = cpu->rdnmi;
            cpu->rdnmi &= 0x7F;
            break;
        case TIMEUP:
            data = cpu->timeup;
            cpu->timeup &= 0x7F;
            break;
        case HVBJOY:
            break;
        case RDIO:
            data = cpu->wrio;
            break;
        case RDDIVL:
            data = cpu->dividend.lo;
            break;
        case RDDIVH:
            data = cpu->dividend.hi;
            break;
        case RDMPYL:
            data = cpu->prodrem.lo;
            break;
        case RDMPYH:
            data = cpu->prodrem.hi;
            break;
        case JOYSER0:
            break;
        case JOYSER1:
            break;
        case JOY1L:
            data = bus_joypad_read(&cpu->bus.joypad[0], 0);
            break;
        case JOY1H:
            data = bus_joypad_read(&cpu->bus.joypad[0], 1);
            break;
        case JOY2L:
            data = bus_joypad_read(&cpu->bus.joypad[1], 0);
            break;
        case JOY2H:
            data = bus_joypad_read(&cpu->bus.joypad[1], 1);
            break;
        case JOY3L:
            data = bus_joypad_read(&cpu->bus.joypad[2], 0);
            break;
        case JOY3H:
            data = bus_joypad_read(&cpu->bus.joypad[2], 1);
            break;
        case JOY4L:
            data = bus_joypad_read(&cpu->bus.joypad[3], 0);
            break;
        case JOY4H:
            data = bus_joypad_read(&cpu->bus.joypad[3], 1);
            break;
        case 0x4300:
        case 0x4310:
        case 0x4320:
        case 0x4330:
        case 0x4340:
        case 0x4350:
        case 0x4360:
        case 0x4370:
        case 0x4301:
        case 0x4311:
        case 0x4321:
        case 0x4331:
        case 0x4341:
        case 0x4351:
        case 0x4361:
        case 0x4371:
        case 0x4302:
        case 0x4312:
        case 0x4322:
        case 0x4332:
        case 0x4342:
        case 0x4352:
        case 0x4362:
        case 0x4372:
        case 0x4303:
        case 0x4313:
        case 0x4323:
        case 0x4333:
        case 0x4343:
        case 0x4353:
        case 0x4363:
        case 0x4373:
        case 0x4304:
        case 0x4314:
        case 0x4324:
        case 0x4334:
        case 0x4344:
        case 0x4354:
        case 0x4364:
        case 0x4374:
        case 0x4305:
        case 0x4315:
        case 0x4325:
        case 0x4335:
        case 0x4345:
        case 0x4355:
        case 0x4365:
        case 0x4375:
        case 0x4306:
        case 0x4316:
        case 0x4326:
        case 0x4336:
        case 0x4346:
        case 0x4356:
        case 0x4366:
        case 0x4376:
        case 0x4307:
        case 0x4317:
        case 0x4327:
        case 0x4337:
        case 0x4347:
        case 0x4357:
        case 0x4367:
        case 0x4377:
        case 0x4308:
        case 0x4318:
        case 0x4328:
        case 0x4338:
        case 0x4348:
        case 0x4358:
        case 0x4368:
        case 0x4378:
        case 0x4309:
        case 0x4319:
        case 0x4329:
        case 0x4339:
        case 0x4349:
        case 0x4359:
        case 0x4369:
        case 0x4379:
        case 0x430A:
        case 0x431A:
        case 0x432A:
        case 0x433A:
        case 0x434A:
        case 0x435A:
        case 0x436A:
        case 0x437A:
        case 0x430B:
        case 0x431B:
        case 0x432B:
        case 0x433B:
        case 0x434B:
        case 0x435B:
        case 0x436B:
        case 0x437B:
        case 0x430F:
        case 0x431F:
        case 0x432F:
        case 0x433F:
        case 0x434F:
        case 0x435F:
        case 0x436F:
        case 0x437F:
            // read and write
            data = dma_mem_read(&cpu->bus, pos);
            break;
        case 0x8000 ... 0xFFFF:
            unsigned int offset;

            if (cpu->bus.rom_type == LoROM)
                offset = 0x7FFF;
            else if (cpu->bus.rom_type == HiROM)
                offset = 0xFFFF;
            else if (cpu->bus.rom_type == ExHiROM)
                offset = 0x40FFFF;

            data = cpu->bus.rom_buffer[pos - offset];
            break;
    }

    return data;
}

void bus_mem_write(CPU *cpu, unsigned int pos, unsigned char data)
{
    switch (pos)
    {
        case 0x0000 ... 0x2000:
            cpu->wram[pos] = data;
            break;
        case PPU_START ... PPU_END:
            ppu_mem_write(&cpu->bus, pos, data);
            break;
        case APUIO0:
            break;
        case APUIO1:
            break;
        case APUIO2:
            break;
        case APUIO3:
            break;
        case WMDATA:
            cpu->wram[bus_addr_get(cpu->address) % 0x1FFFF] = data;
            break;
        case WMADDL:
            cpu->address.lo = data;
            break;
        case WMADDM:
            cpu->address.mi = data;
            break;
        case WMADDH:
            cpu->address.hi = data & 1;
            break;
        case NMITIMEN:
            cpu->nmitimen = data;
            break;
        case MDMAEN:
            cpu->mdmaen = data;

            if (cpu->mdmaen & 0b1) dma_dma_write(cpu, 0);
            if (cpu->mdmaen & 0b10) dma_dma_write(cpu, 1);
            if (cpu->mdmaen & 0b100) dma_dma_write(cpu, 2);
            if (cpu->mdmaen & 0b1000) dma_dma_write(cpu, 3);
            if (cpu->mdmaen & 0b10000) dma_dma_write(cpu, 4);
            if (cpu->mdmaen & 0b100000) dma_dma_write(cpu, 5);
            if (cpu->mdmaen & 0b1000000) dma_dma_write(cpu, 6);
            if (cpu->mdmaen & 0b10000000) dma_dma_write(cpu, 7);
            break;
        case HDMAEN:
            cpu->hdmaen = data;
            break;
        case MEMSEL:
            cpu->memsel = data;
            break;
        case JOYOUT:
            bus_joypad_write(cpu->bus.joypad, data);
            break;
        case WRIO:
            cpu->wrio = data;
            break;
        case WRMPYA:
            cpu->multiply._a = data;
            break;
        case WRMPYB:
            cpu->multiply._b = data;
            // also starts multiplication process
            break;
        case WRDIVL:
            cpu->dividend.lo = data;
            break;
        case WRDIVH:
            cpu->dividend.hi = data;
            // starts division process and puts result in RDDIV (quotient) and RDMPY (remainder)
            break;
        case WRDIVB:
            cpu->divisor = data;
            break;
        case HTIMEL:
            cpu->htimer &= data;
            break;
        case HTIMEH:
            cpu->htimer &= (unsigned short)data << 8;
            break;
        case VTIMEL:
            cpu->vtimer &= data;
            break;
        case VTIMEH:
            cpu->vtimer &= (unsigned short)data << 8;
            break;
        case 0x4300:
        case 0x4310:
        case 0x4320:
        case 0x4330:
        case 0x4340:
        case 0x4350:
        case 0x4360:
        case 0x4370:
        case 0x4301:
        case 0x4311:
        case 0x4321:
        case 0x4331:
        case 0x4341:
        case 0x4351:
        case 0x4361:
        case 0x4371:
        case 0x4302:
        case 0x4312:
        case 0x4322:
        case 0x4332:
        case 0x4342:
        case 0x4352:
        case 0x4362:
        case 0x4372:
        case 0x4303:
        case 0x4313:
        case 0x4323:
        case 0x4333:
        case 0x4343:
        case 0x4353:
        case 0x4363:
        case 0x4373:
        case 0x4304:
        case 0x4314:
        case 0x4324:
        case 0x4334:
        case 0x4344:
        case 0x4354:
        case 0x4364:
        case 0x4374:
        case 0x4305:
        case 0x4315:
        case 0x4325:
        case 0x4335:
        case 0x4345:
        case 0x4355:
        case 0x4365:
        case 0x4375:
        case 0x4306:
        case 0x4316:
        case 0x4326:
        case 0x4336:
        case 0x4346:
        case 0x4356:
        case 0x4366:
        case 0x4376:
        case 0x4307:
        case 0x4317:
        case 0x4327:
        case 0x4337:
        case 0x4347:
        case 0x4357:
        case 0x4367:
        case 0x4377:
        case 0x4308:
        case 0x4318:
        case 0x4328:
        case 0x4338:
        case 0x4348:
        case 0x4358:
        case 0x4368:
        case 0x4378:
        case 0x4309:
        case 0x4319:
        case 0x4329:
        case 0x4339:
        case 0x4349:
        case 0x4359:
        case 0x4369:
        case 0x4379:
        case 0x430A:
        case 0x431A:
        case 0x432A:
        case 0x433A:
        case 0x434A:
        case 0x435A:
        case 0x436A:
        case 0x437A:
        case 0x430B:
        case 0x431B:
        case 0x432B:
        case 0x433B:
        case 0x434B:
        case 0x435B:
        case 0x436B:
        case 0x437B:
        case 0x430F:
        case 0x431F:
        case 0x432F:
        case 0x433F:
        case 0x434F:
        case 0x435F:
        case 0x436F:
        case 0x437F:
            dma_mem_write(&cpu->bus, pos, data);
            break;
    }

    bus_increment_wmadd(&cpu->address);
}

void bus_init(BUS *bus)
{
    bus->joypad->btn_status = 0;
    bus->joypad->index = 0;
    bus->joypad->strobe = 0;

    bus->rom_buffer = NULL;

    int i = 0;
    
    for (; i < 8; i++)
    {
        bus->ppu.dmapn[i] = 0xFF;
        bus->ppu.bbadn[i] = 0xFF;
        bus->ppu.dasbn[i] = 0xFF;
        bus->ppu.dasnh[i] = 0xFF;
        bus->ppu.dasnl[i] = 0xFF;
        bus->ppu.nltrn[i] = 0xFF;
        bus->ppu.a2anh[i] = 0xFF;
        bus->ppu.a2anl[i] = 0xFF;
        bus->ppu.a1tnb[i] = 0xFF;
        bus->ppu.a1tnh[i] = 0xFF;
        bus->ppu.a1tnl[i] = 0xFF;
    }
}

void cpu_interrupt_nmi(CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        cpu_mem_write_u16(cpu, 0x0100 + cpu->stack_pointer - 1, cpu->program_counter);
        cpu->stack_pointer -= 2;

        cpu_mem_write_u8(cpu, 0x0100 + cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;

        cpu->program_counter = cpu_mem_read_u16(cpu, 0xFFFA);
    }
    else 
    {
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter);
        cpu->stack_pointer -= 2;

        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;

        cpu->program_counter = cpu_mem_read_u16(cpu, 0xFFEA);
    }

    cpu->cycles += 2;
}

unsigned int cpu_get_operand_address(CPU *cpu, enum AddressingMode mode)
{
    unsigned int    opaddr = 0;
    unsigned short  base_16;
    unsigned char   base_8;
    
    switch (mode)
    {
        default:
        case ACCUMULATOR:
        case SOURCE_DEST:
        case IMPLIED:
            // these are instruction specific
            break;
        case ABSOLUTE:
            opaddr = (unsigned int)(cpu->data_bank << 16) | cpu_mem_read_u16(cpu, cpu->program_counter + 1);
            break;
        case ABSOLUTE_X:
            base_16 = cpu_mem_read_u16(cpu, cpu->program_counter + 1);
            opaddr = (unsigned int)(cpu->data_bank << 16) | ((base_16 + cpu->register_x) % 0x10000);
            break;
        case ABSOLUTE_Y:
            base_16 = cpu_mem_read_u16(cpu, cpu->program_counter + 1);
            opaddr = (unsigned int)(cpu->data_bank << 16) | ((base_16 + cpu->register_y) % 0x10000);
            break;
        case ABSOLUTE_JMP:
            opaddr = (unsigned int)(cpu->program_bank << 16) | cpu_mem_read_u16(cpu, cpu->program_counter + 1);
            break;
        case ABSOLUTE_JMP_LONG:
            opaddr = cpu_mem_read_u24(cpu, cpu->program_counter + 1);
            break;
        case ABSOLUTE_JMP_X:
            base_16 = cpu_mem_read_u16(cpu, cpu->program_counter + 1);
            opaddr = (unsigned int)(cpu->program_bank << 16) | ((base_16 + cpu->register_x) % 0x10000);
            break;
        case DIRECT:
            base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
            opaddr = (cpu->register_direct + base_8) % 0x10000;
            break;
        case DIRECT_X:
            if (cpu->emulation_mode && (cpu->register_direct & 0xFF) == 0)
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                opaddr = ((cpu->register_direct & 0xFF00) + base_8 + cpu->register_x) % 0x10000;
            }
            else
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                opaddr = (cpu->register_direct + base_8 + cpu->register_x) % 0x10000;
            }
            break;
        case DIRECT_Y:
            if (cpu->emulation_mode && (cpu->register_direct & 0xFF) == 0)
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                opaddr = ((cpu->register_direct & 0xFF00) + base_8 + cpu->register_y) % 0x10000;
            }
            else
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                opaddr = (cpu->register_direct + base_8 + cpu->register_y) % 0x10000;
            }
            break;
        case DIRECT_SHORT:
            if (cpu->emulation_mode && (cpu->register_direct & 0xFF) == 0)
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                base_16 = cpu_mem_read_u16(cpu, (cpu->register_direct & 0xFF00) + base_8);
                opaddr = (unsigned int)(cpu->data_bank << 16) | base_16;
            }
            else
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                base_16 = cpu_mem_read_u16(cpu, (cpu->register_direct + base_8) % 0x10000);
                opaddr = (unsigned int)(cpu->data_bank << 16) | base_16;
            }
            break;
        case DIRECT_SHORT_X:
            if (cpu->emulation_mode && (cpu->register_direct & 0xFF) == 0)
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                base_16 = ((cpu->register_direct & 0xFF00) + base_8 + cpu->register_x) % 0x10000;
                opaddr = (unsigned int)(cpu->data_bank << 16) | cpu_mem_read_u16(cpu, base_16);
            }
            else
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                base_16 = (cpu->register_direct + base_8 + cpu->register_x) % 0x10000;
                opaddr = (unsigned int)(cpu->data_bank << 16) | cpu_mem_read_u16(cpu, base_16);
            }
            break;
        case DIRECT_SHORT_Y:
            if (cpu->emulation_mode && (cpu->register_direct & 0xFF) == 0)
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                base_16 = cpu_mem_read_u16(cpu, (cpu->register_direct & 0xFF00) + base_8);
                opaddr = (unsigned int)((cpu->data_bank << 16) | base_16) + cpu->register_y;
            }
            else
            {
                base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
                base_16 = cpu_mem_read_u16(cpu, (base_8 + cpu->register_direct) % 0x10000);
                opaddr = (unsigned int)((cpu->data_bank << 16) | base_16) + cpu->register_y;
            }
            break;
        case DIRECT_LONG:
            base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
            opaddr = cpu_mem_read_u24(cpu, (cpu->register_direct + base_8) % 0x10000);
            break;
        case DIRECT_LONG_Y:
            base_8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
            opaddr = cpu_mem_read_u24(cpu, (cpu->register_direct + base_8) % 0x10000) + cpu->register_y;
            break;
        case LONG:
            opaddr = cpu_mem_read_u24(cpu, cpu->program_counter + 1);
            break;
        case LONG_X:
            opaddr = cpu_mem_read_u24(cpu, cpu->program_counter + 1 + cpu->register_x % 0x10000);
            break;
        case IMMEDIATE:
        case RELATIVE_8:
        case RELATIVE_16:
            opaddr = (unsigned int)(cpu->program_bank << 16) | (cpu->program_counter + 1);
            break;
        case STACK_S:
            opaddr = (unsigned short)(cpu_mem_read_u8(cpu, cpu->program_counter + 1) + cpu->stack_pointer);
            break;
        case STACK_S_Y:
            base_16 = cpu_mem_read_u16(cpu, (cpu->program_counter + 1 + cpu->stack_pointer) % 0x10000);
            opaddr = ((unsigned int)cpu->data_bank << 16) | base_16;
            break;
    }

    return opaddr % 0x1000000;
}

unsigned char cpu_mem_read_u8(CPU *cpu, unsigned int pos)
{
    return bus_mem_read(cpu, pos);
}
void cpu_mem_write_u8(CPU *cpu, unsigned int pos, unsigned char data)
{
    bus_mem_write(cpu, pos, data);
}

unsigned short cpu_mem_read_u16(CPU *cpu, unsigned int pos)
{
    unsigned short  lo = cpu_mem_read_u8(cpu, pos), 
                    hi = cpu_mem_read_u8(cpu, pos + 1);

    return (hi << 8) | lo;
}

void cpu_mem_write_u16(CPU *cpu, unsigned int pos, unsigned short data)
{
    unsigned char   hi = data >> 8, 
                    lo = data & 0xFF;

    cpu_mem_write_u8(cpu, pos, lo);
    cpu_mem_write_u8(cpu, pos + 1, hi);
}

unsigned int cpu_mem_read_u24(CPU *cpu, unsigned int pos)
{
    unsigned char   lo = cpu_mem_read_u8(cpu, pos), 
                    mi = cpu_mem_read_u8(cpu, pos + 1),
                    hi = cpu_mem_read_u8(cpu, pos + 2);

    return (unsigned int)(hi << 16) | (unsigned short)(mi << 8) | lo;
}

void cpu_adc(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  arg =  cpu_mem_read_u16(cpu, addr);

    int             sum = cpu->register_a + arg + (cpu->status & CARRY_FLAG);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char   rega_arg = (unsigned char)cpu->register_a ^ cpu->wram[addr % 0x1FFFF],
                        rega_sum = (unsigned char)cpu->register_a ^ (unsigned char)sum;

        if (~rega_arg & rega_sum & 0x80)
            cpu->status = cpu->status | OVERFLOW_FLAG;
        else 
            cpu->status = cpu->status & 0b10111111;

        unsigned short old = cpu->register_a;

        cpu->register_a = sum & 0xFF;

        if (sum > 0xFF) cpu_sec(mode, &cpu->status);
        else cpu_clc(mode, &cpu->status);

        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;

        switch (mode)
        {
            default: break;
            case DIRECT_SHORT_Y:
            case ABSOLUTE_X:
            case ABSOLUTE_Y:
                if (cpu->status & INDEX_WIDTH)
                {
                    cpu->cycles -= 1;
                    if ((old >> 8) != (cpu->register_a >> 8))
                        cpu->cycles += 1;
                }
                break;
        }
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

        cpu_set_negative_u16(&cpu->status, cpu->register_a);
    }

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}
void cpu_sbc(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  arg = cpu_mem_read_u16(cpu, addr) ^ 0xFFFF;

    int             sum = cpu->register_a + arg + (cpu->status & CARRY_FLAG);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned char   rega_arg = (unsigned char)cpu->register_a ^ (unsigned char)arg,
                        rega_sum = (unsigned char)cpu->register_a ^ (unsigned char)sum;

        if (~rega_arg & rega_sum & 0x80)
            cpu->status = cpu->status | OVERFLOW_FLAG;
        else 
            cpu->status = cpu->status & 0b10111111;

        unsigned short old = cpu->register_a;

        cpu->register_a = sum & 0xFF;

        if (sum > 0xFF) cpu_sec(mode, &cpu->status);
        else cpu_clc(mode, &cpu->status);

        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;

        switch (mode)
        {
            default: break;
            case DIRECT_SHORT_Y:
            case ABSOLUTE_X:
            case ABSOLUTE_Y:
                if (cpu->status & INDEX_WIDTH)
                {
                    cpu->cycles -= 1;
                    if ((old >> 8) != (cpu->register_a >> 8))
                        cpu->cycles += 1;
                }
                break;
        }
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

        cpu_set_negative_u16(&cpu->status, cpu->register_a);
    }

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}

void cpu_cmp(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    unsigned short result;

    if (cpu->status & MEMORY_WIDTH)
    {
        if ((unsigned char)cpu->register_a >= cpu->wram[addr % 0x1FFFF])
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        unsigned short old = cpu->register_a;

        result = (unsigned char)cpu->register_a - cpu->wram[addr % 0x1FFFF];
        cpu_set_negative_u8(&cpu->status, (unsigned char)result);
        cpu->cycles -= 1;

        switch (mode)
        {
            default: break;
            case DIRECT_SHORT_Y:
            case ABSOLUTE_X:
            case ABSOLUTE_Y:
                if (cpu->status & INDEX_WIDTH)
                {
                    cpu->cycles -= 1;
                    if ((old >> 8) != (result >> 8))
                        cpu->cycles += 1;
                }
                break;
        }
    }
    else
    {
        if (cpu->register_a >= cpu_mem_read_u16(cpu, addr))
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = cpu->register_a - cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, result);
    }

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }

    cpu_set_zero_flag(&cpu->status, result);
}
void cpu_cpx(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    unsigned short result;

    if (cpu->status & INDEX_WIDTH)
    {
        if ((unsigned char)cpu->register_x >= cpu->wram[addr % 0x1FFFF])
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = (unsigned char)cpu->register_x - cpu->wram[addr % 0x1FFFF];
        cpu_set_negative_u8(&cpu->status, (unsigned char)result);
        cpu->cycles -= 1;
    }
    else
    {
        if (cpu->register_x >= cpu_mem_read_u16(cpu, addr))
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = cpu->register_x - cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT) cpu->cycles += 1;

    cpu_set_zero_flag(&cpu->status, result);
}
void cpu_cpy(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);
    unsigned short result;

    if (cpu->status & INDEX_WIDTH)
    {
        if ((unsigned char)cpu->register_y >= cpu->wram[addr % 0x1FFFF])
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = (unsigned char)cpu->register_y - cpu->wram[addr % 0x1FFFF];
        cpu_set_negative_u8(&cpu->status, (unsigned char)result);
        cpu->cycles -= 1;
    }
    else
    {
        if (cpu->register_y >= cpu_mem_read_u16(cpu, addr))
            cpu->status |= CARRY_FLAG;
        else 
            cpu->status &= 0b11111110;

        result = cpu->register_y - cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT) cpu->cycles += 1;

    cpu_set_zero_flag(&cpu->status, result);
}

void cpu_dec(enum AddressingMode mode, CPU *cpu)
{
    unsigned short result;
    if (cpu->status & MEMORY_WIDTH)
    {
        if (mode == ACCUMULATOR)
        {
            cpu->register_a = (unsigned char)cpu->register_a - 1;
            result = (unsigned char)cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);
            cpu->wram[addr % 0x1FFFF] -= 1;
            result = cpu->wram[addr % 0x1FFFF];
        }
        cpu_set_negative_u8(&cpu->status, (unsigned char)result);
        cpu->cycles -= 2;
    }
    else
    {
        if (mode == ACCUMULATOR)
        {
            cpu->register_a--;
            result = cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);
            cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) - 1);
            result = cpu_mem_read_u16(cpu, addr);
        }
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT || mode == DIRECT_X) 
    {
        if ((cpu->register_direct & 0xFF) != 0) cpu->cycles += 1;
    }

    cpu_set_zero_flag(&cpu->status, result);
}
void cpu_dex(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_x = (unsigned char)cpu->register_x - 1;
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_x);
    }
    else
    {
        cpu->register_x--;
        cpu_set_negative_u16(&cpu->status, cpu->register_x);
    }
    cpu_set_zero_flag(&cpu->status, cpu->register_x);
}
void cpu_dey(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_y = (unsigned char)cpu->register_y - 1;
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_y);
    }
    else
    {
        cpu->register_y--;
        cpu_set_negative_u16(&cpu->status, cpu->register_y);
    }
    cpu_set_zero_flag(&cpu->status, cpu->register_y);
}

void cpu_inc(enum AddressingMode mode, CPU *cpu)
{
    unsigned short result;

    if (cpu->status & MEMORY_WIDTH)
    {
        if (mode == ACCUMULATOR)
        {
            cpu->register_a = (unsigned char)cpu->register_a + 1;
            result = (unsigned char)cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);
            cpu->wram[addr % 0x1FFFF] += 1;
            result = cpu->wram[addr % 0x1FFFF];
        }
        cpu_set_negative_u8(&cpu->status, (unsigned char)result);
        cpu->cycles -= 2;
    }
    else
    {
        if (mode == ACCUMULATOR)
        {
            cpu->register_a++;
            result = cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);
            cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) + 1);
            result = cpu_mem_read_u16(cpu, addr);
        }
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT || mode == DIRECT_X) 
    {
        if ((cpu->register_direct & 0xFF) != 0) cpu->cycles += 1;
    }

    cpu_set_zero_flag(&cpu->status, result);
}
void cpu_inx(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_x = (unsigned char)cpu->register_x + 1;
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_x);
    }
    else
    {
        cpu->register_x++;
        cpu_set_negative_u16(&cpu->status, cpu->register_x);
    }
    cpu_set_zero_flag(&cpu->status, cpu->register_x);
}
void cpu_iny(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->register_y = (unsigned char)cpu->register_y + 1;
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_y);
    }
    else
    {
        cpu->register_y++;
        cpu_set_negative_u16(&cpu->status, cpu->register_y);
    }
    cpu_set_zero_flag(&cpu->status, cpu->register_y);
}

void cpu_and(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned short old = cpu->register_a;
        
        cpu->register_a = ((unsigned char)cpu->register_a & cpu->wram[addr % 0x1FFFF]);
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;

        switch (mode)
        {
            default: break;
            case DIRECT_SHORT_Y:
            case ABSOLUTE_X:
            case ABSOLUTE_Y:
                if (cpu->status & INDEX_WIDTH)
                {
                    cpu->cycles -= 1;
                    if ((old >> 8) != (cpu->register_a >> 8))
                        cpu->cycles += 1;
                }
                break;
        }
    }
    else
    {
        cpu->register_a &= cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, cpu->register_a);
    }

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}
void cpu_eor(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned short old = cpu->register_a;

        cpu->register_a = (unsigned char)cpu->register_a ^ cpu->wram[addr % 0x1FFFF];
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;

        switch (mode)
        {
            default: break;
            case DIRECT_SHORT_Y:
            case ABSOLUTE_X:
            case ABSOLUTE_Y:
                if (cpu->status & INDEX_WIDTH)
                {
                    cpu->cycles -= 1;
                    if ((old >> 8) != (cpu->register_a >> 8))
                        cpu->cycles += 1;
                }
                break;
        }
    }
    else
    {
        cpu->register_a ^= cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, cpu->register_a);
    }

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}
void cpu_ora(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned short old = cpu->register_a;
        
        cpu->register_a = (unsigned char)cpu->register_a | cpu->wram[addr % 0x1FFFF];
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;

        switch (mode)
        {
            default: break;
            case DIRECT_SHORT_Y:
            case ABSOLUTE_X:
            case ABSOLUTE_Y:
                if (cpu->status & INDEX_WIDTH)
                {
                    cpu->cycles -= 1;
                    if ((old >> 8) != (cpu->register_a >> 8))
                        cpu->cycles += 1;
                }
                break;
        }
    }
    else
    {
        cpu->register_a |= cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, cpu->register_a);
    }

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}

void cpu_bit(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode); 
    unsigned short  result;

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned short old = cpu->register_a;
        result = (unsigned char)cpu->register_a & cpu->wram[addr % 0x1FFFF];

        switch (mode)
        {
            default: break;
            case DIRECT:
            case ABSOLUTE:
            case DIRECT_X:
                cpu_set_negative_u8(&cpu->status, result);
                cpu_set_overflow_flag_u8(
                    &cpu->status, 
                    (unsigned char)cpu->register_a, 
                    cpu->wram[addr % 0x1FFFF], 
                    (unsigned char)result);
                break;
            case ABSOLUTE_X:
                cpu_set_negative_u8(&cpu->status, result);
                cpu_set_overflow_flag_u8(
                    &cpu->status, 
                    (unsigned char)cpu->register_a, 
                    cpu->wram[addr % 0x1FFFF], 
                    (unsigned char)result);

                    if ((old >> 8) != (result >> 8))
                        cpu->cycles += 1;
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
                cpu_set_negative_u16(&cpu->status, result);
                cpu_set_overflow_flag_u16(
                    &cpu->status, 
                    cpu->register_a,
                    cpu_mem_read_u16(cpu, addr),
                    result);
                break;
        }
    }

    if (mode == ABSOLUTE_X && cpu->status & INDEX_WIDTH)
        cpu->cycles -= 1;

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
    }

    cpu_set_zero_flag(&cpu->status, result);
}

void cpu_trb(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  result;

    if (cpu->status & MEMORY_WIDTH)
    {
        result = (unsigned char)cpu->register_a & cpu->wram[addr % 0x1FFFF];
        cpu->wram[addr % 0x1FFFF] &= (unsigned char)~cpu->register_a;
        cpu->cycles -= 2;
    }
    else
    {
        result = cpu->register_a & cpu_mem_read_u16(cpu, addr);
        cpu_mem_write_u16(cpu, addr, cpu->register_a & ~cpu_mem_read_u16(cpu, addr));
    }

    if (mode == DIRECT)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
    }

    cpu_set_zero_flag(&cpu->status, result);
}
void cpu_tsb(enum AddressingMode mode, CPU *cpu)
{
    unsigned int    addr = cpu_get_operand_address(cpu, mode);
    unsigned short  result;

    if (cpu->status & MEMORY_WIDTH)
    {
        result = (unsigned char)cpu->register_a & cpu->wram[addr % 0x1FFFF];
        cpu->wram[addr % 0x1FFFF] |= (unsigned char)cpu->register_a;
        cpu->cycles -= 2;
    }
    else
    {
        result = cpu->register_a & cpu_mem_read_u16(cpu, addr);
        cpu_mem_write_u16(cpu, addr, cpu_mem_read_u16(cpu, addr) | cpu->register_a);
    }

    if (mode == DIRECT)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
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

            old_bits = cpu->wram[addr % 0x1FFFF];
            cpu->wram[addr] = cpu->wram[addr % 0x1FFFF] << 1;
            result = cpu->wram[addr % 0x1FFFF];

            cpu->cycles -= 2;
        }

        if ((old_bits & 0b10000000) != 0)
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_negative_u8(&cpu->status, result);
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

        cpu_set_zero_flag(&cpu->status, result);
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
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

            old_bits = cpu->wram[addr % 0x1FFFF];
            cpu->wram[addr % 0x1FFFF] >>= 1;
            cpu->wram[addr % 0x1FFFF] &= 0b01111111;
            result = cpu->wram[addr % 0x1FFFF];

            cpu->cycles -= 2;
        }

        if ((old_bits & 0b10000000) != 0)
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag(&cpu->status, result);
        cpu_set_negative_u8(&cpu->status, result);
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

        cpu_set_zero_flag(&cpu->status, result);
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
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

            old_bits = cpu->wram[addr % 0x1FFFF];
            shift = cpu->wram[addr % 0x1FFFF] << 1;
            rotate = cpu->wram[addr % 0x1FFFF] >> 7;

            cpu->wram[addr % 0x1FFFF] = (unsigned char)(shift | rotate);
            result = &cpu->wram[addr % 0x1FFFF];

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

        cpu_set_zero_flag(&cpu->status, *result);
        cpu_set_negative_u8(&cpu->status, *result);
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

        cpu_set_zero_flag(&cpu->status, result);
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
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
            rotate = low_acc << 8;

            cpu->register_a = (unsigned char)(shift | rotate);
            result = (unsigned char)cpu->register_a;
        }
        else
        {
            unsigned int addr = cpu_get_operand_address(cpu, mode);

            old_bits = cpu->wram[addr % 0x1FFFF];
            shift = cpu->wram[addr % 0x1FFFF] >> 1;
            rotate = cpu->wram[addr % 0x1FFFF] << 8;

            cpu->wram[addr % 0x1FFFF] = shift | rotate;
            result = cpu->wram[addr % 0x1FFFF];

            cpu->cycles -= 2;
        }

        if (carry)  result |= 0b10000000;
        else        result &= 0b01111111;

        if (old_bits & 0b00000001) 
            cpu->status = cpu->status | CARRY_FLAG;
        else 
            cpu->status = cpu->status & 0b11111110;

        cpu_set_zero_flag(&cpu->status, result);
        cpu_set_negative_u8(&cpu->status, result);
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

        cpu_set_zero_flag(&cpu->status, result);
        cpu_set_negative_u16(&cpu->status, result);
    }

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
    }
}

void cpu_bcc(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if ((cpu->status & CARRY_FLAG) == 0)
    {
        unsigned short old = cpu->program_counter + 1;

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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

    cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];

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

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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

        cpu->program_counter += (char)cpu->wram[addr % 0x1FFFF];
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
        cpu_mem_write_u16(cpu, (cpu->stack_pointer - 1) % 0x0100, cpu->program_counter + 2);
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
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 2);
        cpu->stack_pointer -= 2;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0xFFE6);
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
        cpu_mem_write_u16(cpu, (cpu->stack_pointer - 1) % 0x0100, cpu->program_counter + 2);
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
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu->program_counter + 2);
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
        cpu_mem_write_u16(cpu, (cpu->stack_pointer - 1) % 0x100, cpu->program_counter + 1);
        cpu->stack_pointer -= 2 % 0x100;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1 % 0x100;
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
        cpu_mem_write_u16(cpu, (cpu->stack_pointer - 1) % 0x100, cpu->program_counter + 1);
        cpu->stack_pointer -= 2 % 0x100;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1 % 0x100;
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
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1 % 0x100, cpu->program_counter + 1);
        cpu->stack_pointer -= 2 % 0x100;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1 % 0x100;
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
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1 % 0x100, cpu->program_counter + 1);
        cpu->stack_pointer -= 2 % 0x100;
        cpu_mem_write_u8(cpu, cpu->stack_pointer, cpu->status);
        cpu->stack_pointer -= 1 % 0x100;
        cpu->program_counter = cpu_mem_read_u16(cpu, 0x00FFFC);
    }
}

void cpu_rti(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->emulation_mode)
    {
        // In emulation mode, the P register is pulled, then the 16-bit program counter is pulled (again low byte first, then high byte). 
        cpu->stack_pointer += 1 % 0x100;
        cpu->status = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu->stack_pointer += 2 % 0x100;
        cpu->program_counter = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1 % 0x100);
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
        cpu->status = cpu->status & ~cpu->wram[addr % 0x1FFFF];
        cpu->status |= 0b00110000; // turn on memory width and index width
    }
    else
    {
        cpu->status = cpu->status & ~cpu->wram[addr % 0x1FFFF];
    }
}
void cpu_sep(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->emulation_mode)
    {
        cpu->status |= cpu->wram[addr % 0x1FFFF];
        cpu->status |= 0b00110000; // turn on memory width and index width
    }
    else
    {
        cpu->status |= cpu->wram[addr % 0x1FFFF];
    }
}

void cpu_lda(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & MEMORY_WIDTH)
    {
        unsigned short old = cpu->register_a;
        cpu->register_a = cpu_mem_read_u8(cpu, addr);
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;

        if (cpu->status & INDEX_WIDTH)
        {
            if (mode == ABSOLUTE_X 
            || mode == ABSOLUTE_Y 
            || mode == DIRECT_SHORT_Y)
                cpu->cycles -= 1;

            if ((old >> 8) != (cpu->register_a >> 8))
                cpu->cycles += 1;
        }
    }
    else
    {
        cpu->register_a = cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, cpu->register_a);
    }

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }
    
    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}
void cpu_ldx(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & INDEX_WIDTH)
    {
        unsigned short old = cpu->register_x;
        cpu->register_x = cpu_mem_read_u8(cpu, addr);
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_x);
        cpu->cycles -= mode == ABSOLUTE_Y ? 2 : 1;

        if (mode == ABSOLUTE_X)
        {
            cpu->cycles -= 2;
            if ((old >> 8) != (cpu->register_x >> 8))
                cpu->cycles += 1;
        }
        else cpu->cycles -= 1;
    }
    else
    {
        cpu->register_x = cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, cpu->register_x);
    }

    if (mode == DIRECT || mode == DIRECT_Y)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
    }
    
    cpu_set_zero_flag(&cpu->status, cpu->register_x);
}
void cpu_ldy(enum AddressingMode mode, CPU *cpu)
{
    unsigned int addr = cpu_get_operand_address(cpu, mode);

    if (cpu->status & INDEX_WIDTH)
    {
        unsigned short old = cpu->register_y;
        cpu->register_y = cpu_mem_read_u8(cpu, addr);
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_y);

        if (mode == ABSOLUTE_X)
        {
            cpu->cycles -= 2;
            if ((old >> 8) != (cpu->register_y >> 8))
                cpu->cycles += 1;
        }
        else cpu->cycles -= 1;
    }
    else
    {
        cpu->register_y = cpu_mem_read_u16(cpu, addr);
        cpu_set_negative_u16(&cpu->status, cpu->register_y);
    }

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
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

    switch (mode)
    {
        default: break;
        case DIRECT:
        case DIRECT_X:
        case DIRECT_SHORT:
        case DIRECT_SHORT_X:
        case DIRECT_SHORT_Y:
        case DIRECT_LONG:
        case DIRECT_LONG_Y:
            if ((cpu->register_direct & 0xFF) != 0)
                cpu->cycles += 1;
            break;
    }
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

    if (mode == DIRECT || mode == DIRECT_Y)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
    }
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

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
    }
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

    if (mode == DIRECT || mode == DIRECT_X)
    {
        if ((cpu->register_direct & 0xFF) != 0)
            cpu->cycles += 1;
    }
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
    if (cpu->emulation_mode && (cpu->register_direct & 0xFF) == 0)
    {
        unsigned char base8 = cpu_mem_read_u8(cpu, cpu->program_counter + 1);
        unsigned int short addr = ((cpu->register_direct & 0xFF00) + base8) % 0x10000;
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1 % 0x100, cpu_mem_read_u16(cpu, addr));
        cpu->stack_pointer -= 2 % 0x100;
    }
    else
    {
        unsigned int addr = cpu_get_operand_address(cpu, mode);
        cpu_mem_write_u16(cpu, cpu->stack_pointer - 1, cpu_mem_read_u16(cpu, addr));
        cpu->stack_pointer -= 2;
    }

    if ((cpu->register_direct & 0xFF) != 0)
        cpu->cycles += 1;
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
        cpu->stack_pointer = (cpu->stack_pointer - 1) % 0x100;
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
        cpu->stack_pointer = (cpu->stack_pointer - 1) % 0x100;
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
        cpu->stack_pointer = (cpu->stack_pointer - 1) % 0x100;
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
        cpu->stack_pointer = (cpu->stack_pointer + 1) % 0x100;
        cpu->register_a = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_a);
        cpu->cycles -= 1;
    }
    else
    {
        cpu->stack_pointer += 2;
        cpu->register_a = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu_set_negative_u16(&cpu->status, cpu->register_a);
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
}
void cpu_plx(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & INDEX_WIDTH)
    {
        cpu->stack_pointer = (cpu->stack_pointer + 1) % 0x100;
        cpu->register_x = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_x);
        cpu->cycles -= 1;
    }
    else
    {
        cpu->stack_pointer += 2;
        cpu->register_x = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu_set_negative_u16(&cpu->status, cpu->register_x);
    }

    cpu_set_zero_flag(&cpu->status, cpu->register_x);
}
void cpu_ply(enum AddressingMode mode, CPU *cpu)
{
    if (cpu->status & MEMORY_WIDTH)
    {
        cpu->stack_pointer = (cpu->stack_pointer + 1) % 0x100;
        cpu->register_y = cpu_mem_read_u8(cpu, cpu->stack_pointer);
        cpu_set_negative_u8(&cpu->status, (unsigned char)cpu->register_y);
        cpu->cycles -= 1;
    }
    else
    {
        cpu->stack_pointer += 2;
        cpu->register_y = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
        cpu_set_negative_u16(&cpu->status, cpu->register_y);
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
    cpu_set_negative_u8(&cpu->status, cpu->program_bank);
}
void cpu_pld(enum AddressingMode mode, CPU *cpu)
{
    cpu->stack_pointer += 2;
    cpu->register_direct = cpu_mem_read_u16(cpu, cpu->stack_pointer - 1);
    cpu_set_negative_u16(&cpu->status, cpu->register_direct);
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
        else cpu->stack_pointer = cpu->register_x;
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
    cpu_set_zero_flag(&cpu->status, cpu->register_direct);
    cpu_set_negative_u16(&cpu->status, cpu->register_direct);
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
    cpu_set_zero_flag(&cpu->status, cpu->register_a);
    cpu_set_negative_u16(&cpu->status, cpu->register_a);
}
void cpu_tsc(enum AddressingMode mode, CPU *cpu)
{
    cpu->register_a = cpu->stack_pointer;
    cpu_set_zero_flag(&cpu->status, cpu->register_a);
    cpu_set_negative_u16(&cpu->status, cpu->register_a);
}

void cpu_xba(enum AddressingMode mode, CPU *cpu)
{
    unsigned char   reg_h = cpu->register_a >> 8,
                    reg_l = cpu->register_a & 0xFF;

    cpu->register_a = (unsigned short)reg_h >> 8 | (unsigned short)reg_l << 8;

    cpu_set_zero_flag(&cpu->status, cpu->register_a);
    cpu_set_negative_u16(&cpu->status, cpu->register_a);
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

void cpu_set_overflow_flag_u8(unsigned char *status, unsigned char reg, 
                            unsigned char arg, unsigned char result)
{
    if (~(reg ^ arg) & (reg ^ result) & 0x80)   
        *status |= OVERFLOW_FLAG;
    else                                        
        *status &= 0b10111111;
}
void cpu_set_overflow_flag_u16(unsigned char *status, unsigned short reg, 
                            unsigned short arg, unsigned short result)
{
    if (~(reg ^ arg) & (reg ^ result) & 0x8000) 
        *status |= OVERFLOW_FLAG;
    else                                        
        *status &= 0b10111111;
}

void cpu_set_zero_flag(unsigned char *status, unsigned short result)
{
    if (result == 0)    *status |= ZERO_FLAG;
    else                *status &= 0b11111101;
}

void cpu_set_negative_u8(unsigned char *status, unsigned char result)
{
    if (result & 0x80)      *status |= NEGATIVE_FLAG;
    else                    *status &= 0b011111111;
}

void cpu_set_negative_u16(unsigned char *status, unsigned short result)
{
    if (result & 0x8000)    *status |= NEGATIVE_FLAG;
    else                    *status &= 0b011111111;
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

    if (cpu->bus.ppu.nmi_vblank && !cpu->bus.ppu.nmi_write)
    {
        cpu_interrupt_nmi(cpu);
        cpu->bus.ppu.nmi_write = 1;
    }

    unsigned char operand = cpu_mem_read_u8(cpu, cpu->program_counter);
                    
    unsigned long cpu_cycles = cpu->cycles;

    //printf("PC:%X OP:%X\n", cpu->program_counter, operand);

    switch (operand)
    {
        case 0x00:
            cpu_brk(IMPLIED, cpu);
            cpu->cycles += 8;
            break;
        case 0x01:
            cpu_ora(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0x02:
            cpu_cop(IMMEDIATE, cpu);
            cpu->cycles += 8;
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & INDEX_WIDTH ? 1 : 0);
            break;
        case 0xA1:
            cpu_lda(DIRECT_SHORT_X, cpu);
            cpu->cycles += 7;
            cpu->program_counter += 2;
            break;
        case 0xA2:
            cpu_ldx(IMMEDIATE, cpu);
            cpu->cycles += 3;
            cpu->program_counter += 3 - (cpu->status & INDEX_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & INDEX_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & INDEX_WIDTH ? 1 : 0);
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
            cpu->program_counter += 3 - (cpu->status & MEMORY_WIDTH ? 1 : 0);
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

    // check for interrupts

    unsigned char   nmi_before = cpu->bus.ppu.nmi_vblank, 
                    op_cycles = cpu->cycles - cpu_cycles;
    
    cpu->bus.ppu.dot_cycles += op_cycles * 4;

    if (cpu->bus.ppu.dot_cycles >= 341)
    {
        cpu->bus.ppu.dot_cycles -= 341;
        cpu->bus.ppu.scanline += 1;

        if (cpu->bus.ppu.scanline == 241)
        {
            cpu->rdnmi |= 0b10000000;

            if (cpu->nmitimen & 0b10000000)
                cpu->bus.ppu.nmi_vblank = 1;
        }

        if (cpu->bus.ppu.scanline == 262)
        {
            cpu->bus.ppu.scanline = 0;
            cpu->bus.ppu.nmi_vblank = 0;
            cpu->bus.ppu.nmi_write = 0;
            cpu->rdnmi &= 0b01111111;
        }
    }

    if (cpu->hdmaen & 0b1) dma_hdma_write(cpu, 0);
    if (cpu->hdmaen & 0b10) dma_hdma_write(cpu, 1);
    if (cpu->hdmaen & 0b100) dma_hdma_write(cpu, 2);
    if (cpu->hdmaen & 0b1000) dma_hdma_write(cpu, 3);
    if (cpu->hdmaen & 0b10000) dma_hdma_write(cpu, 4);
    if (cpu->hdmaen & 0b100000) dma_hdma_write(cpu, 5);
    if (cpu->hdmaen & 0b1000000) dma_hdma_write(cpu, 6);
    if (cpu->hdmaen & 0b10000000) dma_hdma_write(cpu, 7);
    

    if (!nmi_before && cpu->bus.ppu.nmi_vblank)
        cpu_callback();
}

void cpu_init(CPU *cpu)
{
    cpu->htimer = 0x1FF;
    cpu->vtimer = 0x1FF;

    cpu->rdnmi &= 0x7F;
    cpu->timeup &= 0x7F;

    cpu->dividend.hi = 0xFF;
    cpu->dividend.lo = 0xFF;
    cpu->multiply._a = 0xFF;
    cpu->wrio = 0xFF;

    cpu->mdmaen = 0;
    cpu->hdmaen = 0;
    cpu->nmitimen = 0;

    cpu->register_a = 0;
    cpu->register_x = 0;
    cpu->register_y = 0;
    cpu->register_direct = 0;
    cpu->data_bank = 0;
    cpu->program_bank = 0;

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

    cpu->program_counter = cpu_mem_read_u16(cpu, 0xFFFC);
}
