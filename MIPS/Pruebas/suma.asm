        
        .text

        .globl main

main:
        la $6 string       # Get string addr
        
        add $sp $sp -4
        sw $6 0($sp)       # Push string addr

        jal writeString

        add $sp $sp 8     # Pop string addr y return value vacio


writeString:
        
        ## PPROLOG

        add $sp $sp -12   # Fp return addres return value

        sw $ra -4($sp)      # push return address

        sw $fp 0($sp)       # push old FP

        mv $fp $sp          # Dejo el FP apuntando al old FP

        # No tiene locales
        
        ## BODY

        lw $a0 8($fp)   
        li $v0 4
        syscall 

        ## EPILOG

        lw $fp 0($sp)      
        add $sp $sp 4     # Pop old FP

        lw $ra 0($sp)       
        add $sp $sp, 4     # pop Return addres

        jr $ra
    
        .data



string: .asciiz "enter an integer: "
