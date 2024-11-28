#include "scaling.h"

void scale_up( char* src, char* dst, int src_height, int src_width )
{
	int dst_width = src_width * SCALING_WIDTH;
	char (*s)[src_width] = (char (*)[src_width])src;
	char (*d)[dst_width] = (char (*)[dst_width])dst;

	for (int src_row = 0; src_row < src_height; src_row++)
	{
		for (int src_col = 0; src_col < src_width; src_col++) 
		{
			for (int cell_row = 0; cell_row < SCALING_HEIGHT; cell_row++)
			{
				for (int cell_col = 0; cell_col < SCALING_WIDTH; cell_col++)
				{
					int dst_row = src_row * SCALING_HEIGHT + cell_row;
					int dst_col = src_col * SCALING_WIDTH + cell_col;

					d[dst_row][dst_col] = s[src_row][src_col];
				}
			}
		}
	}
}



