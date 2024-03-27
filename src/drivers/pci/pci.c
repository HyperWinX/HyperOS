#include <drivers/pci/pci.h>
#include <screen/screen.h>
#include <kernel/kernel.h>
#include <common/stdlib.h>
#include <common/stdio.h>
#include <memory/memory.h>

#define CONFIG_ADDRESS_PORT 0xCF8
#define CONFIG_DATA_PORT 0xCFC

uint32_t read_pci_port(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset) {
    uint32_t address = 0;
    
    // Clear out any bits not within range
    function &= 0b111; // 3 bits
    
    // Set the bits
    address |= 0x80000000; // bit 31: enable = 1
    // bits 30-24: reserved, leave them as 0
    address |= (bus << 16); // bits 23-16: bus
    address |= (slot << 11); // bits 15-11: device/slot
    address |= (function << 8); // bits 10-8: function
    address |= offset; // bits 7-0: offset

    // Set the address to read
    outl(CONFIG_ADDRESS_PORT, address);

    // Read data from PCI
    return inl(CONFIG_DATA_PORT);
}

struct PCI_Device get_pci_device(uint8_t bus, uint8_t slot, uint8_t function) {
    struct PCI_Device device;
    uint32_t pci_data = read_pci_port(bus, slot, function, 0);
    device.vendor_id = pci_data & 0xffff;
    device.device_id = (pci_data >> 16) & 0xffff;
    pci_data = read_pci_port(bus, slot, function, 0x04);
    device.command = pci_data & 0xffff;
    device.status = (pci_data >> 16) & 0xffff;
    pci_data = read_pci_port(bus, slot, function, 0x09);
    device.base_class = (pci_data >> 16) & 0xff;
    device.sub_class = (pci_data >> 8) & 0xff;
    device.prog_interface = pci_data & 0xff;
    return device;
}

void lspci() {
    for (uint8_t i = 0; i < 255; i++) {
        for (uint8_t j = 0; j < 255; j++) {
            for (uint8_t k = 0; k < 8; k++) {
                struct PCI_Device device = get_pci_device(i, j, k);
                if (device.vendor_id == 0xFFFF) {
                    continue;
                }
                printf("Bus %d Device %d Function %d: Vendor=%x Device=%x Class=%x SubClass=%x ProgIf=%x\n",
                       i,
                       j,
                       k,
                       device.vendor_id,
                       device.device_id,
                       device.base_class,
                       device.sub_class,
                       device.prog_interface);
            }
        }
    }
}