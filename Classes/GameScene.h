#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Field.h"
#include "BlockController.h"
#include "EndScene.h"

class Controller : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    //十字キー用コールバック
    void upKeyCallback(cocos2d::Ref* pSender_up);
    void downKeyCallback(cocos2d::Ref* pSender_down);
    void rightKeyCallback(cocos2d::Ref* pSender_right);
    void leftKeyCallback(cocos2d::Ref* pSender_left);
    
    //回転ボタン用コールバック
    void turnRightButtonCallback(cocos2d::Ref* pSender_turnRight);
    void turnLeftButtonCallback(cocos2d::Ref* pSender_turnLeft);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Controller);
    
    //毎フレームごとに処理されるメソッド
    void update(float delta);
    
    //タッチ操作
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    
private:
    
    //variable
    BlockController blockController;    //BlockControllerクラス
    Field field;                        //Fieldクラス
    
    //Sprite
    cocos2d::Sprite *background;        //背景
    cocos2d::Sprite *upkey;             //上キー
    cocos2d::Sprite *downkey;           //下キー
    cocos2d::Sprite *rightkey;          //右キー
    cocos2d::Sprite *leftkey;           //左キー
    cocos2d::Sprite *turnRightButton;   //右回転ボタン
    cocos2d::Sprite *turnLeft_Button;   //左回転ボタン
    cocos2d::Sprite *arrow;             //射出方向を示す矢印
    
    //ラベル
    std::string levelString_score;
    cocos2d::Label *score_text;
    std::string levelString_time;
    cocos2d::Label *time_text;

    //フィールド
    cocos2d::Size visibleSize;
    std::vector< std::vector<FIELD_DATA> > field_data;
    cocos2d::Size screen_brank;     //スクリーンブランク
    
    //カレントブロック情報
    Color current_block_color[2];       //現在のブロックの色
    Direction current_block_direction;  //現在のブロック射出方向
    cocos2d::Vec2 current_shooting_position[2];    //現在のブロックを射出する座標
    
    //ネクストブロック情報
    Color next_block_color[2];          //次のブロックの色
    Direction next_block_direction;     //次のブロック射出方向
    cocos2d::Vec2 next_shooting_position[2];    //次のブロックを射出する座標
    cocos2d::Vec2 next_waiting_position[2];    //次のブロックを待機させる座標
    
    cocos2d::Vec2 future_position[2];   //上下左右移動の移動先座標
    
    //ブロック情報をBlockControllerクラスから取得するためのポインタ
    Color *block_color;
    cocos2d::Vec2 *shooting_position;

    int block_speed = 2;    //ブロックスピード(FILED_SIZE_PXで割り切れる数)
    int tag_count=10000;    //Spriteのタグ用カウンタ
    int score = 0;          //ゲームスコア
    int minTime = 0;        //タイム(分)
    int secTime = 0;        //タイム(秒)
    int chain_count = 0;    //スコア計算で使用する連鎖用カウンタ
    unsigned long int game_frame = 0;   //ゲームフレームカウント
    
    GamePhase game_phase = INIT;    //ゲームフェーズを管理する列挙型変数
    Direction turn_status=DOWN;       //blockの回転状態

    std::vector<int> vanish_block_tag;  //消去されるブロックのタグリスト
    std::vector< std::vector<cocos2d::Sprite*> > block_in_field;    //フィールドに設置されているブロック群
    
    //画像をTexture2Dで定義する
    cocos2d::Texture2D *pTexture_blue = cocos2d::TextureCache::sharedTextureCache()->addImage("block000.png");
    cocos2d::Texture2D *pTexture_red = cocos2d::TextureCache::sharedTextureCache()->addImage("block001.png");
    cocos2d::Texture2D *pTexture_yellow = cocos2d::TextureCache::sharedTextureCache()->addImage("block002.png");
    cocos2d::Texture2D *pTexture_green = cocos2d::TextureCache::sharedTextureCache()->addImage("block003.png");
    
    //method
    void addSpriteImage(cocos2d::Sprite*, Color);  //Blockクラスで決定したものを基にSpriteに画像を加える
    Direction updateTurnCount(Direction);     //turn_countの初期値を決定する
    void showTime(int, int);
    void showScore(int);
    void rotateArrow(Direction);
    void removeBlock();
    
    void debug_show();
    
};

#endif // __HELLOWORLD_SCENE_H__
