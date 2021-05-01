#include "taches.h"

// fusionne deux listes en triant les elements
p_tache fusion(p_tache liste_1, p_tache liste_2)
{
    p_tache liste_fusione = NULL;
 
    if (!liste_1) return (liste_2);
    if (!liste_2) return (liste_1);
 
    if (liste_1->date_limite <= liste_2->date_limite) {   // '<=' ordre croissant 
        liste_fusione = liste_1;
        liste_fusione->suivant = fusion(liste_1->suivant, liste_2);
        liste_fusione->suivant->precedent = liste_fusione;
    }
    else {
        liste_fusione = liste_2;
        liste_fusione->suivant = fusion(liste_1, liste_2->suivant);
        liste_fusione->suivant->precedent = liste_fusione;
    }
    return (liste_fusione);
}
 

// divise une liste chaînee en deux
void division(p_tache tete, p_tache* liste_1, p_tache* liste_2)
{
	/*
		On parcourt une liste chaînée avec 2 pointeurs: le premier (milieu_de_liste) avec un pas de 1, le deuxieme (fin_de_liste) avec un pas de 2
		Lorsque le deuxieme pointeur arrive a la fin de la liste, le premier est au milieu.
	*/
    p_tache fin_de_liste;
    p_tache milieu_de_liste;

    milieu_de_liste = tete;
    fin_de_liste = tete->suivant;
 
    while (fin_de_liste) {
        fin_de_liste = fin_de_liste->suivant;
        if (fin_de_liste) {
            milieu_de_liste = milieu_de_liste->suivant;
            fin_de_liste = fin_de_liste->suivant;
        }
    }

    *liste_1 = tete;
    *liste_2 = milieu_de_liste->suivant;
    milieu_de_liste->suivant = NULL;
}

void tri_fusion(p_tache* p_tete)
{
    p_tache tete = *p_tete;
    p_tache liste_1;
    p_tache liste_2;
 
    if (!tete) return;
    if (!tete->suivant) return;
 
    division(tete, &liste_1, &liste_2);
 
    tri_fusion(&liste_1);
    tri_fusion(&liste_2);
 
    *p_tete = fusion(liste_1, liste_2);
}