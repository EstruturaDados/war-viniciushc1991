#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 50
#define MAX_COR 30
#define NUM_TERRITORIOS 5
#define MAX_MISSAO 100

// Struct Territorio mantida dos níveis anteriores
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int numeroTropas;
} Territorio;

// NOVA: Struct para Missão
typedef struct {
    int tipo; // 1 = Destruir exército, 2 = Conquistar territórios
    char alvo[MAX_COR]; // Cor do exército a ser destruído
    int quantidade; // Quantidade de territórios a conquistar
    char descricao[MAX_MISSAO];
} Missao;

// PROTÓTIPOS DAS FUNÇÕES (Modularização total)
void inicializarTerritorios(Territorio *territorios);
void exibirMapa(const Territorio *territorios);
void simularAtaque(Territorio *atacante, Territorio *defensor, int idAtacante, int idDefensor);
void atribuirMissao(Missao *missao);
int verificarMissao(const Missao *missao, const Territorio *territorios);
void menuPrincipal(Territorio *territorios, Missao *missao);
void realizarAtaque(Territorio *territorios);

// FUNÇÃO: Inicialização automática dos territórios
void inicializarTerritorios(Territorio *territorios) {
    // Territórios pré-definidos para inicialização automática
    const char* nomes[NUM_TERRITORIOS] = {"Brasil", "Argentina", "Chile", "Peru", "Colombia"};
    const char* cores[NUM_TERRITORIOS] = {"Verde", "Azul", "Vermelho", "Amarelo", "Verde"};
    const int tropas[NUM_TERRITORIOS] = {10, 8, 6, 7, 9};
    
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(territorios[i].nome, nomes[i]);
        strcpy(territorios[i].corExercito, cores[i]);
        territorios[i].numeroTropas = tropas[i];
    }
}

// FUNÇÃO: Exibir mapa (com const correctness)
void exibirMapa(const Territorio *territorios) {
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    printf("%-3s %-20s %-15s %-10s\n", 
           "ID", "TERRITORIO", "COR DO EXERCITO", "TROPAS");
    printf("--------------------------------------------------------\n");
    
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-3d %-20s %-15s %-10d\n",
               i + 1,
               territorios[i].nome,
               territorios[i].corExercito,
               territorios[i].numeroTropas);
    }
    printf("\n");
}

// FUNÇÃO: Simular ataque (mantida do nível anterior com melhorias)
void simularAtaque(Territorio *atacante, Territorio *defensor, int idAtacante, int idDefensor) {
    printf("\n=== SIMULACAO DE BATALHA ===\n");
    printf("%s (ID: %d) ataca %s (ID: %d)!\n", 
           atacante->nome, idAtacante + 1, defensor->nome, idDefensor + 1);
    
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;
    
    printf("Dado de ataque: %d\n", dadoAtaque);
    printf("Dado de defesa: %d\n", dadoDefesa);
    
    if (dadoAtaque >= dadoDefesa) {
        defensor->numeroTropas--;
        printf("VITORIA DO ATACANTE! %s perde 1 tropa.\n", defensor->nome);
        
        if (defensor->numeroTropas <= 0) {
            printf("CONQUISTA! %s foi conquistado por %s!\n", 
                   defensor->nome, atacante->nome);
            strcpy(defensor->corExercito, atacante->corExercito);
            defensor->numeroTropas = 1;
        }
    } else {
        atacante->numeroTropas--;
        printf("VITORIA DO DEFENSOR! %s perde 1 tropa.\n", atacante->nome);
        
        if (atacante->numeroTropas <= 0) {
            printf("%s ficou sem tropas para atacar!\n", atacante->nome);
            atacante->numeroTropas = 1;
        }
    }
}

// NOVA FUNÇÃO: Atribuir missão aleatória
void atribuirMissao(Missao *missao) {
    missao->tipo = (rand() % 2) + 1; // 1 ou 2
    
    if (missao->tipo == 1) {
        // Missão: Destruir um exército de cor específica
        const char* cores[] = {"Verde", "Azul", "Vermelho", "Amarelo"};
        int corIndex = rand() % 4;
        strcpy(missao->alvo, cores[corIndex]);
        snprintf(missao->descricao, MAX_MISSAO, 
                "Destruir completamente o exercito %s", missao->alvo);
        missao->quantidade = 0;
    } else {
        // Missão: Conquistar 3 territórios
        missao->quantidade = 3;
        strcpy(missao->alvo, "");
        snprintf(missao->descricao, MAX_MISSAO, 
                "Conquistar %d territorios", missao->quantidade);
    }
    
    printf("\n=== MISSAO ATRIBUIDA ===\n");
    printf("%s\n", missao->descricao);
    printf("=======================\n\n");
}

