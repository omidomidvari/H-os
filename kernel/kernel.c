/* ===============================
    (c) 2026 hommemovie corpration. all rights reserved.
    ==============================
    please do not use or distribute this code without permission even if its on github. its supposed to be there.
        - hommemovie
        ==============================
        This is the main kernel code for H-OS, a minimal 64-bit operating system. It includes basic VGA text output, keyboard input handling, and a simple shell with a few commands. The kernel is designed to be as small and simple as possible while still providing a functional environment for users to interact with.
        ==============================
        Note: This code is intended for educational purposes and may not be suitable for production use. It lacks many features of a full operating system, such as memory management, multitasking, and file systems.
        ===============================
        also anyone can help with the project just ask update it.
        i just realized is it 10 pm?
    who is coding at 10 pm? its 10:30 pm. i should go to sleep. good night.
    hell nah i just woke up. its 10:30 pm no quitting coding now. this kernel has to be done by the end of the week. i have to finish it.
    im just weird....
    ============================ */

/* ===============================
   H-OS Minimal 64-bit Kernel
   =============================== */

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/* VGA */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile u16* vga = (volatile u16*)0xB8000;
u8 row = 0;
u8 col = 0;
u8 color = 0x0F;

/* ===============================
   Port I/O
   =============================== */
static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
}

/* ===============================
   VGA Text Output
   =============================== */
void putchar(char c) {
    if (c == '\n') {
        col = 0;
        row++;
        return;
    }

    vga[row * VGA_WIDTH + col] = ((u16)color << 8) | c;
    col++;

    if (col >= VGA_WIDTH) {
        col = 0;
        row++;
    }
}

void print(const char* s) {
    while (*s)
        putchar(*s++);
}

void clear_screen() {
    for (u32 i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
        vga[i] = ((u16)color << 8) | ' ';
    row = 0;
    col = 0;
}

/* ===============================
   Keyboard (minimal US layout)
   =============================== */
char keymap[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' '
};

char getch() {
    while (!(inb(0x64) & 1));
    u8 sc = inb(0x60);
    if (sc > 127) return 0;
    return keymap[sc];
}

/* ===============================
   String Compare
   =============================== */
int streq(const char* a, const char* b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return (*a == 0 && *b == 0);
}

/* ===============================
   Commands
   =============================== */
void cmd_listdir() {
    print("bin  boot  dev  etc  home\n");
}

void cmd_mkdir() {
    print("mkdir: directory created\n");
}

void cmd_reboot() {
    print("Rebooting...\n");
    outb(0x64, 0xFE);
    for (;;)
        __asm__ volatile ("hlt");
}

void cmd_shutdown() {
    print("System halted.\n");
    for (;;)
        __asm__ volatile ("cli; hlt");
}

/* ===============================
   Shell
   =============================== */
void shell() {
    char buf[64];
    u32 i;

    while (1) {
        print("#> ");
        i = 0;

        while (1) {
            char c = getch();
            if (!c) continue;

            if (c == '\n') {
                putchar('\n');
                buf[i] = 0;
                break;
            }

            if (c == '\b' && i > 0) {
                i--;
                col--;
                putchar(' ');
                col--;
                continue;
            }

            buf[i++] = c;
            putchar(c);
        }

        if (streq(buf, "listdir")) {
            cmd_listdir();
        } else if (streq(buf, "mkdir")) {
            cmd_mkdir();
        } else if (streq(buf, "md")) {
            cmd_mkdir();
        } else if (streq(buf, "reboot")) {
            cmd_reboot();
        } else if (streq(buf, "shutdown")) {
            cmd_shutdown();
        } else {
            print("Unknown command\n");
        }
    }
}

/* ===============================
   Kernel Entry
   =============================== */
void kmain() {
    clear_screen();

    /* EXACT startup banner */
    print("H os hello world!\n");

    print("[ OK ] kernel start\n");
    print("[ OK ] vga ready\n");
    print("[ OK ] keyboard ready\n\n");

    shell();
}

