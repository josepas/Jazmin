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
    return hash;
}



int main(int argc, char const *argv[])
{
	printf("%u\n", one_at_a_time_hash("a", 2) );
	printf("%u\n", one_at_a_time_hash("b", 2) );
	printf("%u\n", one_at_a_time_hash("c", 2) );


	return 0;
}