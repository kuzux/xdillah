#include <kb.h>
#include <isr.h>
#include <tty.h>
#include <ioprim.h>

#define SC_CTRL 29
#define SC_LSHIFT 42
#define SC_RSHIFT 54
#define SC_ALT 56
#define SC_CAPSLOCK 58

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
uint8_t kbdus[128] =
{
    0,  27, /* Escape */
    '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',         /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,        /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
  'm', ',', '.', '/',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

uint8_t kbdus_shift[128] = {
    0,  27, /* Escape */
    '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
  '(', ')', '_', '+', '\b', /* Backspace */
  '\t',         /* Tab */
  'Q', 'W', 'E', 'R',   /* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
    0,          /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
 '"', '~',   0,        /* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
  'M', '<', '>', '?',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

kb_status_t kbstat;

static void kb_handler(){
    uint8_t scancode;

    scancode = inb(0x60);

    if(scancode & 0x80){
        // key released
        scancode &= (~0x80);
        if(scancode == SC_CTRL){
            kbstat.ctrl = 0;
        } else if(scancode == SC_LSHIFT || scancode == SC_RSHIFT){
            kbstat.shift = 0;
        } else if(scancode == SC_ALT){
            kbstat.alt = 0;
        }
    } else {
        if(scancode == SC_CTRL){
            kbstat.ctrl = 1;
        } else if(scancode == SC_LSHIFT || scancode == SC_RSHIFT){
            kbstat.shift = 1;
        } else if(scancode == SC_ALT){
            kbstat.alt = 1;
        } else if(scancode == SC_CAPSLOCK){
            kbstat.capslock = !kbstat.capslock;
        } else if(kbdus[scancode]){
            uint8_t shiftmod = kbstat.shift;

            // caps lock is only affecting alphabetical characters
            if(kbstat.capslock && kbdus[scancode] >= 'a' && kbdus[scancode] <= 'z') 
                shiftmod = !shiftmod;
            
            if(shiftmod) 
                tty_putch(kbdus_shift[scancode]);
            else 
                tty_putch(kbdus[scancode]);
        } 
    }
}

void kb_init(){
    register_interrupt_handler(IRQ1, kb_handler);
}

kb_status_t getkbstat(){
    return kbstat;
}
