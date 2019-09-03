/*
 * Memoria.h
 *
 *  Created on: 28 de ago de 2019
 *      Author: Aluno
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdint.h>

#define NOP		0x00
#define STA		0x10
#define LDA		0x20
#define ADD		0x30
#define OR		0x40
#define AND		0x50
#define NOT		0x60
#define JMP		0x80
#define JN		0x90
#define JZ		0xA0
#define HLT		0xF0

unsigned char mem[256];



#endif /* MEMORIA_H_ */
