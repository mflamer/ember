/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "parse.h"
#include "data.h"
			

int main(void)
{

	init_data();
	init_table();
	Expr* e = parse("5+5");
	void* c = 0;
	int result = e->op(e, c);
	for(;;);
}
