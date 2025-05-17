#ifndef FACE_EXPLOSION_H
#define FACE_EXPLOSION_H

#include <SOP/SOP_Node.h>
#include <UT/UT_DSOVersion.h>


class SOP_FaceExplosion : public SOP_Node
{

public:
    static OP_Node* create(OP_Network *network, const char *name, OP_Operator *op);
    static PRM_Template template_list[];

    SOP_FaceExplosion(OP_Network *network, const char *name, OP_Operator *op);
    ~SOP_FaceExplosion() override;
    virtual OP_ERROR cookMySop(OP_Context &context) override;
};

#endif
