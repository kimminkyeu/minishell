#include <stdio.h>
#include "string.h"

/** NOTE 
 *
 * This is a test_case for string.h
 *
 * */

int main(void)
{
	t_string *str;

	str = new_string(2);
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	str->append(str, "hello");
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	str->append(str, " world!");
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	str->append(str, ", welcome");
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	str->push_back(str, 'a');
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);
	str->push_back(str, 'b');
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);
	str->push_back(str, 'c');
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);
	str->push_back(str, 'd');
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	str->pop_back(str);
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);
	str->pop_back(str);
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);
	str->pop_back(str);
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	str->shrink_to_fit(str);
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	str->clear(str);
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);
	str->shrink_to_fit(str);
	printf("%s\nlen:%zd capa:%zd\n", str->text, str->text_len, str->capacity);

	delete_string(&str);
}
