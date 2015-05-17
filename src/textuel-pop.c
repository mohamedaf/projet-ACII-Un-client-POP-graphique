#include <peroraison.h>

void traitement_RETR(char* answer, char* s)
{
  FILE *f;
  regmatch_t matches[3];
  const char * ligne = answer;
  int contentType=0, multipart=0, debmess=0, mess=0, c=0;

  while(ligne){
    char * nextLine = strchr(ligne, '\n');
    // temporarily terminate the current line
    if (nextLine) *nextLine = '\0';

    if(mess){
      if(!strcmp(ligne,"") && !debmess){
	debmess=1;
      }
      else if(!strcmp(ligne,"") && debmess){
	fclose(f);
	mess=0;
	debmess=0;
	contentType=0;
      }
      else if(debmess){
	fputs(ligne, f);
      }
    }
    else if(!regexec(&re_content_type, ligne, 3, matches, 0)){
      /* Entete Content-Type de type multipart */
      contentType = 1;

      if(!strncmp(ligne+matches[1].rm_so, "multipart",
		  matches[1].rm_eo-matches[1].rm_so)){

	if(mkdir(s, 0777) == -1){
	  peroraison("fichier textuel-pop.c : fonction textuel_pop : cas RETR multipart",
		     "erreur creation repertoire multipart\n",1);
	}
	multipart = 1;
      }
      else{
	char *s2, *s3;
	/* recuperer l'extension */
	s2 = strndup(ligne+matches[2].rm_so, matches[2].rm_eo-matches[2].rm_so);
	s3 = strdup(s);
	strcat(s3, s2);
	strcat(s3, ".txt");

	if(multipart){
	  char *s4;

	  s4 = strdup(s);
	  strcat(s4, "/");
	  strcat(s4, s3);
	  f = fopen(s4, "w+");
	}
	else
	  f = fopen(s3, "w+");

	mess=1;
      }
    }
    else if(!contentType){
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
