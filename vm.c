// Darin Doria and Jorge Berroa
// COP 3402 - Fall 2014
// HW3 - PL/0 Parser

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH  500
#define MAX_LEXI_LEVELS  3

struct instruction
{
	
	int op;
	int l;
	int m;
    
    
};

struct cpu
{
	int sp;
	int bp;
	int pc;
    int arbracket;

    int  * activation_record; 
	int  * stack;
	struct instruction  * code;
};


char* OPCODES[] =
{
  "",
  "lit",
  "opr",
  "lod",
  "sto",
  "cal",
  "inc",
  "jmp",
  "jpc",
  "soi",
  "sio","sio"
};



int base(int l, int base, struct cpu * cpu) // l stand for L in the instruction format
{
    int b1; //find base L levels down
    b1 = base;
    while (l > 0)
    {
        b1 = cpu->stack[b1 + 1];
        l--;
    }
    return b1;
}




void LIT(int l, int m , struct cpu * cpu) {
    cpu->sp += 1;
    cpu->stack[cpu->sp]= m;
    
}


void LOD(int l, int m , struct cpu * cpu) {
    cpu->sp+=1;
    cpu->stack[cpu->sp]= cpu->stack[base(l,cpu->bp,cpu)+m];
}

void INC(int l, int m , struct cpu * cpu) {
	cpu->sp += m;
    
}

void JMP(int l, int m , struct cpu * cpu) {
	cpu->pc = m;
}

void JPC(int l, int m , struct cpu * cpu) {
	if (cpu->stack[cpu->sp] == 0)
	{
		cpu->pc = m  ;
	}
    cpu->sp -=1 ;
}

void STO (int l, int m , struct cpu * cpu) {
	cpu->stack[base( l , cpu->bp ,cpu) + m ] = cpu->stack[cpu->sp];
	cpu->sp -=1;
}

void SIO_1(int l, int m , struct cpu * cpu) {
	
	printf("%d\n",cpu->stack[cpu->sp] );
	cpu->sp -= 1;
    
}

void CAL(int l, int m , struct cpu * cpu) {
    
	cpu->stack[cpu->sp + 1]=0;
	cpu->stack[cpu->sp + 2]= base( l , cpu->bp, cpu);
	cpu->stack[cpu->sp + 3]=cpu->bp;
	cpu->stack[cpu->sp + 4]= cpu->pc;
	cpu->bp = cpu->sp + 1;
	cpu->pc = m;

  cpu->activation_record[cpu->arbracket] = cpu->bp;
  cpu->arbracket+=1;
}



void SIO_2(int l, int m , struct cpu * cpu) {
    cpu->sp+=1;
    
	scanf("%d",&cpu->stack[cpu->sp]);
    
    
}

int SIO_3(int l , int m , struct cpu * cpu){
	return 0 ; // halt = 0
}

int OPR(int l, int m, struct cpu * cpu){
    
    
	switch(m){
            
		case  0:
              if(cpu->bp == 1 ){
                cpu->pc=0;
                cpu->sp=0;
                cpu->bp=0;
                    return 0;
                  }

        	   cpu->sp = cpu->bp -1;
            cpu->pc = cpu->stack[cpu->sp+4] ;
            cpu->bp = cpu->stack[cpu->sp+3];
            cpu->activation_record[cpu->arbracket]=cpu->bp;
            cpu->arbracket-=1;

            break;
            
            
		case  1:  	cpu->stack[cpu->sp] = cpu->stack[cpu->sp]* -1  ;
            break;
		case  2:  	cpu->sp -= 1;
            cpu->stack[cpu->sp]= cpu->stack[cpu->sp] + cpu->stack[cpu->sp +1];
            break;
            
            
		case  3:  	cpu->sp -= 1;
            cpu->stack[cpu->sp]= cpu->stack[cpu->sp] - cpu->stack[cpu->sp +1];
            break;
            
            
		case  4:  	cpu->sp -= 1;
            cpu->stack[cpu->sp]= cpu->stack[cpu->sp] * cpu->stack[cpu->sp +1];
            break;
            
            
		case  5:  	cpu->sp -= 1;
            cpu->stack[cpu->sp]= cpu->stack[cpu->sp] / cpu->stack[cpu->sp +1];
            break;
            
            
		case  6:  	cpu->stack[cpu->sp]= cpu->stack[cpu->sp] % 2;
            break;
            
            
		case  7:  	cpu->sp -= 1;
            cpu->stack[cpu->sp]= cpu->stack[cpu->sp] % cpu->stack[cpu->sp +1];
            break;
            
            
		case  8:  	cpu->sp -= 1;
            if (cpu->stack[cpu->sp] == cpu->stack[cpu->sp +1])
            {
                cpu->stack[cpu->sp]=1;
            }
            else
                cpu->stack[cpu->sp]=0;
            break;
            
            
		case  9:  	cpu->sp -= 1;
            if (cpu->stack[cpu->sp] != cpu->stack[cpu->sp +1])
            {
                cpu->stack[cpu->sp]=1;
            }
            else
                cpu->stack[cpu->sp]=0;break;
            
            
		case  10:  	cpu->sp -= 1;
            if (cpu->stack[cpu->sp] < cpu->stack[cpu->sp +1])
            {
                cpu->stack[cpu->sp]=1;
            }
            else
                cpu->stack[cpu->sp]=0;break;
            
            
		case  11:  	cpu->sp -= 1;
            if (cpu->stack[cpu->sp] <= cpu->stack[cpu->sp +1])
            {
                cpu->stack[cpu->sp]=1;
            }
            else
                cpu->stack[cpu->sp]=0;break;
            
            
		case  12:  	cpu->sp -= 1;
            if (cpu->stack[cpu->sp] > cpu->stack[cpu->sp +1])
            {
                cpu->stack[cpu->sp]=1;
            }
            else
                cpu->stack[cpu->sp]=0;	break;
            
            
		case  13:  cpu->sp -= 1;
            if (cpu->stack[cpu->sp] >= cpu->stack[cpu->sp +1])
            {
                cpu->stack[cpu->sp]=1;
            }
            else
                cpu->stack[cpu->sp]=0;	break;        
	}
    return 1;
    
}


