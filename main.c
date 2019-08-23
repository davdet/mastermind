/*
..::WHAT'S NEW::..

v 1.4-beta - Eliminato il puntatore int_pointer.

v 1.3-beta - Modificata macro INWHITE (correzione colore).

v 1.2-beta - Inglobato il puntatore int_pointer nella funzione checkMatches.

v 1.1-beta - . Aggiunto controllo iniziale sulla lettura delle istruzioni da parte dell'utente: se l'utente prova ad
             avviare il gioco senza aver prima letto le istruzioni, viene visualizzato un messaggio che consiglia di leggerle.
             . Corretto typo nelle istruzioni.

v 1.0-beta - Release iniziale (disponibile anche per Windows).
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


//DICHIARAZIONI FUNZIONI
//int charToInt(char mn, int startAscii, int endAscii);
void ignoreInputUntil(const char endCh);
void getSequence(char usrSeq[], const int sz);
bool checkSequence(const char usrSeq[], const int sz);
void printSequence(const char seq[]);
void randomizeSequence(char cpuSeq[], const int sz);
void startGame(void);
bool printHowTo(void);
void printCredits(void);
int chooseDifficultyLevel(void);
void checkMatches(const char usrSeq[], const char cpuSeq[], const int sz);
bool checkExit(const char usrSeq[]);


//PARTI GRAFICHE
/*
#define RED printf("\033[1;31m");
#define GREEN printf("\033[1;32m");
#define YELLOW printf("\033[1;33m");
#define BLUE printf("\033[1;34m");
#define RESETCOLOR printf("\033[0m");
*/

#define INRED "\033[1;31m"
#define INGREEN "\033[1;32m"
#define INYELLOW "\033[1;33m"
#define INBLUE "\033[1;34m"
#define INWHITE "\033[1;39m"
#define INRESETCOLOR "\033[0m"
#define ERRORCOLOR "\033[1;31m\a"
#define WINCOLOR "\033[5;1;39m"

#define BLOCK "\u2B24"
#define BLACKMATCH "\u25A0 "
#define WHITEMATCH "\u25A1 "

#define RBLOCK INRED BLOCK INRESETCOLOR "  "
#define GBLOCK INGREEN BLOCK INRESETCOLOR "  "
#define BBLOCK INBLUE BLOCK INRESETCOLOR "  "
#define YBLOCK INYELLOW BLOCK INRESETCOLOR "  "

#define TITLE printf(INWHITE "               _                 _       _ \n");\
              printf(INWHITE " _____ ___ ___| |_ ___ ___ _____|_|___ _| | by\n");\
              printf(INWHITE "|     | .'|_ -|  _| -_|  _|     | |   | . | Davide\n");\
              printf(INWHITE "|_|_|_|__,|___|_| |___|_| |_|_|_|_|_|_|___| Dettori\n" INRESETCOLOR);\
              printf("                                 v 1.3-beta\n");

#define PROMPT printf("\n> ");
#define CLEARSCR printf("\033[H\033[J");


//CONDIZIONI
#define IS_LOWER_CASE(x) (((x)>='a') && ((x)<='z'))
#define IS_A_COLOR(x) (((x)=='R') || ((x)=='G') || ((x)=='V') || ((x)=='B'))
#define IS_NOT_A_COLOR(x) (((x)!='R') && ((x)!='G') && ((x)!='V') && ((x)!='B'))
#define IS_NOT_YES_OR_NO(x) (((x)!='y') && ((x)!='Y') && ((x)!='n') && ((x)!='N'))
#define IS_YES(x) (((x)=='y') || ((x)=='Y'))


//VARIABILI GLOBALI
/*Queste variabili sono settate come globali perché verranno utilizzate in 2 differenti funzioni.*/
int whMatch;
int blMatch;
/*Questa variabile è settata come globale perché verrà utilizzata in 3 differenti funzioni. Serve da flag nel caso l'utente, durante una
sessione di gioco, digiti la sequenza speciale EXIT per tornare immediatamente al menu principale.*/
bool exitNow;





