
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

void KageFrameManager::duplicateFrame() {
	//TODO: 1. call extendFrame if next Frame is Extended
	//      2. copy content of Current frame to Next Frame
}

bool KageFrameManager::moveToLeft() {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		if (_currentFrameIndex > 0) {
			reorder_child(*frames[_currentFrameIndex], _currentFrameIndex);
			swap(frames[_currentFrameIndex], frames[_currentFrameIndex-1]);
			--_currentFrameIndex;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				if (_currentFrameIndex > 0) {
					reorder_child(*frames[_currentFrameIndex], _currentFrameIndex);
					swap(frames[_currentFrameIndex], frames[_currentFrameIndex-1]);
					--_currentFrameIndex;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
bool KageFrameManager::moveToLeftAt(unsigned int p_frame) {
	bool l_return = false;
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		while (_currentFrameIndex > p_frame) {
			l_return = moveToLeft();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				while (_currentFrameIndex > p_frame) {
					l_return = moveToLeft();
				}
				return l_return;
			}
		}
	}
	return false;
}
bool KageFrameManager::moveToLeftMost() {
	bool l_return = false;
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		while (_currentFrameIndex > 0) {
			l_return = moveToLeft();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				while (_currentFrameIndex > 0) {
					l_return = moveToLeft();
				}
				return l_return;
			}
		}
	}
	return false;
}

void KageFrameManager::extendFrame() {
	++frameCtr;
	unsigned int l_currentFrame = getCurrentFrame();
	frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*frames.back(), Gtk::PACK_SHRINK);
	
	if (l_currentFrame == getFrameCount()) {
		KageFrame::extension l_extension = frames[frames.size()-2]->getExtension();
		if (l_extension == KageFrame::extension::EXTENSION_NOT) {
			frames[frames.size()-2]->setExtension(KageFrame::extension::EXTENSION_START);
			frames[frames.size()-1]->setExtension(KageFrame::extension::EXTENSION_END);
		} else if (l_extension == KageFrame::extension::EXTENSION_END) {
			frames[frames.size()-2]->setExtension(KageFrame::extension::EXTENSION_MID);
			frames[frames.size()-1]->setExtension(KageFrame::extension::EXTENSION_END);
		}
	} else if (l_currentFrame < getFrameCount()) {
//		frames.insert(frames.begin() + l_currentFrame, Gtk::manage(new KageFrame(this, layerID, frameCtr)));
//			pack_start(*frames[l_currentFrame], Gtk::PACK_SHRINK);
		unsigned int l_frameIndex = getFrameCount();
		while (l_frameIndex > l_currentFrame+1) {
			setCurrentFrame(l_frameIndex);
			moveToLeft();
			--l_frameIndex;
		}
			
		KageFrame::extension l_extension = frames[l_currentFrame-1]->getExtension();
			cout << "\t\t l_extension " << l_extension << " " << KageFrame::extension::EXTENSION_NOT << " " << KageFrame::extension::EXTENSION_START << " " << KageFrame::extension::EXTENSION_MID << " " << KageFrame::extension::EXTENSION_END << endl;
		if (l_extension == KageFrame::extension::EXTENSION_NOT) {
			frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_START);
			frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_END);
		} else if (l_extension == KageFrame::extension::EXTENSION_START) {
			l_extension = frames[l_currentFrame+1]->getExtension();
			if (       l_extension == KageFrame::extension::EXTENSION_NOT
					|| l_extension == KageFrame::extension::EXTENSION_START) {
				frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_END);
			} else if (l_extension == KageFrame::extension::EXTENSION_MID
					|| l_extension == KageFrame::extension::EXTENSION_END) {
				frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_MID);
			}
		} else if (l_extension == KageFrame::extension::EXTENSION_MID) {
			l_extension = frames[l_currentFrame+1]->getExtension();
			if (       l_extension == KageFrame::extension::EXTENSION_NOT
					|| l_extension == KageFrame::extension::EXTENSION_START) {
				frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_END);
			} else if (l_extension == KageFrame::extension::EXTENSION_MID
					|| l_extension == KageFrame::extension::EXTENSION_END) {
				frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_MID);
			}
		} else if (l_extension == KageFrame::extension::EXTENSION_END) {
			frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_MID);
			frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_END);
		}
		setCurrentFrame(l_currentFrame); //restore Current Frame because moveToLeft changed Current Frame
	}
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
 * \param p_frame is Frame Number known to KageFrame(/Layer?)
 * \sa getCurrentFrame()
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

