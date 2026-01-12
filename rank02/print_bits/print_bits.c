#include <stdio.h>
#include <unistd.h>

// Function to print binary representation of a number
void printBinary(int num) {
	int i = sizeof(num) * 8; // Total bits in the integer
	int bit;
	while (--i >= 0)
	{
		bit = (num >> i) & 1;
		printf("%d", bit);
	}
    printf("\n"); // Newline for clarity
}

int main() {
    int num = -2; // Example number
    printf("Binary representation of %d is: ", num);
    printBinary(num);
    return 0;
}
