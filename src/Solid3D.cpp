//
// Created by joachimverleysen on 3/27/24.
//

#include "Solid3D.h"

void Solid3D::generateCube(Figure &fig) {
    pair<int, int> face_indxs_dims(6, 4);
    int pointArr[8][3] = {
        {1,-1,-1},
        {-1,1,-1},
        {1,1,1},
        {-1,-1,1},
        {1,1,-1},
        {-1,-1,-1},
        {1,-1,1},
        {-1,1,1},
    };

    int face_indexes[6][4] = {
            {1, 5, 3, 7},
            {5, 2, 8, 3},
            {2, 6, 4, 8},
            {6, 1, 7, 4},
            {7, 3, 8, 4},
            {1, 6, 2, 5},
    };

    vector<Vector3D> points;
    for (auto p : pointArr) {
        Vector3D point = Vector3D::point(p[0], p[1], p[2]);
        points.push_back(point);
    }
    fig.setPoints(points);

    vector<Face> faces;
    for (auto f : face_indexes) {
        Face face;
        vector<int> indexes;
        for (int i=0; i < face_indxs_dims.second; i++) {
            indexes.push_back(f[i]-1);  // -1 because index start at 0
        }
        face.point_indexes = indexes;
        faces.push_back(face);
    }
    fig.setFaces(faces);
}

void Solid3D::generateTetrahedron(Figure &fig) {
    pair<int, int> face_indxs_dims(4,3);
    int pointArr[4][3] = {
            {1,-1,-1},
            {-1,1,-1},
            {1,1,1},
            {-1,-1,1}

    };

    int face_indexes[4][3] = {
            {1, 2, 3},
            {2, 4, 3},
            {1, 4, 2},
            {1, 3, 4}
    };

    vector<Vector3D> points;
    for (auto p : pointArr) {
        Vector3D point = Vector3D::point(p[0], p[1], p[2]);
        points.push_back(point);
    }
    fig.setPoints(points);

    vector<Face> faces;
    for (auto f : face_indexes) {
        Face face;
        vector<int> indexes;
        indexes.reserve(sizeof(f));
        for (int i=0; i < face_indxs_dims.second; i++) {
            indexes.push_back(f[i]-1);  // -1 because index start at 0
        }
        face.point_indexes = indexes;
        faces.push_back(face);
    }
    fig.setFaces(faces);
}

void Solid3D::generateOctahedron(Figure &fig) {
    pair<int, int> face_indxs_dims(8,3);
    int pointArr[6][3] = {
            {1,0,0},
            {0,1,0},
            {-1,0,0},
            {0,-1,0},
            {0,0,-1},
            {0,0,1},

    };

    int face_indexes[8][3] = {
            {1, 2, 6},
            {2, 3, 6},
            {3, 4, 6},
            {4, 1, 6},
            {2, 1, 5},
            {3, 2, 5},
            {4, 3, 5},
            {1, 4, 5},
    };

    vector<Vector3D> points;
    for (auto p : pointArr) {
        Vector3D point = Vector3D::point(p[0], p[1], p[2]);
        points.push_back(point);
    }
    fig.setPoints(points);

    vector<Face> faces;
    for (auto f : face_indexes) {
        Face face;
        vector<int> indexes;
        indexes.reserve(sizeof(f));
        for (int i=0; i < face_indxs_dims.second; i++) {
            indexes.push_back(f[i]-1);  // -1 because index start at 0
        }
        face.point_indexes = indexes;
        faces.push_back(face);
    }
    fig.setFaces(faces);
}
