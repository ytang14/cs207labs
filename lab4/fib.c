#include <stdint.h>
#include <stdio.h>

int fib(int n)
{
	if (n==1)
		return 0;
	if (n==2)
		return 1;
	else 
		{
			return fib(n-1) + fib(n-2);
		}
}
int main(int argc, char **argv)
{
  int64_t a= atoi(argv[1]),b;
  b=fib(a);
  printf("Fibonacci(%lld)=%ld\n",a, b);
  return 0;
}