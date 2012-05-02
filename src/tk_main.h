
/*  Emulador do computador TK2000 (Microdigital)
 *  por F�bio Belavenuto - Copyright (C) 2004
 *
 *  Adaptado do emulador Applewin por Michael O'Brien
 *  Part of code is Copyright (C) 2003-2004 Tom Charlesworth
 *
 *  Este arquivo � distribuido pela Licen�a P�blica Geral GNU.
 *  Veja o arquivo Licenca.txt distribuido com este software.
 *
 *  ESTE SOFTWARE N�O OFERECE NENHUMA GARANTIA
 *
 */

#ifndef MAIN_H
#define MAIN_H

// Defini��o do	Modo do	Processador
#define	 MODE_LOGO		0
#define	 MODE_PAUSED	1
#define	 MODE_RUNNING	2
#define	 MODE_DEBUG		3
#define	 MODE_STEPPING	4

// Vers�o, Titulo e Arq de Rom
#define	VERSIONSTRING "0.2"
#define	TITULO        "Emulador TK2000"
#define	NOMEARQROM    "TK2000.rom"
#define	NOMEARQSYM    "TK2000.sym"
#define	NOMEARQBMP    "TK2000.bmp"
#define NOMEARQTKDOS  "TKDOS2000.dsk"

// Defini��es Gerais
#define	 SPEED_NORMAL	10
#define	 SPEED_MAX		40

#define  SPKR_SAMPLE_RATE 44100

#define CLK_6502 (14.31818e6 / 14.0)

#define	 MAX(a,b)	(((a) > (b))	? (a) :	(b))
#define	 MIN(a,b)	(((a) < (b))	? (a) :	(b))

typedef	struct _IMAGE__
{
	int unused;
} *HIMAGE;

// Vari�veis Externas
extern HINSTANCE instance;
extern DWORD  cumulativecycles;
extern DWORD  cyclenum;
extern DWORD  extbench;
extern int    fullspeed;
//extern DWORD  image;
//extern DWORD  lastimage;
//extern BYTE   slotext;
extern int    mode;	
extern char   progdir[MAX_PATH];
extern char   tempdir[MAX_PATH];
extern int    resettiming;
extern int    restart;
extern DWORD  speed;
extern double g_fCurrentCLK6502;
extern int    g_nCpuCyclesFeedback;
extern unsigned __int64 g_nCumulativeCycles;
extern double g_fMHz;
extern FILE*  g_fh;

// Prototipos
void SetCurrentCLK6502();
void Inicializar1();
void Inicializar2();
void Finalizar(int restart);

#endif
// EOF