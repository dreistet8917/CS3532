
#include<stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned short ushort;
typedef unsigned int uint32;
typedef unsigned long uint64;

char* getFname();
uint32 getCode(char *file, int count);
bool testValidity(char *newFile, uint32 someCode);
int putInBuffer(char * name);

int main() {
 
/* - ask filename from user and put file contents in a buffer and update the count of bytes/words.
 - call getCode()function to calculate code using one's complement.
*/
 char* fileName = getFname(); // assigned to fd called infile
 printf("File Name = %s\n", fileName);

 int count = putInBuffer(fileName);
 printf("Number of bytes is %d\n", count);
 uint32 checkSum = getCode(fileName, count);
 printf("Check sum: %d\n", checkSum);
 //code = getCode(infile,count);
/* use code to validate another user-specified file:
* get new file, calculate code and see if they match. If both codes match, then they are have the same contents */
 //testValidity(infile, code);

} //end main

/**
  *Gets the file name for user input
  *@return string filename
  */ 
char* getFname(){
	char* fname = (char*)malloc(100*sizeof(char));
	printf("\n\nEnter the file name: \n");
	scanf("%s", fname);
//	printf("%s\n", &fname);
	return fname;
}

/**
  *Takes the file, reads the file, and calculates the number of bytes in the file
  *@param char* name the string file name 
  *@return int numbytes the number of bytes in the file
  */ 
int putInBuffer(char* name){
//variables, file, buffer string and int number of bytes
FILE *infile;
char *buffer;
int numbytes;

if (access( name, F_OK) != -1){ //check to see if the file exists before attempting to open
	printf("File does exist\n");
} else {
	printf("File does not exist\n");
}

infile = fopen(name, "r"); //open the file to read
 
if(infile == NULL) // quit if the file does not exist
    return 1;

fseek(infile, 0L, SEEK_END); // Get the number of bytes
numbytes = ftell(infile);
 
fseek(infile, 0L, SEEK_SET); // reset the file position indicator to the beginning of the file 	
 
buffer = (char*)calloc(numbytes, sizeof(char));	 //grab sufficient memory for the buffer to hold the text.
 
if(buffer == NULL) //if there is a memory error
    return 1;
 
fread(buffer, sizeof(char), numbytes, infile); //copy all test into the buffer

printf("The file called contains this text\n%s\n", buffer); // confirm we have read the file

fclose(infile); //close the file
free(buffer); //free the memory we used for the buffer 
	return numbytes; //return number of bytes in the file
}

/**
  *Sends in a file, puts the contents in a 32-bit buffer, returns a code for checksum
  *@param char* file the string file name 
  *@param int count the number of bytes in the file
  *@return uint32 unsigned int 32 code that is toggled for the checksum algorithm
  */ 
uint32 getCode(char *file, int count) {
	 int numBytes = count;
	 int numBits = numBytes * 8;
	 int numChunks;
	 int remainder = 0;
	 uint32 remainderChunks;
	 //char *buffer; // put 32-bits of file in a buffer
	 int* buffer; 
	 FILE* infile;
	 uint64 sum=0;



	 if(numBits % 32 == 0){ //if the number of chunks is evenly chunked
	 	numChunks = numBits / 32; //numchunks = numbytes * 8 divided by 32
	 } else { //otherwise there is not an even number of chunks
	 	numChunks = numBits / 32; //number of chunks with remainder
	 	remainder = numBits % 32; //number to be padded
	 }
	 printf("NumBits: %d\n", numBits);
	 printf("NumChunks: %d\n", numChunks);
	 printf("Remainder: %d\n", remainder);

	 infile = fopen(file, "r");
	 int currentChar;
	 int* charArray[numBits];
	 int counter = 0;
	 int chunkCount = 0;
	 int* chunkArray[numChunks];
	 int charSum = 0;
	 //currentChar = fgetc(infile);
	 //charArray[counter] = currentChar;
	 //counter += 4;
	 //printf("Current Char: %d\n", currentChar);
	 while(counter <= 32) {
	 	printf("Counter: %d\n", counter);
	 	if(counter != 32) {
	 		currentChar = fgetc(infile);
	 		charArray[counter] = currentChar;
	 		charSum += currentChar;
	 		printf("Current Char: %d\n", currentChar);
	 		if(feof(infile)) {
	 			break;
	 		}
	 		counter += 4;
	 	} else if (counter == 32) {
	 		printf("---------------------\n");
	 		buffer = charArray;
	 		printf("Sum: %d\n", charSum);
	 		chunkArray[chunkCount] = buffer;
	 		sum += chunkArray[chunkCount];
	 		chunkCount++;
	 		counter = 0;
	 	}
	 }
	 fclose(infile);
	 printf("OUT WHILE\n");
	/*	while (numBytes--) {
			 if(charSum & 0xffffffff00000000){
				printf("There is an overflow bit\n");
			}else{
				charSum++;
				printf("There is no overflow\n");
			} 
			numBytes--;
		} */
		printf("RETURNING!\n");
		return sum ; // one’s complement of the total 
} 
/* send file and some code in → returns true if getCode of the file == some code sent in as argument */
/* bool testValidity(char *newfile, uint32 someCode){
 

return (getCode(newfile, count)== someCode);
} */