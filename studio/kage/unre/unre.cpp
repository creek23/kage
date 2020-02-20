
#include "unre.h"

UnRe::UnRe() {
	clear();
}

UnRe::~UnRe() {
	clear();
}

void UnRe::clear() {
	_stackIndex = -1;
	_undoStack.clear();
}

void UnRe::stackDo(unsigned int p_layer, unsigned int p_frame, vector<VectorData> p_data) {
	KageDo p_kageDo(p_layer, p_frame);
		p_kageDo.setVectorData(p_data);
	
	unsigned int l_size = _undoStack.size();
	if (_stackIndex < l_size - 1) {
			_undoStack.erase(_undoStack.begin() + _stackIndex+1, _undoStack.end());
	}
	
	_undoStack.push_back(p_kageDo.clone());
	cout << "UNDO SIZE: " << _undoStack.size() << endl;
	
	++_stackIndex;
}

KageDo UnRe::undo() {
	KageDo l_kageDo;
	
	if (_stackIndex > 0 && _stackIndex != -1) {
		--_stackIndex;
		l_kageDo = _undoStack[_stackIndex].clone();
	}
	
	return l_kageDo;
}

KageDo UnRe::redo() {
	KageDo l_kageDo;
	
	unsigned int l_size = _undoStack.size();
	if (_stackIndex < l_size-1 && _stackIndex != -1) {
		++_stackIndex;
		l_kageDo = _undoStack[_stackIndex].clone();
	} else if (_stackIndex == -1 && l_size > 0) {
		++_stackIndex;
		l_kageDo = _undoStack[_stackIndex].clone();
	}
	
	return l_kageDo;
}

PointData UnRe::applyZoomRatio(PointData p_zoomReference, double p_zoomRatio, PointData p_value) {
	if (p_zoomRatio == 0.0f) { return p_value; }
	
	double l_value;
	
	if (p_value.x < p_zoomReference.x) {
		l_value = (p_zoomReference.x - p_value.x) * p_zoomRatio;
		p_value.x = p_zoomReference.x - l_value;
	} else if (p_value.x > p_zoomReference.x) {
		l_value = (p_value.x - p_zoomReference.x) * p_zoomRatio;
		p_value.x = p_zoomReference.x + l_value;
	}
	if (p_value.y < p_zoomReference.y) {
		l_value = (p_zoomReference.y - p_value.y) * p_zoomRatio;
		p_value.y = p_zoomReference.y - l_value;
	} else if (p_value.y > p_zoomReference.y) {
		l_value = (p_value.y - p_zoomReference.y) * p_zoomRatio;
		p_value.y = p_zoomReference.y + l_value;
	}
	
	return p_value.clone();
}

void UnRe::applyZoom(PointData p_originBefore, PointData p_originAfter, PointData p_zoomReference, double p_zoomRatio) {
	unsigned int l_size = _undoStack.size();
	for (unsigned int l_undoStackIndex = 0; l_undoStackIndex < l_size; ++l_undoStackIndex) {
		vector<VectorData> v = _undoStack[l_undoStackIndex].getVectorData();
		
		for (unsigned int i = 0; i < v.size(); ++i) {
			if (v[i].vectorType == VectorData::TYPE_MOVE
					|| v[i].vectorType == VectorData::TYPE_LINE) {
				v[i].points[0].x += p_originBefore.x;
				v[i].points[0].y += p_originBefore.y;
					v[i].points[0] = applyZoomRatio(p_zoomReference, p_zoomRatio, v[i].points[0]);
				v[i].points[0].x -= p_originAfter.x;
				v[i].points[0].y -= p_originAfter.y;
			} else if (v[i].vectorType == VectorData::TYPE_CURVE_CUBIC
					|| v[i].vectorType == VectorData::TYPE_CURVE_QUADRATIC) {
				for (unsigned int j = 0; j < 3; ++j) {
					v[i].points[j].x += p_originBefore.x;
					v[i].points[j].y += p_originBefore.y;
						v[i].points[j] = applyZoomRatio(p_zoomReference, p_zoomRatio, v[i].points[j]);
					v[i].points[j].x -= p_originAfter.x;
					v[i].points[j].y -= p_originAfter.y;
				}
			}
		}
		
		_undoStack[l_undoStackIndex].setVectorData(v);
	}
}
