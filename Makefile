CC					= gcc
LINK					= gcc

CC_FLAGS				= -c -g -Id:\usr\local\include
LINK_FLAGS				= -g -LD:\usr\local\lib
LIB_FLAGS				= 

LIBS					= -lutils

TARGET1					= jsonschema.exe
OBJS1					= $(sort				\
					    jsonschema.o                        \
					    JSONInfo.o				\
					   )

TARGET2					= jsonparse.exe
OBJS2					= $(sort				\
					    jsonparse.o                         \
					   )

TARGETS					= $(TARGET1) $(TARGET2)

%.o					: %.c
					  @echo [C+] $@
					  @$(CC) $(CC_FLAGS) $<

include					  depends.mk

.PHONY					: all
all					: $(TARGETS)

$(TARGET1)				: $(OBJS1)
					  @echo [LD] $@
					  @$(LINK) $(LINK_FLAGS) $(LIB_FLAGS) -o $(TARGET1) $(OBJS1) $(LIBS)

$(TARGET2)				: $(OBJS2)
					  @echo [LD] $@
					  @$(LINK) $(LINK_FLAGS) $(LIB_FLAGS) -o $(TARGET2) $(OBJS2) $(LIBS)

jsonparse.o				: jsonparse.c

.PHONY					: junkclean
junkclean				:
					  rm -rf $(wildcard *~ *.bak)

.PHONY					: clean
clean					: junkclean
					  rm -rf $(wildcard $(TARGET) $(OBJS))
