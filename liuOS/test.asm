[bits 32]
;hah:
;  MOV  eAX,1*8   ;  段偏移量 ，还是在这再初始化一次吧
;  MOV  DS,AX
;  MOV  ES,AX
;  MOV  FS,AX
;  MOV  SS,AX
;  mov esp,0x280000;把栈地址放到这把
  mov eax ,0xa0000
loops:
  mov byte [eax],14
  inc eax
  cmp eax, 0xaffff
  jbe loops
  jmp loo
  
loo:
  hlt
    jmp loo