int main(void)
{
    char menu;
    /*Selettore del menu iniziale.*/
    bool readHowTo;
    /*Flag che segnala se sono state lette le istruzioni*/
    char howTo;

    do
    {
        CLEARSCR
        TITLE

        printf("Benvenuto in Mastermind.\n");

        do
        {
            printf("1. Gioca\n2. Istruzioni\n3. Crediti\n4. Esci");
            PROMPT
            menu=getchar();
            ignoreInputUntil('\n');

            switch(menu)
            {
                case '1':
                    if(!readHowTo)
                    /*Nel caso in cui si tenti di iniziare una partita senza aver letto le istruzioni, il programma consiglia di leggerle.*/
                    {
                        CLEARSCR
                        printf(INWHITE "ATTENZIONE: prima di giocare si consiglia di leggere le istruzioni.\n\n" INRESETCOLOR);
                        do
                        /*Ciclo di controllo valore in input: vengono accettate solo le risposte y, Y, n, N.*/
                        {
                            printf("Vuoi leggere le istruzioni? [Y/N]");
                            PROMPT
                            howTo=getchar();
                            ignoreInputUntil('\n');
                            if(IS_NOT_YES_OR_NO(howTo))
                                printf(ERRORCOLOR "ERRORE: scelta non valida.\n" INRESETCOLOR);
                        }while(IS_NOT_YES_OR_NO(howTo));

                        if(IS_YES(howTo))
                        /*Se il giocatore decide di leggere le istruzioni, queste vengono mostrate prima della prima partita.*/
                            readHowTo=printHowTo();
                        else
                        /*Altrimenti il flag readHowTo viene posto a true e questo suggerimento iniziale non viene più mostrato fino al
                        prossimo avvio del programma.*/
                            readHowTo=true;
                    }
                    startGame();
                    break;
                case '2':
                    readHowTo=printHowTo();
                    break;
                case '3':
                    printCredits();
                    break;
                case '4':
                    return 0;
                    break;
                default:
                    ignoreInputUntil('\n');
                    printf(ERRORCOLOR "ERRORE: scelta non valida.\n" INRESETCOLOR);
            }
        }while(menu<'1' || menu>'4');
    }while(menu!='4');

    return 0;
}


/*La funzione ignora tutti i caratteri in input finché non viene digitato un carattere uguale a endCh*/
void ignoreInputUntil(const char endCh)
{
    char ch;

    do
    /*Questo ciclo serve per ignorare tutti gli eventuali caratteri che l'utente potrebbe digitare dopo l'uscita dal ciclo precedente.*/
    {
        ch=getchar();
    }while(ch!=endCh);
}


/*La funzione prende in ingresso un carattere char e un range di codici ASCII (startAscii, endAscii), verifica che il carattere rientri
in questo range e lo converte nel rispettivo valore intero. Se il carattere non rientra nel range viene restituito il valore NULL
int charToInt(char mn, int startAscii, int endAscii)
{
	int intValue;

	if(mn<startAscii || mn>endAscii)
		return NULL;
	else
	{
		intValue=mn-'0';
		return intValue;
	}
}
*/


