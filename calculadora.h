#ifndef EXPRESSAO_H 
#define EXPRESSAO_H 
typedef struct { 
char posFixa[512];     
char inFixa[512];      
float Valor;           
} Expressao; 
// Expressão na forma pos-fixa, como 3 12 4 + * 
// Expressão na forma infixa, como 3*(12+4) 
// Valor numérico da expressão   
char * getInFixa(char *Str); // Retorna a forma inFixa de Str (posFixa) 
float getValor (char *Str); // Calcula o valor de Str (na forma posFixa) 
#endif 