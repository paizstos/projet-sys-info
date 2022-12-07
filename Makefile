include help.mk
CC=gcc
CFLAGS=-Wall -Werror

## ---------------------------------------------------------------------------------------------------------
## How to use our make file
##
## 1. Open a new terminal at the root of our project folder.
## 2. Use 'make all' to compile all the source files from the /dst folder.
## 3. Use 'make experiments' to benchmark our compiled files and plot their results.
##
## You can then find the resulting plots in the /plots folder and the benchmarks in the /benchmarks folder
##
## ---------------------------------------------------------------------------------------------------------

all: dst/Philosophe dst/Prod-Cons dst/Readers-Writers dst/lock_test # Compile all the .c files present in folder /src

dst/%: src/%.c
	if [ ! -d dst ]; then mkdir dst; fi
	$(CC) $(CFLAGS) -o $@ $^ -lpthread
 

.PHONY: clean all experiments
clean: # Delete all compiled files with the /dst folder and the content of folders benchmarks and plots
	rm -rf dst
	rm -f benchmarks/*
	rm -f plots/*

experiments: dst/* # Run the benchmarks on compiled files and plots the results using python
	./experiments.sh $^