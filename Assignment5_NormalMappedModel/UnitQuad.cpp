#include "UnitQuad.h"

UnitQuad::UnitQuad() : lightPos_(0.5f, 0.5f, -2.0f), sign_(1.0f)
{}

UnitQuad::~UnitQuad()
{}

void UnitQuad::init(const QString& textureFile, const QString& normalFile)
{ 
	// The unit quad goes from 0.0 to 1.0 in each dimension.
	QVector<QVector3D> pos;
	QVector<QVector3D> norm;
	QVector<QVector2D> texCoord;
    QVector<QVector3D> tangents;
    QVector<QVector3D> bitangents;
    QVector<unsigned int> idx;

  
    // unit positions.
    QVector3D pos1(0.0, 0.0, 0.0);
    QVector3D pos2(1.0, 0.0, 0.0);
    QVector3D pos3(0.0, 1.0, 0.0);
    QVector3D pos4(1.0, 1.0, 0.0);
    pos << pos1;
    pos << pos2;
    pos << pos3;
    pos << pos4;

    // We use normals for shading and lighting
    QVector3D norm1(0.0, 0.0, 1.0);
    QVector3D norm2(0.0, 0.0, 1.0);
    QVector3D norm3(0.0, 0.0, 1.0);
    QVector3D norm4(0.0, 0.0, 1.0);
    norm << norm1;
    norm << norm2;
    norm << norm3;
    norm << norm4;

    // Add in the texcoords
    QVector2D uv1(0.0, 0.0);
    QVector2D uv2(1.0, 0.0);
    QVector2D uv3(0.0, 1.0);
    QVector2D uv4(1.0, 1.0);
    texCoord << uv1;
    texCoord << uv2;
    texCoord << uv3;
    texCoord << uv4;

    //triangle indices triangle 1 = (0,1,2) triangle 2 = (2,1,3)
    idx << 0 << 1 << 2 << 2 << 1 << 3;


    //first triangle's edges and delta tex coords
    QVector3D edge1 = pos2 - pos1;
    QVector3D edge2 = pos3 - pos1;
    QVector2D deltaUV1 = uv2 - uv1;
    QVector2D deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    QVector3D tangent1((f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x())), (f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y())), (f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z())));
    tangent1.normalize();
    QVector3D bitangent1(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()), f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()), f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));
    bitangent1.normalize();

    //second triangle
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    QVector3D tangent2(f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()), f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()), f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()));
    tangent2.normalize();
    QVector3D bitangent2(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()), f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()), f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));
    bitangent2.normalize();

    tangents << tangent1;
    tangents << tangent1;

    tangents << tangent2;
    tangents << tangent2;

    bitangents << bitangent1;
    bitangents << bitangent1;

    bitangents << bitangent2;
    bitangents << bitangent2;

    
    Renderable::init(pos, norm, texCoord, idx, tangents, bitangents, textureFile, normalFile);
}

void UnitQuad::update(const qint64 msSinceLastFrame)
{
    // This is where we want to maintain our light.
    float secs = (float)msSinceLastFrame / 1000.0f;
    float angle = secs * 180.0f;
    // Rotate our light around the scene
    QMatrix4x4 rot;
    rot.setToIdentity();
    rot.rotate(angle, 0.0, 1.0, 0.0);
    QVector3D newPos = rot * lightPos_;
    lightPos_ = newPos;
    // Because we aren't doing any occlusion, the lighting on the walls looks
    // super wonky.  Instead, just move the light on the z axis.
    newPos.setX(1.0);
    newPos.setY(0.5);
    newPos.setZ(0.05);
    // TODO:  Understand how the light gets initialized/setup.
    shader_.bind();

    shader_.setUniformValue("pointLights[0].color", 1.0f, 1.0f, 1.0f);
    shader_.setUniformValue("pointLights[0].position", newPos);

    shader_.setUniformValue("pointLights[0].ambientIntensity", 0.175f);
    shader_.setUniformValue("pointLights[0].specularIntensity", 0.1f);
    shader_.setUniformValue("pointLights[0].constant", 1.0f);
    shader_.setUniformValue("pointLights[0].linear", 0.00f);
    shader_.setUniformValue("pointLights[0].quadratic", 0.00f);

    shader_.release();
}