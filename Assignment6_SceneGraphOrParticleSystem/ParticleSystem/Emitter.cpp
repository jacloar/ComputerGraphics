#include <QtOpenGL>
#include <QtGui>
#include <iostream>

#include "Emitter.h"

Emitter::Emitter() {
    // leave everything default
}

Emitter::Emitter(const QVector3D& position, const QVector3D& orientation, unsigned int pps, Renderable* particleModel) {
    position_ = position;
    orientation_ = orientation;
    particlesPerSecond_ = pps;
    particleModel_ = particleModel;
    msSinceEmit_ = 0;
}

void Emitter::update(unsigned int msSinceLastFrame) {
    for (int i = 0; i < particles_.count(); i += 1) {
        Particle* p = particles_.at(i);
        p->update(msSinceLastFrame);
    }

    msSinceEmit_ += msSinceLastFrame;
    int numParticles = particlesPerSecond_ * (msSinceEmit_ / 1000.);
    for (int i = 0; i < numParticles; i += 1) {
        msSinceEmit_ = 0;
        emitParticle();
    }

    for (int i = 0; i < particles_.count(); i += 1) {
        Particle* p = particles_.at(i);

        if (p->isDead()) {
            delete p;
            particles_.remove(i);
            i -= 1;
        }
    }
}

void Emitter::draw(const QMatrix4x4& view, const QMatrix4x4& projection) {
    for (int i = 0; i < particles_.count(); i += 1) {
        Particle* p = particles_.at(i);
        p->draw(view, projection);
    }
}

float getOffset() {
    return ((float)rand() / (2 * RAND_MAX)) - 0.25;
}

void Emitter::emitParticle() {
    float offsetX = getOffset();
    float offsetY = getOffset();
    float offsetZ = getOffset();
    QVector3D velocity(orientation_.x() + offsetX, orientation_.y() + offsetY, orientation_.z() + offsetZ);
    velocity *= 10;

    QMatrix4x4 transform;
    transform.translate(position_);

    Particle* p = new Particle(particleModel_, transform, velocity, 3);

    particles_.push_back(p);
}