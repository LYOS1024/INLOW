#ifndef KERNEL_INTERRUPTS_H
#define KERNEL_INTERRUPTS_H

#include <stdint.h>

struct InterruptContext
{
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;

	uint32_t interrupt;
	uint32_t error;

	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	
	// There are only valid if the interrupt came from Ring 3
	uint32_t esp;
	uint32_t ss;
};

namespace Interrupts
{
	extern void (*irqHandlers[])(int);
	void disable();
	void enable();
	void initPic();
}

#endif
