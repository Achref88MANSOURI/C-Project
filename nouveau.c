#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Définition de la structure Patient
typedef struct Patient {
    int id;
    char nom[100];
    int age;
    char sexe[10];
    char telephone[8];
    struct Patient* suivant; // Pointeur vers le patient suivant (liste chaînée)
} Patient;

Patient* creerPatient(int id, char* nom, int age, char* sexe, char* telephone) {
    // Allouer de la mémoire pour un nouveau patient
    Patient* nouveauPatient = (Patient*)malloc(sizeof(Patient));
    
    // Initialiser les informations du patient
    nouveauPatient->id = id;
    strcpy(nouveauPatient->nom, nom);
    nouveauPatient->age = age;
    strcpy(nouveauPatient->sexe, sexe);
    strcpy(nouveauPatient->telephone, telephone);
    nouveauPatient->suivant = NULL; // Le prochain patient n'existe pas encore (liste vide)
    
    return nouveauPatient;
}

void ajouterPatient(Patient** tete, int id, char* nom, int age, char* sexe, char* telephone) {
    // Créer un nouveau patient
    Patient* nouveauPatient = creerPatient(id, nom, age, sexe, telephone);
    
    // Si la liste est vide, le nouveau patient devient la tête de la liste
    if (*tete == NULL) {
        *tete = nouveauPatient;
    } else {
        // Sinon, on parcourt la liste jusqu'à la fin pour ajouter le patient
        Patient* temp = *tete;
        while (temp->suivant != NULL) {
            temp = temp->suivant;
        }
        temp->suivant = nouveauPatient;
    }
}

void afficherPatients(Patient* tete) {
    Patient* temp = tete;
    
    // Si la liste est vide
    if (temp == NULL) {
        printf("Aucun patient à afficher.\n");
        return;
    }
    
    // Parcourir la liste et afficher les informations
    while (temp != NULL) {
        printf("ID: %d\n", temp->id);
        printf("Nom: %s\n", temp->nom);
        printf("Age: %d\n", temp->age);
        printf("Sexe: %s\n", temp->sexe);
        printf("Telephone: %s\n\n", temp->telephone);
        temp = temp->suivant;
    }
}

// Fonction pour sauvegarder un patient dans un fichier (ajout à la fin du fichier)
void sauvegarderPatientDansFichier(Patient* patient) {
    FILE *fichier = fopen("patients.txt", "a"); // Ouvrir le fichier en mode ajout
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    
    // Écrire les informations du patient dans le fichier (chaque champ séparé par une virgule)
    fprintf(fichier, "%d,%s,%d,%s,%s\n", 
            patient->id, 
            patient->nom, 
            patient->age, 
            patient->sexe, 
            patient->telephone);
    
    fclose(fichier); // Fermer le fichier après l'écriture
}

// Fonction pour charger les patients depuis un fichier et les ajouter à la liste chaînée
void chargerPatientsDepuisFichier(Patient **tete) {
    FILE *fichier = fopen("patients.txt", "r"); // Ouvrir le fichier en mode lecture
    if (fichier == NULL) {
        printf("Aucun fichier trouvé, démarrage avec une liste vide.\n");
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        Patient* nouveau = (Patient*)malloc(sizeof(Patient));
        if (nouveau == NULL) {
            printf("Erreur d'allocation mémoire.\n");
            fclose(fichier);
            return;
        }
        
        // Extraire les données du fichier et les stocker dans la structure Patient
        sscanf(ligne, "%d,%49[^,],%d,%9[^,],%14[^\n]", 
               &nouveau->id, 
               nouveau->nom, 
               &nouveau->age, 
               nouveau->sexe, 
               nouveau->telephone);
        
        // Ajouter le nouveau patient à la liste chaînée
        nouveau->suivant = *tete;
        *tete = nouveau;
    }

    fclose(fichier); // Fermer le fichier après la lecture
}

// Fonction pour réécrire tous les patients dans le fichier (utile après une suppression ou mise à jour)
void reecrireFichier(Patient* tete) {
    FILE *fichier = fopen("patients.txt", "w"); // Ouvrir le fichier en mode écriture (écrase le fichier existant)
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    Patient* temp = tete;
    while (temp != NULL) {
        // Réécrire chaque patient dans le fichier
        fprintf(fichier, "%d,%s,%d,%s,%s\n", 
                temp->id, 
                temp->nom, 
                temp->age, 
                temp->sexe, 
                temp->telephone);
        temp = temp->suivant;
    }

    fclose(fichier); // Fermer le fichier après l'écriture
}

