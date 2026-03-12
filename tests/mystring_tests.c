
#include <criterion/criterion.h>

#include "../src/mystring.h"

#include <criterion/internal/assert.h>
#include <stdbool.h>
#include <stdio.h>

String *GVAR_str = NULL;

void setup(void) { GVAR_str = Str_new(); }

TestSuite(String_tests_constructors, .init = setup);

Test(String_tests_constructors, create) {
    String *str = string("Sumbo");
    const char *const content = String_data(str);

    cr_expect(strcmp("Sumbo", content) == 0,
              "create(\"%s\") returns this \"%s\"", "Sumbo", content);

    String_free(str);
}

Test(String_tests_constructors, new) {
    String *str = Str_new();
    cr_expect(str != NULL, "String should not be NULL!");

    const char *const content = String_data(str);
    cr_expect(strcmp("", content) == 0, "new() returns this \"%s\"", content);
    String_free(str);
}

Test(String_tests_constructors, dup) {
    String *str = string("Dias");
    String *copy = String_dup(str);

    const char *const str_text = String_data(str);
    const char *const copy_text = String_data(copy);

    cr_expect(strcmp(str_text, copy_text) == 0,
              "copy  \"%s\" and duplicate ar different \"%s\"", str_text,
              copy_text);

    String_free(str);
}

/* Test(String_tests_destructors, assign) */
/* { */
/*     String * GVAR_str = String_create("Sumbo", strlen("Sumbo")); */
/*     cr_expect(GVAR_str != NULL, "String should not be NULL!"); */
/* } */

/* Test(String_tests_querying, len) */
/* { */
/*     String * GVAR_str = String_create("Sumbo", strlen("Sumbo")); */
/*     cr_expect(GVAR_str != NULL, "String should not be NULL!"); */
/* } */

/* int main(int argc, char *argv[]) { */

/*     String * line = string(""); */
/*     String * ptr = line; */
/*     line = String_readln("Enter some text: "); */
/*     printf("You've entered: %s\n", String_data(line)); */

/*     if (argc > 1) */
/*     { */
/*         String * cmd_args = string(""); */
/*         printf("Y've also entered: \n"); */

/*          for (int32_t i = 1; i < argc; i++) { */
/*              String_catcstr(cmd_args, argv[i]); */
/*              String_catcstr(cmd_args, "\n"); */
/*          } */
/*          String_print(cmd_args); */
/*          String_free(cmd_args); */
/*      } */

/*     String_free(line); */
/*     String_free(ptr); */

/*     return EXIT_SUCCESS; */
/* } */
