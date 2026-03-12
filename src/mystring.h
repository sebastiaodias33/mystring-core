
#ifndef MYSTRING_H_
#define MYSTRING_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct _string_t String;

#define Str(VAR) String *(VAR) = NULL

extern const struct _string_t *String_GVAR_root;

// Constructors

String *Str_new();

String *String_create(const char *c_string, int32_t cstr_len);

#define string(CSTR) String_create((CSTR), strlen((CSTR)))

String *String_dup(const String *self);

String *String_assign(String **l_str, const String *r_str);

// destructors

void String_destroy(String **self);

#define String_free(S)                                                         \
    if ((S)) {                                                                 \
        String_destroy(&(S));                                                  \
        (S) = NULL;                                                            \
    }

// Query operations

int32_t String_len(const String *self);

bool String_isempty(const String *self);

int32_t String_find(const String *self);

int32_t String_findc(char c);

bool String_equals(const String *self, const String *other);

#define String_eq(A, B) String_equals((A), (B))

bool String_equals_cstr(const String *self, const char *other,
                        int32_t cstr_len);

#define String_eq_cstr(SELF, CSTR)                                             \
    String_equals_cstr((SELF), (CSTR), strlen(CSTR))

bool String_equals_ignore_case(const String *self, const String *other);

#define String_eqi(A, B) String_equals_ignore_case((A), (B))

bool String_equals_ignore_case_cstr(const String *self, const char *other,
                                    int32_t cstr_len);

#define String_eqi_cstr(SELF, CSTR)                                            \
    String_equals_ignore_case_cstr((SELF), (CSTR), strlen(CSTR))

// returns a copy of the underline buffer
char *String_cstr(const String *self);

// returns pointer to the first char of the the underline buffer
// this pointer should not modify the de buffer
const char *String_data(const String *self);

char String_getc(const String *self, int32_t pos);

void String_print(String *self);

//  Mutatable operations &

String *String_ensurecap(String *self, int32_t cap);

String *String_popc(String *self);

String *String_delat(String *self, int32_t pos);

String *String_trim(String *self);

String *String_slice(const String *self, int32_t start, int32_t count);

String *String_cpy(String *dest, const String *src);
// String * String_cpy(String * dest, const String * src);

String *String_cpy_carray(String *self, const char *carray, int32_t len);

#define String_cpycstr(STR, CSTR)                                              \
    String_cpy_carray((STR), (CSTR), strlen(CSTR));

String *String_cat(String *self, const String *suffix);

String *String_catcstr(String *self, const char *suffix);

String *String_push(String *self, String *preffix);

String *String_pushc(String *self, char c);

String *String_insert(String *self, int32_t pos, String *str);

String *String_insertc(String *self, int32_t pos, char ch);

String *String_rem(String *self, String *slice);

String *String_remc(String *self, char c);

// Array os String for quick str operations

typedef struct _strarray_t {
    int32_t len;
    int32_t cap;
    String *elements;
} StrArray;

typedef enum SortOrder { REV = -1, NOR = 1 } SortOrder;

StrArray *StrArray_new(int32_t init_cap);

void StrArray_set(StrArray *array, int32_t pos, const String *str);

#define StrArray_setcstr(array, pos, cstr)                                     \
    StrArray_set((array), (pos), string((str)))

String *StrArray_get(StrArray *array, int32_t pos);

void StrArray_add(StrArray *array, const String *str);

#define StrArray_addcstr(array, str)                                           \
    StrArray_addstr(StrArray *array, string((str)))

String *StrArray_pop(StrArray *array);

void StrArray_push(StrArray *array, const String *str);

#define StrArray_pushcstr(array, str) StrArray_pushstr((array), string((str)))

void StrArray_sort(StrArray *array, SortOrder order);

int32_t StrArray_find(int32_t init_cap);

// StrArray * StrArray_fromlist(const char *...);
// StrArray * StrArray_fromlist(const char *...);

typedef struct _string_list StringList;

// StrArray * String_array(const String * self, const char separator);

StrArray *String_split(const String *self, const char *separator);
// StringList * String_split(const String * self, const char separator);

char *String_carray(const String *self);

// IO functions

String *String_readln(const char *prompt_text);

#endif
