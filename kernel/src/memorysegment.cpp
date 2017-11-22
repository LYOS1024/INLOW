#include <assert.h>
#include <string.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/kthread.h>
#include <inlow/kernel/memorysegment.h>
#include <inlow/kernel/physicalmemory.h>

static char segmentPage[0x1000] ALIGNED(0x1000) = {0};
static kthread_mutex_t mutex = KTHREAD_MUTEX_INITIALIZER;

static inline size_t getFreeSpaceAfter(MemorySegment* segment)
{
	return segment->next->address - (segment->address + segment->size);
}

MemorySegment::MemorySegment(vaddr_t address, size_t size, int flags, MemorySegment* prev, MemorySegment* next)
{
	this->address = address;
	this->size = size;
	this->flags = flags;
	this->prev = prev;
	this->next = next;
}
void MemorySegment::addSegment(MemorySegment* firstSegment, MemorySegment* newSegment)
{
	vaddr_t endAddress = newSegment->address + newSegment->size;

	MemorySegment* currentSegment = firstSegment;

	while(currentSegment->next && currentSegment->next->address < endAddress)
			currentSegment = currentSegment->next;

	assert(currentSegment->address + currentSegment->size <= newSegment->address);
	assert(!currentSegment->next || currentSegment->next->address >= endAddress);

	newSegment->prev = currentSegment;
	newSegment->next = currentSegment->next;

	currentSegment->next = newSegment;
	if (newSegment->next)
			newSegment->next->prev = newSegment;
}

void MemorySegment::addSegment(MemorySegment* firstSegment, vaddr_t address, size_t size, int protection)
{
	AutoLock lock(&mutex);
	MemorySegment* newSegment = allocateSegment(address, size, protection);
	addSegment(firstSegment, newSegment);
	verifySegmentList();
}

MemorySegment* MemorySegment::allocateSegment(vaddr_t address, size_t size, int flags)
{
	MemorySegment* current = (MemorySegment*) segmentPage;

	while (current->address != 0 && current->size != 0)
	{
		current++;
		if (((uintptr_t) current & 0xFFF) == (0x1000 - 0x1000 % sizeof(MemorySegment)))
		{
			MemorySegment** nextPage = (MemorySegment**) current;
			assert(*nextPage != nullptr);
			current = *nextPage;
		}
	}

	current->address = address;
	current->size = size;
	current->flags = flags;

	return current;
}

void MemorySegment::deallocateSegment(MemorySegment* segment)
{
	memset(segment, 0, sizeof(MemorySegment));
}

void MemorySegment::removeSegment(MemorySegment* firstSegment, vaddr_t address, size_t size)
{
	AutoLock lock(&mutex);
	MemorySegment* currentSegment = firstSegment;
	while (currentSegment->address + currentSegment->size <= address)
	{
		currentSegment = currentSegment->next;
	}

	while (size)
	{
		if (currentSegment->address == address && currentSegment->size <= size)
		{
			address += currentSegment->size;
			size -= currentSegment->size;

			if (size < getFreeSpaceAfter(currentSegment))
			{
				size = 0;
			}
			else
			{
				size -= getFreeSpaceAfter(currentSegment);
			}

			MemorySegment* next = currentSegment->next;
			if (next)
			{
				next->prev = currentSegment->prev;
			}
			if (currentSegment->prev)
			{
				currentSegment->prev->next = next;
			}

			deallocateSegment(currentSegment);
			currentSegment = next;
			continue;
		}
		else if (currentSegment->address == address && currentSegment->size > size)
		{
			currentSegment->address += size;
			currentSegment->size -= size;
			size = 0;
		}
		else if (currentSegment->address + currentSegment->size < address + size)
		{
			size_t diff = currentSegment->address + currentSegment->size - address;
			currentSegment->size -= diff;
			size -= diff;
			address += diff;
		}
		else
		{
			size_t firstSize = address - currentSegment->address;
			size_t secondSize = currentSegment->size - firstSize - size;

			MemorySegment* newSegment = allocateSegment(address + size, secondSize, currentSegment->flags);
			newSegment->prev = currentSegment;
			newSegment->next = currentSegment->next;

			currentSegment->next = newSegment;
			currentSegment->size = firstSize;
		}

		if (size < getFreeSpaceAfter(currentSegment))
		{
			size = 0;
		}
		else
		{
			size -= getFreeSpaceAfter(currentSegment);
		}
		currentSegment = currentSegment->next;
	}
	verifySegmentList();
}

vaddr_t MemorySegment::findFreeSegment(MemorySegment* firstSegment, size_t size)
{
	MemorySegment* currentSegment = firstSegment;

	while (getFreeSpaceAfter(currentSegment) < size)
	{
		currentSegment = currentSegment->next;
		if (!currentSegment)
				return 0;
	}
	return currentSegment->address + currentSegment->size;
}

vaddr_t MemorySegment::findAndAddNewSegment(MemorySegment* firstSegment, size_t size, int protection)
{
	AutoLock lock(&mutex);
	vaddr_t address = findFreeSegment(firstSegment, size);
	MemorySegment* newSegment = allocateSegment(address, size, protection);
	addSegment(firstSegment, newSegment);
	verifySegmentList();
	return address;
}

void MemorySegment::verifySegmentList()
{
	MemorySegment* current = (MemorySegment*) segmentPage;
	MemorySegment** nextPage;

	int freeSegmentSpaceFound = 0;
	MemorySegment* freeSegment = nullptr;

	while (true)
	{
		if (current->address == 0 && current->size == 0)
		{
			freeSegment = current;
			freeSegmentSpaceFound++;
		}

		current++;
		if (((uintptr_t) current & 0xFFF) == (0x1000 - 0x1000 % sizeof(MemorySegment)))
		{
			nextPage = (MemorySegment**) current;
			if (!*nextPage)
					break;
			current = *nextPage;
		}
	}

	if (freeSegmentSpaceFound == 1)
	{
		vaddr_t address = findFreeSegment(kernelSpace->firstSegment, 0x1000);
		paddr_t physical = PhysicalMemory::popPageFrame();
		kernelSpace->mapAt(address, physical, PROT_READ | PROT_WRITE);
		*nextPage = (MemorySegment*) address;
		memset(*nextPage, 0, 0x1000);
		freeSegment->address = address;
		freeSegment->size = 0x1000;
		freeSegment->flags = PROT_READ | PROT_WRITE;
		addSegment(kernelSpace->firstSegment, freeSegment);
	}

}
