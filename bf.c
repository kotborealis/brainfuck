#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DATA_SIZE 128

#define STACK_SIZE 512
static unsigned short STACK[STACK_SIZE];
static unsigned short SP = 0;
#define STACK_PUSH(A) (STACK[SP++] = A)
#define STACK_POP() (STACK[--SP])
#define STACK_EMPTY() (SP == 0)
#define STACK_FULL() (SP == STACK_SIZE)

#define PROGRAM_SIZE 4096

#define PERROR(A) (printf("Error at %hu: %s\n", pc, A))

typedef enum {
    op_ptr_inc, op_ptr_dec,
    op_data_inc, op_data_dec,
    op_out, op_in,
    op_jmp_fwd, op_jmp_bck,
    op_end
} bf_operator_type;

typedef struct {
    bf_operator_type type;
    unsigned short meta;
} bf_operator;

int main(int argc, char** argv){
    unsigned short data[sizeof(short) * DATA_SIZE] = {0};
    unsigned short* ptr = data;

    if(argc < 2){
        printf("Usage: bf ./asdasd.bf\n");
        return 0;
    }

    FILE* file = fopen(argv[1], "r");

    static bf_operator program[PROGRAM_SIZE];

    unsigned short pc = 0;
    unsigned short jmp_pc;

    //compile
    char c;
    while((c = getc(file)) != EOF && pc < PROGRAM_SIZE){
        switch(c){
            case '>':
                program[pc].type = op_ptr_inc;
                break;
            case '<':
                program[pc].type = op_ptr_dec;
                break;
            case '+':
                program[pc].type = op_data_inc;
                break;
            case '-':
                program[pc].type = op_data_dec;
                break;
            case '.':
                program[pc].type = op_out;
                break;
            case ',':
                program[pc].type = op_in;
                break;
            case '[':
                program[pc].type = op_jmp_fwd;
                if(STACK_FULL()){
                    PERROR("I'm performance artist. Fisting is 300 bucks");
                    return 1;
                }
                STACK_PUSH(pc);
                break;
            case ']':
                if(STACK_EMPTY()){
                    PERROR("Fuck you");
                    return 1;
                }
                jmp_pc = STACK_POP();
                program[pc].type = op_jmp_bck;
                program[pc].meta = jmp_pc;
                program[jmp_pc].meta = pc;
                break;
            default: 
                pc--;
                break;
        }
        pc++;
    }

    if(!STACK_EMPTY()){
        PERROR("You got the wrong [door] buddy");
        return 1;
    }
    if(pc == PROGRAM_SIZE){
        PERROR("It's so fucking big");
        return 1;
    }

    program[pc].type = op_end;
        
    //run
    pc = 0;
    while(program[pc].type != op_end){
        switch(program[pc].type){
            case op_ptr_inc:
                ++ptr;
                break;
            case op_ptr_dec:
                --ptr;
                break;
            case op_data_inc:
                ++*ptr;
                break;
            case op_data_dec:
                --*ptr;
                break;
            case op_out:
                putchar(*ptr);
                break;
            case op_in:
                *ptr = (unsigned short)getchar();
                break;
            case op_jmp_fwd:
                if(!*ptr) pc = program[pc].meta;
                break;
            case op_jmp_bck:
                if(*ptr)
                    pc = program[pc].meta;
            default:
                break;
        }
        pc++;
    }
    printf("\n");
}
