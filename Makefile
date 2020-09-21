#Get directory path
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
THIS_DIR := $(dir $(mkfile_path))

#Includes
LOCAL_INC := -I$(THIS_DIR)inc/

#Dirs and files lists
SRC_DIR := $(THIS_DIR)src
OBJ_DIR := $(THIS_DIR)obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

#Compiler options
CC=gcc
CFLAGS=-c -Wall -Ofast -std=c99
LFLAGS=-Wall -fuse-ld=gold -std=c99 -static


$(OBJ_DIR)/iPod_DB_Builder: $(OBJ_FILES)
	$(CC) $(LFLAGS) -o $@ $^

#Compile Application Source
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(LOCAL_INC) -o $@ $<

clean:
	rm $(OBJ_DIR)/*
