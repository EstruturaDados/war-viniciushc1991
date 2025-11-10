#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 50
#define MAX_COR 30
#define NUM_TERRITORIOS 5

// Struct Territorio mantida do nível Novato
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int numeroTropas;
} Territorio;

// Função para cadastrar territórios (similar ao nível Novato)
void cadastrarTerritorios(Territorio *territorios) {
    printf("=== CADASTRO DE TERRITORIOS ===\n\n");
    
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- Territorio %d ---\n", i + 1);
        
        printf("Nome do territorio: ");
        fgets(territorios[i].nome, MAX_NOME, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;
        
        printf("Cor do exercito: ");
        fgets(territorios[i].corExercito, MAX_COR, stdin);
        territorios[i].corExercito[strcspn(territorios[i].corExercito, "\n")] = 0;
        
        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].numeroTropas);
        
        while(getchar() != '\n');
        printf("\n");
    }
}

// Função para exibir o mapa
void exibirMapa(Territorio *territorios) {
    printf("=== ESTADO ATUAL DO MAPA ===\n\n");
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

// NOVA FUNÇÃO: Simular ataque entre dois territórios
void simularAtaque(Territorio *atacante, Territorio *defensor, int idAtacante, int idDefensor) {
    printf("\n=== SIMULACAO DE BATALHA ===\n");
    printf("%s (ID: %d) ataca %s (ID: %d)!\n", 
           atacante->nome, idAtacante + 1, defensor->nome, idDefensor + 1);
    
    // Simular dados de ataque e defesa com rand()
    int dadoAtaque = (rand() % 6) + 1;  // Dado de 1 a 6
    int dadoDefesa = (rand() % 6) + 1;  // Dado de 1 a 6
    
    printf("Dado de ataque: %d\n", dadoAtaque);
    printf("Dado de defesa: %d\n", dadoDefesa);
    
    // Lógica da batalha conforme especificado
    if (dadoAtaque >= dadoDefesa) {
        // Atacante vence - defensor perde 1 tropa
        defensor->numeroTropas--;
        printf("VITORIA DO ATACANTE! %s perde 1 tropa.\n", defensor->nome);
        
        // Verificar se defensor perdeu todas as tropas
        if (defensor->numeroTropas <= 0) {
            printf("CONQUISTA! %s foi conquistado por %s!\n", 
                   defensor->nome, atacante->nome);
            
            // Conquistar território: copiar dados do atacante para defensor
            strcpy(defensor->corExercito, atacante->corExercito);
            defensor->numeroTropas = 1; // Tropa mínima para ocupação
        }
    } else {
        // Defensor vence - atacante perde 1 tropa
        atacante->numeroTropas--;
        printf("VITORIA DO DEFENSOR! %s perde 1 tropa.\n", atacante->nome);
        
        // Verificar se atacante perdeu todas as tropas
        if (atacante->numeroTropas <= 0) {
            printf("%s ficou sem tropas para atacar!\n", atacante->nome);
            atacante->numeroTropas = 1; // Mantém pelo menos 1 tropa
        }
    }
}

// Função principal do nível Aventureiro
int main() {
    // Inicializar semente para números aleatórios
    srand(time(NULL));
    
    printf("=== NIVEL AVENTUREIRO - BATALHAS ESTRATEGICAS ===\n\n");
    
    // SUBSTITUIR VETOR ESTÁTICO POR ALOCAÇÃO DINÂMICA COM CALLOC
    Territorio *territorios = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    
    if (territorios == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1;
    }
    
    // Cadastrar territórios
    cadastrarTerritorios(territorios);
    
    // LAÇO INTERATIVO para batalhas
    int opcao;
    do {
        exibirMapa(territorios);
        
        printf("=== FASE DE ATAQUE ===\n");
        printf("1. Realizar ataque\n");
        printf("2. Sair do jogo\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        while(getchar() != '\n'); // Limpar buffer
        
        if (opcao == 1) {
            int atacante, defensor;
            
            // Escolha de território atacante
            printf("\nDigite o ID do territorio ATACANTE (1 a %d): ", NUM_TERRITORIOS);
            scanf("%d", &atacante);
            
            // Escolha de território defensor
            printf("Digite o ID do territorio DEFENSOR (1 a %d): ", NUM_TERRITORIOS);
            scanf("%d", &defensor);
            
            while(getchar() != '\n'); // Limpar buffer
            
            // Validar entradas
            if (atacante < 1 || atacante > NUM_TERRITORIOS || 
                defensor < 1 || defensor > NUM_TERRITORIOS) {
                printf("IDs invalidos! Use numeros de 1 a %d.\n", NUM_TERRITORIOS);
                continue;
            }
            
            if (atacante == defensor) {
                printf("Um territorio nao pode atacar a si mesmo!\n");
                continue;
            }
            
            // Converter para índices do array (0-based)
            int idxAtacante = atacante - 1;
            int idxDefensor = defensor - 1;
            
            // Verificar se há tropas suficientes
            if (territorios[idxAtacante].numeroTropas <= 1) {
                printf("%s nao tem tropas suficientes para atacar!\n", 
                       territorios[idxAtacante].nome);
                continue;
            }
            
            if (territorios[idxDefensor].numeroTropas <= 0) {
                printf("%s ja esta sem tropas!\n", territorios[idxDefensor].nome);
                continue;
            }
            
            // USO DE PONTEIROS: Passar endereços dos territórios para a função
            simularAtaque(&territorios[idxAtacante], &territorios[idxDefensor], 
                         idxAtacante, idxDefensor);
            
        } else if (opcao != 2) {
            printf("Opcao invalida!\n");
        }
        
    } while (opcao != 2);
    
    // LIBERAR MEMÓRIA ALOCADA DINAMICAMENTE
    free(territorios);
    printf("\nJogo encerrado. Memoria liberada.\n");
    
    return 0;
}