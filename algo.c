#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tri_fusion.h"

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int penalites[NOMBRE_DE_TACHES_A_FAIRE];

// liste des taches décalées
int tache_decalee[NOMBRE_DE_TACHES_A_FAIRE][NOMBRE_DE_TACHES_A_FAIRE];

// Determine quelles taches doivent etre faites pour minimiser les penalites
int ordonancement(int n, p_tache tete, int* tableau){

	if (!n) return 0;

	// iter prend la valeur du néme element de la liste
	p_tache iter = tete;
	for (int i = 1; i < n; ++i) iter = iter->suivant;


	/***** Hypothèse 1: La tâche courante ne fait pas partie de l'ordonnancement optimal *****/

	//if(iter->penalite/iter->duree_de_fabrication > iter->suivant->penalite/iter->suivant->duree_de_fabrication )
	// Dans le cadre de l'hypothèse effectuée, l'ordonnancement optimal est celui de la tâche suivante
	// On stockera l'ordonnancement optimal dans le tableau temp_2. 
	int* temp_2 = (int*)(malloc((n-1)*sizeof(int)));
	memset(temp_2, 0, (n-1)*sizeof(int));
	int b = ordonancement(n-1, tete, temp_2);


	/***** Hypothèse 2: La tâche courante fait partie de l'ordonnancement optimal *****/


	// Sâchant que la tache courante sera effectuée, on décale toutes les tâches non compatibles (car leur date de fin doit être inférieure à la date de début de la tâche courante) 
	p_tache iter_decalage = tete;
	int counter = 0;

	int flag_aucun_decalage_effectue = 1;
	// parcourt toutes les taches de la tete a la tache courante
	while(iter_decalage && counter < n-1){
		if (iter_decalage->date_de_fin > iter->date_de_fin - iter->duree_de_fabrication) // si une tache est incompatible (sa date de fin est apres la date de debut courante)
		{
			if ( iter->date_de_fin - iter->duree_de_fabrication >= iter_decalage->duree_de_fabrication) // s'il est possible de la decaler sans depasser 0 
			{
				// On sauvegarde leur date de fin actuelle
				tache_decalee[n-1][counter] = iter_decalage->date_de_fin;
				penalites[counter] = 0;

				// On décale la date de fin
				iter_decalage->date_de_fin = iter->date_de_fin - iter->duree_de_fabrication;

				// On indique que des décalages ont été éffectués
				flag_aucun_decalage_effectue = 0;
			}else{

				// S'il n'est pas possible de décaler une tache (car la duree de fabrication est trop longue)
				// On supprime sa penalité pour qu'elle soit ignorée par l'algorithme
				if (iter_decalage->penalite)
				{
					// On sauvegarde sa penalité dans un tableau pour pouvoir la récupérer plus tard
					penalites[counter] = iter_decalage->penalite;

					// On supprime la pénalité de la tache
					iter_decalage->penalite = 0;
				}
			}
		}
		counter++;
		iter_decalage = iter_decalage->suivant;
	}

	// Si des décalages ont été faits, il faut mettre à jour les taches compatibles (pi)
	if (!flag_aucun_decalage_effectue) recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);

	// Dans le cadre de l'hypothèse effectuée, l'ordonnancement optimal est celui de la tâche suivante suivi de la tache courante
	// On stockera l'ordonnancement optimal dans le tableau temp_2. 
	int* temp_1 = (int*)(malloc((n-1)*sizeof(int)));
	memset(temp_1, 0, (n-1)*sizeof(int));
	int a = iter->penalite + ordonancement(iter->pi, tete, temp_1);

	// remise des penalites supprimees
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

	// int a : Penalité totale évitée de l'ordonnancement opmimal avec la tache courante
	// int b : Penalité totale évitée de l'ordonnancement opmimal sans la tache courante
	if (a > b)
	{
		// Si a > b, alors on evite plus de pénalités en faisant la tache courante

		// Le tableau temp_1 contient les tâches éffectuées dans l'ordonnancement 'a'
		// On recopie ces tâches  dans 'tableau' et on ajoute à la première case la tâche courante
		for (int i = 0; i < n-1; ++i)
			tableau[i+1] = temp_1[i];
		tableau[0]=n;

		// on libère la mémoire allouée
		free(temp_1);
		free(temp_2);

		// On renvoie la penalité évitée par cet ordonnancement (ie. avec la pénalité de la tâche courante)
		return a;
	}else{
		
		// Si b > a, alors on evite plus de pénalités si on ne fait pas la tache courante

		// Alors, on doit enlever les décalages qui ont été faits pour l'hypothèse 2 (qui est fausse)
		if (!flag_aucun_decalage_effectue) {
			iter_decalage = tete;
			counter = 0;
			while(iter_decalage && counter < n-1){
				if (tache_decalee[n-1][counter])
					iter_decalage->date_de_fin = tache_decalee[n-1][counter];
				counter++;
				iter_decalage = iter_decalage->suivant;
			}

			recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);
		}

		// Le tableau temp_2 contient les tâches éffectuées dans l'ordonnancement 'b'
		// On recopie ces tâches  dans 'tableau' et on s'assure que la première case de la tâche courante
		for (int i = 0; i < n-1; ++i)
			tableau[i+1] = temp_2[i];
		tableau[0]=0;

		// on libère la mémoire allouée
		free(temp_1);
		free(temp_2);

		// On renvoie la penalité évitée par cet ordonnancement (ie. sans la pénalité de la tâche courante)
		return b;
	}
}

