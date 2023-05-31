//#include <GL/glew.h>
//#include <GL/glut.h>
#include "snes_emu.h"

int main(int argc, char *argv[])
{
    CPU snes_cpu;

    rom_load(&snes_cpu.bus);

    if (snes_cpu.bus.rom_buffer != NULL)
    {
        free(snes_cpu.bus.rom_buffer);
    }

    return 0;
}
