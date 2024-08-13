#include "../tools/easy_image.h"
#include "../tools/ini_configuration.h"
#include "LineDrawer.h"
#include "MyLSystem2D.h"
#include "../tools/vector3d.h"
#include "Figure.h"
#include "Drawing3D.h"
#include "Solid3D.h"
#include "MyParser.h"
#include "MyTools.h"
#include "MyLSystem3D.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>

using namespace std;

enum class drawingType {LSystem2D, Wireframe, LSystem3D};



void doTransitions(Drawing3D &drawing) {
    MyTools mt;
    Figures3D figs = drawing.getFigures();
    for (auto& fig : figs) {
        mt.doTransitions(fig);
    }
    drawing.setFigures(figs);
}

Lines2D do_projection(Figures3D& figures, Vector3D& eye) {
    MyTools mt;
    Lines2D result;
    Figures3D figs = figures;
    for (Figure &fig : figs) {
        mt.convert_fig_to_eyesys(fig, eye);
        vector<Point2D> proj_points = mt.compute_proj_points(fig);
        fig.setProjPoints(proj_points);
//        vector<Face> faces = fig.getFaces();
        Lines2D  lines;
        if (fig.getType() == "3DLsystem") {
            lines = mt.getLineArray2D(fig.getProjPoints(), fig.getColor());
        }
        else lines = mt.computeLines(fig);
        result.insert(result.end(), lines.begin(), lines.end());
    }
    return result;
}
//todo: move extra functions to seperate file

/**
@brief draws all the figures in the drawing, but with color fill instead of wireframe
\n Makes use of z-buffer
 */

img::EasyImage zbuffDrawing(Drawing3D &drawing, bool lighted) {
    MyTools mt;
    LineDrawer ld;
    Vector3D eye = drawing.getEye();
    Color bgColor = drawing.getBgColor();
    Figures3D figs = drawing.getFigures();

    for (auto& f : figs) {
        mt.triangulateFigure(f);
    }

    Lines2D lines = do_projection(figs, eye);
    int size = drawing.getSize();

    Dimensions dim = ld.computeDims(lines, size);


    img::EasyImage image(dim.width, dim.height);





    ZBuffer zbuffer(dim.width, dim.height);

    auto lights = drawing.getLights();

    // Convert direction to eyesystem
    for (Light& l : lights) {
        Vector3D direction = Vector3D::point(0,0,0);
        direction.x = l.direction.x;
        direction.y = l.direction.y;
        direction.z = l.direction.z;
        mt.convert_point_to_eyesys(direction, eye);
        l.direction.x = direction.x;
        l.direction.y = direction.y;
        l.direction.z = direction.z;
    }

    for (auto& f : figs) {
        mt.convert_fig_to_eyesys(f, eye);
        Color color = f.getColor();
        for (auto& face : f.getFaces()) {
            if (face.point_indexes.size() != 3) {
                cerr<<"Error - Face is not a triangle\n";
                exit(1);
            }
            Vector3D p1 = f.getPoints()[face.point_indexes[0]];
            Vector3D p2 = f.getPoints()[face.point_indexes[1]];
            Vector3D p3 = f.getPoints()[face.point_indexes[2]];

            Color ambientReflec = f.getAmbientReflection();
            Color diffuseReflec = f.getDiffuseReflection();

            if (lighted)
                ld.drawZbuffTriangLighted(zbuffer, image, p1, p2, p3, dim.d, dim.dx, dim.dy, color, ambientReflec,
                                          diffuseReflec, 0,
                                          lights);
            else
                ld.drawZbuffTriang(zbuffer, image, p1, p2, p3, dim.d, dim.dx, dim.dy, color);


        }
    }
    return image;
}

