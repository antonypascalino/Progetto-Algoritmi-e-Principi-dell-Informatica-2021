#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

//metodi
void stampaGrafo();
void allocaGrafo();
void leggiGrafo();
void leggiComandi();
void leggiPrimoComando();
void calcolaPeso();
void aggiornaTop();
void stampaTopK();

//variabili globali
int **graph;
int d;  //graph dimension
int k;  //number of graph to show

int main() {

    leggiPrimoComando();
    allocaGrafo();
    leggiComandi();
}

void leggiPrimoComando() {
    char firstCommand[22];

    fgets(firstCommand, (10 * 2 + 2), stdin);
    sscanf(firstCommand, "%d %d", &d, &k);
    printf("\nd = %d\nk = %d\n", d, k);
}

void leggiComandi() {
    char comando[15];

    while(fgets(comando, 15, stdin) != NULL ) {
        if(strcmp(comando, "AggiungiGrafo\n") == 0) {
            printf("Ho letto AggiungiGrafo\n\n");
            leggiGrafo();
            //Calcola peso, compara, aggiungi, elimina
        }
        else if(strcmp(comando, "TopK\n") == 0) {
            printf("Ho letto TopK\n\n");
            //Stampa i top K grafi
        }
        else {
            printf("Comando non valido\n\n");
        }
    }
}

void allocaGrafo() {
    graph = (int **) malloc(d * sizeof(int *));
    for(int i = 0; i < d; ++i) {
        graph[i] = (int *) malloc(d * sizeof(int));
    }
    printf("Ho allocato un grafo %dx%d\n\n", d,d);
}

void stampaGrafo() {

    for(int row = 0; row < d; row++) {
        for(int col = 0; col < d; col++) {
            printf("%d ", graph[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

void leggiGrafo() {
    char riga[1000];
    char numero[11];
    int currRowChar = 0, currNumberDigit = 0, currReadNumber = 0;

    for(int rows = 0; rows < d; rows++) {
        fgets(riga, (11 * d) + 1, stdin); //10 è il numero massimo di cifre che può avere un numero a 32 bit; 10+1=11 per la virgola che ogni numero ha, il +1 finale per il carattere \n
        for(currRowChar = 0, currNumberDigit = 0, currReadNumber = 0;; currRowChar++) {
            if(riga[currRowChar] == ',' || riga[currRowChar] == '\n') {
                //Converti e salva il numero con atoi
                graph[rows][currReadNumber] = atoi(numero);
                memset(numero, '\0', 11);
                currNumberDigit = 0;
                //Se è stata raggiunta la fine della riga
                if(riga[currRowChar] == '\n') {
                    break;
                }
                currReadNumber++;
            } else {
                numero[currNumberDigit] = riga[currRowChar];
                currNumberDigit++;
            }
        }
    }
    printf("Ho letto questo grafo:\n");
    stampaGrafo();
}
