#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define constants for the assembler
#define MAX_LINE_LENGTH 500
#define MAX_INSTRUCTION_COUNT 4096
#define MAX_WORDS_PER_LINE 100
#define MAX_LABEL_COUNT 1000 // Adjust based on expected number of labels
// Function prototypes
int checkLABEL(char* word);
char* createBinaryLinePkoda(char words[MAX_WORDS_PER_LINE][MAX_LINE_LENGTH]);
void opcode_to_hex(const char *opcode_name, char *addr);
void processLine(char *line, char words[MAX_WORDS_PER_LINE][MAX_LINE_LENGTH]);
void writeLineToFile(const char* line);
void reg_to_hex(const char* regName, char* hexValue);
void int_to_hex3(const char* intString, char* hexString);
void storeLabel(const char* labelName, int lineNumber);
void int_to_hex3_jal(char* Label, char* hexString);
int hextoint(const char* hexString);
void hexImmiHandler(const char* Label, char* hexString);
char* trim(char* str);
int isEmptyLine(const char *line);

typedef struct {
    char labelName[MAX_LINE_LENGTH]; // Assuming label names won't exceed MAX_LINE_LENGTH
    int lineNumber;
} LabelInfo;


LabelInfo labels[MAX_LABEL_COUNT];
int labelCount = 0;
// Global variables to store instructions and their binary encodings
// char** imemin[MAX_INSTRUCTION_COUNT][MAX_LINE_LENGTH];
int PC = 0;
int PC2 = 0;
int adressChanges[4096] = {0};
int maxAdress = 0;
int adress;
int data;
char hex[9];
FILE *filePointer;
FILE *fileWriter;
char* filename;
char* fileDememin;
int indexofnikodothaim;
char* wordchecking;

