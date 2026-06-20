
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define PI 3.14159265358979323846

//definicao da pilha para os calculos
typedef struct Pilha {
    float Vetor[512];
    int topo;
} Pilha;

void inicioPilha(Pilha *s) {
    s->topo = -1;
}

void pushFloat(Pilha *s, float val) {
    s->Vetor[++(s->topo)] = val;
}

float popFloat(Pilha *s, int *error) {
    if (s->topo < 0) {
        *error = 1;
        return 0;
    }
    return s->Vetor[(s->topo)--];
}

//definicao da pilha para a conversao
typedef struct {
    char Vetor[512];
    int prec;//1 para + ou -, 2 para *,( e %, 3 para ^, 4 para operacoes com um operando e numeros
} TokenInfixo;

typedef struct PilhaStr {
    TokenInfixo Vetor[512];
    int topo;
} PilhaStr;

void initStackStr(PilhaStr *s) {
    s->topo = -1;
}

void pushStr(PilhaStr *s, const char *str, int prec) {
    s->topo++;
    strcpy(s->Vetor[s->topo].Vetor, str);
    s->Vetor[s->topo].prec = prec;
}

TokenInfixo popStr(PilhaStr *s, int *error) {
    TokenInfixo vazio = {"", 0};
    if (s->topo < 0) {
        *error = 1;
        return vazio;
    }
    return s->Vetor[(s->topo)--];
}

//verifica se a string esta valida
int isOperator(const char *token) {
    if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
        strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
        strcmp(token, "%") == 0 || strcmp(token, "^") == 0 ||
        strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
        strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
        strcmp(token, "log") == 0) {
        return 1;
    }
    return 0;
}

//verifica se a funcao so tem um operando
int isUnary(const char *token) {
    if (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
        strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
        strcmp(token, "log") == 0) {
        return 1;
    }
    return 0;
}

// Retorna a precedência do operador
int getPrecedencia(const char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 2;
    if (strcmp(op, "^") == 0) return 3;
    return 4; // Funções unárias e números isolados
}

float getValor(char *Str) {
    if (!Str || strlen(Str) == 0) return 0.0;

    Pilha s;
    inicioPilha(&s);

    char strCopy[512];
    strcpy(strCopy, Str);

    int error = 0;
    char *token = strtok(strCopy, " ");

    while (token != NULL && !error) {
        if (!isOperator(token)) {
            char *endptr;
            float val = strtof(token, &endptr);
            pushFloat(&s, val);
        } else {
            if (isUnary(token)) {
                float op = popFloat(&s, &error);
                if (error) break;

                if (strcmp(token, "raiz") == 0) {
                    if (op < 0) { error = 1; break; }
                    pushFloat(&s, sqrtf(op));
                } else if (strcmp(token, "sen") == 0) {
                    pushFloat(&s, sinf(op * PI / 180.0));
                } else if (strcmp(token, "cos") == 0) {
                    pushFloat(&s, cosf(op * PI / 180.0));
                } else if (strcmp(token, "tg") == 0) {
                    pushFloat(&s, tanf(op * PI / 180.0));
                } else if (strcmp(token, "log") == 0) {
                    if (op <= 0) { error = 1; break; }
                    pushFloat(&s, log10f(op));
                }
            } else {
                float op2 = popFloat(&s, &error);
                float op1 = popFloat(&s, &error);
                if (error) break;

                if (strcmp(token, "+") == 0) {
                    pushFloat(&s, op1 + op2);
                } else if (strcmp(token, "-") == 0) {
                    pushFloat(&s, op1 - op2);
                } else if (strcmp(token, "*") == 0) {
                    pushFloat(&s, op1 * op2);
                } else if (strcmp(token, "/") == 0) {
                    if (op2 == 0) { error = 1; break; }
                    pushFloat(&s, op1 / op2);
                } else if (strcmp(token, "%") == 0) {
                    if (op2 == 0) { error = 1; break; }
                    pushFloat(&s, fmodf(op1, op2));
                } else if (strcmp(token, "^") == 0) {
                    pushFloat(&s, powf(op1, op2));
                }
            }
        }
        token = strtok(NULL, " ");
    }

    if (error || s.topo != 0) {
        return 0.0;
    }

    return s.Vetor[0];
}

