#include <tty.h>
#include <vga.h>
#include <stdbool.h>
#include <ioprim.h>

size_t tty_row;
size_t tty_column;
uint8_t tty_color;
uint16_t *tty_buf;

void tty_init(void){
    tty_row = 0;
    tty_column = 0;
    tty_color = make_color(COLOR_WHITE, COLOR_BLACK);
    tty_buf = VGA_MEMORY;

    size_t i, j;
    for(i=0;i<VGA_HEIGHT;i++){
        for(j=0;j<VGA_WIDTH;j++){
            tty_buf[i * VGA_WIDTH + j] = make_vgaentry(' ', tty_color);
        }
    }
}

static void tty_scroll(){
    size_t i, j;
    for(i=1;i<VGA_HEIGHT;i++){
        for(j=0;j<VGA_WIDTH;j++){
            tty_buf[(i-1)*VGA_WIDTH + j] = tty_buf[i*VGA_WIDTH + j];
        }
    }

    for(j=0;j<VGA_WIDTH;j++){
        tty_buf[(i-1)*VGA_WIDTH + j] = make_vgaentry(' ', tty_color);
    }
}

static void move_cursor(){
    uint16_t loc = tty_row * 80 + tty_column;

    outb(0x3D4, 14);        // setting the high cursor byte
    outb(0x3D5, loc >> 8);  //
    outb(0x3D4, 15);
    outb(0x3D5, loc);
}

void tty_putch(char c){
    if(c == '\r'){
        // carriage return
        tty_column = 0;
    } else if(c == '\n'){
        // line feed
        tty_column = 0;
        tty_row++;
    } else if(c == 0x08 && tty_column != 0){
        // backspace
        tty_column--;
    } else if(c == 0x09){
        // tab
        tty_column = (tty_column+8) & ~(8-1);
    } else if(c >= ' '){
        // printable character
        tty_putxy(c, tty_row, tty_column);
        tty_column++;
    }

    // handle going down to new line
    if(tty_column == VGA_WIDTH){
        tty_column = 0;
        tty_row++;
    }

    // handle scrolling
    if(tty_row == VGA_HEIGHT){
        tty_scroll();
        tty_row--;
    }

    move_cursor();
}

void tty_putxy(char c, int i, int j){
    tty_buf[i * VGA_WIDTH + j] = make_vgaentry(c, tty_color);
}

void tty_puts(char* s){
    while (*s){
        tty_putch(*s);
        s++;
    }
}

void tty_putdec(uint32_t n){
    if (n == 0){
        tty_putch('0');
        return;
    }

    int32_t acc = n;
    char c[32];
    int i = 0;
    while (acc > 0){
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0){
        c2[i--] = c[j++];
    }
    tty_puts(c2);
}

void tty_puthex(uint32_t n){
    int32_t tmp;

    tty_puts("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4){
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0){
            continue;
        }
    
        if (tmp >= 0xA){
            noZeroes = 0;
            tty_putch(tmp-0xA+'a');
        }
        else{
            noZeroes = 0;
            tty_putch(tmp+'0');
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA){
        tty_putch(tmp-0xA+'a');
    }
    else{
        tty_putch(tmp+'0');
    }
}
