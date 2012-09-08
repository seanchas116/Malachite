#ifndef MLBLENDMODE_H
#define MLBLENDMODE_H

#include "mlglobal.h"

#include <QString>
#include <QPainter>
#include <QHash>
#include <QMetaType>
#include "mlblendop.h"

class MLBlendModeDictionary
{
public:
	MLBlendModeDictionary();
	
	QString name(int index) const
		{ return nameHash.value(index, "normal"); }
	QPainter::CompositionMode qPainterMode(int index) const
		{ return qtCompositionModeHash.value(index, QPainter::CompositionMode_SourceOver); }
	int indexForName(const QString &name) const
		{ return nameHash.key(name, ML::BlendModeSourceOver); }
	
private:
	QHash<int, QString> nameHash;
	QHash<int, QPainter::CompositionMode> qtCompositionModeHash;
};

class MALACHITESHARED_EXPORT MLBlendMode
{
public:
	
	MLBlendMode() : _index(ML::BlendModeNormal) {}
	MLBlendMode(int index) : _index(index) {}
	MLBlendMode(const QString &modeName) : _index(_dict.indexForName(modeName)) {}
	
	int index() const { return _index; }
	void setIndex(int index) { _index = index; }
	
	QString name() const { return _dict.name(_index); }
	void setName(const QString &name) { _index = _dict.indexForName(name); }
	
	QPainter::CompositionMode qPainterMode() const { return _dict.qPainterMode(_index); }
	
	MLBlendOp *op() const { return mlBlendOpDictionary()->blendOp(_index); }
	
	MLBlendMode &operator=(int index)
	{
		_index = index;
		return *this;
	}
	
	bool operator==(int index) { return _index == index; }
	bool operator!=(int index) { return _index != index; }
	
private:
	
	static MLBlendModeDictionary _dict;
	
	int _index;
};

Q_DECLARE_METATYPE(MLBlendMode)

#endif // MLBLENDMODE_H