int main(int argc, char* argv[]) {

    int i;
    //check how many args there are
    if (argc != 4) {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }
    filename = argv[2];
    fileDememin = argv[3];

    filePointer = fopen(filename, "w");
    fclose(filePointer);

    // Open the assembly file::
    FILE* assemblyFile = fopen(argv[1], "r");
    if (assemblyFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // for each instruction translate ::
    char line[500];
    char* bLine;
    char readTo[500];
    // @@@@@@@@@@@@@@@@@@@@@@@ MAKE THE LABEL STRUCT @@@@@@@@@@@@
    FILE* assemblyFile2 = fopen(argv[1], "r");
    if (assemblyFile2 == NULL) {
        perror("Error opening file");
        return 1;
    }


    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    while (fgets(line, sizeof(line), assemblyFile2)) {
        if(isEmptyLine(line)==1)
        {
            continue;
        }
        char words[MAX_WORDS_PER_LINE][MAX_LINE_LENGTH] = {{0}};
        processLine(line, words);
     // Check if it's a label line: if the first word ends with ':'
        wordchecking = trim(words[0]);
        if (wordchecking[strlen(wordchecking) - 1] == ':') {
            indexofnikodothaim = strlen(words[0]) - 1;
            storeLabel(wordchecking, PC2);
            continue;
        }
        // if(words[0][strlen(words[0]) - 2] == ':')
        // {
        //     storeLabel(words[0], PC2);
        //     continue;
        // }
    // do something if .word ::
        i=0;
        
        while (words[0][i] == ' ')
        {
            i++;
            if (i>50)
            {
                break;
            }
        }
        if (words[0][i] == '.')
        {
            if((words[1][1] == 'x' || words[1][1] == 'X')&&((words[1][0]) == '0'))
            {   
                adress = hextoint(words[1]); 
            }else{
                adress = atoi(words[1]);
            }
            
            if((words[2][1] == 'x' || words[2][1] == 'X')&&((words[2][0]) == '0'))
            {   
                data = hextoint(words[2]); 
            }else{
                data = atoi(words[2]);
            }
            adressChanges[adress] = data;
            if (adress > maxAdress)
            {
                maxAdress=adress;
            }
            continue;
        }
        PC2++;
    }
    
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    // Open file in write mode. This will create the file if it doesn't exist
    // or overwrite it if it does.
    fileWriter = fopen(filename, "a");

    while (fgets(line, sizeof(line), assemblyFile)) {
        if(isEmptyLine(line)==1)
        {
            continue;
        }
        char words[MAX_WORDS_PER_LINE][MAX_LINE_LENGTH] = {{0}};
        processLine(line, words);
        i=0;
        while (words[0][i] == ' ')
        {
            i++;
            if (i>50)
            {
                break;
            }
        }
        if (words[0][i] == '.')
        {
            continue;
        }
     
     // Check if it's a label line: if the first word ends with ':'
        wordchecking = trim(words[0]);
        if (wordchecking[strlen(wordchecking) - 1] == ':') {
            continue;
        }
    // Otherwise, it's considered an opcode line
        else if (words[0][0] != '\0') { // Also checks that the line is not empty
            bLine = createBinaryLinePkoda(words);
            writeLineToFile(bLine);
        }

        PC++;
    }
    
    fclose(assemblyFile);
    fclose(fileWriter);
    
    // lets make the dmemin file :::: @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    FILE *filePointer;

    // Open file in write mode. This will create the file if it doesn't exist
    // or overwrite it if it does.
    filePointer = fopen(fileDememin, "w");
    fclose(filePointer);
    filePointer = fopen(fileDememin, "a");
    if (filePointer == NULL) {
        printf("Failed to open or create the file.\n");
        return 1; // Exit the function if file opening fails
    }
    for (i=0;i<=maxAdress;i++)
    {
        snprintf(hex, 9, "%08X", adressChanges[i]);
        // Write the line to the file
        fprintf(filePointer, "%s\n", hex); // Adding a newline character at the end
         
    }
    fclose(filePointer);



}

char* createBinaryLinePkoda(char words[MAX_WORDS_PER_LINE][MAX_LINE_LENGTH]) {
    // Allocate memory for the entire line and ensure it's enough
    char* bLine = malloc(1000); // Assuming 1000 is enough for the entire line
    if (!bLine) {
        fprintf(stderr, "Memory allocation failed for bLine\n");
        return NULL;
    }
    char opcode[3] = {0}; // Enough for 2 chars + null terminator
    char rd[3] = {0}, rs[3] = {0}, rt[3] = {0}, rm[3] = {0}; // Register values
    char imm1[4] = {0}, imm2[4] = {0}; // Immediate values
    
    // Fill opcode and other variables
    opcode_to_hex(words[0], opcode);

    reg_to_hex(words[1], rd);
    reg_to_hex(words[2], rs);
    reg_to_hex(words[3], rt);
    reg_to_hex(words[4], rm);
    if (isalpha(words[5][0])!=0)
    {
        int_to_hex3_jal(words[5], imm1);
    }
    else if (words[5][1] == 'x' || words[5][1] == 'X')
    {
        hexImmiHandler(words[5], imm1);
    }
    else{
        int_to_hex3(words[5], imm1);
    }

    if (isalpha(words[6][0])!=0)
    {
        int_to_hex3_jal(words[6], imm2);
    }
    else if (words[6][1] == 'x' || words[6][1] == 'X')
    {
        hexImmiHandler(words[6], imm2);
    }
    else{
        int_to_hex3(words[6], imm2);
    }

    bLine[0] = '\0';
    strcat(bLine, opcode);
    strcat(bLine, rd);
    strcat(bLine, rs);
    strcat(bLine, rt);
    strcat(bLine, rm);
    strcat(bLine, imm1);
    strcat(bLine, imm2);

    return bLine;
}

//function to make opcode to a hex
void opcode_to_hex(const char *opcode_name, char *addr) {

    if (strcmp(opcode_name, "add") == 0) {
        strcpy(addr, "00");
    } else if (strcmp(opcode_name, "sub") == 0) {
        strcpy(addr, "01");
    } else if (strcmp(opcode_name, "mac") == 0) {
        strcpy(addr, "02");
    } else if (strcmp(opcode_name, "and") == 0) {
        strcpy(addr, "03");
    } else if (strcmp(opcode_name, "or") == 0) {
        strcpy(addr, "04");
    } else if (strcmp(opcode_name, "xor") == 0) {
        strcpy(addr, "05");
    } else if (strcmp(opcode_name, "sll") == 0) {
        strcpy(addr, "06");
    } else if (strcmp(opcode_name, "sra") == 0) {
        strcpy(addr, "07");
    } else if (strcmp(opcode_name, "srl") == 0) {
        strcpy(addr, "08");
    } else if (strcmp(opcode_name, "beq") == 0) {
        strcpy(addr, "09");
    } else if (strcmp(opcode_name, "bne") == 0) {
        strcpy(addr, "0A");
    } else if (strcmp(opcode_name, "blt") == 0) {
        strcpy(addr, "0B");
    } else if (strcmp(opcode_name, "bgt") == 0) {
        strcpy(addr, "0C");
    } else if (strcmp(opcode_name, "ble") == 0) {
        strcpy(addr, "0D");
    } else if (strcmp(opcode_name, "bge") == 0) {
        strcpy(addr, "0E");
    } else if (strcmp(opcode_name, "jal") == 0) {
        strcpy(addr, "0F");
    } else if (strcmp(opcode_name, "lw") == 0) {
        strcpy(addr, "10");
    } else if (strcmp(opcode_name, "sw") == 0) {
        strcpy(addr, "11");
    } else if (strcmp(opcode_name, "reti") == 0) {
        strcpy(addr, "12");
    } else if (strcmp(opcode_name, "in") == 0) {
        strcpy(addr, "13");
    } else if (strcmp(opcode_name, "out") == 0) {
        strcpy(addr, "14");
    } else if (strcmp(opcode_name, "halt") == 0) {
        strcpy(addr, "15");
    } else {
        // Invalid opcode name
        strcpy(addr, "FF"); // Use 0xFF to indicate an error/invalid opcode
    }
    

}


 //opcode rd, rt, rs, rm, imm1, imm2

// Function to split a line into words and store them in an array
// Function to remove trailing comma from a word if present
void removeTrailingComma(char* word) {
    size_t len = strlen(word); // Get the length of the word
    if (len > 0 && word[len - 1] == ',') {
        word[len - 1] = '\0'; // Remove the comma by setting it to null character
    }
}

// Modified processLine function to remove trailing commas from words
void processLine(char *line, char words[MAX_WORDS_PER_LINE][MAX_LINE_LENGTH]) {
    // Check for and remove any comments
    char *comment = strchr(line, '#');
    if (comment) *comment = '\0'; // Terminate the line at the start of the comment

    // Tokenize the line into words
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < MAX_WORDS_PER_LINE) {
        removeTrailingComma(token); // Remove trailing comma if present
        strncpy(words[i], token, MAX_LINE_LENGTH - 1); // Copy the modified token to words array
        words[i][MAX_LINE_LENGTH - 1] = '\0'; // Ensure the string is null-terminated
        i++;
        token = strtok(NULL, " \t\n");
    }
}

