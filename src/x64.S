.intel_syntax noprefix

.globl swap32
.globl swap16
.globl zero_arr16

.type swap32, @function
.type swap16, @function
.type zero_arr16, @function

.text
swap16:
  mov ax, di
  xchg ah, al
  ret

swap32:
  mov eax, edi
  bswap eax
  ret

zero_arr16:
  xor rax, rax
  mov esi, esi
.loop:
  mov word ptr [rdi + rax * 2], 0
  inc rax
  cmp rax, rsi
  jg .fini
  jmp .loop
.fini:
  xor eax, eax
  ret
