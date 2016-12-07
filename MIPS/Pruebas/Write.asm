        
        .text

        .globl main

main:
        la $6 string       # Get string addr
        
        add $sp $sp -4
        sw $6 0($sp)       # Push string addr (Param 1)

        jal writeString

        # CLEANUP
        add $sp $sp 4     # Pop string addr


writeString:
        
        ## PPROLOG
        add $sp $sp -8         # Fp return addres, old fp

        sw $ra 4($sp)           # push return address

        sw $fp 0($sp)           # push old FP

        move $fp $sp            # Dejo el FP apuntando al old FP

        # No tiene locales
        
        ## BODY

        lw $a0 8($fp)   
        li $v0 4
        syscall 

        ## EPILOG

        lw $fp 0($sp)      
        add $sp $sp 4           # Pop old FP

        lw $ra 0($sp)       
        add $sp $sp, 4          # pop Return addres

        jr $ra

exit:
        li $v0 10
        syscall

       
        .data

string: .asciiz "enter an integer: "
