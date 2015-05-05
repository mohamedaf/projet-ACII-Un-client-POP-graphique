#include <peroraison.h>

Display *dpy;
Window fen, sfen1, sfen2, sfen3, sfen4;
int last1, last2, last3;
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

  /* Modifier le champ override_redirect pour les sous fenetres
     de la fenetre principale*/
  xsaw.override_redirect = True;
  XChangeWindowAttributes(dpy, sfen1, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen2, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen3, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen4, CWOverrideRedirect, &xsaw);

  /* Afin de reagir aux evenements souris et clavier */
  XSelectInput(dpy, sfen1, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen2, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen3, ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen4, ButtonPressMask | ExposureMask);

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

    if(submit(sock)){
      /* USER ou PASS incorrect ou l'une des deux chaines est vide */
      /* On vide donc les deux champs de saisie */
      clean(sfen1);
      buffer1[0] = '\0';

      clean(sfen2);
      buffer2[0] = '\0';

      XDrawString(dpy, fen, gc,
	      100,
	      100,
	      "Erreur : Le nom d'utilsateur ou le mot de passe est incorrect, veuillez les resaisir.",
	      strlen("Erreur : Le nom d'utilsateur ou le mot de passe est incorrect, veuillez les resaisir."));
    }

    /* Effectuer le reste des traitements TODO */
  }
  else if(e->window == sfen4){
    write(sock, "QUIT\n", 5);
    XCloseDisplay(dpy);
    exit(0);
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

int submit(int sock){

  /* Si l'une des deux chaines est vide */
  if(!strlen(buffer1) || !strlen(buffer2))
    return 1;

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
  if(!strncmp(answer, "-ERR", 4))
    return 1;

  return 0;
}
