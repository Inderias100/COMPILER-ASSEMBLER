
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to store labels and their assigned memory addresses
typedef struct {
    char name[50];
    int address;
} Label;


// Table to store all labels found during the first pass
Label symbolTable[100];
int labelCount = 0;

// Function to convert MIPS register strings (mnemonic or numeric) to integers 0-31
int get_register(char* reg) {

    // Create a buffer to hold the cleaned register name
    char clean[20];
    
    int j = 0;
    
    // Iterate through characters to remove commas, spaces, or parentheses
    for(int i = 0; reg[i] != '\0'; i++) {
        if(isalnum(reg[i]) || reg[i] == '$') clean[j++] = reg[i];
    }
    
    clean[j] = '\0';
    
    // Map standard MIPS register names to their hardware indices
    if (strcmp(clean, "$zero") == 0) return 0;
    if (strcmp(clean, "$at") == 0) return 1;
    if (strcmp(clean, "$v0") == 0) return 2;
    if (strcmp(clean, "$v1") == 0) return 3;
    if (strcmp(clean, "$a0") == 0) return 4;
    if (strcmp(clean, "$a1") == 0) return 5;
    if (strcmp(clean, "$a2") == 0) return 6;
    if (strcmp(clean, "$a3") == 0) return 7;
    if (strcmp(clean, "$t0") == 0 || strcmp(clean, "$8") == 0) return 8;
    if (strcmp(clean, "$t1") == 0 || strcmp(clean, "$9") == 0) return 9;
    if (strcmp(clean, "$t2") == 0 || strcmp(clean, "$10") == 0) return 10;
    if (strcmp(clean, "$t3") == 0 || strcmp(clean, "$11") == 0) return 11;
    if (strcmp(clean, "$t4") == 0 || strcmp(clean, "$12") == 0) return 12;
    if (strcmp(clean, "$t5") == 0 || strcmp(clean, "$13") == 0) return 13;
    if (strcmp(clean, "$t6") == 0 || strcmp(clean, "$14") == 0) return 14;
    if (strcmp(clean, "$t7") == 0 || strcmp(clean, "$15") == 0) return 15;
    if (strcmp(clean, "$s0") == 0 || strcmp(clean, "$16") == 0) return 16;
    if (strcmp(clean, "$s1") == 0 || strcmp(clean, "$17") == 0) return 17;
    if (strcmp(clean, "$s2") == 0 || strcmp(clean, "$18") == 0) return 18;
    if (strcmp(clean, "$s3") == 0 || strcmp(clean, "$19") == 0) return 19;
    if (strcmp(clean, "$s4") == 0 || strcmp(clean, "$20") == 0) return 20;
    if (strcmp(clean, "$s5") == 0 || strcmp(clean, "$21") == 0) return 21;
    if (strcmp(clean, "$s6") == 0 || strcmp(clean, "$22") == 0) return 22;
    if (strcmp(clean, "$s7") == 0 || strcmp(clean, "$23") == 0) return 23;
    if (strcmp(clean, "$t8") == 0 || strcmp(clean, "$24") == 0) return 24;
    if (strcmp(clean, "$t9") == 0 || strcmp(clean, "$25") == 0) return 25;
    if (strcmp(clean, "$sp") == 0 || strcmp(clean, "$29") == 0) return 29;
    if (strcmp(clean, "$ra") == 0 || strcmp(clean, "$31") == 0) return 31;
    
    // If input is already numeric like "$8", convert it to int
    if (clean[0] == '$') return atoi(&clean[1]);
    
    return 0;
}

// Function to retrieve the memory address associated with a specific label
int get_label_address(char* label) {

    char clean[50];
    
    // Strip whitespace from label string
    sscanf(label, "%s", clean);
    
    // Search the symbol table for a matching name
    for (int i = 0; i < labelCount; i++) {
        if (strcmp(symbolTable[i].name, clean) == 0) return symbolTable[i].address;
    }
    
    return 0;
}

