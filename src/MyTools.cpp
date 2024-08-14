//
// Created by joachimverleysen on 4/1/24.
//

#include "MyTools.h"
#include "LineDrawer.h"
#include "Figure.h"
#include "Transformations.h"


Lines2D MyTools::getLineArray2D(const vector<Point2D> &points, Color col) {
    // Pairs the points in the pointArray 2 by 2 to create lines
    Lines2D result;
    for (int i=0; i<points.size(); i+=2) {
        Line2D line(points[i], points[i+1], col);
        line.z1 = points[i].z_val;
        line.z2 = points[i+1].z_val;
        result.push_back(line);
    }
    return result;
}

Lines2D MyTools::computeLines(Figure &fig) {
    // Make sure the fig.proj_points vector has been made when calling this
    Color col = fig.getColor();
    Lines2D result;
    for (auto face : fig.getFaces()) {
        vector<Point2D> points;
        if (face.point_indexes.size()==2) {
            for (auto i : face.point_indexes) {
                Point2D point = fig.getProjPoints()[i];
                points.push_back(point);
            }
            Lines2D lines = getLineArray2D(points, col);
            result.insert(result.end(), lines.begin(), lines.end());
        }
        else if (face.point_indexes.size() > 2) {
            for (int i = 0; i < face.point_indexes.size(); i++) {
                Point2D p1 = fig.getProjPoints()[face.point_indexes[i]];
                Point2D p2(-1,-1);
                if (i == face.point_indexes.size()-1) {
                    p2 = fig.getProjPoints()[face.point_indexes[0]];    // Connect last point with first point
                }
                else p2 = fig.getProjPoints()[face.point_indexes[i+1]];

                // Adding points in pairs to the vector to facilitate generating lineArray
                points.push_back(p1);
                points.push_back(p2);
            }
            Lines2D lines = getLineArray2D(points, col);
            result.insert(result.end(), lines.begin(), lines.end());
        }

    }
    return result;
}

void MyTools::triangulateFigure(Figure &fig) {
    vector<Face> result_faces;
    vector<Face> faces = fig.getFaces();
    for (auto& f : faces) {
        int n = f.point_indexes.size();
        for (int i=1; i<n-1; i++) {
            Face triangle({f.point_indexes[0], f.point_indexes[i], f.point_indexes[i+1]});
            result_faces.push_back(triangle);
        }
    }
    fig.setFaces(result_faces);
}

void MyTools::applyTransformation(Figure &fig, Matrix &mtx) {
    auto points = fig.getPoints();
    vector<Vector3D> points_new;
    for (auto p : points) {
        Vector3D point_new = Vector3D::point(p.x, p.y, p.z);
        point_new *= mtx;
        points_new.push_back(point_new);
    }
    fig.setPoints(points_new);

}
