/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QTest>
#include <Qt3DCore/private/qnode_p.h>
#include <Qt3DCore/private/qscene_p.h>

#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferdatagenerator.h>

#include "testpostmanarbiter.h"

class TestFunctor : public Qt3DRender::QBufferDataGenerator
{
public:
    explicit TestFunctor(int size)
        : m_size(size)
    {}

    QByteArray operator ()() Q_DECL_FINAL
    {
        return QByteArray();
    }

    bool operator ==(const Qt3DRender::QBufferDataGenerator &other) const
    {
        const TestFunctor *otherFunctor = functor_cast<TestFunctor>(&other);
        if (otherFunctor != Q_NULLPTR)
            return otherFunctor->m_size == m_size;
        return false;
    }

    QT3D_FUNCTOR(TestFunctor)

private:
    int m_size;
};

// We need to call QNode::clone which is protected
// So we sublcass QNode instead of QObject
class tst_QBuffer: public Qt3DCore::QNode
{
    Q_OBJECT
public:
    ~tst_QBuffer()
    {
        QNode::cleanup();
    }

private Q_SLOTS:

    void checkCloning_data()
    {
        QTest::addColumn<Qt3DRender::QBuffer *>("buffer");

        Qt3DRender::QBuffer *defaultConstructed = new Qt3DRender::QBuffer();
        QTest::newRow("defaultConstructed") << defaultConstructed;

        Qt3DRender::QBuffer *buffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
        buffer->setUsage(Qt3DRender::QBuffer::DynamicRead);
        buffer->setData(QByteArrayLiteral("There's no replacement"));
        buffer->setDataGenerator(Qt3DRender::QBufferDataGeneratorPtr(new TestFunctor(883)));
        QTest::newRow("vertex") << buffer;

        Qt3DRender::QBuffer *indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer);
        indexBuffer->setUsage(Qt3DRender::QBuffer::StaticCopy);
        indexBuffer->setData(QByteArrayLiteral("For displacement"));
        indexBuffer->setDataGenerator(Qt3DRender::QBufferDataGeneratorPtr(new TestFunctor(1340)));
        indexBuffer->setSyncData(true);
        QTest::newRow("index") << indexBuffer;
    }

    void checkCloning()
    {
        // GIVEN
        QFETCH(Qt3DRender::QBuffer *, buffer);

        // WHEN
        Qt3DRender::QBuffer *clone = static_cast<Qt3DRender::QBuffer *>(QNode::clone(buffer));

        // THEN
        QVERIFY(clone != Q_NULLPTR);

        QCOMPARE(buffer->id(), clone->id());
        QCOMPARE(buffer->data(), clone->data());
        QCOMPARE(buffer->usage(), clone->usage());
        QCOMPARE(buffer->type(), clone->type());
        QCOMPARE(buffer->dataGenerator(), clone->dataGenerator());
        QCOMPARE(buffer->isSyncData(), clone->isSyncData());
        if (buffer->dataGenerator()) {
            QVERIFY(clone->dataGenerator());
            QVERIFY(*clone->dataGenerator() == *buffer->dataGenerator());
        }
    }

    void checkPropertyUpdates()
    {
        // GIVEN
        QScopedPointer<Qt3DRender::QBuffer> buffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer));
        TestArbiter arbiter(buffer.data());

        // WHEN
        buffer->setType(Qt3DRender::QBuffer::IndexBuffer);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        Qt3DCore::QNodePropertyChangePtr change = arbiter.events.first().staticCast<Qt3DCore::QNodePropertyChange>();
        QCOMPARE(change->propertyName(), "type");
        QCOMPARE(change->value().value<int>(), static_cast<int>(Qt3DRender::QBuffer::IndexBuffer));

        arbiter.events.clear();

        // WHEN
        buffer->setUsage(Qt3DRender::QBuffer::DynamicCopy);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        change = arbiter.events.first().staticCast<Qt3DCore::QNodePropertyChange>();
        QCOMPARE(change->propertyName(), "usage");
        QCOMPARE(change->value().value<int>(), static_cast<int>(Qt3DRender::QBuffer::DynamicCopy));

        arbiter.events.clear();

        // WHEN
        buffer->setData(QByteArrayLiteral("Z28"));
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        change = arbiter.events.first().staticCast<Qt3DCore::QNodePropertyChange>();
        QCOMPARE(change->propertyName(), "data");
        QCOMPARE(change->value().value<QByteArray>(), QByteArrayLiteral("Z28"));

        arbiter.events.clear();

        // WHEN
        Qt3DRender::QBufferDataGeneratorPtr functor(new TestFunctor(355));
        buffer->setDataGenerator(functor);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        change = arbiter.events.first().staticCast<Qt3DCore::QNodePropertyChange>();
        QCOMPARE(change->propertyName(), "dataGenerator");
        QCOMPARE(change->value().value<Qt3DRender::QBufferDataGeneratorPtr>(), functor);

        arbiter.events.clear();

        // WHEN
        buffer->setSyncData(true);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        change = arbiter.events.first().staticCast<Qt3DCore::QNodePropertyChange>();
        QCOMPARE(change->propertyName(), "syncData");
        QCOMPARE(change->value().toBool(), true);

        arbiter.events.clear();
    }

protected:
    Qt3DCore::QNode *doClone() const Q_DECL_OVERRIDE
    {
        return Q_NULLPTR;
    }

};

QTEST_MAIN(tst_QBuffer)

#include "tst_qbuffer.moc"
