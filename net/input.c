#include "ns.h"

extern union Nsipc nsipcbuf;

void
input(envid_t ns_envid)
{
	binaryname = "ns_input";

	// LAB 6: Your code here:
	// 	- read a packet from the device driver
	//	- send it to the network server
	// Hint: When you IPC a page to the network server, it will be
	// reading from it for a while, so don't immediately receive
	// another packet in to the same physical page.
    int32_t reqno;
	uint32_t whom;
	int perm;
    uint32_t start;
    uint32_t end;
    
    cprintf("envid in input is [%08x]\n", ns_envid);
    while(1)
    {
        while ((nsipcbuf.pkt.jp_len = sys_rx_receive(E1000_DEFAULT_BUS, 
                                                     E1000_DEFAULT_DEV, 
                                                     E1000_DEFAULT_FUN, 
                                                     nsipcbuf.pkt.jp_data)) < 0);
        ipc_send(ns_envid, NSREQ_INPUT, &nsipcbuf, PTE_P|PTE_W|PTE_U);
        start = sys_time_msec();
        while(sys_time_msec() - start < 100)
            sys_yield();
    }
}
