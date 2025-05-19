#include "faceexplosion.h"

#include <GA/GA_Attribute.h>
#include <GA/GA_GBMacros.h>
#include <GA/GA_Handle.h>
#include <GA/GA_Iterator.h>
#include <GA/GA_PrimitiveTypes.h>
#include <GA/GA_Types.h>
#include <GEO/GEO_Detail.h>
#include <GEO/GEO_Primitive.h>
#include <GEO/GEO_PrimPoly.h>
#include <GU/GU_Detail.h>
#include <OP/OP_Error.h>
#include <OP/OP_Network.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <OP/OP_AutoLockInputs.h>
#include <PRM/PRM_Name.h>
#include <PRM/PRM_Shared.h>
#include <PRM/PRM_Template.h>
#include <PRM/PRM_Type.h>
#include <SOP/SOP_Error.h>
#include <SOP/SOP_Node.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <UT/UT_BoundingBox.h>
#include <UT/UT_Error.h>
#include <UT/UT_Interrupt.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_Hash.h>

/// Register new SOP node definition with Houdini plugin interface.
void newSopOperator(OP_OperatorTable *table)
{
    OP_Operator *op = new OP_Operator(
        "FaceExplosion",
        "Face Explosion",
        SOP_FaceExplosion::create,
        SOP_FaceExplosion::params,
        1,
        1,
        nullptr
    );

    table->addOperator(op);
}

/// Default node templated parameters.
PRM_Template SOP_FaceExplosion::params[] = {
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
    PRM_Template(PRM_FLT_J, 1, &PRM_DIST_NAME, PRMzeroDefaults),
    PRM_Template(PRM_FLT_J, 1, &PRM_NOISE_NAME, PRMzeroDefaults),
    PRM_Template(PRM_ORD, 1, &PRM_NORMAL_NAME, PRMzeroDefaults, &PRM_NORMAL_MENU),
    PRM_Template(),
};

/// Default SOP node constructor.
SOP_FaceExplosion::SOP_FaceExplosion(OP_Network *network, const char *name, OP_Operator *op)
    : SOP_Node(network, name, op),
      input_points(nullptr)  
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
    OP_AutoLockInputs inputs(this);
    if (inputs.lock(context) >= UT_ERROR_ABORT)
    {
        return error();
    }

    /// Since transforming triangle along their normals will split them, we need a new set
    /// of point goemetry point and normal data.
    this->gdp->clearAndDestroy();

    /// Grab access to this mesh data and its normal attributes.
    const GU_Detail *input_mesh = inputGeo(0, context);
    UT_BoundingBox input_bbox;
    input_mesh->getBBox(&input_bbox);

    GA_ROHandleV3 input_pos_attrib = input_mesh->getP();
    GA_RWHandleV3 out_normal_attrib = GA_RWHandleV3(this->gdp->addFloatTuple(GA_ATTRIB_POINT, "N", 3));

    /// Fetch node parameter values.
    fpreal distance = this->evalFloat(PRM_DIST_NAME, 0, context.getTime());
    fpreal noise = this->evalFloat(PRM_NOISE_NAME, 0, context.getTime());
    int normal_mode = this->evalInt(PRM_NORMAL_NAME, 0, context.getTime());

    /// Loop through all input primitives and build new output mesh data.
    for (GA_Iterator it(input_mesh->getPrimitiveRange()); !it.atEnd(); ++it)
    {
        const GEO_Primitive *prim = input_mesh->getGEOPrimitive(*it);
        if (prim->getTypeDef().getId() != GA_PRIMPOLY)
        {
            continue;
        }

        /// Initialise new prim (face)
        GEO_Primitive *out_prim = this->gdp->appendPrimitive(GA_PRIMPOLY);
        GEO_PrimPoly *out_poly = static_cast<GEO_PrimPoly *>(out_prim);
        out_poly->setSize(prim->getVertexCount());

        /// Derive face normal vector.
        UT_Vector3 normal = UT_Vector3(0, 0, 0);
        switch (normal_mode)
        {
            case NORMAL_MODE_FACE:
                prim->evaluateNormalVector(normal, 0.5, 0.5);
                normal.normalize();
                break;
            case NORMAL_MODE_RADIAL:
                UT_Vector3 center = prim->baryCenter();
                normal = center - input_bbox.center();
                normal.normalize();
                break;
        }

        /// Noise.
        const fpreal max_noise = 2.0;
        uint seed = SYSpointerHash(prim);
        fpreal rand = SYSfastRandom(seed) * noise;

        /// Loop through input prim points and create new set thats transformed along
        /// the normal vector.
        for (GA_Size i=0; i<prim->getVertexCount(); ++i)
        {
            /// Fetch input point position.
            GA_Offset point_offset = prim->getPointOffset(i);
            UT_Vector3 point = input_pos_attrib.get(point_offset);

            /// Append new point position.
            GA_Offset out_point_offset = this->gdp->appendPointOffset();
            UT_Vector3 pos = point + (normal * (distance + rand * max_noise));

            this->gdp->setPos3(out_point_offset, pos);
            out_normal_attrib.set(out_point_offset, normal);

            /// Add point to face primitive.
            out_poly->setPointOffset(i, out_point_offset);
        }
        
        out_poly->close();
    }

    return error();
}
