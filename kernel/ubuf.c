#include "types.h"
#include "defs.h"

#define UBUF_SIZE 16

// buffer writes to head and read from tail 
struct ubuf {
    char data[UBUF_SIZE];
    int head;  
    int tail;
};
struct ubuf ubuf;

char ubuf_read(void) {
    char data = ubuf.data[ubuf.head];
    yield();
    ubuf.head = (ubuf.head + 1) % UBUF_SIZE;
    return data;
}

void ubuf_write(char val) {
    ubuf.data[ubuf.tail] = val;
    yield();
    ubuf.tail = (ubuf.tail + 1) % UBUF_SIZE;
}

void ubuf_init() {
    ubuf.head = 0;
    ubuf.tail = 0;
}