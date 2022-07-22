
#include <stdio.h>
#include "string.h"

int main(void)
{
/**     t_string *str = new_string(5);
  *     printf("%s\n", str->text);
  *     printf("len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_append(str, "hi");
  *     printf("%s\n", str->text);
  *     printf("(1) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, 0, 1, "");
  *     printf("%s\n", str->text);
  *     printf("(2) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, 0, 1, "");
  *     printf("%s\n", str->text);
  *     printf("(3) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, 0, 1, "");
  *     printf("%s\n", str->text);
  *     printf("(4) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, 0, 1, "");
  *     printf("%s\n", str->text);
  *     printf("(5) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, 5, 20, "@@@");
  *     printf("%s\n", str->text);
  *     printf("(6) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *
  *     str->f_replace(str, 0, 2, "#############");
  *     printf("%s\n", str->text);
  *     printf("(7) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, 4, 2, "&&&");
  *     printf("%s\n", str->text);
  *     printf("(8) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, 0, 100, NULL);
  *     printf("%s\n", str->text);
  *     printf("(9) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *
  *     str->f_replace(str, str->text_len - 1, 4, "QQ");
  *     printf("%s\n", str->text);
  *     printf("(10) len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
  *  */

	t_string *str = new_string(10);


	str->f_append(str, "\"test_string \"to\" delete\"");
	printf("%s\n", str->text);
	printf("len: %zd, capa: %zd\n\n", str->text_len, str->capacity);

	str->f_replace_all(str, "\"", "");
	printf("%s\n", str->text);
	printf("len: %zd, capa: %zd\n\n", str->text_len, str->capacity);

	str->f_clear(str);
	printf("%s\n", str->text);
	printf("len: %zd, capa: %zd\n\n", str->text_len, str->capacity);
	str->f_shrink_to_fit(str);
	printf("%s\n", str->text);
	printf("len: %zd, capa: %zd\n\n", str->text_len, str->capacity);

	str->f_append(str, "ec$ECHO!");
	printf("%s\n", str->text);
	printf("len: %zd, capa: %zd\n\n", str->text_len, str->capacity);

	str->f_replace_all(str, "$ECHO", "ho hello");
	printf("%s\n", str->text);
	printf("len: %zd, capa: %zd\n\n", str->text_len, str->capacity);

	delete_string(&str);

	return (0);
}
