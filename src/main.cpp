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
    vec3    satellite_pos;
    vector<vec3> orbit;

    Light   sun;

    void setup() {

        string frag = R"(
            #ifdef GL_ES
            precision mediump float;
            #endif

            uniform sampler2D   u_tex0;
            uniform vec3        u_light;

            #ifdef MODEL_VERTEX_NORMAL
            varying vec3        v_normal;
            #endif

            #ifdef MODEL_VERTEX_TEXCOORD
            varying vec2        v_texcoord;
            #endif

            void main () {
                vec3 color = vec3(1.);

                #ifdef MODEL_VERTEX_TEXCOORD
                color = vec3( 0.5 + texture2D(u_tex0, v_texcoord).r * 0.5);
                #endif
                
                #ifdef MODEL_VERTEX_NORMAL
                float shade = dot(v_normal, normalize(u_light));
                shade = smoothstep(-0.25, 0.25, shade);
                color *= 0.2 + shade * 0.8;
                // color = normalize(u_light);
                #endif

                gl_FragColor = vec4(color, 1.);
            }
        )";

        world_shader = createShader(frag);
        world.load( sphereMesh() );

        satellite.load( boxMesh(0.075f, 0.075f, 0.075f) );
        addLablel("Satellite", &satellite_pos);
        
        world_texture.load( "earth-water.png" );

        Camera* myCamera = createCamera();
        myCamera->setPosition( vec3(0.0f, 0.0f, -3.0f) );
        myCamera->lookAt( vec3(0.0f, 0.0f, 0.0f) );

        sun.setPosition( vec3(1.0f,1.0f,1.0f) );
        sun.setType(LIGHT_POINT);
        addLight(sun);

        background(0.0);
        blendMode(BLEND_ALPHA);
    }

    void draw() {
        orbitControl();

        sun.setPosition( vec3(cos(frameCount * 0.01f), 0.0, sin(frameCount * 0.01f)) );

        push();
        rotateY(frameCount * 0.0025f);
        shader(world_shader);
        texture(world_texture);
        model( world );
        pop();

        push();
        rotateY(frameCount * 0.0035f);
        rotateX(frameCount * 0.005f);
        translate(0.0f,0.0f,1.2f);
        fill(0.75f + sin(millis() * 0.005f) * 0.25f, 0.0f, 0.0f);
        model( satellite );
        satellite_pos = ( getWorldMatrix() * vec4(0.0f, 0.0f, 1.2f, 0.0f) ).xyz;
        pop();

        if (frameCount%15 == 0) {
            orbit.push_back( satellite_pos );
            if (orbit.size() > 500)
                orbit.erase(orbit.begin());
        }
    
        strokeWeight(1.0);
        stroke(0.75f, 0.0f, 0.0f);
        line(orbit);

        fill(1.0f);
        labels();

        textAlign(ALIGN_CENTER);
        textSize(28.0f);
        text("Hello World", width * 0.5f, height * 0.5f);
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