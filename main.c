#include <GL/glew.h>
#include <GL/glut.h>
#include "snes_emu.h"

CPU cpu;

unsigned char key_states[256], frame_data[FRAME_LENGTH];
unsigned int texture;

void key_down(unsigned char key, int x, int y)
{
    key_states[key] = 1;
}
void key_up(unsigned char key, int x, int y)
{
    key_states[key] = 0;
}

void input()
{
    if (key_states[','])    cpu.bus.joypad[0].btn_status |= JOY_B;
    else                    cpu.bus.joypad[0].btn_status &= 0b0111111111111111;

    if (key_states['k'])    cpu.bus.joypad[0].btn_status |= JOY_Y;
    else                    cpu.bus.joypad[0].btn_status &= 0b1011111111111111;

    if (key_states[8])      cpu.bus.joypad[0].btn_status |= JOY_SELECT;
    else                    cpu.bus.joypad[0].btn_status &= 0b1101111111111111;

    if (key_states[13])     cpu.bus.joypad[0].btn_status |= JOY_START;
    else                    cpu.bus.joypad[0].btn_status &= 0b1110111111111111;

    if (key_states['w'])    cpu.bus.joypad[0].btn_status |= JOY_UP;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111011111111111;

    if (key_states['s'])    cpu.bus.joypad[0].btn_status |= JOY_DOWN;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111101111111111;

    if (key_states['a'])    cpu.bus.joypad[0].btn_status |= JOY_LEFT;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111110111111111;

    if (key_states['d'])    cpu.bus.joypad[0].btn_status |= JOY_RIGHT;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111111011111111;

    if (key_states['.'])    cpu.bus.joypad[0].btn_status |= JOY_A;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111111101111111;

    if (key_states['l'])    cpu.bus.joypad[0].btn_status |= JOY_X;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111111110111111;

    if (key_states['c'])    cpu.bus.joypad[0].btn_status |= JOY_L;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111111111011111;

    if (key_states['v'])    cpu.bus.joypad[0].btn_status |= JOY_R;
    else                    cpu.bus.joypad[0].btn_status &= 0b1111111111101111;
}

void render()
{
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 240, GL_RGB, GL_UNSIGNED_BYTE, frame_data);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, 1.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(+1.0f, 1.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(+1.0f, -1.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, -1.0f);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glutSwapBuffers();
    glFlush();
}

void cpu_callback()
{
    ppu_render(&cpu.bus.ppu);
    render();
    input();
}

void loop()
{
    cpu_interpret(&cpu);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(256, 240);
    glutCreateWindow("SNES Emulator");

    // setup texture
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, frame_data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    glutDisplayFunc(render);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);
    glutIdleFunc(loop);

    for (int i = 0; i < 256; i++)
        key_states[i] = 0;

    cpu.bus.ppu.frame_data = frame_data;

    rom_load(&cpu.bus);

    cpu_init(&cpu);

    glutMainLoop();

    if (cpu.bus.rom_buffer != NULL)
        free(cpu.bus.rom_buffer);

    glDeleteTextures(1, &texture);

    return 0;
}