Drawing3D getSingleTriangleDrawing() {
    Drawing3D result;
    Vector3D a = Vector3D::point(1, 1, 1);
    Vector3D b = Vector3D::point(1, -1, 1);
    Vector3D c = Vector3D::point(-1, 1, 1);
    vector<Vector3D> points;
    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
    Figure testfig;
    testfig.setColor(Color(1,0,0));
    testfig.setPoints(points);
    Face f({0,1,2});
    vector<Face> faces; faces.push_back(f);
    testfig.setFaces(faces);
//    mt.convert_point_to_eyesys(testfig, eye);
    Figures3D figs; figs.push_back(testfig);
    result.setFigures(figs);
    Vector3D eye = Vector3D::point(100, 50, 75);
    result.setEye(eye);
    result.setSize(1000);
    return result;
}



img::EasyImage generateImage(const ini::Configuration &conf) {
    img::EasyImage image;
    MyParser parser;
    LineDrawer ld;
    MyTools mt;
    Drawing3D drawing;
    parser.drawing3D_parse(conf, drawing);
    string type = drawing.getType();

    if (type=="ZBufferedWireframe") {
        Vector3D eye = drawing.getEye();
        Color bgColor = drawing.getBgColor();
        doTransitions(drawing);
        Figures3D figs = drawing.getFigures();
        Lines2D lines = do_projection(figs, eye);
        image = ld.draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type=="ZBuffering") {
        doTransitions(drawing);
        image = zbuffDrawing(drawing, false);
    }
    else if (type=="Wireframe") {
        Vector3D eye = drawing.getEye();
        Color bgColor = drawing.getBgColor();
        doTransitions(drawing);
        Figures3D figs = drawing.getFigures();
        Lines2D lines = do_projection(figs, eye);
        image = ld.draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type == "2DLSystem") {
        MyLSystem2D ls = parser.lsys_parse(conf);
        Color bgColor = ls.getBgColor();
        vector<Point2D> points;
        ls.computePoints(points);
        ls.setPoints(points);
        Lines2D lines = mt.getLineArray2D(points, ls.getColor());
        image = ld.draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type == "LightedZBuffering") {
        doTransitions(drawing);
        image = zbuffDrawing(drawing, true);
    }






//    image = ld.draw2Dlines(lines, drawing.getSize(), bgcolor);
    std::ofstream fout("../out.bmp", std::ios::binary);
    fout << image;
    fout.close();
    return image;
}



int main(int argc, char const* argv[])
{
    int retVal = 0;
    try
    {
        std::vector<std::string> args = std::vector<std::string>(argv+1, argv+argc);
        if (args.empty()) {
            std::ifstream fileIn("filelist");
            std::string filelistName;
            while (std::getline(fileIn, filelistName)) {
                args.push_back(filelistName);
            }
        }
        for(std::string fileName : args)
        {
            ini::Configuration conf;
            try
            {
                std::ifstream fin(fileName);
                if (fin.peek() == std::istream::traits_type::eof()) {
                    std::cout << "Ini file appears empty. Does '" <<
                              fileName << "' exist?" << std::endl;
                    continue;
                }
                fin >> conf;
                fin.close();
            }
            catch(ini::ParseException& ex)
            {
                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                retVal = 1;
                continue;
            }

            img::EasyImage image = generateImage(conf);
            if(image.get_height() > 0 && image.get_width() > 0)
            {
                std::string::size_type pos = fileName.rfind('.');
                if(pos == std::string::npos)
                {
                    //filename does not contain a '.' --> append a '.bmp' suffix
                    fileName += ".bmp";
                }
                else
                {
                    fileName = fileName.substr(0,pos) + ".bmp";
                }
                try
                {
                    std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                    f_out << image;

                }
                catch(std::exception& ex)
                {
                    std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                    retVal = 1;
                }
            }
            else
            {
                std::cout << "Could not generate image for " << fileName << std::endl;
            }
        }
    }
    catch(const std::bad_alloc &exception)
    {
        //When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
        //Basically this return value tells our automated drawLine2D scripts to run your engine on a pc with more memory.
        //(Unless of course you are already consuming the maximum allowed amount of memory)
        //If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
        //mark the drawLine2D as failed while in reality it just needed a bit more memory
        std::cerr << "Error: insufficient memory" << std::endl;
        retVal = 100;
    }
    return retVal;
}
