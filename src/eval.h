/*
 * eval.h
 *
 *  Created on: Jul 3, 2015
 *      Author: mflamer
 */

#ifndef EVAL_H_
#define EVAL_H_

#include "parse.h"

int eval_vector(Expr* self, void* c);
int eval_string(Expr* self, void* c);
int eval_add(Expr* self, void* c);

#endif /* EVAL_H_ */
