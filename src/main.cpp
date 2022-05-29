#include "ada/app.h"

#include "ada/geom/meshes.h"
#include "ada/shaders/defaultShaders.h"

#include "ada/scene/light.h"

#include <iostream>

using namespace std;
using namespace ada;
using namespace glm;

class myApp : public App {
    Vbo     myVbo;
    Shader  myShader;

    Light   myLight;
    Camera  myCamera;

    void setup() {
        myShader = createShader(FRAG_DEFAULT_SCENE, VERT_DEFAULT_SCENE);

        myVbo.load( cubeMesh(1.0f) );

        myCamera.setViewport(width, height);
        myCamera.setPosition( vec3(0.0f, 0.0f, -10.0f) );
        myCamera.lookAt( glm::vec3(0.0f, 0.0f, 0.0f) );
        
        myLight.setPosition( vec3(1.0f,1.0f,1.0f) );

        setCamera(myCamera);
        addLight(myLight);
    }

    void draw() {
        clear(0.0f);

        orbitControl();
        shader(myShader);
        model( myVbo );

    }

};

myApp       app;

int main(int argc, char **argv) {
    // Set the size and type of window
    ivec4 window_viewport = ivec4(0);
    window_viewport.z = 512;
    window_viewport.w = 512;

    #if defined(DRIVER_BROADCOM) || defined(DRIVER_GBM) 
    // RASPBERRYPI default windows size (fullscreen)
    ivec2 screen = getScreenSize();
    window_viewport.z = screen.x;
    window_viewport.w = screen.y;
    #endif

    WindowProperties window_properties;
    window_properties.msaa = 4;
    window_properties.major = 2.0;
    window_properties.minor = 0.0;

    // Initialize openGL context
    app.run(window_viewport);

    return 1;
}