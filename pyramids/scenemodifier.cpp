/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scenemodifier.h"

#include <QtCore/QDebug>
#include <cmath>

static const QVector3D g_offset(-1.2f, -5.5f, -1.5f);
static constexpr float g_radius = 1.0f;

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
    : m_rootEntity(rootEntity)
    , m_floor()
    , m_spheres()
    , m_a(1)
    , m_b(0)
{
    vec2d v(0.0f, 0.0f);
    m_floor.insert(v, 0.0f);
    for (int ring = 1; ring <= 12; ring++) {
        const float radius = ring * g_radius;
        const int steps = ring * 6;
        for (int angle = 0; angle < 360; angle += 360/steps) {
            const float x = static_cast<float>(cos(M_PI * angle / 180.0)) * radius;
            const float z = static_cast<float>(sin(M_PI * angle / 180.0)) * radius;
            v = vec2d(x, z);
            m_floor.insert(v, 0.0f);
        }
    }
    setAB(1, 0);
}

SceneModifier::~SceneModifier()
{
}

bool SceneModifier::setAB(int a, int b)
{
    foreach(const vec2d& v, m_floor.keys())
        m_floor[v] = 0.0f;
    qDeleteAll(m_spheres);
    m_spheres.clear();

    for (int i = 0; i < a; i++)
        addLayer(i);

    for (int i = 0; i < b; i++)
        addLayer(i, QColor(0x29, 0x29, 0xe9));
    return true;
}

void SceneModifier::changedA(int a)
{
    if (setAB(a, m_b))
        m_a = a;
}

void SceneModifier::changedB(int b)
{
    if (setAB(m_a, b))
        m_b = b;
}

void SceneModifier::addSphere(const QVector3D& vec, const QColor& color)
{
    // Sphere shape data
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRings(10);
    sphereMesh->setSlices(10);
    sphereMesh->setRadius(g_radius / 2);

    // Sphere mesh transform
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

    sphereTransform->setScale(1.0f);
    sphereTransform->setTranslation(vec + g_offset);

    Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
    sphereMaterial->setDiffuse(color);

    // Sphere
    Qt3DCore::QEntity* sphere = new Qt3DCore::QEntity(m_rootEntity);
    sphere->addComponent(sphereMesh);
    sphere->addComponent(sphereMaterial);
    sphere->addComponent(sphereTransform);

    m_spheres.append(sphere);
}

void SceneModifier::addSphere(const float x, const float y, const float z, const QColor& color)
{
    addSphere(QVector3D(x,y,z), color);
}

bool SceneModifier::findSphere(const float x, const float z, QVector3D& vec)
{
    foreach(const vec2d& v, m_floor.keys()) {
        if (!qFuzzyCompare(x, v.x()))
            continue;
        if (!qFuzzyCompare(z, v.z()))
            continue;
        const float y = m_floor[v];
        vec.setX(x);
        vec.setY(y);
        vec.setZ(z);
        m_floor[v] = y + g_radius;
        return true;
    }
    return false;
}

void SceneModifier::addLayer(const int n, const QColor& color)
{
    QVector3D v;
    findSphere(0.0f, 0.0f, v);
    addSphere(v.x(), v.y(), v.z(), color);
    for (int ring = 1; ring <= n; ring++) {
        const float rad = ring * g_radius;
        const int steps = ring * 6;
        for (int angle = 0; angle < 360; angle += 360/steps) {
            const float x = static_cast<float>(cos(M_PI * angle / 180.0)) * rad;
            const float z = static_cast<float>(sin(M_PI * angle / 180.0)) * rad;
            if (findSphere(x,z,v))
                addSphere(v.x(), v.y(), v.z(), color);
        }
    }
}

