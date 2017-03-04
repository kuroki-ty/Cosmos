//
//  Block.h
//  Cosmos
//
//  Created by tomotaka.yamasaki on 2014/09/04.
//
//

#ifndef __Cosmos__Block__
#define __Cosmos__Block__

#include <iostream>
#include "cocos2d.h"
#include "Struct.h"

class BlockController : cocos2d::Layer{
    
public:
    
    //method
    BlockController();
    
    bool isWallnearBlock(cocos2d::Vec2);
    bool isCollisionBlock(cocos2d::Sprite*, Direction, std::vector< std::vector<FIELD_DATA> >, int);
    bool isCollisionBlock(cocos2d::Sprite*, cocos2d::Vec2, std::vector< std::vector<FIELD_DATA> >);
    
    void DecideNextBlockData(); //ブロックの色と射出位置を決めるメソッド
    
    //get method
    //不動ブロック位置を取得
    cocos2d::Vec2 *getImmovableBlockPosition(){
        return (immovable_block_position);
    }
    
    //現在のブロックカラーを取得
    Color *getBlockColor(){
        return (block_color);
    }
    
    //現在のブロック射出方向を取得
    Direction getBlockDirection(){
        return (block_direction);
    }
    
    //ブロック射出位置を取得(引数:ブロック射出方向)
    cocos2d::Vec2 *getBlockShootingPosition(Direction d){

        switch (d) {
            case UP:
                return (shooting_up_position);

            case RIGHT:
                return (shooting_right_position);

            case DOWN:
                return (shooting_down_position);

            case LEFT:
                return (shooting_left_position);

        }
    }
    
    //set method
    void setShootingPosition(cocos2d::Size brank){
        
        screen_brank = brank;
        
        //射出座標の決定
        //ブロック1
        shooting_up_position[0] = cocos2d::Vec2(screen_brank.width + FIELD_SIZE_PX/2.0, screen_brank.height + FIELD_SIZE_PX - BLOCK_SIZE/2 - BLOCK_SIZE);     //上
        shooting_down_position[0] = cocos2d::Vec2(screen_brank.width + FIELD_SIZE_PX/2.0, screen_brank.height + BLOCK_SIZE/2  + BLOCK_SIZE);   //下
        shooting_right_position[0] = cocos2d::Vec2(screen_brank.width + FIELD_SIZE_PX - BLOCK_SIZE/2 - BLOCK_SIZE, screen_brank.height + FIELD_SIZE_PX/2.0);  //右
        shooting_left_position[0] = cocos2d::Vec2(screen_brank.width + BLOCK_SIZE/2 + BLOCK_SIZE, screen_brank.height + FIELD_SIZE_PX/2.0);   //左
        //ブロック2
        shooting_up_position[1] = cocos2d::Vec2(screen_brank.width + FIELD_SIZE_PX/2.0, screen_brank.height + FIELD_SIZE_PX - BLOCK_SIZE/2);     //上
        shooting_down_position[1] = cocos2d::Vec2(screen_brank.width + FIELD_SIZE_PX/2.0, screen_brank.height + BLOCK_SIZE/2);   //下
        shooting_right_position[1] = cocos2d::Vec2(screen_brank.width + FIELD_SIZE_PX - BLOCK_SIZE/2, screen_brank.height + FIELD_SIZE_PX/2.0);  //右
        shooting_left_position[1] = cocos2d::Vec2(screen_brank.width + BLOCK_SIZE/2, screen_brank.height + FIELD_SIZE_PX/2.0);   //左
    }
    
    //フィールドに設置されたブロックのRectをセット
    void setFoundBlock(std::vector< std::vector<cocos2d::Sprite*> > found_blk, std::vector< std::vector<FIELD_DATA> > field_data){
        
        rect_found_block.clear();
        
        std::vector<cocos2d::Rect> rect_list;
        for (int i=0; i<FIELD_SIZE; i++) {
            for (int j=0; j<FIELD_SIZE; j++) {
                if(field_data[i][j].color != BRANK){
                    rect_list.push_back(found_blk[i][j]->boundingBox());
                }
                else{
                    rect_list.push_back(cocos2d::Rect(j*BLOCK_SIZE+screen_brank.width, i*BLOCK_SIZE+screen_brank.height, 0, 0));
                }
            }
            rect_found_block.push_back(rect_list);
            rect_list.clear();
        }
    }
    
    //不動ブロックの設置位置をセット
    void setImmovableBlockPosition(cocos2d::Size brank){
        for(int i=0; i<IMMOVABLE_BLOCK; i++){
            for(int j=0; j<IMMOVABLE_BLOCK; j++){
                immovable_block_position[IMMOVABLE_BLOCK*i+j] = cocos2d::Vec2((j+7)*BLOCK_SIZE+brank.width+BLOCK_SIZE/2, (i+7)*BLOCK_SIZE+brank.height+BLOCK_SIZE/2);
            }
        }
    }
    
private:
    
    //variable
    Color block_color[2];  //ブロックの色
    Direction block_direction;  //ブロック射出方向
    
    //スクリーンブランク
    cocos2d::Size screen_brank;
    
    //ブロック生成位置
    cocos2d::Vec2 shooting_up_position[2];     //上
    cocos2d::Vec2 shooting_down_position[2];   //下
    cocos2d::Vec2 shooting_right_position[2];  //右
    cocos2d::Vec2 shooting_left_position[2];   //左
    
    //不動ブロック位置
    cocos2d::Vec2 immovable_block_position[9];
    
    //設置されたブロックのRect情報
    std::vector< std::vector<cocos2d::Rect> > rect_found_block;
    
    //method
    Color DecideBlockColor();
    Direction DecideBlockDirection();
    
};


#endif /* defined(__Cosmos__Block__) */
