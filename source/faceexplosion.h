#ifndef FACE_EXPLOSION_H
#define FACE_EXPLOSION_H

#include <PRM/PRM_Name.h>
#include <PRM/PRM_Type.h>
#include <SOP/SOP_Node.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_DSOVersion.h>
#include <PRM/PRM_Include.h>


static PRM_Name PRM_DIST_NAME("distance", "Distance");
static PRM_Name PRM_NOISE_NAME("noise", "Noise");
static PRM_Name PRM_NORMAL_NAME("normal_mode", "Normal Mode");
static PRM_Name PRM_NORMAL_OPTIONS[] = {
    PRM_Name("face_normal", "Face Normal"),
    PRM_Name("radial_normal", "Radial Normal"),
    PRM_Name(0)
};

constexpr uint8 NORMAL_MODE_FACE = 0;
constexpr uint8 NORMAL_MODE_RADIAL = 1;
static PRM_ChoiceList PRM_NORMAL_MENU(PRM_CHOICELIST_SINGLE, PRM_NORMAL_OPTIONS);

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
