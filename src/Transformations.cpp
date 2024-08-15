//
// Created by joachimverleysen on 8/14/24.
//

#include "Transformations.h"

double degree_to_rad(double deg) {
    return deg*M_PI/180;
}

Matrix Transformations::get_rot_x(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(2,2) = cos(angle);
    a(2,3) = sin(angle);
    a(3,2) = -sin(angle);
    a(3,3) = cos(angle);
    return a;
}

Matrix Transformations::get_rot_y(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(1,1) = cos(angle);
    a(1,3) = -sin(angle);
    a(3,1) = sin(angle);
    a(3,3) = cos(angle);
    return a;
}

Matrix Transformations::get_rot_z(const double angle_deg) {
    double angle = degree_to_rad(angle_deg);
    Matrix a;
    a(1,1) = cos(angle);
    a(1,2) = sin(angle);
    a(2,1) = -sin(angle);
    a(2,2) = cos(angle);
    return a;
}

Matrix Transformations::get_scale_mtx(const double scale) {
    Matrix a;
    a(1,1) = scale;
    a(2,2) = scale;
    a(3,3) = scale;
    return a;
}

Matrix Transformations::get_translate_mtx(const Vector3D center) {
    Matrix a;
    a(4,1) = center.x;
    a(4,2) = center.y;
    a(4,3) = center.z;
    return a;
}

Matrix Transformations::get_transformation_matrix(const Matrix rot_x, const Matrix rot_y, const Matrix rot_z,
                                                  const Matrix scale, const Matrix translate) {
    return scale*rot_x*rot_y*rot_z*translate;
}

polars Transformations::to_polar(const Vector3D &point) {
    double theta;
    double phi;
    double r;
    r = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
    theta = atan2(point.y, point.x);
    if (theta < 0) {
        theta += 2 * M_PI; // Voeg 2π toe aan negatieve waarden om het interval te verschuiven naar [0, 2π)
    }
    phi = acos(point.z/r);
    // !!! atan in interval ]-pi/2, pi/2[
    polars p(r, theta, phi);
    return p;
}

Matrix Transformations::get_eye_trans_matrix(const Vector3D& eye) {
    Matrix m;
    polars p = to_polar(eye);
    m(1,1) = -sin(p.theta);
    m(1,2) = -cos(p.theta)*cos(p.phi);
    m(1,3) = cos(p.theta)*sin(p.phi);
    m(2,1) = cos(p.theta);
    m(2,2) = -sin(p.theta)*cos(p.phi);
    m(2,3) = sin(p.theta)*sin(p.phi);
    m(3,2) = sin(p.phi);
    m(3,3) = cos(p.phi);
    m(4, 3) = -p.r;
    return m;
}

Point2D Transformations::project_point(const Vector3D p, const double d) {
    // Point should already be in eye-coordinate system
    double x_new = d*p.x/-p.z;
    double y_new = d*p.y/-p.z;
    Point2D proj_point(x_new, y_new);
    proj_point.z_val=p.z;
    return proj_point;
}

vector<Point2D> Transformations::compute_projected_points(Figure &figure) {
    vector<Point2D> result;
    for (auto p : figure.getPoints()) {
        Point2D proj = project_point(p);
        result.push_back(proj);
    }
    return result;
}

void Transformations::to_eye_system(Figures3D &figures, const Vector3D eye) {
    Matrix eyeTrans = get_eye_trans_matrix(eye);
    for (auto &fig : figures) {
        vector<Vector3D> convertedPoints;
        for (const auto& p : fig.getPoints()) {
            Vector3D point_new = p*eyeTrans;
            convertedPoints.push_back(point_new);
        }
        fig.setPoints(convertedPoints);
    }

}

void Transformations::to_eye_system(Figure &figure, const Vector3D eye) {
    Matrix eyeTrans = get_eye_trans_matrix(eye);
    vector<Vector3D> convertedPoints;
    for (const auto& p : figure.getPoints()) {
        Vector3D point_new = p*eyeTrans;
        convertedPoints.push_back(point_new);
    }

}

void Transformations::apply_transformations(Figures3D &figures) {
    for (auto& fig : figures) {
        apply_transformations(fig);
    }
}

Lines2D Transformations::do_projection(Figures3D &figures) {
    // IMPORTANT: figures should already be converted to eye coordinate system
    Lines2D result;
    Figures3D figs = figures;
    for (Figure &fig : figs) {
        vector<Point2D> proj_points = Transformations::compute_projected_points(fig);
        fig.setProjPoints(proj_points);
        Lines2D  lines;
        if (fig.getType() == "3DLsystem") {
            lines = Figure::getLineArray2D(fig.getProjPoints(), fig.getColor());
        }
        else lines = Figure::computeLines(fig);
        result.insert(result.end(), lines.begin(), lines.end());
    }
    return result;
}

Vector3D Transformations::to_eye_system(Vector3D &point, Vector3D &eye) {
    Matrix eyeTrans = get_eye_trans_matrix(eye);
    Vector3D point_new = point*eyeTrans;
    return point_new;
}

void Transformations::apply_transformations(Figure &figure) {
    Matrix rot_x = Transformations::get_rot_x(figure.getRotateX());
    Matrix rot_y = Transformations::get_rot_y(figure.getRotateY());
    Matrix rot_z = Transformations::get_rot_z(figure.getRotateZ());
    Matrix scale = Transformations::get_scale_mtx(figure.getScale());
    Matrix translate = Transformations::get_translate_mtx(figure.getCenter());
    Matrix trans = get_transformation_matrix(rot_x, rot_y, rot_z, scale, translate);
    vector<Vector3D> converted_points;

    for (const auto& p : figure.getPoints()) {
        Vector3D point_new = p*trans;
        converted_points.push_back(point_new);
    }
    figure.setPoints(converted_points);
}

void Transformations::apply_transformation(Figure &figure, const Matrix& trans) {
    auto points = figure.getPoints();
    vector<Vector3D> points_new;
    for (auto p : points) {
        Vector3D point_new = Vector3D::point(p.x, p.y, p.z);
        point_new *= trans;
        points_new.push_back(point_new);
    }
    figure.setPoints(points_new);
}
