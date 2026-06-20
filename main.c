#include "calculadora.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char *nomeTeste;
    char *posFixa;
    char *esperadoInFixa;
    float esperadoValor;
    int esperaErro; 
} TestCase;

int floatIgual(float a, float b) {
    return fabs(a - b) <= 0.001;
}

int main() {
    TestCase testes[20] = {
        {"Tabela 1 - Basico", "3 4 + 5 *", "(3+4)*5", 35.0, 0},
        {"Tabela 2 - Sem parenteses", "7 2 * 4 +", "7*2+4", 18.0, 0},
        {"Tabela 3 - Precedencia natural", "8 5 2 4 + * +", "8+5*(2+4)", 38.0, 0}, 
        {"Tabela 4 - Divisao", "6 2 / 3 + 4 *", "(6/2+3)*4", 24.0, 0},
        {"Tabela 5 - Monstro da precedencia", "9 5 2 8 * 4 + * +", "9+5*(2*8+4)", 109.0, 0}, 
        {"Tabela 6 - Logaritmo", "2 3 + log 5 /", "log(2+3)/5", 0.139794, 0},
        {"Tabela 7 - Potencia", "10 log 3 ^ 2 +", "log(10)^3+2", 3.0, 0},
        {"Tabela 8 - Cosseno", "45 60 + 30 cos *", "(45+60)*cos(30)", 90.9326, 0},
        {"Tabela 9 - Seno e Float", "0.5 45 sen 2 ^ +", "0.5+sen(45)^2", 1.0, 0},
        
        {"Extra 10 - Tangente", "60 tg 2 *", "tg(60)*2", 3.4641, 0},
        {"Extra 11 - Raiz Quadrada", "16 raiz 2 /", "raiz(16)/2", 2.0, 0},
        {"Extra 12 - Resto de Divisao (%)", "10 3 %", "10%3", 1.0, 0},
        {"Extra 13 - Resto e Soma", "10 3 % 5 +", "10%3+5", 6.0, 0},
        {"Extra 14 - Subtracao aninhada", "10 5 - 2 ^", "(10-5)^2", 25.0, 0},
        {"Extra 15 - Multiplas Funcoes", "100 log 9 raiz +", "log(100)+raiz(9)", 5.0, 0},

        {"Erro 16 - Divisao por zero", "5 0 /", "5/0", 0.0, 1},
        {"Erro 17 - Resto por zero", "5 0 %", "5%0", 0.0, 1},
        {"Erro 18 - Raiz Negativa", "-4 raiz", "raiz(-4)", 0.0, 1},
        {"Erro 19 - Log de Zero", "0 log", "log(0)", 0.0, 1},
        {"Erro 20 - Log Negativo", "-10 log", "log(-10)", 0.0, 1}
    };

    int total_testes = 20;
    int acertos = 0;

    printf("======================================\n");
    printf("   AVALIADOR DE EXPRESSOES - TP03\n");
    printf("======================================\n\n");

    for (int i = 0; i < total_testes; i++) {
        printf("--- Teste %d: %s ---\n", i + 1, testes[i].nomeTeste);
        printf("Entrada (Pos-Fixa): %s\n", testes[i].posFixa);

        // --- AVALIANDO INFIXA ---
        char *str_infixa = getInFixa(testes[i].posFixa);
        int infixa_ok = 0;
        
        if (testes[i].esperaErro) {
            if (str_infixa == NULL) {
                printf("[OK] Conversao bloqueada corretamente (Erro Matematico esperado).\n");
                infixa_ok = 1;
            } else {
                printf("[FALHA] Conversao gerou texto, mas deveria ter retornado NULL.\n");
            }
        } else {
            if (str_infixa != NULL && strcmp(str_infixa, testes[i].esperadoInFixa) == 0) {
                printf("[OK] Conversao Infixa correta: %s\n", str_infixa);
                infixa_ok = 1;
            } else {
                printf("[FALHA] Conversao Infixa.\n");
                printf("  -> Obtido: %s\n", str_infixa != NULL ? str_infixa : "NULL");
                printf("  -> Esperado: %s\n", testes[i].esperadoInFixa);
            }
        }

        // --- AVALIANDO VALOR ---
        float valor_obtido = getValor(testes[i].posFixa);
        int valor_ok = 0;

        if (floatIgual(valor_obtido, testes[i].esperadoValor)) {
            printf("[OK] Resultado Matematico: %.4f\n", valor_obtido);
            valor_ok = 1;
        } else {
            printf("[FALHA] Resultado Matematico.\n");
            printf("  -> Obtido: %.4f\n", valor_obtido);
            printf("  -> Esperado: %.4f\n", testes[i].esperadoValor);
        }

        if (infixa_ok && valor_ok) {
            acertos++;
        }
        
        printf("\n"); // Espaço em branco entre um teste e outro para facilitar a leitura
    }

    printf("======================================\n");
    printf(" RESULTADO FINAL\n");
    printf("======================================\n");
    printf(" Testes Realizados : %d\n", total_testes);
    printf(" Total de Acertos  : %d\n", acertos);
    printf(" Desempenho        : %.2f%%\n", ((float)acertos / total_testes) * 100);
    printf("======================================\n");

    return 0;
}