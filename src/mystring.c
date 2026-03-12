#include "mystring.h"

#include "dbg.h"

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _string_t {
    char *text;
    int32_t length;
    int32_t end;
    int32_t capacity;
};

#define INIT_CAPACITY 16

// Private helper functions declarations

static void fast_print(const String *self);

static void String_clear(String *self);

static void String_init(String *self, const char *c_string, int32_t cstr_len);

static String *String__new__();

// Constructors

String *Str_new() {

    /* strtok; */
    String *self = String__new__();
    String_init(self, NULL, 0);

    return self;
}

String *String_create(const char *c_string, int32_t cstr_len) {

    if (!c_string || cstr_len < 0) {
        return Str_new();
    }

    String *self = String__new__();
    check_mem(self);

    String_init(self, c_string, cstr_len);
    return self;

error:
    exit(EXIT_FAILURE);
    /* return NULL; */
}

String *String_dup(const String *self) {

    check(self != NULL, "In String_dup(). Invalid String reference.");

    String *clone = String__new__();
    check_mem(clone);

    String_init(clone, self->text, self->length);
    return clone;

error:
    exit(EXIT_FAILURE);
}

String *String_assign(String **l_str, const String *r_str) {

    check(r_str != NULL,
          "in String_assign(). Invalid String reference 'r_str'.");

    if (*l_str != NULL) {
        String_free((*l_str));
    }
    *l_str = String__new__();
    check_mem(*l_str);

    String_init(*l_str, r_str->text, r_str->length);
    return *l_str;

error:
    exit(EXIT_FAILURE);
}

// Destructor

void String_destroy(String **self) {

    if (*self != NULL) {

        if ((*self)->text != NULL) {
            String_clear((*self));
        }
        free(*self);
        *self = NULL;
    }
}

String *String_cpy(String *dest, const String *src) {

    check(src, "In String_cpy(dest, src). Invalid String reference 'src'");
    check(dest, "In String_cpy(src, dest). Invalid String reference 'dest'");

    if (src == dest)
        return dest;

    if (src->length == 0)
        return dest;

    if (src->length > dest->capacity) {
        String_clear(dest);
        dest->text = String_carray(src);
        dest->capacity = src->length;
    } else {
        for (int32_t i = 0; i <= src->end; i++) {
            dest->text[i] = src->text[i];
        }
    }
    dest->length = src->length;
    dest->end = src->end;
    return dest;

error:
    exit(EXIT_FAILURE);
}

bool String_isempty(const String *const self) {
    check(self != NULL, "In String_isempty(). Invalid String reference.");
    return self->length == 0 || self->text == NULL;

error:
    exit(EXIT_FAILURE);
}

int32_t String_len(const String *self) {
    check(self != NULL, "In String_len(). Invalid String reference.");
    return self->length;

error:
    exit(EXIT_FAILURE);
}

char String_getc(const String *self, int32_t pos) {

    check(self != NULL, "In String_getch(). Invalid String reference.");
    check(self->length > 0 && pos > -1 && pos < self->length,
          "In String_getch(). Index out of bound '%d'.", pos);

    return self->text[pos];

error:
    exit(EXIT_FAILURE);
}

String *String_ensurecap(String *self, int32_t cap) {

    check(self != NULL,
          "In String_ensurecap(). Invalid String reference'self'.");
    check(cap > 0,
          "In String_ensurecap(). Invalid arguement: negative capacity '%d'.",
          cap);

    if (self->capacity < cap) {
        char *p = realloc(self->text, cap);

        check(p != NULL,
              "In String_ensurecap(). Out Of memory.\n Cannot allocate the "
              "requested amount of space(%d) for the string buffer",
              cap);

        self->text = p;
    }
    return self;

error:
    exit(EXIT_FAILURE);
}

String *String_cat(String *self, const String *suffix) {

    check(self != NULL, "In String_cat(). Invalid String reference'self'.");
    check(suffix != NULL,
          "In String_cat(). Invalid String reference 'suffix'.");

    /* if the suffix not empty do  th following: */
    if (suffix->length > 0) {

        int32_t tot_len = self->length + suffix->length;
        int32_t new_cap = self->capacity;
        if (tot_len > self->capacity) {
            new_cap = tot_len * 2;
            char *p = realloc(self->text, new_cap);
            check_mem(p);
            self->text = p;
        }
        memmove(self->text + self->length, suffix->text, suffix->length);

        self->capacity = new_cap;
        self->length = tot_len;
        self->end = self->length - 1;
    }
    return self;

error:
    exit(EXIT_FAILURE);
}

