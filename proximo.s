#include <mips/regdef.h>
		.text
		.align 2

#Todos los define juntos:
		#define 0_sp 8
		#En el lugar del 32 va a haber padding
		#define 0_fp 4
		#define 0_gp 0
		#define arg5 24 
						#voy a suponer que esta en t3
		#define 0_a3 20
		#define 0_a2 16
		#define 0_a1 12
		#define 0_a0 8

		.globl proximo #Defino a la funcion como global 
		.ent proximo
#unsigned char proximo(unsigned char *a,unsigned int i,
	#unsigned int j,unsigned char regla,unsigned int N)
proximo:
		sub 	sp,sp,0_sp
		sw		$fp,0_fp(sp)
		sw		gp,0_gp(sp)
		move	$fp,sp		# fp<-sp
		#sw		a0,0_a0($fp)	# a
		#sw		a1,0_a1($fp)	# i
		#sw		a2,0_a2($fp)	# j
		#sw		a3,0_a3($fp)	# regla
		lw		t3,arg5($fp)	# N
#Local area. Creacion y inicialisacion de local vars

#Regla de acceso al dato (TAM_DATO = TAM_CHAR = 1 byte)
#		a[i][j]=a+i*N*TAM_DATO+j*TAM_DATO
		addi	t4,zero,1		#t4=acumulador de i*N
		add 	t2,zero,zero	#t2=0 (i*N)
iporN:	addu	t2,t2,a1		#t2+=i
		addi 	t4,t4,1			#t4=t4+1			
		beq		t4,t3,iporN		#if(acumulador<N)
		addu	t2,t2,a2		#t2 = (i*N) + j
		addu	t2,t2,a0		#t2 = a + (i*N) + j
		beq		a2,zero,extrIzq #si esta en el extremo izq.
		beq		a2,t3,extrDer	#si esta en el extremo der
		addi	t1,t2,1			#t1 <= *(a[i][j+1])
		lbu		t7,t1			#t7 <= a[i][j+1]
		addi	t1,t2,-1		#t1 <= *(a[i][j-1])
		lbu		t0,t1			#t0 <= a[i][j-1]/4
		sll		t0,t0,2			#t0 <= a[i][j-1]
		addu	t0,t0,t7		#t0 <= a[i][j+1] + a[i][j-1]
		b 		centro			#Falta Cargar el Centro
#**********************************************************#
extrIzq:addi	t1,t2,1			#t1 <= *(a[i][j+1])
		lbu		t7,t1			#t7 <= a[i][j+1]
		subu	t1,t2,a2		#t1 = a + (i*N)
		addu 	t1,t1,t3		#t1 = a + (i*N) + N Extremo Der.
		lbu		t0,t1			#t0 <= a[i][j-1]/4
		sll		t0,t0,2			#t0 <= a[i][j-1]
		addu	t0,t0,t7		#t0 <= a[i][j+1] + a[i][j-1]
		b centro 				#Falta Cargar el Centro
#**********************************************************#
extrDer:subu	t1,t2,a2		#t1 = a + (i*N)
		lbu		t7,t1			#t7 <= a[i][j+1]
		addi	t1,t2,-1		#t1 <= *(a[i][j-1])
		lbu		t0,t1			#t0 <= a[i][j-1]/4
		sll		t0,t0,2			#t0 <= a[i][j-1]
		addu	t0,t0,t7		#t0 <= a[i][j+1] + a[i][j-1]
#**********************************************************#
centro:	addi	t1,t2,-1		#t1 <= *(a[i][j-1])
		lbu		t7,t1			#t7 = a[i][j-1]/2
		sll 	t7,t7,1			#t7 = a[i][j-1]	
		addu 	t0,t0,t7		#t0	=a[i][j-1]+a[i][j]+a[i][j+1]
		srlv	t0,a3,t0		#Dejo el bit de regla en la 1ra pos
		andi	v0,t0,1			#En el primer bit esta el proximo 
volver:	lw	 	$fp,0_fp(sp)	#Stack frame destruccion
		addu 	sp,sp,0_sp	
		j 		ra 				#jump and return
		.end proximo