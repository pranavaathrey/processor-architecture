#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define RESERVE 8 // lines to reserve (so labels don't conflict with registers)

int isDecimal(const char *str) {
    for (int i = 0; str[i]; i++) 
        if (!isdigit((unsigned char)str[i])) 
            return 0;
    return 1;
}

void decimalToHex(const char *decimal, char *hex_out) {
    int value = atoi(decimal);
    sprintf(hex_out, "%02X", value); 
}

// look up table for keywords
typedef struct {
    const char *word;
    const char *code;
} Mapping;

Mapping table[] = {
    //------------> OPCODES <------------//

    // memory management
    {"copy", "40"},         // 0100 0000
    {"copy_i", "C0"},       // 1100 0000
    
    {"ram_load", "3F"},     // 0011 1111
    {"ram_save", "3E"},     // 0011 1110

    {"push", "3B"},         // 0011 1011
    {"pop", "3C"},          // 0011 1100

    {"jump", "E0"},         // 1110 0000
    {"call", "38"},         // 0011 1000
    {"return", "39"},       // 0011 1101
    {"end", "3A"},          // 0011 1110

    // arithmetic
    {"add", "00"},          // 0000 0000
    {"add_ii", "C0"},       // 1100 0000
    {"add_ir", "80"},       // 1000 0000
    {"add_ri", "40"},       // 0100 0000
    
    {"sub", "01"},          // 0000 0001
    {"sub_ii", "C1"},       // 1100 0001
    {"sub_ir", "81"},       // 1000 0001
    {"sub_ri", "41"},       // 0100 0001

    {"mult", "0E"},         // 0000 1110
    {"mult_ii", "CE"},      // 1100 1110
    {"mult_ir", "8E"},      // 1000 1110
    {"mult_ri", "4E"},      // 0100 1110

    {"mult_hi_byte", "0F"}, // 0000 1111

    {"div", "10"},          // 0001 0000
    {"div_ii", "D0"},       // 1101 0000
    {"div_ir", "90"},       // 1001 0000
    {"div_ri", "50"},       // 0101 0000

    {"mod", "11"},          // 0001 0001
    {"mod_ii", "D1"},       // 1101 0001
    {"mod_ir", "91"},       // 1001 0001
    {"mod_ri", "51"},       // 0101 0001

    // logical
    {"AND", "02"},          // 0000 0010
    {"AND_ii", "C2"},       // 1100 0010
    {"AND_ir", "82"},       // 1000 0010
    {"AND_ri", "42"},       // 0100 0010
    
    {"OR", "03"},           // 0000 0011
    {"OR_ii", "C3"},        // 1100 0011
    {"OR_ir", "83"},        // 1000 0011
    {"OR_ri", "43"},        // 0100 0011

    {"NOT", "04"},          // 0000 0100
    {"NOT_ii", "C4"},       // 1100 0100
    {"NOT_ir", "84"},       // 1000 0100
    {"NOT_ri", "44"},       // 0100 0100

    {"XOR", "05"},          // 0000 0101
    {"XOR_ii", "C5"},       // 1100 0101
    {"XOR_ir", "85"},       // 1000 0101
    {"XOR_ri", "45"},       // 0100 0101

    // conditional
    {"if_equal", "20"},     // 0010 0000
    {"if_equal_ii", "E0"},  // 1110 0000
    {"if_equal_ir", "A0"},  // 1010 0000
    {"if_equal_ri", "60"},  // 0110 0000

    {"if_nt_eql", "21"},    // 0010 0001
    {"if_nt_eql_ii", "E1"}, // 1110 0001
    {"if_nt_eql_ir", "A1"}, // 1010 0001
    {"if_nt_eql_ri", "61"}, // 0110 0001

    // unsigned conditionals
    {"if_less", "22"},      // 0010 0010
    {"if_less_ii", "E2"},   // 1110 0010
    {"if_less_ir", "A2"},   // 1010 0010
    {"if_less_ri", "62"},   // 0110 0010

    {"if_le_eq", "23"},     // 0010 0011
    {"if_le_eq_ii", "E3"},  // 1110 0011
    {"if_le_eq_ir", "A3"},  // 1010 0011
    {"if_le_eq_ri", "63"},  // 0110 0011

    {"if_more", "24"},      // 0010 0100
    {"if_more_ii", "E4"},   // 1110 0100
    {"if_more_ir", "A4"},   // 1010 0100
    {"if_more_ri", "64"},   // 0110 0100

    {"if_mo_eq", "25"},     // 0010 0101
    {"if_mo_eq_ii", "E5"},  // 1110 0101
    {"if_mo_eq_ir", "A5"},  // 1010 0101
    {"if_mo_eq_ri", "65"},  // 0110 0101

    // signed conditionals
    {"if_less_s", "26"},    // 0010 0110
    {"if_less_s_ii", "E6"}, // 1110 0110
    {"if_less_s_ir", "A6"}, // 1010 0110
    {"if_less_s_ri", "66"}, // 0110 0110

    {"if_le_eq_s", "27"},   // 0010 0111
    {"if_le_eq_s_ii", "E7"},// 1110 0111
    {"if_le_eq_s_ir", "A7"},// 1010 0111
    {"if_le_eq_s_ri", "67"},// 0110 0111

    {"if_more_s", "28"},    // 0010 1000
    {"if_more_s_ii", "E8"}, // 1110 1000
    {"if_more_s_ir", "A8"}, // 1010 1000
    {"if_more_s_ri", "68"}, // 0110 1000

    {"if_mo_eq_s", "29"},   // 0010 1001
    {"if_mo_eq_s_ii", "E9"},// 1110 1001
    {"if_mo_eq_s_ir", "A9"},// 1010 1001
    {"if_mo_eq_s_ri", "69"},// 0110 1001
    

    //------------> FILLER <------------//

    {"from", "00"},         // 0000 0000
    {"to", "00"},           // 0000 0000
    {"upon", "00"},         // 0000 0000

    {"loc", "00"},          // 0000 0000
    {"location", "00"},     // 0000 0000

    {"function", "00"},     // 0000 0000    
    {"stack", "00"},        // 0000 0000


    //------------> ADDRESSES <------------//

    {"ram_address", "00"},  // 0000 0000
    {"counter", "06"},      // 0000 0110

    {"input", "07"},        // 0000 0111
    {"output", "07"},       // 0000 0111

    {"reg_0", "00"},        // 0000 0000 -- UNUSED
    {"reg_1", "01"},        // 0000 0001
    {"reg_2", "02"},        // 0000 0010
    {"reg_3", "03"},        // 0000 0011
    {"reg_4", "04"},        // 0000 0100
    {"reg_5", "05"},        // 0000 0101

    // end of the mapping
    {NULL, NULL}
};

