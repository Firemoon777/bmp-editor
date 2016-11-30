global sse
; rdi, rsi, rdx, rcx

;0.393, 0.349, 0.272, 0.393, 0.349, 0.272 
;0.769, 0.686, 0.543, 0.769, 0.686, 0.543 
;0.189, 0.168, 0.131, 0.189, 0.168, 0.131

; 0.272, 0.349, 0.393, 0.272, 0.349, 0.393
; 0.543, 0.686, 0.769, 0.543, 0.686, 0.769
; 0.131, 0.168, 0.189, 0.131, 0.168, 0.189 

align 16
c1_1: dd 0.272, 0.349, 0.393, 0.272
align 16
c2_1: dd 0.543, 0.686, 0.769, 0.543
align 16
c3_1: dd 0.131, 0.168, 0.189, 0.131

align 16
c1_2: dd 0.349, 0.393, 0.272, 0.349
align 16
c2_2: dd 0.686, 0.769, 0.543, 0.686
align 16
c3_2: dd 0.168, 0.189, 0.131, 0.168

align 16
c1_3: dd 0.393, 0.272, 0.349, 0.393
align 16
c2_3: dd 0.769, 0.543, 0.686, 0.769
align 16
c3_3: dd 0.189, 0.131, 0.168, 0.189 
sse:
	
	movq xmm0, [rdi]
	shufps xmm0, xmm0, 0b01000000
	movq xmm1, [rsi]
	shufps xmm1, xmm1, 0b01000000
	movq xmm2, [rdx]
	shufps xmm2, xmm2, 0b01000000
	movdqa xmm3, [rel c1_1]
	movdqa xmm4, [rel c2_1]
	movdqa xmm5, [rel c3_1]
	mulps xmm0, xmm3
	mulps xmm1, xmm4
	mulps xmm2, xmm5
	addps xmm0, xmm1
	addps xmm0, xmm2
	movdqa [rcx], xmm0
	
	add rdi, 4	
	add rsi, 4	
	add rdx, 4	
	add rcx, 16	
	
	movq xmm0, [rdi]
	shufps xmm0, xmm0, 0b01010000
	movq xmm1, [rsi]
	shufps xmm1, xmm1, 0b01010000
	movq xmm2, [rdx]
	shufps xmm2, xmm2, 0b01010000
	movdqa xmm3, [rel c1_2]
	movdqa xmm4, [rel c2_2]
	movdqa xmm5, [rel c3_2]
	mulps xmm0, xmm3
	mulps xmm1, xmm4
	mulps xmm2, xmm5
	addps xmm0, xmm1
	addps xmm0, xmm2
	movdqa [rcx], xmm0
	
	add rdi, 4	
	add rsi, 4	
	add rdx, 4	
	add rcx, 16
	
	movq xmm0, [rdi]
	shufps xmm0, xmm0, 0b01010100
	movq xmm1, [rsi]
	shufps xmm1, xmm1, 0b01010100
	movq xmm2, [rdx]
	shufps xmm2, xmm2, 0b01010100
	movdqa xmm3, [rel c1_3]
	movdqa xmm4, [rel c2_3]
	movdqa xmm5, [rel c3_3]
	mulps xmm0, xmm3
	mulps xmm1, xmm4
	mulps xmm2, xmm5
	addps xmm0, xmm1
	addps xmm0, xmm2
	movdqa [rcx], xmm0
	
	;movq xmm0, [rcx]
	;shufps xmm0, xmm0, 0b01010100
	;movdqa [rdi], xmm0
	ret