// Determine quelles taches doivent etre faites pour minimiser les penalites
int ordonancement_opt(int n, p_tache tete, int* tableau){

	if (!n) return 0;

	// iter prend la valeur du néme element de la liste
	p_tache iter = tete;
	for (int i = 1; i < n; ++i) iter = iter->suivant;

	// On décale toutes les tâches non compatibles (car leur date de fin doit être inférieure à la date de début de la tâche courante) 
	p_tache iter_decalage = tete;
	int counter = 0;

	int flag_aucun_decalage_effectue = 1;
	// parcourt toutes les taches de la tete a la tache courante
	while(iter_decalage && counter < n-1){
		if (iter_decalage->date_de_fin > iter->date_de_fin - iter->duree_de_fabrication) // si une tache est incompatible (sa date de fin est apres la date de debut courante)
		{
			if ( iter->date_de_fin - iter->duree_de_fabrication >= iter_decalage->duree_de_fabrication) // s'il est possible de la decaler sans depasser 0 
			{
				// On sauvegarde leur date de fin actuelle
				tache_decalee[n-1][counter] = iter_decalage->date_de_fin;
				penalites[counter] = 0;

				// On décale la date de fin
				iter_decalage->date_de_fin = iter->date_de_fin - iter->duree_de_fabrication;

				// On indique que des décalages ont été éffectués
				flag_aucun_decalage_effectue = 0;
			}else{

				// S'il n'est pas possible de décaler une tache (car la duree de fabrication est trop longue)
				// On supprime sa penalité pour qu'elle soit ignorée par l'algorithme
				if (iter_decalage->penalite)
				{
					// On sauvegarde sa penalité dans un tableau pour pouvoir la récupérer plus tard
					penalites[counter] = iter_decalage->penalite;

					// On supprime la pénalité de la tache
					iter_decalage->penalite = 0;
				}
			}
		}
		counter++;
		iter_decalage = iter_decalage->suivant;
	}

	// Si des décalages ont été faits, il faut mettre à jour les taches compatibles (pi)
	if (!flag_aucun_decalage_effectue) recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);

	int a = iter->penalite + ordonancement_opt(iter->pi, tete, tableau + 1);

	// remise des penalites supprimees
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

	tableau[0]=n;

	return a;
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
	p_tache tete = creation_tache_non_aleatoire(3,4,12);
	tete = append_tache(creation_tache_non_aleatoire(5,5, 12),tete);
	tete = append_tache(creation_tache_non_aleatoire(5,8, 17),tete);
	tete = append_tache(creation_tache_non_aleatoire(2,15,18),tete);
	tete = append_tache(creation_tache_non_aleatoire(5,15,13),tete);
	tete = append_tache(creation_tache_non_aleatoire(6,22, 9),tete);
	tete = append_tache(creation_tache_non_aleatoire(4,22, 9),tete);
	tete = append_tache(creation_tache_non_aleatoire(7,27,10),tete);
	tete = append_tache(creation_tache_non_aleatoire(2,33,15),tete);
	tete = append_tache(creation_tache_non_aleatoire(8,50,19),tete);
#endif

	// affichage de la liste cainee
	printf("\n\nListe des taches a faire: \n");
	print_chaine(tete);
	
	// Triage par rapport à la date limite
	tri_fusion_opt(&tete);

	// Calcul de Pi
	// identification de la tache comptable de date de fin la plus proche
	recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);
	
	printf("\n\nListe triee des taches a faire: \n");
	print_chaine(tete);
	printf("\n\nEmploi du temps: \n----------------\n\n");
	print_edt(tete);

	int taches_faites[NOMBRE_DE_TACHES_A_FAIRE];
	for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE; ++i) taches_faites[i] = 0;
	printf("\nPenalite evitee maximale = %d\n", ordonancement_opt(NOMBRE_DE_TACHES_A_FAIRE, tete, taches_faites));

	// On compte le nombre total de taches effectues (nombre de cases non nulles dans 'taches_faites')
    int nb_taches_final = 0;
    for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE; ++i)
    	if (taches_faites[i]) nb_taches_final++;

	// On crée le tableau 'taches_final' avec les valeurs non nulles du tableau 'taches_faites'
    int k = 0;
    int* taches_final =(int*) (malloc(nb_taches_final*sizeof(int)));
    for (int i = 0; i < NOMBRE_DE_TACHES_A_FAIRE; ++i)
    	if (taches_faites[i]) taches_final[k++] = taches_faites[i];
	
	// Pour l'affichage 
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
	recherche_tache_compatible(NOMBRE_DE_TACHES_A_FAIRE, tete);

	print_edt(tete);

	printf("\nLes tâches à faire sont : \n");
	for (int i = 1; i < nb_taches_final; ++i)
		printf("%d, ", taches_final[nb_taches_final - i]);
	
	printf("%d.", taches_final[0]);

	free(taches_final);
	free_liste(tete);
	return 0;
}
