;/*
;* pOS
;* jmptable.s - c64 jumptable
;* 11/14/02
;*/

// WARNING: all kernel functions need to be defined as EXTERN

.export _k_reboot

; DO NOT EDIT

_k_reboot:
        jmp entry_main
        jmp _k_addtask
        jmp _k_createfile
        jmp _k_createproc
        jmp _k_deletefile
	jmp _k_devstartstop
        jmp _k_free
        jmp _k_freedevice
        jmp _k_getc
        jmp _k_getdevice
        jmp _k_getfilelength
        jmp _k_getfreemem
        jmp _k_getlasterror
        jmp _k_getproc
        jmp _k_getreservedmem
        jmp _k_gettime
        jmp _k_getusedmem
        jmp _k_killproc
        jmp _k_initdisk
        jmp _k_isowner
        jmp _k_malloc
        jmp _k_memset
        jmp _k_putc
        jmp _k_readblock
        jmp _k_readfile
        jmp _k_remalloc
        jmp _k_removetask
        jmp _k_renamefile
        jmp _k_sendsignal
        jmp _k_setlasterror
        jmp _k_settime
        jmp _k_writeblock
        jmp _k_writefile
