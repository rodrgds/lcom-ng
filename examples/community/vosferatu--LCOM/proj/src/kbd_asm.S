#define OUT_OK 0x01
#define STATUS_REG 0x64
#define OUT_BUFF 0x60

.global _kbd_handler

.text
_kbd_handler:
		inb $STATUS_REG,%al
		and $OUT_OK,%al
		cmp 1,%al
		jne end

		mov %eax, 0
		inb $OUT_BUFF,%al
end:	ret
