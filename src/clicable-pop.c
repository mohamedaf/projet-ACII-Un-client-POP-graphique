#include <peroraison.h>

Display *dpy;
Window fen, sfen1, sfen2, sfen3, sfen4, sfen5, sfen6, mails[10];
int last1=0, last2=0, last3=0, connecte=0, init=0;
GC gc;
char buffer1[15], buffer2[15], *from[10], *date[10];

void clicable_pop(int sock)
{
  int i;
  XEvent e;
  XColor bgcolor;
  XSetWindowAttributes xsaw;
  XGCValues values;

  if ((dpy = XOpenDisplay(NULL)) == NULL)
    peroraison("fichier graphique-pop.c : fonction graphique_pop : XOpenDisplay",
	       XDisplayName(NULL), 1);

  /* On initialise les cases a 0 ce qui veux dire fenetre vide */
  for(i=0; i<10; i++){
    mails[i] = 0;
    from[i] = NULL;
    date[i] = NULL;
  }

  /* Creation de la fenetre principale */
  fen = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 250, 500,
				   580 + (MARGIN<<1),
				   555 + (MARGIN<<1),
				   0,
				   BlackPixel(dpy,DefaultScreen(dpy)),
				   BlackPixel(dpy,DefaultScreen(dpy)));

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "red", &bgcolor, &bgcolor);

  /* creation des 3 sous fenetres de la fenetre principale */
  sfen1 = XCreateSimpleWindow(dpy, fen, 0, 0,
			      149 + BORDER,
			      50,
			      0,
			      bgcolor.pixel,
			      bgcolor.pixel);

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "orange", &bgcolor, &bgcolor);

  sfen2 = XCreateSimpleWindow(dpy, fen, 150, 0,
			      149 + BORDER,
			      50,
			      0,
			      bgcolor.pixel,
			      bgcolor.pixel);

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "grey", &bgcolor, &bgcolor);

  sfen3 = XCreateSimpleWindow(dpy, fen, 300, 0,
			      149 + BORDER,
			      50,
			      0,
			      bgcolor.pixel,
			      bgcolor.pixel);

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "brown", &bgcolor, &bgcolor);

  sfen4 = XCreateSimpleWindow(dpy, fen, 450, 0,
			      149 + BORDER,
			      50,
			      0,
			      bgcolor.pixel,
			      bgcolor.pixel);

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "black", &bgcolor, &bgcolor);

  sfen5 = XCreateSimpleWindow(dpy, fen, 0, 51,
			      599 + BORDER,
			      20,
			      0,
			      bgcolor.pixel,
			      bgcolor.pixel);

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "blue", &bgcolor, &bgcolor);

  sfen6 = XCreateSimpleWindow(dpy, fen, 20, 72,
			      599 + BORDER,
			      500,
			      0,
			      bgcolor.pixel,
			      bgcolor.pixel);

  /* Modifier le champ override_redirect pour les sous fenetres
     de la fenetre principale*/
  xsaw.override_redirect = True;
  XChangeWindowAttributes(dpy, sfen1, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen2, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen3, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen4, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen5, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen6, CWOverrideRedirect, &xsaw);

  /* Afin de reagir aux evenements souris et clavier */
  XSelectInput(dpy, sfen1, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen2, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen3, ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen4, ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen5, ExposureMask);
  XSelectInput(dpy, sfen6, ExposureMask);

  /* Declaration du GC */

  values.foreground = WhitePixel(dpy, DefaultScreen(dpy));
  values.background = BlackPixel(dpy, DefaultScreen(dpy));

  gc = XCreateGC(dpy,
		 DefaultRootWindow(dpy),
		 GCForeground|GCBackground,
		 &values);

  /* Affichages des fenetres */
  XMapWindow(dpy, fen);
  XMapSubwindows(dpy, fen);

  /* Attente des evenements */
  while (1){
    XNextEvent(dpy, &e);
    switch (e.type)
      {
      case KeyPress: fKeyPress(&e.xkey); break;
      case Expose: {fExposeEvent(&e.xexpose); break;}
      case ButtonPress: {fButtonPress(&e.xbutton, sock); break;}
      default: break;
      }
  }

  XCloseDisplay(dpy);
}

void fExposeEvent(XExposeEvent *e)
{
  if(!init){
    init=1;

    XDrawString(dpy, sfen3, gc,
		55,
		30,
		"Submit",
		6);

    XDrawString(dpy, sfen4, gc,
		60,
		30,
		"Quit",
		4);
  }
}

