/****************************************************************************
*
*  APPLE //E EMULATOR FOR WINDOWS                    
*
*  Copyright (C) 1994-96, Michael O'Brien.  All rights reserved.
*
***/

/*  Emulador do computador TK2000 (Microdigital)
 *  por Fábio Belavenuto - Copyright (C) 2004
 *
 *  Adaptado do emulador Applewin por Michael O'Brien
 *  Part of code is Copyright (C) 2003-2004 Tom Charlesworth
 *
 *  Este arquivo é distribuido pela Licença Pública Geral GNU.
 *  Veja o arquivo Licenca.txt distribuido com este software.
 *
 *  ESTE SOFTWARE NÃO OFERECE NENHUMA GARANTIA
 *
 */

// Emula o teclado do TK2000

// Nota: Cada tecla é simulada como mantida por 10ms apertada.
//       Combinações de teclas não é simulada, a não ser o Joystick, SHIFT e o CTRL


#include "tk_stdhdr.h"
#include "tk_teclado.h"
#include "tk_main.h"
#include "tk_cpu.h"
#include "tk_memoria.h"
#include "tk_janela.h"
#include "tk_video.h"
#include "tk_joystick.h"
#include "tk_tape.h"
#include "tk_impressora.h"
#include "tk_disco.h"

// Definições
#define VK_IGUALMAIS  0xBB
#define VK_VIRGULA    0xBC
#define VK_MENOS      0xBD
#define VK_PONTO      0xBE
#define VK_DOISPONTOS 0xBF
#define VK_APOSTROFO  0xC0
#define VK_PERGUNTA   0xC1
#define VK_NPPONTO    0xC2

#define KBMAXCICLOS   15000
#define KBMAXCICLOS2   8000

#define FLGINVSH      0x01

typedef struct
{
	BYTE linha;
	BYTE coluna;
	BYTE flags;
} TVk;

typedef struct
{
	BYTE teclasub1;  // se shift off
	BYTE onshift1;   // se shift off
	BYTE teclasub2;  // se shift on
	BYTE onshift2;   // se shift on
} Convs;

