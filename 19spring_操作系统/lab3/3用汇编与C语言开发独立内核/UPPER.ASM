	ifndef	??version
?debug	macro
	endm
	endif
	?debug	S "upper.c"
_TEXT	segment	byte public 'CODE'
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP,ss:DGROUP
_TEXT	ends
_DATA	segment word public 'DATA'
d@	label	byte
d@w	label	word
_DATA	ends
_BSS	segment word public 'BSS'
b@	label	byte
b@w	label	word
	?debug	C E91D7B78460775707065722E63
_BSS	ends
_DATA	segment word public 'DATA'
_Message	label	byte
	db	79
	db	83
	db	97
	db	97
	db	97
	db	98
	db	98
	db	98
	db	65
	db	97
	db	66
	db	98
	db	67
	db	99
	db	68
	db	100
	db	69
	db	101
_DATA	ends
_TEXT	segment	byte public 'CODE'
;	?debug	L 3
_upper	proc	near
	push	si
;	?debug	L 4
	xor	si,si
	jmp	short @2
@4:
;	?debug	L 6
	cmp	byte ptr DGROUP:_Message[si],97
	jl	@5
	cmp	byte ptr DGROUP:_Message[si],122
	jg	@5
;	?debug	L 7
	mov	al,byte ptr DGROUP:_Message[si]
	add	al,-32
	mov	byte ptr DGROUP:_Message[si],al
@5:
;	?debug	L 8
	inc	si
@2:
;	?debug	L 5
	cmp	byte ptr DGROUP:_Message[si],0
	jne	@4
@3:
@1:
;	?debug	L 10
	pop	si
	ret	
_upper	endp
_TEXT	ends
	?debug	C E9
_DATA	segment word public 'DATA'
s@	label	byte
_DATA	ends
_TEXT	segment	byte public 'CODE'
_TEXT	ends
	public	_upper
	public	_Message
	end
