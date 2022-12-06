CC=gcc
CFLAGS=-Wall -Werror

all: dst/Philosophe dst/Prod-Cons dst/Readers-Writers
final : benchmark plots

dst/%: src/%.c
	if [ ! -d dst ]; then mkdir dst; fi
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

.PHONY: clean all benchmark plots
clean:
	rm -rf dst
	rm -f benchmarks/*
	rm -f plots/*

benchmark: dst/*
	./benchmark.sh $^

plots: benchmarks/*
	./plots.sh $^