const char caracs[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 /?.>,<;:@^+=-*)('&!\"#$%\x0D";

// Variáveis
BYTE  bKBOUT  = 0;
BYTE  bCTRL   = 0;
TVk   Teclas[256];
Convs Conversoes[256];
char  *ListaTeclas;
unsigned __int64 CicloUT = 0, CicloUT2 = 0;
static bool g_bPasteFromClipboard = false;
static bool g_bClipboardActive = false;
static HGLOBAL hglb = NULL;
static LPTSTR lptstr = NULL;
int ColarRapido = 0;

//===========================================================================
static void ClipboardDone()
{
	if (g_bClipboardActive)
	{
		g_bClipboardActive = false;
		GlobalUnlock(hglb);
		CloseClipboard();
	}
}

//===========================================================================
static void ClipboardInit()
{
	ClipboardDone();

	if (!IsClipboardFormatAvailable(CF_TEXT))
		return;

	if (!OpenClipboard((HWND)framewindow))
		return;

	hglb = GetClipboardData(CF_TEXT);
	if (hglb == NULL)
	{	
		CloseClipboard();
		return;
	}

	lptstr = (char*) GlobalLock(hglb);
	if (lptstr == NULL)
	{	
		CloseClipboard();
		return;
	}

	g_bPasteFromClipboard = false;
	g_bClipboardActive = true;
}

//===========================================================================
static char ClipboardCurrChar(bool bIncPtr)
{
	char nKey;
	int nInc = 1;

	if((lptstr[0] == 0x0D) && (lptstr[1] == 0x0A)) {
		nKey = 0x0D;
		nInc = 2;
	} else {
		nKey = lptstr[0];
		if (nKey > 0x60 && nKey < 0x7B)
			nKey -= 0x20;
	}

	if(bIncPtr)
		lptstr += nInc;

	return nKey;
}

//
// ----- ALL GLOBALLY ACCESSIBLE FUNCTIONS ARE BELOW THIS LINE -----
//

//===========================================================================
void ClipboardInitiatePaste()
{
	if (g_bClipboardActive)
		return;

	g_bPasteFromClipboard = true;
}

//===========================================================================
void KeybInicia(void)
{
	ListaTeclas = (char *)malloc(1024);
	memset(ListaTeclas, 0, 1024);
	memset(Teclas, 0xFF, sizeof(TVk)*256);
	memset(Conversoes, 0, sizeof(Convs)*256);

#define CONFTECLA(vlinha, vcoluna, vflags, tecla)	\
				Teclas[(tecla)].linha  = (vlinha);	\
				Teclas[(tecla)].coluna = (vcoluna);	\
				Teclas[(tecla)].flags  = (vflags);

	CONFTECLA(7, 5, FLGINVSH, VK_PERGUNTA)
	CONFTECLA(7, 4, 0, VK_PONTO)
	CONFTECLA(7, 3, 0, VK_VIRGULA)
	CONFTECLA(7, 2, 0, 'M')
	CONFTECLA(7, 1, 0, 'N')
	CONFTECLA(7, 0, 0, VK_RETURN)
	CONFTECLA(6, 5, FLGINVSH, VK_DOISPONTOS)
	CONFTECLA(6, 4, 0, 'L')
	CONFTECLA(6, 3, 0, 'K')
	CONFTECLA(6, 2, 0, 'J')
	CONFTECLA(6, 1, 0, 'H')
	CONFTECLA(6, 0, 0, VK_UP)
	CONFTECLA(5, 5, 0, 'P')
	CONFTECLA(5, 4, 0, 'O')
	CONFTECLA(5, 3, 0, 'I')
	CONFTECLA(5, 2, 0, 'U')
	CONFTECLA(5, 1, 0, 'Y')
	CONFTECLA(5, 0, 0, VK_DOWN)
	CONFTECLA(4, 5, 0, '0')
	CONFTECLA(4, 4, 0, '9')
	CONFTECLA(4, 3, 0, '8')
	CONFTECLA(4, 2, 0, '7')
	CONFTECLA(4, 1, 0, '6')
	CONFTECLA(4, 0, 0, VK_RIGHT)
	CONFTECLA(3, 5, 0, '1')
	CONFTECLA(3, 4, 0, '2')
	CONFTECLA(3, 3, 0, '3')
	CONFTECLA(3, 2, 0, '4')
	CONFTECLA(3, 1, 0, '5')
	CONFTECLA(3, 0, 0, VK_LEFT)
	CONFTECLA(2, 5, 0, 'Q')
	CONFTECLA(2, 4, 0, 'W')
	CONFTECLA(2, 3, 0, 'E')
	CONFTECLA(2, 2, 0, 'R')
	CONFTECLA(2, 1, 0, 'T')
	CONFTECLA(2, 0, 0, ' ')
	CONFTECLA(1, 5, 0, 'A')
	CONFTECLA(1, 4, 0, 'S')
	CONFTECLA(1, 3, 0, 'D')
	CONFTECLA(1, 2, 0, 'F')
	CONFTECLA(1, 1, 0, 'G')
	CONFTECLA(0, 5, 0, 'Z')
	CONFTECLA(0, 4, 0, 'X')
	CONFTECLA(0, 3, 0, 'C')
	CONFTECLA(0, 2, 0, 'V')
	CONFTECLA(0, 1, 0, 'B')
#undef CONFTECLA

#define CONFCONV(tecla, tecla1, onsh1, tecla2, onsh2)		\
				Conversoes[(tecla)].teclasub1 = (tecla1);	\
				Conversoes[(tecla)].onshift1  = (onsh1);	\
				Conversoes[(tecla)].teclasub2 = (tecla2);	\
				Conversoes[(tecla)].onshift2  = (onsh2);

	CONFCONV(VK_BACK,       VK_LEFT,  0, 0,   0)	// Backspace = Left
	CONFCONV(VK_IGUALMAIS,  'O',      1, 'P', 1)	// = = SHIFT+O	+ = SHIFT+P
	CONFCONV(VK_MENOS,      'I',      1, 0,   0)    // - = SHIFT+I
	CONFCONV(VK_APOSTROFO,  '7',      1, '2', 1)    // ' = SHIFT+7	" = SHIFT+2
	CONFCONV('2',           0,        0, 'L', 1)    // @ = SHIFT+L
	CONFCONV('6',           0,        0, 'K', 1)    // ^ = SHIFT+K
	CONFCONV('7',           0,        0, '6', 1)    // & = SHIFT+6
	CONFCONV('8',           0,        0, '0', 1)    // * = SHIFT+0
	CONFCONV('9',           0,        0, '8', 1)    // ( = SHIFT+8
	CONFCONV('0',           0,        0, '9', 1)    // ) = SHIFT+9


#undef CONFCONV
}

//===========================================================================
void KeybAtualiza(DWORD totalcycles)
{
	if(g_bPasteFromClipboard) {
		ClipboardInit();
		CicloUT2 = g_nCumulativeCycles;
	}

	if (CicloUT) {
		if ((g_nCumulativeCycles - CicloUT > KBMAXCICLOS)) {
			strcpy(ListaTeclas, ListaTeclas+1);
			if (strlen(ListaTeclas))
				CicloUT = g_nCumulativeCycles;
			else
				CicloUT = 0;
			CicloUT2 = g_nCumulativeCycles;
		}
	}

	if (CicloUT2) {
		if ((g_nCumulativeCycles - CicloUT2 > KBMAXCICLOS2)) {
			CicloUT2 = 0;
			if(g_bClipboardActive) {
				if(*lptstr == 0) {
					ClipboardDone();
				} else {
					KeybQueueKeypress(ClipboardCurrChar(true), true);
					CicloUT = g_nCumulativeCycles;
				}
			}
		}
	}
};

//===========================================================================
void KeybFinaliza(void)
{
	if (ListaTeclas)
	{
		free(ListaTeclas);
		ListaTeclas = NULL;
	}
}

//===========================================================================
void KeybQueueKeypress (int key, BOOL ascii)
{
	int shift = 0;
	if (!ascii)
		shift = GetKeyState(VK_SHIFT);

	if (GetKeyState(VK_MENU) < 0)
		return;
/*
	{
		if (key > 0x7F) return;
		if ((key >= 'a') && (key <= 'z'))
			key -= ('a'-'A');
		if (strchr(caracs, key))
		{
			shift = VkKeyScan(key) & 0x100 ? -1 : 0;
			key = LOBYTE(VkKeyScan(key));
		}
		else
			return;
	}
*/
	if ((key == VK_CANCEL && GetKeyState(VK_CONTROL) < 0) || key == VK_F12)
	{
		// RESET
		MemResetPaging();
		DiskReset();
		ImpressoraReset();
		VideoResetState();
		regs.pc = mem_readw(0xFFFC, 0);
		return;
	}
	else if ((key == VK_INSERT) && (GetKeyState(VK_SHIFT) < 0))
	{
		// Shift+Insert
		ClipboardInitiatePaste();
		return;
	}
	key &= 0xFF;

	// Conversões de teclas:
	if ((key >= VK_NUMPAD0) && (key <= VK_NUMPAD9))
		key -= 0x30;
	else
		switch (key)
		{
			case VK_DECIMAL:   key = VK_VIRGULA;      break; // ,
			case VK_MULTIPLY:  key = '8'; shift = -1; break; // *
			case VK_ADD:       key = VK_IGUALMAIS; 
								shift = -1;           break; // +
			case VK_NPPONTO:   key = VK_PONTO;        break; // .
			case VK_SUBTRACT:  key = VK_MENOS;        break; // -
			case VK_DIVIDE:    key = VK_PERGUNTA;     break; // /
		}


	if (Conversoes[key].teclasub1 && !(shift < 0))
	{
		shift = Conversoes[key].onshift1 ? -1 : 0;
		key   = Conversoes[key].teclasub1;
	}
	else if (Conversoes[key].teclasub2 && (shift < 0))
	{
		shift = Conversoes[key].onshift2 ? -1 : 0;
		key   = Conversoes[key].teclasub2;
	}

	if (Teclas[key].linha != 0xFF)
	{
		char temp[2];
		BYTE r;

		if (Teclas[key].flags & FLGINVSH)
			shift = shift < 0 ? 0 : -1;

		r = (shift < 0 ? 0x80 : 0x00);

		r |= (Teclas[key].linha) | (Teclas[key].coluna << 3);
		sprintf(temp, "%c", r);
		if (strlen(ListaTeclas) > 1023) {
			//
		} else {
			strncat(ListaTeclas, temp, 1); // Adiciona 1 tecla na lista
		}
		CicloUT  = g_nCumulativeCycles; // Memoriza Ciclo da Ultima Tecla
	}
}

//===========================================================================
BYTE __stdcall KeybCTRL0 (WORD programcounter, BYTE address, BYTE write, BYTE value)
{
	bCTRL = 0;
	return MemReturnRandomData(0);
}

//===========================================================================
BYTE __stdcall KeybCTRL1 (WORD programcounter, BYTE address, BYTE write, BYTE value)
{
	bCTRL = 1;
	return MemReturnRandomData(0);
}

//===========================================================================
BYTE __stdcall KeybKBOUT (WORD programcounter, BYTE address, BYTE write, BYTE value)
{
	if (write)
	{
		bKBOUT = value;
	}
	return bKBOUT;
}

//===========================================================================
BYTE __stdcall KeybKBIN (WORD programcounter, BYTE address, BYTE write, BYTE value)
{
	int  l;
	BYTE t;
//	BYTE result = TapeCasIn();
	BYTE result = 0;

	if (bCTRL && (GetKeyState(VK_CONTROL) < 0))
		return result | 0x01;

	KeybAtualiza(0);

	t = (BYTE)ListaTeclas[0];

	for (l = 0; l < 8; l++)
		if (bKBOUT == (1 << l))
		{
			result |= JoyVerificaTecla(l);
			if (l == 0 && (t & 0x80))
				result |= 0x01; // SHIFT
			if (t && l == (t & 0x07)) {
				result |= 1 << ((t & 0x38) >> 3);
			}
			return result;
		}
	return result;
}

// EOF