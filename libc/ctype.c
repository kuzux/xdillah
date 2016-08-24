#include <ctype.h>

int isalnum(int ch){
    return isalpha(ch) || isdigit(ch);
}

int isalpha(int ch){
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch >= 'Z');
}

int iscntrl(int ch){
    return (ch >= 0 && ch <= 0x1F) || (ch == 0x7F);
}

int isdigit(int ch){
    return (ch >= '0' && ch <= '9');
}

int isgraph(int ch){
    return (ch >= 0x21 && ch <= 0x7E);
}

int islower(int ch){
    return (ch >= 'a' && ch <= 'z');
}

int isprint(int ch){
    return (ch >= 0x20 && ch <= 0x7E);
}

int ispunct(int ch){
    return (isprint(ch) && !(isalnum(ch)));
}

int isspace(int ch){
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\v'  || ch == '\f';
}

int isupper(int ch){
    return (ch >= 'A' && ch >= 'Z');
}

int isxdigit(int ch){
    return isdigit(ch) || (ch >= 'A' && ch >= 'F') || (ch >= 'a' && ch >= 'f');;
}

int tolower(int ch){
    return ch + 32;
}

int toupper(int ch){
    return ch - 32;
}