// Fonction pour rechercher un patient par son ID
Patient* rechercherPatient(Patient* tete, int id) {
    Patient* temp = tete;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp; // Retourne le patient si l'ID correspond
        }
        temp = temp->suivant;
    }
    return NULL; // Retourne NULL si le patient n'est pas trouvé
}

// Fonction pour mettre à jour un patient par son ID
void mettreAJourPatient(Patient* tete, int id, char* nouveauNom, int nouvelAge, char* nouveauSexe, char* nouveauTelephone) {
    Patient* patient = rechercherPatient(tete, id); // Recherche le patient
    if (patient != NULL) {
        // Mise à jour des informations
        strcpy(patient->nom, nouveauNom);
        patient->age = nouvelAge;
        strcpy(patient->sexe, nouveauSexe);
        strcpy(patient->telephone, nouveauTelephone);
        printf("Patient ID %d mis à jour avec succès.\n", id);
        // Réécriture du fichier avec les nouvelles informations
        reecrireFichier(tete);
    } else {
        printf("Patient avec ID %d non trouvé.\n", id);
    }
}

// Fonction pour supprimer un patient par son ID
void supprimerPatient(Patient** tete, int id) {
    Patient* temp = *tete;
    Patient* precedent = NULL;

    // Recherche du patient à supprimer
    while (temp != NULL && temp->id != id) {
        precedent = temp;
        temp = temp->suivant;
    }

    // Si le patient n'est pas trouvé
    if (temp == NULL) {
        printf("Patient avec ID %d non trouvé.\n", id);
        return;
    }

    // Si le patient à supprimer est à la tête de la liste
    if (precedent == NULL) {
        *tete = temp->suivant; // La tête devient le patient suivant
    } else {
        precedent->suivant = temp->suivant; // Le précédent saute le patient supprimé
    }

    free(temp); // Libère la mémoire allouée pour le patient supprimé
    printf("Patient avec ID %d supprimé avec succès.\n", id);

    // Réécriture du fichier après suppression
    reecrireFichier(*tete);
}

int main() {
    Patient* tete = NULL;
    int choix, id, age;
    char nom[100], sexe[10], telephone[8];

    // Charger les patients depuis le fichier au démarrage
    chargerPatientsDepuisFichier(&tete);

    // Menu interactif
    do {
        printf("\n--- Menu ---\n");
        printf("1. Ajouter un patient\n");
        printf("2. Afficher tous les patients\n");
        printf("3. Rechercher un patient\n");
        printf("4. Modifier un patient\n");
        printf("5. Supprimer un patient\n");
        printf("6. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("ID: ");
                scanf("%d", &id); // Correction ici
                printf("Nom: ");
                scanf("%s", nom); // Utilisez fgets pour plus de sécurité
                printf("Age: ");
                scanf("%d", &age); // Correction ici
                printf("Sexe: ");
                scanf("%s", sexe); // Utilisez fgets pour plus de sécurité
                printf("Téléphone: ");
                scanf("%s", telephone); // Utilisez fgets pour plus de sécurité
                ajouterPatient(&tete, id, nom, age, sexe, telephone);
                sauvegarderPatientDansFichier(tete); // Correction ici
                printf("Patient ajouté avec succès.\n");
                break;

            case 2:
                afficherPatients(tete);
                break;

            case 3:
                printf("ID du patient à rechercher: ");
                scanf("%d", &id);
                Patient* patient = rechercherPatient(tete, id);
                if (patient != NULL) {
                    printf("Patient trouvé:\n");
                    printf("ID: %d\n", patient->id);
                    printf("Nom: %s\n", patient->nom);
                    printf("Age: %d\n", patient->age);
                    printf("Sexe: %s\n", patient->sexe);
                    printf("Téléphone: %s\n", patient->telephone);
                } else {
                    printf("Patient non trouvé.\n");
                }
                break;

            case 4:
                printf("ID du patient à modifier: ");
                scanf("%d", &id);
                printf("Nouveau Nom: ");
                scanf("%s", nom); // Utilisez fgets pour plus de sécurité
                printf("Nouvel Age: ");
                scanf("%d", &age); // Correction ici
                printf("Nouveau Sexe: ");
                scanf("%s", sexe); // Utilisez fgets pour plus de sécurité
                printf("Nouveau Téléphone: ");
                scanf("%s", telephone); // Utilisez fgets pour plus de sécurité
                mettreAJourPatient(tete, id, nom, age, sexe, telephone);
                break;

            case 5:
                printf("ID du patient à supprimer: ");
                scanf("%d", &id);
                supprimerPatient(&tete, id);
                break;

            case 6:
                printf("Sortie du programme.\n");
                break;

            default:
                printf("Option invalide.\n");
                break;
        }
    } while (choix != 6);

    return 0;
}
