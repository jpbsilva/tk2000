
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

#ifndef MEMORIA_H
#define MEMORIA_H

// Defini��es Gerais
BYTE __stdcall NullIo (WORD programcounter, BYTE address, BYTE write, BYTE value);

// Defini��es de Tipos
typedef	BYTE (__stdcall	*iofunction)(WORD,BYTE,BYTE,BYTE);

// Vari�veis Externas
extern iofunction ioread[0x100];
extern iofunction iowrite[0x100];
//extern BYTE* memaux;
extern BYTE* memdirty;
extern BYTE* memmain;
extern BYTE* memread [0x100];
extern BYTE* memwrite[0x100];
extern BYTE	 SlotAux;
extern BYTE  NewSlotAux;

// Prot�tipos
int   MemAtualizaSlotAux(void);
int   MemRetiraSlotAux();
int   MemInsereSlotAux(iofunction[0x10], iofunction[0x10],char*);
BYTE  mem_readb(WORD, BOOL);
void  mem_writeb(WORD, BYTE);
WORD  mem_readw(WORD, BOOL);
void  mem_writew(WORD, WORD);
void  MemDestroy();
BYTE* MemGetMainPtr(WORD);
void  MemInitialize();
void  MemReset();
BYTE  MemReturnRandomData(BYTE);
BOOL  MemImportar(WORD);
BOOL  MemExportar(WORD,WORD);
void  MemResetPaging();

// Prot�tipos Softswitches
BYTE __stdcall MemCheckPaging (WORD,BYTE,BYTE,BYTE);
BYTE __stdcall MemSetPaging (WORD,BYTE,BYTE,BYTE);

#endif
// EOF