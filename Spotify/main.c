#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DIM_TITLE 50
#define DIM_GENRE 50
#define DIM_NAME 50

//struttura che rappresenta una CANZONE
typedef struct song {
    char title[DIM_TITLE];
    char genre[DIM_GENRE];
    float length;
    struct song *prev;
    struct song *next;
} Song;

//struttura che rappresenta una PLAYLIST
typedef struct playlist {
    char name[DIM_NAME];
    Song *top;
} PlayList;

//funzioni (gia' definite) per l'acquisizione e per la stampa di una canzone
Song *acquireSong(); //acquisisce i dati di UNA canzone chiedendoli all'utente
void printSong(Song *s); //stampa i dati relativi ad UNA canzone (passata tramite puntatore)

//prototipi funzioni STUDENTE
void insertSong(PlayList *pl, Song *s);

Song *findSong(PlayList *pl, char *title);

void modifySong(PlayList *pl, Song *s);

void deleteSong(PlayList *pl, Song *s);

void printPlayList(PlayList *pl);

Song *nextSong(Song *s);

Song *prevSong(Song *s);

void mergePlayList(PlayList *plA, PlayList *plB, PlayList *plC);

int main() {
    //Creazione di una PLAYLIST. Utilizzare e riempire questa playlist
    PlayList p;
    strcpy(p.name, "My favourite songs");
    p.top = NULL;

    //altre chiamate e istruzioni per testare il funzionamento delle funzioni create
    Song *s1 = acquireSong();
    Song *s2 = acquireSong();
    Song *s3 = acquireSong();
    Song *s4 = acquireSong();
    Song *s5 = acquireSong();

    insertSong(&p, s1);
    insertSong(&p, s2);
    insertSong(&p, s3);
    insertSong(&p, s4);
    printPlayList(&p);
    insertSong(&p, s5);
    printPlayList(&p);

    free(s1);
    free(s2);
    free(s3);
    free(s4);
    free(s5);
    return 0;
}


/* ---------------------------------------------
* funzioni da NON modificare
* --------------------------------------------*/
Song *acquireSong() {
    Song *new_s = (Song *) malloc(sizeof(Song));
    printf("\nInsert title -> ");
    scanf("%[^\n]s", new_s->title);
    getchar();

    printf("Insert genre -> ");
    scanf("%[^\n]s", new_s->genre);
    getchar();

    printf("Insert length -> ");
    scanf("%f", &(new_s->length));
    getchar();

    new_s->prev = NULL;
    new_s->next = NULL;

    return new_s;
}

void printSong(Song *s) {
    printf("TITLE:\t %s \n", s->title);
    printf("GENRE:\t %s \n", s->genre);
    printf("LENGTH\t %.2f \n\n", s->length);
}

/* ---------------------------------------------
                funzioni STUDENTE
   --------------------------------------------*/
/**
 * @brief funzione che inserisce una canzone in una playList
 * @description inserisce una canzone (creata con la funzione acquire ) nella lista in maniera ordinata
 * @param pl puntatore alla playList
 * @param s puntatore alla canzone da aggiungere
 */
void insertSong(PlayList *pl, Song *s) {
    if (pl->top == NULL) {
        s->next = NULL;
        pl->top = s;
    } else {
        Song *tmp_prev = NULL;
        Song *tmp_next = pl->top;
        while (tmp_next != NULL && strcmp(tmp_next->title, s->title) <= 0) {
            tmp_prev = tmp_next;
            tmp_next = tmp_next->next;
        }
        if (tmp_next == NULL) {
            if (tmp_prev == NULL)
                exit(EXIT_FAILURE);
            s->next = NULL;
            s->prev = tmp_prev;
            tmp_prev->next = s;
        } else if (tmp_prev == NULL) {
            s->next = tmp_next;
            s->prev = NULL;
            tmp_next->prev = s;
            pl->top = s;
        } else {
            s->next = tmp_next;
            s->prev = tmp_prev;
            tmp_prev->next = s;
            tmp_next->prev = s;
        }
    }
}

