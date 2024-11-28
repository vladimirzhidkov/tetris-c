#include <stdint.h>
#include "scaling.h"

void scale_up( u_int8_t *src, u_int8_t *dst, int src_height, int src_width )
{
	int dst_width = src_width * SCALING_WIDTH;
	u_int8_t (*s)[src_width] = (u_int8_t (*)[src_width])src;
	u_int8_t (*d)[dst_width] = (u_int8_t (*)[dst_width])dst;

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



