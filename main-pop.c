#include "peroraison.h"

/* Fonction compilant toutes les regexp definies dans peroraison.h */
void verif_regexp()
{
  if (regcomp(&re_user, _RE_USER, REG_EXTENDED)) {
      peroraison("_RE_USER", "mauvaise regexp", 1);
  }
  if (regcomp(&re_pass, _RE_PASS, REG_EXTENDED)) {
      peroraison("_RE_PASS", "mauvaise regexp", 1);
  }
  if (regcomp(&re_list, _RE_LIST, REG_EXTENDED)) {
      peroraison("_RE_LIST", "mauvaise regexp", 1);
  }
  if (regcomp(&re_top, _RE_TOP, REG_EXTENDED)) {
      peroraison("_RE_TOP", "mauvaise regexp", 1);
  }
  if (regcomp(&re_retr, _RE_RETR, REG_EXTENDED)) {
      peroraison("_RE_RETR", "mauvaise regexp", 1);
  }
  if (regcomp(&re_quit, _RE_QUIT, REG_EXTENDED)) {
      peroraison("_RE_QUIT", "mauvaise regexp", 1);
  }
}

int main(int argc, char* argv[])
{

  /* Verifier que le nombre des parametres est exact */
  if(argc > 4 || argc < 4)
    peroraison("Nombre d'arguments incorrect",
	       "./application option[-t,-c,-g] adresseServeur port\n", 1);

  int a=0;

  /* Verifier quel mode le client a demande */
  if(!strcmp(argv[1], "-t"))
    a=1;
  else if (!strcmp(argv[1], "-c"))
    a=2;
  else if (!strcmp(argv[1], "-g"))
    a=3;

  if(!a)
    peroraison("Option inconnu", "option[-t,-c,-g]\n", 1);

  /* verification des expressions regulieres definies dans peroraison.h */
  verif_regexp();

  /* le premier parametre est l'adresse du serveur POP
     le second le numero du port
     cette fonction se connecte donc au serveur indiquee
     puis reenvoie la socket de connexion */
  int sock = InitConnexion(argv[2], atoi(argv[3]));

  if(a == 1){
    /* mode textuel */
    textuel_pop(sock);
  }
  else if(a == 2){
    /* mode clicable */
    /* TODO */
  }
  else if(a == 3){
    /* mode graphique */
    /* TODO */
  }

  close(sock);
  exit(0);
}
