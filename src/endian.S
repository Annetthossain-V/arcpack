.intel_syntax noprefix

.globl conv16
.globl conv32

.type conv16, @function
.type conv32, @function

.text
conv32:
  mov eax, edi
  bswap eax
  ret
conv16:
  mov edi, edi
  mov ax, di 
  xchg ah, al
  ret
