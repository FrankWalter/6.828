#include "ns.h"

extern union Nsipc nsipcbuf;

void
output(envid_t ns_envid)
{
    // LAB 6: Your code here:
	// 	- read a packet from the network server
	//	- send the packet to the device driver
	binaryname = "ns_output";
	int32_t reqno;
	uint32_t whom;
	int perm;
    int r;
    
    while(1)
    {
        reqno = ipc_recv((int32_t *) &whom, &nsipcbuf, &perm);
        if (reqno == NSREQ_OUTPUT)
            while ((r = sys_tx_send(E1000_DEFAULT_BUS, 
                                 E1000_DEFAULT_DEV, 
                                 E1000_DEFAULT_FUN, 
                                 nsipcbuf.pkt.jp_data, 
                                 nsipcbuf.pkt.jp_len)) < 0);
    }
}
