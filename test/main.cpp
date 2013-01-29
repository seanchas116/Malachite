#include "../src/vector_sse.h"
#include "../src/pixel.h"
#include <QtGui>

using namespace Malachite;

void printVec(Vector<float, 4> vec)
{
	qDebug() << vec.e<0>() << vec.e<1>() << vec.e<2>() << vec.e<3>();
}

void testVec()
{
	Vector<float, 4> vec = { 1.f, 2.f, 3.f, 4.f };
	
	printVec(vec);
	printVec(vec.extract<3>());
}

void testPixel()
{
	
}

int main(int argc, char **argv)
{
	testVec();
	
	return 0;
	
	//QApplication app(argc, argv);
	
	//return app.exec();
}
