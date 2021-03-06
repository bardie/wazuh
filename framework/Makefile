# Makefile for Wazuh Framework
# Copyright 2017 Wazuh Inc.
# May 3, 2017
#
# Syntax: make [ all | build | install | examples | clean ]

uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

OSSEC_GROUP  = ossec
PREFIX       = /var/ossec

CC           = gcc
CFLAGS       = -pipe -O2
THREAD_FLAGS = -pthread -Wall
LFLAGS       = -O2
RM_FILE      = rm -f
INSTALL_DIR  = install -o root -g ${OSSEC_GROUP} -m 0750  -d
INSTALL_EXEC = install -o root -g ${OSSEC_GROUP} -m 0750
INSTALL_FILE = install -o root -g ${OSSEC_GROUP} -m 0640

SQLITE_DIR       = ../src/external/sqlite
HEADERS_DIR      = ../src/headers

DEBUG_OP         = ../src/shared/debug_op.o
JSON_LIB         = ../src/external/cJSON/cJSON.o
XML_LIB          = ../src/os_xml/os_xml.o
XML_ACCESS       = ../src/os_xml/os_xml_access.o
STRBREAK         = ../src/os_regex/os_regex_strbreak.o
REGEX_OP         = ../src/shared/regex_op.o
STRING_OP        = ../src/shared/string_op.o
FILE_OP          = ../src/shared/file_op.o
PRIVSEP_OP       = ../src/shared/privsep_op.o
HELP_O           = ../src/shared/help.o
ERROR_MSGS       = ../src/error_messages
DEBUG_OP_DEPS    = $(DEBUG_OP) $(JSON_LIB) $(XML_LIB) $(XML_ACCESS) $(STRBREAK) $(STRING_OP) $(FILE_OP) $(PRIVSEP_OP) $(REGEX_OP) $(HELP_O)

ifeq (${uname_S},Linux)
BUILD_TARGET           = wazuh-clusterd-internal libsqlite3.so.0
endif

.PHONY: all build install examples clean

all: build

install:
	$(INSTALL_DIR) $(PREFIX)/framework
	$(INSTALL_DIR) $(PREFIX)/framework/wazuh
	$(INSTALL_DIR) $(PREFIX)/framework/lib

	$(INSTALL_EXEC) wazuh/*.py ${PREFIX}/framework/wazuh
	$(INSTALL_FILE) libsqlite3.so.0 ${PREFIX}/framework/lib/
ifeq (${uname_S},Linux)
	$(INSTALL_EXEC) wazuh-clusterd-internal ${PREFIX}/bin
endif

#	Install scripts/%.py on $(PREFIX)/bin/%
	$(foreach script,$(wildcard scripts/*),$(INSTALL_EXEC) $(script) $(patsubst scripts/%.py,$(PREFIX)/bin/%,$(script));)

examples: install
	$(INSTALL_DIR) $(PREFIX)/framework/examples
	$(INSTALL_EXEC) examples/*.py ${PREFIX}/framework/examples

build: $(BUILD_TARGET)

libsqlite3.so.0: $(SQLITE_DIR)/sqlite3.o
	$(CC) $(LFLAGS) -shared -o $@ $^

ifeq (${uname_S},Linux)
wazuh-clusterd-internal: wazuh-clusterd-internal.o $(SQLITE_DIR)/sqlite3.o $(DEBUG_OP_DEPS)
	$(CC) $(THREAD_FLAGS) -o $@ $^ -ldl
endif

ifeq (${uname_S},Linux)
wazuh-clusterd-internal.o: wazuh/wazuh-clusterd-internal.c
	$(CC) $(CFLAGS) $(THREAD_FLAGS) -c $^ -o $@ -I $(SQLITE_DIR) -I $(HEADERS_DIR) -I $(ERROR_MSGS) -ldl
endif

clean:
	$(RM_FILE) $(BUILD_TARGET) *.o *.so.0