/*La funzione acquisisce una sequenza digitata dal giocatore ed effettua tutti i controlli per verificare se tale sequenza sia corretta.*/
void getSequence(char usrSeq[], const int sz)
{
    int i=0;
    char ch;
    bool isOk;

    do
    /*Ciclo di controllo valore in input: viene ripetuto finché non viene inserita una sequenza di colori correttamente
    riconosciuta dalla funzione checkSequence. La variabile booleana isOk viene utilizzata per questo controllo e viene posta a true
    se la sequenza di colori è corretta.*/
    {
        i=0;
        PROMPT

        do
        /*Questo ciclo serve per popolare la stringa usrSeq con una sequenza di colori. Continua finché non viene premuto il tasto return
        (condizione while finale) o finché l'indice i non sia uguale al valore di size (che varia in base alla difficoltà scelta).*/
        {
            if(i==sz)
            /*Se l'indice i è uguale alla dimensione massima della stringa, viene memorizzato return in ch causando
            l'uscita immediata dal ciclo.*/
            {
                ch='\n';
                usrSeq[i]=ch;

                i++;
            }
            else
            {
                ch=getchar();
                /*Legge un carattere in input.*/
                if(IS_LOWER_CASE(ch))
                /*Controlla che il carattere letto sia una lettera minuscola e, nel caso, la trasforma in maiuscola.*/
                    ch-=32;
                usrSeq[i]=ch;
                /*Memorizza il carattere letto nell'array usrSeq.*/

                i++;
            }
        }while(ch!='\n');

        ignoreInputUntil('\n');

        usrSeq[i-1]='\0';
        /*Il return alla fine dell'array viene sostituito col carattere nullo.*/

        exitNow=checkExit(usrSeq);
        /*La variabile booleana exitNow viene posta a true qualora il giocatore decidesse di interrompere una sessione di gioco per tornare
        immediatamente al menu principale digitando EXIT.*/

        /*
        //ISTRUZIONE DI CHECK
        printf("%i", exitNow);
        */

        if(!exitNow)
        /*Se exitNow è uguale a false, significa che il giocatore non ha inserito la sequenza exit.*/
        {
            isOk=checkSequence(usrSeq, sz);
            /*La sequenza viene quindi controllata per vedere se è valida (ossia se tutti i caratteri in essa contenuti sono
            associabili ad un colore).*/
            if(!isOk)
            /*Se non è valida viene visualizzato un messaggio di errore*/
                printf(ERRORCOLOR "ERRORE: la sequenza inserita non è valida." INRESETCOLOR);
        }
        else
        /*Se exitNow è uguale a true, significa che il giocatore ha inserito la sequenza exit. La variabile bool isOk
        viene quindi posta a true causando così l'uscita dalla funzione.*/
            isOk=true;

        /*
        //INIZIO BLOCCO DI CHECK
        printf("\n%s", usrSeq);
        printf("\nisOk = %i", isOk);
        for(; i>=0; i--)
            printf("\nusrSeq[%i] vale %c", i, usrSeq[i]);
        printf("\n");
        //FINE BLOCCO DI CHECK
        */
    }while(!isOk);
}


/*La funzione controlla che se l'utente, durante una sessione di gioco, digita la sequenza speciale EXIT per tornare
immediatamente al menu principale.*/
bool checkExit(const char usrSeq[])
{
    const char exitKey[]="EXIT";
    int i=0;

    while(usrSeq[i]==exitKey[i] && usrSeq[i]!='\0' && exitKey[i]!='\0')
    /*Fintanto che usrSeq e exitKey sono uguali nella posizione i e in queste posizioni non sia presente il carattere nullo viene
    incrementato di uno l'indice i. Si esce dal ciclo quando viene trovato un carattere nullo.*/
        i++;

    if(usrSeq[i]==exitKey[i])
    /*Se dopo essere usciti dal ciclo precedente, entrambe le stringhe nell'indice i contengono il carattere nullo, allora possiamo
    essere sicuri che usrSeq sia uguale a exitKey, e che quindi il giocatore abbia digitato EXIT. Viene restituito true.*/
        return true;

    else
    /*Altrimenti, se le stringhe non coincidono, significa che il giocatore non ha digitato exit e viene quindi restituito false.*/
        return false;
}


/*La funzione controlla che i caratteri inseriti dall'utente siano validi, ossia "trasformabili" in un colore dalla funzione printSequence.*/
bool checkSequence(const char usrSeq[], const int sz)
{
    int i;

    for(i=0; i<sz; i++)
        if(IS_NOT_A_COLOR(usrSeq[i]))
            return false;

    return true;
}


/*La funzione stampa una sequenza di colori.*/
void printSequence(const char seq[])
{
    int i;

    for(i=0; seq[i]!='\0'; i++)
        switch(seq[i])
        {
            case 'R':
                printf(RBLOCK);
                break;
            case 'G':
                printf(YBLOCK);
                break;
            case 'V':
                printf(GBLOCK);
                break;
            case 'B':
                printf(BBLOCK);
                break;
        }
}


