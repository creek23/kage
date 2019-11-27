
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
