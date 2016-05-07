#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

uint32_t one_at_a_time_hash(char *key, size_t len)
{
    uint32_t hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % 17;
}



int main(int argc, char const *argv[])
{
	printf("%u\n", one_at_a_time_hash("x", 1) );
	printf("%u\n", one_at_a_time_hash("y", 1) );
	printf("%u\n", one_at_a_time_hash("z", 1) );
	printf("%u\n", one_at_a_time_hash("i", 1) );


	return 0;
}