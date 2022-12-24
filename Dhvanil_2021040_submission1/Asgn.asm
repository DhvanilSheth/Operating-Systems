section .data
	msg1: db "Enter an integer value: ",10,0
	msg2: db "Enter a string value: ",10,0
	msg3: db " ",10,0
	input1: db "%d",0
	input2: db "%s",0

	global main
	extern printf
	extern scanf


section .bss
	inp1: resd 1
	inp2: resb 100

section .text
main:
	push rbp
	mov rbp,rsp

	;int print block
	lea rdi,[msg1]
	xor al,al
	call printf

	;int input block
	lea rdi,[input1]
	lea rsi,[inp1]
	xor al,al
	call scanf

	;int output block
	lea rdi,[input1]
	mov rsi,[inp1]
	xor al,al
	call printf

	;space
	lea rdi,[msg3]
	xor al,al
	call printf

	;string print block
	lea rdi,[msg2]
	xor al,al
	call printf

	;string input block
	lea rdi,[input2]
	lea rsi,[inp2]
	xor al,al
	call scanf

	;string output block
	lea rdi,[input2]
	lea rsi,[inp2]
	xor al,al
	call printf

	;space
	lea rdi,[msg3]
	xor al,al
	call printf

	;return call
	pop rbp
	mov rax,0
	ret

