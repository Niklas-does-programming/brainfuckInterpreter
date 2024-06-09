#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAXSIZE = 30000;

int main(int argc, char* argv[]){
        const char* FILEPATH = argv[1];
	FILE* fptr;
	char* code;
	long numbytes;

	//next open the file
	fptr = fopen(FILEPATH, "r");

	//check if the file was found
	if(fptr == NULL){
               	printf("could not find file %s\n", FILEPATH);
               	return 0;
       	}

	//set up the cells and a pointer to the current cell
	unsigned int cells[30000] = {0};
	int pointerPosition = 0;
	unsigned int maxPointerPosition = 0;
	int openedBrackets = 0;
	
	//read out code
	fseek(fptr, 0L, SEEK_END);
	numbytes = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);

	code = (char*)calloc(numbytes, sizeof(char));
	if(code == NULL){
		printf("could not read out code\n");
		return 0;
	}

	fread(code, sizeof(char), numbytes, fptr);

	int codePosition = 0;

	printf("reading following code: \n{\n%s}\n", code);
	while(codePosition < strlen(code)){
		char currentSign = code[codePosition];
		printf("current sign is: %c\n", currentSign);
		switch(currentSign){
			case '>':
				if(pointerPosition >= MAXSIZE){
					printf("pointer is exceeding maximum cell count %d\n", MAXSIZE);
					return 0;
				}
				pointerPosition++;
				if(pointerPosition > maxPointerPosition){
					maxPointerPosition = pointerPosition;
				}
				break;
			case '<':
				if(pointerPosition <= 0){
					printf("pointer has reached %d invalid position\n", pointerPosition - 1);
					return 0;
				}
				pointerPosition--;
				break;
			case '+':
				cells[pointerPosition] = cells[pointerPosition] + 1;
				break;
			case '-':
				cells[pointerPosition] = cells[pointerPosition] - 1;
				break;
			case '\n':
				break;
			case '[':
				openedBrackets++;
				break;
			case ']':
				//wenn bei pointer 0 dann weiter sonst zu entsprechendem '[' 
				openedBrackets--;
				if(openedBrackets < 0){
					printf("closed more brackets than you opened\n");
					return 0;
				}
				if(cells[pointerPosition] != 0){
					int tmpBracketCount = 1;	
					while(tmpBracketCount > 0){
						codePosition--;
						if(code[codePosition] == ']'){
							tmpBracketCount++;
						}else if(code[codePosition] == '['){
							tmpBracketCount--;
						}
					}
					//decreasing one more so that it counts the opened bracket in again
					codePosition--;
				}
				break;
			case '.':
				printf("printing char: %c\n", (char)cells[pointerPosition]);
				break;
			case ',':
                                char input[1];
                                printf("please input one character that has is in the 7-bit ascii table:\n");
                                scanf("%c", input);
                                cells[pointerPosition] = (int)input[0];
				break;
			default:
				printf("sign \"%c\" is not a valid sign\n", currentSign);
				return 0;
		}
		printf("current cells look like this:\n");
		for(int i = 0; i < maxPointerPosition + 1; i++){
                	if(i == pointerPosition){
				printf("{[%d]}", cells[i]);
			}else{
				printf("[%d]", cells[i]);
			}
        	}
		printf("\n");
		codePosition++;
	}

	if(openedBrackets != 0){
		printf("%d unclosed brackets, invalid syntax", openedBrackets);
	}

	fclose(fptr);
	return 0;
}
