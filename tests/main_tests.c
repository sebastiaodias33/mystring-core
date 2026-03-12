#include "../src/mystring.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int32_t Test_String_create();
int32_t Test_String_clone();
int32_t Test_String_from_cstr();
int32_t Test_String_create();
int32_t Test_String_create();
int32_t Test_String_create();
int32_t Test_String_create();

int main(int argc, char *argv[]) {

    String *line = String_readln("Enter some text: ");
    printf("You've entered: %s\n", String_data(line));

    if (argc > 1) {
        printf("Y've also entered: \n");
        String *cmd_args = string("");
        for (int32_t i = 1; i < argc; i++) {
            String_catcstr(cmd_args, argv[i]);
            String_catcstr(cmd_args, "\n");
        }
        String_print(cmd_args);

        String_free(cmd_args);
    }

    String_free(line);

    return EXIT_SUCCESS;
}
