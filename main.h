#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstring>


typedef unsigned int uint;

#define ANGLE_TYPE_NONE     0
#define ANGLE_TYPE_UP       1
#define ANGLE_TYPE_STRAIGHT 2
#define ANGLE_TYPE_DOWN     3

#define ARROW_TYPE_LEFT     1
#define ARROW_TYPE_DOWN     2
#define ARROW_TYPE_RIGHT    3

typedef struct {
	int col_pos;
	int row_pos;
	int arrow;
	std::string value;
} USER;

typedef struct {
	int used;
	int row_pos;
	int opst_cpos;
	int opst_rpos;
    int angle_type;
} ROWITEM;

typedef struct {
	int col_pos;
	int row_cnt;
	int max_rcnt;
	std::string value;
	std::vector<ROWITEM> rows;
} COLITEM;

typedef struct {
	int cols_cnt;
	int rows_cnt;
	int height;
	std::vector<COLITEM> cols;
} GLOBAL;


// for memory search
COLITEM* getPrevColItem(int col_pos);
COLITEM* getNextColItem(int col_pos);
COLITEM* getColItemByIndex(int col_pos);
ROWITEM* getPrevRowItem(int col_pos, int row_pos);
ROWITEM* getNextRowItem(int col_pos, int row_pos);
ROWITEM* getRowItemByIndex(int col_pos, int row_pos);

// for data set
int getRandomNumber(int min, int max);
int getAngleType(int opst_cpos, int row_pos);
int setRowItem(int col_pos, int row_pos, ROWITEM* out_row);

// for data search
int getRowOppositeByPrevCol(int col_pos, int row_pos);
int getRowOppositeByNextCol(int col_pos, int row_pos);
int getRowOppositeByPrevNextCol(int col_pos, int row_pos);
int calcRowOppositeByAngleType(int row_pos, int angle_type);
int calcRowAngleTypeByOpposite(int row_pos, int opst_rpos);

// for play
int inputNumber(std::string description);

// for LOG
int printGhostLegAbout();
int printCurrentUserPosition(USER* user, std::string description);


#endif // _MAIN_H_

