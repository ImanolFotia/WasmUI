#pragma once

#include "../string.hpp"

#include "vector.hpp"
#include <stdint.h>

namespace Math {
    struct mat4 {
        mat4() = default;
        mat4(vec4 a, vec4 b, vec4 c, vec4 d) {
            uint8_t i = 0;
            data[i++] = a.x; data[i++] = a.y; data[i++] = a.z; data[i++] = a.w;
            data[i++] = b.x; data[i++] = b.y; data[i++] = b.z; data[i++] = b.w;
            data[i++] = c.x; data[i++] = c.y; data[i++] = c.z; data[i++] = c.w;
            data[i++] = d.x; data[i++] = d.y; data[i++] = d.z; data[i++] = d.w;
        }
        mat4 identity() {
            memset(data, 0, 16*sizeof(float));
            data[0] = 1;
            data[5] = 1;
            data[10] = 1;
            data[15] = 1;
            return *this;
        }

        mat4 lookAt(vec3 pos, vec3 target, vec3 up) {
            vec3 f = (pos - target).normalize();
            vec3 r = up.cross(f).normalize();
/*
            float tx = pos.dot(r);
            float ty = pos.dot(up);
            float tz = pos.dot(f);

            return {vec4(r.x, r.y, r.z, 0), 
                    vec4(up.x, up.y, up.z, 0), 
                    vec4(f.x, f.y, f.z, 0), 
                    vec4(pos.x,  pos.y, pos.z, 1)};
*/
            return {vec4(  r.x,   up.x,   f.x, 0.0f), 
                    vec4(  r.y,   up.y,   f.y, 0.0f), 
                    vec4(  r.z,   up.z,   f.z, 0.0f), 
                    vec4(pos.x,  pos.y, pos.z, 1.0f)};
        }


        float data[16];
    };
};