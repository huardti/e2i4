;librairie de gestion du convertisseur ADC intégré au HCS12
;23/09/05 : version 1corr corrigée par KM ....
;13/10/05 : version 1a commentée YL

;registres pour ATD0 conv A/N

ATD0CTL0  equ 	$80
ATD0CTL1  equ 	$81
ATD0CTL2  equ 	$82
ATD0CTL3  equ 	$83
ATD0CTL4  equ 	$84
ATD0CTL5  equ	$85

ATD0STAT0   	EQU	$86
ATD0DR0   	EQU 	$90
ATD0DR2  	EQU	$94

BM_ADPU		equ 	$80
BM_PRS2		equ	$04
BM_PRS1		equ 	$02
BM_PRS0		equ	$01
BM_S1C		equ	$08
BM_DJM		equ	$80
BM_SCF		equ	$80



initATD0:
	ldaa	#BM_ADPU
	staa	ATD0CTL2	;enable ATD module
	ldaa	#BM_PRS2		;10 bits, CLK/12, 2nd sample time = 2 ATD clocks
	oraa	#BM_PRS0
	staa	ATD0CTL4
	rts

* lecture du canal 2 = entrée PAD02 connectée à un potentiomètre sur la carte HCS12T-board
* sortie 10 bits justifiés à gauche Acc A contient les 8 bits de poids fort

getATD02:
	ldab	#BM_S1C		;1 conv/sequence FIFO 0 FRZ 00
	stab	ATD0CTL3	
	ldab	#0		;10 bits non signés justif gauche, conv simple mono canal
	orab	#$02		;selection canal AN2
	stab	ATD0CTL5
	
getATD02_1:
	ldaa	ATD0STAT0
	BITA	#BM_SCF		;attente flag sequence complete
	BEQ	getATD02_1
	ldd	ATD0DR0

	rts					
		
* lecture du canal x [0..7] entrée no canal dans Acc A
* sortie 10 bits justifiés à gauche Acc A contient les 8 bits de poids fort

getATD0x:
	ldab	#BM_S1C		;1 conv/sequence FIFO 0 FRZ 00
	stab	ATD0CTL3
	anda	#$07	
	tab			;10 bits non signés justif gauche, conv simple mono canal	
	stab	ATD0CTL5
	
getATD0x_1:
	ldaa	ATD0STAT0
	BITA	#BM_SCF		;attente flag sequence complete
	BEQ	getATD0x_1
	ldd	ATD0DR0

	rts						

					
	