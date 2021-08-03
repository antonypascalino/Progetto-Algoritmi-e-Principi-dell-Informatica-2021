#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//tipi definiti
struct Grafo {
    int indice;
    long peso;
    struct Grafo* next;
    struct Grafo* prev;
};

//funzioni
void stampaGrafo();
void allocaGrafo();
void leggiGrafo();
void leggiComandi();
void leggiPrimoComando();
long calcolaPeso();
struct Grafo* creaNodo(unsigned int peso);
void aggiornaClassifica(struct Grafo* grafo);
void stampaClassifica();
unsigned int fastAtoi(char numero[11]);

//variabili globali
unsigned int **matrice;
int d;  //graph dimension
int k;  //number of graph to show
int grafiLetti = 0;
long *pesiTmp;
int *visited;
int *pred;
char *riga;
struct Grafo* testa = NULL;
struct Grafo* ultimo = NULL;
struct Grafo* curr;

int main() {
    leggiPrimoComando();
    leggiComandi();
}

void leggiPrimoComando() {
    char firstCommand[22];

    if(fgets(firstCommand, (10 * 2 + 2), stdin) != NULL);
    sscanf(firstCommand, "%d %d", &d, &k);
//    printf("\nd = %d\nk = %d\n", d, k);
    allocaGrafo(); //alloco il grafo d*d

    //alloco tutti gli array usiliari che mi serviranno per calcolare il peso del grafo
    //li alloco qua e non in calcolaPeso() in modo tale da chiamare una sola volta malloc
    pesiTmp = malloc((d) * sizeof(long)); //alloco l'array che mi servirà per calcolare i pesi
    visited = malloc((d) * sizeof(int));
    pred = malloc((d) * sizeof(int));
    riga = malloc(11 * d + 1 *sizeof(int)); //alloca la stringa per leggere l'input
}

void leggiComandi() {
    char comando[15];

    while(fgets(comando, 15, stdin) != NULL ) {
        if(strcmp(comando, "AggiungiGrafo\n") == 0) {
            //printf("Ho letto AggiungiGrafo\n\n");
            leggiGrafo();
            aggiornaClassifica(creaNodo(calcolaPeso()));
            grafiLetti++;
        }
        else if(strcmp(comando, "TopK\n") == 0) {
            //printf("Ho letto TopK\n\n");
            stampaClassifica();
        }
        else {
            printf("Comando non valido\n\n");
        }
    }
}

void allocaGrafo() {
    matrice = (unsigned int **) malloc(d * sizeof(unsigned int *));
    for(int i = 0; i < d; ++i) {
        matrice[i] = (unsigned int *) malloc(d * sizeof(unsigned int));
    }
//    printf("Ho allocato un grafo %dx%d\n\n", d,d);
}

void stampaGrafo() {
    for(int row = 0; row < d; row++) {
        for(int col = 0; col < d; col++) {
            printf("%d ", matrice[row][col]);
        }
        printf("\n");
    }
    printf("Ho letto il grafo: %d\n", grafiLetti);
}

void leggiGrafo() {

    char numero[11];
    int currRowChar = 0, currNumberDigit = 0, currReadNumber = 0;

    for(int rows = 0; rows < d; rows++) {
        if(fgets(riga, 11 * d + 1, stdin) != NULL); //10 è il numero massimo di cifre che può avere un numero a 32 bit; 10+1=11 per la virgola (o \n) che ogni numero ha, il +1 final eè richiesto  da fgets
        for(currRowChar = 0, currNumberDigit = 0, currReadNumber = 0;; currRowChar++) {
            if(riga[currRowChar] == ',' || riga[currRowChar] == '\n') {
                //Converti e salva il numero con atoi
                matrice[rows][currReadNumber] = fastAtoi(numero);
                //Inizializzo l'arrey
                for(int i = 0; i<11; i++) {
                    numero[i] = '\0';
                }
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
    //printf("Ho letto questo grafo:\n");
    //stampaGrafo();
}

unsigned int fastAtoi(char numero[11]) {
    int convertedNumber = 0;

    for(int i = 0; numero[i]<='9' && numero[i]>='0'; i++) {
        convertedNumber = convertedNumber*10 + numero[i]-48;
    }
    return convertedNumber;
}

long calcolaPeso() {
    long pesoTotale = 0;
    unsigned int pesoMinoreCurr;
    int indicePesoMinore = 0;
    int nodi = 1;

    //Inizializzo l'array visited, l'array pesiTmp
    for(int col = 0; col < d; col++) {
        pesiTmp[col] = matrice[0][col];
        visited[col] = 0;
        pred[col]= 0;
    }
    visited[0] = 1; //Nodo sorgente già visitato
    pesiTmp[0] = 0; //Distanza nodo sorgente da nodo sorgente = 0

    while(nodi < d-1) {
        pesoMinoreCurr = 4294967295;
        //Cerco, fra i pesi che ho scritto, quello col peso minore
        for(int indice = 0; indice<d; indice++) {
            if(pesiTmp[indice] < pesoMinoreCurr && pesiTmp[indice] > 0 && !visited[indice]) {
                pesoMinoreCurr = pesiTmp[indice];
                indicePesoMinore = indice;
            }
        }
        //Mi metto sul nodo appena trovato e guardo se trovo percorsi col peso minore rispetto a quelli che ho già trovato
        visited[indicePesoMinore] = 1;
        for(int i = 0; i < d; i++) {
            if(!visited[i]) {
                if(matrice[indicePesoMinore][i]!=0 && (pesoMinoreCurr+matrice[indicePesoMinore][i] < pesiTmp[i] || pesiTmp[i] == 0)) {
                    pesiTmp[i] = pesoMinoreCurr+matrice[indicePesoMinore][i];
                    pred[i] = indicePesoMinore;
                }
            }
        }
        nodi++;
    }

    //Calcolo la somma di tutti i cammini minimi
    for(int i = 0; i < d; i++) {
        pesoTotale += pesiTmp[i];
    }
    //printf("Il peso del grafo è: %d\n\n", pesoTotale);
    return pesoTotale;
}

struct Grafo* creaNodo(unsigned int peso) {
    struct Grafo* nuovoGrafo;

    nuovoGrafo = (struct Grafo*)malloc(sizeof(struct Grafo));
    nuovoGrafo->indice = grafiLetti;
    nuovoGrafo->peso = peso;
    //printf("Ho creato un nuovo nodo\nPeso: %d\nIndice: %d\n", nuovoGrafo->peso, nuovoGrafo->indice);
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

//    printf("La classifica al momento è:\n");
    if(curr != NULL) {
        printf("%d", curr->indice);
        while(curr->next != NULL) {
            curr = curr->next;
            printf(" %d", curr->indice);
        }
        printf("\n");
    }
}