const char* lookup(const char *word) {
    for (int i = 0; table[i].word != NULL; i++) {
        if (strcmp(word, table[i].word) == 0) 
            return table[i].code;
    }
    return NULL;
}

// structure for labels
typedef struct {
    char name[64];
    int linePos;
} Label;

#define MAX_LABELS 256
Label labels[MAX_LABELS];
int labelCount = 0;

int findLabel(const char *name) {
    for (int i = 0; i < labelCount; i++) {
        if (strcmp(labels[i].name, name) == 0)
            return labels[i].linePos;
    }
    return -1; // not found
}

int main() {
    FILE *in = fopen("instructions.txt", "r");
    FILE *out = fopen("binary.bin", "wb");

    if (!in || !out) {
        perror("File error");
        return 1;
    }

    char line[512];
    int byteOffset = RESERVE * 4; // reserve first few lines

    // ---------- FIRST PASS ---------- (get all label locations)
    while (fgets(line, sizeof(line), in)) {
        // strip leading whitespace
        char *trimmed = line;
        while (isspace((unsigned char)*trimmed)) trimmed++;

        if (strncmp(trimmed, "//", 2) == 0 
        || strncmp(trimmed, "#", 1) == 0)
            continue;

        char *token = strtok(line, " \t\r\n");
        if (!token) continue;

        if (strcmp(token, "label") == 0) {
            // store label name at current byte offset
            char *labelName = strtok(NULL, " \t\r\n");
            if (labelName && labelCount < MAX_LABELS) {
                strcpy(labels[labelCount].name, labelName);
                labels[labelCount].linePos = byteOffset / 4;
                labelCount++;
            }
            continue; // skip generating bytes
        }

        // simulate how many bytes this line will output
        while (token) {
            const char *mapped = lookup(token);
            if (mapped) {
                byteOffset++;
            } else if (isDecimal(token)) {
                byteOffset++;
            } else {
                // assume label reference = 1 byte
                byteOffset++;
            }
            token = strtok(NULL, " \t\r\n");
        }
    }

    // ---------- SECOND PASS ---------- (assemble given code)
    rewind(in);
    byteOffset = RESERVE * 4; // reset for second pass

    // jump to actual code
    unsigned char jumpInstr[4] = {
        0xE0, 0x00, 0x00, RESERVE
    };
    fwrite(jumpInstr, 1, 4, out);

    // write the dummy bytes (reserved space)
    for (int i = 4; i < RESERVE * 4; i++) {
        unsigned char zero = 0x00;
        fwrite(&zero, 1, 1, out);
    }

    while (fgets(line, sizeof(line), in)) {
        // strip leading whitespace
        char *trimmed = line;
        while (isspace((unsigned char)*trimmed)) trimmed++;

        if (strncmp(trimmed, "//", 2) == 0 
        || strncmp(trimmed, "#", 1) == 0)
            continue;

        char *token = strtok(line, " \t\r\n");
        if (!token) continue;

        if (strcmp(token, "label") == 0) {
            // don't emit labels to output
            continue;
        }

        while (token) {
            char hexCode[16];
            const char *mapped = lookup(token);

            if (mapped) 
                strcpy(hexCode, mapped);
            else if (isDecimal(token)) 
                decimalToHex(token, hexCode);
            else {
                // check if it's a label reference
                int addr = findLabel(token);
                if (addr != -1) 
                    sprintf(hexCode, "%02X", addr);
                else {
                    printf("Unknown instruction: %s\n", token);
                    strcpy(hexCode, "??");
                }
            }
            unsigned int byte;
            if (sscanf(hexCode, "%x", &byte) == 1) {
                unsigned char b = (unsigned char)byte;
                fwrite(&b, 1, 1, out);
                byteOffset++;
            }
            token = strtok(NULL, " \t\r\n");
        }
    }
    fclose(in);
    fclose(out);

    printf("Conversion complete. Output saved to binary.bin\n");
    return 0;
}