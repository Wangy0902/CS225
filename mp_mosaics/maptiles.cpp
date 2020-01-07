/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
     // the canvas to be returned
    MosaicCanvas* myCanvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    // the vector of points to build the kdtree
    vector<Point<3>> myPoints;

    // the map of cooresponding points and pointer of tileimage
    map<Point<3>, TileImage*> myMap;

    // put everything in to the vector of points
    for (unsigned i = 0; i < theTiles.size(); i++) {
      // find the point of every tile image using given function
    	Point<3> p = convertToXYZ(theTiles[i].getAverageColor());
    	myPoints.push_back(p);
      // use points as index of the map, cooresponding values are tile images
    	myMap[p] = &theTiles[i];
    }

    // construct the KDtree with the vector of points
     KDTree<3> myTree(myPoints);

    // go through every region of the canves
    for (int x = 0; x < myCanvas->getRows(); x++) {
    	for (int y = 0; y < myCanvas->getColumns(); y++) {
        // the color we want to fill in this region
    		Point<3> wanted = convertToXYZ(theSource.getRegionColor(x, y));
        // the cloest we can get from the given points
    		Point<3> nearest = myTree.findNearestNeighbor(wanted);
        // find the cooresponding tile image from the map
    		TileImage* bestFit = myMap[nearest];
    		myCanvas->setTile(x, y, bestFit); // draw on the canvas
    	}
    }

    return myCanvas;
}
