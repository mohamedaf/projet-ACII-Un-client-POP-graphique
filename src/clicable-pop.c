#include <peroraison.h>

Display *dpy;
Window fen, sfen1, sfen2, sfen3, sfen4, sfen5;
int last1, last2, last3, connecte=0;
GC gc;
char buffer1[15], buffer2[15];

void clicable_pop(int sock)
{
  XEvent e;
  XColor bgcolor;
  XSetWindowAttributes xsaw;
  XGCValues values;

  last1=0;
  last2=0;
  last3=0;

  if ((dpy = XOpenDisplay(NULL)) == NULL)
    peroraison("fichier graphique-pop.c : fonction graphique_pop : XOpenDisplay",
	       XDisplayName(NULL), 1);

  /* Creation de la fenetre principale */
  fen = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 250, 500,
				   580 + (MARGIN<<1),
				   400 + (MARGIN<<1),
				   0,
				   BlackPixel(dpy,DefaultScreen(dpy)),
				   BlackPixel(dpy,DefaultScreen(dpy)));

  XAllocNamedColor(dpy, DefaultColormap(dpy, DefaultScreen(dpy)),
		   "green", &bgcolor, &bgcolor);

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
		   "blue", &bgcolor, &bgcolor);

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

  /* Modifier le champ override_redirect pour les sous fenetres
     de la fenetre principale*/
  xsaw.override_redirect = True;
  XChangeWindowAttributes(dpy, sfen1, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen2, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen3, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen4, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen5, CWOverrideRedirect, &xsaw);

  /* Afin de reagir aux evenements souris et clavier */
  XSelectInput(dpy, sfen1, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen2, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen3, ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen4, ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen5, ExposureMask);

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

  /* afficher la nouvelle chaine */
  if(last1){
    if(e->keycode == 59){
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
    if(e->keycode == 59){
      clean(sfen2);
      buffer2[strlen(buffer2)-1] = '\0';
    }
    else
      strcat(buffer2, c);

    XDrawString(dpy, sfen2, gc,
	      25,
	      30,
	      buffer2,
	      strlen(buffer2));
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

  strtok(answer, "\n");

  while(strcmp(strtok(NULL, "\n"), "."))
    nbMessages++;

  printf("nbMessages = %d\n", nbMessages);
}
