//
//  Field.cpp
//  Cosmos
//
//  Created by tomotaka.yamasaki on 2014/09/04.
//
//

#include "Field.h"

USING_NS_CC;

//コンストラクタ
Field::Field(){
    
    std::vector<Sprite*> list;
    cocos2d::Sprite *tmp;
    for(int i=0; i<FIELD_SIZE; i++){
        for(int j=0; j<FIELD_SIZE; j++){
            tmp = Sprite::create();
            list.push_back(tmp);
        }
        block_in_field.push_back(list);
        list.clear();
    }
    
    for (int i=0; i<FIELD_SIZE*FIELD_SIZE; i++) {
        spiral_i.push_back(0);
        spiral_j.push_back(0);
    }
 
}

void Field::setFieldData(Size screen_brank){
        
    field_origin.y = screen_brank.height;
    field_origin.x = screen_brank.width;
    
    //メッシュの中心座標をfield_data配列に格納
    std::vector<FIELD_DATA> list;
    FIELD_DATA tmp;
    for (int i=0; i < FIELD_SIZE; i++) {
        for (int j=0; j < FIELD_SIZE; j++) {
            list.push_back(addFirstFieldData(i, j, tmp));
        }
        field_data.push_back(list);
        list.clear();
    }
}

//フィールド情報にブロック情報を追加するメソッド
void Field::setUpdateFieldData(Vec2 block_position, Color block_color){
    
    int field_num_x;
    int field_num_y;
    
    //配列番号を求める
    blockPosToFieldNum(block_position, &field_num_x, &field_num_y);
    
    //フィールド情報にブロックカラーを格納
    field_data[field_num_y][field_num_x].color = block_color;
    
}

//フィールドに設置されたブロックをセットするメソッド
void Field::setBlockInField(Sprite* block){
    
    int field_num_x;
    int field_num_y;
    
    //設置されたブロックの位置を特定する
    blockPosToFieldNum(block->getPosition(), &field_num_x, &field_num_y);
    
    block_in_field[field_num_y][field_num_x] = block;
    
}

//2次元配列を螺旋状にループさせるためのi,jの値をセットする
void Field::setSpiral(){
    int n = FIELD_SIZE;
    int k_max = (int)(n/2.0);
    std::vector<int> value;
    
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            int tmp = j;
            
            //格子点判定
            if (abs((n-1)/2.0-i)>abs((n-1)/2.0-j)) {
                tmp = i;
            }
            int k = (int)(k_max-abs((n-1)/2.0-tmp));
            int s = -4*k*(k-n)+1;
            
            int branch = n-1-2*k;
            
            if (i==k){
                value.push_back(s+(j-k));
            }
            else if (j==n-1-k){
                value.push_back(s+branch+(i-k));
            }
            else if (i==n-1-k){
                value.push_back(s+3*branch-(j-k));
            }
            else{
                value.push_back(s+4*branch-(i-k));
            }
        }
    }
    
    //valueの順番でi,jをソート
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            spiral_i[value[i*n+j]-1] = i;
            spiral_j[value[i*n+j]-1] = j;
        }
    }
    
    //末尾は不動ブロックなので削除
    spiral_i.pop_back();
    spiral_j.pop_back();
    
    //螺旋を中->外に変更
    std::reverse(spiral_i.begin(), spiral_i.end());
    std::reverse(spiral_j.begin(), spiral_j.end());

}

/*vectorで動的配列を作成するためのメソッド
 構造体で配列を作っているため，内部のデータを個別に設定する必要がある*/
FIELD_DATA Field::addFirstFieldData(int i, int j, FIELD_DATA tmp){
    tmp.coord.y = field_origin.y + BLOCK_SIZE/2.0f + BLOCK_SIZE*i;    //height側決定
    tmp.coord.x = field_origin.x + BLOCK_SIZE/2.0f + BLOCK_SIZE*j;      //width側決定
    tmp.color = BRANK;  //カラーはブランク
    tmp.gravity = gravity_list[i*FIELD_SIZE+j];    //重力リストから読み込み
    
    return tmp;
}

/*ブロックの座標をフィールドの配列番号に変換するメソッド．
2次元配列の配列番号を求めたいのでポインタを受け取ってそこに代入．返り値は無し*/
void Field::blockPosToFieldNum(cocos2d::Vec2 block_pos, int *field_num_x, int *field_num_y){
    
    *field_num_x = 0;     //x軸のフィールド番号
    *field_num_y = 0;     //y軸のフィールド番号
    
    for(int i=0; i<FIELD_SIZE; i++){
        for(int j=0; j<FIELD_SIZE; j++){
            if((block_pos.x >= field_data[i][j].coord.x-BLOCK_SIZE/2.0f)  &&
               (block_pos.x <  field_data[i][j].coord.x+BLOCK_SIZE/2.0f)  &&
               (block_pos.y >= field_data[i][j].coord.y-BLOCK_SIZE/2.0f) &&
               (block_pos.y <  field_data[i][j].coord.y+BLOCK_SIZE/2.0f) ){
                
                *field_num_x = j;
                *field_num_y = i;
            }
        }
    }
    
}

