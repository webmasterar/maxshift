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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <string.h>
#include "maxshift.h"

int main(int argc, char** argv)
{
	const char * info = "\
The valid options for MaxShift are:\n\
-m\tRequired. The model to use. 0=Hamming distance, 1=Edit distance\n\
-t\tRequired. The text to search through\n\
-f\tOptional. File for -t if -t won't do\n\
-p\tRequired. The pattern you are searching for\n\
-l\tOptional. File for -p if -p won't do\n\
-w\tRequired. Length of factor, e.g. 9\n\
-h\tThis help message\n";

	unsigned char * t, * p;
	unsigned int model;
	unsigned int c = 0, w = 0;
	size_t s;
	FILE * ft, * fp;

	unsigned int i;
	for ( i = 1; i < argc; i++ ) {
		if ( '-' == (int)argv[i][0] ) {
			switch ( (int)argv[i][1] ) {
				case 'm':
					model = atoi ( argv[i + 1] );
					c++;
					break;

				case 't':
					t = (unsigned char *) argv[i + 1];
					c++;
					break;

				case 'f':
					if ( ( ft = fopen ( argv[i + 1], "r" ) ) == NULL ) {
						fprintf( stderr, "Input file f could not be opened!\n");
						return ( EXIT_FAILURE );
					} else {
						fseek ( ft, 0, SEEK_END );
						unsigned long length = ftell ( ft );
						fseek ( ft, 0, SEEK_SET );
						t = (unsigned char *) malloc ( length );
						if ( t ) {
							s = fread ( t, 1, length, ft );
						}
						fclose ( ft );
					}
					c++;
					break;

				case 'p':
					p = (unsigned char *) argv[i + 1];
					c++;
					break;

				case 'l':
					if ( ( fp = fopen ( argv[i + 1], "r" ) ) == NULL ) {
						fprintf( stderr, "Input file l could not be opened!\n");
						return ( EXIT_FAILURE );
					} else {
						fseek ( fp, 0, SEEK_END );
						unsigned long length = ftell ( fp );
						fseek ( fp, 0, SEEK_SET );
						p = (unsigned char *) malloc ( length );
						if ( p ) {
							s = fread ( p, 1, length, fp );
						}
						fclose ( fp );
					}
					c++;
					break;

				case 'w':
					w = atoi ( argv[i + 1] );
					c++;
					break;

				case 'h':
					printf ( "%s", info );
					return EXIT_SUCCESS;
				break;

				default:
					fprintf( stderr, "Invalid option '-%c' supplied!\n", argv[i][1] );
					printf ( "%s", info );
					return ( EXIT_FAILURE );
			}
		}
	}

	if ( c != 4 )
	{
		fprintf ( stderr, "Error: Invalid number of arguments supplied!\n%s", info );
		return EXIT_FAILURE;
	}

	if ( !(model == 0 || model == 1) )
	{
		fprintf ( stderr, "Error. Invalid model supplied!\n%s", info );
		return EXIT_FAILURE;
	}

	if ( w >= WORDSIZE )
	{
		fprintf ( stderr, "Error: Factor length size too large!\n" );
		return EXIT_FAILURE;
	}
	else if ( w <= 0 )
	{
		fprintf ( stderr, "Please povide a valid factor length!\n" );
		return EXIT_FAILURE;
	}

	unsigned int ii, jj, kk = UINT_MAX;
	unsigned int m = strlen((char *)p);
	unsigned int n = strlen((char *)t);

	if ( model == 0 )
		maxshift_hd_ls ( p, m, t, n, w, &ii, &jj, &kk );
	else
		maxshift_ed_ls ( p, m, t, n, w, &ii, &jj, &kk );

	printf("e: %u, i: %u, j: %u\n", kk, ii, jj);

	return EXIT_SUCCESS;
}
