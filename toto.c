#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char buff[6] = {0};
	while (read(0, buff, 6))
		printf("ecrit : 0x%X%X%X\n", buff[2], buff[1], buff[0]);
	return 0;
}