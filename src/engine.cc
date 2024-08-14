#include "tools/easy_image.h"
#include "tools/ini_configuration.h"
#include "LineDrawer.h"
#include "MyLSystem2D.h"
#include "tools/vector3d.h"
#include "Figure.h"
#include "Drawing3D.h"
#include "Solid3D.h"
#include "MyParser.h"
#include "MyTools.h"
#include "MyLSystem3D.h"
#include "Transformations.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath>


//todo: move extra functions to seperate file
//todo: logging
//todo: change project structure, classes...
//todo: more efficient code
//todo: error handling
//todo: remove using namespace std

using namespace std;


//void doTransitions(Drawing3D &drawing) {
//    MyTools mt;
//    Figures3D figs = drawing.getFigures();
//    for (auto& fig : figs) {
//        mt.doTransitions(fig);
//    }
//    drawing.setFigures(figs);
//}

Lines2D do_projection(Figures3D& figures, Vector3D& eye) {
    MyTools mt;
    Lines2D result;
    Figures3D figs = figures;
    for (Figure &fig : figs) {
        Transformations::to_eye_system(fig, eye);
        vector<Point2D> proj_points = Transformations::compute_projected_points(fig);
        fig.setProjPoints(proj_points);
        Lines2D  lines;
        if (fig.getType() == "3DLsystem") {
            lines = mt.getLineArray2D(fig.getProjPoints(), fig.getColor());
        }
        else lines = mt.computeLines(fig);
        result.insert(result.end(), lines.begin(), lines.end());
    }
    return result;
}

/**
@brief draws all the figures in the drawing, but with color fill instead of wireframe
\n Makes use of z-buffer
 */

img::EasyImage zbuffDrawing(Drawing3D &drawing) {
    // do transformations

    LineDrawer ld;
    Vector3D eye = drawing.getEye();
    Figures3D figs = drawing.getFigures();
    Transformations::apply_transformations(figs);
    Transformations::to_eye_system(figs, drawing.getEye());

    for (auto& f : figs) {
        MyTools::triangulateFigure(f);
    }

    Lines2D lines = do_projection(figs, eye);
    int size = drawing.getSize();

    Dimensions dim = ld.computeDims(lines, size);

    img::EasyImage image(dim.width, dim.height);

    ZBuffer zbuffer(dim.width, dim.height);

    auto lights = drawing.getLights();

    // Convert direction to eyesystem
    //todo: optimize
    for (Light& l : lights) {
        Vector3D direction = Vector3D::point(0,0,0);
        direction.x = l.direction.x;
        direction.y = l.direction.y;
        direction.z = l.direction.z;
        Transformations::to_eye_system(direction, eye);
        l.direction.x = direction.x;
        l.direction.y = direction.y;
        l.direction.z = direction.z;
    }
//    Transformations::to_eye_system(figs, eye);

    for (auto& f : figs) {
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

                ld.drawZbuffTriang(zbuffer, image, p1, p2, p3, dim.d, dim.dx, dim.dy, color, ambientReflec,
                                          lights);
        }
    }
    return image;
}


img::EasyImage generateImage(const ini::Configuration &conf) {
    img::EasyImage image;
    MyParser parser;
    LineDrawer ld;
    MyTools mt;
    Drawing3D drawing;
    SuccessEnum success_enum = parser.drawing3D_parse(conf, drawing);
    if (success_enum!=success) {
        return image;
    }
    string type = drawing.getType();

    //todo: move code to seperate files
    if (type=="ZBufferedWireframe") {
        Vector3D eye = drawing.getEye();
        Color bgColor = drawing.getBgColor();
        Figures3D figs = drawing.getFigures();
        Transformations::apply_transformations(figs);
        Transformations::to_eye_system(figs, drawing.getEye());
        Lines2D lines = do_projection(figs, eye);
        image = ld.draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type=="ZBuffering") {
        image = zbuffDrawing(drawing);
    }
    else if (type=="Wireframe") {
        Vector3D eye = drawing.getEye();
        Color bgColor = drawing.getBgColor();
        Figures3D figs = drawing.getFigures();
        Transformations::apply_transformations(figs);
        Transformations::to_eye_system(figs, drawing.getEye());

        Lines2D lines = do_projection(figs, eye);
        image = ld.draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type == "2DLSystem") {
        MyLSystem2D ls = parser.parse_Lsystem2D(conf);
        Color bgColor = ls.getBgColor();
        vector<Point2D> points;
        ls.computePoints(points);
        ls.setPoints(points);
        Lines2D lines = mt.getLineArray2D(points, ls.getColor());
        image = ld.draw2Dlines(lines, drawing.getSize(), bgColor);
    }
    else if (type == "LightedZBuffering") {
        image = zbuffDrawing(drawing);
    }






//    image = ld.draw2Dlines(lines, drawing.getSize(), bgcolor);
    std::ofstream fout("../../out.bmp", std::ios::binary);
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
