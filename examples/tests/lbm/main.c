/* $Id: main.c,v 1.4 2004/04/21 04:23:43 pohlt Exp $ */

/*############################################################################*/

#include "main.h"
#include "lbm.h"
#include <stdio.h>
#include <stdlib.h>

#include <edge_call.h>
#include "syscall.h"

#if defined(SPEC)
#   include <time.h>
#else
#   include <sys/times.h>
#   include <unistd.h>
#endif

#include <sys/stat.h>

/*############################################################################*/

static LBM_GridPtr srcGrid, dstGrid;

/*############################################################################*/

void __attribute__((__section__(".text._start"))) main() {
	MAIN_Param param;
#if !defined(SPEC)
	MAIN_Time time;
#endif
	int t;
	int nArgs;
  char* arg[6];

	int x = SIZE_X;
	int y = SIZE_Y;
	int z = SIZE_Z;

	ocall(2,&x,sizeof(int),NULL,0,0);
	ocall(2,&y,sizeof(int),NULL,0,0);
	ocall(2,&z,sizeof(int),NULL,0,0);
	ocall(1,"########\n",10,NULL,0,0);

	MAIN_parseCommandLine( nArgs, arg, &param );
	MAIN_printInfo( &param );
	MAIN_initialize( &param );
/*#if !defined(SPEC)
	MAIN_startClock( &time );
#endif*/

	for( t = 1; t <= param.nTimeSteps; t++ ) {
		if( param.simType == CHANNEL ) {
			LBM_handleInOutFlow( *srcGrid );
		}

		LBM_performStreamCollideTRT( *srcGrid, *dstGrid );
		LBM_swapGrids( &srcGrid, &dstGrid );

		if( (t & 63) == 0 ) {
			//printf( "timestep: %i\n", t );
			ocall(1,"Timestep Complete\n",19,NULL,0,0);
			LBM_showGridStatistics( *srcGrid );
		}
	}
	ocall(1,"Complete\n",10,NULL,0,0);

/*#if !defined(SPEC)
	MAIN_stopClock( &time, &param );
#endif*/
	MAIN_finalize( &param );
	char *j=(char*)0x0;
	*j=*j;

	EAPP_RETURN(0);
}

/*############################################################################*/

void MAIN_parseCommandLine( int nArgs, char* arg[], MAIN_Param* param ) {
	struct stat fileStat;

	/*if( nArgs < 5 || nArgs > 6 ) {
		//printf( "syntax: lbm <time steps> <result file> <0: nil, 1: cmp, 2: str> <0: ldc, 1: channel flow> [<obstacle file>]\n" );
		ocall(1,"Wrong call\n",12,NULL,0,0);
		exit( 1 );
	}*/

	param->nTimeSteps     =  1;     //atoi( arg[1] );
	param->resultFilename =  "Store_result";         //arg[2];
	param->action         = (MAIN_Action)0;      //atoi( arg[3] );
	param->simType        = (MAIN_SimType)0;       //atoi( arg[4] );
	param->obstacleFilename = NULL;

	/*if( nArgs == 6 ) {
		param->obstacleFilename = arg[5];

		if( stat( param->obstacleFilename, &fileStat ) != 0 ) {
			//printf( "MAIN_parseCommandLine: cannot stat obstacle file '%s'\n",
			//         param->obstacleFilename );
			ocall(1,"Cannot stat obstacle file\n",27,NULL,0,0);
			exit( 1 );
		}
		if( fileStat.st_size != SIZE_X*SIZE_Y*SIZE_Z+(SIZE_Y+1)*SIZE_Z ) {
			//printf( "MAIN_parseCommandLine:\n"
			//        "\tsize of file '%s' is %i bytes\n"
			//		    "\texpected size is %i bytes\n",
			//        param->obstacleFilename, (int) fileStat.st_size,
			//        SIZE_X*SIZE_Y*SIZE_Z+(SIZE_Y+1)*SIZE_Z );
			ocall(1,"Unexpected Size\n",17,NULL,0,0);
			exit( 1 );
		}
	}
	else param->obstacleFilename = NULL;*/

	/*if( param->action == COMPARE &&
	    stat( param->resultFilename, &fileStat ) != 0 ) {
		//printf( "MAIN_parseCommandLine: cannot stat result file '%s'\n",
		//         param->resultFilename );
		ocall(1,"Cannot stat result file\n",25,NULL,0,0);
		exit( 1 );
	}*/
}

