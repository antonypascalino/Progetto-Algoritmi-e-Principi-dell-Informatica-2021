#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int main()
{
    int d = 5;
    char riga[1000];
    char numero[11];
    int readNumbers[10];

    printf("%s", riga);

    int currRowChar=0, currNumberDigit=0, currReadNumber=0;

    for(int rows = 0; rows < d; rows++)
    {
        fgets(riga, (11*d)+1, stdin); //10 è il numero massimo di cifre che può avere un numero a 32 bit; 10+1=11 per la virgola che ogni numero ha, il +1 finale per il carattere \n
        printf("Riga #%d\n", rows+1);
        for(currRowChar=0, currNumberDigit=0, currReadNumber=0;;currRowChar++) {
            if(riga[currRowChar] == ',' || riga[currRowChar] == '\n') {
                //Converti e salva il numero con atoi
                readNumbers[currReadNumber] = atoi(numero);
                printf("numero appena letto: %d\n", readNumbers[currReadNumber]);
                memset(numero, '\0', 11);
                currNumberDigit = 0;
                //Se pè stata raggiunta la fine della riga
                if (riga[currRowChar] == '\n') {
                    break;
                }
                currReadNumber++;
                printf("%s", numero);
            } else {
                numero[currNumberDigit] = riga[currRowChar];
                currNumberDigit++;
            }
        }
    }
}
