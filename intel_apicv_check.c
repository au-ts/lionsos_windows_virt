/*
 * Copyright 2026, UNSW
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MSR_IA32_VMX_PROCBASED_CTLS 0x482
#define MSR_IA32_VMX_PROCBASED_CTLS2 0x48B

int main() {
    int fd = open("/dev/cpu/0/msr", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open /dev/cpu/0/msr. Are you root? Is the 'msr' module loaded (sudo modprobe msr)?");
        return EXIT_FAILURE;
    }

    uint64_t val1, val2;

    // Read Primary Processor-Based VM-Execution Controls
    if (pread(fd, &val1, 8, MSR_IA32_VMX_PROCBASED_CTLS) != 8) {
        perror("Failed to read MSR 0x482");
        close(fd);
        return EXIT_FAILURE;
    }

    // The allowed-1 settings are in the upper 32 bits
    uint32_t primary_high = (uint32_t)(val1 >> 32);

    int tpr_shadow = (primary_high & (1 << 21)) != 0;
    int sec_controls_supported = (primary_high & (1 << 31)) != 0;

    printf("Hardware APICv Support Check\n");
    printf("----------------------------\n");
    printf("TPR Shadow supported: %s\n", tpr_shadow ? "YES" : "NO");

    if (sec_controls_supported) {
        // Read Secondary Processor-Based VM-Execution Controls
        if (pread(fd, &val2, 8, MSR_IA32_VMX_PROCBASED_CTLS2) != 8) {
            perror("Failed to read MSR 0x48B");
        } else {
            uint32_t secondary_high = (uint32_t)(val2 >> 32);

            int apic_reg_virt = (secondary_high & (1 << 8)) != 0;
            int virt_int_delivery = (secondary_high & (1 << 9)) != 0;

            printf("APIC-Register Virtualization: %s\n", apic_reg_virt ? "YES" : "NO");
            printf("Virtual Interrupt Delivery: %s\n", virt_int_delivery ? "YES" : "NO");
        }
    } else {
        printf("Secondary VMX controls not supported (Virtual Interrupt Delivery is NO).\n");
    }

    close(fd);
    return EXIT_SUCCESS;
}
