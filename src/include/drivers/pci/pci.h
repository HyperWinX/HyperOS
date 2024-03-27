#ifndef PCI_H
#define PCI_H

#include <common/stdint.h>

struct PCI_Device {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t base_class;
    uint8_t sub_class;
    uint8_t prog_interface;
    uint16_t command;
    uint16_t status;
};

struct PCI_Device get_pci_device(uint8_t bus, uint8_t slot, uint8_t function);
void lspci();

#endif