String *String_catcstr(String *self, const char *suffix) {

    check(self != NULL,
          "In String_catcstr(). Invalid String reference 'self'.");
    check(self != NULL,
          "In String_catcstr(). Invalid cstring reference 'suffix'.");

    int32_t suf_len = strlen(suffix);
    if (suf_len > 0) {

        int32_t new_cap = self->capacity;
        int32_t tot_len = self->length + suf_len;

        if (tot_len > self->capacity) {

            new_cap = tot_len * 2;
            char *p = realloc(self->text, new_cap);
            check_mem(p);
            self->text = p;
        }
        memmove(self->text + self->length, suffix, suf_len);

        self->length = tot_len;
        self->end = tot_len - 1;
        self->capacity = new_cap;
    }

    return self;

error:
    exit(EXIT_FAILURE);
}

bool String_equals_ignore_case(const String *self, const String *other) {

    check(self != NULL, "In String_eqig_() {aka String_equals_ignore_case()}. "
                        "Invalid String reference 'self'.");
    check(other != NULL, "In String_eqig_() {aka String_equals_ignore_case()}. "
                         "Invalid String reference 'other'.");

    if (self == other) {
        return true;
    }
    if (self->length != other->length) {
        return false;
    }

    for (int32_t i = 0; i < self->length; i++) {
        if (tolower(self->text[i]) != tolower(self->text[i])) {
            return false;
        }
    }

    return true;

error:
    exit(EXIT_FAILURE);
}

bool String_equals(const String *self, const String *other) {

    check(self != NULL, "In String_equals(). Invalid String reference 'self'.");
    check(other != NULL,
          "In String_equals(). Invalid String reference 'other'.");

    if (self == other) {
        return true;
    }
    if (self->length != other->length) {
        return false;
    }

    for (int32_t i = 0; i < self->length; i++) {
        if (self->text[i] != other->text[i]) {
            return false;
        }
    }

    return true;

error:
    exit(EXIT_FAILURE);
}

String *String_trim(String *self) {

    check(self != NULL, "In String_trim(). Invalid String reference.");

    if (self->length < 0) {
        return Str_new();
    }

    char *first = self->text;
    char *last = first + self->end;
    int32_t cur_len = self->length;

    while (first < last && isspace(*first)) {
        first++;
        cur_len--;
    }

    while (last >= first && isspace(last)) {
        last--;
        cur_len--;
    }
    if (cur_len) {
        memmove(self->text, first, cur_len);
    }

    self->length = cur_len;
    self->end = self->length - 1;
    return String_dup(self);

    /* int32_t first = 0; */
    /* int32_t last = self->end; */
    /* int32_t new_len = self->length; */

    /* while (first < last && isspace(self->text[first])) { */
    /*     first++; */
    /*     new_len--; */
    /* } */
    /* int ptr = new_len; */

    /* while (last >= first && isspace(self->text[last])) { */
    /*     last--; */
    /*     new_len--; */
    /* } */
    /* if (new_len) { */
    /*     memmove(self->text, self->text + ptr, new_len); */
    /* } */

    /* self->length = new_len; */
    /* self->end = self->length - 1; */
    /* return self; */

error:
    exit(EXIT_FAILURE);
}

bool String_equals_ignore_case_cstr(const String *self, const char *c_str,
                                    int32_t cstr_len) {

    check(self != NULL,
          "in String_eqig() aka {String_equals_ignore_case_cstr()}. Invalid "
          "String reference.");
    check(c_str != NULL,
          "in String_eqig_cstr() aka {String_equals_ignore_case_cstr()}. "
          "Invalid cstring reference.");
    check(cstr_len >= 0,
          "in String_eqig_cstr() aka {String_equals_ignore_case_cstr()}. "
          "Illegal argument cstr_len '%d'.",
          cstr_len);

    if (self->text == c_str) {
        return true;
    }
    if (self->length != cstr_len) {
        return false;
    }

    for (int32_t i = 0; i < self->length; i++) {
        if (tolower(self->text[i]) != tolower(c_str[i])) {
            return false;
        }
    }

    return true;

error:
    exit(EXIT_FAILURE);
}

