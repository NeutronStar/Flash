#include <Flash++.h>
#include <stdio.h>

// use the UI worker reference (see sample 2 for more info)
using namespace AS3::ui;

class man_c
{
public:
    man_c(flash::display::Sprite s)
    {
        this->g = s->graphics;
        this->r = 25.0;
        this->x = 200.0;
        this->y = 200.0;
        jumping = false;
        falling = false;
        left = false;
        right = false;
    }

    ~man_c() {}

    void traverse()
    {
        if (this->left)
            this->x -= 10;
        if (this->right)
            this->x += 10;
    }

    void gravity()
    {
        if (this->jumping)
        {
            if (y > target)
                y-=5;
            else
            {
                jumping = false;
                falling = true;
            }
        }
        else
        {
            if (y <= 500)
                y+=5;
            else
                falling = false;
        }
    }

    void render()
    {
        this->g->clear();
        this->g->beginFill(0x0000ff, 1.0);
        this->g->drawCircle(this->x, this->y, this->r);
        this->g->endFill();      
    }

    void update()
    {
        this->traverse();
        this->gravity();
        this->render();
    }

    flash::display::Graphics g;
    int x;
    int y;
    int r;

    bool jumping;
    bool falling;
    bool left;
    bool right;
    int target;
};

man_c* man;

static var enterFrame(void *arg, var as3Args)
{
    try 
    {
        man->update();
    } 
    catch(var _e) 
    {
        exit(-1);
    }
    return internal::_undefined;
}

var keyDownHandler(void *arg, var as3Args){
    // get the event object
    flash::events::KeyboardEvent event = (flash::events::KeyboardEvent) as3Args[0];
    
    // pull some information out of it
    String type = event->type;
    Object target = event->target;
    int keyCode = event->keyCode;
        switch(keyCode) {
            case 37: // left
                man->left = true;
                break;
            case 39: // right
                man->right = true;
                break;
            case 18:
                if (!man->jumping && !man->falling)
                {
                    man->jumping = true;
                    man->target = man->y - 100;
                }
        }
    
    return internal::_undefined;
}

var keyUpHandler(void *arg, var as3Args){
    // get the event object
    flash::events::KeyboardEvent event = (flash::events::KeyboardEvent) as3Args[0];
    
    // pull some information out of it
    String type = event->type;
    Object target = event->target;
    int keyCode = event->keyCode;
        switch(keyCode) {
            case 37: // left
                man->left = false;
                break;
            case 39: // right
                man->right = false;
                break;
        }
    
    return internal::_undefined;
}

int main()
{
    flash::display::Stage stage = internal::get_Stage();
    stage->scaleMode = flash::display::StageScaleMode::NO_SCALE;
    stage->align = flash::display::StageAlign::TOP_LEFT;
    stage->frameRate = 60;
    
    flash::display::Sprite mySprite = flash::display::Sprite::_new();
    man = new man_c(mySprite);
    stage->addChild(mySprite);
    
    // handle keyboard events
    stage->addEventListener(flash::events::Event::ENTER_FRAME, Function::_new(enterFrame, NULL));
    stage->addEventListener(flash::events::KeyboardEvent::KEY_DOWN, Function::_new(keyDownHandler, NULL));
    stage->addEventListener(flash::events::KeyboardEvent::KEY_UP, Function::_new(keyUpHandler, NULL));
    
    // Throw an exception so main does not return normally
    // and cause the static destructors to be executed.
    AS3_GoAsync();
}