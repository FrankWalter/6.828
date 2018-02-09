#ifndef JOS_KERN_PCI_H
#define JOS_KERN_PCI_H

#include <inc/types.h>

#define MAX_PCI 64
// PCI subsystem interface
enum { pci_res_bus, pci_res_mem, pci_res_io, pci_res_max };

struct pci_bus;

struct pci_func {
    struct pci_bus *bus;	// Primary bus for bridges

    uint32_t bus_num;
    uint32_t dev;
    uint32_t func;

    uint32_t dev_id;
    uint32_t dev_class;

    uint32_t reg_base[6];
    uint32_t reg_size[6];
    uint8_t irq_line;
    
    volatile char *mmio;
    volatile struct e1000_tx_desc *tx_desc_ring;
    volatile struct e1000_rx_desc *rx_desc_ring;
};

struct pci_bus {
    struct pci_func *parent_bridge;
    uint32_t busno;
};

inline void pci_write_conf_uint8_t(struct pci_func *pcif, uint32_t offset, uint8_t val)
{
    *((uint8_t*) (pcif->mmio + offset)) = val;
}
inline uint8_t pci_read_conf_uint8_t(struct pci_func *pcif, uint32_t offset)
{
    return *((uint8_t*) (pcif->mmio + offset));
}

inline void pci_write_conf_uint16_t(struct pci_func *pcif, uint32_t offset, uint16_t val)
{
    *((uint16_t*) (pcif->mmio + offset)) = val;
}
inline uint16_t pci_read_conf_uint16_t(struct pci_func *pcif, uint32_t offset)
{
    return *((uint16_t*) (pcif->mmio + offset));
}

inline void pci_write_conf_uint32_t(struct pci_func *pcif, uint32_t offset, uint32_t val)
{
    *((uint32_t*) (pcif->mmio + offset)) = val;
}
inline uint32_t pci_read_conf_uint32_t(struct pci_func *pcif, uint32_t offset)
{
    return *((uint32_t*) (pcif->mmio + offset));
}

int  pci_init(void);
void pci_func_enable(struct pci_func *f);
struct pci_func *pci_get_by_bdf(uint32_t bus, uint32_t dev, uint32_t func);

#endif
