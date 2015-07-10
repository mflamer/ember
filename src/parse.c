/*
 * parse.c
 *
 *  Created on: Jul 3, 2015
 *      Author: mflamer
 */

#include "parse.h"
#include "data.h"
#include "eval.h"

typedef enum
{
	mode_init,
	mode_vec,
	mode_string,
	mode_symbAlph,
	mode_symbOp
}ParseMode;

static Expr* root;
static ParseMode mode;
static Strength stkStr;
static int value;

//push bind L
//pop bind R
void finish_expr()
{
	if(mode != mode_init)
	{
		Expr* expr = new_expr();
		Binding bind;
		Strength str;

		switch(mode)
		{
			case mode_vec:
			{
				expr->al = get_int_array_base();
				int x;
				((uint16_t*)&x)[0] = 0;
				((uint16_t*)&x)[1] = get_int_array_size();
				expr->ar = (void*)x;
				expr->op = (eFunc)&eval_vector;
				bind = bind_none;
				break;
			}
			case mode_string:
			{
				expr->al = get_char_array_base();
				int x;
				((uint16_t*)&x)[0] = 0;
				((uint16_t*)&x)[1] = get_char_array_size();
				expr->ar = (void*)x;
				expr->op = (eFunc)&eval_string;
				bind = bind_none;
				break;
			}
			case mode_symbAlph:
			case mode_symbOp:
			{
				Symbol* symb = get_symbol(value);
				expr = symb->expr;
				bind = symb->bind;
				str = symb->str;
				break;
			}
			case mode_init:
			default:
				break;
		}

		if(bind == bind_none)
		{
			root = expr;
		}
		else
		{
			while(get_parse_stack_size() > 0)
			{
				if(((bind == bind_L || bind == bind_LR) && str <= stkStr) ||
				   ((bind == bind_R || bind == bind_LR) && str < stkStr))
				{
					Expr* popped = pop_expr();
					popped->ar = root;
					root = popped;
				}
				else break;
			}
			expr->al = root;
			push_expr(expr);
		}


	}
	mode = mode_init;
	value = 0;
}

void drop_stack()
{
	while(get_parse_stack_size() > 0)
	{
		Expr* popped = pop_expr();
		popped->ar = root;
		root = popped;
	}
}

void finish_scalar()
{
	if(mode == mode_vec)
	{
		push_int(value);
		value = 0;
	}
}


Expr* parse(uint8_t* str)
{
	root = 0;
	mode = mode_init;
	stkStr = stren_express;
	value = 0;

	while(*str != 0)
	{
		uint8_t c = *str;
		uint8_t next_c = *(str+1);
		//numeral///////////////
		if(c >= 0x30 && c <= 0x39)
		{
			if(mode == mode_string)
			{
				push_char(c);
			}
			else
			{
				if(mode != mode_vec)
				{
					finish_expr();
					new_int_array();
					mode = mode_vec;
				}
				value = value * 10 + (c - 48);
			}

		}
		//alpha/////////////////
		else if((c >= 0x41 && c <= 0x5A) || (c >= 0x61 && c <= 0x7A))
		{
			if(mode == mode_string)
			{
				push_char(c);
			}
			else if(mode == mode_symbAlph)
			{
				value = ((value << 5) ^ value) + c;
			}
			else
			{
				finish_scalar();
				finish_expr();
				mode = mode_symbAlph;
				value = 5381;
				value = ((value << 5) ^ value) + c;
			}
		}
		//space
		else if(c == 0x20)
		{
			if(mode == mode_string)
			{
				push_char(c);
			}
			else
			{
				finish_scalar();
				if(mode != mode_vec)
					finish_expr();
			}
		}
		// '
		else if(c == 0x27)
		{
			if(mode == mode_string)
			{
				if(next_c == 0x27) //escape '
				{
					push_char(c);
					str++;
				}
				else
					finish_expr();
			}
			else
			{
				finish_scalar();
				finish_expr();
				new_char_array();
				mode = mode_string;
			}
		}
		// (
		else if(c == 0x28 && mode != mode_string)
		{
			finish_scalar();
			finish_expr();
		}
		// )
		else if(c == 0x29 && mode != mode_string)
		{
			finish_scalar();
			finish_expr();
		}
		//other/////////////////
		else
		{
			finish_scalar();
			finish_expr();
			value = 5381;
			value = ((value << 5) ^ value) + c;
			mode = mode_symbOp;
			finish_expr();
		}

		str++;
	}
	finish_scalar();
	finish_expr();
	drop_stack();
	return root;
}













