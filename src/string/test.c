
#include <stdio.h>
#include "string.h"

int main(void)
{
	t_string *str = new_string(100);
	str->f_append(str, "hello");
	str->f_insert(str, 1, "hi");
	printf("%s\n", str->text);

	return (0);
}
