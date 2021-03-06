																																		  #include <iostream.h>																																	  #include <iostream.h>
#include	<malloc.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#undef outp				// for BC++ 4.51
#undef inp				// for BC++ 4.51
#define AD_BASE 			0x300
#define DATA_H				AD_BASE              //r/w
#define DATA_L				AD_BASE+1            //r
#define MUX					AD_BASE+2            //r/w
#define IO_DIGITAL		    AD_BASE+3            //r/w
#define STATUS				AD_BASE+8            //r et w pour int=0
#define TRIG_CTRL			AD_BASE+9            //r/w
#define PACER_CTRL		    AD_BASE+10           //w
#define INPUT_RANGE		    AD_BASE+11           //w
#define COUNTER0			AD_BASE+12           //r/w
#define COUNTER1			AD_BASE+13           //r/w
#define COUNTER2			AD_BASE+14           //r/w
#define COUNTER_CTRL		AD_BASE+15           //r/w
#define	DAC_BASE	0x180
#define K_MUL	10000L

void Init()
{
   float freq_ech;
	//Tensions d'entr?e
	outp(INPUT_RANGE,8); 	// Echelle conversion 0 to +10V

	//Mux des entr?es
	outp(MUX,0x00);			//Scan channel 0
									// 1 sur bit 7-4  numero de la derni?re entr?e
									// 1 sur bit 3-0  numero de la 1ere entree

	//I/O registers
	outp(IO_DIGITAL,0);  	//Entrees sorties digitales TTL(pour synchro externe)
									//Non utilis?

	// DMA, interrupt, trigger
	outp(TRIG_CTRL,0x02);		//DMA desactive
									//interruptions desactiv?e (INTE)
									//Mode Trigger (software)
	//Pacer clock
	outp(PACER_CTRL,0);		//Timer sur trigger (config 8254)
									//Non utilis?

	//Demande ? l'utilisateur de saisir la frequence d'echantillonnage
	cout <<"Entrez la frequence d'echantillonnage en KHz:" ;
	cin >> freq_ech;

	//Counter					//Non utilis? (Avec trigger)
	outp(COUNTER_CTRL, 0x76); 	// cnt 1 Mode 3, binary, R/W least significant byte first
	outp(COUNTER_CTRL,0xB6);   // cnt 2 Mode 3, binary, R/W least significant byte first


	if (freq_ech==5)
	{
	outp(COUNTER1, 0xE8);outp(COUNTER1,0x03); outp(COUNTER2,0x02);	outp(COUNTER2,0x00);
	}
	if (freq_ech==4)
	{
	outp(COUNTER1, 0x64);outp(COUNTER1,0x00); outp(COUNTER2,0x19);	outp(COUNTER2,0x00);
	}
	if (freq_ech==3)
	{
	outp(COUNTER1, 0x64);outp(COUNTER1,0x00); outp(COUNTER2,0x21);	outp(COUNTER2,0x00);
	}
	if (freq_ech==2)
	{
	outp(COUNTER1, 0xE8);outp(COUNTER1,0x03); outp(COUNTER2,0x05);	outp(COUNTER2,0x00);
	}
	if (freq_ech==1)
	{
	outp(COUNTER1, 0xE8);outp(COUNTER1,0x03); outp(COUNTER2,0x0A);	outp(COUNTER2,0x00);
	}
	if (freq_ech==0.5)
	{
	outp(COUNTER1, 0xE8);outp(COUNTER1,0x03); outp(COUNTER2,0x14);	outp(COUNTER2,0x00);
	}


}

///////////////////////////////////
//****  fonctions de filtrage ****
///////////////////////////////////

///////////////////
//****   FIR ****//
///////////////////

int echantillon_filtre_FIR(int xin, int n, long *b, int *x )
{
	// sortie
	long yout = 0;
	int i=0;

	// xin contient l'echantillon d'entree
	x[0]=xin;

	// calcul du num?rateur
   for(i=0;i<n;i++)	yout=yout+x[i]*b[i];

	// decalage du tampon d'entr?e
	for(i=n -1;i>0 ;i--)	x[i]=x[i-1];

	// sortie de yout
	return(yout/K_MUL);
}

///////////////////
//****   IIR ****//
///////////////////

int echantillon_filtre_IIR(int xin, int n_num, int n_denom, long *b, long *c, int *x, int *y )
{
	// sortie
	long yout = 0;
	int i=0;

	// xin contient l'echantillon d'entree
	x[0]=xin;

	// calcul du num?rateur
	for(i=0;i<n_num;i++)	yout=yout+x[i]*b[i];

	// calcul du d?nominateur , partie recursive du filtre
	for(i=0;i<n_denom;i++)	yout=yout-y[i]*c[i];

	// decalage du tampon d'entr?e
	for(i=n_num-1;i>0;i--)	x[i]=x[i-1];

	// decalage du tampon de sortie
	for(i=n_denom-1;i>0;i--)	y[i]=y[i-1];
	y[0]=yout/K_MUL;

	// sortie de yout
	return(yout/K_MUL);
}

////////////////////////////////////////////////////////////////
// ********** fonction principale  ****************************
////////////////////////////////////////////////////////////////

