#ifndef SOS_KERNEL_IO_H
#define SOS_KERNEL_IO_H 1

namespace sos
{
namespace io
{

/**
 * x86 assembly:
 *   out port, byte
 */
void out8(short port, char byte);
void out32(short port, int word);
char in8(short port);
int in32(short port);

}
}

#endif /* OS_KERNEL_IO_H */
