; pages64.inc
;

[bits 32]

%include "page.inc"

;-----------------------------------------
; void clear_4K_page()：清4K页面
; input:  
;                esi: address
;------------------------------------------        
clear_4k_page:
clear_4K_page:
        pxor xmm1, xmm1
        mov eax, 4096
clear_4K_page_loop:        
        movdqu [esi + eax - 16], xmm1
        movdqu [esi + eax - 32], xmm1
        movdqu [esi + eax - 48], xmm1
        movdqu [esi + eax - 64], xmm1
        movdqu [esi + eax - 80], xmm1
        movdqu [esi + eax - 96], xmm1
        movdqu [esi + eax - 112], xmm1
        movdqu [esi + eax - 128], xmm1
        sub eax, 128
        jnz clear_4K_page_loop
        ret

;----------------------------------
; clear_4K_pages()：清数个4K页
; input:
;                esi: address,        edi: number
;----------------------------------
clear_4k_pages:
clear_4K_pages:
        test edi, edi
        jz clear_4K_pages_done
clear_4K_pages_loop:        
        call clear_4K_page
        add esi, 0x1000
        dec edi
        jnz clear_4K_pages_loop        
clear_4K_pages_done:        
        ret        
        

;---------------------------------------------------------------
; init_page(): 初始化 long mode 的页结构
; 描述：
;        在进入 long-mode 模式之前进行页表初始化
;----------------------------------------------------------------        
init_page:
        mov esi, 200000h;
        call clear_4k_page
        ;mov esi, 201000h
        ;call clear_4k_page
        ;mov esi, 202000h
        ;call clear_4k_page
        ;mov esi, 203000h
        ;call clear_4k_page
        ;mov esi, 204000h
        ;call clear_4k_page
        ;mov esi, 205000h
        ;call clear_4k_page
        ;mov esi, 206000h
        ;call clear_4k_page


; 设置 PML4T 表，PML4T 的地址在 200000H
        mov DWORD [200000h], 201000h | RW | US | P                  ; PML4T[0]
        mov DWORD [200004h], 0


; 设置 PDPT 表， 第 0 个 PDPT 表在 101000H，第 511 个 PDPT 表在 102000H
        mov DWORD [201000h], 203000h | RW | US | P                 ; PDPT[0] for PML4T[0]
        mov DWORD [201004h], 0

; set PDT
        mov DWORD [203000h], 205000h | RW | US | P                 ; PDT[0] for PDPT[0] for PML4T[0]
        mov DWORD [203004h], 0

        ; virtual address 100000h - 2FFFFFh 映射到 100000h - 2FFFFFh 上
        ; 不可执行，用户不可访问
        ; 系统数据区

        mov DWORD [203000h + 1 * 8], 200000h | PS | RW | P
        mov DWORD [203000h + 1 * 8 + 4], XD

        mov DWORD [203000h + 2 * 8], 207000h | RW | P
        mov DWORD [203000h + 2 * 8 + 4], XD

; set PT
        ; virutal address 0 - 0FFFh 映射到物理地址 0 - 0FFFh 上（4K页）
        ; r/w = u/s = p = 1
        mov DWORD [205000h + 0 * 8], 0000h | RW | US | P
        mov DWORD [205000h + 0 * 8 + 4], 0

        ; virtual address 0B000 - 0BFFFh 映射到物理地址 0B000 - 0BFFFh 上(4K页）
        ; r/w = u/s = p = 1
        mov DWORD [205000h + 0Bh * 8], 0B000h | RW | US | P
        mov DWORD [205000h + 0Bh * 8 + 4], 0
        
        ; virtual address 9000h - 0FFFFh 映射到物理地址 09000h - 0FFFFh 上
        mov DWORD [205000h + 09h * 8], 09000h | RW | US | P
        mov DWORD [205000h + 09h * 8 + 4], 0
        mov DWORD [205000h + 0Ah * 8], 0A000h | RW | US | P
        mov DWORD [205000h + 0Ah * 8 + 4], 0
        mov DWORD [205000h + 0Bh * 8], 0B000h | RW | US | P
        mov DWORD [205000h + 0Bh * 8 + 4], 0
        mov DWORD [205000h + 0Ch * 8], 0C000h | RW | US | P
        mov DWORD [205000h + 0Ch * 8 + 4], 0
        mov DWORD [205000h + 0Dh * 8], 0D000h | RW | US | P
        mov DWORD [205000h + 0Dh * 8 + 4], 0
        mov DWORD [205000h + 0Eh * 8], 0E000h | RW | US | P
        mov DWORD [205000h + 0Eh * 8 + 4], 0
        mov DWORD [205000h + 0Fh * 8], 0F000h | RW | US | P
        mov DWORD [205000h + 0Fh * 8 + 4], 0

        ; virtual address 10000h - 18FFFh 映射到物理地址 10000h - 18FFFh上（8共个4K页）
        ; 可执行，r/w = u/s = p = 1
        ; 用于 long.asm 模块执行空间
        mov DWORD [205000h + 10h * 8], 10000h | RW | US | P
        mov DWORD [205000h + 10h * 8 + 4], 0
        mov DWORD [205000h + 11h * 8], 11000h | RW | US | P
        mov DWORD [205000h + 11h * 8 + 4], 0
        mov DWORD [205000h + 12h * 8], 12000h | RW | US | P
        mov DWORD [205000h + 12h * 8 + 4], 0
        mov DWORD [205000h + 13h * 8], 13000h | RW | US | P
        mov DWORD [205000h + 13h * 8 + 4], 0
        mov DWORD [205000h + 14h * 8], 14000h | RW | US | P
        mov DWORD [205000h + 14h * 8 + 4], 0
        mov DWORD [205000h + 15h * 8], 15000h | RW | US | P
        mov DWORD [205000h + 15h * 8 + 4], 0
        mov DWORD [205000h + 16h * 8], 16000h | RW | US | P
        mov DWORD [205000h + 16h * 8 + 4], 0
        mov DWORD [205000h + 17h * 8], 17000h | RW | US | P
        mov DWORD [205000h + 17h * 8 + 4], 0
        mov DWORD [205000h + 18h * 8], 18000h | RW | US | P
        mov DWORD [205000h + 18h * 8 + 4], 0
        mov DWORD [205000h + 20h * 8], 20000h | RW | US | P
        mov DWORD [205000h + 20h * 8 + 4], 0


        ; virtual address 0B8000h - 0B9FFFh 映射到物理地址 0B8000h - 0B9FFFh 上（2个4K页）
        ; 不可执行，r/w = u/s = p = 1
        ; 用于 video 区域
        mov DWORD [205000h + 0B8h * 8], 0B8000h | RW | US | P
        mov DWORD [205000h + 0B8h * 8 + 4], XD
        mov DWORD [205000h + 0B9h * 8], 0B9000h | RW | US | P
        mov DWORD [205000h + 0B9h * 8], XD


        ; virutal address 400000h 映射到物理地址 400000h 上（使用 4K 页）
        ; 不可执行，用户不可访问，用于 DS save 区域
        mov DWORD [207000h], 400000h | RW | P
        mov DWORD [207004h], XD

        ret
        

        
;********* page table base *******
pml4t_base      dq PML4T_BASE
pdpt_base       dq 0
pdt_base        dq 0
pt_base         dq 0

maxphyaddr_select_mask  dq 0

