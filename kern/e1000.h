#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#endif	// JOS_KERN_E1000_H
#include <kern/pci.h>
#include <inc/error.h>
#define PCI_VENDOR_ID_INTEL              0x8086
#define E1000_DEV_ID_82540EM             0x100E
#define MAC_LOW 0x12005452
#define MAC_HIGH 0x80005634

/* e1000 registers (refer to qemu/e1000_hw.h) */
#define E1000_STATUS   0x00008  /* Device Status - RO */

/* Transmit related registers */
#define E1000_TDBAL    0x03800  /* TX Descriptor Base Address Low - RW */
#define E1000_TDBAH    0x03804  /* TX Descriptor Base Address High - RW */
#define E1000_TDLEN    0x03808  /* TX Descriptor Length - RW */
#define E1000_TDH      0x03810  /* TX Descriptor Head - RW */
#define E1000_TDT      0x03818  /* TX Descripotr Tail - RW */

/* Transmit Control */
#define E1000_TCTL     0x00400  /* TX Control - RW */
#define E1000_TCTL_EXT 0x00404  /* Extended TX Control - RW */
#define E1000_TCTL_RST    0x00000001    /* software reset */
#define E1000_TCTL_EN     0x00000002    /* enable tx */
#define E1000_TCTL_BCE    0x00000004    /* busy check enable */
#define E1000_TCTL_PSP    0x00000008    /* pad short packets */
#define E1000_TCTL_CT     0x00000ff0    /* collision threshold */
#define E1000_TCTL_COLD   0x003ff000    /* collision distance */
#define E1000_TCTL_SWXOFF 0x00400000    /* SW Xoff transmission */
#define E1000_TCTL_PBE    0x00800000    /* Packet Burst Enable */
#define E1000_TCTL_RTLC   0x01000000    /* Re-transmit on late collision */
#define E1000_TCTL_NRTU   0x02000000    /* No Re-transmit on underrun */
#define E1000_TCTL_MULR   0x10000000    /* Multiple request support */

#define E1000_TXD_CMD_EOP    0x01 /* End of Packet */
#define E1000_TXD_CMD_IFCS   0x02 /* Insert FCS (Ethernet CRC) */
#define E1000_TXD_CMD_IC     0x04 /* Insert Checksum */
#define E1000_TXD_CMD_RS     0x08 /* Report Status */
#define E1000_TXD_CMD_RPS    0x10 /* Report Packet Sent */
#define E1000_TXD_CMD_DEXT   0x20 /* Descriptor extension (0 = legacy) */
#define E1000_TXD_CMD_VLE    0x40 /* Add VLAN tag */
#define E1000_TXD_CMD_IDE    0x80 /* Enable Tidv register */

#define E1000_TXD_STAT_DD    0x00000001 /* Descriptor Done */
#define E1000_TXD_STAT_EC    0x00000002 /* Excess Collisions */
#define E1000_TXD_STAT_LC    0x00000004 /* Late Collisions */
#define E1000_TXD_STAT_TU    0x00000008 /* Transmit underrun */

#define E1000_TIPG     0x00410  /* TX Inter-packet gap -RW */

/* receive related registers*/
#define E1000_RCTL     0x00100  /* RX Control - RW */
#define E1000_RDBAL    0x02800  /* RX Descriptor Base Address Low - RW */
#define E1000_RDBAH    0x02804  /* RX Descriptor Base Address High - RW */
#define E1000_RDLEN    0x02808  /* RX Descriptor Length - RW */
#define E1000_RDH      0x02810  /* RX Descriptor Head - RW */
#define E1000_RDT      0x02818  /* RX Descriptor Tail - RW */

#define E1000_MTA      0x05200  /* Multicast Table Array - RW Array */
#define E1000_RAL       0x05400  /* Receive Address LOW - RW Array */
#define E1000_RAH       0x05404  /* Receive Address HIGH - RW Array */



/* definitions for e1000 related structures*/
struct e1000_tx_desc
{
	uint64_t addr;
	uint16_t length;
	uint8_t cso;
	uint8_t cmd;
	uint8_t status;
	uint8_t css;
	uint16_t special;
};

struct e1000_rx_desc {
    uint64_t addr; /* Address of the descriptor's data buffer */
    uint16_t length;     /* Length of data DMAed into data buffer */
    uint16_t csum;       /* Packet checksum */
    uint8_t status;      /* Descriptor status */
    uint8_t errors;      /* Descriptor Errors */
    uint16_t special;
};

int tx_send(struct pci_func *pcif, void* data, size_t len);
int rx_receive(struct pci_func *pcif, void *data, size_t len);
int attach_e1000(struct pci_func *pcif);
