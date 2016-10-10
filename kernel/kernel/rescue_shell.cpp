#include <stdio.h>

#include <kernel/rescue_shell.h>
#include <arch/i386/keyboard.h>

void enter_rescue () {
    while (true) {
        printf ("$> ");
        printf ("%s", gets ());
    }
}
