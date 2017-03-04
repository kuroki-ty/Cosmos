#include "GameScene.h"

USING_NS_CC;

Scene* Controller::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Controller::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Controller::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //screenの左右のブランクをスクリーンサイズから求める
    switch((int)visibleSize.width){
            //iPhone 4inch
        case 1136:
            screen_brank.width = 268;
            screen_brank.height = 20;
            break;
            //iPhone 3.5inch
        case 960:
            screen_brank.width = 180;
            screen_brank.height = 20;
            break;
        default:
            screen_brank.width = 200;
            screen_brank.height = 20;
            break;
    }
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Controller::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    //十字キーをmenuボタンで作成
    auto upKeyItem = MenuItemImage::create("upkey.png", "upkey.png", CC_CALLBACK_1(Controller::upKeyCallback, this));
	upKeyItem->setPosition(Vec2(screen_brank.width - 85, screen_brank.height + 190));
    
    auto downKeyItem = MenuItemImage::create("downkey.png", "downkey.png", CC_CALLBACK_1(Controller::downKeyCallback, this));
	downKeyItem->setPosition(Vec2(screen_brank.width - 85, screen_brank.height + 90));
    
    auto rightKeyItem = MenuItemImage::create("rightkey.png", "rightkey.png", CC_CALLBACK_1(Controller::rightKeyCallback, this));
	rightKeyItem->setPosition(Vec2(screen_brank.width - 35, screen_brank.height + 140));
    
    auto leftKeyItem = MenuItemImage::create("leftkey.png", "leftkey.png", CC_CALLBACK_1(Controller::leftKeyCallback, this));
	leftKeyItem->setPosition(Vec2(screen_brank.width - 135, screen_brank.height + 140));
    
    //回転ボタンをmenuボタンで作成
    auto turnRightButtonItem = MenuItemImage::create("turn_right_button.png", "turn_right_button.png", CC_CALLBACK_1(Controller::turnRightButtonCallback, this));
	turnRightButtonItem->setPosition(Vec2(screen_brank.width+FIELD_SIZE_PX+10+75+37.5, screen_brank.height+150+37.5));
    auto turnLeftButtonItem = MenuItemImage::create("turn_left_button.png", "turn_left_button.png", CC_CALLBACK_1(Controller::turnLeftButtonCallback, this));
	turnLeftButtonItem->setPosition(Vec2(screen_brank.width+FIELD_SIZE_PX+10+37.5, screen_brank.height+75+37.5));
    
    //メニュー生成
    auto menu = Menu::create(closeItem, upKeyItem, downKeyItem, rightKeyItem, leftKeyItem, turnRightButtonItem, turnLeftButtonItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    field.setFieldData(screen_brank);               //フィールドの２次元配列作成
    blockController.setShootingPosition(screen_brank);        //ブロックの初期情報設定
    field.setSpiral();      //螺旋状にループさせるための値の設定
    
    //シングルタッチイベントリスナーを作成する
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(Controller::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Controller::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Controller::onTouchEnded, this);
    
    auto dip = Director::getInstance()->getEventDispatcher();
    dip->addEventListenerWithSceneGraphPriority(listener, this);
    
    //Background表示
    switch ((int)visibleSize.width) {
        case 1136:
            background = Sprite::create("background_1136.png");
            break;
        case 960:
            background = Sprite::create("background_960.png");
            break;
        default:
            background = Sprite::create("background_960.png");
            break;
    }
    background->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(background);
    
    //スコア表示
    levelString_score = StringUtils::format("000000");
    score_text = Label::createWithSystemFont(levelString_score, "Times new Roman", 40);
    score_text->setColor(Color3B(0,0,0));
    score_text->setPosition(Vec2(screen_brank.width-10-75, screen_brank.height+visibleSize.height/2+180));
    this->addChild(score_text);
    
    //タイム表示
    levelString_time = StringUtils::format("00:00");
    time_text = Label::createWithSystemFont(levelString_time, "Times new Roman", 48);
    time_text->setColor(Color3B(0,0,0));
    time_text->setPosition(Vec2(screen_brank.width-10-75, screen_brank.height+visibleSize.height/2+40));
    this->addChild(time_text);
    
    //ネクストブロックを待機させる座標を決定する
    next_waiting_position[0] = Vec2(screen_brank.width+FIELD_SIZE_PX+85, visibleSize.height-screen_brank.height-80);
    next_waiting_position[1] = Vec2(screen_brank.width+FIELD_SIZE_PX+85, visibleSize.height-screen_brank.height-80-BLOCK_SIZE);
    
    //不動ブロック設置
    //不動ブロックの設置位置をセット
    blockController.setImmovableBlockPosition(screen_brank);
    Vec2 *immovable_block_position = blockController.getImmovableBlockPosition();
    int x,y;
    for (int i=0; i<IMMOVABLE_BLOCK; i++) {
        Sprite *immovable_block;
        immovable_block = Sprite::create("immovable_block.png");
        immovable_block->setPosition(immovable_block_position[i]);
        this->addChild(immovable_block);
        field.blockPosToFieldNum(immovable_block_position[i], &x, &y);
        immovable_block->setTag(1000*y+x);
        field.setUpdateFieldData(immovable_block->getPosition(), IMMOVABLE);    //フィールドクラスに色をセット
        field.setBlockInField(immovable_block); //フィールドクラスにスプライト情報をセット
    }

    //ネクストブロック生成
    //block情報を取得
    block_color = blockController.getBlockColor();
    next_block_direction = blockController.getBlockDirection();
    shooting_position = blockController.getBlockShootingPosition(next_block_direction);
    
    //block情報を格納
    next_block_color[0] = block_color[0];
    next_block_color[1] = block_color[1];
    next_shooting_position[0] = shooting_position[0];
    next_shooting_position[1] = shooting_position[1];
    
    //block0
    Sprite *next_block0;
    next_block0 = Sprite::create("block000.png");
    addSpriteImage(next_block0, next_block_color[0]);
    next_block0->setPosition(next_waiting_position[0]);
    this->addChild(next_block0);
    next_block0->setTag(tag_count);
    
    //block1
    Sprite *next_block1;
    next_block1 = Sprite::create("block000.png");
    addSpriteImage(next_block1, next_block_color[1]);
    next_block1->setPosition(next_waiting_position[1]);
    this->addChild(next_block1);
    next_block1->setTag(tag_count+1);
    
    //arrow
    arrow = Sprite::create("arrow000.png");
    arrow->setPosition(Vec2(screen_brank.width+FIELD_SIZE_PX+85, screen_brank.height+320));
    this->addChild(arrow);
    
    game_phase = CREATE;
    
    //フィールドクラスの情報を更新
    field_data = field.getFieldData();  //フィールドデータを取得する
    block_in_field = field.getBlockInField();   //フィールドのスプライトデータを取得する
    blockController.setFoundBlock(block_in_field, field_data);
    
    this->scheduleUpdate();
    
    return true;
}


