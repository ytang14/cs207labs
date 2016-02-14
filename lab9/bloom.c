#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "bloom.h"
#include <time.h>
#include <unistd.h>
/* Hash functions*/

index_t hash2(bloom_filter_t *B, key_t k)
{

  index_t v = k * 3935559000370003845 + 2691343689449507681;

  v ^= v >> 21;
  v ^= v << 37;
  v ^= v >>  4;

  v *= 4768777513237032717;

  v ^= v << 20;
  v ^= v >> 41;
  v ^= v <<  5;

  return v;
}

index_t hash1(bloom_filter_t *B, key_t k)
{
	key_t x = k;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x);
    return x;
}
index_t* rand100(void)
{
		srand(time(NULL));

	index_t* r = calloc(100,sizeof(uint64_t));
	for(uint64_t i = 0; i<100; i++)
	{
		r[i] = rand()%1000000;
	}
	return r;
}
int smokeTest(void)
{
	bloom_filter_t* t = (bloom_filter_t*) malloc(sizeof(bloom_filter_t));
	bloom_init(t,1000);
	for(uint64_t i = 1; i<=70;i++)
	{
	}
	uint64_t ctr = 0;
	for(uint64_t i = 0; i<1000;i++)
	{
		if (get_bit(t,i))
			ctr++;
	}
	printf("smoke test of occupany = %llu\n",ctr);
	return 0;
}
int maintest(uint64_t* a, uint64_t* b)
{
	uint64_t keys[6] = {0,1,2,3,13,97};
	bloom_filter_t* B = malloc(sizeof(bloom_filter_t));
	bloom_init(B,(uint64_t)1000);
	for (uint64_t i = 0; i<100; i++)
	{
		bloom_add(B, a[i]);
	}
	for (uint64_t i = 0; i < 6; i++)
	{
		printf("hash1(%llu) = %llu\thash2(%llu) = %llu\n",keys[i],hash1(NULL,keys[i]),keys[i],hash2(NULL,keys[i]));
	}
	uint64_t ctr = 0;
	for (uint64_t i = 0; i<1000; i++)
	{
		if (get_bit(B,i))
			ctr++;
	}
	printf("Occupancy = %llu\n",ctr);
	ctr = 0;
	for (uint64_t i = 0; i<100; i++)
	{
		if(bloom_check(B,b[i]) == (uint64_t)1)
			{
				ctr++;
			}
	}
	printf("There are %llu False Positive Occurance(s).\n",ctr);
	return 1;
}
int main(int argc, char **argv)
{
	
	smokeTest();
	bloom_filter_t *t = (bloom_filter_t *)malloc(sizeof(bloom_filter_t));
	uint64_t *a = rand100();
	sleep(1);
	uint64_t *b = rand100();
	maintest(a,b);	
	return 0; 
}

/* set Bit of the given index*/
void set_bit(bloom_filter_t *B, index_t i)
{
	if (i>=B->size)
	{
		printf("Set Index Out of Range!\n");
		return;
	}
	index_t *head = B->table;
	index_t intIdx = i>>(uint64_t)6;
	index_t bitIdx = i%(uint64_t)64;
	B->table[intIdx] |= (uint64_t)1<<bitIdx;
}

index_t get_bit(bloom_filter_t *B, index_t i)
{
	if (i>=B->size)
	{
		printf("Get Index Out of Range!\n");
		return 0;
	}
	index_t *head = B->table;
	index_t intIdx = i>>(uint64_t)6;
	index_t bitIdx = i%(uint64_t)64;
	index_t mask = ((uint64_t)1)<<bitIdx;
	return (head[intIdx]&mask)>>bitIdx;
}

/* Data related functions*/

void bloom_init(bloom_filter_t *B, index_t size_in_bits)
{
	B->count = (uint64_t)0;
	index_t size_in_64 = (uint64_t)0;
	if (size_in_bits%(sizeof(uint64_t)*8) != 0)
		size_in_64 = size_in_bits/(8*sizeof(uint64_t))+1;
	else
		size_in_64 = size_in_bits/(8*sizeof(uint64_t));
	B->size = size_in_bits;
	B->table = calloc(size_in_bits/64,sizeof(uint64_t));
}

void bloom_destroy(bloom_filter_t *B)
{
	free(B);
}

void bloom_add(bloom_filter_t *B, key_t k)
{
	for(index_t i = (uint64_t)0; i<N_HASHES; i++)
	{
		index_t hash = (hash1(B,k)+i*hash2(B,k))%B->size;
		set_bit(B,hash);
		B->count++;
	}

}

int bloom_check(bloom_filter_t *B, key_t k)
{
	index_t ctr = (uint64_t)0;
	for (index_t i = (uint64_t)0; i < N_HASHES; i++)
	{
		index_t hashKey = (hash1(B,k)+i*hash2(B,k))%B->size;
		ctr+=get_bit(B,hashKey);
	}
	if (ctr==N_HASHES)
		{
			return 1;
		}
	else 
		return 0;
}