void fButtonPress(XButtonEvent *e, int sock)
{
  char *err;

  if(e->window == sfen1){
    last1 = 1;
    last3 = 0;
    last2 = 0;
  }
  else if(e->window == sfen2){
    last1 = 0;
    last2 = 1;
    last3 = 0;
  }
  else if(e->window == sfen3){
    last1 = 0;
    last2 = 0;
    last3 = 1;

    if((err = submit(sock))){
      /* USER ou PASS incorrect ou l'une des deux chaines est vide */
      /* On vide donc les deux champs de saisie */

      clean(sfen1);
      buffer1[0] = '\0';

      clean(sfen2);
      buffer2[0] = '\0';

      cleanfenErr();

      XDrawString(dpy, sfen5, gc,
		  25,
		  10,
		  err,
		  strlen(err));
    }
    else{
      /* Effectuer le reste des traitements */
      ListAndTop(sock);
    }
  }
  else if(e->window == sfen4){
    write(sock, "QUIT\n", 5);
    XCloseDisplay(dpy);
    exit(0);
  }
  else{
    int i=0;

    while(mails[i]){
      if(e->window == mails[i])
	RETR(sock, i);

      i++;
    }
  }

  /* Debugage */
  /*if(last1)
    printf("clic sur fenetre 1\n");
  else if(last2)
    printf("clic sur fenetre 2\n");
  else if(last3)
  printf("clic sur fenetre 3\n");*/
}


void fKeyPress (XKeyEvent *e)
{
  char *c = (char*) malloc(2*sizeof(char));

  /* recuperation du caractere tapee */
  XLookupString(e, c, 2, NULL, NULL);

  /* Verifier si l'une des deux chaines a atteint la longueur max autorise */
  if(strlen(buffer1) == 14){
    clean(sfen1);
    buffer1[0] = '\0';
  }
  else if(strlen(buffer2) == 14){
    clean(sfen2);
    buffer2[0] = '\0';
  }

  /* Debugage */
  /*if(last1)
    printf("fenetre 1, keycode : %d, chaine : %s\n", e->keycode, c);
  else if(last2)
    printf("fenetre 2, keycode : %d, chaine : %s\n", e->keycode, c);
  else if(last3)
  printf("fenetre 3, keycode : %d, chaine : %s\n", e->keycode, c);*/

  /* afficher la nouvelle chaine */
  if(last1){
    if(e->keycode == 22){
      clean(sfen1);
      buffer1[strlen(buffer1)-1] = '\0';
    }
    else
      strcat(buffer1, c);

    XDrawString(dpy, sfen1, gc,
	      25,
	      30,
	      buffer1,
	      strlen(buffer1));
  }
  else if(last2){
    int i;
    char *f = (char*) malloc(15);
    f[0] = '\0';

    if(e->keycode == 22){
      clean(sfen2);
      buffer2[strlen(buffer2)-1] = '\0';
    }
    else
      strcat(buffer2, c);

    for(i=0; i<strlen(buffer2); i++)
      strcat(f, "*");

    XDrawString(dpy, sfen2, gc,
	      25,
	      30,
	      f,
	      strlen(f));
  }
}

void clean(Window w){
  XUnmapWindow(dpy, w);

  XClearArea(dpy, w, 25,
	     30,
	     140,
	     10,
	     False);

  XMapWindow(dpy, w);
}

void cleanfenErr(){
  XUnmapWindow(dpy, sfen5);

  XClearArea(dpy, sfen5, 25,
	     5,
	     549,
	     15,
	     False);

  XMapWindow(dpy, sfen5);
}

char* submit(int sock){

  /* Si l'une des deux chaines est vide */
  if(!strlen(buffer1) || !strlen(buffer2))
    return strdup("Erreur : Veuillez entrer un nom d'utilisateur et un mot de passe !");

  if(connecte)
    return strdup("Erreur : vous etes deja connecte sur un compte utilisateur !");

  char *requete, answer[MESSAGELENGTH];

  requete = (char*) malloc((strlen("USER ")+strlen(buffer1)+1)*sizeof(char));

  strcat(requete, "USER ");
  strcat(requete, buffer1);
  strcat(requete, "\n");

  write(sock, requete, strlen(requete));

  /* lecture reponse serveur */
  read(sock, answer, MESSAGELENGTH);

  requete = (char*) malloc((strlen("PASS ")+strlen(buffer2)+1)*sizeof(char));

  strcat(requete, "PASS ");
  strcat(requete, buffer2);
  strcat(requete, "\n");

  write(sock, requete, strlen(requete));

  /* lecture reponse serveur */
  read(sock, answer, MESSAGELENGTH);

  printf("%s\n", answer);

  /* si probleme USER ou PASS incorrect return 1 */
  if(!strncmp(answer, "+ERR", 4))
    return strdup("Erreur : Le nom d'utilsateur ou le mot de passe est incorrect, veuillez les resaisir.");

  connecte=1;

  return NULL;
}

