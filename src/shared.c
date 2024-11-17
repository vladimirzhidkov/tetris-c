#include <sys/time.h>
#include "shared.h"

void transpose( char* matrix, int size )
{
	char (*m)[size] = (char (*)[size])matrix;
	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			char temp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = temp;
		}
	}
}

void reverseRows( char* matrix, int size )
{
	char (*m)[size] = (char (*)[size])matrix;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0, k = size - 1; j < k; j++, k--)
		{
			char temp = m[i][j];
			m[i][j] = m[i][k];
			m[i][k] = temp;
		}
	}
}

void reverseColumns( char* matrix, int size )
{
	char (*m)[size] = (char (*)[size])matrix;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0, k = size - 1; j < k; j++, k--)
		{
			char temp = m[j][i];
			m[j][i] = m[k][i];
			m[k][i] = temp;
		}
	}
}


long long currentTimeMs( void )
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (long long)time.tv_sec * 1000 + time.tv_usec / 1000;
}


void scaleUp( char* src, char* dst, int srcHeight, int srcWidth, int heightScale, int widthScale )
{
	int dstWidth = srcWidth * widthScale;
	char (*src2d)[srcWidth] = (char (*)[srcWidth])src;
	char (*dst2d)[dstWidth] = (char (*)[dstWidth])dst;

	for (int srcRow = 0; srcRow < srcHeight; srcRow++)
	{
		for (int srcCol = 0; srcCol < srcWidth; srcCol++) 
		{
			for (int cellRow = 0; cellRow < heightScale; cellRow++)
			{
				for (int cellCol = 0; cellCol < widthScale; cellCol++)
				{
					int dstRow = srcRow * heightScale + cellRow;
					int dstCol = srcCol * widthScale + cellCol;

					dst2d[dstRow][dstCol] = src2d[srcRow][srcCol];
				}
			}
		}
	}
}