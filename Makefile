# ---------------------------------------------------------------

DISTRIB := .
#"$(pwd)/../distrib"

C_SRCS := \
 config/AppConfigControl.cpp \
 control/CommandControl.cpp \
 control/consoleinterface.cpp \
 control/About.cpp \
 control/DefaultSchema.cpp \
 control/History.cpp \
 control/Log.cpp \
 db/db_table_class_list.cpp \
 db/selector.cpp \
 db/sqlexec.cpp \
 gui/OutputForm.cpp \
 thread/log_thread.cpp \
 utils/apputils.cpp \
 pg_sql.cpp


OBJS := \
 $(DISTRIB)/config/AppConfigControl.o \
 $(DISTRIB)/control/CommandControl.o \
 $(DISTRIB)/control/consoleinterface.o \
 $(DISTRIB)/control/About.o \
 $(DISTRIB)/control/DefaultSchema.o \
 $(DISTRIB)/control/History.o \
 $(DISTRIB)/control/Log.o \
 $(DISTRIB)/db/db_table_class_list.o \
 $(DISTRIB)/db/selector.o \
 $(DISTRIB)/db/sqlexec.o \
 $(DISTRIB)/gui/OutputForm.o \
 $(DISTRIB)/thread/log_thread.o \
 $(DISTRIB)/utils/apputils.o \
 $(DISTRIB)/pg_sql.o


EXECUTABLE := pg_sql

LIBS := -lstdc++fs -lcpprest -lpqxx -lpthread


.PHONY: prepare build all clean 

all: build test

prepare:
	sudo apt update
	sudo apt install libcpprest-dev libpqxx-dev

build: $(OBJS) 
	@echo 'Building target: $@'
	g++ -o $(EXECUTABLE) $(OBJS) $(LIBS)

install:
	mkdir -p /opt/pg_client
	sudo cp $(EXECUTABLE) /opt/pg_client
	sudo cp config.json /opt/pg_client

%.o: %.cpp
	@echo 'Building file: $<'
	g++ -std=c++1z -O3 -c -o "$@" "$<"

clean:
	rm -rf $(OBJS) $(EXECUTABLE)

test:
	./$(EXECUTABLE)

