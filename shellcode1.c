
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char** argv) {
	void* code; void* stack;

	// read input shellcode
	code = mmap(NULL, 0x1000, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	stack = mmap(NULL, 0x1000, PROT_WRITE|PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	printf("Send me your shellcode (max 1024 bytes)\n");
	read(0, code, 0x400);
	printf("Got it. Running your code...\n");
	
	// clear registers and call shellcode
	__asm__("mov rbp, rax\n"
	"mov rsp, rbp\n"
	"xor rax, rax\n"
	"xor rbx, rbx\n"
	"xor rcx, rcx\n"
	"xor rdi, rdi\n"
	"xor rsi, rsi\n"
	"xor r8, r8\n"
	"xor r9, r9\n"
	"xor r10, r10\n"
	"xor r11, r11\n"
	"xor r12, r12\n"
	"xor r13, r13\n"
	"xor r14, r14\n"
	"xor r15, r15\n"
	"jmp rdx\n"
	: 
	: "d" (code), "a" (stack + 0x1000 - 8)
	: );

	return 0;
}
