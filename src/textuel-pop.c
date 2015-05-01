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

      //recuperer le numero du message
      int contentType = 0;
      char *ligne;

      /* lecture et affichage de la reponse du serveur */
      read(sock, answer, MESSAGELENGTH);
      printf("%s\n", answer);

      /* Traitement pour la commande RETR */
      ligne = strdup(answer);

      ligne = strtok(ligne, "\n");

      int multipart=0;

      do{
	if(!regexec(&re_content_type, ligne, 3, matches, 0)){
	  /* Entete Content-Type de type multipart */
	  contentType = 1;

	  if(!strncmp(ligne+matches[1].rm_so, "multipart", matches[1].rm_eo-matches[1].rm_so)){
	    printf("ligne : %s\n", ligne);

	    if(mkdir(s, 0777) == -1){
	      peroraison("fichier textuel-pop.c : fonction textuel_pop : cas RETR multipart",
			 "erreur creation repertoire multipart\n",1);
	    }
	    multipart = 1;
	    continue;
	  }
	  else{
	    printf("ligne : %s\n", ligne);

	    char *s2, *s3;
	    /* recuperer l'extension */
	    s2 = strndup(ligne+matches[2].rm_so, matches[2].rm_eo-matches[2].rm_so);
	    s3 = strdup(s);
	    strcat(s3, s2);
	    strcat(s3, ".txt");

	    printf("extension : %s\n", s2);

	    FILE *f;

	    if(multipart){
	      char *s4;

	      s4 = strdup(s);
	      strcat(s4, "/");
	      strcat(s4, s);
	      f = fopen(s4, "w+");
	    }
	    else
	      f = fopen(s, "w+");

	    fputs(answer, f);
	    fclose(f);
	  }

	}
      }while((ligne = strtok(NULL, "\n")));

      if(!contentType){
	/* pas de Content-Type dans la reponse recu du serveur
	   on stocke donc le message dans un fichier "num-message.txt" */
	strcat(s, ".txt");
	FILE *f = fopen(s, "w+");
	fputs(answer, f);
	fclose(f);
      }

      /* vider la chaine de reponse */
      memset (answer, '\0', MESSAGELENGTH);
      continue;
    }
    else if(!regexec(&re_quit, requete, 3, matches, 0)){
      write(sock, "QUIT\n", 5);

      /* lecture et affichage de la reponse du serveur */
      read(sock, answer, LINELENGTH);
      printf("%s\n", answer);
      break;
    }
    else{
      printf("-ERR\n");
      continue;
    }

    /* lecture et affichage de la reponse du serveur */
    read(sock, answer, MESSAGELENGTH);
    printf("%s\n", answer);

    /* vider la chaine de reponse */
    memset (answer, '\0', MESSAGELENGTH);
  }
}
