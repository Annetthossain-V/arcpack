.intel_syntax noprefix

.globl swap32
.globl swap16

.type swap32, @function
.type swap16, @function

.text
swap16:
  # move edi to ax
  mov ax, di
  
  xor ax, ax
  ret

swap32:
  xor edi, edi
  ret