// Function to write a bLine into the file imemin.txt 
void writeLineToFile(const char* line) {


    if (fileWriter == NULL) {
        printf("Failed to open or create the file.\n");
        return; // Exit the function if file opening fails
    }

    // Write the line to the file
    fprintf(fileWriter, "%s\n", line); // Adding a newline character at the end
}

// General function to convert register name to hex value
void reg_to_hex(const char* regName, char* hexValue) {

    if (strcmp(regName, "$zero") == 0) strcpy(hexValue, "0");
    else if (strcmp(regName, "$imm1") == 0) strcpy(hexValue, "1");
    else if (strcmp(regName, "$imm2") == 0) strcpy(hexValue, "2");
    else if (strcmp(regName, "$v0") == 0) strcpy(hexValue, "3");
    else if (strcmp(regName, "$a0") == 0) strcpy(hexValue, "4");
    else if (strcmp(regName, "$a1") == 0) strcpy(hexValue, "5");
    else if (strcmp(regName, "$a2") == 0) strcpy(hexValue, "6");
    else if (strcmp(regName, "$t0") == 0) strcpy(hexValue, "7");
    else if (strcmp(regName, "$t1") == 0) strcpy(hexValue, "8");
    else if (strcmp(regName, "$t2") == 0) strcpy(hexValue, "9");
    else if (strcmp(regName, "$s0") == 0) strcpy(hexValue, "A");
    else if (strcmp(regName, "$s1") == 0) strcpy(hexValue, "B");
    else if (strcmp(regName, "$s2") == 0) strcpy(hexValue, "C");
    else if (strcmp(regName, "$gp") == 0) strcpy(hexValue, "D");
    else if (strcmp(regName, "$sp") == 0) strcpy(hexValue, "E");
    else if (strcmp(regName, "$ra") == 0) strcpy(hexValue, "F");
    else strcpy(hexValue, "Invalid"); // Handle invalid register name
}

