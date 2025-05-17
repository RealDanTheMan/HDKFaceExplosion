#include "faceexplosion.h"

#include <OP/OP_Network.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <SOP/SOP_Node.h>

/// Register new SOP node definition with Houdini plugin interface.
void newSopOperator(OP_OperatorTable *table)
{
    OP_Operator *op = new OP_Operator(
        "FaceExplosion",
        "Face Explosion",
        SOP_FaceExplosion::create,
        SOP_FaceExplosion::template_list,
        1,
        1,
        nullptr
    );

    table->addOperator(op);
}

/// Default node templated parameters.
PRM_Template SOP_FaceExplosion::template_list[] = {
    PRM_Template(
        PRM_STRING,
        1,
        &PRMgroupName,
        0,
        &SOP_Node::pointGroupMenu,
		0,
        0,
        SOP_Node::getGroupSelectButton(GA_GROUP_POINT)
    ),
    PRM_Template(PRM_ORD, 1, &PRMorientName, 0, &PRMplaneMenu),
    PRM_Template(PRM_DIRECTION, 3, &PRMdirectionName, PRMzaxisDefaults),
    PRM_Template(),
};

/// Default SOP node constructor.
SOP_FaceExplosion::SOP_FaceExplosion(OP_Network *network, const char *name, OP_Operator *op)
    : SOP_Node(network, name, op)
{

}

/// Default SOP node destructor.
SOP_FaceExplosion::~SOP_FaceExplosion()
{

}

/// Static constructor.
OP_Node* SOP_FaceExplosion::create(OP_Network *network, const char *name, OP_Operator *op)
{
    return new SOP_FaceExplosion(network, name, op);
}

/// Recook this node data and outputs.
OP_ERROR SOP_FaceExplosion::cookMySop(OP_Context &context)
{
    return error();
}
