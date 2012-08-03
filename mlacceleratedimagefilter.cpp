#include <QtCore>
#include "glu.h"
#include "mlacceleratedimagefilter.h"

#define LOG_BUFFER_SIZE	1024

MLAcceleratedImageFilter::MLAcceleratedImageFilter(GLuint texId, const QSize &size) :
	_size(size),
	_texId(texId)
{
	init();
}

MLAcceleratedImageFilter::MLAcceleratedImageFilter(MLAcceleratedImage *image) :
	_size(image->size()),
	_texId(image->textureId())
{
	init();
}

MLAcceleratedImageFilter::~MLAcceleratedImageFilter()
{
	glDeleteFramebuffers(1, &_fboId);
	glDeleteRenderbuffers(1, &_rboId);
}

void MLAcceleratedImageFilter::init()
{
	glGenRenderbuffers(1, &_rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _size.width(), _size.height());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	glGenFramebuffers(1, &_fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texId, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboId);
	GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if (stat != GL_FRAMEBUFFER_COMPLETE)
		qWarning() << Q_FUNC_INFO << ": could not create framebuffer";
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MLAcceleratedImageFilter::addInputImage(GLuint id, const QByteArray &name)
{
	if (_inTexInfos.size() >= MaxInImageCount)
	{
		qWarning() << Q_FUNC_INFO << ": too many input images";
		return;
	}
	
	_inTexInfos << TextureInfo(id, name);
}

void MLAcceleratedImageFilter::render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	
	_program.bind();
	
	bindInTextures();
	
	for (int i = 0; i < _inTexInfos.size(); ++i)
	{
		GLuint loc = glGetUniformLocation(_program.programId(), _inTexInfos.at(i).name.constData());
		glUniform1i(loc, i);
		//_program.setUniformValue(_inTexInfos.at(i).name.data(), i);
	}
	
	//GLuint viewportLoc = glGetUniformLocation(_program.programId(), "ml_CoordFactor");
	//glUniform2f(viewportLoc, 1.0 / _size.width(), 1.0 / _size.height());
	//_program.setUniformValue("ml_Viewport", _size.width(), _size.height());
	
	glViewport(0, 0, _size.width(), _size.height());
	
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	glColor3f(1, 0.5, 0.5);
	glVertex2i(-1, -1);
	glColor3f(0.5, 1, 0.5);
	glVertex2i(1, -1);
	glColor3f(1, 0.5, 0.5);
	glVertex2i(1, 1);
	glColor3f(0.5, 1, 0.5);
	glVertex2i(-1, 1);
	glEnd();
	
	releaseInTextures();
	
	_program.release();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MLAcceleratedImageFilter::bindInTextures()
{
	int count = _inTexInfos.size();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, count > 0 ? _inTexInfos.at(0).id : 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, count > 1 ? _inTexInfos.at(1).id : 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, count > 2 ? _inTexInfos.at(2).id : 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, count > 3 ? _inTexInfos.at(3).id : 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, count > 4 ? _inTexInfos.at(4).id : 0);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, count > 5 ? _inTexInfos.at(5).id : 0);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, count > 6 ? _inTexInfos.at(6).id : 0);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, count > 7 ? _inTexInfos.at(7).id : 0);
	glActiveTexture(GL_TEXTURE0);
}

void MLAcceleratedImageFilter::releaseInTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}
