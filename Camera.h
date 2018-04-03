//
// Created by jeff2310 on 3/28/18.
//

#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera{
private:
    quat _quaternion;
    vec3 _cameraPos;
    float _pitch, _yaw, _roll;
    mat4 _viewMatrix;
    vec3 _front, _up, _right;
public:
    Camera(vec3 pos = vec3(0.0f, 0.0f, 0.0f)){
        // right hand coordinate system
        _cameraPos = pos;
        _quaternion = quat(0.0f, 0.0f, 0.0f, 1.0f);
        _front = vec3(0.0f, 0.0f, -1.0f);
        _up = vec3(0.0f, 1.0f, 0.0f);
        _right = vec3(1.0f, 0.0f, 0.0f);
        _viewMatrix = mat4_cast(_quaternion);
        _pitch = _yaw = _roll = 0.0f;
    }
    void updateView(float delta_pitch, float delta_yaw, float delta_roll = 0.0f){

        _pitch += delta_pitch;
        _yaw += delta_yaw;
        _roll += delta_roll;
        if(_pitch > radians(89.0f))
            _pitch = radians(89.0f);
        if(_pitch < radians(-89.0f))
            _pitch = radians(-89.0f);
        if(_yaw > radians(359.0f))
            _yaw -= radians(359.0f);
        if(_yaw < radians(-359.0f))
            _yaw += radians(359.0f);

        // yaw globally and pitch locally
        _quaternion = quat(glm::vec3(_pitch, 0.0f, _roll)) * angleAxis(_yaw, vec3(0.0f, 1.0f, 0.0f));

        mat4 rotate = mat4_cast(_quaternion);
        mat4 translate = mat4(1.0f);
        translate = glm::translate(translate,  -_cameraPos);
        _viewMatrix = rotate * translate;
    }
    void updatePos(vec3 delta_r){
        // why is it -yaw here?
        // because when yaw angle increases while X position decreases.
        _cameraPos += (angleAxis(-_yaw, vec3(0.0f, 1.0f, 0.0f))) * vec3(delta_r.x, delta_r.y, delta_r.z) ;
        mat4 rotate = mat4_cast(_quaternion);
        mat4 translate = mat4(1.0f);
        _viewMatrix = translate * glm::translate(rotate,  -_cameraPos);
    }
    void updatePos(float delta_x, float delta_y, float delta_z){
        _cameraPos += angleAxis(_yaw, vec3(0.0f, 1.0f, 0.0f)) * vec3(delta_x, 0.0f, delta_z) + vec3(0.0f, delta_y, 0.0f);
        mat4 rotate = mat4_cast(_quaternion);
        mat4 translate = mat4(1.0f);
        _viewMatrix = rotate * glm::translate(translate,  -_cameraPos);
    }
    vec3 pos(){
        return _cameraPos;
    }
    vec3 euler(){
        return vec3(_pitch, _yaw, _roll);
    }
    mat4 viewMatrix(){
        return  _viewMatrix;
    }
    vec3 front(){
        return _front;
    }
    vec3 up(){
        return _up;
    }
    vec3 right(){
        return _right;
    }

    void reset(){
        _cameraPos = vec3(0.0f, 0.0f, 0.0f);
        _quaternion = quat(0.0f, 0.0f, 0.0f, 1.0f);
        _front = vec3(0.0f, 0.0f, -1.0f);
        _up = vec3(0.0f, 1.0f, 0.0f);
        _right = vec3(1.0f, 0.0f, 0.0f);
        _yaw = _pitch = _roll = 0.0f;
        _viewMatrix = mat4(1.0f);
    }
};

#endif //GRAPHICS_CAMERA_H
