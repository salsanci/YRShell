#include <stdio.h>

int main( int argc, char *argv[] ) {
	int c;
	printf( "#ifndef CurrrentGit_h\n#define CurrrentGit_h\n#define GIT_COMMIT_HASH \"");

	while( EOF != (c = getchar())) {
		if( c > 0x20 && c < 0x7F) {
			putchar( c);
		} else if( c == '\n' || c == '\r') {
			printf( "\"\n#define GIT_COMMIT_SHORT_HASH \"[");
			while( EOF != (c = getchar())) {
				if( c > 0x20 && c < 0x7F) {
					putchar( c);
				}
			}
		}
	}
	printf( "]\"\n#endif\n");
	return( 0);
}