//
// Created by joachimverleysen on 8/14/24.
//

#ifndef MYENGINE_TRANSFORMATIONS_H
#define MYENGINE_TRANSFORMATIONS_H


#include "Drawing3D.h"

class Transformations {
public:

static void apply_transformations(Figure &figure);

static void apply_transformations(Figures3D &figures);

static Lines2D do_projection(Figures3D &figures);

static void to_eye_system(Figures3D &figures, Vector3D eye);

static Vector3D to_eye_system(Vector3D &point, Vector3D &eye);

static vector<Point2D> compute_projected_points(Figure &figure);

static Point2D project_point(Vector3D p, double d=1);

static Matrix get_eye_trans_matrix(const Vector3D& eye);

static Matrix get_transformation_matrix(Matrix rot_x, Matrix rot_y, Matrix rot_z,
                                        Matrix scale, Matrix translate);

static Matrix get_rot_x(double angle_deg);
static Matrix get_rot_y(double angle_deg);
static Matrix get_rot_z(double angle_deg);
static Matrix get_scale_mtx(double scale);
static Matrix get_translate_mtx(Vector3D center);

static polars to_polar(const Vector3D &point);

static void to_eye_system(Figure &figure, const Vector3D eye);
};


#endif //MYENGINE_TRANSFORMATIONS_H
