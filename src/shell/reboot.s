.intel_syntax noprefix

.section .text

.global reboot_kernel

reboot_kernel:
	cli

.wait:
	in al, 0x64
	test al, 0x02
	jnz .wait

	mov al, 0xFE
	out 0x64, al

.hang:
	hlt
	jmp .hang