void main ()
{
	//Declarations
	long nb_points;
	int choix;
	int nb_coeffs;
	int nb_coeffs_num;
	int nb_coeffs_denom;
	float *coeffs;
	long *coeffs_mult;
	float *coeffs_num;
	long *coeffs_num_mult;
	float *coeffs_denom;
	long *coeffs_denom_mult;
		int sortie;
		unsigned int usortie;

	 long i1,tempo;
	 unsigned int	r_H,r_L,res;
	 int	sres;

			printf("test DAS16 DAC06\n");

	//initialisation des registres DAS
	Init();

	 //Demande ? l'utilisateur de saisir le nombre de points d'echantillonnage
	cout <<"Entrez le nombre de points d'echantillonnage:" ;
	cin >> nb_points;

	// choix du type de filtre : FIR ou IIR
	cout <<"Choisir le type de filtre ( 1 pour FIR ou 2 pour IIR ) :" ;
	cin >> choix;

  if (choix==1)  // filtre FIR
  {
	//Demande ? l'utilisateur de saisir le nombre de coefficients du filtre
	cout <<"Entrez le nombre de coefficients du filtre:" ;
	cin >> nb_coeffs;

		 //Demande ? l'utilisateur de saisir les coefficients du filtre
	coeffs=(float*)malloc(nb_coeffs*sizeof(float));
	coeffs_mult=(long*)malloc(nb_coeffs*sizeof(long));

	int j=0;
	do
	{
		cout <<"Entrez coeff:" ;
		cin >> coeffs[j];
		coeffs_mult[j]=coeffs[j]*K_MUL;
		j++;
	}
	while (j!=nb_coeffs);

			 //tableau pour conserver les echantillons precedents
	int *precedents;
	precedents=(int*)malloc(nb_coeffs*sizeof(int));


	long i=0;

	do
	{
		while((inp(STATUS) & 0x10) == 0);	// Wait for INT = 1 (external puse received on DIG0/TRIGGER
		outp(STATUS, 0x00);					// Clear INT, start for a new sampling period
		outp(IO_DIGITAL,0xFF);
		//Start conversion
		//outp(DATA_H,0x1);  						//Lancement d'une conversion par ecriture

		while((inp(STATUS) & 0x8F) != 0 )		//Test sur EOC si conversion fini
		{                                      //On reboucle tant que EOC = 1
			//cout << "Conversion en cours";
		}

		r_H=inp(AD_BASE+1);    //recuperation de valeur convertie poids fort
		r_L=((inp(AD_BASE))&0xF0)>>4;    //recuperation de valeur convertie poids faible
		res = (r_L+(r_H<<4));
		sres = res-2048;

		 // operation de filtrage
		sortie=echantillon_filtre_FIR(sres, nb_coeffs, coeffs_mult, precedents);

		usortie = sortie+2048;
		outp(DAC_BASE,usortie%256);
		outp(DAC_BASE+1,usortie/256);
		outp(IO_DIGITAL,0x00);
		//for(tempo = 0; tempo <=4920;tempo++);


	i++;
	}while(i!= nb_points);
  }

  else     // filtre IIR
  {
	//Demande ? l'utilisateur de saisir le nombre de coefficients du numerateur
	cout <<"Entrez le nombre de coefficients du numerateur:" ;
	cin >> nb_coeffs_num;

	//Demande ? l'utilisateur de saisir les coefficients du numerateur
	coeffs_num=(float*)malloc(nb_coeffs_num*sizeof(float));
	coeffs_num_mult=(long*)malloc(nb_coeffs_num*sizeof(long));

	int j=0;
	do
	{
		cout <<"Entrez coeff de numerateur:" ;
		cin >> coeffs_num[j];
		coeffs_num_mult[j]=coeffs_num[j]*K_MUL;
		j++;
	}
	while (j!=nb_coeffs_num);


	//Demande ? l'utilisateur de saisir le nombre de coefficients du denominateur
	cout <<"Entrez le nombre de coefficients du denominateur:" ;
	cin >> nb_coeffs_denom;

	 //Demande ? l'utilisateur de saisir les coefficients du denominateur
	coeffs_denom=(float*)malloc(nb_coeffs_denom*sizeof(float));
	coeffs_denom_mult=(long*)malloc(nb_coeffs_denom*sizeof(long));

	int k=0;
	do
	{
		cout <<"Entrez coeff du denominateur:" ;
		cin >> coeffs_denom[k];
		coeffs_denom_mult[k]=coeffs_denom[k]*K_MUL;
		k++;
	}
	while (k!=nb_coeffs_denom);

	//tableau pour conserver les echantillons d'entree precedents
	int *entrees_precedentes;
	entrees_precedentes=(int*)malloc(nb_coeffs_num*sizeof(int));

	//tableau pour conserver les echantillons de sortie precedents
	int *sorties_precedentes;
	sorties_precedentes=(int*)malloc(nb_coeffs_denom*sizeof(int));

	long i=0;

	do
	{
		while((inp(STATUS) & 0x10) == 0);	// Wait for INT = 1 (external puse received on DIG0/TRIGGER
		outp(STATUS, 0x00);					// Clear INT, start for a new sampling period
		outp(IO_DIGITAL,0xFF);
		//Start conversion
		//outp(DATA_H,0x1);  						//Lancement d'une conversion par ecriture

		while((inp(STATUS) & 0x8F) != 0 )		//Test sur EOC si conversion fini
		{                                      //On reboucle tant que EOC = 1
			//cout << "Conversion en cours";
		}

		r_H=inp(AD_BASE+1);    //recuperation de valeur convertie poids fort
		r_L=((inp(AD_BASE))&0xF0)>>4;    //recuperation de valeur convertie poids faible
		res = (r_L+(r_H<<4));
		sres = res-2048;

		 // operation de filtrage
		sortie=echantillon_filtre_IIR(sres, nb_coeffs_num, nb_coeffs_denom, coeffs_num_mult, coeffs_denom_mult, entrees_precedentes, sorties_precedentes);

		usortie = sortie+2048;
		outp(DAC_BASE,usortie%256);
		outp(DAC_BASE+1,usortie/256);
		outp(IO_DIGITAL,0x00);
		//for(tempo = 0; tempo <=4920;tempo++);


	i++;
	}while(i!= nb_points);
  }

}



