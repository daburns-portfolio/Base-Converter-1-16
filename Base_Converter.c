#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

typedef unsigned long max_size;

/* Analyzes the command line from the user, ensuring all inputs are valid.
 * This includes Base 1 - 16
 *
 */
void checkCommandLine(int argc, char * argv[]);
int numDigits(int value, int base);
max_size tobase10(char * string, int from_base);
unsigned int ascii2digit(char value);
void string_upper(char * string);
int num_digits(max_size value, int base);
int valid_base(char * string, int from_base);
int no_char(char * string);
void from_10_to_base(int * new, max_size start, int new_base);
void print_new_base(int * new_base, int digits);
void checkNull(void * ptr);
max_size topower(int from_base, int exp);

/* Command Line Argument Example
 * ./a.out 123 10 5
 * convert 123 from base 10 to base 5
 *
 * argc = 4
 * argv[1] = 123
 *
 * argv[1][0]
 *
 * argv[2] = 10
 * argv[3] = 5
 *
 */
int main(int argc, char *argv[]){
	
	checkCommandLine(argc,argv);
	int i = 0;
	int from_base = atoi(argv[2]);
	int to_base = atoi(argv[3]);
	max_size dec_value = tobase10(argv[1],from_base);
	int digits = num_digits(dec_value, to_base);
	int * new_value = (int *) calloc(digits, sizeof(int));
	checkNull(new_value);

	from_10_to_base(new_value, dec_value, to_base);

	print_new_base(new_value, digits);

	free(new_value);
	return 0;

}

max_size topower(int from_base, int exp){
	max_size sum = 1;
	int i;
	for (i = 0; i < exp; i++){
		sum = sum * from_base;
	}

	return sum;
}

void checkNull(void * ptr){
	if (ptr == NULL){
		printf("Malloc Failed\n");
		exit(1);
	}
}

int num_digits(max_size value, int base){
	return 1 + floor(log(value) / log(base));	
}


void string_upper(char * string){
	int i = 0;
	while (string[i] != '\0'){
		string[i] = toupper(string[i]);
		i++;
	}
}

/* Assumes the value is already verified to be either
 * a ascii number or letter.
 */
unsigned int ascii2digit(char value){
	if ((value >= 48) && (value <= 57)){
		return value - 48;
	} else {
		return value - 55;
	}
}


max_size tobase10(char * string, int from_base){
	int i = 0, exp = strlen(string) - 1;
	max_size sum = 0;
	max_size overflow = sum;

	while (string[i] != '\0'){
		//sum = sum + (ascii2digit(string[i]) * topower(from_base,exp));
		sum = sum + ascii2digit(string[i]) * (max_size) pow(from_base,exp);
		if (overflow == sum){
			printf("Value too large for register\n");
			exit(0);
		}
		overflow = sum;
		i++;
		exp--;
	}

	return sum;
}

int valid_base(char * string, int from_base){
	int i = 0;
	while (string[i] != '\0'){
		if (ascii2digit(string[i]) >= from_base){
			return 0;
		}
		i++;
	}
	return 1;
}

int no_char(char * string){
	int i =0;

	while (string[i] != '\0'){
		if ((string[i] < 48) || (string[i] > 57)){
			return 0;
		}
		i++;
	}
	return 1;
}

void checkCommandLine(int argc, char * argv[]){
	string_upper(argv[1]);

	if (!strcmp(argv[1],"-HELP")){	
		printf("Command Line should take the form:\n./convert <value> <base_from> <base_to>\nWhere base_from and base_to are between 1 and 16\n");
		exit(0);
	} else if (argc != 4){
		printf("Improper Syntax, use ./convert -help for proper usage\n");
		exit(0);
	} else if ((!no_char(argv[2])) || (!no_char(argv[3]))){
		printf("Invalid Bases\n");
		exit(0);
	} else if ((atoi(argv[2]) > 16) || (atoi(argv[2]) < 2) || (atoi(argv[3]) > 16) || (atoi(argv[3]) < 2)){
		printf("Can only convert between bases 1-16\n");
		exit(0);
	} else if (!valid_base(argv[1], atoi(argv[2]))){
		printf("Value not base %s\n", argv[2]);
		exit(0);
	}
}

void from_10_to_base(int * new, max_size start, int new_base){
	int i = 0;
	do {
		new[i] = start % new_base;
		start = start / new_base;
		i++;
	} while (start != 0);
}

/* Assumes the value is a value that can be
 * represented in hex. identifies the letter
 * corresponding to a 2 digits value
 *
 * value	the value between 10 and 15 that is 
 * 		analyzed. Supports upto base 16
 *
 * Return	returns the corresponding character
 */
char hex_digit(int value){
	switch (value) {
		case 10: return 'A';
		case 11: return 'B';
		case 12: return 'C';
		case 13: return 'D';
		case 14: return 'E';
		case 15: return 'F';
	}
}

void print_new_base(int * new_base, int digits){
	int i = 0;	
	for (i=digits-1; i >= 0; i--){
		if (new_base[i] < 10){
			printf("%d", new_base[i]);
		} else {
			printf("%c", hex_digit(new_base[i]));
		}
	}
	printf("\n");
}
