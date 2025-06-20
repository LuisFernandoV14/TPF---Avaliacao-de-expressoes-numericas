#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calculadora.h"
#include <ctype.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define MAX 512

typedef struct {
    float itens[MAX];
    int topo;
} PilhaFloat;

void inicializaPilhaFloat(PilhaFloat *p) {
    p->topo = -1;
}

int pilhaFloatVazia(PilhaFloat *p) {
    return p->topo == -1;
}

int empilhaFloat(PilhaFloat *p, float val) {
    if (p->topo >= MAX - 1) return 0;
    p->itens[++p->topo] = val;
    return 1;
}

float desempilhaFloat(PilhaFloat *p) {
    if (pilhaFloatVazia(p)) {
        printf("Erro: pilha vazia!\n");
        return 0;
    }
    return p->itens[p->topo--];
}

typedef struct {
    char itens[MAX][MAX];
    int topo;
} PilhaString;

void inicializaPilhaStr(PilhaString *p) {
    p->topo = -1;
}

int empilhaStr(PilhaString *p, char *val) {
    if (p->topo >= MAX - 1) return 0;
    strcpy(p->itens[++p->topo], val);
    return 1;
}

char *desempilhaStr(PilhaString *p) {
    if (p->topo < 0) {
        printf("Erro: pilha vazia!\n");
        return NULL;
    }
    return p->itens[p->topo--];
}

float grausParaRadianos(float graus) {
    return graus * M_PI / 180.0;
}

float getValorPosFixa(char *StrPosFixa) {
    PilhaFloat p;
    inicializaPilhaFloat(&p);

    char copia[MAX];
    strcpy(copia, StrPosFixa);

    char *token = strtok(copia, " ");

    while (token != NULL) {
        if (strcmp(token, "+") == 0) {
            float b = desempilhaFloat(&p);
            float a = desempilhaFloat(&p);
            empilhaFloat(&p, a + b);
        } else if (strcmp(token, "-") == 0) {
            float b = desempilhaFloat(&p);
            float a = desempilhaFloat(&p);
            empilhaFloat(&p, a - b);
        } else if (strcmp(token, "*") == 0) {
            float b = desempilhaFloat(&p);
            float a = desempilhaFloat(&p);
            empilhaFloat(&p, a * b);
        } else if (strcmp(token, "/") == 0) {
            float b = desempilhaFloat(&p);
            float a = desempilhaFloat(&p);
            if (b == 0) {
                printf("Erro: divisao por zero!\n");
                return INFINITY;
            }
            empilhaFloat(&p, a / b);
        } else if (strcmp(token, "%") == 0) {
            float b = desempilhaFloat(&p);
            float a = desempilhaFloat(&p);
            if (b == 0) {
                printf("Erro: modulo por zero!\n");
                return INFINITY;
            }
            empilhaFloat(&p, fmod(a, b));
        } else if (strcmp(token, "^") == 0) {
            float b = desempilhaFloat(&p);
            float a = desempilhaFloat(&p);
            empilhaFloat(&p, pow(a, b));
        } else if (strcmp(token, "raiz") == 0) {
            float a = desempilhaFloat(&p);
            if (a < 0) {
                printf("Erro: raiz de numero negativo!\n");
                return NAN;
            }
            empilhaFloat(&p, sqrt(a));
        } else if (strcmp(token, "log") == 0) {
            float a = desempilhaFloat(&p);
            if (a <= 0) {
                printf("Erro: logaritmo de numero nao positivo!\n");
                return NAN;
            }
            empilhaFloat(&p, log10(a));
        } else if (strcmp(token, "sen") == 0) {
            float a = desempilhaFloat(&p);
            empilhaFloat(&p, sin(grausParaRadianos(a)));
        } else if (strcmp(token, "cos") == 0) {
            float a = desempilhaFloat(&p);
            empilhaFloat(&p, cos(grausParaRadianos(a)));
        } else if (strcmp(token, "tg") == 0) {
            float a = desempilhaFloat(&p);
            double cosValor = cos(grausParaRadianos(a));
            if (fabs(cosValor) < 1e-10) {
                printf("Erro: tangente indefinida para %.2f graus!\n", a);
                return INFINITY;
            }
            empilhaFloat(&p, tan(grausParaRadianos(a)));
        } else {
            empilhaFloat(&p, atof(token));
        }

        token = strtok(NULL, " ");
    }

    if (p.topo != 0) {
        printf("Erro: expressao mal formada!\n");
        return NAN;
    }

    return desempilhaFloat(&p);
}

float getValorInFixa(char *StrInFixa) {
    char posfixa[MAX];
    strcpy(posfixa, getFormaPosFixa(StrInFixa));
    return getValorPosFixa(posfixa);
}

