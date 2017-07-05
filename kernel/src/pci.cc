#include "pci.hh"
#include "io.hh"

namespace sos
{
namespace pci
{

u32 device_addr(u8 bus, u8 device, u8 function, u8 offset)
{
    return 0x80000000 | bus << 16 | device << 11 | function <<  8 | offset;
}

u32 read(u8 bus, u8 device, u8 function, u8 offset)
{
    io::out32(address_port, device_addr(bus, device, function, offset));
    return io::in32(data_port);
}

}
}

