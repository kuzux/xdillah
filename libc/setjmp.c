#include <setjmp.h>

extern void _setjmp(jmp_buf*);
extern void _longjmp(jmp_buf);

int setjmp(jmp_buf env){
    _setjmp(&env);
    return 0; 
}

void longjmp(jmp_buf env, int ret){
    _longjmp(env);
}
