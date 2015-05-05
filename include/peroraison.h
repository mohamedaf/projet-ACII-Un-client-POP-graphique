#include <stdio.h>
#include <stdlib.h>		/* pour EXIT_SUCCESS, EXIT_FAILURE */
#include <ctype.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <regex.h>
#include <sys/socket.h>		/* pour socket */
#include <netinet/in.h>		/* pour sockaddr_in et INET_ADDRSTRLEN */
#include <netdb.h>		/* pour gethostbyname */
#include <unistd.h>		/* pour read /write */
#include <arpa/inet.h>		/* pour inet_aton */
#include <time.h>		/* clair */
#include <string.h>		// pour strcmp

#define MARGIN 10
#define BORDER 2
#define LINELENGTH 1024
#define MESSAGELENGTH 100000

#define _RE_USER "^USER +([a-zA-Z0-9\\._]+@[a-zA-Z0-9\\.]*[^\\.][^\\.]) *$"
#define _RE_PASS "^PASS +(.*) *$"
#define _RE_LIST "^LIST *$"
#define _RE_TOP  "^TOP +([0-9]+) *$"
#define _RE_RETR "^RETR +([0-9]+) *$"
#define _RE_QUIT "^QUIT *$"
#define _RE_CONTENT_TYPE "^Content-Type: (.+)/([a-zA-Z]+)"

regex_t re_user, re_pass, re_list, re_top, re_retr, re_quit, re_content_type;

extern char argv0[128];
extern int InitConnexion(char *, int);
extern void peroraison (char *f, char *m, int n);
extern void verif_regexp();
/************** Partie 1 -t ****************/
extern void textuel_pop(int sock);
/************** Partie 2 -c ****************/
extern void clicable_pop(int sock);
extern void fExposeEvent(XExposeEvent *e);
extern void fButtonPress(XButtonEvent *e, int sock);
extern void fKeyPress (XKeyEvent *e);
extern void clean(Window w);
extern void cleanfenErr();
extern char* submit(int sock);
extern void ListAndTop(int sock);
/************** Partie 3 -g ****************/
