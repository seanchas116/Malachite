#include <QTest>
#include <QDebug>
#include <Malachite/BlendMode>
#include <random>
#include <boost/range.hpp>

#include "test.h"

using namespace Malachite;

Test::Test(QObject *parent) :
	QObject(parent)
{
}

constexpr int pixelVectorSize = 10;

static Pixel blendSourceOver(const Pixel &dst, const Pixel &src)
{
	return src.v() + (1.0f - src.a()) * dst.v();
}

static QVector<Pixel> makeRandomPixelVector()
{
	QVector<Pixel> vec(pixelVectorSize);
	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.f, 1.f);
	
	for (int i = 0; i < pixelVectorSize; ++i)
	{
		float a = dist(mt);
		float r = dist(mt) * a;
		float g = dist(mt) * a;
		float b = dist(mt) * a;
		vec[i] = Pixel(a, r, g, b);
	}
	
	return vec;
}

static bool comparePixelVectors(const QVector<Pixel> &v1, const QVector<Pixel> &v2)
{
	auto predicate = [](const Pixel &p1, const Pixel &p2)->bool
	{
		return qFuzzyCompare(p1.a(), p2.a()) && qFuzzyCompare(p1.r(), p2.r()) && qFuzzyCompare(p1.g(), p2.g()) && qFuzzyCompare(p1.b(), p2.b());
	};
	
	return boost::range::equal(v1, v2, predicate);
}

void Test::test_blend()
{
	auto blendOp = BlendMode(BlendMode::SourceOver).op();
	
	const auto dstPixels = makeRandomPixelVector();
	const auto srcPixels = makeRandomPixelVector();
	
	// simple blending
	{
		auto correctResultPixels = dstPixels;
		
		for (int i = 0; i < pixelVectorSize; ++i)
			correctResultPixels[i] = blendSourceOver(correctResultPixels[i], srcPixels[i]);
		
		auto resultPixels = dstPixels;
		auto dst = wrapPointer(resultPixels.data(), pixelVectorSize * sizeof(Pixel), resultPixels.data());
		auto src = wrapPointer(srcPixels.data(), pixelVectorSize * sizeof(Pixel), srcPixels.data());
		
		blendOp->blend(srcPixels.size(), dst, src);
		
		qDebug() << "dst" << dstPixels;
		qDebug() << "src" << srcPixels;
		qDebug() << "correct" << correctResultPixels;
		qDebug() << "result" << resultPixels;
		
		QVERIFY(comparePixelVectors(correctResultPixels, resultPixels));
	}
	
	// blending with pixel masks
	{
		const auto maskPixels = makeRandomPixelVector();
		
		auto correctResultPixels = dstPixels;
		
		for (int i = 0; i < pixelVectorSize; ++i)
		{
			PixelVec src = srcPixels[i].v() * maskPixels[i].aV();
			correctResultPixels[i] = blendSourceOver(correctResultPixels[i], src);
		}
		
		auto resultPixels = dstPixels;
		auto dst = wrapPointer(resultPixels.data(), pixelVectorSize * sizeof(Pixel), resultPixels.data());
		auto src = wrapPointer(srcPixels.data(), pixelVectorSize * sizeof(Pixel), srcPixels.data());
		auto mask = wrapPointer(maskPixels.data(), pixelVectorSize * sizeof(Pixel), maskPixels.data());
		
		blendOp->blend(srcPixels.size(), dst, src, mask);
		
		qDebug() << "dst" << dstPixels;
		qDebug() << "src" << srcPixels;
		qDebug() << "correct" << correctResultPixels;
		qDebug() << "result" << resultPixels;
		
		QVERIFY(comparePixelVectors(correctResultPixels, resultPixels));
	}
}

QTEST_MAIN(Test)