// Logic to pack R-Type instruction components into a 32-bit machine code
unsigned int encode_r(int rs, int rt, int rd, int funct) {
    // Opcode is always 0 for R-Type; Shift RS, RT, RD and Funct into position
    return (0x00 << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (0x00 << 6) | funct;
}

// Logic to pack I-Type instruction components into a 32-bit machine code
unsigned int encode_i(int opcode, int rs, int rt, int imm) {
    // Combine opcode, register RS, register RT, and the 16-bit immediate/offset
    return (opcode << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF);
}

// Logic to pack J-Type instruction components into a 32-bit machine code
unsigned int encode_j(int opcode, int target_addr) {
    // Combine opcode and the 26-bit jump target (address divided by 4)
    return (opcode << 26) | ((target_addr >> 2) & 0x03FFFFFF);
}

int main(int argc, char* argv[]) {

    // Ensure the user provided an input file as a command line argument
    if (argc < 2)
      {
      printf("Please Enter File Name");
      exit(0);
      }
      
    // Open the input assembly file
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL)
      {
      printf("Please Enter Correct File Name");
      exit(0);
      }
    
    
    char instructions[100][100];
    
    // Start address defined by the assignment requirement
    int current_addr = 0x00400000;
    int instr_count = 0;
    char buffer[150];

    // PASS 1: Identify all labels and filter out instructions
    while (fgets(buffer, sizeof(buffer), fp)) {
    
        char *line_ptr = buffer;
        
        // Skip any leading whitespace characters
        while(isspace(*line_ptr)) line_ptr++;
        
        // Skip empty lines
        if (*line_ptr == '\0' || *line_ptr == '\n') continue;
        
        // Check if the line defines a label (ends with colon)
        char *colon = strchr(line_ptr, ':');
        
        if (colon) {
        
            char name[50];
            
            // Extract label name and store it in our symbol table with current address
            sscanf(line_ptr, "%[^:]", name);
            strcpy(symbolTable[labelCount].name, name);
            symbolTable[labelCount].address = current_addr;
            labelCount++;
            
            // Move pointer forward to see if an instruction is on the same line
            line_ptr = colon + 1;
            while(isspace(*line_ptr)) line_ptr++;
            if (*line_ptr == '\0' || *line_ptr == '\n') continue;
        }
        
        // Save instruction for the second pass and increment address by 4
        strcpy(instructions[instr_count++], line_ptr);
        current_addr += 4;
    }

    // Rewind file to print the original source code first as requested
    rewind(fp);
    
    printf("\n");
    printf("Assembly language program:\n");
    while (fgets(buffer, sizeof(buffer), fp)) printf("%s", buffer);
    
    printf("\n");
    printf("\n");

    // PASS 2: Translate instructions into hexadecimal machine code
    printf("\nMachine Code:\n");
    current_addr = 0x00400000;
    
    for (int i = 0; i < instr_count; i++) {
    
        char op[15], r1[30], r2[30], r3[30];
        unsigned int code = 0;
        
        // Parse the instruction name and the three possible comma-separated operands
        int parts = sscanf(instructions[i], "%s %[^,], %[^,], %s", op, r1, r2, r3);

        // Core Translation Logic
        if (strcmp(op, "add") == 0) {
        
            // Check if third operand is a number (starts with digit or minus)
            if (isdigit(r3[0]) || r3[0] == '-') {
            
                // If it is a number, encode as ADDI (Opcode 0x08)
                code = encode_i(0x08, get_register(r2), get_register(r1), atoi(r3));
            
            } else {
            
                // Otherwise, encode as R-type ADD (Opcode 0, Funct 0x20)
                code = encode_r(get_register(r2), get_register(r3), get_register(r1), 0x20);
            }
        } 
        
        else if (strcmp(op, "addi") == 0) code = encode_i(0x08, get_register(r2), get_register(r1), atoi(r3));
        
        else if (strcmp(op, "and") == 0)  code = encode_r(get_register(r2), get_register(r3), get_register(r1), 0x24);
        
        else if (strcmp(op, "or") == 0)   code = encode_r(get_register(r2), get_register(r3), get_register(r1), 0x25);
        
        else if (strcmp(op, "xor") == 0)  code = encode_r(get_register(r2), get_register(r3), get_register(r1), 0x26);
        
        else if (strcmp(op, "slt") == 0)  code = encode_r(get_register(r2), get_register(r3), get_register(r1), 0x2a);
        
        else if (strcmp(op, "andi") == 0) code = encode_i(0x0c, get_register(r2), get_register(r1), atoi(r3));
        
        else if (strcmp(op, "ori") == 0)  code = encode_i(0x0d, get_register(r2), get_register(r1), atoi(r3));
        
        else if (strcmp(op, "lw") == 0 || strcmp(op, "sw") == 0) {
        
            int offset; char base[20];
            
            // Parse memory address format: offset($register)
            sscanf(r2, "%d($%[^)])", &offset, base);
            char full_base[25]; sprintf(full_base, "$%s", base);
            int op_code = (strcmp(op, "lw") == 0) ? 0x23 : 0x2b;
            code = encode_i(op_code, get_register(full_base), get_register(r1), offset);
            
        } else if (strcmp(op, "beq") == 0) {
            // Branch encoding requires calculating distance from PC+4 to target
            int offset = (get_label_address(r3) - (current_addr + 4)) / 4;
            code = encode_i(0x04, get_register(r1), get_register(r2), offset);
            
        } else if (strcmp(op, "j") == 0) {
            // Jump encoding uses the target address directly
            code = encode_j(0x02, get_label_address(r1));
        }

        // Clean instruction string for output display
        instructions[i][strcspn(instructions[i], "\r\n")] = 0;
        
        // Print the 3-column result: Address, Machine Code, Original Assembly Line
        printf("0x%08X 0x%08X         %s\n", current_addr, code, instructions[i]);
        current_addr += 4;
    }
    printf("\n");
    
    
    // Cleanup and close file
    fclose(fp);
    return 0;
}
