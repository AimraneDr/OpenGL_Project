#ifndef MATH_DEFINES_H
#define MATH_DEFINES_H

#include "defines.h"

#define PI 3.14159265358979323846f
#define TWO_PI  6.28318530717958647692f
#define EPSILON 0.00000001f

typedef union matrix3x3{
    /// @brief first index determine row, second for column
    f32 elements[9];
    f32 table[3][3];
}Mat3;

typedef union matrix4x4{
    /// @brief first index determine row, second for column
    f32 elements[16];
    f32 table[4][4];
}Mat4;
typedef enum RotationType{
    ROT_DEFAULT,
    ROT_X,
    ROT_Y,
    ROT_Z,
    ROT_XY,
    ROT_XZ,
    ROT_YX,
    ROT_YZ,
    ROT_ZX,
    ROT_ZY,
    ROT_XYZ,
    ROT_XZY,
    ROT_YXZ,
    ROT_YZX,
    ROT_ZXY,
    ROT_ZYX
}RotationType;


/**
 * @brief A 2-element vector.
 */
typedef union Vector2 {
    /** @brief An array of x, y */
    f32 elements[2];
    struct {
        union {
            /** @brief The first element. */
            f32 x,
                /** @brief The first element. */
                r,
                /** @brief The first element. */
                s,
                /** @brief The first element. */
                u;
        };
        union {
            /** @brief The second element. */
            f32 y,
                /** @brief The second element. */
                g,
                /** @brief The second element. */
                t,
                /** @brief The second element. */
                v;
        };
    };
} Vec2;

/**
 * @brief A 3-element vector.
 */
typedef union Vector3 {
    /** @brief An array of x, y, z */
    f32 elements[3];
    struct {
        union {
            /** @brief The first element. */
            f32 x,
                /** @brief The first element. */
                r,
                /** @brief The first element. */
                s,
                /** @brief The first element. */
                u;
        };
        union {
            /** @brief The second element. */
            f32 y,
                /** @brief The second element. */
                g,
                /** @brief The second element. */
                t,
                /** @brief The second element. */
                v;
        };
        union {
            /** @brief The third element. */
            f32 z,
                /** @brief The third element. */
                b,
                /** @brief The third element. */
                p,
                /** @brief The third element. */
                w;
        };
    };
} Vec3;

/**
 * @brief A 4-element vector.
 */
typedef union Vector4 {
    /** @brief An array of x, y, z, w */
    f32 elements[4];
    union {
        struct {
            union {
                /** @brief The first element. */
                f32 x,
                    /** @brief The first element. */
                    r,
                    /** @brief The first element. */
                    s;
            };
            union {
                /** @brief The second element. */
                f32 y,
                    /** @brief The third element. */
                    g,
                    /** @brief The third element. */
                    t;
            };
            union {
                /** @brief The third element. */
                f32 z,
                    /** @brief The third element. */
                    b,
                    /** @brief The third element. */
                    p;
            };
            union {
                /** @brief The fourth element. */
                f32 w,
                    /** @brief The fourth element. */
                    a,
                    /** @brief The fourth element. */
                    q;
            };
        };
    };
} Vec4;

/** @brief A quaternion, used to represent rotational orientation. */
// typedef Vec4 Quaterion;

typedef Vec4 Quaternion;


/**
 * @brief Represents a single vertex in 3D space.
 */
typedef struct vertex_3d {
    /** @brief The position of the vertex */
    Vec3 position;
    /** @brief The normal of the vertex. */
    Vec3 normal;
    /** @brief The texture coordinate of the vertex. */
    Vec2 texcoord;
    /** @brief The colour of the vertex. */
    Vec4 colour;
    /** @brief The tangent of the vertex. */
    Vec3 tangent;
} Vertex3D;

/**
 * @brief Represents a single vertex in 2D space.
 */
typedef struct vertex_2d {
    /** @brief The position of the vertex */
    Vec2 position;
    /** @brief The texture coordinate of the vertex. */
    Vec2 texcoord;
} Vertex2D;

#endif /* MATH_DEFINES_H */