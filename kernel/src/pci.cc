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


// TODO: read the rest of the header.
// Currently only reads vendor and device IDs.
DeviceHeader read_device_info(u8 bus, u8 device)
{
    auto data = pci::read(bus, device, 0, 0);
    u16 vendor_id = data & 0xFFFF;
    u16 device_id = (data >> 16) & 0xFFFF;
    return DeviceHeader{vendor_id, device_id};
}

}
}
