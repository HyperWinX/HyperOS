#pramga once

#include <common/stdint.h>

struct multiboot_tag {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
};

struct multiboot_info {
    uint32_t total_size;
    uint32_t reserved;
    struct multiboot_tag tags[0];
};

