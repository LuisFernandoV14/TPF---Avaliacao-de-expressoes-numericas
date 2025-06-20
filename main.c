#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "calculadora.h"

void menu() {
    printf("\n========== AVALIADOR DE EXPRESSOES ==========\n");
    printf("1. Avaliar expressao INFIXADA\n");
    printf("2. Avaliar expressao POS-FIXADA\n");
    printf("3. Rodar testes automaticos\n");
    printf("0. Sair\n");
    printf("==============================================\n");
    printf("Digite o numero da opcao: ");
}

void testarExpressaoInfixa() {
    char entrada[512];
    printf("\nDigite a expressao INFIXADA (ex: (3 + 4) * 5):\n> ");
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0'; // remove o \n do final

    char posfixa[512];
    strcpy(posfixa, getFormaPosFixa(entrada));

    printf("\n=== Resultado ===\n");
    printf("Forma pos-fixada: %s\n", posfixa);
    printf("Resultado: %.4f\n", getValorInFixa(entrada));
    printf("=================\n");
}

void testarExpressaoPosfixa() {
    char entrada[512];
    printf("\nDigite a expressao POS-FIXADA (ex: 3 4 + 5 *):\n> ");
    fgets(entrada, sizeof(entrada), stdin);
    entrada[strcspn(entrada, "\n")] = '\0'; 

    printf("\n=== Resultado ===\n");
    printf("Forma infixa: %s\n", getFormaInFixa(entrada));
    printf("Resultado: %.4f\n", getValorPosFixa(entrada));
    printf("=================\n");
}

void testesAutomaticos() {
    printf("\n=========== TESTAR AUTOMATICAMENTE ===========\n");

    char *testes[][3] = {
        {"3 4 + 5 *", "(3 + 4) * 5", "35.00"},
        {"7 2 * 4 +", "(7 * 2) + 4", "18.00"},
        {"8 5 2 4 + * +", "8 + 5 * (2 + 4)", "38.00"},
        {"6 2 / 3 + 4 *", "(6 / 2 + 3) * 4", "24.00"},
        {"9 5 2 8 * 4 + * +", "9 + 5 * (2 + 8 * 4)", "109.00"},
        {"2 3 + log 5 /", "log(2 + 3) / 5", "0.14"},
        {"10 log 3 ^ 2 +", "(log10)^3 + 2", "3.00"},
        {"45 60 + 30 cos *", "(45 + 60) * cos(30)", "90.93"},
        {"0.5 45 sen 2 ^ +", "0.5 + sen(45)^2", "1.00"}
    };

    const double margemErro = 0.01;

    for (int i = 0; i < 9; i++) {
        printf("\n------ Teste %d ------\n", i + 1);
        printf("Expressao pos-fixada: %s\n", testes[i][0]);

        char *infixa = getFormaInFixa(testes[i][0]);
        float valorCalculado = getValorPosFixa(testes[i][0]);
        float valorEsperado = atof(testes[i][2]);

        printf("Forma infixa esperada: %s\n", testes[i][1]);
        printf("Forma infixa obtida   : %s\n", infixa);
        printf("Valor esperado         : %.4f\n", valorEsperado);
        printf("Valor calculado        : %.4f\n", valorCalculado);

        if (fabs(valorCalculado - valorEsperado) < margemErro) {
            printf("? Resultado correto!\n");
        } else {
            printf("? Resultado incorreto!\n");
        }
    }
    printf("\n===========================================\n");
}

int main() {
    int opcao;
    do {
        menu();
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }
        getchar();

        switch (opcao) {
            case 1:
                testarExpressaoInfixa();
                break;
            case 2:
                testarExpressaoPosfixa();
                break;
            case 3:
                testesAutomaticos();
                break;
            case 0:
                printf("\nEncerrando...\n");
                break;
            default:
                printf("\n? Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