// Function to convert a string representing an integer to a 3-character hexadecimal string, with leading zeros
void int_to_hex3(const char* intString, char* hexString) {
    int num = atoi(intString); // Convert the string to an integer

    // Mask the number to 12 bits to simulate a 12-bit 2's complement system.
    // This works because 0xFFF is the 12-bit maximum, and bitwise ANDing with
    // 0xFFF effectively keeps only the lowest 12 bits of 'num',
    // which simulates a 12-bit 2's complement for negative numbers.
    int maskedNum = num & 0xFFF;

    snprintf(hexString, 4, "%03X", maskedNum); // Format the number as a 3-digit hexadecimal with leading zeros
}

void storeLabel(const char* labelName, int lineNumber) {
    if (labelCount >= MAX_LABEL_COUNT) {
        fprintf(stderr, "Label storage limit reached.\n");
        return; // Optionally, handle this more gracefully
    }

    // Copy label name, excluding the colon at the end
    strncpy(labels[labelCount].labelName, labelName, strlen(labelName) - 1);
    labels[labelCount].labelName[strlen(labelName) - 1] = '\0'; // Null-terminate
    
    labels[labelCount].lineNumber = lineNumber;
    labelCount++;
}

void int_to_hex3_jal(char* Label, char* hexString) {
    
    int lineNum = -1; // Use this to store the found line number, initialize to an invalid value
    // Search for the label
    // printf("%s \n",Label);
    for (int i = 0; i < labelCount; i++) {
        if (strcmp(labels[i].labelName, Label) == 0) {
        
            lineNum = labels[i].lineNumber; // Store the line number if the label matches
            break; // Exit the loop once the label is found
        }
    }

    if (lineNum != -1) {
        // If a matching label was found, convert its line number to a hexadecimal string.
        snprintf(hexString, 4, "%03X", lineNum);
    } else {
        // Handle the case where the label was not found
        strcpy(hexString, "000"); // Or some other error handling
    }
}

int checkLABEL(char* word)
{
    if (strcmp(word,"beq") == 0 )
        {
            return 0;
        }
    if (strcmp(word,"bne") == 0 )
        {
            return 0;
        }
    if (strcmp(word,"blt") == 0 )
        {
            return 0;
        }
    if (strcmp(word,"bgt") == 0 )
        {
            return 0;
        }
    if (strcmp(word,"ble") == 0 )
        {
            return 0;
        }
    if (strcmp(word,"bge") == 0 )
        {
            return 0;
        }
    if (strcmp(word,"jal") == 0 )
        {
            return 0;
        }
    return 1;
}


void hexImmiHandler(const char* Label, char* hexString)
{
    hexString = strcpy(hexString,Label+2);
}

int hextoint(const char* hexString){
    int num =0;
    sscanf(hexString, "%x", &num);
    return num;
}
char* trim(char* str) {
    char* end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

// Function to check if a line is empty (contains only whitespace characters)
int isEmptyLine(const char *line) {
    while (*line != '\0') { // Loop through each character in the string
        if (!isspace((unsigned char)*line)) {
            return 0; // Found a non-whitespace character
        }
        line++; // Move to the next character
    }
    return 1; // Only whitespace characters found
}