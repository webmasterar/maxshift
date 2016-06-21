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
 
#define WORD unsigned long int
#define WORDSIZE sizeof(WORD)
#define popcount(x) __builtin_popcountl((x))
#define delta(a,b) (int)((a) != (b))
#define shift(x) (x) << 1
#define shiftc(a,x) shift((a) & (x))
#define max(a,b) ((a) > (b)) ? (a) : (b)
#define min(a,b) ((a) < (b)) ? (a) : (b)
#define max3(a,b,c) max((a), max((b),(c)))

void maxshift_hd_ls ( unsigned char * p, unsigned int m, unsigned char * t, unsigned int n, unsigned int h, unsigned int * ii, unsigned int * jj, unsigned int * kk );

void maxshift_ed_ls ( unsigned char * p, unsigned int m, unsigned char * t, unsigned int n, unsigned int h, unsigned int * ii, unsigned int * jj, unsigned int * kk );
