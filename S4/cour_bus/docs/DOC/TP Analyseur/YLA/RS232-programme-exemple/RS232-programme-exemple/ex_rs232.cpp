// comm01 :
// 1) initialise un port de communication s?rie, puis
// 2) les carat?res re?us au clavier sont envoy?s sur la liaison s?rie,
 //  les caract?res lus sur la liaison s?rie sont affich?s
 // sortie par ESC

#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define TEMPO_GF265		50
#define MAX_FRAME			20

HANDLE hComm = NULL;
COMMTIMEOUTS ctmoNew = {0}, ctmoOld;
 DCB dcbCommPort;										// Device Control Block
char InBuff[100];
char OutBuff[100];
char Frame[100];
char *str;

char done;
DWORD dwBytesRead,dwBytesWritten;

//prototypes de fonctions
static void SendFrame(char *ptr_buffer);


//fonction permettant d'envoyer une cha?ne de caract?res
//La fonction envoie caract?res par caract?res au GBF jusqu'? la d?tection d'un caract?re de fin de cha?ne
static void SendFrame(char *ptr_buffer)
{
   unsigned int i;

   i = 0;

   while((*(ptr_buffer + i) != 0) && (i < MAX_FRAME))
   {
		//strcpy(OutBuff,*ptr_buffer);
      OutBuff[0] = *(ptr_buffer + i);   	//stocke dans le buffer de transmission un caract?re de la cha?ne ? transmettre
      OutBuff[1] = 0x00;						//NULL character
		WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
   	Sleep(TEMPO_GF265);        			//le GBF ne poss?de pas de Handshaking - il est n?cessaire d'ajouter une tempo entre chaque envoie de caract?res
      i++;
   }
}



int main(void)
{
	char Key;

  // Ouverture du port serie

  hComm = CreateFile("COM1",
							 GENERIC_READ | GENERIC_WRITE,
							 0,
							 0,
							 OPEN_EXISTING,
							 0,
							 0);

  if(hComm == INVALID_HANDLE_VALUE) exit(-1);	// echec de l'ouverture

  // Initialisation.

	GetCommTimeouts(hComm,&ctmoOld);					// sauvegarde params actuels
	ctmoNew.ReadIntervalTimeout = MAXDWORD;		//	Retour imm?diat de l'op?ration de lecture
	ctmoNew.ReadTotalTimeoutConstant = 0;			// m?me si aucun car. n'a ?t? re?u
	ctmoNew.ReadTotalTimeoutMultiplier = 0;			//
	ctmoNew.WriteTotalTimeoutMultiplier = 0;
	ctmoNew.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(hComm, &ctmoNew);

	// D?finition  BAUD RATE, PARITY, WORD SIZE, et STOP BITS.
	// Nota :
	// l'argument de BuildCommDCB doit ?tre un pointeur sur une cha?ne.
  // BuildCommDCB() val par d?faut : NO HANDSHAKING.

	dcbCommPort.DCBlength = sizeof(DCB);
	GetCommState(hComm, &dcbCommPort);
	BuildCommDCB("9600,N,8,1", &dcbCommPort);
	SetCommState(hComm, &dcbCommPort);


	// Boucle principale : les carat?res re?us au clavier sont envoy?s sur la liaison s?rie,
	// les caract?res lus sur la liaison s?rie sont affich?s

   printf("welcome\r\n");
   printf("menu\r\n");
   printf("1: set mode\r\n");
   printf("2: set frequency\r\n");
   printf("3: get mode\r\n");
   printf("4: get frequency\r\n");
   printf("9: exit\r\n");
   printf("\r\n");

	done = FALSE;
	do
	{
		if(kbhit())
		{
			Key = getch();
			switch(Key)
			{
				case (0x39): done = TRUE; break;

            //set mode
				case (0x31) :
				{
					strcpy(OutBuff,"0");
					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            	Sleep(1000);

					strcpy(OutBuff,"0");
            	WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            	Sleep(1000);

					OutBuff[0] = 0x0D;
					OutBuff[1] = 0x0A;
					OutBuff[2] = 0;
					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            }
            break;

            //set frequency
            case 0x32:
            {
               strcpy(OutBuff,"4");
            	WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            	Sleep(1000);

					strcpy(OutBuff,"0");
            	WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            	Sleep(1000);
            }
            break;

            //get mode
				case (0x33) :
				{
            	printf("get mode\n\r");

               strcpy(Frame, "40?\r");
               SendFrame(Frame);



  //					strcpy(OutBuff,"0");
  //					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
  //          	Sleep(1000);

  //					strcpy(OutBuff,"0");
  //          	WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
  //          	Sleep(1000);

  //             strcpy(OutBuff,"?");
  //          	WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
  //          	Sleep(1000);

  //					OutBuff[0] = 0x0D;
  //					OutBuff[1] = 0x0A;
  //					OutBuff[2] = 0;
  //					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            }
            break;

            //get frequency
				case (0x34) :
				{
                printf("get frequency\n\r");

					strcpy(OutBuff,"4");
					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            	Sleep(1000);

					strcpy(OutBuff,"0");
            	WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            	Sleep(1000);

               strcpy(OutBuff,"?");
            	WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            	Sleep(1000);

					OutBuff[0] = 0x0D;
					OutBuff[1] = 0x0A;
					OutBuff[2] = 0;
					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
            }
            break;


				case (0x04) :
				{

					/* allocate memory for string */
					if ((str = (char *) malloc(10)) == NULL)
					{
						printf("Not enough memory to allocate buffer\r\n");
					}

					/* copy "Hello" into string */
					strcpy(str, "Hello");

			/* 	free memory */
					WriteFile(hComm,str,strlen(str),&dwBytesWritten,NULL);
					free(str);
					break;
				}
			default:
				{
					/*OutBuff[0] = Key;
					OutBuff[1] = 0;
					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);*/
               printf("invalid key\r\n");

					break;
				}
			}

			//TransmitCommChar(hComm, Key);

		}

		ReadFile(hComm, InBuff, 1, &dwBytesRead, NULL);
		if(dwBytesRead)
		{
			putch(*InBuff);
		}
	}while(!done);

	// Fermeture du port de comm
	if(hComm)
	{
		SetCommTimeouts(hComm, &ctmoOld);
		CloseHandle(hComm);
	}

}
