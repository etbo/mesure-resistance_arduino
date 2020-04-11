#include <Wire.h>
#include "Arduino.h"

/**********************************************************/
/* Broches utilisées : */
#define PIN_ANA PIN_A0

/**********************************************************/
/* Variables générales */
char message[100]; // Utilisé pour écrire des messages (i.e : du Serial)
int temps_cycle = 5; // Temps de cycle de la loop(), en secondes
unsigned long temps = 0; // Pour stocker le temps avec fonction millis();

/* Resistance de reference */
int resistance_reference = 10000; // Resistance de reference (en ohm) entre +5V et A0

void afficher_tab_serial(char intitule_ligne[30], double valeur)
{
  sprintf(message, "%20.20s  =  ", intitule_ligne);
  Serial.print(message);
  Serial.println(valeur);
}

/*********************************************************/
void afficher_heures_minutes_secondes(long temps_millis)
// Fonction pour convertir les millisecondes en min et secondes
{
  int temps_secondes = temps_millis / 1000;
  int minutes = temps_secondes / 60;
  int secondes = temps_secondes % 60;
  sprintf(message, "%20.20s  =  ", "Temps ecoule");
  Serial.print(message);
  sprintf(message, "%d min %02d s", minutes, secondes);
  Serial.println(message);
}

/************************************************************/
void setup()
// Setup de l'arduino
{
  // Setup Serial :
  Serial.begin(9600); // initialise la connexion série à 9600 bauds
  for (int i = 0 ; i < 10 ; i++)
    Serial.println(""); // On affiche x lignes vides

  Serial.println("___Restart___");
  Serial.println("");
}

double fonction_lineaire(double x_point_inconnu, int x_point_1, int x_point_2, int y_point_1,  int y_point_2)
// On donne 2 points connus et la fonction renvoi la valeur à la troisième abscisse donnée
{
  double coef_dir = ((double)y_point_2 - y_point_1) / (x_point_2 - x_point_1);
  double y_point_inconnu = coef_dir * (x_point_inconnu - x_point_1) + y_point_1;
  return y_point_inconnu;
}

/************************************************************/
void loop()
{
  while (millis() % (temps_cycle * 1000) > 0); // Tant que l'on est pas à un multiple du temps de cycle
  
  // Temps :
  temps = millis(); // On récupère le temps
  afficher_heures_minutes_secondes(temps); // On le rend plus lisible

  int mesure_analogique = analogRead(PIN_ANA);
  afficher_tab_serial("Mesure sur A0", mesure_analogique);

  double tension_mesuree = 0.0;
  tension_mesuree = fonction_lineaire(mesure_analogique, 0, 1023, 0, 5); // fonction map avec des doubles
  afficher_tab_serial("Conversion en Volt", tension_mesuree);

  int resistance_mesuree = 0; // On veut un entier
  resistance_mesuree = resistance_reference * tension_mesuree / (5-tension_mesuree);
  afficher_tab_serial("Conversion en Ohm", resistance_mesuree);

  Serial.println("");
}