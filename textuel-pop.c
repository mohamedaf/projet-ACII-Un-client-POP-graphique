#include "peroraison.h"


void textuel_pop(int sock)
{
  char *s, *message = (char*) malloc(LINELENGTH);

  /* boucle effectuant la tache principale de ce mode */
  while(1){
    regmatch_t matches[3];
    char requete[LINELENGTH], answer[MESSAGELENGTH];

    /* lecture de la requete du client */
    fgets (requete, LINELENGTH, stdin);

    //pour retirer le retour a la ligne a la fin
    requete[strlen(requete)-1] = '\0';
    message[0] = '\0';

    /* verification et envoi de la requete */
    if(!regexec(&re_user, requete, 3, matches, 0)){
      strcat(message, "USER ");
      s = strndup(requete+matches[1].rm_so, matches[1].rm_eo-matches[1].rm_so);
      strcat(message, s);
      strcat(message, "\n");

      write(sock, message, strlen(message));
    }
    else if(!regexec(&re_pass, requete, 3, matches, 0)){
      strcat(message, "PASS ");
      s = strndup(requete+matches[1].rm_so, matches[1].rm_eo-matches[1].rm_so);
      strcat(message, s);
      strcat(message, "\n");

      write(sock, message, strlen(message));
    }
    else if(!regexec(&re_list, requete, 3, matches, 0)){
      write(sock, "LIST\n", 5);
    }
    else if(!regexec(&re_top, requete, 3, matches, 0)){
      strcat(message, "TOP ");
      s = strndup(requete+matches[1].rm_so, matches[1].rm_eo-matches[1].rm_so);
      strcat(message, s);
      strcat(message, " ");
      s = strndup(requete+matches[2].rm_so, matches[2].rm_eo-matches[2].rm_so);
      strcat(message, s);
      strcat(message, "\n");

      write(sock, message, strlen(message));
    }
    else if(!regexec(&re_retr, requete, 3, matches, 0)){
      strcat(message, "RETR ");
      s = strndup(requete+matches[1].rm_so, matches[1].rm_eo-matches[1].rm_so);
      strcat(message, s);
      strcat(message, "\n");

      write(sock, message, strlen(message));
    }
    else if(!regexec(&re_quit, requete, 3, matches, 0)){
      write(sock, "QUIT\n", 5);

      /* lecture et affichage de la reponse du serveur */
      read(sock, answer, LINELENGTH);
      printf("%s\n", answer);
      break;
    }
    else{
      printf("vous avez tapee:\n%s\n",requete);
      printf("Commande inconnue !\n");
      continue;
    }

    /* lecture et affichage de la reponse du serveur */
    read(sock, answer, MESSAGELENGTH);
    printf("%s\n", answer);

    /* vider la chaine de reponse */
    memset (answer, '\0', MESSAGELENGTH);
  }
}
