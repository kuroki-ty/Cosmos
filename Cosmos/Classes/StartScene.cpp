#include "StartScene.h"

USING_NS_CC;

Scene* Start::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Start::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Start::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(Start::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    //メニュー生成
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    
    //シングルタッチイベントリスナーを作成する
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(Start::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Start::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Start::onTouchEnded, this);
    
    auto dip = Director::getInstance()->getEventDispatcher();
    dip->addEventListenerWithSceneGraphPriority(listener, this);
    
    //Background表示
    Sprite *background;
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
    
    //スタート画像表示
    Sprite *start_image;
    start_image = Sprite::create("start.png");
    start_image->setScale(2.0f);
    start_image->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(start_image);
    
    return true;
}

//Closeボタン処理
void Start::menuCloseCallback(Ref* pSender)
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

//タッチした瞬間
bool Start::onTouchBegan(Touch *touch, Event *event){
    
    Director::getInstance()->replaceScene(Controller::createScene());

    return true;
}

//タッチ中
void Start::onTouchMoved(Touch *touch, Event *event){
    
}

//タッチした後
void Start::onTouchEnded(Touch *touch, Event *event){

}
