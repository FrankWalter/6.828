// hello, world
#include <inc/lib.h>
char test[] = "hello, world!";
void
umain(int argc, char **argv)
{
    sys_tx_send(0, 0x3, 0, (void*)test, sizeof test);
	cprintf("hello, world\n");
	cprintf("i am environment %08x\n", thisenv->env_id);
}
