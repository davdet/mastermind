#mastermind

Davide Dettori (#909387)
Università degli Studi di Milano, esame di Programmazione
Docente: prof. Marco Anisetti

DESCRIZIONE DEL PROGETTO MASTERMIND

L’idea alla base è stata quella di sviluppare il gioco Mastermind, creando un’interfaccia pulita che non confondesse l’utente e dando al giocatore la possibilità di scegliere tra tre diverse difficoltà di gioco:
    • FACILE: il giocatore deve decifrare una sequenza di 4 cerchi colorati;
    • MEDIO: il giocatore deve decifrare una sequenza di 5 cerchi colorati;
    • DIFFICILE: il giocatore deve decifrare una sequenza di 6 cerchi colorati.
Per tutte le modalità di gioco, i cerchi possono essere di quattro differenti colori, anche ripetibili: rosso, giallo, verde e blu.

Nel far ciò si è cercato di ridurre al minimo il codice del main, scrivendo opportune funzioni alle quali delegare lo svolgimento dei singoli compiti necessari. Il risultato è un codice per quanto più possibile suddiviso in blocchi indipendenti che rendono eventuali modifiche più agevoli. Il tutto è stato inoltre commentato dettagliatamente.

Particolare attenzione è stata inoltre posta ai controlli sugli input, in maniera tale da ridurre al minimo le possibilità di loop.

Il programma è stato scritto utilizzando Code::Blocks IDE per Linux. Data la massiccia presenza di parti grafiche quali testi colorati e caratteri Unicode, si consiglia di compilare lo script utilizzando un compilatore C per Linux. Utilizzando Windows queste formattazioni potrebbero venire visualizzate in maniera errata.

Mastermind è stato interamente sviluppato da Davide Dettori, ad eccezion fatta dell’ASCII art iniziale per la quale è stato utilizzato il software Text to ASCII Art Generator (TAAG) di Patrick Gillespie, disponibile al seguente indirizzo: http://patorjk.com/software/taag


1 – PREPROCESSORE 

Per esigenze di consegna, le direttive dedicate al preprocessore non sono state inserite in un file include ma sono tutte indicate all’inizio dello script.

In questa sezione si trovano le macro dedicate alla formattazione delle parti grafiche e alle condizioni di alcuni degli if presenti nel codice.


2 – VARIABILI GLOBALI E DICHIARAZIONI DELLE FUNZIONI

All’inizio dello script si trovano anche le dichiarazioni delle funzioni e delle variabili globali.

Il programma utilizza tre variabili globali, due delle quali vengono modificate dalla medesima funzione tramite l’utilizzo di un puntatore.


3 – MAIN

Come già sottolineato precedentemente, il codice del main è stato per quanto possibile ridotto al minimo: lo scopo del main è quello di visualizzare il menu di gioco iniziale e di chiamare le funzioni a seguito di una scelta operata dall’utente.


4 – LE FUNZIONI

Il programma utilizza undici differenti funzioni. Nello script, immediatamente sopra la definizione di ogni funzione, si trova un commento che ne descrive le finalità. Il codice di alcune di queste funzioni è di immediata lettura, mentre per altre è stato necessario commentare le singole istruzioni contenute al loro interno.


5 – NOTE

A scopo dimostrativo, nello script sono state lasciate commentate alcune parti necessarie in fase di sviluppo per controllare i valori delle variabili. Queste parti sono individuabili dalle diciture //INIZIO BLOCCO DI CHECK, //FINE BLOCCO DI CHECK e //ISTRUZIONE DI CHECK.

Per lo stesso motivo è stata lasciata commentata la funzione charToInt la quale, dopo essere stata sviluppata, si è rivelata inutile ai fini del programma.

Qualora per esigenze di controllo si volesse far partire il programma visualizzando sin da subito la sequenza da decifrare, basta decommentare la funzione printSequence alla linea 497.
