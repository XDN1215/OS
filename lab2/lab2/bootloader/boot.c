#include "boot.h"

#define SECTSIZE 512






void bootMain(void) {


	unsigned int elf = 0x200000;
	
	for (int i = 0; i < 200; i++) {
		readSect((void*)(elf + i*512), 1+i);
	}

	// TODO: 填写kMainEntry、phoff、offset...... 然后加载Kernel（可以参考NEMU的某次lab）
	ELFHeader * eh = (ELFHeader *)elf;
	ProgramHeader *ph = (ProgramHeader *)(elf + eh->phoff);
	ProgramHeader *eph = (ProgramHeader *)(ph + eh->phnum);
	int j = 0;
	for(;ph < eph;ph++)
	{
		if(ph->type == 1)
		{
			for(j = 0;j < ph->filesz;j++)
			{
				*(unsigned char*)(ph->paddr+j) = *(unsigned char*)(elf+ph->off+j);
			}
			for(;j < ph->memsz;j++)
			{
				*(unsigned char*)(ph->paddr+j) = (unsigned char)0;
			}
		}
	}
	
	void (*kMainEntry)(void);
	kMainEntry = (void(*)(void))(((ELFHeader*)elf)->entry);
	kMainEntry();
}

void waitDisk(void) { // waiting for disk
	while((inByte(0x1F7) & 0xC0) != 0x40);
}

void readSect(void *dst, int offset) { // reading a sector of disk
	int i;
	waitDisk();
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}
