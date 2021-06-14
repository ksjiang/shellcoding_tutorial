
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <seccomp.h>
#include <sys/mman.h>

const int blacklist[] = {SCMP_SYS(write), SCMP_SYS(writev), SCMP_SYS(pwrite64), SCMP_SYS(pwritev), 328, 
	SCMP_SYS(sendfile), 326, 
	SCMP_SYS(execve), SCMP_SYS(readlink), SCMP_SYS(fork), SCMP_SYS(vfork), SCMP_SYS(clone), SCMP_SYS(mmap), SCMP_SYS(mprotect), SCMP_SYS(munmap), SCMP_SYS(brk), SCMP_SYS(execveat), 
	SCMP_SYS(socket), SCMP_SYS(connect), SCMP_SYS(bind), SCMP_SYS(sendto), SCMP_SYS(sendmsg)};

int main(int argc, char** argv) {
	int i;
	scmp_filter_ctx ctx;
	void* code; void* stack;

	// read input shellcode
	code = mmap(NULL, 0x1000, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	stack = mmap(NULL, 0x1000, PROT_WRITE|PROT_READ, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	printf("Send me your shellcode (max 1024 bytes)\n");
	read(0, code, 0x400);
	printf("Got it. Setting up secure environment and running your code...\n");

	// set up secure computing environment
	ctx = seccomp_init(SCMP_ACT_ALLOW);
	if (!ctx) {
		printf("seccomp_init failed\n");
		return 1;
	}

	for (i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
		if (seccomp_rule_add(ctx, SCMP_ACT_KILL, blacklist[i], 0)) {
			printf("seccomp_rule_add failed\n");
			return 1;
		}

	}

	if (seccomp_load(ctx)) {
		printf("seccomp_load failed\n");
		return 1;
	}

//	printf("This should not print!\n");
	
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
