#include <QtOpenGL>
#include <QtGui>
#include <iostream>

#include "Particle.h"

Particle::Particle(Renderable* model, const QMatrix4x4& transform, const QVector3D& velocity, float lifespan) {
	modelToRender_ = model;
	velocity_ = QVector3D(velocity);
	secondsLeftToLive_ = lifespan;
	particleTransform_ = QMatrix4x4(transform);
}

void Particle::update(unsigned int msSinceLastFrame) {
	float seconds = msSinceLastFrame / 1000.;
	secondsLeftToLive_ -= seconds;
	particleTransform_.translate(velocity_ * seconds);
	velocity_ -= seconds * QVector3D(0., 10., 0.);

	QVector4D col = particleTransform_.column(3);
}

void Particle::draw(const QMatrix4x4& view, const QMatrix4x4& projection) {
	QVector4D col = particleTransform_.column(3);
	modelToRender_->draw(particleTransform_, view, projection);
}