/*La funzione crea la sequenza casuale di colori che l'utente dovrà indovinare.*/
void randomizeSequence(char cpuSeq[], const int sz)
{
    int i, chaos;

    srand(time(0));

    for(i=0; i<sz; i++)
    {
        chaos=rand()%4;
        /*Nella variabile chaos viene memorizzato un numero casuale tra 0 e 3 che andrà poi switchato e associato ad
        un colore da inserire nell'array cpuSeq.*/

        switch(chaos)
        {
            case 0:
                cpuSeq[i]='R';
                break;
            case 1:
                cpuSeq[i]='G';
                break;
            case 2:
                cpuSeq[i]='V';
                break;
            case 3:
                cpuSeq[i]='B';
                break;
        }
    }

    cpuSeq[i]='\0';

    /*
    //INIZIO BLOCCO DI CHECK
    printf("\n%s", cpuSeq);
    for(; i>=0; i--)
        printf("\ncpuSeq[%i] vale %c", i, cpuSeq[i]);
    printf("\n");
    //FINE BLOCCO DI CHECK
    */
}


/*La funzione serve per selezionare il livello di difficoltà del gioco: in base alla scelta viene restituito il numero di colori che
il giocatore dovrà decifrare, ossia la lunghezza delle stringhe userSequence e cpuSequence.*/
int chooseDifficultyLevel(void)
{
    char diff;

    do
    {
        printf("Seleziona il livello di difficoltà:\n1. Facile\n2. Medio\n3. Difficile");
        PROMPT
        diff=getchar();
        ignoreInputUntil('\n');

        switch(diff)
        {
            case '1':
                return 4;
                break;
            case '2':
                return 5;
                break;
            case '3':
                return 6;
                break;
            default:
                ignoreInputUntil('\n');
                printf(ERRORCOLOR "ERRORE: scelta non valida.\n" INRESETCOLOR);
        }
    }while(diff<'1' || diff>'3');
}


/*La funzione confronta la sequenza immessa dall'utente con quella del computer e calcola il numero di match bianchi e match neri.
Fa uso di due array di flag per tenere traccia delle posizioni per le quali è già stato trovato un match, in maniera tale da evitare
la ripetizione dei match bianchi in alcune situazioni critiche.*/
void checkMatches(const char usrSeq[], const char cpuSeq[], const int sz)
{
    int i, j;
    bool usrFlags[7], cpuFlags[7];
    /*Variabile puntatore utilizzata per modificare i valori delle variabili globali blMatch e whMatch nella stessa funzione.*/

    for(i=0; i<7; i++)
    {
        usrFlags[i]=false;
        cpuFlags[i]=false;
    }

    blMatch=0;
    /*Il valore di blMatch viene inizializzato a 0*/

    for(i=0; i<sz; i++)
    /*Questo ciclo for serve a calcolare il numero di match neri, che indicano i cerchi di colore esatto nella corretta posizione:
    finché i è minore della dimensione della stringa contenuta in usrSeq (ad esclusione del carattere nullo)...*/
        if(usrSeq[i]==cpuSeq[i])
        /*...se usrSeq[i] è uguale a cpuSeq[i] significa che c'è un colore giusto nel posto giusto.*/
        {
            blMatch++;
            /*Viene quindi incrementato di 1 il numero dei match neri,*/
            usrFlags[i]=true;
            /*nell'array usrFlag viene posta a true la posizione coincidente con quella di usrSeq[i]*/
            cpuFlags[i]=true;
            /*e nell'array cpuFlag viene posta a true la posizione coincidente con quella di cpuSeq[i]*/
        }

    /*
    //ISTRUZIONE DI CHECK
    printf("%i", blMatch);
    */

    whMatch=0;
    /*Il valore di whMatch viene inizializzato a 0*/

    for(i=0; i<sz; i++)
    /*Questo ciclo for serve a calcolare il numero di match bianchi, che indicano i cerchi di colore esatto in una posizione sbagliata:
    finché i è minore della dimensione della stringa contenuta in usrSeq (ad esclusione del carattere nullo)...*/
        for(j=0; j<sz; j++)
        /*...viene fatto scorrere l'array cpuSeq tramite l'indice j.*/
            if(i!=j && usrSeq[i]==cpuSeq[j] && usrFlags[i]==false && cpuFlags[j]==false)
            /*Se i è diverso da j significa che le posizioni di usrSeq e cpuSeq alle quali stanno puntando i due indici sono differenti,
            pertanto potrebbe esserci la possibilità di un white match. Se usrFlags e cpuFlags nelle posizioni puntati dagli indici i e j
            sono uguali a false, significa che per quelle stesse posizioni negli array usrSeq e cpuSeq non è stato ancora trovato un white match.
            Allora, se usrSeq[i] e cpuSeq[i] contengono lo stesso valore, ci troviamo sicuramente di fronte ad un white match.*/
            {
                whMatch++;
                /*Viene incrementato di 1 il numero dei white match,*/
                usrFlags[i]=true;
                /*nell'array usrFlag viene posta a true la posizione coincidente con quella di usrSeq[i],*/
                cpuFlags[j]=true;
                /*e nell'array cpuFlag viene posta a true la posizione coincidente con quella di cpuSeq[i].*/
            }

    /*
    //ISTRUZIONE DI CHECK
    printf("%i", whMatch);
    */
}


