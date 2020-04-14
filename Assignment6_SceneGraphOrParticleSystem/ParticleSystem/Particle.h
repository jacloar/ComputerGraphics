#pragma once

#include <QtOpenGL>
#include <QtGui>

#include "Renderable.h"

class Particle {
protected:
    Renderable* modelToRender_;
    QMatrix4x4 particleTransform_;
    QVector3D velocity_;
    float secondsLeftToLive_;

public:
    Particle(Renderable* model, const QMatrix4x4& transform, const QVector3D& velocity, float lifespan);

    void update(unsigned int msSinceLastFrame);
    void draw(const QMatrix4x4& view, const QMatrix4x4& projection);
    bool isDead() const { return secondsLeftToLive_ <= 0.0; }
};