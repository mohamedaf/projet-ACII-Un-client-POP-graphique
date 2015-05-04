#include <peroraison.h>

Display *dpy;
Window fen, sfen1, sfen2, sfen3;
int last1, last2, last3;
GC gc;
char buffer1[15], buffer2[15];

void clicable_pop()
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
				   430 + (MARGIN<<1),
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

  /* Modifier le champ override_redirect pour les sous fenetres
     de la fenetre principale*/
  xsaw.override_redirect = True;
  XChangeWindowAttributes(dpy, sfen1, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen2, CWOverrideRedirect, &xsaw);
  XChangeWindowAttributes(dpy, sfen3, CWOverrideRedirect, &xsaw);

  /* Afin de reagir aux evenements souris et clavier */
  XSelectInput(dpy, sfen1, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen2, KeyPressMask | ButtonPressMask | ExposureMask);
  XSelectInput(dpy, sfen3, ButtonPressMask | ExposureMask);

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
      case ButtonPress: {fButtonPress(&e.xbutton); break;}
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
}

void fButtonPress(XButtonEvent *e)
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
  }

  /* Debugage */
  if(last1)
    printf("clic sur fenetre 1\n");
  else if(last2)
    printf("clic sur fenetre 2\n");
  else if(last3)
    printf("clic sur fenetre 3\n");
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