bool String_equals_cstr(const String *self, const char *c_str,
                        int32_t cstr_len) {

    check(self != NULL, "in String_eq_cstr() aka {String_equals_cstr()}. "
                        "Invalid String reference.");
    check(c_str != NULL, "in String_eq_cstr() aka {String_equals_cstr()}. "
                         "Invalid cstring reference.");
    check(cstr_len >= 0,
          "in String_eq_cstr() aka {String_equals_cstr()}. Illegal argument "
          "cstr_len '%d'.",
          cstr_len);

    if (self->text == c_str) {
        return true;
    }
    if (self->length != cstr_len) {
        return false;
    }

    for (int32_t i = 0; i < self->length; i++) {
        if (self->text[i] != c_str[i]) {
            return false;
        }
    }

    return true;

error:
    exit(EXIT_FAILURE);
}

void String_print(String *self) {
    check(self != NULL, " in String_print(). Invalid String reference.");

    if (self->length > 0) {
        if (self->length > 1023) {
            fast_print(self);
        } else {
            String_catcstr(self, "\n");
            printf("\"%s\"\n", self->text);
        }
    } else {
        printf("\"\"\n");
    }
    return;

error:
    exit(EXIT_FAILURE);
}

String *String_slice(const String *self, int32_t start, int32_t count) {

    check(self != NULL, " In String_slice(). Invalid String reference.");
    check(count > -1, " In String_slice(). Invalid arguement 'count':%d.",
          count);

    if (start > self->end) {
        return Str_new();
    }
    start = start < 0 ? 0 : start;
    if (count + start > self->end) {
        return String_dup(self);
    }

    String *slice = String__new__();
    slice->text = malloc(count < INIT_CAPACITY ? INIT_CAPACITY : count);
    check_mem(self->text);

    memmove(slice->text, self->text + start, count);
    return slice;

error:
    exit(EXIT_FAILURE);
}

char *String_carray(const String *self) {

    check(self != NULL, "in String_charray(). Invalid String refence.");

    if (self->length == 0)
        return strdup("");

    char *array = malloc(self->length);
    check_mem(array);
    memmove(array, self->text, self->length);

    return array;

error:
    exit(EXIT_FAILURE);
}

const char *String_data(const String *self) {

    check(self != NULL, "in String_data(). Invalid String refence.");
    check(self->text != NULL, "in String_data(). No buffer allocated.");

    return self->text;

error:
    exit(EXIT_FAILURE);
}

char *String_cstr(const String *self) {

    check(self != NULL, "in String_cstr(). Invalid String refence.");

    if (self->length == 0)
        return strdup("");

    char *c_string = malloc(self->length + 1);
    check_mem(c_string);

    if (self->length > 0) {
        memmove(self->text, c_string, self->length);
    }
    c_string[self->length] = '\0';

    return c_string;

error:
    exit(EXIT_FAILURE);
}

// Private helper functions

static String *String__new__() {

    String *self = malloc(sizeof(String));
    check_mem(self);
    return self;

error:
    exit(EXIT_FAILURE);
}

static void String_init(String *self, const char *c_string, int32_t cstr_len) {

    self->capacity = INIT_CAPACITY > cstr_len ? INIT_CAPACITY : cstr_len * 2;

    self->text = calloc(self->capacity, 1);
    check_mem(self->text);

    memcpy(self->text, c_string, cstr_len);

    self->length = cstr_len;
    self->end = cstr_len - 1;
    return;

error:
    exit(EXIT_FAILURE);
}

static void String_clear(String *self) {

    free(self->text);
    self->text = NULL;
    self->capacity = 0;
    self->length = 0;
    self->end = -1;
}

static void fast_print(const String *self) {

    int64_t left = String_len(self);
    printf("\"");

    if (self->length <= 4096) {
        self->length % 2
            ? fwrite(self->text, sizeof(char), self->length, stdout)
            : fwrite(self->text, self->length * sizeof(char), sizeof(char),
                     stdout);
    } else {

        int64_t writen_chars = 0;
        do {
            writen_chars = fwrite(self->text, sizeof(char), 4096, stdout);
            left -= 1024;
        } while (left > 0 && writen_chars > 0);

        if (llabs(left) > 0) {
            fwrite(self->text, sizeof(char), left, stdout);
        }
    }
    printf("\"");
}

struct {
    const enum SortOrder REVERSED;
    const enum SortOrder NORMAL;
} order_t = {.REVERSED = REV, .NORMAL = NOR};

/* static void StrArry_init() { */

/* } */

/* static StrArray * StrArray__new__()  { */
/*     StrArray * arraylist = (StrArray *) malloc(sizeof(StrArray *)); */
/*     check_mem(arraylist); */
/*     return arraylist; */

