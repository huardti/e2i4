// comm01 :
// 1) initialise un port de communication série, puis
// 2) les caratères reçus au clavier sont envoyés sur la liaison série,
 //  les caractères lus sur la liaison série sont affichés
 // sortie par ESC

#include <windows.h>
#include <stdio.h>
#include <conio.h>

HANDLE hComm = NULL;
COMMTIMEOUTS ctmoNew = {0}, ctmoOld;
 DCB dcbCommPort;										// Device Control Block
char InBuff[100];
char	OutBuff[100];
char *str;

char done;
DWORD dwBytesRead,dwBytesWritten;

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
	ctmoNew.ReadIntervalTimeout = MAXDWORD;		//	Retour immédiat de l'opération de lecture
	ctmoNew.ReadTotalTimeoutConstant = 0;			// même si aucun car. n'a été reçu
	ctmoNew.ReadTotalTimeoutMultiplier = 0;			//
	ctmoNew.WriteTotalTimeoutMultiplier = 0;
	ctmoNew.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(hComm, &ctmoNew);

	// Définition  BAUD RATE, PARITY, WORD SIZE, et STOP BITS.
	// Nota :
	// l'argument de BuildCommDCB doit être un pointeur sur une chaîne.
  // BuildCommDCB() val par défaut : NO HANDSHAKING.

	dcbCommPort.DCBlength = sizeof(DCB);
	GetCommState(hComm, &dcbCommPort);
	BuildCommDCB("9600,N,8,1", &dcbCommPort);
	SetCommState(hComm, &dcbCommPort);


	// Boucle principale : les caratères reçus au clavier sont envoyés sur la liaison série,
	// les caractères lus sur la liaison série sont affichés

	done = FALSE;
	do
	{
		if(kbhit())
		{
			Key = getch();
			switch(Key)
			{
				case (0x1B): done = TRUE; break;
				case (0x02) :
				{
				//strcpy(OutBuff,"BREAK\r");
				//WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
				strcpy(OutBuff,"B");
				WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
				strcpy(OutBuff,"R");
				WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
				strcpy(OutBuff,"E");
				WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
				strcpy(OutBuff,"A");
				WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
				strcpy(OutBuff,"K");
				WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
				OutBuff[0] = 0x0D;
				OutBuff[1] = 0x0A;
				OutBuff[2] = 0;
				WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
				break;
				}
				case (0x04) :
				{

					/* allocate memory for string */
					if ((str = (char *) malloc(10)) == NULL)
					{
						printf("Not enough memory to allocate buffer\n");
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
					OutBuff[0] = Key;
					OutBuff[1] = 0;
					WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
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

