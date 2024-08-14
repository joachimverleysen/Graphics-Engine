//
// Created by joachimverleysen on 3/27/24.
//

#include "Solid3D.h"
#include "Transformations.h"
#include "math.h"

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


void Solid3D::generateIcosahedron(Figure &fig) {
    pair<int, int> face_indxs_dims(20,3);
    double pointArr[12][3];
    for (int i=1; i<13; i++) {
        double x=0; double y=0; double z=0;
        if (i==1) {
            x=0;
            y=0;
            z= sqrt(5)/2;
        }
        else if (i>1 && i<7) {
            x = cos((i-2)*2*M_PI/5);
            y = sin((i-2)*2*M_PI/5);
            z = 0.5;
        }
        else if (i>6 && i<12) {
            x = cos(M_PI/5+(i-7)*2*M_PI/5);
            y = sin(M_PI/5+(i-7)*2*M_PI/5);
            z = -0.5;
        }
        else if (i==12) {
            x = 0;
            y = 0;
            z = -sqrt(5)/2;
        }
        pointArr[i-1][0] = x;
        pointArr[i-1][1] = y;
        pointArr[i-1][2] = z;
    }
    int face_indexes[20][3] = {
            {1, 2, 3},
            {1, 3, 4},
            {1, 4, 5},
            {1, 5, 6},
            {1, 6, 2},
            {2, 7, 3},
            {3, 7, 8},
            {3, 8, 4},
            {4, 8, 9},
            {4, 9, 5},
            {5, 9, 10},
            {5, 10, 6},
            {6, 10, 11},
            {6, 11,2},
            {2, 11, 7},
            {12, 8, 7},
            {12, 9, 8},
            {12, 10, 9},
            {12, 11, 10},
            {12, 7, 11}
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

Vector3D computeMiddlePoint(Vector3D &p1, Vector3D &p2, Vector3D &p3) {
    Vector3D result;
    double x; double y; double z;
    x = (p1.x+p2.x+p3.x)/3;
    y = (p1.y+p2.y+p3.y)/3;
    z = (p1.z+p2.z+p3.z)/3;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;

}

void Solid3D::generateDodecahedron(Figure &fig) {
    pair<int, int> face_indxs_dims(12,5);
    generateIcosahedron(fig);
    vector<Vector3D> points;
    for (auto f : fig.getFaces()) {
            if (f.point_indexes.size() != 3){
                cerr<<"error\n";
                continue;
            }
            Vector3D p1 = fig.getPoints()[f.point_indexes[0]];
            Vector3D p2 = fig.getPoints()[f.point_indexes[1]];
            Vector3D p3 = fig.getPoints()[f.point_indexes[2]];
            Vector3D middle = computeMiddlePoint(p1, p2, p3);
            points.push_back(middle);
    }

    int face_indexes[12][5] = {
            {1, 2, 3, 4, 5},
            {1, 6, 7, 8, 2},
            {2, 8, 9, 10, 3},
            {3, 10, 11, 12, 4},
            {4, 12, 13, 14, 5},
            {5, 14, 15, 6, 1},
            {20, 19, 18, 17, 16},
            {20, 15, 14, 13, 19},
            {19, 13, 12, 11, 18},
            {18, 11, 10, 9, 17},
            {17, 9, 8, 7, 16},
            {16, 7, 6, 15, 20}
    };

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

    fig.setPoints(points);
    fig.setFaces(faces);
}

// Returns 3 points
vector<Vector3D> divideTriangle(Vector3D& A, Vector3D& B, Vector3D& C) {
    Vector3D D = (A+B)/2;
    Vector3D E = (A+C)/2;
    Vector3D F = (B+C)/2;
    return {D, E, F};
};

void Solid3D::generateSphere(Figure &fig, const int n) {
    generateIcosahedron(fig);
    vector<Vector3D> points=fig.getPoints();
    vector<Face> faces=fig.getFaces();
    int p_count=0;
    vector<Face> _faces;
    vector<Vector3D> _points;

    // Divide every triangle in smaller triangles
    for (int i=0; i<n; i++) {
        _faces.clear();
        _points.clear();
        for (auto & face : faces) {
            vector<int> indexes = face.point_indexes;

            if (indexes.size() != 3) {
                cerr<<"error\n";
                continue;
            }
            Vector3D A = points[indexes[0]];
            Vector3D B = points[indexes[1]];
            Vector3D C = points[indexes[2]];
            vector<Vector3D> trianglePoints = divideTriangle(A, B, C);
            Vector3D D = trianglePoints[0];
            Vector3D E = trianglePoints[1];
            Vector3D F = trianglePoints[2];
            _points.push_back(A); _points.push_back(D); _points.push_back(E);
            _points.push_back(B); _points.push_back(F); _points.push_back(D);
            _points.push_back(C); _points.push_back(E); _points.push_back(F);
            _points.push_back(D); _points.push_back(E); _points.push_back(F);

            _faces.push_back(Face({p_count, p_count+1, p_count+2}));   // triangle ADE
            p_count+=3;
            _faces.push_back(Face({p_count, p_count+1, p_count+2}));   // triangle BFD
            p_count+=3;
            _faces.push_back(Face({p_count, p_count+1, p_count+2}));   // triangle CEF
            p_count+=3;
            _faces.push_back(Face({p_count, p_count+1, p_count+2}));   // triangle CEF
            p_count+=3;



        }
        faces = _faces;
        points = _points;
        p_count=0;

    }
    for (Vector3D& p : points) {
        p.normalise();
    }
    fig.setPoints(points);
    fig.setFaces(faces);
}

void Solid3D::generateCone(Figure &fig, const int n, const double h) {
    vector<Vector3D> points;
    vector<Face> faces;

    for (int i=0; i<n; i++) {
        double x = cos(2*i*M_PI/n);
        double y = sin(2*i*M_PI/n);
        double z = 0;
        Vector3D p = Vector3D::point(x, y, z);
        points.push_back(p);
    }
    points.push_back(Vector3D::point(0,0,h));   // top point

    for (int i=0; i<n; i++) {
        int i1 = i;
        int i2 = (i+1)%n;
        int i3 = n;
        faces.push_back(Face({i1, i2, i3}));
    }
    vector<int> lastFace;
    for (int i=n-1; i<0; i--) {
        lastFace.push_back(i);
    }
    faces.push_back(Face(lastFace));
    fig.setFaces(faces);
    fig.setPoints(points);

}

void Solid3D::generateCylinder(Figure &fig, const int n, const double h) {
    vector<Vector3D> points;
    vector<Face> faces;
    // Base points
    for (int i=0; i<n; i++) {
        double x = cos(2*i*M_PI/n);
        double y = sin(2*i*M_PI/n);
        double z = 0;
        Vector3D p = Vector3D::point(x, y, z);
        points.push_back(p);
    }
    // Top points
    for (int i=0; i<n; i++) {
        double x = cos(2*i*M_PI/n);
        double y = sin(2*i*M_PI/n);
        double z = h;
        Vector3D p = Vector3D::point(x, y, z);
        points.push_back(p);
    }
    //Faces
    for (int i = 0; i < n; i++) {
        int i1 = i;
        int i2 = (i + 1) % n;
        int i3 = n + (i + 1) % n;
        int i4 = n + i;
        faces.push_back(Face({i1, i4, i3, i2}));
    }
//    faces.push_back(Face({0, n, 2*n-1, n-1}));
    //Base+top face

    vector<int> baseFace;
    vector<int> topFace;
    for (int i=0; i<n; i++) {
        baseFace.push_back(i);
        topFace.push_back(n+i);
    }
    faces.push_back(Face(baseFace));
    faces.push_back(Face(topFace));
    fig.setPoints(points);
    fig.setFaces(faces);

}

int getPointIndex(int i, int j, int m) {
    return i*m+j;
}

void Solid3D::generateTorus(Figure &fig, const double r, const double R, const int n, const int m) {
    vector<Vector3D> points;
    vector<Face> faces;
    double u; double v;
    double x; double y; double z;




    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            u = 2*i*M_PI/n;
            v = 2*j*M_PI/n;
            x=(R+r*cos(v))*cos(u);
            y=(R+r*cos(v))*sin(u);
            z=r*sin(v);
            points.push_back(Vector3D::point(x, y, z));
        }
    }

    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            int i1 = getPointIndex(i, j, m);
            int i2 = getPointIndex((i+1)%n, j, m);
            int i3 = getPointIndex((i+1)%n, (j+1)%m, m);
            int i4 = getPointIndex(i, (j+1)%m, m);
            faces.push_back(Face({i1, i2, i3, i4}));
        }
    }

    fig.setFaces(faces);
    fig.setPoints(points);
}

void Solid3D::generateFractal(Figure &fig, Figures3D &fractal, const int nr_iterations, const double scale) {
    MyTools mt;

    int count = nr_iterations;
    fractal.push_back(fig);
    while (count != 0) {
        auto prev_fractal = fractal;
        fractal.clear();
        for (auto fig : prev_fractal) {
            Figure original = fig;
            Matrix scale_mtx = Transformations::get_scale_mtx(1/scale);
            for (int i=0; i!=original.getPoints().size(); i++){
                Figure fig_copy = fig;
                mt.applyTransformation(fig_copy, scale_mtx);
                Vector3D point_new = fig_copy.getPoints()[i];
                Vector3D trans = original.getPoints()[i] - point_new    ;
                auto trans_mtx = Transformations::get_translate_mtx(trans);
                mt.applyTransformation(fig_copy, trans_mtx);
                fractal.push_back(fig_copy);
            }
        }
        count --;

    }

}
