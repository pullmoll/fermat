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

#ifndef SCENEMODIFIER_H
#define SCENEMODIFIER_H

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

class vec2d
{
public:
    explicit vec2d() : m_x(), m_z() {}
    vec2d(float x, float z ) : m_x(x), m_z(z) {}
    float x() const { return m_x; }
    float z() const { return m_z; }
    bool operator < (const vec2d& other) const
    {
        if (qFuzzyCompare(m_x, other.m_x)) {
            return m_z < other.m_z;
        }
        return m_x < other.m_x;
    }
private:
    float m_x;
    float m_z;
};

class SceneModifier : public QObject
{
    Q_OBJECT

public:
    explicit SceneModifier(Qt3DCore::QEntity *rootEntity);
    ~SceneModifier();

public slots:
    bool setAB(int a, int b);

    void changedA(int a);
    void changedB(int b);

private:
    void addSphere(const QVector3D& vec, const QColor& color = QColor(0xa6, 0x99, 0x29));
    void addSphere(const float x, const float y, const float z, const QColor& color = QColor(0xa6, 0x99, 0x29));
    bool findSphere(const float x, const float z, QVector3D& vec);
    void addLayer(const int n, const QColor& color = QColor(0xa6, 0x99, 0x29));

    Qt3DCore::QEntity *m_rootEntity;
    QMap<vec2d, float> m_floor;
    QVector<Qt3DCore::QEntity*> m_spheres;
    int m_a;
    int m_b;
};

#endif // SCENEMODIFIER_H