/*La funzione dà il via ad una partita.*/
void startGame(void)
{
    char userSequence[7], cpuSequence[7], replay;
    int size, i, tryCounter;
    /*In tryCounter vengono memorizzati il numero di tentativi effettuati per decifrare la sequenza.*/

    do
    /*Questo ciclo è associato alla variabile char replay e serve a far cominciare un'altra partita nel caso
    il valore di replay sia 'y' o 'Y'.*/
    {
        CLEARSCR
        size=chooseDifficultyLevel();
        CLEARSCR

        randomizeSequence(cpuSequence, size);
        //printSequence(cpuSequence);
        /*NB: DECOMMENTARE LA FUNZIONE PRECEDENTE SE SI VUOLE VISUALIZZARE LA SEQUENZA DEL COMPUTER DURANTE LA PARTITA.*/

        tryCounter=0;
        printf("Inserisci una sequenza di %i colori: ", size);

        do
        /*Questo è il ciclo dove si svolge il gioco: il giocatore digita la sequenza da decifrare e riceve i relativi indizi. Il ciclo si
        può fermare in due modi: o il giocatore vince indovinando la sequenza del computer (e quindi ottenendo tanti black match quanti
        sono i colori della sequenza) o il giocatore digita la sequenza speciale EXIT per tornare immediatamente al menu principale.*/
        {
            getSequence(userSequence, size);
            /*Il giocatore inserisce una sequenza.*/
            if(!exitNow)
            /*Se la sequenza non è EXIT, allora questa viene stampata e confrontata con quella del computer.*/
            {
                printSequence(userSequence);
                checkMatches(userSequence, cpuSequence, size);

                printf(" ");
                for(i=0; i<blMatch; i++)
                    printf(BLACKMATCH);
                for(i=0; i<whMatch; i++)
                    printf(WHITEMATCH);

                tryCounter++;
                /*Il numero dei tentativi viene incrementato.*/

                if(blMatch==size && tryCounter==1)
                /*Se il numero dei black match è uguale al numero dei colori da indovinare, viene visualizzato il messaggio che
                indica la vittoria della partita.*/
                    printf(WINCOLOR "\n\nComplimenti: hai deficrato la sequenza in un tentativo!" INRESETCOLOR);
                else if(blMatch==size)
                    printf(WINCOLOR "\n\nComplimenti: hai decifrato la sequenza in %i tentativi!" INRESETCOLOR, tryCounter);
            }
            else
            /*Se la sequenza è EXIT, allora il numero dei black match viene posto uguale al numero dei colori da indovinare, causando
            l'uscita dal ciclo.*/
                blMatch=size;
        }while(blMatch<size);


        /*
        //INIZIO BLOCCO DI CHECK
        printf("\nblack: %i", blMatch);
        printf("\nwhite: %i\n", whMatch);
        //FINE BLOCCO DI CHECK
        */

        if(!exitNow)
        /*Se la sequenza inserita dal giocatore non è EXIT, gli viene chiesto se desidera iniziare un'altra partita.*/
        {
            PROMPT
            ignoreInputUntil('\n');

            printf("\n");

            do
            /*Ciclo di controllo valore in input: serve per acquisire un valore valido che permetta al giocatore di iniziare
            un'altra partita o ritornare al menu principale.*/
            {
                printf("Vuoi fare un'altra partita? [Y/N]");
                PROMPT
                replay=getchar();
                ignoreInputUntil('\n');
                if(IS_NOT_YES_OR_NO(replay))
                    printf(ERRORCOLOR "ERRORE: scelta non valida.\n" INRESETCOLOR);
            }while(IS_NOT_YES_OR_NO(replay));
        }
        else
        /*Se la sequenza inserita dal giocatore è EXIT, la variabile replay viene posta ad un valore diverso da 'y' o 'Y' (per facilità di
        lettura qui è stato scelto 'N') causando l'uscita dal ciclo do while e il ritorno immediato al menu iniziale.*/
            replay='N';
    }while(IS_YES(replay));
}