int prioridade(char *op) {
    if (strcmp(op, "sen") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tg") == 0 ||
        strcmp(op, "log") == 0 || strcmp(op, "raiz") == 0)
        return 5;
    if (strcmp(op, "^") == 0) return 4;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) return 3;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 2;
    if (strcmp(op, "(") == 0) return 1;
    return 0;
}

// Coloque esta função no lugar da getFormaPosFixa() original em expressao.c

char *getFormaPosFixa(char *StrInFixa) {
    static char resultado[MAX] = "";
    resultado[0] = '\0';

    PilhaString operadores;
    inicializaPilhaStr(&operadores);

    int i = 0;
    while (StrInFixa[i] != '\0') {
        // Ignora espaços em branco
        if (isspace((unsigned char)StrInFixa[i])) {
            i++;
            continue;
        }

        // Se for um número (pode ter vários dígitos ou ser decimal)
        if (isdigit((unsigned char)StrInFixa[i]) || (StrInFixa[i] == '.' && isdigit((unsigned char)StrInFixa[i+1]))) {
            char numero[MAX];
            int k = 0;
            while (isdigit((unsigned char)StrInFixa[i]) || StrInFixa[i] == '.') {
                numero[k++] = StrInFixa[i++];
            }
            numero[k] = '\0';
            strcat(resultado, numero);
            strcat(resultado, " ");
            continue; // Continua a análise da string de entrada
        }

        char token[MAX];

        // Se for uma função (log, sen, cos, etc.)
        if (isalpha((unsigned char)StrInFixa[i])) {
            int k = 0;
            while (isalpha((unsigned char)StrInFixa[i])) {
                token[k++] = StrInFixa[i++];
            }
            token[k] = '\0';
        } else { // Se for um operador de um caractere ou parêntese
            token[0] = StrInFixa[i++];
            token[1] = '\0';
        }

        // Agora, 'token' contém um operador ou parêntese. Vamos processá-lo.
        if (strcmp(token, "(") == 0) {
            empilhaStr(&operadores, token);
        } else if (strcmp(token, ")") == 0) {
            while (operadores.topo >= 0 && strcmp(operadores.itens[operadores.topo], "(") != 0) {
                strcat(resultado, desempilhaStr(&operadores));
                strcat(resultado, " ");
            }
             if (operadores.topo < 0) { // Erro de parênteses
                printf("Erro: parenteses desalinhados na expressao!\n");
                resultado[0] = '\0';
                return resultado;
            }
            desempilhaStr(&operadores); // Descarta o "("
        } else { // É um operador
            while (operadores.topo >= 0 && strcmp(operadores.itens[operadores.topo], "(") != 0 &&
                   prioridade(token) <= prioridade(operadores.itens[operadores.topo])) {
                strcat(resultado, desempilhaStr(&operadores));
                strcat(resultado, " ");
            }
            empilhaStr(&operadores, token);
        }
    }

    // Desempilha os operadores restantes
    while (operadores.topo >= 0) {
        if (strcmp(operadores.itens[operadores.topo], "(") == 0) {
             printf("Erro: parenteses desalinhados na expressao!\n");
             resultado[0] = '\0';
             return resultado;
        }
        strcat(resultado, desempilhaStr(&operadores));
        strcat(resultado, " ");
    }

    // Remove o espaço extra no final, se houver
    int len = strlen(resultado);
    if (len > 0 && resultado[len - 1] == ' ') {
        resultado[len - 1] = '\0';
    }

    return resultado;
}

char *getFormaInFixa(char *StrPosFixa) {
    static char resultado[MAX] = "";
    resultado[0] = '\0';

    PilhaString pilha;
    inicializaPilhaStr(&pilha);

    char copia[MAX];
    strcpy(copia, StrPosFixa);

    char *token = strtok(copia, " ");

    while (token != NULL) {
        if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
            strcmp(token, "%") == 0 || strcmp(token, "^") == 0) {
            
            char *b = desempilhaStr(&pilha);
            char *a = desempilhaStr(&pilha);
            char temp[MAX];
            snprintf(temp, MAX, "(%s %s %s)", a, token, b);
            empilhaStr(&pilha, temp);

        } else if (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 ||
                   strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 ||
                   strcmp(token, "raiz") == 0) {

            char *a = desempilhaStr(&pilha);
            char temp[MAX];
            snprintf(temp, MAX, "%s(%s)", token, a);
            empilhaStr(&pilha, temp);

        } else {
            empilhaStr(&pilha, token);
        }

        token = strtok(NULL, " ");
    }

    strcpy(resultado, desempilhaStr(&pilha));
    return resultado;
}
