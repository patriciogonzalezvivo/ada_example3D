#include "ada/app.h"
#include "ada/gl/draw.h"
#include "ada/gl/meshes.h"
#include "ada/gl/shader.h"
#include "ada/shaders/defaultShaders.h"
#include "ada/scene/light.h"
#include "ada/scene/camera.h"

#include <iostream>

using namespace std;
using namespace ada;
using namespace glm;

class myApp : public App {
    Vbo*    geom;
    Shader  shader;

    Light   light;
    Camera  cam;
    float   lat = 180.0f;
    float   lon = 0.0f;

    void setup() {
        shader.load(getDefaultSrc(FRAG_DEFAULT_SCENE), 
                    getDefaultSrc(VERT_DEFAULT_SCENE));

        geom = cubeMesh(1.0f).getVbo();

        cam.setViewport(width, height);
        cam.setPosition( vec3(0.0f, 0.0f, -10.0f) );
        cam.lookAt( glm::vec3(0.0f, 0.0f, 0.0f) );
        
        light.setPosition( vec3(1.0f,1.0f,1.0f) );

        camera(true);
    }

    void draw() {
        clear(0.0f);

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        shader.use();
        shader.setUniform("u_modelViewProjectionMatrix", cam.getProjectionViewMatrix() );
        shader.setUniform("u_projectionMatrix", cam.getProjectionMatrix());
        shader.setUniform("u_normalMatrix", cam.getNormalMatrix());
        shader.setUniform("u_viewMatrix", cam.getViewMatrix() );

        shader.setUniform("u_camera", -cam.getPosition() );
        shader.setUniform("u_cameraDistance", cam.getDistance());
        shader.setUniform("u_cameraNearClip", cam.getNearClip());
        shader.setUniform("u_cameraFarClip", cam.getFarClip());
        shader.setUniform("u_cameraEv100", cam.getEv100());
        shader.setUniform("u_cameraExposure", cam.getExposure());
        shader.setUniform("u_cameraAperture", cam.getAperture());
        shader.setUniform("u_cameraShutterSpeed", cam.getShutterSpeed());
        shader.setUniform("u_cameraSensitivity", cam.getSensitivity());
        shader.setUniform("u_cameraChange", cam.bChange);
        shader.setUniform("u_iblLuminance", 30000.0f * cam.getExposure());

        // shader.setUniform("u_lightMatrix", light.getBiasMVPMatrix() );
        // shader.setUniformDepthTexture("u_lightShadowMap", light.getShadowMap(), .shader.textureIndex++ );
        if (light.getType() != LIGHT_DIRECTIONAL)
            shader.setUniform("u_light", light.getPosition());
        if (light.getType() == LIGHT_DIRECTIONAL || light.getType() == LIGHT_SPOT)
            shader.setUniform("u_lightDirection", light.direction);
        if (light.falloff > 0)
            shader.setUniform("u_lightFalloff", light.falloff);
        shader.setUniform("u_lightIntensity", light.intensity);
        shader.setUniform("u_lightColor", light.color);

        shader.setUniform("u_resolution", width, height );
        shader.setUniform("u_time", time);
        geom->render( &shader );

    }

    void mouseDragged() {
        float x = mouseX;
        float y = mouseY;

        if (x <= 0) x = width;
        else if (x > width) x = 0; 

        if (y <= 0) y = height - 2;
        else if (y >= height) y = 2;

        if (x != mouseX || y != mouseY) 
            setMousePosition(x, y);
    
        if (mouseButton == 1) {
            // Left-button drag is used to rotate geometry.
            float dist = cam.getDistance();

            float vel_x = movedX;
            float vel_y = movedY;

            if (fabs(vel_x) < 50.0 && fabs(vel_y) < 50.0) {
                lat -= vel_x;
                lon -= vel_y * 0.5;
                cam.orbit(lat, lon, dist);
                cam.lookAt(glm::vec3(0.0));
            }
        } 
        else {
            // Right-button drag is used to zoom geometry.
            float dist = cam.getDistance();
            dist += (-.008f * movedY);
            if (dist > 0.0f) {
                cam.orbit(lat, lon, dist);
                cam.lookAt(glm::vec3(0.0));
            }
        }
    }

    void onViewportResize(int _newWidth, int _newHeight) {
        cam.setViewport(_newWidth, _newHeight);
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