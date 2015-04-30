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

#define LINELENGTH 1024
#define MESSAGELENGTH 100000

#define _RE_USER "^USER +([a-zA-Z0-9\\._]+@[a-zA-Z0-9\\.]*[^\\.][^\\.]) *$"
#define _RE_PASS "^PASS +(.*) *$"
#define _RE_LIST "^LIST *$"
#define _RE_TOP  "^TOP +([0-9]+) +([0-9]+) *$"
#define _RE_RETR "^RETR +([0-9]+) *$"
#define _RE_QUIT "^QUIT *$"

regex_t re_user, re_pass, re_list, re_top, re_retr, re_quit;

extern char argv0[128];
extern int InitConnexion(char *, int);
extern void peroraison (char *f, char *m, int n);
extern void verif_regexp();
extern void textuel_pop(int sock);

