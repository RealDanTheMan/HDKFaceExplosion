#ifndef FACE_EXPLOSION_H
#define FACE_EXPLOSION_H

#include <PRM/PRM_Name.h>
#include <SOP/SOP_Node.h>
#include <UT/UT_DSOVersion.h>


static PRM_Name PRM_DIST_NAME("distance", "Distance");
static PRM_Name PRM_NOISE_NAME("noise", "Noise");

class SOP_FaceExplosion : public SOP_Node
{

public:
    static OP_Node* create(OP_Network *network, const char *name, OP_Operator *op);
    static PRM_Template params[];

    SOP_FaceExplosion(OP_Network *network, const char *name, OP_Operator *op);
    ~SOP_FaceExplosion() override;
    virtual OP_ERROR cookMySop(OP_Context &context) override;

private:
    const GA_PointGroup *input_points;
};

#endif
