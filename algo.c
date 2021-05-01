#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tri_fusion.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int penalites[NOMBRE_DE_TACHES_A_FAIRE];
int tache_decalee[NOMBRE_DE_TACHES_A_FAIRE][NOMBRE_DE_TACHES_A_FAIRE];

// Determine quelles taches doivent etre faites pour minimiser les penalites
int ordonancement(int n, p_tache tete, int* tableau){ // sans decalages
	if (!n) return 0;
	p_tache iter = tete;
	for (int i = 1; i < n; ++i) iter = iter->suivant;


	int* temp_1 = (int*)(malloc((n-1)*sizeof(int)));
	int* temp_2 = (int*)(malloc((n-1)*sizeof(int)));
	memset(temp_1, 0, (n-1)*sizeof(int));
	memset(temp_2, 0, (n-1)*sizeof(int));

	int b = ordonancement(n-1, tete, temp_2);

	// decale toutes les taches non compatibles
	p_tache iter_decalage = tete;
	int counter = 0;

	// parcourt toutes les taches de la tete a la tache courante
	while(iter_decalage && counter < n-1){
		// si une tache est incompatible (sa date de fin est apres ma date de debut)
		if (iter_decalage->date_de_fin > iter->date_de_fin - iter->duree_de_fabrication)
		{
			// si je peux la decaler sans depasser 0 
			if ( iter->date_de_fin - iter->duree_de_fabrication >= iter_decalage->duree_de_fabrication)
			{
				tache_decalee[n-1][counter] = iter_decalage->date_de_fin;
				penalites[counter] = 0;
				iter_decalage->date_de_fin = iter->date_de_fin - iter->duree_de_fabrication;
			}else{
				if (iter_decalage->penalite)
				{
					penalites[counter] = iter_decalage->penalite;
					iter_decalage->penalite = 0;

				}
			}
		}
		counter++;
		iter_decalage = iter_decalage->suivant;
	}
	recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);
	int a = iter->penalite + ordonancement(iter->pi, tete, temp_1);

	// remise des penalites pour les non decalees
	iter_decalage = tete;
	counter = 0;
	while(iter_decalage && counter < n){
		if (penalites[counter]){
			iter_decalage->penalite = penalites[counter];
			penalites[counter] = 0;
		}
		counter++;
		iter_decalage = iter_decalage->suivant;
	}

	if (a > b)
	{
		for (int i = 0; i < n-1; ++i)
			tableau[i+1] = temp_1[i];
		tableau[0]=n;
		free(temp_1);
		free(temp_2);
		return a;
	}else{
		
		// enlever les decalages
		iter_decalage = tete;
		counter = 0;
		while(iter_decalage && counter < n-1){
			if (tache_decalee[n-1][counter])
				iter_decalage->date_de_fin = tache_decalee[n-1][counter];
			counter++;
			iter_decalage = iter_decalage->suivant;
		}

		recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);

		for (int i = 0; i < n-1; ++i)
			tableau[i+1] = temp_2[i];
		tableau[0]=0;

		free(temp_1);
		free(temp_2);
		return b;
	}
}

#define aleatoire 1
int main(int argc, char const *argv[])
{
//memset(tache_decalee, 0, (NOMBRE_DE_TACHES_A_FAIRE*NOMBRE_DE_TACHES_A_FAIRE)*sizeof(int));
//memset(penalites, 0, (NOMBRE_DE_TACHES_A_FAIRE)*sizeof(int));
	// Création d'une liste chainee de taches aleatoires

#ifdef aleatoire
	srand(time(NULL));
	p_tache tete = creation_tache();
	for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE - 1; ++i) tete = append_tache(creation_tache(),tete);
#else

//	p_tache tete = creation_tache_non_aleatoire(4,47,18);
//	tete = append_tache(creation_tache_non_aleatoire(6,40,13),tete);
//	tete = append_tache(creation_tache_non_aleatoire(8,39,12),tete);
//	tete = append_tache(creation_tache_non_aleatoire(6,38,8),tete);
//	tete = append_tache(creation_tache_non_aleatoire(9,25,15),tete);
//	tete = append_tache(creation_tache_non_aleatoire(6,23,7),tete);
//	tete = append_tache(creation_tache_non_aleatoire(9,20,6),tete);
//	tete = append_tache(creation_tache_non_aleatoire(4,17,16),tete);
//	tete = append_tache(creation_tache_non_aleatoire(3,16,2),tete);
//	tete = append_tache(creation_tache_non_aleatoire(4,15,12),tete);

	p_tache tete = creation_tache_non_aleatoire(6,48,14);
	tete = append_tache(creation_tache_non_aleatoire(7,47, 5),tete);
	tete = append_tache(creation_tache_non_aleatoire(3,37, 1),tete);
	tete = append_tache(creation_tache_non_aleatoire(4,31, 1),tete);
	tete = append_tache(creation_tache_non_aleatoire(2,30,13),tete);
	tete = append_tache(creation_tache_non_aleatoire(2,28, 1),tete);
	tete = append_tache(creation_tache_non_aleatoire(8,27,10),tete);
	tete = append_tache(creation_tache_non_aleatoire(4,23,17),tete);
	tete = append_tache(creation_tache_non_aleatoire(4,23, 7),tete);
	tete = append_tache(creation_tache_non_aleatoire(9,17,15),tete);
#endif
	// affichage de la liste cainee
	printf("\n\nListe des taches a faire: \n");
	//print_chaine(tete);
	
	// Triage par rapport à la date limite
	tri_fusion(&tete);

	// Calcul de Pi
	// identification de la tache comptable de date de fin la plus proche
	recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);
	
	printf("debut\n");
	printf("\n\nListe triee des taches a faire: \n");
	//print_chaine(tete);
	printf("\n\nEmploi du temps: \n----------------\n\n");
	print_edt(tete);
	
	p_tache queue = tete;
	while(queue->suivant) queue = queue->suivant;

	int taches_faites[NOMBRE_DE_TACHES_A_FAIRE];
	for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE; ++i) taches_faites[i] = 0;
	printf("\nPenalite evitee maximale = %d\n", ordonancement(NOMBRE_DE_TACHES_A_FAIRE, tete, taches_faites));



	//effacer les 0
    int nb_taches_final = 0;
    for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE; ++i)
    	if (taches_faites[i]) nb_taches_final++;

    int k = 0;
    int* taches_final =(int*) (malloc(nb_taches_final*sizeof(int)));
    for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE; ++i)
    	if (taches_faites[i]) taches_final[k++] = taches_faites[i];
	
	int counter = 1;
	int index = nb_taches_final-1;
	int prev = 0;
	p_tache iter = tete;
	while(iter){
		if (counter == taches_final[index])
		{
			iter->date_de_fin = prev + iter->duree_de_fabrication;
			prev = iter->date_de_fin;
			if(index) index --;
		}else{
			iter->date_de_fin = iter->date_limite;
		}
		counter++;
		iter = iter->suivant;
	}

	print_edt(tete);

	printf("\nLes tâches à faire sont : \n");
	for (int i = 1; i < nb_taches_final; ++i)
		printf("%d, ", taches_final[nb_taches_final - i]);
	
	printf("%d.", taches_final[0]);


	free_liste(tete);
	return 0;
}
