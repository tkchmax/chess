#pragma once
#include <iostream>
#include <vector>

using namespace std;

#ifndef _HEADERS_H_
#define _HEADERS_H_

typedef unsigned long long U64;

#define INF 100000000000000000

#define KING 6
#define QUEEN 5
#define ROOK 4
#define BISHOP 3
#define KNIGHT 2
#define PAWN 1
#define NO_FIGURE 0

#define NORTH 0
#define SOUTH 1
#define WEST 2
#define EAST 3
#define NORTH_WEST 4
#define SOUTH_EAST 5
#define NORTH_EAST 6
#define SOUTH_WEST 7

#define WHITE 0
#define BLACK 1

#define WHITEPAWN_STARTPOSITION 0xFF00
#define BLACKPAWN_STARTPOSITION 0xFF000000000000
#define WHITEKNIGHT_STARTPOSITION 0x42
#define BLACKKNIGHT_STARTPOSITION 0x4200000000000000
#define WHITEBISHOP_STARTPOSITION 0x24
#define BLACKBISHOP_STARTPOSITION 0x2400000000000000
#define WHITEROOK_STARTPOSITION 0x81
#define BLACKROOK_STARTPOSITION 0x8100000000000000
#define WHITEQUEEN_STARTPOSITION 0x8
#define BLACKQUEEN_STARTPOSITION 0x800000000000000
#define WHITEKING_STARTPOSITION 0x10
#define BLACKKING_STARTPOSITION 0x1000000000000000

#define NOT_H_FILE ~0x8080808080808080
#define NOT_A_FILE ~0x0101010101010101
#define NOT_G_FILE ~4629771061636907072
#define NOT_B_FILE ~144680345676153346

#define ALL -1
#define MOVE_TYPE_SILENT 0
#define MOVE_TYPE_TAKE 1
#define MOVE_TYPE_0_0 2
#define MOVE_TYPE_0_0_0 3
#define MOVE_TYPE_2SQUARE 4
#define PAWN_TO_KNIGHT 5
#define PAWN_TO_BISHOP 6
#define PAWN_TO_ROOK 7
#define PAWN_TO_QUEEN 8

//Write-Read information about move
#define WRITE_FROM(from,num) (num|from)
#define WRITE_TO(to,num) (num|(to<<6))
#define WRITE_FIGURE(figure,num) (num|(figure<<12))
#define WRITE_CAPTURE(capture,num) (num|(capture<<15))
#define WRITE_MOVE_TYPE(mt, num) (num|(mt<<18))
#define WRITE_COLOR(color, num) (num|(color<<22))

#define READ_FROM(num) (num&63)
#define READ_TO(num) ((num>>6)&63)
#define READ_FIGURE(num) ((num>>12)&7)
#define READ_CAPTURE(num) ((num>>15)&7)
#define READ_MOVE_TYPE(num) ((num>>18)&15)
#define READ_COLOR(num) ((num>>22)& 1)


//namespace coords
//{
//	int a1 = 0;
//	int b1 = 1;
//	int c1 = 2;
//	int d1 = 3;
//	int e1 = 4;
//	int f1 = 5;
//	int g1 = 6;
//	int h1 = 7;
//}

#endif // !_HEADERS_H_

