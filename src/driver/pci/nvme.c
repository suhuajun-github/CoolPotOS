#include "nvme.h"
#include "pcie.h"
#include "kprint.h"
#include "hhdm.h"
#include "io.h"
#include "krlibc.h"

NvmeRegisters *registers;

void nvme_setup(){
    pcie_device_t *device = pcie_find_class(0x10802);
    if(device == NULL){
        return;
    }
    kinfo("Loading Nvme driver...");
//
//    uint32_t command = pci_read_command_status(device);
//    if((command & (1 << 2)) == 0){ //enable bus mastering.
//        command |= PCI_RCMD_BUS_MASTER;
//        pci_write_command_status(device,command);
//    }
//
//    if(!pci_bar_present(device,0)){
//        kwarn("Unable to local bar0 for nvme.");
//        return;
//    }
//
//    uint32_t bar0 = read_pci(device,0);
//    uint64_t nvme_base_addr = (uint64_t)phys_to_virt(bar0);
//    uint32_t vs = mmio_read64((void *) (nvme_base_addr + NVME_VERSION_OFFSET));
//
//    uint32_t major_version = (vs >> 16) & 0xffff;
//    uint32_t minor_version = (vs >> 8) & 0xff;
//    uint32_t tertiary_version = vs & 0xff;
//
//    UNUSED(major_version);
//    UNUSED(minor_version);
//    UNUSED(tertiary_version);

    //printk("[%d:%d:%d]\n",major_version,minor_version,tertiary_version);
}
