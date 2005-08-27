/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Copyright 2004-2005 Simon Newton
 *  nomis52@westnet.com.au
 */


#include <stdio.h>
#include <stdlib.h>
#include <lla/lla.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include <getopt.h>


typedef struct {
	int dev;
	int prt;
	int uni;
	int act;
	int help;
} options;

/*
 * parse our cmd line options
 *
 */
int parse_options(int argc, char *argv[], options *opts) {
	static struct option long_options[] = {
			{"patch", 		no_argument,		0, 'a'},
			{"unpatch", 	no_argument,		0, 'r'},
			{"device", 		required_argument, 	0, 'd'},
			{"port", 		required_argument, 	0, 'p'},
			{"universe", 	required_argument, 	0, 'u'},
			{"help", 		no_argument, 		0, 'h'},
			{0, 0, 0, 0}
		};

	int c ;
	int option_index = 0;
	
	while (1) {
     
		c = getopt_long(argc, argv, "ard:p:u:h", long_options, &option_index);
		
		if (c == -1)
			break;

		switch (c) {
			case 0:
				break;
				
			case 'a':
				opts->act = LLA_PORT_ACTION_PATCH;	
		        break;
			case 'd':
				opts->dev = atoi(optarg) ;
				break ;
			case 'p':
				opts->prt = atoi(optarg) ;
				break ;
			case 'r':
				opts->act = LLA_PORT_ACTION_UNPATCH;	
		        break;
			case 'u':
				opts->uni = atoi(optarg) ;
				break ;
			case 'h':
				opts->help = 1 ;
				break;
			case '?':
				break;
     
			default:
				;
		}
	}

	return 0;
}

/*
 * Init options
 */
void init_options(options *opts) {
	opts->dev = -1 ;
	opts->prt = -1 ;
	opts->uni = 0 ;
	opts->act = LLA_PORT_ACTION_PATCH ;
	opts->help = 0 ;
}

/*
 * Display the help message
 */
void display_help_and_exit() {

	printf(
"Usage: lla_patch [--patch | --unpatch] --device <dev> --port <port> [--universe <uni>]\n"
"\n"
"Control lla port <-> universe mappings.\n"
"\n"
"  -a, --patch              Patch this port (default).\n"
"  -d, --device <device>    Id of device to patch.\n"
"  -h, --help               Display this help message and exit.\n"
"  -p, --port <port>        Id of the port to patch.\n"
"  -r, --unpatch            Unpatch this port.\n"
"  -u, --universe <uni>     Id of the universe to patch to (default 0).\n"
"\n"
	) ;
	
	exit(0);
}

/*
 * main
 */
int main(int argc, char*argv[]) {
	lla_con con ;
	options opts ;

	init_options(&opts);
	parse_options(argc, argv, &opts) ;

	if(opts.help)
		display_help_and_exit() ;

	if(opts.dev == -1 || opts.prt == -1) {
		printf("Error: --device and --port must be supplied\n");
		exit(1) ;
	}

	con = lla_connect() ;

	if (!con) {
		printf("error: %s\n", strerror(errno) ) ;
		exit(1) ;
	}

	if( lla_patch(con, opts.dev, opts.prt, opts.act, opts.uni) ) {
		printf("patch failed\n") ;
	}

	lla_disconnect(con) ;

	return 0;
}
