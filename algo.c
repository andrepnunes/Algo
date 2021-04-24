#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tri_fusion.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

// Determine quelles taches doivent etre faites pour minimiser les penalites
int ordonancement(int n, p_tache tete, int* tableau){ // sans decalages
	if (!n) return 0;
	p_tache iter = tete;
	for (int i = 1; i < n; ++i) iter = iter->suivant;

	int* temp_1 = (int*)(malloc((n-1)*sizeof(int)));
	int* temp_2 = (int*)(malloc((n-1)*sizeof(int)));
	memset(temp_1, 0, (n-1)*sizeof(int));
	memset(temp_2, 0, (n-1)*sizeof(int));
	int a = iter->penalite + ordonancement(iter->pi, tete, temp_1);
	int b = ordonancement(n-1, tete, temp_2);

	if (a > b)
	{
		for (int i = 0; i < n-1; ++i)
			tableau[i+1] = temp_1[i];
		tableau[0]=n;
		free(temp_1);
		free(temp_2);
		return a;
	}else{
		for (int i = 0; i < n-1; ++i)
			tableau[i+1] = temp_2[i];
		free(temp_1);
		free(temp_2);
		return b;
	}
}

int main(int argc, char const *argv[])
{
	// Création d'une liste chainee de taches
	srand(time(NULL));
	p_tache tete = creation_tache();
	for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE - 1; ++i) tete = append_tache(creation_tache(),tete);

	// affichage de la liste cainee
	printf("\n\nListe des taches a faire: \n");
	print_chaine(tete);
	
	// Triage par rapport à la date limite
	tri_fusion(&tete);

	// Calcul de Pi
	// identification de la tache comptable de date de fin la plus proche
	recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);
	
	printf("\n\nListe triee des taches a faire: \n");
	print_chaine(tete);
	printf("\n\nEmploi du temps: \n----------------\n\n");
	print_edt(tete);
	
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
    
	printf("\nLes tâches à faire sont : \n");
	for (int i = 1; i < nb_taches_final; ++i)
		printf("%d, ", taches_final[nb_taches_final - i]);
	
	printf("%d.", taches_final[0]);


	free_liste(tete);
	return 0;
}
