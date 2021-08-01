#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

//tipi definiti
struct Grafo {
    int indice;
    int peso;
    struct Grafo* next;
    struct Grafo* prev;
};

//funzioni
void stampaGrafo();
void allocaGrafo();
void leggiGrafo();
void leggiComandi();
void leggiPrimoComando();
int calcolaPeso();
struct Grafo* creaNodo(int peso);
void aggiornaClassifica(struct Grafo* grafo);
//void stampaClassifica();
int fastAtoi(char numero[11]);

//variabili globali
int **matrice;
int *classifica;
int d;  //graph dimension
int k;  //number of graph to show
int grafiLetti = 0;
int topGrafo = -1; //Il grafo che attualmente ha il peso minore (variabile usata per capire se aggiungere in testa o meno)
struct Grafo* testa = NULL;
struct Grafo* curr;

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
            aggiornaClassifica(creaNodo(calcolaPeso()));
            grafiLetti++;
        }
        else if(strcmp(comando, "TopK\n") == 0) {
            printf("Ho letto TopK\n\n");
//            stampaTopK();
        }
        else {
            printf("Comando non valido\n\n");
        }
    }
}

void allocaGrafo() {
    matrice = (int **) malloc(d * sizeof(int *));
    for(int i = 0; i < d; ++i) {
        matrice[i] = (int *) malloc(d * sizeof(int));
    }
    printf("Ho allocato un grafo %dx%d\n\n", d,d);
}

void stampaGrafo() {

    for(int row = 0; row < d; row++) {
        for(int col = 0; col < d; col++) {
            printf("%d ", matrice[row][col]);
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
        fgets(riga, 11 * d + 1, stdin); //10 è il numero massimo di cifre che può avere un numero a 32 bit; 10+1=11 per la virgola (o \n) che ogni numero ha, il +1 final eè richiesto  da fgets
        for(currRowChar = 0, currNumberDigit = 0, currReadNumber = 0;; currRowChar++) {
            if(riga[currRowChar] == ',' || riga[currRowChar] == '\n') {
                //Converti e salva il numero con atoi
                matrice[rows][currReadNumber] = fastAtoi(numero);
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

int fastAtoi(char numero[11]) {
    int convertedNumber = 0;

    for(int i = 0; numero[i]<='9' && numero[i]>='0'; i++) {
        convertedNumber = convertedNumber*10 + numero[i]-48;
    }
    return convertedNumber;
}

int calcolaPeso() {
    return grafiLetti;
}

struct Grafo* creaNodo(int peso)  {
    struct Grafo* nuovoGrafo;

    nuovoGrafo = (struct Grafo*)malloc(sizeof(struct Grafo));
    nuovoGrafo->indice = grafiLetti;
    nuovoGrafo->peso = peso;
    return nuovoGrafo;
}

void aggiornaClassifica(struct Grafo* nuovoGrafo) {
    if(testa == NULL) {
        testa = nuovoGrafo;
    }
    else {
        curr = testa;
        while(curr->next->peso <= nuovoGrafo->peso && curr->next != NULL) {
            curr = curr->next;
        }
        if(curr->next == NULL && grafiLetti < k) {
            curr->next = nuovoGrafo;
            nuovoGrafo->next = NULL;
        }
        else if(curr->next == NULL && grafiLetti >= k) {
            return;
        }
        else if(curr->next != NULL) {
            nuovoGrafo->next = curr->next;

        }
    }
}



