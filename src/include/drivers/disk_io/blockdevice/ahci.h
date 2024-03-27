#pragma once

#include <common/stdbool.h>
#include <common/stdint.h>

class AHCI{
private:
    bool _supports_64bit_addressing;
    bool _supports_native_command_queuing;
    bool _supports_snotification_register;
    bool _supports_staggered_spinup;
    bool _supports_aggressive_link_power_mgmt;
    bool _supports_activity_LED;
    bool _supports_command_list_override;
    bool _supports_ahci_mode_only;
    bool _supports_port_multiplier;
    bool _FIS_based_switching_supported;
    bool _PIO_multiple_DRQ_block;
    bool _slubmer_state_capable;
    bool _partial_state_capable;
    bool _command_completion_coalsecing_supported;
    bool _enclosure_mgmt_supported;
    bool _support_external_SATA;
    uint32_t _num_of_command_slots;
    uint32_t _interface_speed_support;
    uint32_t _num_of_ports;

    void _init_driver(void);


public:
    const uint64_t _regular_sector_size = 512UL;
    char _serial_no[9];
};