int opcode(int op, int l, int m , struct cpu * cpu, FILE * stacktrace){
	
        
	switch(op){
            
        case 1:  LIT(0 , m,cpu)	; 	break;
        case 2:  return OPR(0, m, cpu);	break;
        case 3:  LOD(l, m, cpu)	;	break;
        case 4:  STO(l, m,cpu)	;	break;
        case 5:  CAL(l, m, cpu)	;	break;
        case 6:  INC(0,m,cpu)	;	break;
        case 7:  JMP(0, m, cpu)	;	break;
        case 8:  JPC(0, m , cpu);	break;
        case 9:  SIO_1(0, 1,cpu);	break;
        case 10: SIO_2(0, 2, cpu);	break;
        case 11: return SIO_3(0, 3, cpu);	break;
            
	}
	return 1;
    
}

void printFile(struct cpu * cpu, int numInstructions,FILE * stacktrace)
{
  int i;

  //Printing the header
  fprintf(stacktrace,"\nLine   OP   L   M\n");
  
  //Printing the lines
  for(i=0; i < numInstructions; i++){
    fprintf(stacktrace,"%4d   %s  %1d   %d\n", i, OPCODES[cpu->code[i+1].op],
           cpu->code[i+1].l, cpu->code[i+1].m);
  }

  fprintf(stacktrace,"\n");
}




void printState(int line, struct instruction op, int pc, int bp, int sp, int stack[], 
    int arlist[],FILE * stacktrace){
  int i, k;
   //Printing header, if necessary
  if(line < 0)
    fprintf(stacktrace,"                 %3s %3s %3s  Stack\n", "pc", "bp", "sp");

  //Printing the current line
  if(line < 0)
    fprintf(stacktrace,"Initial Values   ");
  else
    fprintf(stacktrace,"%3d %s %3d %3d  ", line, OPCODES[op.op], op.l, op.m);

  //Printing the register statuses
fprintf(stacktrace,"%3d %3d %3d  ", pc, bp, sp);

  //Printing the stack

  



if(sp>0){
  int k=0;
  for(i=1 ; i<=sp; i++){
    
    //Determining whether or not to insert a pipe before this element
    if(arlist[k] == i)
    {
      fprintf(stacktrace,"| ");
      k++;
    }

    fprintf(stacktrace,"%d ", stack[i]);
  }

}


    fprintf(stacktrace,"\n");
 }





int main(int argc, char const *argv[])
{
    FILE * ifp = fopen("mcode.txt", "r");
    FILE * stacktrace = fopen("stacktrace.txt","w");

    if (!ifp || !stacktrace)
    {
      printf("ERROR: Could not open mcode.txt\n");
      return -1;
    }

	struct cpu	* cpu = malloc(sizeof(struct cpu));
    
	cpu->code = malloc(sizeof(struct instruction) * MAX_CODE_LENGTH);
	cpu->stack = malloc(sizeof(int)*MAX_STACK_HEIGHT);
	cpu->stack = calloc(MAX_STACK_HEIGHT,sizeof(int));
    cpu->activation_record = malloc(sizeof(int)*MAX_STACK_HEIGHT);
    cpu->activation_record = calloc(MAX_STACK_HEIGHT,sizeof(int));
    cpu->arbracket=0;
    
    cpu->sp=0;
    cpu->bp=1;
    cpu->pc=0;
    cpu->arbracket = 0;
	int counter=0;
    
	while (!feof(ifp))
	{
    counter++;  
		fscanf(ifp,"%d",&cpu->code[counter].op);
		fscanf(ifp,"%d",&cpu->code[counter].l);
		fscanf(ifp,"%d",&cpu->code[counter].m);
		
	}

    printFile(cpu, counter-1, stacktrace);
    
    fclose(ifp);

        
    fprintf(stacktrace,"                 %3s %3s %3s  Stack\n", "pc", "bp", "sp");

  //Printing the current line
  
    fprintf(stacktrace,"Initial Values   ");
     fprintf(stacktrace,"%3d %3d %3d  ", cpu->pc, cpu->bp, cpu->sp);
     fprintf(stacktrace,"\n");

  
	int on = 1 ;
    
  int j = cpu->pc;
  int jump=0;
    
    	while(on)
	{			

            on = opcode(cpu->code[j+1].op , cpu->code[j+1].l, cpu->code[j+1].m,cpu, stacktrace);
          
                  

       
      printState(j,cpu->code[j+1] , cpu->pc, cpu->bp, cpu->sp, cpu->stack, cpu->activation_record, stacktrace);


      if(cpu->code[j+1].op==7)
        jump=1;     
      if(jump){
        j=cpu->pc;
        ++cpu->pc;
      }
      else
      if(cpu->code[j+1].m != cpu->pc ){

        j=cpu->pc+1;
        ++cpu->pc;
      }
      else{
        if (cpu->code[j+1].op==1)
        {
            j=cpu->pc+1;
        ++cpu->pc;
        }
        else{

          j=cpu->pc;
        cpu->pc++;

        }
      }

      
            
     
     		
  }
  int i;
  for(i=1; i<=10; i++){
    
    //Determining whether or not to insert a pipe before this element
  
      }
     fclose(stacktrace);
	
	return 0;
}