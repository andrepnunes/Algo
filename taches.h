#include <stdlib.h>
#include <stdio.h>

#define NOMBRE_DE_TACHES_A_FAIRE 10

struct tache{
	int duree_de_fabrication;
	int date_limite;
	int date_de_fin;
	int penalite;
	int pi;
	int effectue;
	struct tache* suivant;
};

typedef struct tache* p_tache;


int random(int debut, int fin){
	return debut + rand()%(fin-debut);
}

// Crée une tache avec des donées aléatoires
p_tache creation_tache(){
	p_tache tache = (p_tache) malloc(sizeof(struct tache));

	tache->duree_de_fabrication = random(2, 10);
	tache->date_limite          = random(tache->duree_de_fabrication, 50);
	tache->date_de_fin          = tache->date_limite;
	tache->penalite             = random(1, 20);
	tache->pi                   = 0;
	tache->effectue             = 0;
	tache->suivant              = NULL;

	return tache;
}
p_tache test_creation_tache(int n){
	p_tache tache = (p_tache) malloc(sizeof(struct tache));

	tache->duree_de_fabrication = 1;
	tache->date_de_fin          = n;
	tache->penalite             = 1;
	tache->pi                   = 0;
	tache->effectue             = 0;
	tache->suivant              = NULL;

	return tache;
}
p_tache test2_creation_tache(int n){
	p_tache tache = (p_tache) malloc(sizeof(struct tache));

	tache->duree_de_fabrication = 4;
	tache->date_de_fin          = n;
	tache->penalite             = 100;
	tache->pi                   = 0;
	tache->effectue             = 0;
	tache->suivant              = NULL;

	return tache;
}

// Ajoute une tache au début d'une liste
p_tache append_tache(p_tache nouvel_element, p_tache tete){
	nouvel_element->suivant = tete;
	return nouvel_element;
}

// Désalloue une liste
void free_liste(p_tache tache){
	// Verifie Valgrind : All heap blocks were freed -- no leaks are possible
	
	// Si tache est NULL -> sortie anticipée
	if (!tache) return;

	p_tache tete = tache;
	p_tache suivant;
	while(tete->suivant){
		suivant = tete->suivant;
		free(tete);
		tete = suivant;
	}
	free(tete);
}

void recherche_tache_compatible(int nb_taches, p_tache tete){
	for (int i = 0; i < nb_taches; ++i)
	{
		p_tache iter = tete;
		int j = nb_taches;
		for (; j > nb_taches-i; --j) iter = iter->suivant;
		int date_limite_i = iter->date_de_fin;
		while(iter->suivant){
			if(iter->suivant->date_de_fin - iter->suivant->duree_de_fabrication >= date_limite_i)
				iter->suivant->pi = i+1;
			iter = iter->suivant;
		}
	}
}


/*******************************    Affichage    *******************************/

// Affiche la structure d'une tache
void print_tache(p_tache tache){
	printf("-----------\n");
	printf("|duree:% 3d|\n", tache->duree_de_fabrication);
	printf("|date :% 3d|\n", tache->date_de_fin);
	printf("|penal:% 3d|\n", tache->penalite);
	printf("|pi   :% 3d|\n", tache->pi);
	printf("-----------\n");
}

// Affiche une chaine de taches
void print_chaine(p_tache tete){
	p_tache iter = tete;
	while(iter){
		print_tache(iter);
		if (iter->suivant) printf("|\nV\n");
		iter = iter->suivant;
	}
}

// Affiche une tache sous forme 'graphique'
void print_tache_edt(p_tache tache){
	int i = 0;
	int date_debut = tache->date_de_fin - tache->duree_de_fabrication;
	for (; i < date_debut; ++i)
		printf(" ");
	for (; i < tache->date_de_fin; ++i)
		printf("-");
	printf("\n");
}

// Affiche un emploi du temps des taches
void print_edt(p_tache tete){
	p_tache iter = tete;
	int i = 1;
	while(iter){
		printf("Tache %2d (pen = %2d)(pi = %2d): ", i++, iter->penalite, iter->pi);
		print_tache_edt(iter);
		iter = iter->suivant;
	}
}
