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
void stampaClassifica();
int fastAtoi(char numero[11]);

//variabili globali
int **matrice;
int d;  //graph dimension
int k;  //number of graph to show
int grafiLetti = 0;
long *pesiTmp;
struct Grafo* testa = NULL;
struct Grafo* ultimo = NULL;
struct Grafo* curr;

int main() {
    leggiPrimoComando();
    leggiComandi();
}

void leggiPrimoComando() {
    char firstCommand[22];

    fgets(firstCommand, (10 * 2 + 2), stdin);
    sscanf(firstCommand, "%d %d", &d, &k);
    printf("\nd = %d\nk = %d\n", d, k);
    allocaGrafo(); //alloco il grafo d*d
    pesiTmp = malloc((d-1) * sizeof(long)); //alloco l'array che mi servirà per calcolare i pesi
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
    int pesoTotale = 0;
    int pesoMinoreCurr = 4294967295;
    int indicePesoMinore = 0;

    memset(pesiTmp, '0', d-1);

    for(int row = indicePesoMinore;;) {
        pesoMinoreCurr=0;
        //copio tutti i pesi per raggiungere tutti i nodi direttamente raggiungibili dalla radice
        for(int col = 1; col < d; col++) {
            if(matrice[0][col] + pesoMinoreCurr > 0 && matrice[0][col] + pesoMinoreCurr < pesiTmp[col-1]) {
                pesiTmp[col-1] = matrice[0][col] + pesoMinoreCurr;
            }
            if(pesiTmp[col-1] < pesoMinoreCurr && pesiTmp[col-1] > 0) {
                pesoMinoreCurr = pesiTmp[col];
                indicePesoMinore = col;
            }
        }
    }


    printf("Il peso del grafo è: %d\n", pesoTotale);
    return pesoTotale;
}

struct Grafo* creaNodo(int peso) {
    struct Grafo* nuovoGrafo;

    nuovoGrafo = (struct Grafo*)malloc(sizeof(struct Grafo));
    nuovoGrafo->indice = grafiLetti;
    nuovoGrafo->peso = peso;
    printf("Ho creato un nuovo nodo\nPeso: %d\nIndice: %d\n", nuovoGrafo->peso, nuovoGrafo->indice);
    return nuovoGrafo;
}

void aggiornaClassifica(struct Grafo* nuovoGrafo) {
    if(testa == NULL || nuovoGrafo->peso < testa->peso) {
        if(testa == NULL) {
            testa = nuovoGrafo;
            ultimo = nuovoGrafo;
        } else {
            testa->prev = nuovoGrafo;
            nuovoGrafo->next = testa;
            nuovoGrafo->prev = NULL;
            testa = nuovoGrafo;
        }
        if(grafiLetti >= k) {
            ultimo = ultimo->prev;
            free(ultimo->next);
            ultimo->next = NULL;
        }
    }
    else {
        curr = testa;
        //Scorro la lista fintanto che o arrivo alla fine o mi trovo nel posto in cui inserire il grafo.
        while(curr->next != NULL && curr->next->peso <= nuovoGrafo->peso) {
            curr = curr->next;
        }
        //Se sono arrivato alla fine della lista e ho ancora spazio nella TopK, aggiungo il nuovoGrafo all fine
        if(curr->next == NULL && grafiLetti < k) {
            curr->next = nuovoGrafo;
            nuovoGrafo->next = NULL;
            nuovoGrafo->prev = curr;
            ultimo = nuovoGrafo;
        }
        //Se sono arrivato alla fine della lista ma non ho più spazio nella TopK, vuol dire che il nuovoGrafo non ha posto in classifica, quindi non faccio niente
        else if(curr->next == NULL && grafiLetti >= k) {
            return;
        }
        //Se non sono arrivato alla fine della lista vuol dire che devo aggiungere il grafo in mezzo alla lista
        else if(curr->next != NULL) {
            nuovoGrafo->next = curr->next;
            nuovoGrafo->prev = curr;
            curr->next->prev = nuovoGrafo;
            curr->next = nuovoGrafo;
            //Se la classifica era piena, devo eliminare l'ultimo nodo e spostarlo
            if(grafiLetti >= k) {
                ultimo = ultimo->prev;
                free(ultimo->next);
                ultimo->next = NULL;
            }
        }
    }
}

void stampaClassifica() {
    curr = testa;

    printf("La classifica al momento è:\n");
    if(curr != NULL) {
        printf("%d ", curr->indice);
        while(curr->next != NULL) {
            curr = curr->next;
            printf("%d ", curr->indice);
        }
    }
}








