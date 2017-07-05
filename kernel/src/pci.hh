#ifndef SOS_KERNEL_PCI_HH
#define SOS_KERNEL_PCI_HH 1

#include "types.hh"

namespace sos
{
namespace pci
{

const short address_port = 0xCF8;
const short data_port = 0xCFC;

using types::u8;
using types::u32;

u32 device_addr(u8 bus, u8 device, u8 function, u8 offset);

/**
 * Read 4 bytes.
 */
u32 read(u8 bus, u8 device, u8 function, u8 offset);

}
}

#endif /* SOS_KERNEL_PCI_HH */
