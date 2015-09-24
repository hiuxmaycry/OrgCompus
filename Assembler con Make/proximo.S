#include <mips/regdef.h>
#define fp	$30
#define jp	$25
		.text
		.align 2
                #a0 = 0_a0($fp) = a
                #a1 = 0_a1($fp) = i
                #a2 = 0_a2($fp) = j
                #a3 = 0_a3($fp) = regla
#Todos los define juntos:
		.equ o_sp, 8
		.equ o_fp, 4
		.equ o_gp, 0
		.equ arg5, 24 			#voy a suponer que esta en t3
		.equ o_a3, 20
		.equ o_a2, 16
		.equ o_a1, 12
		.equ o_a0, 8
		.globl proximo #Defino a la funcion como global 
		.ent proximo
#unsigned char proximo(unsigned char *a,unsigned int i,
	#unsigned int j,unsigned char regla,unsigned int N)
proximo:	sub 	sp,sp,o_sp
		sw	fp,o_fp(sp)
		sw	gp,o_gp(sp)
		move	fp,sp			# fp<-sp
		lw	t3,arg5(fp)		# N
#Local area. Creacion y inicialisacion de local vars
#Regla de acceso al dato (TAM_DATO = TAM_CHAR = 1 byte)
#		a[i][j]=a+i*N*TAM_DATO+j*TAM_DATO
		addu	t4,zero,zero		#t4=acumulador de i*N
		addu 	t2,zero,zero		#t2=0 (i*N)
iporN:		addu	t2,t2,a1		#t2+=i
		addi 	t4,t4,1			#t4=t4+1			
		bne	t4,t3,iporN		#if(acumulador<N)
		addu	t2,t2,a2		#t2 = (i*N) + j
		addu	t2,t2,a0		#t2 = a + (i*N) + j
		addi	t8,t3,-1		#t8 = N - 1
		beq	a2,zero,extrIzq 	#si esta en el extremo izq.
		beq	a2,t8,extrDer		#si esta en el extremo der
		lbu	t7,1(t2)		#t7 <= a[i][j+1]
		andi	t7,t7,1			#Dejo solo el primer bit		
		lbu	t0,-1(t2)		#t0 <= a[i][j-1]/4
		andi	t0,t0,1			#Dejo solo el primer bit 
		sll	t0,t0,2			#t0 <= a[i][j-1]
		addu	t0,t0,t7		#t0 <= a[i][j+1] + a[i][j-1]
		b 	centro			#Falta Cargar el Centro
#**********************************************************#
extrIzq:	lbu	t7,1(t2)		#t7 <= a[i][j+1]
		andi 	t7,t7,1			#Dejo solo el primer bit 
		subu	t1,t2,a2		#t1 = a + (i*N)
		addu 	t1,t1,t8		#t1 = a + (i*N) + N -1 Extremo Der.
		lbu	t0,0(t1)		#t0 <= a[i][j-1]/4
		andi 	t0,t0,1			#Dejo solo el primer bit 		
		sll	t0,t0,2			#t0 <= a[i][j-1]
		addu	t0,t0,t7		#t0 <= a[i][j+1] + a[i][j-1]
		b 	centro 			#Falta Cargar el Centro
#**********************************************************#
extrDer:	subu	t2,t2,a2		#t2 <= a + (i*N) 
		lbu	t7,0(t2)		#t7 <= a[i][0]
		andi 	t7,t7,1			#Dejo solo el primer bit 
		addu	t2,t2,a2		#t2 <= a + (i*N) +j		
		lbu	t0,-1(t2)		#t0 <= a[i][j-1]/4
		andi	t0,t0,1			#Dejo solo el primer bit 			
		sll	t0,t0,2			#t0 <= a[i][j-1]
		addu	t0,t0,t7		#t0 <= a[i][j+1] + a[i][j-1]
#**********************************************************#
centro:		lbu	t7,0(t2)		#t7 = a[i][j]/2
		andi	t7,t7,1			#Dejo solo el primer bit 		
		sll 	t7,t7,1			#t7 = a[i][j]	
		addu 	t0,t0,t7		#t0 = a[i][j-1]+a[i][j]+a[i][j+1]
		srlv	t0,a3,t0		#Dejo el bit de regla en la 1ra pos
		andi	v0,t0,1			#En el primer bit esta el proximo 
volver:		lw	fp,o_fp(sp)		#Stack frame destruccion
                lw      gp,o_gp(sp)		
		addu 	sp,sp,o_sp	
		j 	ra 			#jump and return
		.end proximo