char * getInFixa(char *Str) {
    if (!Str || strlen(Str) == 0) return NULL;

    PilhaStr s;
    initStackStr(&s);

    // Pilha numérica paralela para validar erros matemáticos invisíveis (Testes 14 a 17)
    Pilha sNum;
    inicioPilha(&sNum);

    char strCopy[512];
    strcpy(strCopy, Str);

    int error = 0;
    char *token = strtok(strCopy, " ");
    static char resultado[512];

    while (token != NULL && !error) {
        if (!isOperator(token)) {
            // Empilha o texto
            pushStr(&s, token, 4);
            
            // Empilha o valor numérico para a validação
            char *endptr;
            float val = strtof(token, &endptr);
            pushFloat(&sNum, val);
        } else {
            char tempExpr[512];
            int precAtual = getPrecedencia(token);

            if (isUnary(token)) {
                TokenInfixo op = popStr(&s, &error);
                float opNum = popFloat(&sNum, &error); // Resgata o número da pilha paralela
                if (error) break;

                // Validações Matemáticas Unárias
                if (strcmp(token, "raiz") == 0) {
                    if (opNum < 0) { error = 1; break; }
                    pushFloat(&sNum, sqrtf(opNum));
                } else if (strcmp(token, "log") == 0) {
                    if (opNum <= 0) { error = 1; break; }
                    pushFloat(&sNum, log10f(opNum));
                } else if (strcmp(token, "sen") == 0) {
                    pushFloat(&sNum, sinf(opNum * PI / 180.0));
                } else if (strcmp(token, "cos") == 0) {
                    pushFloat(&sNum, cosf(opNum * PI / 180.0));
                } else if (strcmp(token, "tg") == 0) {
                    pushFloat(&sNum, tanf(opNum * PI / 180.0));
                }

                sprintf(tempExpr, "%s(%s)", token, op.Vetor);
                pushStr(&s, tempExpr, 4);
            } else {
                TokenInfixo op2 = popStr(&s, &error);
                TokenInfixo op1 = popStr(&s, &error);
                
                float op2Num = popFloat(&sNum, &error);
                float op1Num = popFloat(&sNum, &error);
                if (error) break;

                // Validações Matemáticas Binárias (Bloqueio de divisão por zero)
                if (strcmp(token, "/") == 0 || strcmp(token, "%") == 0) {
                    if (op2Num == 0) { error = 1; break; }
                }

                // Simula o cálculo na pilha auxiliar para uso futuro nas contas empilhadas
                if (strcmp(token, "+") == 0) pushFloat(&sNum, op1Num + op2Num);
                else if (strcmp(token, "-") == 0) pushFloat(&sNum, op1Num - op2Num);
                else if (strcmp(token, "*") == 0) pushFloat(&sNum, op1Num * op2Num);
                else if (strcmp(token, "/") == 0) pushFloat(&sNum, op1Num / op2Num);
                else if (strcmp(token, "%") == 0) pushFloat(&sNum, fmodf(op1Num, op2Num));
                else if (strcmp(token, "^") == 0) pushFloat(&sNum, powf(op1Num, op2Num));

                char strOp1[512];
                char strOp2[512];

                if (op1.prec < precAtual) {
                    sprintf(strOp1, "(%s)", op1.Vetor);
                } else {
                    strcpy(strOp1, op1.Vetor);
                }

                int precisaParentesesDir = 0;
                if (op2.prec < precAtual) {
                    precisaParentesesDir = 1;
                } else if (op2.prec == precAtual) {
                    if (strcmp(token, "-") == 0 || strcmp(token, "/") == 0 || strcmp(token, "%") == 0) {
                        precisaParentesesDir = 1;
                    }
                }

                if (precisaParentesesDir) {
                    sprintf(strOp2, "(%s)", op2.Vetor);
                } else {
                    strcpy(strOp2, op2.Vetor);
                }

                sprintf(tempExpr, "%s%s%s", strOp1, token, strOp2);
                pushStr(&s, tempExpr, precAtual);
            }
        }
        token = strtok(NULL, " ");
    }

    if (error || s.topo != 0) {
        return NULL;
    }

    strcpy(resultado, s.Vetor[0].Vetor);
    return resultado;
}