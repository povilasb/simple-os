#ifndef _SYSTEMFUNC_H
#define _SYSTEMFUNC_H

void uprintf(const char *str, ...);
void ureadln(char *dest);
void uexit();
void *umalloc(unsigned int size);
void ufree(void *addr);
unsigned int uexec(char *program);
void ukill(unsigned int pid);
void print_processes();

#endif
