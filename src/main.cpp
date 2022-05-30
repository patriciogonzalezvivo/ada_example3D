#include "ada/app.h"

#include "ada/geom/meshes.h"
#include "ada/shaders/defaultShaders.h"

#include "ada/scene/light.h"

#include <iostream>

using namespace std;
using namespace ada;
using namespace glm;

class myApp : public App {
    
    Vbo     world;
    Shader  world_shader;
    Texture world_texture;

    Vbo     satellite;

    Light   myLight;

    void setup() {

        string frag = R"(
            #ifdef GL_ES
            precision mediump float;
            #endif

            uniform sampler2D   u_tex0;

            #ifdef MODEL_VERTEX_TEXCOORD
            varying vec2        v_texcoord;
            #endif

            void main () {
                vec3 color = vec3(1.);
                
                #ifdef MODEL_VERTEX_TEXCOORD
                color = vec3( 0.5 + texture2D(u_tex0, v_texcoord).r * 0.5);
                #endif

                gl_FragColor = vec4(color, 1.);
            }
        )";

        world_shader = createShader(frag);
        world.load( sphereMesh() );
        satellite.load( boxMesh(0.1f, 0.1f, 0.1f) );
        
        world_texture.load( "earth-water.png" );

        CameraPtr myCamera = createCamera();
        myCamera->setPosition( vec3(0.0f, 0.0f, -3.0f) );
        myCamera->lookAt( vec3(0.0f, 0.0f, 0.0f) );
        
        myLight.setPosition( vec3(1.0f,1.0f,1.0f) );

        addLight(myLight);
        background(0.0);
    }

    void draw() {

        orbitControl();

        push();
        rotateY(frameCount * 0.0025f);
        shader(world_shader);
        texture(world_texture);
        model( world );
        pop();

        push();
        rotateY(frameCount * 0.005f);
        rotateX(frameCount * 0.005f);
        translate(0.0f,0.0f,1.2f);
        fill(1.0f, 0.0f, 0.0f);
        model( satellite );
        pop();

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