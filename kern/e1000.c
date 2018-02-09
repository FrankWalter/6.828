// LAB 6: Your driver code here
#include <inc/string.h>
#include <kern/e1000.h>
#include <kern/pmap.h>

const uint32_t e1000_tctl_default = 0x4010a;
const uint32_t e1000_tipg_default = 0x60100a;
const unsigned int buffer_size = 42;
const unsigned int tdlen = 16;

void tx_init(struct pci_func *pcif)
{
    int i;
    struct PageInfo* pp;
    
    if (!(pp = page_alloc(ALLOC_ZERO)))
        panic("out of memory during tx_init!");
    pcif->tx_desc_ring = (struct tx_desc*)page2kva(pp);
    
    pci_write_conf_uint32_t(pcif, E1000_TDBAL, PADDR((void*)pcif->tx_desc_ring));
    pci_write_conf_uint32_t(pcif, E1000_TDBAH, 0);
    pci_write_conf_uint32_t(pcif, E1000_TDLEN, tdlen * sizeof(struct tx_desc));
    pci_write_conf_uint32_t(pcif, E1000_TDH, 0);
    pci_write_conf_uint32_t(pcif, E1000_TDT, 0);
    pci_write_conf_uint32_t(pcif, E1000_TCTL, e1000_tctl_default);
    pci_write_conf_uint32_t(pcif, E1000_TIPG, e1000_tipg_default);
    
    for (i = 0; i < tdlen; i++)
    {
        if (!(pp = page_alloc(ALLOC_ZERO)))
            panic("out of memory during tx_init!");
        pcif->tx_desc_ring[i].addr = page2pa(pp);
        pcif->tx_desc_ring[i].length = buffer_size;
        pcif->tx_desc_ring[i].cmd |= (E1000_TXD_CMD_RS | E1000_TXD_CMD_EOP);
        pcif->tx_desc_ring[i].status |= E1000_TXD_STAT_DD;
    }
}

int tx_send(struct pci_func *pcif, void *data, size_t len)
{
    uint32_t tail;
    volatile struct tx_desc *tail_tx_desc;
   
    //cprintf("enter tx_send, pcif is %p, data is %s, len is %d\n", pcif, data, len); 
    if (len > buffer_size)
        return -E_INVAL;
    
    tail = pci_read_conf_uint32_t(pcif, E1000_TDT);
    
    if (!(pcif->tx_desc_ring[tail].status & E1000_TXD_STAT_DD))
        return -E_TX_FULL;
    memmove(KADDR((physaddr_t)(pcif->tx_desc_ring[tail].addr)), data, len);
    pcif->tx_desc_ring[tail].length = len;
    pcif->tx_desc_ring[tail].status &= ~E1000_TXD_STAT_DD;
    pci_write_conf_uint32_t(pcif, E1000_TDT, (tail + 1) % tdlen);

    return 0;
}
int attach_e1000(struct pci_func *pcif)
{
    int r;
    
    pci_func_enable(pcif);
    pcif->mmio = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);
    cprintf("e1000_mmio is %p, status is %x\n", pcif->mmio, pci_read_conf_uint32_t(pcif, E1000_STATUS));
    tx_init(pcif);
    
    /* test */
//    char test[] = "hello, world!";
//    if ((r = tx_send(pcif, test, sizeof(test))) < 0)
//        cprintf("tx_send error: %e\n", r);
    return 0;
}