// NOVA FUNÇÃO: Verificar cumprimento da missão
int verificarMissao(const Missao *missao, const Territorio *territorios) {
    if (missao->tipo == 1) {
        // Verificar se o exército alvo foi destruído
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(territorios[i].corExercito, missao->alvo) == 0) {
                return 0; // Missão não cumprida - ainda existe território com essa cor
            }
        }
        return 1; // Missão cumprida - nenhum território tem essa cor
    } else {
        // Verificar se conquistou a quantidade necessária de territórios
        // Considera que o jogador controla o primeiro território (Brasil)
        int territoriosConquistados = 0;
        const char* corJogador = territorios[0].corExercito;
        
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(territorios[i].corExercito, corJogador) == 0) {
                territoriosConquistados++;
            }
        }
        
        return (territoriosConquistados >= missao->quantidade) ? 1 : 0;
    }
}

// FUNÇÃO: Realizar ataque (modularizada)
void realizarAtaque(Territorio *territorios) {
    int atacante, defensor;
    
    printf("Digite o ID do territorio ATACANTE (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &atacante);
    printf("Digite o ID do territorio DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
    scanf("%d", &defensor);
    
    while(getchar() != '\n');
    
    // Validações
    if (atacante < 1 || atacante > NUM_TERRITORIOS || 
        defensor < 1 || defensor > NUM_TERRITORIOS) {
        printf("IDs invalidos! Use numeros de 1 a %d.\n", NUM_TERRITORIOS);
        return;
    }
    
    if (atacante == defensor) {
        printf("Um territorio nao pode atacar a si mesmo!\n");
        return;
    }
    
    int idxAtacante = atacante - 1;
    int idxDefensor = defensor - 1;
    
    if (territorios[idxAtacante].numeroTropas <= 1) {
        printf("%s nao tem tropas suficientes para atacar!\n", 
               territorios[idxAtacante].nome);
        return;
    }
    
    if (territorios[idxDefensor].numeroTropas <= 0) {
        printf("%s ja esta sem tropas!\n", territorios[idxDefensor].nome);
        return;
    }
    
    // Passagem por referência dos territórios
    simularAtaque(&territorios[idxAtacante], &territorios[idxDefensor], 
                 idxAtacante, idxDefensor);
}

// FUNÇÃO: Menu principal interativo
void menuPrincipal(Territorio *territorios, Missao *missao) {
    int opcao;
    
    do {
        exibirMapa(territorios);
        
        printf("=== MENU PRINCIPAL ===\n");
        printf("1. Atacar\n");
        printf("2. Verificar Missao\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        while(getchar() != '\n');
        
        switch(opcao) {
            case 1:
                realizarAtaque(territorios);
                break;
            case 2:
                printf("\n=== VERIFICACAO DE MISSAO ===\n");
                printf("Missao atual: %s\n", missao->descricao);
                
                if (verificarMissao(missao, territorios)) {
                    printf("🎉 PARABENS! MISSAO CUMPRIDA! 🎉\n");
                    printf("Voce venceu o jogo!\n");
                    return; // Encerra o jogo
                } else {
                    printf("Missao ainda nao cumprida. Continue sua conquista!\n");
                }
                printf("==============================\n\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
        
    } while (opcao != 0);
}

// FUNÇÃO PRINCIPAL
int main() {
    srand(time(NULL));
    
    printf("=== NIVEL MESTRE - MISSOES E MODULARIZACAO ===\n\n");
    
    // Alocação dinâmica mantida do nível Aventureiro
    Territorio *territorios = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1;
    }
    
    // Inicialização automática
    inicializarTerritorios(territorios);
    
    // Sistema de missões
    Missao missao;
    atribuirMissao(&missao);
    
    // Menu principal interativo
    menuPrincipal(territorios, &missao);
    
    // Liberar memória
    free(territorios);
    printf("\nJogo encerrado. Obrigado por jogar!\n");
    
    return 0;
}