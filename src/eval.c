/*
 * eval.c
 *
 *  Created on: Jul 5, 2015
 *      Author: mflamer
 */

#include "eval.h"

int eval_vector(Expr* self, void* c)
{
	int x = (int)self->ar;
	if(((uint16_t*)&x)[0] < ((uint16_t*)&x)[1])
	{
		c = (void*)0;
		self->ar++;
		return ((int*)self->al)[((uint16_t*)&x)[0]];
	}
	else
	{
		c = (void*)1;
		return 0;
	}
}

int eval_string(Expr* self, void* c)
{

	return 0;
}

int eval_add(Expr* self, void* c)
{
	void* cr = 0;
	void* cl = 0;
	int result = 0;
	int r = ((Expr*)self->al)->op(self->al, cr);
	int l = ((Expr*)self->ar)->op(self->ar, cl);
	if(!cr && !cl)
		result = r + l;
	c = (void*)((int)cr & (int)cl);
	return result;
}