/*############################################################################*/

void MAIN_printInfo( const MAIN_Param* param ) {
	const char actionString[3][32] = {"nothing", "compare", "store"};
	const char simTypeString[3][32] = {"lid-driven cavity", "channel flow"};
	ocall(1,"Main Print Info Trimmed\n",25,NULL,0,0);
	/*printf( "MAIN_printInfo:\n"
	        "\tgrid size      : %i x %i x %i = %.2f * 10^6 Cells\n"
	        "\tnTimeSteps     : %i\n"
	        "\tresult file    : %s\n"
	        "\taction         : %s\n"
	        "\tsimulation type: %s\n"
	        "\tobstacle file  : %s\n\n",
	        SIZE_X, SIZE_Y, SIZE_Z, 1e-6*SIZE_X*SIZE_Y*SIZE_Z,
	        param->nTimeSteps, param->resultFilename,
	        actionString[param->action], simTypeString[param->simType],
	        (param->obstacleFilename == NULL) ? "<none>" :
	                                            param->obstacleFilename );*/
}

/*############################################################################*/

void MAIN_initialize( const MAIN_Param* param ) {
	LBM_allocateGrid( (double**) &srcGrid );
	LBM_allocateGrid( (double**) &dstGrid );

	LBM_initializeGrid( *srcGrid );
	LBM_initializeGrid( *dstGrid );

	if( param->obstacleFilename != NULL ) {
		ocall(1,"Reading Obstacle\n",18,NULL,0,0);
		LBM_loadObstacleFile( *srcGrid, param->obstacleFilename );
		LBM_loadObstacleFile( *dstGrid, param->obstacleFilename );
	}

	if( param->simType == CHANNEL ) {
		LBM_initializeSpecialCellsForChannel( *srcGrid );
		LBM_initializeSpecialCellsForChannel( *dstGrid );
	}
	else {
		LBM_initializeSpecialCellsForLDC( *srcGrid );
		LBM_initializeSpecialCellsForLDC( *dstGrid );
	}

	LBM_showGridStatistics( *srcGrid );
}

/*############################################################################*/

void MAIN_finalize( const MAIN_Param* param ) {
	LBM_showGridStatistics( *srcGrid );

	if( param->action == COMPARE )
	ocall(1,"Comparison Disabled\n",21,NULL,0,0);
		//LBM_compareVelocityField( *srcGrid, param->resultFilename, TRUE );
	if( param->action == STORE )
	ocall(1,"Storing Disabled\n",18,NULL,0,0);
	//LBM_storeVelocityField( *srcGrid, param->resultFilename, TRUE );

	LBM_freeGrid( (double**) &srcGrid );
	LBM_freeGrid( (double**) &dstGrid );
}

/*#if !defined(SPEC)
//############################################################################

void MAIN_startClock( MAIN_Time* time ) {
	time->timeScale = 1.0 / sysconf( _SC_CLK_TCK );
	time->tickStart = times( &(time->timeStart) );
}


//############################################################################

void MAIN_stopClock( MAIN_Time* time, const MAIN_Param* param ) {
	time->tickStop = times( &(time->timeStop) );

	//printf( "MAIN_stopClock:\n"
	//        "\tusr: %7.2f sys: %7.2f tot: %7.2f wct: %7.2f MLUPS: %5.2f\n\n",
	 //       (time->timeStop.tms_utime - time->timeStart.tms_utime) * time->timeScale,
	  //      (time->timeStop.tms_stime - time->timeStart.tms_stime) * time->timeScale,
	  //      (time->timeStop.tms_utime - time->timeStart.tms_utime +
	  //       time->timeStop.tms_stime - time->timeStart.tms_stime) * time->timeScale,
	 //       (time->tickStop           - time->tickStart          ) * time->timeScale,
	//        1.0e-6 * SIZE_X * SIZE_Y * SIZE_Z * param->nTimeSteps /
	//        (time->tickStop           - time->tickStart          ) / time->timeScale );
	ocall(1,"Main stop clock\n",17,NULL,0,0);
}
#endif*/
