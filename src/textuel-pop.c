#include <peroraison.h>

void traitement_RETR(char* answer, char* s)
{
  FILE *f;
  regmatch_t matches[3];
  const char * ligne = answer;
  int contentType=0, multipart=0, debmess=0, mess=0, c=0;

  /* boucle permettant de lire la reponse du serveur ligne par ligne */
  while(ligne){
    char * nextLine = strchr(ligne, '\n');
    // temporarily terminate the current line
    if (nextLine) *nextLine = '\0';

    /* si un message doit etre lu */
    if(mess){
      /* ligne vide signalant le debut du message */
      if(!strcmp(ligne,"") && !debmess){
	debmess=1;
      }
      /* ligne vide signalant la fin du message */
      else if(!strcmp(ligne,"") && debmess){
	fclose(f);
	mess=0;
	debmess=0;
	contentType=0;
      }
      else if(debmess){
	/* ecire le message dans le fichier concerne */
	fputs(ligne, f);
      }
    }
    else if(!regexec(&re_content_type, ligne, 3, matches, 0)){
      /* Entete Content-Type de type multipart */
      contentType = 1;

      if(!strncmp(ligne+matches[1].rm_so, "multipart",
		  matches[1].rm_eo-matches[1].rm_so)){

	/* creer le repertoire (cas multipart) */
	if(mkdir(s, 0777) == -1){
	  peroraison("fichier textuel-pop.c : fonction textuel_pop : cas RETR multipart",
		     "erreur creation repertoire multipart\n",1);
	}
	/* signaler qu'on est dans un cas multipart */
	multipart = 1;
      }
      else{
	char *s2, *s3;
	/* recuperer l'extension */
	s2 = strndup(ligne+matches[2].rm_so, matches[2].rm_eo-matches[2].rm_so);
	s3 = strdup(s);
	strcat(s3, s2);
	strcat(s3, ".txt");

	/* si multipart creer fichier dans le repertoire */
	if(multipart){
	  char *s4;

	  s4 = strdup(s);
	  strcat(s4, "/");
	  strcat(s4, s3);
	  f = fopen(s4, "w+");
	}
	else
	  f = fopen(s3, "w+");

	/* signaler la lecture d'un message dans la suite */
	mess=1;
      }
    }
    else if(!contentType){
      /* pas de content-type, ecrire le message apres detection des lignes vides
         signalant les début et fin de message, puis quitter lors de la rencontre
         d'une ligne contenant seulement le caractere "." */
      if(!strcmp(ligne,"") && !debmess){
	c=1;
	debmess=1;
      }
      else if(!strcmp(ligne,"") && debmess){
	fclose(f);
	debmess=0;
      }
      else if(strcmp(ligne,".") && debmess){
	if(c){
	  /* pas de Content-Type dans la reponse recu du serveur
	     on stocke donc le message dans un fichier "num-message.txt" */
	  strcat(s, ".txt");
	  f = fopen(s, "w+");
	  c=0;
	}
	fputs(ligne, f);
      }
    }

    /* fin de la reponse du seveur, ont quitte */
    if(!strcmp(ligne, "."))
      break;

    // then restore newline-char, just to be tidy
    if (nextLine) *nextLine = '\n';
    ligne = nextLine ? (nextLine+1) : NULL;
  }
}

void textuel_pop(int sock)
{
  char *s, *message = (char*) malloc(LINELENGTH);

  /* boucle effectuant la tache principale de ce mode */
  while(1){
    regmatch_t matches[3];
    char requete[LINELENGTH], answer[MESSAGELENGTH];

    /* lecture de la requete du client */
    fgets (requete, LINELENGTH, stdin);

    /* pour retirer le retour a la ligne en fin de chaine */
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
      strcat(message, " 0\n");

      write(sock, message, strlen(message));
    }
    else if(!regexec(&re_retr, requete, 3, matches, 0)){
      strcat(message, "RETR ");
      s = strndup(requete+matches[1].rm_so, matches[1].rm_eo-matches[1].rm_so);
      strcat(message, s);
      strcat(message, "\n");

      write(sock, message, strlen(message));

      /* lecture et affichage de la reponse du serveur */
      read(sock, answer, MESSAGELENGTH);
      printf("%s", answer);

      traitement_RETR(answer, s);

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
    printf("%s", answer);

    /* vider la chaine de reponse */
    memset (answer, '\0', MESSAGELENGTH);
  }
}
