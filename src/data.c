
#include "data.h"

static Expr expr_pool[EXPR_POOL_SIZE];
static Expr* expr_ptrs[EXPR_POOL_SIZE];
static ExprStack expr_stack;

static Expr* parse_stack_data[32];
static ExprStack parse_stack;

static uint8_t array_pool[DATA_POOL_SIZE];
static ArrayStack array_stack;

static Symbol table[table_size];


/*Expression Pool////////////////////////////////////////////*/

void init_data()
{
	expr_stack.elements = expr_ptrs;
	expr_stack.size = 0;

	int i;
	for(i = 0; i < EXPR_POOL_SIZE; i++)
		_push_expr(&expr_stack, &expr_pool[i]);

	parse_stack.elements = parse_stack_data;
	parse_stack.size = 0;

	array_stack.elements = array_pool;
	array_stack.elemSize = 1;
	array_stack.size = 0;
}

Expr* new_expr()
{
	return _pop_expr(&expr_stack);
}

void drop_expr(Expr* e)
{
	e->al = 0;
	e->ar = 0;
	e->op = 0;
	e->self = 0;
	_push_expr(&expr_stack, e);
}

/*ExprStack//////////////////////////////////////////////////*/

void push_expr(Expr* e)
{
	_push_expr(&parse_stack, e);
}

Expr* pop_expr()
{
	return _pop_expr(&parse_stack);
}

void _push_expr(ExprStack* s, Expr* e)
{
	s->elements[s->size] = e;
	s->size++;
}

Expr* _pop_expr(ExprStack* s)
{
	s->size--;
	return s->elements[s->size];
}

int	get_parse_stack_size()
{
	return parse_stack.size;
}

/*Data Pool////////////////////////////////////////////////////*/

uint8_t* new_char_array()
{
	array_stack.elements = array_stack.elements + array_stack.size;
	array_stack.size = 0;
	array_stack.elemSize = 1;
	return array_stack.elements;
}

int* new_int_array()
{
	array_stack.elements = align(&array_stack);
	array_stack.size = 0;
	array_stack.elemSize = 4;
	return (int*)array_stack.elements;
}

uint8_t* get_char_array_base()
{
	return array_stack.elements;
}

int* get_int_array_base()
{
	return (int*)array_stack.elements;
}

uint16_t get_char_array_size()
{
	return array_stack.size;
}

uint16_t get_int_array_size()
{
	return array_stack.size / 4;
}

void push_char(uint8_t c)
{
	array_stack.elements[array_stack.size] = c;
	array_stack.size++;
}

void push_int(int x)
{
	((int*)array_stack.elements)[array_stack.size] = x;
	array_stack.size += 4;
}

uint8_t* align(ArrayStack *S)
{
	int result = (int)S->elements + S->size;
	if((int)result & 0x3)
	{
		result = result & 0xFFFFFFFC;
		result += 0x3;
	}
	return (void*)result;
}


/*Symbol table/////////////////////////////////////////////////////////*/

void init_table()
{
	init_symbol("+", bind_LR, stren_add, (eFunc)&eval_add);
}

void set_symbol(int key, Binding bind, Strength str, Expr* expr)
{
	int idx = key;
	for(;; idx++)
	{
		idx &= table_size-1;
		if(table[idx].key == key || table[idx].key == 0)
		{
			table[idx].key = key;
			table[idx].bind = bind;
			table[idx].str = str;
			table[idx].expr = expr;
			return;
		}
	}
}

void init_symbol(char* s, Binding bind, Strength str, eFunc f)
{
	int k = hash_str(s);
	Expr* expr = new_expr();
	expr->op = f;
	set_symbol(k, bind, str, expr);
}


Symbol* get_symbol(int key)
{
	int idx = key;
	for(;; idx++)
	{
		idx &= table_size-1;
		if(table[idx].key == key)
			return &table[idx];
		if(table[idx].key == 0)
			return 0;
	}
	return 0;
}

int hash_str(const char* s)
{
	int result = 5381;
	for(;*s;s++)
		result = ((result << 5) ^ result) + *s;
	return result;
}