unsigned int KageFrameManager::getCurrentFrame() {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _currentFrameIndex+1;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				return i+1;
			}
		}
	}
	return 1;
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->raiseSelectedShape(p_selectedShapes);
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->lowerSelectedShape(p_selectedShapes);
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->raiseToTopSelectedShape(p_selectedShapes);
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->lowerToBottomSelectedShape(p_selectedShapes);
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->groupSelectedShapes(p_selectedShapes);
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->ungroupSelectedShapes(p_selectedShapes);
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->duplicateShapes(p_selectedShapes);
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
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->flipHorizontalSelectedShape(p_selectedShapes);
			}
		}
	}
	
	return false;
}
bool KageFrameManager::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return doFlipVerticalSelectedShapeOn(_currentFrameIndex, p_selectedShapes);
/*		if (       frames[_currentFrameIndex]->getExtension() == KageFrame::extension::EXTENSION_NOT
				|| frames[_currentFrameIndex]->getExtension() == KageFrame::extension::EXTENSION_START) {
			return frames[_currentFrameIndex]->flipVerticalSelectedShape(p_selectedShapes);
		} else {
			return doFlipVerticalSelectedShapeOnExtendedFrame(_currentFrameIndex-1, p_selectedShapes);
			unsigned int l_frameIndex = _currentFrameIndex-1;
			while (l_frameIndex > 0) {
				if (       frames[l_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_NOT
						|| frames[l_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_START) {
					return frames[l_frameIndex]->flipVerticalSelectedShape(p_selectedShapes);
				} else {
					--l_frameIndex;
				}
			}
			return false;
		}*/
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return doFlipVerticalSelectedShapeOn(_currentFrameIndex, p_selectedShapes);
			}
		}
	}
	
	return false;
}
	bool KageFrameManager::doFlipVerticalSelectedShapeOn(unsigned int p_frameIndex, vector<unsigned int> p_selectedShapes) {
		if (       frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_NOT
				|| frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_START) {
			return frames[p_frameIndex]->flipVerticalSelectedShape(p_selectedShapes);
		} else {
			return doFlipVerticalSelectedShapeOnExtendedFrame(p_frameIndex-1, p_selectedShapes);
		}
	}
	bool KageFrameManager::doFlipVerticalSelectedShapeOnExtendedFrame(unsigned int p_frameIndex, vector<unsigned int> p_selectedShapes) {
		while (p_frameIndex > 0) {
			if (       frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_NOT
					|| frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_START) {
				return frames[p_frameIndex]->flipVerticalSelectedShape(p_selectedShapes);
			} else {
				--p_frameIndex;
			}
		}
		return false;
	}


bool KageFrameManager::addDataToFrame(VectorDataManager p_vectorsData) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		frames[_currentFrameIndex]->addDataToFrame(p_vectorsData);
		return true;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				frames[i]->addDataToFrame(p_vectorsData);
				return true;
			}
		}
	}
	
	return false;
}

bool KageFrameManager::setFrameData(VectorDataManager p_vectorsData) {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		frames[_currentFrameIndex]->setFrameData(p_vectorsData.clone());
		return true;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				frames[i]->setFrameData(p_vectorsData.clone());
				return true;
			}
		}
	}
	
	return false;
}

VectorDataManager KageFrameManager::getFrameData() {
	if (_currentFrameIndex < getFrameCount() && frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return frames[_currentFrameIndex]->getFrameData();
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = frames[i]->frameID;
				return frames[i]->getFrameData();
			}
		}
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}
VectorDataManager KageFrameManager::getFrameDataAt(unsigned int p_frame) {
	if (p_frame > 0 && p_frame <= getFrameCount()) {
		--p_frame; //layer now becomes Frame Index
		return frames[p_frame]->getFrameData();
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}
VectorDataManager KageFrameManager::getPreviousFrameData(unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (frames[i]->frameID == p_frameID) {
			return frames[i-1]->getFrameData();
		}
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}

bool KageFrameManager::setFrameDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (frames[i]->frameID == p_frameID) {
			frames[i-1]->setFrameData(p_vectorsData);
			return true;
		}
	}
	
	return false;
}

bool KageFrameManager::addDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (frames[i]->frameID == p_frameID) {
			frames[i-1]->addDataToFrame(p_vectorsData);
			return true;
		}
	}
	
	return false;
}

bool KageFrameManager::switchToPreviousFrame(unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (frames[i]->frameID == p_frameID) {
			///index + 1 = becomes frameNumber for current. then -1 becomes frameNumber of Previous Frame; thus `setCurrentFrame(i)'
			setCurrentFrame(i);
			focusFrame(i);
			return true;
		}
	}
	
	return false;
}

bool KageFrameManager::switchToNextFrame(unsigned int p_frameID) {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		if (frames[i]->frameID == p_frameID) {
			///index + 1 = becomes frameNumber for current. then +1 becomes frameNumber of Next Frame
			setCurrentFrame(i+2);
			focusFrame(i+2);
			return true;
		}
	}
	
	return false;
}

bool KageFrameManager::isCurrentFrame(unsigned int p_frameID) {
	if (frames[_currentFrameIndex]->frameID == p_frameID
			&& _currentFrameID == p_frameID) {
		return true;
	}
	
	return false;
}
