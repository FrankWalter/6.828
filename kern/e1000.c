// LAB 6: Your driver code here
#include <inc/string.h>
#include <kern/e1000.h>
#include <kern/pmap.h>

const uint32_t e1000_tctl_default = 0x4010a;
const uint32_t e1000_tipg_default = 0x60100a;
const unsigned int buffer_size = 2048;
const unsigned int tdlen = 16;

const uint32_t e1000_rctl_default = 0x4000022;
const unsigned int rdlen = 16;
int rx_index;

static void tx_init(struct pci_func *pcif)
{
    int i;
    struct PageInfo* pp;
    
    if (!(pp = page_alloc(ALLOC_ZERO)))
        panic("out of memory during tx_init!");
    pcif->tx_desc_ring = (struct e1000_tx_desc*)page2kva(pp);
    
    pci_write_conf_uint32_t(pcif, E1000_TDBAL, PADDR((void*)pcif->tx_desc_ring));
    pci_write_conf_uint32_t(pcif, E1000_TDBAH, 0);
    pci_write_conf_uint32_t(pcif, E1000_TDLEN, tdlen * sizeof(struct e1000_tx_desc));
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

static void rx_init(struct pci_func *pcif)
{
    int i;
    struct PageInfo* pp;
    
    cprintf("enter rx_init\n");
    pci_write_conf_uint32_t(pcif, E1000_RAL, MAC_LOW);
    pci_write_conf_uint32_t(pcif, E1000_RAH, MAC_HIGH);
    
    for (i = 0; i < 4; i++)
    {
        pci_write_conf_uint32_t(pcif, E1000_MTA + i * sizeof(uint32_t), 0);
    }
    
    if (!(pp = page_alloc(ALLOC_ZERO)))
        panic("out of memory during tx_init!");
    pcif->rx_desc_ring = (struct e1000_rx_desc*)page2kva(pp);
    pci_write_conf_uint32_t(pcif, E1000_RDBAL, PADDR((void*)pcif->rx_desc_ring));
    pci_write_conf_uint32_t(pcif, E1000_RDBAH, 0);
    pci_write_conf_uint32_t(pcif, E1000_RDLEN, rdlen * sizeof(struct e1000_rx_desc));
    pci_write_conf_uint32_t(pcif, E1000_RDH, 1);
    pci_write_conf_uint32_t(pcif, E1000_RDT, 0);
    rx_index = 0;
    
    for (i = 0; i < rdlen; i++)
    {
        if (!(pp = page_alloc(ALLOC_ZERO)))
            panic("out of memory during tx_init!");
        pcif->rx_desc_ring[i].addr = page2pa(pp);
        pcif->rx_desc_ring[i].length = buffer_size;
    }
    
    pci_write_conf_uint32_t(pcif, E1000_RCTL, e1000_rctl_default);
}

int tx_send(struct pci_func *pcif, void *data, size_t len)
{
    uint32_t tail;
   
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

int rx_receive(struct pci_func *pcif, void *data)
{   
    uint32_t new_tail;
    new_tail = (pci_read_conf_uint32_t(pcif, E1000_RDT) + 1) % rdlen;

    if (!(pcif->rx_desc_ring[new_tail].status & E1000_RXD_STAT_DD))
        return -E_RX_EMPTY;
    memmove(data, KADDR((physaddr_t)(pcif->rx_desc_ring[new_tail].addr)), pcif->rx_desc_ring[new_tail].length);
    pci_write_conf_uint32_t(pcif, E1000_RDT, new_tail);
    return pcif->rx_desc_ring[new_tail].length;
}

int attach_e1000(struct pci_func *pcif)
{
    int r;
    
    pci_func_enable(pcif);
    pcif->mmio = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);
    cprintf("e1000_mmio is %p, status is %x\n", pcif->mmio, pci_read_conf_uint32_t(pcif, E1000_STATUS));
    tx_init(pcif);
    rx_init(pcif);
    
    /* test */
//    char test[] = "hello, world!";
//    if ((r = tx_send(pcif, test, sizeof(test))) < 0)
//        cprintf("tx_send error: %e\n", r);
    return 0;
}