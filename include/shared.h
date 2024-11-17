#ifndef SHARED_H
#define SHARED_H

void transpose( char* matrix, int size );
void reverseRows( char* matrix, int size );
void reverseColumns( char* matrix, int size );
void scaleUp( char* src, char* dst, int srcHeight, int srcWidth, int heightScale, int widthScale );
long long currentTimeMs( void );

#endif /* SHARED_H */
