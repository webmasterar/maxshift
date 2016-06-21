/*
 * MaxShift
 * Copyright (C) 2015-2016 Solon Pissis and Ahmad Retha
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "maxshift.h"

/*
given integers a, b, c this function returns one of the integers a, b, c
with the property that it has the least number of 1's (bits set on). If there is
a draw then it returns the maximum of the two when viewed as decimal integers
*/
inline WORD bitminmax ( WORD a, WORD b, WORD c )
{
	unsigned int x , y , z; 
	WORD minimum;
	x = popcount ( a );
	y = popcount ( b );
	z = popcount ( c );

	minimum = min( x , y );
	if( z < minimum )
	{
		return c;
	}
	else if ( z == minimum && x != y )
	{
		return ( x < y ) ? max ( c , a ) : max ( c , b );
	}
	else if ( z == x && x == y )
	{
		return max3 ( a, b, c );
	}
	else if ( z > minimum && x != y )
	{
		return ( x < y ) ? a : b;
	}
	return  max ( a , b );
}

/*
 MaxShift algorithm for Hamming Distance in linear space
*/
void maxshift_hd_ls ( unsigned char * p, unsigned int m, unsigned  char * t, unsigned int n, unsigned int h, unsigned int * ii, unsigned int * jj, unsigned int * kk )
{
	WORD y, v; 
	WORD * M0, * M1;
	unsigned int i, j, k;

	if ( ( M0 = ( WORD * ) calloc ( ( n + 1 ) , sizeof( WORD ) ) ) == NULL )
	{
		fprintf( stderr, " Error: M0 could not be allocated!\n");
		return;
	}
	if ( ( M1 = ( WORD * ) calloc ( ( n + 1 ) , sizeof( WORD ) ) ) == NULL )
	{
		fprintf( stderr, " Error: M1 could not be allocated!\n");
		return;
	}

	y = ( ( WORD ) 1 << ( h - 1 ) ) - 1;

	for ( i = 0; i < m + 1; i ++ ) //loop through p
	{

		for( j = 0; j < n + 1 ; j++ ) //loop through t		
		{

			if( i == 0 ) continue;

			switch ( i % 2 )
			{
				case 0 :

					if ( j == 0 )
						M0[j] = ( ( WORD ) 2 << ( min ( i , h ) - 1 ) ) - 1;
					else
						M0[j] = shiftc ( M1[j - 1], y ) | delta ( p[i - 1], t[j - 1] );

					v = M0[j];

					break;

				case 1 :

					if ( j == 0 )
						M1[j] = ( ( WORD ) 2 << ( min ( i , h ) - 1 ) ) - 1;
					else
						M1[j] = shiftc ( M0[j - 1], y ) | delta ( p[i - 1], t[j - 1] );

					v = M1[j];

					break;
			}

			if ( i >= h  && j >= h )
			{
				k = popcount ( v );

				if ( k < (* kk) )
				{
					(* ii) = i - 1;
					(* jj) = j - 1;
					(* kk) = k;
				}
			}

                }

        }

	free ( M0 );
	free ( M1 );
}

/*
 MaxShift algorithm for Edit Distance in linear space
*/
void maxshift_ed_ls ( unsigned char * p, unsigned int m, unsigned  char * t, unsigned int n, unsigned int h, unsigned int * ii, unsigned int * jj, unsigned int * kk )
{
	WORD y, v; 
	WORD * M0, * M1; 		
	unsigned int i, j, k;

	if ( ( M0 = ( WORD * ) calloc ( ( n + 1 ) , sizeof( WORD ) ) ) == NULL )
	{
		fprintf( stderr, " Error: M0 could not be allocated!\n");
		return;
	}
	if ( ( M1 = ( WORD * ) calloc ( ( n + 1 ) , sizeof( WORD ) ) ) == NULL )
	{
		fprintf( stderr, " Error: M1 could not be allocated!\n");
		return;
	}
	
	y = ( ( WORD ) ( 1 ) << ( h - 1 ) ) - 1;

	for ( i = 0; i < m + 1; i ++ ) 
	{

		for( j = 0; j < n + 1 ; j++ )			
		{
			if( i == 0 ) continue;

			switch ( i % 2 )
			{
				case 0 :

					if ( j == 0 )
						M0[j] = ( ( WORD ) ( 2 ) << ( min ( i , h ) - 1 ) ) - 1;
					else if ( i <= h )
						M0[j] = bitminmax ( shift ( M1[j] ) | 1, shift ( M0[j - 1] ) | 1, shift ( M1[j - 1] ) | delta ( p[i - 1], t[j - 1] ) );
					else
						M0[j] = bitminmax ( shiftc ( M1[j], y ) | 1, shift ( M0[j - 1] ) | 1, shiftc ( M1[j - 1], y ) | delta ( p[i - 1], t[j - 1] ) );

					v = M0[j];

					break;

				case 1 :

					if ( j == 0 )
						M1[j] = ( ( WORD ) ( 2 ) << ( min ( i , h ) - 1 ) ) - 1;
					else if ( i <= h )
						M1[j] = bitminmax ( shift ( M0[j] ) | 1, shift ( M1[j - 1] ) | 1, shift ( M0[j - 1] ) | delta ( p[i - 1], t[j - 1] ) );
					else
						M1[j] = bitminmax ( shiftc ( M0[j] , y ) | 1, shift ( M1[j - 1] ) | 1, shiftc ( M0[j - 1], y ) | delta ( p[i - 1], t[j - 1] ) );

					v = M1[j];

					break;
			} 

			if ( i >= h )
			{
				k = popcount ( v );

				if ( k < (* kk) )	
				{
					(* ii) = i - 1;
					(* jj) = j - 1;
					(* kk) = k;
				}
			}

                }
        }

	free ( M0 );
	free ( M1 );
}