/* error: */
/*     exit(EXIT_FAILURE); */
/* } */

/* static void StrArray_init(StrArray * arraylist, int32_t init_cap)  { */

/*     for (int32_t i = 0; i < init_cap; ++i) { */
/*         arraylist->elements[i] = (struct _string_t) { */
/*         // arraylist->elements[i] = (String) { */
/*             .text=NULL, */
/*             .length=0, */
/*             .end=-1, */
/*             .capacity=INIT_CAPACITY */
/*         }; */
/*         char * p =  calloc(INIT_CAPACITY, 1); */
/*         check_mem(p); */

/*         (arraylist->elements[i]).text = p; */
/*     } */

/*     arraylist->cap = init_cap; */
/*     arraylist->len = 0; */

/* error: */
/*     exit(EXIT_FAILURE); */
/* } */

/* StrArray * StrArray_new(int32_t init_cap)  { */

/*     StrArray * arraylist =  StrArray__new__(); */

/*     init_cap = init_cap < 16 ? 16 : init_cap; */
/*     arraylist->elements = (String *) malloc(init_cap * sizeof(struct
 * _string_t)); */

/*     StrArray_init(arraylist, init_cap); */

/*     return arraylist; */
/* } */

/* void StrArray_set(StrArray * arraylist, int32_t pos, const String * str) { */

/*     check(arraylist != NULL, " in StrArray_set(). Invalid StrArray
 * reference."); */
/*     check(pos >= 0 && pos < arraylist->cap, " in StrArray_set(). Index ouf of
 * bounds %d", pos); */

/*     String * ptr_str = (arraylist->elements + pos); */

/*     if (!str) { */
/*         if (ptr_str) { */
/*             String_free(ptr_str); */
/*             &(arraylist->elements[pos]) = NULL; */
/*         } */

/*     } */

/*     if (str != NULL && (pos + 1) < arraylist->len) { */

/*         char * p = NULL; */
/*         if (ptr_str->capacity < str->length) { */

/*             p =  malloc(str->length * sizeof(char)); */
/*             check_mem(p); */
/*             if (ptr_str->length > 0) { */
/*                 String_clear(ptr_str); */
/*             } */
/*             memmove(p, str->text, str->length); */
/*             ptr_str->text   = p; */
/*             ptr_str->length = str->length; */
/*             ptr_str->end    = str->length - 1; */
/*         } */
/*     } */

/*     return; */

/* error: */
/*     exit(EXIT_FAILURE); */
/* } */

/* char * StrArray_getcstr(StrArray * arraylist, int32_t pos) { */

/*     check(arraylist != NULL, " in StrArray_getcstr(). Invalid StrArray
 * reference."); */
/*     check(pos >= 0 && pos < arraylist->cap, " in StrArray_getcstr(). Index
 * ouf of bounds %d", pos); */

/*     return (arraylist->elements[pos]).text; */

/* error: */
/*     exit(EXIT_FAILURE); */
/* } */

/* /1* StrArray_at(stud_namesw)[]; *1/ */

/* String * StrArray_at(StrArray * arraylist, int32_t pos) { */

/*     check(arraylist != NULL, " in StrArray_get(). Invalid StrArray
 * reference."); */
/*     check(pos >= 0 && pos < arraylist->cap, " in StrArray_get(). Index ouf of
 * bounds %d", pos); */

/*     return &(arraylist->elements[pos]); */

/* error: */
/*     exit(EXIT_FAILURE); */
/* } */

/* String * StrArray_get(StrArray * arraylist, int32_t pos) { */

/*     check(arraylist != NULL, " in StrArray_get(). Invalid StrArray
 * reference."); */
/*     check(pos >= 0 && pos < arraylist->cap, " in StrArray_get(). Index ouf of
 * bounds %d", pos); */

/*     return String_dup( &(arraylist->elements[pos])); */

/* error: */
/*     exit(EXIT_FAILURE); */
/* } */

/* void StrArray_add(StrArray * array, const String * str) { */

/*     check(arraylist != NULL, " in StrArray_get(). Invalid StrArray
 * reference."); */
/*     check(str != NULL, " in StrArray_get(). Invalid String reference."); */

/*     return String_dup( &(arraylist->elements[pos])); */

/* error: */
/*     exit(EXIT_FAILURE); */
/* } */

/* String * StrArray_pop(StrArray * array) { */

