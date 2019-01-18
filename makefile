define genrule
prefix := $$(subst src,obj,$1/)
$$(filter $$(prefix)%.o,$$(OBJ)):\
$$(prefix)%.o: $1/%.c $$(prefix)%.d
endef

DEPFLAGS = -MT $@ -MMD -MP -MF $(@:.o=.Td)
CFLAGS = -o $@ -c $< -std=c89 -Wall -Wextra -pedantic -Wno-pointer-sign -g
DEFINES = -DMEM_DEBUG -JSMN_PARENT_LINKS

INC := -Iinclude -include stdbool.h
SRC := $(wildcard $(foreach d,src,$d/*.c $d/*/*.c))
OBJ := $(subst src/,obj/,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d)
BIN = dfpatch

all: $(BIN)

FORCE:

$(BIN): $(OBJ)
	@echo Building $@.exe...
	@gcc -o $@ $(OBJ)


$(foreach d,src,$(eval $(call genrule,$d)))
$(OBJ):
	@mkdir -p $(@D)
	@echo Building $(notdir $@)...
	@gcc $(DEPFLAGS) $(CFLAGS) $(DEFINES) $(INC)
	@mv -f $(@:.o=.Td) $(@:.o=.d) && touch $@

obj/%.d: ;
.PRECIOUS: obj/%.d

.PHONY: clean
clean:
	@rm -f $(BIN)
	@rm -f $(OBJ)
	@rm -f $(DEP)

include $(DEP)