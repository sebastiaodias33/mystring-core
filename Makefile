CC 	   =cc
CFLAGS =-g -Og -Wall -Wextra -I$(INC_DIR) -pedantic-errors -Werror -Werror=overflow -Warray-bounds=1 -Wno-error=switch -Wimplicit-function-declaration $(OPTFLAGS)

# LIBS=-ldl-lm $(OPTLIBS) # -ldl -> to load the dl*funcs(dlopen, etc)

LIBRARY  :=mystring
TARGET   =$(LIB_DIR)/lib$(LIBRARY).a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

LIB_DIR:=lib

LINK_NAME =$(strip $(LIB_DIR)/, $(SO_TARGET))
SO_NAME   =$(LINK_NAME).1
LIB_NAME  =$(LINK_NAME).1.0

SRC_DIR  :=src
OBJ_DIR  :=obj
INC_DIR  :=include
TEST_DIR :=tests

HEADERS=$(wildcard $(INC_DIR)/*.h)
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TEST_SRC:=$(wildcard tests/*_tests.c)
TEST_BINS:=$(patsubst tests/%.c, tests/bin/%,$(TEST_SRC))

PREFIX     ?=/usr/local
ROOT_DIR   ?=mystrlib
DEST_DIR   ?=sumbolibs/$(ROOT_DIR)
LIB_PATH    =$(PREFIX)/lib
INST_DIR    =$(LIB_PATH)/$(DEST_DIR)
INCLUDE_DIR =$(PREFIX)/include


all: $(TARGET) $(SO_TARGET) 

rebuild: clean clear_my_screen all

release: CFLAGS=-g -O2 -Wall -Wextra -I$(INC_DIR) -rdynamic -DNDEBUG -pedantic-errors -Werror -Werror=overflow -Warray-bounds=1 -Wno-error=switch -Wimplicit-function-declaration $(OPTFLAGS)
release: clean
release: all


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -fPIC $< -o $@ 


$(TARGET): $(OBJS) | $(LIB_DIR)
	ar rcs $@ $^
# when your can use '$^', use $(sort words) to remove duplicates


$(SO_TARGET): $(OBJS) | $(LIB_DIR)
	$(CC) -fPIC -shared $^ -o $@ -lc


$(OBJ_DIR):
	@mkdir -p $@ 
	
 
$(LIB_DIR):
	@mkdir -p $@ 

$(TEST_DIR)/bin:
	@mkdir -p $@ 


# The Unit Tests
.PHONY: all tests rebuild clean

$(TEST_DIR)/bin/%: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $< $(OBJS) $(LIBS) -lcriterion -o $@ 

	
tests: clean-tests $(TEST_DIR)/bin $(TEST_BINS) 
	for test in $(TEST_BINS) ; do ./$$test ; done


clean-tests: clear_my_screen
	@$(RM) $(TEST_BINS)
	@$(RM) -d tests/bin 


clean:
	@$(RM) $(TARGET) $(SO_TARGET) $(OBJS) $(TEST_BINS)
	@$(RM) -d $(LIB_DIR) $(OBJ_DIR)
	@$(RM) -f tests/tests.log


clear_my_screen:
	@clear

install-headers:
	@install -d $(INCLUDE_DIR)/$(DEST_DIR)
	@install $(HEADERS) $(INCLUDE_DIR)/$(DEST_DIR)


install: all install-headers
	@install -d $(INST_DIR) 
	@install $(TARGET) $(INST_DIR)
	@install $(SO_TARGET) $(INST_DIR)
	@mv $(INST_DIR)/$(LINK_NAME) $(INST_DIR)/$(LIB_NAME)

	@ln -s -f $(INST_DIR)/$(LIB_NAME)  $(LIB_PATH)/$(SO_NAME)  
	@ln -s -f $(LIBPATH)/$(SO_NAME)  $(LIB_PATH)/$(LINK_NAME)  

	@ldconfig -n $(LIB_PATH)
	@ldconfig -n $(INST_DIR)


uninstall:
	@rm -I $(INCLUDE_DIR)/$(DEST_DIR)/*.h
	@rm -i $(INST_DIR)/$(LIB_NAME)  
	@rm -i $(INST_DIR)/$(LINK_NAME:.so=.a)  
	@echo "\nremoving the the symbolic links"
	@rm -i $(LIB_PATH)/$(SO_NAME)
	@rm -i $(LIB_PATH)/$(LINK_NAME)  

# Security audit tool - Adapted from Zed Shaw's 'Learn C The Hard Way'
# The Checker
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SRCS) || true
