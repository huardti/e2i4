#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#define TEMPO_GF265		50
#define MAX_FRAME		50

using namespace std;

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


//fonction permettant d'envoyer une chaîne de caractères
//La fonction envoie caractères par caractères au GBF jusqu'à la détection d'un caractère de fin de chaîne
static void SendFrame(char *ptr_buffer)
{
   unsigned int i;

   i = 0;

   while((*(ptr_buffer + i) != 0) && (i < MAX_FRAME))
   {
      OutBuff[0] = *(ptr_buffer + i); //stocke dans le buffer de transmission un caractère de la chaîne à transmettre
      //cout << OutBuff[0];
      OutBuff[1] = 0x00;						//NULL character
		WriteFile(hComm,OutBuff,strlen(OutBuff),&dwBytesWritten,NULL);
   	Sleep(TEMPO_GF265);        			//le GBF ne possède pas de Handshaking - il est nécessaire d'ajouter une tempo entre chaque envoie de caractères
      i++;
   }
}

int main()
{

    char conv_int_char[1];

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

    char trame[MAX_FRAME];
    SendFrame("00\r"); // envoie sinus

    SendFrame("80 1\r");
    for(int i=1; i<10000; i+=100)
    {
        sprintf(trame , "40 %i\r" , i);
        SendFrame(trame);
    }




    // Fermeture du port de comm
	if(hComm)
	{
		SetCommTimeouts(hComm, &ctmoOld);
		CloseHandle(hComm);
	}

    return 0;
}
