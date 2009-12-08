#ifndef MSG_H
#define MSG_H

#ifdef DEBUG
#define debug(msg,arg...)	fprintf(stdout, "%s ", _("[Debug]"));   fprintf(stdout, _(msg), ##arg); fprintf(stdout, "\n")
#else /* DEBUG */
#define debug(msg,arg...)
#endif /* DEBUG */

#define warning(msg,arg...)	fprintf(stderr, "%s ", _("[Warning]")); fprintf(stderr, _(msg), ##arg); fprintf(stderr, "\n")
#define error(msg,arg...)	fprintf(stderr, "%s ", _("[Error]"));   fprintf(stderr, _(msg), ##arg); fprintf(stderr, "\n")
#define fatal(msg,arg...)										\
{													\
	fprintf(stderr, "%s ", _("[Fatal]")); fprintf(stderr, _(msg), ##arg); fprintf(stderr, "\n");	\
	abort();											\
}

#endif /* MSG_H */