void ListAndTop(int sock)
{
  int nbMessages = 0;
  char answer[MESSAGELENGTH];

  /* Envoyer la requete LIST au serveur */
  write(sock, "LIST\n", 5);

  /* Recuperer la reponse du serveur */
  read(sock, answer, MESSAGELENGTH);

  printf("%s\n", answer);

  strtok(answer, "\n");

  while(strcmp(strtok(NULL, "\n"), "."))
    nbMessages++;

  if(nbMessages > 10)
    nbMessages = 10;

  Top(sock, answer, nbMessages);

  TopAffichage(nbMessages);
}

void Top(int sock, char *answer, int nbMessages)
{
  int i;
  char message[LINELENGTH];
  const char * ligne;

  /* vider la chaine de reponse */
  memset (answer, '\0', MESSAGELENGTH);

  for(i=0; i<nbMessages; i++){
    char num[2];

    message[0] = '\0';
    printf("Message %d\n", i);

    /* construire la chaine "TOP i\n" */
    strcat(message, "TOP ");
    sprintf(num, "%d", i);
    strcat(message, num);
    strcat(message, " 0\n");

    /* Envoyer la requete TOP i au serveur */
    write(sock, message, strlen(message));

    printf("message : %s\n", message);
    fflush(stdout);

    /* Recuperer la reponse du serveur */
    read(sock, answer, MESSAGELENGTH);

    ligne = answer;

    while(ligne){
      char * nextLine = strchr(ligne, '\n');
      // temporarily terminate the current line
      if (nextLine) *nextLine = '\0';

      if(!strncmp(ligne, "From: ", strlen("From: "))){
	from[i] = strdup(ligne+6);
	printf("From : %s\n", from[i]);
      }
      else if(!strncmp(ligne, "Date: ", strlen("Date: "))){
	date[i] = strdup(ligne+6);
	printf("Date : %s\n", date[i]);
      }

      // then restore newline-char, just to be tidy
      if (nextLine) *nextLine = '\n';
      ligne = nextLine ? (nextLine+1) : NULL;
    }

    /* vider la chaine de reponse */
    memset (answer, '\0', MESSAGELENGTH);
  }
}


void TopAffichage(int nbMessages){
  int i, y = 5;
  XColor bgcolor;
  XSetWindowAttributes xsaw;

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "green", &bgcolor, &bgcolor);

  char f[LINELENGTH], d[LINELENGTH], num[3];

  for(i=0; i<nbMessages; i++){
    f[0] = '\0';
    d[0] = '\0';

    strcat(f, "From: ");
    strcat(d, "Date: ");

    if(from[i])
      strcat(f, from[i]);

    if(date[i])
      strcat(d, date[i]);

    mails[i] = XCreateSimpleWindow(dpy, fen, 0, y+68,
				   19,
				   49,
				   0,
				   bgcolor.pixel,
				   bgcolor.pixel);

    xsaw.override_redirect = True;
    XChangeWindowAttributes(dpy, mails[i], CWOverrideRedirect, &xsaw);

    XSelectInput(dpy, mails[i], ButtonPressMask | ExposureMask);
    XMapWindow(dpy, mails[i]);

    sprintf(num, "%d", i);

    XDrawString(dpy, mails[i], gc,
		8,
		25,
		num,
		strlen(num));

    XDrawString(dpy, sfen6, gc,
		5,
		y+10,
		f,
		strlen(f));

    y = y + 20;

    XDrawString(dpy, sfen6, gc,
		5,
		y+10,
		d,
		strlen(d));

    y = y + 30;
  }

}

void RETR(int sock, int numMessage)
{
  char answer[MESSAGELENGTH], message[LINELENGTH], num[20];

  message[0] = '\0';

  strcat(message, "RETR ");
  sprintf(num, "%d", numMessage);
  strcat(message, num);
  strcat(message, "\n");

  write(sock, message, strlen(message));

  /* lecture et affichage de la reponse du serveur */
  read(sock, answer, MESSAGELENGTH);
  printf("%s", answer);

  traitement_RETR(answer, num);
}
