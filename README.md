# Graphics Engine
This was a university project during first bachelor of Computer Science (2023-2024) at UAntwerpen within the "Computer Graphics" course.

## Functionality (to be updated):
- 2D line drawings
- Lindenmayer systems (L-systems)
- 3D line drawings

## How to use:
The engine will parse an .ini file that is given as parameter in the run configurations.
The generateImage() function is the function that should be modified, depending on what you want to do.

### L-system example:

    img::EasyImage generateImage(const ini::Configuration &conf) {
        LSystem ls = lsys_parse(conf);
        vector<Point2D> points;
        ls.computePoints(points);
            ls.setPoints(points);
            Lines2D myLines = lineDrawer._getLineArray(points, ls.color);
            img::EasyImage image;
            image = lineDrawer.draw2Dlines(myLines, ls.size);
            return image;
    };

### 3D drawing example:

    img::EasyImage generateImage(const ini::Configuration &conf) {
        img::EasyImage image;
        Figures3D figs;
        Drawing3D drawing;
        LineDrawer ld;
        drawing3D_parse(conf, drawing);
        Lines2D lines = do_projection(drawing);
        Color bgCol = drawing.getBgColor();
        image = ld.draw2Dlines(lines, drawing.getSize(), bgCol);
    
        return image;
    }

