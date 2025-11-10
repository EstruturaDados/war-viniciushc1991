#include <stdio.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_COR 30
#define NUM_TERRITORIOS 5

// Struct Territorio conforme especificado
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int numeroTropas;
} Territorio;

int main() {
    // Vetor estático de 5 elementos para armazenar os territórios
    Territorio territorios[NUM_TERRITORIOS];
    
    printf("=== CADASTRO DE TERRITORIOS - NIVEL NOVATO ===\n\n");
    
    // Cadastro dos 5 territórios
    for(int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("--- Territorio %d ---\n", i + 1);
        
        // Leitura do nome do território usando fgets
        printf("Nome do territorio: ");
        fgets(territorios[i].nome, MAX_NOME, stdin);
        // Remover o \n do final da string
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;
        
        // Leitura da cor do exército usando fgets
        printf("Cor do exercito: ");
        fgets(territorios[i].corExercito, MAX_COR, stdin);
        // Remover o \n do final da string
        territorios[i].corExercito[strcspn(territorios[i].corExercito, "\n")] = 0;
        
        // Leitura do número de tropas usando scanf
        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].numeroTropas);
        
        // Limpar o buffer do teclado após o scanf
        while(getchar() != '\n');
        
        printf("\n");
    }
    
    // Exibir o estado atual do mapa
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
    
    return 0;
}