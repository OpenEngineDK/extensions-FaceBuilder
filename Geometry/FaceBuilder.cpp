/*
 *  FaceBuilder.cpp
 *  OpenEngine
 *
 *  Created by Peter Kristensen on 21/08/09.
 *  Copyright 2009 Lucky Software. All rights reserved.
 *
 */

#include "FaceBuilder.h"
#include <Logging/Logger.h>

namespace OpenEngine {
namespace Geometry {


FaceBuilder::FaceBuilder() {
    fs = new FaceSet();
    mat = new Material();
}


void FaceBuilder::AddSquare(Vector<3,float> a, Vector<3,float> b,
                            Vector<3,float> c, Vector<3,float> d) {

    Face *f1 = new Face(a,b,c);
    
    Vector<3,float> norm = (c-a) % (b-a);
    norm.Normalize();
    
    f1->norm[0] = norm;
    f1->norm[1] = norm;
    f1->norm[2] = norm;
    
    f1->colr[0] = colr;
    f1->colr[1] = colr;
    f1->colr[2] = colr;
    f1->mat = MaterialPtr(mat);
    
    fs->Add(FacePtr(f1));
    
    Face *f2 = new Face(a,c,d);
    f2->colr[0] = colr;
    f2->colr[1] = colr;
    f2->colr[2] = colr;
    
    f2->norm[0] = norm;
    f2->norm[1] = norm;
    f2->norm[2] = norm;
    
    f2->mat = MaterialPtr(mat);
    
    fs->Add(FacePtr(f2));
    
}

void FaceBuilder::AddSquare(FaceSet* fs, FaceState state,
                            Vector<3,float> a, Vector<3,float> b,
                            Vector<3,float> c, Vector<3,float> d) {
    
    Face *f1 = new Face(a,b,c);
    
    Vector<3,float> norm = (c-a) % (b-a);
    norm.Normalize();
    
    f1->norm[0] = norm;
    f1->norm[1] = norm;
    f1->norm[2] = norm;

    f1->texc[0] = Vector<2,float>(0,0);
    f1->texc[1] = Vector<2,float>(0,1);
    f1->texc[2] = Vector<2,float>(1,1);
    
    f1->colr[0] = state.color;
    f1->colr[1] = state.color;
    f1->colr[2] = state.color;
    f1->mat = MaterialPtr(state.mat);
    
    fs->Add(FacePtr(f1));
    
    Face *f2 = new Face(a,c,d);
    f2->colr[0] = state.color;
    f2->colr[1] = state.color;
    f2->colr[2] = state.color;
    
    f2->norm[0] = norm;
    f2->norm[1] = norm;
    f2->norm[2] = norm;


    f2->texc[0] = Vector<2,float>(0,0);
    f2->texc[1] = Vector<2,float>(1,1);
    f2->texc[2] = Vector<2,float>(1,0);
    
    f2->mat = MaterialPtr(state.mat);

    
    fs->Add(FacePtr(f2));
    
}

void FaceBuilder::AddTriangle(FaceSet* fs, FaceState state, Vector<3,float> v1, Vector<3,float> v2, Vector<3,float> v3) {

    Face *f1 = new Face(v1,v2,v3);
    
    Vector<3,float> norm = (v3-v1) % (v2-v1);
    norm.Normalize();

    f1->norm[0] = f1->norm[1] = f1->norm[2] = norm;
    f1->colr[0] = f1->colr[1] = f1->colr[2] = state.color;
    
    fs->Add(FacePtr(f1));

}

FaceSet* FaceBuilder::GetFaceSet() {
    return fs;
}

void FaceBuilder::MakeASphere(FaceSet* fs, FaceState state, Vector<3,float> origin, float radius, int resolution) {
    
    // x = x_0 + r * sin ( theta ) * cos ( phi )
    // y = y_0 + r * sin ( theta ) * sin ( phi )
    // z = z_0 + r * cos ( theta ) 

    float fRes = float(resolution);

    float dPhi = 2*PI/fRes;
    float dTheta = PI/fRes;

    for (int i=0;i<2*resolution;i++) {
        float phi = PI*(i/fRes)+dPhi;

        for (int j=0;j<resolution;j++) {            
            
            float theta = PI*(j/fRes)+dTheta;
            logger.info << phi << " " << theta << logger.end;
            if (abs(theta - PI) < 0.0001)
                continue;

            Vector<3,float> a(origin[0] + radius * sin(theta) * cos(phi),
                              origin[1] + radius * sin(theta) * sin(phi),
                              origin[2] + radius * cos(theta));
            
            Vector<3,float> b(origin[0] + radius * sin(theta) * cos(phi+dPhi),
                              origin[1] + radius * sin(theta) * sin(phi+dPhi),
                              origin[2] + radius * cos(theta));

            Vector<3,float> c(origin[0] + radius * sin(theta+dTheta) * cos(phi),
                              origin[1] + radius * sin(theta+dTheta) * sin(phi),
                              origin[2] + radius * cos(theta+dTheta));
           
            Vector<3,float> d(origin[0] + radius * sin(theta+dTheta) * cos(phi+dPhi),
                              origin[1] + radius * sin(theta+dTheta) * sin(phi+dPhi),
                              origin[2] + radius * cos(theta+dTheta));

            AddTriangle(fs, state, a,b,c);
            AddTriangle(fs, state, b,d,c);
            
            
        }
    }

}

void FaceBuilder::MakeABox(FaceSet* fs, FaceState state, Vector<3,float> origin, Vector<3,float> size) {
    list<FaceState> states;
    states.push_back(state);
    MakeABox(fs, states, origin, size);
}        

void FaceBuilder::MakeABox(FaceSet* fs, list<FaceState> states, Vector<3,float> origin, Vector<3,float> size) {
    
    float width = size.Get(0)/2;
    float height = size.Get(1)/2;
    float depth = size.Get(2)/2;

    
    float x = origin.Get(0);
    float y = origin.Get(1);
    float z = origin.Get(2);
    

    
    //fb.mat->specular = Vector<4,float>(.3,.3,.3,1);
    //fb.mat->shininess = 1;


    
    Vector<3,float> a = Vector<3,float>(x-width,y-height,z+depth);
    Vector<3,float> b = Vector<3,float>(x-width,y+height,z+depth);
    Vector<3,float> c = Vector<3,float>(x+width,y+height,z+depth);
    Vector<3,float> d = Vector<3,float>(x+width,y-height,z+depth);
    Vector<3,float> e = Vector<3,float>(x+width,y+height,z-depth);
    Vector<3,float> f = Vector<3,float>(x+width,y-height,z-depth);
    Vector<3,float> g = Vector<3,float>(x-width,y+height,z-depth);
    Vector<3,float> h = Vector<3,float>(x-width,y-height,z-depth);

    FaceState state;
    if (!states.empty()) {
        state = states.front();
        states.pop_front();
    }
    
    
    AddSquare(fs, state, a, b, c, d);
    
    if (!states.empty()) {
        state = states.front();
        states.pop_front();
    }
    AddSquare(fs, state, d, c, e, f);
    
    if (!states.empty()) {
        state = states.front();
        states.pop_front();
    }
    
    AddSquare(fs, state, f, e, g, h);
    
    if (!states.empty()) {
        state = states.front();
        states.pop_front();
    }
    AddSquare(fs, state, h, g, b, a);
    
    if (!states.empty()) {
        state = states.front();
        states.pop_front();
    }
    
    AddSquare(fs, state, c, b, g, e);
    
    if (!states.empty()) {
        state = states.front();
        states.pop_front();
    }
    
}

    
}
}
