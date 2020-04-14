#pragma once

#include <QtOpenGL>
#include <QtGui>

#include "Particle.h"
#include "Renderable.h"

class Emitter {
protected:
    QVector<Particle*> particles_;
    QVector3D position_;
    QVector3D orientation_;
    unsigned int particlesPerSecond_;
    Renderable* particleModel_;
    unsigned int msSinceEmit_;

public:
    Emitter();
    Emitter(const QVector3D& position, const QVector3D& orientation, unsigned int pps, Renderable* particleModel);

    void update(unsigned int msSinceLastFrame);
    void draw(const QMatrix4x4& view, const QMatrix4x4& projection);
    void emitParticle();
};