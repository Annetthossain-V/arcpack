.intel_syntax noprefix

.globl swap32
.globl swap16

.type swap32, @function
.type swap16, @function

.text
swap16:
  mov ax, di
  xchg ah, al
  ret

swap32:
  mov eax, edi
  bswap eax
  ret
