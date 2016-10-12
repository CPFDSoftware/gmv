/*
 * Copyright 1994, Integrated Computer Solutions, Inc.
 *
 * All Rights Reserved.
 *
 * Author: Rick Umali
 *
 * extlist.h
 *
 */

#ifndef _extlist_h
#define _extlist_h
typedef enum { PPORSCHE, PCLOWN, PSTOP, PCRAB } PlayerPix;

#define NUM_PLAYERS 14

/**************************************************************
 *		DATA STRUCTURES
 **************************************************************/
typedef struct _DemoInfo {
  Widget extlist;
  Widget rem_pb;
  Widget player[14];
} DemoStruct, *DemoInfo;

typedef struct _PlayerData {
  PlayerPix picture;
    char *name;
    int at_bats;
    int runs;
    int hits;
    int rbi;
    int average; /* entered as an integer...displayed as a "float" */
    int home_runs;
} PlayerData;

#define NUM_COLUMNS 8
#define MAX_ARGS 15
#define LINEUP_LIMIT 9
#endif
