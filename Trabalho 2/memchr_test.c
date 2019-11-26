#include <stdio.h>

const void *memchr(const void *ptr, int value, size_t num);

void memchr_test() {
	char *s1 = "";
	char *s2 = "abcdefabcdef";
	char *s3 = "11111111111111111111";

	printf("Testing memchr():\nTest1...");
	if (memchr(s1, 'x', 0) == NULL)
		printf("passed.");
	else	
		printf("FAILED.");

	printf("\nTest2...");
	if (memchr(s2, 'y', 0) == NULL)
		printf("passed.");
	else	
		printf("FAILED.");

	printf("\nTest3...");
	if ((char *)memchr(s2, 'a', 1) - s2 == 0)
		printf("passed.");
	else	
		printf("FAILED.");

	printf("\nTest4...");
	if (memchr(s2, 'd', 2) == NULL)
		printf("passed.");
	else	
		printf("FAILED.");

	printf("\nTest5...");
	if ((char *)memchr(s2, 'd', 12)  - s2 == 3)
		printf("passed.");
	else	
		printf("FAILED.");

	printf("\nTest6...");
	if ((char *)memchr(s2, 'f', 12)  - s2 == 5)
		printf("passed.");
	else	
		printf("FAILED.");
	printf("\nTest7...");

	if ((char *)memchr(s3, '1', 20)  - s3 == 0)
		printf("passed.");
	else	
		printf("FAILED.");

	putchar('\n');
}

int main(int argc, char *argv[]) {
	memchr_test();
	return 0;
}