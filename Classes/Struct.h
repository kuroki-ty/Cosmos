//
//  Struct.h
//  Cosmos
//
//  Created by tomotaka.yamasaki on 2014/09/04.
//
//

#ifndef Cosmos_Struct_h
#define Cosmos_Struct_h

#include <iostream>
#include "cocos2d.h"

#define FIELD_SIZE 15   //フィールドサイズ(ブロック15個分)
#define FIELD_SIZE_PX 600.0   //フィールドサイズ600px
#define BLOCK_SIZE 40.0  //ブロックサイズ40px
#define DELETE_BLOCK 3      //この個数より多ければブロックは消去される
#define IMMOVABLE_BLOCK 1   //不動ブロックの一列分の個数

//ブロックの色
enum Color{
    BLUE,
    RED,
    YELLOW,
    GREEN,
    VIOLET,
    IMMOVABLE,
    BRANK,
};

//ブロックの射出方向
enum Direction{
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

//ゲームフェーズ
enum GamePhase{
    INIT,       //初期化
    CREATE,     //生成
    FALL,       //落下
    FOUND,      //設置
    VANISH,     //消去
    GRAVITY,    //重力移動
    GAMEOVER,   //ゲームオーバー
};

//フィールド情報
struct FIELD_DATA{
    cocos2d::Vec2 coord; //メッシュの中心座標を示す
    Color color;    //ブロックの色
    int gravity;    //重力
};

//ブロック情報
struct BLOCK_DATA{
    cocos2d::Sprite *sprite;
    Color color;
    cocos2d::Vec2 shooting_position;
    Direction direction;
};

#endif
