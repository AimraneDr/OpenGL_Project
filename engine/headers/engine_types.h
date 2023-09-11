#ifndef ENGINE_TYPES_H
#define ENGINE_TYPES_H

#include "defines.h"

#include "platform/platform_types.h"
#include "math/math_defines.h"

#include "component_types.h"
#include "render/render_types.h"

typedef struct Pixel{
    u8 r,g,b,a;
}Pixel;

typedef struct Image{
    Pixel* pixels;
    u8* raw;
    s32 resolutionX,resolutionY;
}Img;

//data structure representing and storing the configurations of the system 

typedef enum {
    WINDOW_STATE_FLOAT,         //floating on the screen
    WINDOW_STATE_MAXED,         //full screen
    WINDOW_STATE_SUSPENDED,     //minimized
    WINDOW_STATE_OUT_OF_FOCUS   //not the top window
}windowStatus;

//data structure representing and storing configurations of the Window 
typedef struct windowConfig{
    char* name;
    u16 width, height;
    u32 x, y;
    bool resizable;
}windowConfig;

typedef struct PlatformState{
    WindowState w_state;
}PlatformState;

typedef struct Camera{
    //move to transform stract
    Transform transform;

    /// @brief stores width in x, height in y
    Vec2 rectSize;
    
    /// @brief stores nearPlane in x, farPlane in y
    Vec2 clipping;

    f32 fieldOfView;

    f32 speed;
    f32 sensitivity;

    // if transform had been updated this should be true so the view matrix get updated.
    bool view_deprecated, projection_deprecated;

    Mat4 viewMatrix;
    Mat4 projctionMatrix;
    Mat4 projViewMatrix;

}Camera;

typedef struct FrameInfo{
    Camera* camera;
    f64 deltaTime;
}FrameInfo;

/**
 * @brief Super Manager
 * 
 */
typedef struct SystemState{
    PlatformState platform;
    rendererState renderer;

    FrameInfo frame;
    bool shouldClose;

    /**
     * @brief Clent setup functionality, called when the system first initialize.
     * usualy used to define game parameters and to register any costume components or systems the game is supposed to use.
     * 
     */
    bool (*client_setup)();
    /**
     * @brief Clent game start functionality, called just before the start of the game loop.
     * here goes any logic needs to be set before running the game.
     */
    bool (*client_start)();
    /**
     * @brief Client's game update functionality, called each frame.
     * here goes the game update logic
     * 
     */
    bool (*client_update)(FrameInfo* frame);
    /**
     * @brief Client's game quit functionality, called when the game shuts down.
     * any last logic or cleanings needs to be done goes here
     * 
     */
    bool (*client_quit)();
}SystemState;




#endif /*ENGINE_TYPES_H*/