/****************************/
/*フレームごとに処理されるメソッド*/
/*ブロック生成→落下→設置→消去   */
/*↑の一連の流れを記述する      */
/****************************/
void Controller::update(float delta){
    
    //タイム計算
    game_frame++;
    secTime = (int)(game_frame/45);
    if(secTime>59){
        game_frame = 0;
        if(minTime < 60){
            minTime++;
        }
    }
    //タイム表示
    showTime(minTime, secTime);
    
    switch (game_phase) {
        case INIT:
        {
            chain_count = 0;
            
            field_data = field.getFieldData();  //新しくブロックを設置したのでフィールドデータを更新する
            
            for (int i=0; i<FIELD_SIZE; i++) {
                for (int j=0; j<FIELD_SIZE; j++) {
                    if (field_data[i][j].color != BRANK && field_data[i][j].color != IMMOVABLE) {
                        field.setBlockInField((Sprite*)this->getChildByTag(1000*i+j));
                    }
                }
            }
            
            //設置されたブロック情報を基に，BlockControllerクラスに設置ブロックのRect群をセット
            block_in_field = field.getBlockInField();
            blockController.setFoundBlock(block_in_field, field_data);
            
            turn_status = updateTurnCount(next_block_direction);
            
            game_phase = CREATE;
            
            break;
        }

        case CREATE:
        {
            
            Sprite *current_block0;
            Sprite *next_block0;
            Sprite *current_block1;
            Sprite *next_block1;
            
            //ネクストブロックをカレントブロックに
            //色，射出方向，射出座標
            current_block_color[0] = next_block_color[0];
            current_block_color[1] = next_block_color[1];
            current_block_direction = next_block_direction;
            current_shooting_position[0] = next_shooting_position[0];
            current_shooting_position[1] = next_shooting_position[1];
            //block0
            current_block0 = (Sprite*)this->getChildByTag(tag_count);
            current_block0->setPosition(current_shooting_position[0]);
            //block1
            current_block1 = (Sprite*)this->getChildByTag(tag_count+1);
            current_block1->setPosition(current_shooting_position[1]);
            
            //ブロック情報を決定する
            blockController.DecideNextBlockData();
            
            tag_count+=2;
            
            //ネクストブロック生成
            //block情報を取得
            block_color = blockController.getBlockColor();
            next_block_direction = blockController.getBlockDirection();
            shooting_position = blockController.getBlockShootingPosition(next_block_direction);
            
            //block情報を格納
            next_block_color[0] = block_color[0];
            next_block_color[1] = block_color[1];
            next_shooting_position[0] = shooting_position[0];
            next_shooting_position[1] = shooting_position[1];
            
            //block0
            next_block0 = Sprite::create("block000.png");
            addSpriteImage(next_block0, next_block_color[0]);
            next_block0->setPosition(next_waiting_position[0]);
            this->addChild(next_block0);
            next_block0->setTag(tag_count);
            //block1
            next_block1 = Sprite::create("block000.png");
            addSpriteImage(next_block1, next_block_color[1]);
            next_block1->setPosition(next_waiting_position[1]);
            this->addChild(next_block1);
            next_block1->setTag(tag_count+1);
            
            //矢印の向きを変更する
            rotateArrow(current_block_direction);
            
            //ゲームオーバー判定
            if(field.isGameover(current_shooting_position[0])){
                game_phase = GAMEOVER;
            }
            else{
                game_phase = FALL;
            }
            break;
        }
            
        case FALL:
        {
            Sprite *current_block0;
            Sprite *current_block1;
            
            current_block0 = (Sprite*)this->getChildByTag(tag_count-2);
            current_block1 = (Sprite*)this->getChildByTag(tag_count-1);
            
            //自由落下
            switch (current_block_direction) {
                case UP:
                    /*block0*/
                    //下壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block0->getPositionX(), current_block0->getPositionY()-block_speed))){
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block0, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                    
                    /*block1*/
                    //下壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block1->getPositionX(), current_block1->getPositionY()-block_speed))){
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block1, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                    
                    current_block0->setPosition(Vec2(current_block0->getPositionX(), current_block0->getPositionY()-block_speed));
                    current_block1->setPosition(Vec2(current_block1->getPositionX(), current_block1->getPositionY()-block_speed));
                    
                    break;
                    
                case DOWN:
                    /*block0*/
                    //上壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block0->getPositionX(), current_block0->getPositionY()+block_speed))) {
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block0, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                    
                    /*block1*/
                    //上壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block1->getPositionX(), current_block1->getPositionY()+block_speed))) {
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block1, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                
                    current_block0->setPosition(Vec2(current_block0->getPositionX(), current_block0->getPositionY()+block_speed));
                    current_block1->setPosition(Vec2(current_block1->getPositionX(), current_block1->getPositionY()+block_speed));
                    
                    break;

                case RIGHT:
                    /*block0*/
                    //左壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block0->getPositionX()-block_speed, current_block0->getPositionY()))) {
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block0, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                    
                    /*block1*/
                    //左壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block1->getPositionX()-block_speed, current_block1->getPositionY()))) {
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block1, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                    
                    current_block0->setPosition(Vec2(current_block0->getPositionX()-block_speed, current_block0->getPositionY()));
                    current_block1->setPosition(Vec2(current_block1->getPositionX()-block_speed, current_block1->getPositionY()));
                    
                    break;
                    
                case LEFT:
                    /*block0*/
                    //右壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block0->getPositionX()+block_speed, current_block0->getPositionY()))) {
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block0, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                    /*block1*/
                    //右壁衝突判定
                    if (blockController.isWallnearBlock(Vec2(current_block1->getPositionX()+block_speed, current_block1->getPositionY()))) {
                        //フェーズチェンジ
                        game_phase = FOUND;
                        break;
                    }
                    //ブロックとの衝突判定
                    if(blockController.isCollisionBlock(current_block1, current_block_direction, field_data, block_speed)){
                        game_phase = FOUND;
                        break;
                    }
                    
                    current_block0->setPosition(Vec2(current_block0->getPositionX()+block_speed, current_block0->getPositionY()));
                    current_block1->setPosition(Vec2(current_block1->getPositionX()+block_speed, current_block1->getPositionY()));
                    
                    break;
                    
                default:
                    break;
            }
            
            
            break;
        }
        case FOUND:
        {
            int field_num_x;
            int field_num_y;
            
            Sprite *current_block0;
            Sprite *current_block1;
            
            current_block0 = (Sprite*)this->getChildByTag(tag_count-2);
            current_block1 = (Sprite*)this->getChildByTag(tag_count-1);
            
            /*設置ブロックのタグをフィールドの座標に書き換え*/
            field.blockPosToFieldNum(current_block0->getPosition(), &field_num_x, &field_num_y);
            current_block0->setTag(1000*field_num_y+field_num_x);
            field.blockPosToFieldNum(current_block1->getPosition(), &field_num_x, &field_num_y);
            current_block1->setTag(1000*field_num_y+field_num_x);
            /********************/
            
            /*ブロックの色をフィールドデータに格納*/
            field.setUpdateFieldData(current_block0->getPosition(), current_block_color[0]);    //フィールドにブロック0のカラーを格納
            field.setUpdateFieldData(current_block1->getPosition(), current_block_color[1]);    //フィールドにブロック1のカラーを格納
            /****************/
            
            /*設置した位置でフィールドクラスにセット*/
//            field.setBlockInField(current_block0);  //設置したブロック0をフィールドクラスにセット
//            field.setBlockInField(current_block1);  //設置したブロック1をフィールドクラスにセット
            /********************/
            
            //スコア表示
            score += 5;    //スコア加算
            showScore(score);
            
            game_phase = GRAVITY;
            
            break;
        }
            
        case GRAVITY:
        {
            if(game_frame%10 != 0){
                break;
            }
            
            Vec2 move_coord;
            std::vector<int> spiral_i;      //螺旋状にループさせるためのi
            std::vector<int> spiral_j;      //螺旋状にループさせるためのj
            
            field_data = field.getFieldData();  //最新のフィールドデータを取得
            
            spiral_i = field.getSpiral_i();
            spiral_j = field.getSpiral_j();
            int i, j;
            for (int k=0; k<FIELD_SIZE*FIELD_SIZE-1; k++) {
                i = spiral_i[k];
                j = spiral_j[k];
                
                if (field_data[i][j].color != BRANK && field_data[i][j].color != IMMOVABLE) {
                    Sprite* block = (Sprite*)this->getChildByTag(1000*i+j);
                    int field_num_x;
                    int field_num_y;
                    Color c;
                    
                    //移動前にブロックの位置をBRANKにする
                    c = field_data[i][j].color;     //移動ブロックのカラーを保持
                    field.setUpdateFieldData(block->getPosition(), BRANK);
                    
                    //ブロックを重力移動させる
                    move_coord = field.moveBlockByGravivy(i, j, field_data[i][j].gravity);
                    block->setPosition(move_coord);
                    
                    //ブロックのタグを新座標に付け替え
                    field.blockPosToFieldNum(move_coord, &field_num_x, &field_num_y);
                    block->setTag(1000*field_num_y+field_num_x);
                    
                    //新座標の位置のブロックの色を更新する
                    field.setUpdateFieldData(move_coord, c);
                }
            }
            
            //まだ落とせるブロックがあれば再度重力フェーズ
            if(field.isBlockMovedByGravity()){
                game_phase = GRAVITY;
            }
            else{
                game_phase = VANISH;
            }

            break;
        }
            
        case VANISH:
        {
            if(game_frame%10 != 0){
                break;
            }
            /*ブロック消去*/
            vanish_block_tag = field.deleteBlock();     //消去可能なブロックのタグリストを取得
            
            if (vanish_block_tag.size() > 0) {
                
                //ブロックを消去
                for (int i=0; i<vanish_block_tag.size(); i++) {
                    field.setUpdateFieldData(getChildByTag(vanish_block_tag[i])->getPosition(), BRANK);
                    
                    ParticleGalaxy *p = ParticleGalaxy::create();
                    p->setDuration(0.1f);
                    p->setSpeed(100.0f);
                    p->setPosition(getChildByTag(vanish_block_tag[i])->getPosition());
                    this->addChild(p);
                    
                    getChildByTag(vanish_block_tag[i])->removeFromParentAndCleanup(true);
                }
                
                field_data = field.getFieldData();
                
                //スコア表示
                chain_count ++;
                score += 10*vanish_block_tag.size()*((vanish_block_tag.size()-3)*chain_count);   //スコア加算
                showScore(score);
                
                game_phase = GRAVITY;
                
            }
            
            else{
                game_phase = INIT;
            }
            
            break;
        }
        
        case GAMEOVER:
            Director::getInstance()->replaceScene(End::createScene());
            break;
            
        default:
            break;
    }
    
    

}

