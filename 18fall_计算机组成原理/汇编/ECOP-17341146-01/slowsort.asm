.text
.globl main
main:
  add $s0,$zero,10
  la $t0,array
  
#-------input-------#

  la $a0,str1
  li $v0,4
  syscall
  add $t1,$zero,$t0
  add $t2,$zero,$0
input:
  li $v0,5
  syscall
  sw $v0,0($t1)
  addi $t1,4
  addi $t2,1
  blt $t2,$s0,input
  
#-------sort-------#

  sub $s3,$s0,1
  add $t2,$zero,$0
sort1:
  sub $s4,$s3,$t2
  add $t3,$zero,$t0
  add $t4,$zero,$0
  sort2:
	lw $s1,0($t3)
	lw $s2,4($t3)
	blt $s2,$s1,go
    sw $s1,4($t3)
    sw $s2,0($t3)
	go:
	  addi $t3,4
      addi $t4,1
	  blt $t4,$s4,sort2
  addi $t2,1
  blt $t2,$s3,sort1 

#-------output-------#

  la $a0,str2
  li $v0,4
  syscall
  add $t1,$zero,$t0
  add $t2,$zero,$0
output:
  lw $a0,0($t1)
  li $v0,1
  syscall
  la $a0,blank
  li $v0,4
  syscall
  addi $t1,4
  addi $t2,1
  blt $t2,$s0,output
  la $a0,newline
  li $v0,4
  syscall
  
.data
  array: .space 40
  str1: .asciiz "input the 10 integers(each num end of [enter]): \n"
  str2: .asciiz "the sequence after sorting:\n"
  blank: .asciiz " "
  newline: .asciiz "\n"