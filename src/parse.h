/*
 * parse.h
 *
 *  Created on: Jul 3, 2015
 *      Author: mflamer
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <stdint.h>

#define table_size 256

typedef enum
{
	bind_none,
	bind_L,
	bind_R,
	bind_LR
} Binding;

typedef enum
{
	stren_express,
	stren_assign,
	stren_low,
	stren_add,
	stren_mul,
	stren_pow,
	stren_strong,
	stren_always
} Strength;

typedef int (*eFunc)(struct Expr*, void*);

typedef struct Expr
{
	eFunc op;
	void* al;
	void* ar;
	struct Expr* self;
}Expr;

typedef struct Symbol
{
	int key;
	Binding bind;
	Strength str;
	Expr* expr;
}Symbol;

Expr* 		parse(uint8_t* str);


#endif /* PARSE_H_ */
