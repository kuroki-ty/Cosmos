//
//  BlockController.cpp
//  Cosmos
//
//  Created by tomotaka.yamasaki on 2014/09/04.
//
//

#include "BlockController.h"

USING_NS_CC;

BlockController::BlockController(){
    
    block_direction = UP;   //射出方向の初期値はUP
    
    block_color[0] = DecideBlockColor();
    block_color[1] = DecideBlockColor();
}

Color BlockController::DecideBlockColor(){
    Color color;

    int rand_num = arc4random()%4;
    
    switch (rand_num) {
        case 0:
            color = BLUE;
            break;
        case 1:
            color = RED;
            break;
        case 2:
            color = YELLOW;
            break;
        case 3:
            color = GREEN;
            break;
        case 4:
            color = VIOLET;
            break;
            
        default:
            break;
    }
    
    return (color);
}

Direction BlockController::DecideBlockDirection(){
    Direction direction;
    
    //射出方向は上→右→下→左と順繰りにループさせる
    switch (block_direction) {
        case UP:
            direction = RIGHT;
            break;
        case RIGHT:
            direction = DOWN;
            break;
        case DOWN:
            direction = LEFT;
            break;
        case LEFT:
            direction = UP;
            break;
            
        default:
            break;
    }
    
    return (direction);
}

//ブロックの色と射出位置を決めるメソッド
void BlockController::DecideNextBlockData(){
    block_color[0] = DecideBlockColor();
    block_color[1] = DecideBlockColor();
    block_direction = DecideBlockDirection();
}

//ブロックの周りに壁があるかどうか
bool BlockController::isWallnearBlock(Vec2 future_block_position){
    //移動先が壁の外であれば，false
    if(future_block_position.x < screen_brank.width+BLOCK_SIZE/2.0f ||
       future_block_position.x > screen_brank.width-BLOCK_SIZE/2.0f+FIELD_SIZE_PX ||
       future_block_position.y < screen_brank.height+BLOCK_SIZE/2.0f ||
       future_block_position.y > screen_brank.height-BLOCK_SIZE/2.0f+FIELD_SIZE_PX)
        return (true);
    else{
        return (false);
    }
}

/*ブロック同士の衝突判定(自由落下)
落下しているブロック(move_block)と設置してあるブロック(found_block)とで判定を行う*/
bool BlockController::isCollisionBlock(Sprite *move_block,Direction block_direction, std::vector< std::vector<FIELD_DATA> > field_data, int block_speed){
    
    Rect rect_move_block;       //落下ブロックの領域
    
    //move_blockのRectのサイズはブロックに対して縦長(width:10,height:BLOCK_SIZE+10)
    switch (block_direction) {
        case UP:
            rect_move_block = Rect(move_block->getPositionX()-BLOCK_SIZE/4, move_block->getPositionY()-BLOCK_SIZE/2, BLOCK_SIZE/2, BLOCK_SIZE);
            break;
        case DOWN:
            rect_move_block = Rect(move_block->getPositionX()-BLOCK_SIZE/4, move_block->getPositionY()-BLOCK_SIZE/2, BLOCK_SIZE/2, BLOCK_SIZE);
            break;
        case RIGHT:
            rect_move_block = Rect(move_block->getPositionX()-BLOCK_SIZE/2, move_block->getPositionY()-BLOCK_SIZE/4, BLOCK_SIZE, BLOCK_SIZE/4);
            break;
        case LEFT:
            rect_move_block = Rect(move_block->getPositionX()-BLOCK_SIZE/2, move_block->getPositionY()-BLOCK_SIZE/4, BLOCK_SIZE, BLOCK_SIZE/4);
            break;
            
        default:
            break;
    }
    

    //衝突していたらtrue
    for (int i=0; i<FIELD_SIZE; i++) {
        for (int j=0; j<FIELD_SIZE; j++) {
            if(field_data[i][j].color != BRANK){
                if(rect_move_block.intersectsRect(rect_found_block[i][j])){
                    return (true);
                }
            }
        }
    }
    
    return (false);

}

/*ブロック同士の衝突判定(上下左右移動)
 落下しているブロック(move_block)と設置してあるブロック(found_block)とで判定を行う*/
bool BlockController::isCollisionBlock(Sprite *move_block, Vec2 future_position, std::vector< std::vector<FIELD_DATA> > field_data){
    
    Rect rect_move_block;       //落下ブロックの領域
    
    rect_move_block = Rect(future_position.x-BLOCK_SIZE/4, future_position.y-BLOCK_SIZE/4, BLOCK_SIZE/2, BLOCK_SIZE/2);
    
    //衝突していたらtrue
    for (int i=0; i<FIELD_SIZE; i++) {
        for (int j=0; j<FIELD_SIZE; j++) {
            if(field_data[i][j].color != BRANK){
                if(rect_move_block.intersectsRect(rect_found_block[i][j])){
                    return (true);
                }
            }
        }
    }
    
    return (false);
    
}