/*La funzione mostra le istruzioni del gioco.*/
bool printHowTo(void)
{
    CLEARSCR
    printf(INWHITE "ISTRUZIONI (pag.1/4)" INRESETCOLOR);
    printf("\n");
    printf("\nIl gioco consiste nel decifrare una sequenza di cerchi colorati determinata dal");
    printf("\ncomputer.");
    printf("\n");
    printf("\nLa sequenza può contenere cerchi dello stesso colore e, a seconda della");
    printf("\nmodalità di gioco, può essere composta da 4 (FACILE), 5 (MEDIO) o ");
    printf("\n6 (DIFFICILE) cerchi.");
    PROMPT
    ignoreInputUntil('\n');

    CLEARSCR
    printf(INWHITE "ISTRUZIONI (pag.2/4)" INRESETCOLOR);
    printf("\n");
    printf("\nIl giocatore interagisce digitando la sequenza di colori tramite i seguenti");
    printf("\ncomandi:");
    printf("\n");
    printf("\nR - rosso   " RBLOCK);
    printf("\nG - giallo  " YBLOCK);
    printf("\nV - verde   " GBLOCK);
    printf("\nB - blu     " BBLOCK);
    printf("\n");
    printf("\nSe, ad esempio, si vuole inserire la sequenza verde, verde, rosso, blu; si");
    printf("\ndevono digitare le lettere VVRB (oppure vvrb) seguite dal tasto invio, senza");
    printf("\nspazi tra di loro.");
    PROMPT
    ignoreInputUntil('\n');

    CLEARSCR
    printf(INWHITE "ISTRUZIONI (pag.3/4)" INRESETCOLOR);
    printf("\n");
    printf("\nOgni volta che il giocatore inserisce una sequenza, questa viene confrontata con");
    printf("\nquella da decifrare e vengono visualizzati i seguenti indizi:");
    printf("\n");
    printf("\n");
    printf(BLACKMATCH " indica un cerchio di colore giusto nella corretta posizione");
    printf("\n");
    printf(WHITEMATCH " indica un cerchio di colore giusto nella posizione sbagliata");
    printf("\n");
    printf("\nIl gioco termina quando il giocatore indovina la sequenza corretta.");
    printf("\n");
    printf("\nIn qualsiasi momento durante una partita, se il giocatore vuole interrompere il");
    printf("\ngioco e tornare al menu principale può digitare EXIT (o exit) seguito dal");
    printf("\ntasto invio.");
    PROMPT
    ignoreInputUntil('\n');

    CLEARSCR
    printf(INWHITE "ISTRUZIONI (pag.4/4)" INRESETCOLOR);
    printf("\n");
    printf("\nNB: in qualsiasi parte del programma, il simbolo > indica che il programma sta");
    printf("\naspettando un input da parte dell'utente.");
    PROMPT
    ignoreInputUntil('\n');

    return true;
}


/*La funzione visualizza la schermata dei crediti.*/
void printCredits(void)
{
    CLEARSCR

    printf(INWHITE "CREDITI" INRESETCOLOR);
    printf("\n\nMastermind è stato interamente sviluppato da Davide Dettori come progetto\nper l'esame di programmazione.\n(Docente: prof. Marco Anisetti, Università degli Studi di Milano)");
    printf("\n\nASCII art del titolo a cura di Patrick Gillespie.\nhttp://patorjk.com/software/taag/");

    PROMPT
    ignoreInputUntil('\n');
}