/**
 * @brief funzione che trova una canzone dato il titolo
 * @description cerca una canzone nella lista (tramite il titolo) e restituisce il suo puntatore
 * @param pl puntatore alla playList
 * @param title titolo della canzone da cercare
 * @return puntatore della canzone, se non esiste false
 */
Song *findSong(PlayList *pl, char *title) {
    Song *tmp = pl->top, *res = NULL;
    bool c = false;
    while (tmp != NULL) {
        if (strcmp(tmp->title, title) == 0) {
            c = true;
            res = tmp;
        }
        tmp = tmp->next;
    }
    if (!c) {
        printf("La canzone non e' presente nella playlist che hai inserito!\n");
        res = NULL;
    }
    return res;
}

/**
 * @brief funzione che stampa la playList
 * @description stampa l’intera playlist di canzoni
 * @param pl puntatore alla playList da stampare
 * */
void printPlayList(PlayList *pl) {
    Song *s = pl->top;
    printf("Name: %s\n", pl->name);
    while (s != NULL) {
        printSong(s);
        s = s->next;
    }
}

/**
 * @brief funzione che data una canzone ne modifica il contenuto
 * @description modifica una canzone gia’ presente (trovata tramite la funzione findSong) e la riposiziona nella posizione corretta
 * @param pl puntatore alla playList
 * @param s puntatore alla canzone da modificare
 */
void modifySong(PlayList *pl, Song *s) {
    Song *tmp = findSong(pl, s->title);
    if (tmp != NULL) {
        printf("Inserisci il nuovo titolo: ");
        scanf(" %[^\n]s", tmp->title);
        getchar();
        printf("Inserisci il nuovo genere: ");
        scanf(" %[^\n]s", tmp->genre);
        getchar();
        printf("Inserisci la nuova durata: ");
        scanf("%f", &(tmp->length));
        getchar();
    }
}

/**
 * @brief funzione che permette di eliminare una canzone da una playList
 * @description elimina una canzone gia’ presente (trovata tramite la funzione findSong) e dealloca l’elemento
 * @param pl puntatore alla playList
 * @param s puntatore alla canzone da eliminare
 */
void deleteSong(PlayList *pl, Song *s) {
    Song *tmp = findSong(pl, s->title);
    if (tmp != NULL) {
        if (tmp->prev == NULL) {
            pl->top = tmp->next;
            if (tmp->next != NULL)
                tmp->next->prev = NULL;
        } else if (tmp->next == NULL) {
            tmp->prev->next = NULL;
        } else {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
        }
        free(tmp);
    }
}

/**
 * @brief funzione che permette di trovare la canzone successiva
 * @description restituisce il puntatore alla canzone successiva rispetto a quella passata come puntatore se presente, altrimenti restituisce NULL
 * @param s puntatore alla canzone
 * @return restituisce il puntatore della canzone successiva, NULL se non esiste
 */
Song *nextSong(Song *s) {
    Song *res = NULL;
    if (s->next != NULL) {
        res = s->next;
    }
    return res;
}

/**
 * @brief funzione che permette di trovare la canzone precedente
 * @description restituisce il puntatore alla canzone precedente rispetto a quella passata come puntatore se presente, altrimenti restituisce NULL
 * @param s puntatore alla canzone
 * @return restituisce il puntatore della canzone precedente, NULL se non esiste
 */
Song *prevSong(Song *s) {
    Song *res = NULL;
    if (s->prev != NULL) {
        res = s->prev;
    }
    return res;
}

/**
 * @brief funzione che permette di fondere due playList (A e B) in un'unica playList ordinata (C)
 * @description fonde le liste plA e plB in un’unica lista plC (anch’essa passata come parametro). NESSUN nuovo nodo deve essere allocato
 * @param plA prima playList di provenienza
 * @param plB seconda playList di provenienza
 * @param plC playList di destinazione
 */
void mergePlayList(PlayList *plA, PlayList *plB, PlayList *plC) {
    plC->top = plA->top;
    Song *tmp = plB->top;
    while (tmp!=NULL){
        insertSong(plC, tmp);
        tmp = tmp->next;
    }
}