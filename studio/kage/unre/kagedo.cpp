
#include "kagedo.h"

KageDo::KageDo() {
	_layer = -1;
	_frame = -1;
}
KageDo::KageDo(unsigned int p_layer, unsigned int p_frame) {
	_layer = p_layer;
	_frame = p_frame;
}

KageDo::~KageDo() {
	_vectorData.clear();
}

void KageDo::setVectorData(vector<VectorData> p_vectorData) {
	_vectorData = p_vectorData;
}

vector<VectorData> KageDo::getVectorData() {
	return _vectorData;
}

void KageDo::clear() {
	_vectorData.clear();
}

void KageDo::push(KageDo p_vectorsData) {
	vector<VectorData> l_v = p_vectorsData.getVectorData();
	int isrc = l_v.size();
	
	for (int i = 0; i < isrc; ++i) {
		_vectorData.push_back(l_v[i]);
	}
}

void KageDo::add(VectorData::type p_type, ColorData p_fill, StrokeColorData p_stroke) {
	VectorData l_vectorData;
	
		l_vectorData.vectorType = p_type;
		l_vectorData.stroke = p_stroke;
		l_vectorData.fillColor = p_fill;
	
	_vectorData.push_back(l_vectorData);
}

KageDo KageDo::clone() {
	vector<VectorData> l_vectorData;
	unsigned int count = _vectorData.size();
		for (unsigned int i = 0; i < count; ++i) {
			l_vectorData.push_back(_vectorData[i].clone());
		}
	KageDo l_kageDo(_layer, _frame);
	l_kageDo.setVectorData(l_vectorData);
	
	return l_kageDo;
}

bool KageDo::isEmpty() {
	return (_vectorData.size() == 0);
}
