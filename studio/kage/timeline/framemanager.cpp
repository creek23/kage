
#include "framemanager.h"
#include "framesmanager.h"

KageFrameManager::KageFrameManager(KageFramesManager *p_fsm, unsigned int p_layerID, unsigned int p_frameCount) {
	pack_start(frame_padding, Gtk::PACK_SHRINK);
	fsm = p_fsm;
	layerID = p_layerID;
	frameCtr = 0;
	for (unsigned int i = 0; i < p_frameCount; ++i) {
		addFrame();
	}
}

KageFrameManager::~KageFrameManager() {
	//
}

void KageFrameManager::addFrame() {
	++frameCtr;
	frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*frames.back(), Gtk::PACK_SHRINK);
}

void KageFrameManager::extendFrame(unsigned int p_frameID) {
	++frameCtr;
	frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*frames.back(), Gtk::PACK_SHRINK);
}

void KageFrameManager::removeFrame(unsigned int p_frameID) {
	frameCtr = getFrameCount();
	for (unsigned int i = 0; i < frameCtr; ++i) {
		if (frames[i]->frameID == p_frameID) {
			remove(*frames[i]);
			--frameCtr;
			for (unsigned int j = i; j < frameCtr; ++j) {
				frames[j]->frameID -= 1;
			}
			break;
		}
	}
	pack_start(*frames.back(), Gtk::PACK_SHRINK);
}

void KageFrameManager::removeAllFrames() {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		remove(*frames[i]);
	}
	frameCtr = 0;
	frames.clear();
//	pack_start(*frames.back(), Gtk::PACK_SHRINK);
}

unsigned int KageFrameManager::getID() {
	return layerID;
}

unsigned int KageFrameManager::getFrameCount() {
	return frames.size();
}

bool KageFrameManager::selectAll(bool p_selectAll) {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		frames[i]->setSelected(p_selectAll);
	}
	return true;
}

KageFramesManager *KageFrameManager::getFsm() {
	return fsm;
}

/** For use of Kage.  When a KageLayer is clicked, KageLayerManager will
 * call this function via Kage then sets currently active Frame along Layer
 * \param p_layer is index of Layer known to KageFrame
 * \sa getCurrentLayer()
*/
void KageFrameManager::setCurrentFrame(unsigned int p_frame) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		frames[_currentFrameIndex]->setSelected(false);
		frames[_currentFrameIndex]->setCurrent(false);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				frames[i]->setSelected(false);
				frames[i]->setCurrent(false);
				break;
			}
		}
	}
	
	unsigned int l_count = getFrameCount();
	if (p_frame > l_count) {
		p_frame = l_count;
	}
	if (p_frame < 1) {
		p_frame = 1;
	}
	_currentFrameIndex = p_frame-1;
	_currentFrameID = frames[_currentFrameIndex]->frameID;
	frames[_currentFrameIndex]->setSelected(true);
	frames[_currentFrameIndex]->setCurrent(true);
}

void KageFrameManager::focusFrame(unsigned int p_frame) {
	if (p_frame < 1 || p_frame > getFrameCount()) {
		return;
	}
	frames[p_frame-1]->setFocus();
}

KageFrame *KageFrameManager::getFrameAt(unsigned int p_frame) {
	if (p_frame < 1 || p_frame > getFrameCount()) {
		return NULL;
	}
	
	return frames[p_frame-1];
}


/** For use of KageFrame.  When a KageFrame is clicked, previously clicked
 * KageFrame should be un-selected.
 * \param p_frame is pointer to KageFrame who called this function
*/
void KageFrameManager::setSelected(KageFrame *p_frame) {
	fsm->selectAll(false);
	fsm->setCurrentLayerByID(p_frame->layerID);
	fsm->setCurrentFrame(p_frame->frameID);
/*	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		frames[_currentFrameIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				frames[i]->setSelected(false);
				break;
			}
		}
	}*/
	p_frame->setSelected(true);
	_currentFrameID = p_frame->frameID;
}

vector<unsigned int> KageFrameManager::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->raiseSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->raiseSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameManager::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->lowerSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->lowerSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameManager::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->raiseToTopSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->raiseToTopSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameManager::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->lowerToBottomSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->lowerToBottomSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageFrameManager::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->groupSelectedShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->groupSelectedShapes(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameManager::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->ungroupSelectedShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->ungroupSelectedShapes(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageFrameManager::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->duplicateShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->duplicateShapes(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

bool KageFrameManager::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->flipHorizontalSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->flipHorizontalSelectedShape(p_selectedShapes);
			}
		}
	}
	
	return false;
}
bool KageFrameManager::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->flipVerticalSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return frames[_currentFrameIndex]->flipVerticalSelectedShape(p_selectedShapes);
			}
		}
	}
	
	return false;
}