/*重力で移動できるブロックがあるかどうか判断するメソッド*/
bool Field::isBlockMovedByGravity(){
    for (int i=0; i<FIELD_SIZE; i++) {
        for (int j=0; j<FIELD_SIZE; j++) {
            switch (field_data[i][j].gravity) {
                case 0:
                    if (field_data[i][j].color!=BRANK && i+1 < FIELD_SIZE && field_data[i+1][j].color == BRANK) {
                        return (true);
                    }
                    break;
                    
                case 1:
                    if (field_data[i][j].color!=BRANK && j-1 >= 0 && field_data[i][j-1].color == BRANK) {
                        return (true);
                    }
                    break;
                    
                case 2:
                    if (field_data[i][j].color!=BRANK && i-1 >= 0 && field_data[i-1][j].color == BRANK) {
                        return (true);
                    }
                    break;
                    
                case 3:
                    if (field_data[i][j].color!=BRANK && j+1 < FIELD_SIZE && field_data[i][j+1].color == BRANK) {
                        return (true);
                    }
                    break;
                    
                default:
                    break;
            }

        }
    }
    return (false);
}

bool Field::isGameover(Vec2 pos){
    int x, y;
    
    blockPosToFieldNum(pos, &x, &y);
    
    if (field_data[y][x].color != BRANK) {
        return (true);
    }
    
    return (false);
}

/*メッシュの重力値を見て，ブロックが落ちる限界の座標を探索するメソッド*/
/*再帰して，最終的に落下限界座標を返す*/
Vec2 Field::moveBlockByGravivy(int y, int x, int gravity){
    
    Vec2 move_coord = field_data[y][x].coord;   //調査するメッシュの中心座標
    
    switch (gravity) {
        case 0:
            if (y+1 < FIELD_SIZE && field_data[y+1][x].color == BRANK) {
                move_coord = moveBlockByGravivy(y+1, x, field_data[y+1][x].gravity);
            }
            break;
            
        case 1:
            if (x-1 >= 0 && field_data[y][x-1].color == BRANK) {
                move_coord = moveBlockByGravivy(y, x-1, field_data[y][x-1].gravity);
            }
            break;
            
        case 2:
            if (y-1 >= 0 && field_data[y-1][x].color == BRANK) {
                move_coord = moveBlockByGravivy(y-1, x, field_data[y-1][x].gravity);
            }
            break;
            
        case 3:
            if (x+1 < FIELD_SIZE && field_data[y][x+1].color == BRANK) {
                move_coord = moveBlockByGravivy(y, x+1, field_data[y][x+1].gravity);
            }
            break;
            
        default:
            break;
    }
    
    return (move_coord);
    
}

/*field_dataに格納されているブロックの色情報を見て，
 隣接するブロック情報を空にするメソッド*/
std::vector<int> Field::deleteBlock(){
    
    int n;
    Color c;
    std::vector< std::vector<FIELD_DATA> > field_data_work;  //フィールド情報のコピー(作業用)

    field_data_work = field_data;
    
    vanish_block_tag.clear();
    
    for (int i=0; i<FIELD_SIZE; i++) {
        for (int j=0; j<FIELD_SIZE; j++) {
            c = field_data_work[i][j].color;
            if(c != BRANK && c != IMMOVABLE){
                n = 0;
                n = countBlock(i, j, field_data_work);
                if (n > DELETE_BLOCK) {
                    field_data_work = vanishBlock(i, j, field_data_work);                    
                }
            }
        }
    }

    //削除するブロックのタグリストを作成
    for (int i=0; i<FIELD_SIZE; i++) {
        for (int j=0; j<FIELD_SIZE; j++) {
            if(field_data[i][j].color != field_data_work[i][j].color){
                vanish_block_tag.push_back(1000*i+j);
            }
        }
    }
    
    field_data = field_data_work;

    return (vanish_block_tag);
}

/*隣接する同色ブロックの個数をカウントするメソッド*/
int Field::countBlock(int i, int j, std::vector< std::vector<FIELD_DATA> > field_data_work){
    Color c = field_data_work[i][j].color;
    int n = 1;
    
    field_data_work[i][j].color = BRANK;
    
    if (i+1<FIELD_SIZE && field_data_work[i+1][j].color==c) {
        n += countBlock(i+1, j, field_data_work);
    }
    if (j+1<FIELD_SIZE && field_data_work[i][j+1].color==c) {
        n += countBlock(i, j+1, field_data_work);
    }
    if (i-1>=0 && field_data_work[i-1][j].color==c) {
        n += countBlock(i-1, j, field_data_work);
    }
    if (j-1>=0 && field_data_work[i][j-1].color==c) {
        n += countBlock(i, j-1, field_data_work);
    }
    
    field_data_work[i][j].color = c;
    
    return (n);
}

/*隣接するブロックの色をBRANKにするメソッド*/
std::vector< std::vector<FIELD_DATA> > Field::vanishBlock(int i, int j, std::vector< std::vector<FIELD_DATA> > field_data_work){
    Color c = field_data_work[i][j].color;
    field_data_work[i][j].color = BRANK;
    
    if (i+1<FIELD_SIZE && field_data_work[i+1][j].color==c) {
        field_data_work = vanishBlock(i+1, j, field_data_work);
    }
    if (j+1<FIELD_SIZE && field_data_work[i][j+1].color==c) {
        field_data_work = vanishBlock(i, j+1, field_data_work);
    }
    if (i-1>=0 && field_data_work[i-1][j].color==c) {
        field_data_work = vanishBlock(i-1, j, field_data_work);
    }
    if (j-1>=0 && field_data_work[i][j-1].color==c) {
        field_data_work = vanishBlock(i, j-1, field_data_work);
    }
    
    return (field_data_work);
    
}