//Closeボタン処理
void Controller::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//上キー処理
void Controller::upKeyCallback(Ref* pSender_up){
    
    if(game_phase == FALL){
        Sprite* block0 = (Sprite*)this->getChildByTag(tag_count-2);
        Sprite* block1 = (Sprite*)this->getChildByTag(tag_count-1);
        
        
        future_position[0] = Vec2(block0->getPositionX(), block0->getPositionY()+BLOCK_SIZE);
        future_position[1] = Vec2(block1->getPositionX(), block1->getPositionY()+BLOCK_SIZE);
        
        if(!blockController.isCollisionBlock(block0, future_position[0], field_data) &&
           !blockController.isCollisionBlock(block1, future_position[1], field_data)){
            switch (current_block_direction) {
                case RIGHT:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                case LEFT:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

//下キー処理
void Controller::downKeyCallback(Ref* pSender_down){

    if(game_phase == FALL){
        Sprite* block0 = (Sprite*)this->getChildByTag(tag_count-2);
        Sprite* block1 = (Sprite*)this->getChildByTag(tag_count-1);
        
        future_position[0] = Vec2(block0->getPositionX(), block0->getPositionY()-BLOCK_SIZE);
        future_position[1] = Vec2(block1->getPositionX(), block1->getPositionY()-BLOCK_SIZE);
        
        if(!blockController.isCollisionBlock(block0, future_position[0], field_data) &&
           !blockController.isCollisionBlock(block1, future_position[1], field_data)){
            switch (current_block_direction) {
                case RIGHT:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                case LEFT:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

//右キー処理
void Controller::rightKeyCallback(Ref* pSender_right){
    
    if(game_phase == FALL){
        Sprite* block0 = (Sprite*)this->getChildByTag(tag_count-2);
        Sprite* block1 = (Sprite*)this->getChildByTag(tag_count-1);
        
        
        future_position[0] = Vec2(block0->getPositionX()+BLOCK_SIZE, block0->getPositionY());
        future_position[1] = Vec2(block1->getPositionX()+BLOCK_SIZE, block1->getPositionY());
        
        if(!blockController.isCollisionBlock(block0, future_position[0], field_data) &&
           !blockController.isCollisionBlock(block1, future_position[1], field_data)){
            switch (current_block_direction) {
                case UP:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                case DOWN:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

//左キー処理
void Controller::leftKeyCallback(Ref* pSender_left){
    
    if(game_phase == FALL){
        Sprite* block0 = (Sprite*)this->getChildByTag(tag_count-2);
        Sprite* block1 = (Sprite*)this->getChildByTag(tag_count-1);
        
        future_position[0] = Vec2(block0->getPositionX()-BLOCK_SIZE, block0->getPositionY());
        future_position[1] = Vec2(block1->getPositionX()-BLOCK_SIZE, block1->getPositionY());
        
        if(!blockController.isCollisionBlock(block0, future_position[0], field_data) &&
           !blockController.isCollisionBlock(block1, future_position[1], field_data)){
            switch (current_block_direction) {
                case UP:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                case DOWN:
                    if(!blockController.isWallnearBlock(future_position[0]) &&
                       !blockController.isWallnearBlock(future_position[1])){
                        block0->setPosition(future_position[0]);
                        block1->setPosition(future_position[1]);
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
}

//右回転ボタン処理
void Controller::turnRightButtonCallback(Ref* pSender_turnRight){
    
    if(game_phase == FALL){
        Sprite* block0 = (Sprite*)this->getChildByTag(tag_count-2);
        Vec2 future_position;
        
        switch (turn_status) {
            case DOWN:
                future_position = Vec2(block0->getPositionX()-BLOCK_SIZE, block0->getPositionY()+BLOCK_SIZE);
                break;
            case LEFT:
                future_position = Vec2(block0->getPositionX()+BLOCK_SIZE, block0->getPositionY()+BLOCK_SIZE);
                break;
            case UP:
                future_position = Vec2(block0->getPositionX()+BLOCK_SIZE, block0->getPositionY()-BLOCK_SIZE);
                break;
            case RIGHT:
                future_position = Vec2(block0->getPositionX()-BLOCK_SIZE, block0->getPositionY()-BLOCK_SIZE);
                break;
        }
        
        if(!blockController.isWallnearBlock(future_position) && !blockController.isCollisionBlock(block0, future_position, field_data)){
            block0->setPosition(future_position);
            
            switch (turn_status) {
                case DOWN:
                    turn_status = LEFT;
                    break;
                case LEFT:
                    turn_status = UP;
                    break;
                case UP:
                    turn_status = RIGHT;
                    break;
                case RIGHT:
                    turn_status = DOWN;
                    break;
            }
        }
    }
}

//左回転ボタン処理
void Controller::turnLeftButtonCallback(Ref* pSender_turnLeft){
    
    if(game_phase == FALL){
        Sprite* block0 = (Sprite*)this->getChildByTag(tag_count-2);
        Vec2 future_position;
        
        switch (turn_status) {
            case DOWN:
                future_position = Vec2(block0->getPositionX()+BLOCK_SIZE, block0->getPositionY()+BLOCK_SIZE);
                break;
            case RIGHT:
                future_position = Vec2(block0->getPositionX()-BLOCK_SIZE, block0->getPositionY()+BLOCK_SIZE);
                break;
            case UP:
                future_position = Vec2(block0->getPositionX()-BLOCK_SIZE, block0->getPositionY()-BLOCK_SIZE);
                break;
            case LEFT:
                future_position = Vec2(block0->getPositionX()+BLOCK_SIZE, block0->getPositionY()-BLOCK_SIZE);
                break;
        }
        
        if(!blockController.isWallnearBlock(future_position) && !blockController.isCollisionBlock(block0, future_position, field_data)){
            block0->setPosition(future_position);
            
            switch (turn_status) {
                case DOWN:
                    turn_status = RIGHT;
                    break;
                case RIGHT:
                    turn_status = UP;
                    break;
                case UP:
                    turn_status = LEFT;
                    break;
                case LEFT:
                    turn_status = DOWN;
                    break;
            }
        }
    }
}

//タッチした瞬間
bool Controller::onTouchBegan(Touch *touch, Event *event){

    return true;
}

//タッチ中
void Controller::onTouchMoved(Touch *touch, Event *event){
    
}

//タッチした後
void Controller::onTouchEnded(Touch *touch, Event *event){
    auto location = touch->getLocation();
}

void Controller::addSpriteImage(Sprite* blk, Color color){
    
    switch (color) {
        case BLUE:
            blk->setTexture(pTexture_blue);
            break;
        case RED:
            blk->setTexture(pTexture_red);
            break;
        case YELLOW:
            blk->setTexture(pTexture_yellow);
            break;
        case GREEN:
            blk->setTexture(pTexture_green);
            break;
            
        default:
            break;
    }
}

//turn_countの初期値を決定する
Direction Controller::updateTurnCount(Direction d){
    
    switch (d) {
        case UP:
            return (DOWN);
        case RIGHT:
            return (LEFT);
        case DOWN:
            return (UP);
        case LEFT:
            return (RIGHT);
    }
    
}

void Controller::showTime(int min, int sec){
    
    if (min<10 && sec<10) {
        levelString_time = StringUtils::format("0%d:0%d", min, sec);
    }
    else if (min<10 && sec>=10){
        levelString_time = StringUtils::format("0%d:%d", min, sec);
    }
    else if (min>=10 && sec<10){
        levelString_time = StringUtils::format("%d:%0d", min, sec);
    }
    else{
        levelString_time = StringUtils::format("%d:%d", min, sec);
    }
    time_text->setString(levelString_time);
    
}

void Controller::showScore(int s){
    
    //スコアの桁数は000000の6桁．
    //それぞれで表示形式を分ける
    if (s<10) {
        levelString_score = StringUtils::format("00000%d", score);
    }
    else if (s<100) {
        levelString_score = StringUtils::format("0000%d", score);
    }
    else if (s<1000) {
        levelString_score = StringUtils::format("000%d", score);
    }
    else if (s<10000) {
        levelString_score = StringUtils::format("00%d", score);
    }
    else if (s<100000) {
        levelString_score = StringUtils::format("0%d", score);
    }
    else {
        levelString_score = StringUtils::format("%d", score);
    }
    
    score_text->setString(levelString_score);
    
}

void Controller::rotateArrow(Direction d){
    
    switch (d) {
        case UP:
            arrow->setRotation(90.0f);
            break;
        case RIGHT:
            arrow->setRotation(180.0f);
            break;
        case DOWN:
            arrow->setRotation(270.0f);
            break;
        case LEFT:
            arrow->setRotation(0.0f);
            break;
            
    }
}

//デバッグ用
void Controller::debug_show(){
    
    std::cout << "/**************************/" << std::endl;
    for (int i=FIELD_SIZE-1; i>=0; i--) {
        for (int j=0; j<FIELD_SIZE; j++) {
            switch (field_data[i][j].color) {
                case BLUE:
                    std::cout << "○ ";
                    break;
                case RED:
                    std::cout << "△ ";
                    break;
                case YELLOW:
                    std::cout << "□ ";
                    break;
                case GREEN:
                    std::cout << "■ ";
                    break;
                case IMMOVABLE:
                    std::cout << "× ";
                    break;

                default:
                    std::cout << "◆ ";
                    break;
            }
        }
        std::cout << std::endl;
    }
    
    
}
