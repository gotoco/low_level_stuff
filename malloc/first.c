#include <stdio.h>

static unsigned char _mem[1024 * 1024]; //reserve 1 MB for malloc

static void * _next_index = &_mem;
static void * _last = NULL;
static void * _free = NULL;

struct _mhead {
	size_t size;
	void * prev;
};

#define _MS	sizeof(struct _mhead)

void *malloc(size_t s)
{
	void *mem;
	struct _mhead * mh;

	if( sizeof(_mem) < (_next_index-(void*)&_mem) + s)
		return NULL;

	mem = _next_index;
    	mh = mem;
	mh->size = s;
	mh->prev = _last;

	_last = _next_index;
	_next_index += (s+_MS);

	return mem + _MS;
}

void free(void *mem)
{
	void * tmp = _free;
	struct _mhead * mh = (mem - _MS);
	mh->prev = tmp;
	_free = (void*)mh;
}

int main()
{
	void * mem [256]; // malloc(sizeof(char)*1024);

	for(int i=0; i<256; ++i) {
		mem[i] = malloc(1024);
		for(int j=0; j<1024; j++)
			*(char*)(mem[i]+j) = i;
	}

	printf("Hello World\n");
	printf("sizeof size_t: %lu \n", sizeof(size_t));
	printf("sizeof char: %lu \n", sizeof(char));
	printf("sizeof void: %lu \n", sizeof(void*));
}
