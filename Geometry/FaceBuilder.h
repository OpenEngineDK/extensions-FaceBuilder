/*
 *  FaceBuilder.h
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 21/08/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include <Geometry/FaceSet.h>
#include <Geometry/Box.h>

using namespace OpenEngine::Geometry;

namespace OpenEngine {
namespace Geometry {



class FaceBuilder {
    
    
public:
    
    struct FaceState {
        Vector<4,float> color;    
        MaterialPtr mat;
    };
    
    static void MakeABox(FaceSet* fs, FaceState state, Vector<3,float> origin, Vector<3,float> size);    
    static void MakeABox(FaceSet* fs, list<FaceState> states, Vector<3,float> origin, Vector<3,float> size);    
    static void AddSquare(FaceSet* fs, FaceState state, Vector<3,float>,Vector<3,float>,Vector<3,float>,Vector<3,float>);
    static void AddTriangle(FaceSet* fs, FaceState state, Vector<3,float>,Vector<3,float>,Vector<3,float>);
    static void MakeASphere(FaceSet* fs, FaceState state, Vector<3,float> origin, float radius, int resolution);

private:
    FaceSet *fs;
    Vector<4,float> colr;
    Material* mat;
    
    FaceBuilder();
    void AddSquare(Vector<3,float>,Vector<3,float>,Vector<3,float>,Vector<3,float>);
    FaceSet* GetFaceSet();
};

}
}