/* } */

/* void StrArray_push(StrArray * array, const String * str); */

/* void StrArray_sort(StrArray * array, SortOrder order); */

/* int32_t StrArray_find(int32_t init_cap); */

typedef enum TokenType {
    CHAR_TOKEN,
    STR_TOKEN,
    INT_TOKEN,
    FLOAT_TOKEN
} TokenType;

typedef void *Data;

typedef struct Token {

    struct Token *next;
    union {

        char char_data;
        int64_t int_data;
        double float_data;
        String *string_data;

    } value;

    TokenType token_type;
    int32_t length;

} Token;

typedef struct TokenList {
    Token *head;
    int32_t count;
} TokenList;

// static Token * new_token(Data *value, int32_t len, TokenType token_type,
// int32_t len) {

//     Token *token = (Token *) malloc(sizeof(Token));
//     assert(token != NULL);

//     token->token_type = token_type;
//     token->length = len;
//     token->next = NULL;

//     switch (token_type) {

//         case CHAR_TOKEN: token->value.char_data   = *( value);
//             break;

//         case INT_TOKEN: token->value.int_data     = *((int64_t *) value);
//             break;

//         case FLOAT_TOKEN: token->value.float_data = *((double *) value);
//             break;

//         case STR_TOKEN: token->value.string_data  = (String *) value;
//             break;
//     }

//     return token;
// }

// static void del_token(Token **token) {

//     if ((*token)->token_type == STR_TOKEN) {
//         free((*token)->value.string_data);
//     }
//     free(*token);
//     *token = NULL;
// }

// static TokenList *token_list =  malloc(sizeof(TokenList));

/* static int32_t readln_cnt = 0; */

// static * TokenList_destroy() {

//     if (token_list != NULL) {

//         while (token_list->count > 0) {
//             del_token(TokenList_pop());
//             token_list->count--;
//         }
//         free(token_list);
//     }
// }

// static Token * TokenList_pop() {

//     Token * temp = token_list;
//     token_list->head = token_list->head->next;
//     token_list->count--;

//     return temp;
// }

// static void TokenList_push_back(Token *token) {

//     assert(token != NULL);

//     Token *current = token_list->head;
//     if (current != NULL) {
//         token_list->head = token;
//     } else {
//         while (current->next != NULL) {
//             current = current->next;
//         }
//         current->next = token;
//     }
//     token_list->count++;
// }
//
//,,w
//,,
#define _STRING_BUF_SZ 100

char _STRING_BUF_[_STRING_BUF_SZ] = {'\0'};
char _BUFF_TRASH_[_STRING_BUF_SZ] = {'\0'};

String *String_readln(const char *prompt) {

    assert(prompt != NULL);

    if (strlen(prompt) > 0) {
        printf("%s\n", prompt);
    }

    String *line = string("");
    String *input = NULL;
    int32_t end = 0;

    char *no_error =
        fgets(_STRING_BUF_, (_STRING_BUF_SZ - 1) * sizeof(char), stdin);

    while (no_error) {

        input = string(_STRING_BUF_);
        String_cat(line, input);

        end = String_len(line) - 1;
        if ('\n' == String_getc(line, end)) {
            String_free(input);
            return String_slice(line, 0, end);
        }
        String_free(input);

        no_error =
            fgets(_STRING_BUF_, (_STRING_BUF_SZ - 1) * sizeof(char), stdin);
    }

    fprintf(stderr, "\nERROR!!!\nCANNOT READ INPUT\n\n");
    return NULL;
}

// String * next() {

//     int32_t first_char = '\0';
//     String *line = NULL;
//     if (readln_cnt > 0) {
//         line = String(bufr, (buffer));
//     }else {
//         line = readline("");
//     }

//     int32_t nput_char = String_char_at(line, 0);
//     first_char = input_char;
//     for (int32_t i = 0; i < String_len(line); i++) {
//         if (isdigit(first_char)) {
//              TokenList_push_back(parse_number(line, i));
//         }else {
//              TokenList_push_back(parse_text(line, i));
//         }
//     }
// }

// Token * parse_num(String * line, int32_t start_pos) {

//     int32_t end_pos = start_post;

//     while(isdigit(String_get_char(end_pos)) ) {
//         end_pos++;
//     }

//    int32_t ch = String_get_char(end_pos + 1);
//     if (ch  == '.') {
//         while(isdigit(String_get_char(end_pos)) ) {
//             end_pos++;
//         }
//     }
// }
