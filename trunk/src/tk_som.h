
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

#ifndef SOM_H
#define SOM_H

// Vari�veis Externas
extern DWORD  soundtype;
extern double g_fClksPerSpkrSample;

// Prot�tipos
void SpkrDestroy ();
void SpkrInitialize ();
void SpkrReinitialize ();
void SpkrReset();
void SetClksPerSpkrSample();
BOOL SpkrSetEmulationType (HWND,DWORD);
static void InitRemainderBuffer(UINT nCyclesRemaining);
static void UpdateRemainderBuffer(LONG* pnCycleDiff);

void SpkrUpdate (DWORD);

// Prot�tipos Softswitches
BYTE __stdcall SpkrToggle (WORD,BYTE,BYTE,BYTE);

#endif
// EOF