
#ifndef DATA_H_
#define DATA_H_

#include <stdint.h>
#include "parse.h"
#include "eval.h"

#define EXPR_POOL_SIZE 256		/*expressions*/
#define DATA_POOL_SIZE 4096 	/*bytes*/



typedef struct ArrayStack
{
	uint16_t size;
	uint8_t* elements;
	uint8_t elemSize;
}ArrayStack;

typedef struct ExprStack
{
	uint16_t size;
	Expr** elements;
}ExprStack;


uint8_t*	align(ArrayStack *S);

/*Expression Pool////////////////////////////////////////////*/

void		init_data();

Expr*		new_expr();
void		drop_expr();

/*Expr Stack////////////////////////////////////////////*/
void		push_expr(Expr* e);
Expr*		pop_expr();

void		_push_expr(ExprStack* s, Expr* e);
Expr*		_pop_expr(ExprStack* stk);
int			get_parse_stack_size();

/*Data Stack////////////////////////////////////////////////////*/

int*		new_int_array();
int*		get_int_array_base();
uint16_t	get_int_array_size();
void		push_int(int x);

uint8_t*    new_char_array();
uint8_t*	get_char_array_base();
uint16_t	get_char_array_size();
void		push_char(uint8_t c);

/*Symbol Table////////////////////////////////////////////////////*/

void		init_table();
void 		set_symbol(int key, Binding bind, Strength str, Expr* expr);
Symbol* 	get_symbol(int key);
void 		init_symbol(char* s, Binding bind, Strength str, eFunc f);
int			hash_str(const char* s);



#endif /* DATA_H_ */
