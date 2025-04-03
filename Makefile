CC=clang
LIBS=-lraylib
CFLAGS=-Wall -Wextra -ggdb

build: build/thatone

build/thatone: src/thatone.c build/libprocess.so
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) -ldl

build/libprocess.so: src/process.c src/mmu.c src/registers.c src/state.c
	-mkdir build
	$(CC) $(CFLAGS) -o $@ $? $(LIBS) -fPIC -shared

run: build
	LD_LIBRARY_PATH=./build/ build/thatone

watch: build
	make run &
	while true; do \
		inotifywait -qr -e modify -e create -e delete -e move src; \
		make build; \
	done

clean:
	-rm -rf build
