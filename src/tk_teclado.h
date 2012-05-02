
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

#ifndef TECLADO_H
#define TECLADO_H

// Vari�veis Externas
extern BYTE bKBOUT;
extern int ColarRapido;

// Prot�tipos
void KeybInicia(void);
void KeybAtualiza(DWORD totalcycles);
void KeybFinaliza(void);
void KeybQueueKeypress (int key, BOOL down, BOOL ascii);
bool KeybIsPasting();

// Prot�tipos Softswitches
BYTE __stdcall KeybCTRL0(WORD, BYTE, BYTE, BYTE);
BYTE __stdcall KeybCTRL1(WORD, BYTE, BYTE, BYTE);
BYTE __stdcall KeybKBOUT(WORD, BYTE, BYTE, BYTE);
BYTE __stdcall KeybKBIN (WORD, BYTE, BYTE, BYTE);

#endif
// EOF