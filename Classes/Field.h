//
//  Field.h
//  Cosmos
//
//  Created by tomotaka.yamasaki on 2014/09/04.
//
//

#ifndef __Cosmos__Field__
#define __Cosmos__Field__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "Struct.h"

class Field : cocos2d::Layer{
public:
    //method
    Field();
    
    void blockPosToFieldNum(cocos2d::Vec2, int*, int*);
    cocos2d::Vec2 moveBlockByGravivy(int, int, int);   //ブロックの重力移動先を返すメソッド
    std::vector<int> deleteBlock(); //ブロック消去メソッド
    bool isBlockMovedByGravity();  //重力で動くブロックがあるかどうか
    bool isGameover(cocos2d::Vec2);              //ゲームオーバーかどうか
    
    //get method
    std::vector< std::vector<FIELD_DATA> > getFieldData(){
        return (field_data);
    }
    std::vector< std::vector<cocos2d::Sprite*> > getBlockInField(){
        return (block_in_field);
    }
    std::vector<int> getSpiral_i(){
        return (spiral_i);
    }
    std::vector<int> getSpiral_j(){
        return (spiral_j);
    }
    
    //set method
    void setFieldData(cocos2d::Size);
    void setUpdateFieldData(cocos2d::Vec2, Color);
    void setBlockInField(cocos2d::Sprite*);
    void setSpiral();
    
private:
    
    //variable
    cocos2d::Vec2 field_origin;  //フィールドの原点座標
    std::vector< std::vector<FIELD_DATA> > field_data;  //フィールド情報
    
    std::vector< std::vector<cocos2d::Sprite*> > block_in_field;    //フィールド内のブロック
    
    std::vector<int> vanish_block_tag;  //消去されるブロックのタグリスト
    
    std::vector<int> spiral_i;      //螺旋状にループさせるためのi
    std::vector<int> spiral_j;      //螺旋状にループさせるためのj
    
    int gravity_list[FIELD_SIZE*FIELD_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
                                               3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
                                               3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                                               3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
                                               3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
                                               3, 3, 3, 3, 3, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
                                               3, 3, 3, 3, 3, 3, 0, 0, 1, 1, 1, 1, 1, 1, 1,
                                               3, 3, 3, 3, 3, 3, 3, 4, 1, 1, 1, 1, 1, 1, 1,
                                               3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1,
                                               3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1,
                                               3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1,
                                               3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
                                               3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1,
                                               3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
                                               3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };   //ブロックを落とす重力リスト
    
    //method
    FIELD_DATA addFirstFieldData(int, int, FIELD_DATA);
    int countBlock(int, int, std::vector< std::vector<FIELD_DATA> >);     //隣接する同色ブロックの個数をカウントするメソッド
    std::vector< std::vector<FIELD_DATA> > vanishBlock(int, int, std::vector< std::vector<FIELD_DATA> >);        //隣接するブロックを消去するメソッド
};

#endif /* defined(__Cosmos__Field__) */
