; useful resources
; http://dbp-consulting.com/tutorials/debugging/linuxProgramStartup.html
; http://www.embecosm.com/appnotes/ean9/html/ch05s02.html

section .data

global _stack

_stack:

section .text

global _start
extern main

_start:
    call main
