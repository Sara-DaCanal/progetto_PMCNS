# progetto_PMCNS
Progetto sviluppato per il corso di Performance Modeling Of Computer Systems And Networks (P.M.C.S.N) del corso di studi di Ingegneria Informatica Magistrale dell'università di Roma Tor Vergata.
## Obbiettivo
Il progetto ha lo scopo di simulare gli accessi, i tempi di attesa e i tempi di servizio al pronto soccorso A.O. VERONA (BORGO ROMA), situato in Veneto, Italia.
Il nostro obiettivo è modellare il sistema trovando la distribuzione ottima di serventi per garantire che non vengano superati i seguenti tempi di attesa:<br />
• I codici rossi dovrebbero essere presi in carico immediatamente<br />
• Per i codici gialli l’attesa dovrebbe essere di non più di 30 minuti<br />
• Per i codici verdi l’attesa dovrebbe essere di non più di 60 minuti<br />
• Per i codici bianchi l’attesa dovrebbe essere di non più di 120 minuti<br />
A questo va aggiunto che l’accettazione non dovrebbe superare i quindici minuti totali tra attesa e servizio.<br />
Per avvicinarci il più possibile a questi tempi abbiamo un budget limitato. I diversi lavoratori hanno stipendi differenti in base al centro in cui lavorano, dato che ricoprono mansioni differenti:<br />
• I medici responsabili di trattare i codici rossi costano 7.000€/mese<br />
• I medici che si occupano di traumatologia e primo intervento costano 4.200€/mese<br />
• I problemi di minore entità possono essere gestiti da infermieri con costo 2.800€/mese<br />
• I lavoratori dell’accettazione costano 1.700€/mese<br />
Il budget che consideriamo è di 65.000 €/mese.<br />
Oltre al budget, anche un altro vincolo va rispettato, ovvero mantenere il numero di morti al minimo possibile, l’ideale sarebbe sotto lo 0.5%.<br />
Successivamente si cambia il modello per separare i codici gialli in due code diverse, codici arancioni e blu, e si vede se il sistema migliora e se è possibile diminuire il numero di server senza aumentare la probabilità di morte. Le code arancioni e blu dovrebbero restare comunque entrambe sotto i trenta minuti, in particolare, essendo i casi arancioni più gravi si vuole tentare di mantenere questa coda sotto i quindici minuti.<br />
## Come compilare il progetto
il progetto é stato sviluppato interamente in C, utilizzando alcune API di sistemi operativi unix-like quali (Kali e Mac OS) non garantiamo la compatibilità su altri S.O.<br />
Quando si é scaricato il repositori github é necessario da terminale entrare nella cartella Simulatore_ProntoSoccorso ed eseguire il comando make progetto.<br />
```
cd Simulatore_ProntoSoccorso
make progetto
```
