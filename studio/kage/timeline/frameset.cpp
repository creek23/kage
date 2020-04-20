
#include "frameset.h"
#include "framesmanager.h"

KageFrameset::KageFrameset(KageFramesetManager *p_fsm, unsigned int p_layerID, unsigned int p_frameCount) {
	pack_start(frame_padding, Gtk::PACK_SHRINK);
	fsm = p_fsm;
	layerID = p_layerID;
	frameCtr = 0;
	for (unsigned int i = 0; i < p_frameCount; ++i) {
		addFrame();
	}
}

KageFrameset::~KageFrameset() {
	//
}

bool KageFrameset::addFrame() {
	unsigned int l_currentFrame = getCurrentFrame();
	
	++frameCtr;
	_frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*_frames.back(), Gtk::PACK_SHRINK);
	
	if (KageFramesetManager::LOADING_MODE == true) {
		return true;
	}
	
	if (l_currentFrame == getFrameCount()) {
		if (_frames.size() > 1) {
			KageFrame::extension l_extension = _frames[_frames.size()-2]->getExtension();
			if (l_extension == KageFrame::EXTENSION_NOT) {
				//keep
			} else if (l_extension == KageFrame::EXTENSION_END) {
				//keep
			}
		}
	} else if (l_currentFrame < getFrameCount()) {
		unsigned int l_frameIndex = getFrameCount();
		while (l_frameIndex > l_currentFrame+1) {
			setCurrentFrame(l_frameIndex);
			moveToLeft();
			--l_frameIndex;
		}
		
		if (_frames.size() > 1) {
			KageFrame::extension l_extension = _frames[l_currentFrame-1]->getExtension();
			if (l_extension == KageFrame::EXTENSION_NOT) {
				//keep
			} else if (l_extension == KageFrame::EXTENSION_START) {
				_frames[l_currentFrame-1]->setExtension(KageFrame::EXTENSION_NOT);
				l_extension = _frames[l_currentFrame+1]->getExtension();
				if (       l_extension == KageFrame::EXTENSION_MID
						|| l_extension == KageFrame::EXTENSION_END) {
					_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_START);
				}
			} else if (l_extension == KageFrame::EXTENSION_MID) {
				_frames[l_currentFrame-1]->setExtension(KageFrame::EXTENSION_END);
				l_extension = _frames[l_currentFrame+1]->getExtension();
				if (       l_extension == KageFrame::EXTENSION_MID
						|| l_extension == KageFrame::EXTENSION_END) {
					_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_START);
				}
			} else if (l_extension == KageFrame::EXTENSION_END) {
				//keep
			}
			setCurrentFrame(l_currentFrame); //restore Current Frame because moveToLeft changed Current Frame
		}
	}
	
	return true;
}

void KageFrameset::duplicateFrame() {
	if (KageFramesetManager::LOADING_MODE == true) {
		return;
	}
	
	VectorDataManager l_frameData = getFrameData().clone();
		switchToNextFrame();
		
		unsigned int l_currentFrame = getCurrentFrame()-1;
		if (_frames.size() > 1) {
			KageFrame::extension l_extension = _frames[l_currentFrame-1]->getExtension();
			if (l_extension == KageFrame::EXTENSION_NOT) {
				//keep
	//			l_extension = _frames[l_currentFrame+1]->getExtension();
	//			if (       l_extension == KageFrame::EXTENSION_MID
	//					|| l_extension == KageFrame::EXTENSION_END) {
	//				_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_START);
	//			}
			} else if (l_extension == KageFrame::EXTENSION_START) {
				_frames[l_currentFrame-1]->setExtension(KageFrame::EXTENSION_NOT);
				if (l_currentFrame+1 < _frames.size()) {
					l_extension = _frames[l_currentFrame+1]->getExtension();
					if (       l_extension == KageFrame::EXTENSION_MID
							|| l_extension == KageFrame::EXTENSION_END) {
						_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_START);
					} else if (l_extension == KageFrame::EXTENSION_NOT
							|| l_extension == KageFrame::EXTENSION_START) {
						_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_NOT);
					}
				} else {
					_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_NOT);
				}
			} else if (l_extension == KageFrame::EXTENSION_MID) {
				_frames[l_currentFrame-1]->setExtension(KageFrame::EXTENSION_END);
				if (l_currentFrame+1 < _frames.size()) {
					l_extension = _frames[l_currentFrame+1]->getExtension();
					if (       l_extension == KageFrame::EXTENSION_MID
							|| l_extension == KageFrame::EXTENSION_END) {
						_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_START);
					} else if (l_extension == KageFrame::EXTENSION_NOT
							|| l_extension == KageFrame::EXTENSION_START) {
						_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_NOT);
					}
				} else {
					_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_NOT);
				}
			} else if (l_extension == KageFrame::EXTENSION_END) {
				//keep previous
				cout << " at END with " << _frames[l_currentFrame]->getExtension() << endl;
				_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_NOT);
			}
		}

	setFrameData(l_frameData);
	switchToPreviousFrame();
}

/**
 * If KageFrame is NULL or Last Frame (but not Frame-index0), remove Frame Widget, else, do belows step.
 * <ol><li>Copy content of Frame+1 to Frame+0; then make Frame+1 as Frame+0</li>
 * <li>If Frame+0 is not Last Frame, repeat 1</li>
 * <li>Set last Frame = NULL</li></ol>
 * \return True if Last Frame widget was removed
 */
bool KageFrameset::removeFrame() {
	unsigned int l_currentFrame = getCurrentFrame();
	unsigned int l_currentFrameIndex = l_currentFrame-1;
	if (_frames[l_currentFrameIndex]->isNull() == true
			|| (l_currentFrame == getFrameCount() && l_currentFrame > 1)) {
		if (getFrameCount() > 1) {
			remove(*_frames[l_currentFrameIndex]);
			_frames.erase (_frames.begin()+_frames.size()-1);
			setCurrentFrame(--l_currentFrame);
			return true;
		} else {
			cout << "last frame? do nothing" << endl;
		}
	} else if (getFrameCount() > 1) {
		KageFrame::extension l_extensionPrevious;
		KageFrame::extension l_extensionNext;
		VectorDataManager l_frameData;
		for (unsigned int i = l_currentFrameIndex; i < getFrameCount()-1; ++i) {
			if (_frames[i]->isNull() == false) {
				if (i > 0) {
					l_extensionPrevious = _frames[i-1]->getExtension();
					l_extensionNext     = _frames[i+1]->getExtension();
					switchToNextFrame();
						l_frameData = getFrameData().clone();
					if (l_extensionPrevious == KageFrame::EXTENSION_NOT) {
						//keep Previous
						if (       l_extensionNext == KageFrame::EXTENSION_START
								&& l_extensionNext == KageFrame::EXTENSION_MID) {
							_frames[i  ]->setExtension(KageFrame::EXTENSION_START);
						} else if (l_extensionNext == KageFrame::EXTENSION_END
								&& l_extensionNext == KageFrame::EXTENSION_NOT) {
							_frames[i  ]->setExtension(KageFrame::EXTENSION_NOT);
						}
					} else if (l_extensionPrevious == KageFrame::EXTENSION_START) {
						if (       l_extensionNext == KageFrame::EXTENSION_MID
								&& l_extensionNext == KageFrame::EXTENSION_END) {
							//keep Previous
							//keep current
						} else if (l_extensionNext == KageFrame::EXTENSION_NOT) {
							_frames[i-1]->setExtension(KageFrame::EXTENSION_NOT);
							_frames[i  ]->setExtension(KageFrame::EXTENSION_NOT);
						} else if (l_extensionNext == KageFrame::EXTENSION_START) {
							_frames[i-1]->setExtension(KageFrame::EXTENSION_END);
							_frames[i  ]->setExtension(KageFrame::EXTENSION_START);
						}
					} else if (l_extensionPrevious == KageFrame::EXTENSION_MID) {
						if (       l_extensionNext == KageFrame::EXTENSION_NOT) {
							_frames[i-1]->setExtension(KageFrame::EXTENSION_END);
							_frames[i  ]->setExtension(KageFrame::EXTENSION_NOT);
						} else if (l_extensionNext == KageFrame::EXTENSION_START) {
							_frames[i-1]->setExtension(KageFrame::EXTENSION_END);
							_frames[i  ]->setExtension(KageFrame::EXTENSION_START);
						} else if (l_extensionNext == KageFrame::EXTENSION_MID) {
							//keep Previous
							//keep current
						} else if (l_extensionNext == KageFrame::EXTENSION_END) {
							//keep Previous
							_frames[i  ]->setExtension(KageFrame::EXTENSION_END);
						}
					} else if (l_extensionPrevious == KageFrame::EXTENSION_END) {
						//keep Previous
						if (       l_extensionNext == KageFrame::EXTENSION_NOT) {
							_frames[i  ]->setExtension(KageFrame::EXTENSION_NOT);
						} else if (l_extensionNext == KageFrame::EXTENSION_START) {
							_frames[i  ]->setExtension(KageFrame::EXTENSION_START);
						} else if (l_extensionNext == KageFrame::EXTENSION_MID) {
							//keep current
						} else if (l_extensionNext == KageFrame::EXTENSION_END) {
							_frames[i  ]->setExtension(KageFrame::EXTENSION_NOT);
						}
					}
				} else {
					l_extensionNext = _frames[i+1]->getExtension();
					_frames[i]->setExtension(l_extensionNext);
				}
				//do copy Next Frame to This Frame
					switchToPreviousFrame();
						setFrameData(l_frameData.clone());
				switchToNextFrame();
			} else {
				_frames[i-1]->setNull(true); //when willl Frame be NULL if not the Last Frame
				break;
			}
		}
		_frames[getFrameCount()-1]->setNull(true);
		setCurrentFrame(l_currentFrame);
	}
	
	return false;
}

bool KageFrameset::moveToLeft() {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		if (_currentFrameIndex > 0) {
			reorder_child(*_frames[_currentFrameIndex], _currentFrameIndex);
			swap(_frames[_currentFrameIndex], _frames[_currentFrameIndex-1]);
			--_currentFrameIndex;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				if (_currentFrameIndex > 0) {
					reorder_child(*_frames[_currentFrameIndex], _currentFrameIndex);
					swap(_frames[_currentFrameIndex], _frames[_currentFrameIndex-1]);
					--_currentFrameIndex;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
bool KageFrameset::moveToLeftAt(unsigned int p_frame) {
	bool l_return = false;
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		while (_currentFrameIndex > p_frame) {
			l_return = moveToLeft();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
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
bool KageFrameset::moveToLeftMost() {
	bool l_return = false;
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		while (_currentFrameIndex > 0) {
			l_return = moveToLeft();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
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

void KageFrameset::extendFrame() {
	++frameCtr;
	unsigned int l_currentFrame = getCurrentFrame();
	_frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*_frames.back(), Gtk::PACK_SHRINK);
	
	if (KageFramesetManager::LOADING_MODE == true) {
		return;
	}
	
	if (l_currentFrame == getFrameCount()) {
		KageFrame::extension l_extension = _frames[_frames.size()-2]->getExtension();
		if (l_extension == KageFrame::EXTENSION_NOT) {
			_frames[_frames.size()-2]->setExtension(KageFrame::EXTENSION_START);
			_frames[_frames.size()-1]->setExtension(KageFrame::EXTENSION_END);
		} else if (l_extension == KageFrame::EXTENSION_END) {
			_frames[_frames.size()-2]->setExtension(KageFrame::EXTENSION_MID);
			_frames[_frames.size()-1]->setExtension(KageFrame::EXTENSION_END);
		}
	} else if (l_currentFrame < getFrameCount()) {
		unsigned int l_frameIndex = getFrameCount();
		while (l_frameIndex > l_currentFrame+1) {
			setCurrentFrame(l_frameIndex);
			moveToLeft();
			--l_frameIndex;
		}
		
		KageFrame::extension l_extension = _frames[l_currentFrame-1]->getExtension();
		if (l_extension == KageFrame::EXTENSION_NOT) {
			_frames[l_currentFrame-1]->setExtension(KageFrame::EXTENSION_START);
			_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_END);
		} else if (l_extension == KageFrame::EXTENSION_START) {
			l_extension = _frames[l_currentFrame+1]->getExtension();
			if (       l_extension == KageFrame::EXTENSION_NOT
					|| l_extension == KageFrame::EXTENSION_START) {
				_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_END);
			} else if (l_extension == KageFrame::EXTENSION_MID
					|| l_extension == KageFrame::EXTENSION_END) {
				_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_MID);
			}
		} else if (l_extension == KageFrame::EXTENSION_MID) {
			l_extension = _frames[l_currentFrame+1]->getExtension();
			if (       l_extension == KageFrame::EXTENSION_NOT
					|| l_extension == KageFrame::EXTENSION_START) {
				_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_END);
			} else if (l_extension == KageFrame::EXTENSION_MID
					|| l_extension == KageFrame::EXTENSION_END) {
				_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_MID);
			}
		} else if (l_extension == KageFrame::EXTENSION_END) {
			_frames[l_currentFrame-1]->setExtension(KageFrame::EXTENSION_MID);
			_frames[l_currentFrame  ]->setExtension(KageFrame::EXTENSION_END);
		}
		setCurrentFrame(l_currentFrame); //restore Current Frame because moveToLeft changed Current Frame
	}
}

void KageFrameset::removeAllFrames() {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		remove(*_frames[i]);
	}
	frameCtr = 0;
	_frames.clear();
}

unsigned int KageFrameset::getID() {
	return layerID;
}

unsigned int KageFrameset::getFrameCount() {
	return _frames.size();
}

bool KageFrameset::selectAll(bool p_selectAll) {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		_frames[i]->setSelected(p_selectAll);
	}
	return true;
}

KageFramesetManager *KageFrameset::getFsm() {
	return fsm;
}

/** For use of Kage.  When a KageLayer is clicked, KageLayerManager will
 * call this function via Kage then sets currently active Frame along Layer
 * \param p_frame is Frame Number known to KageFrame(/Layer?)
 * \sa getCurrentFrame()
*/
void KageFrameset::setCurrentFrame(unsigned int p_frame) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		_frames[_currentFrameIndex]->setSelected(false);
		_frames[_currentFrameIndex]->setCurrent(false);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_frames[i]->setSelected(false);
				_frames[i]->setCurrent(false);
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
	_currentFrameID = _frames[_currentFrameIndex]->frameID;
	_frames[_currentFrameIndex]->setSelected(true);
	_frames[_currentFrameIndex]->setCurrent(true);
}

unsigned int KageFrameset::getCurrentFrame() {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _currentFrameIndex+1;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				return i+1;
			}
		}
	}
	return 1;
}
void KageFrameset::focusFrame(unsigned int p_frame) {
	if (p_frame < 1 || p_frame > getFrameCount()) {
		return;
	}
	_frames[p_frame-1]->setFocus();
}

KageFrame *KageFrameset::getFrameAt(unsigned int p_frame) {
	if (p_frame < 1 || p_frame > getFrameCount()) {
		return NULL;
	}
	
	return _frames[p_frame-1];
}

unsigned int KageFrameset::getFrameNumberByID(unsigned int p_frameID) {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			return i+1;
		}
	}
	
	return 0;
}


/** For use of KageFrame.  When a KageFrame is clicked, previously clicked
 * KageFrame should be un-selected.
 * \param p_frame is pointer to KageFrame who called this function
*/
void KageFrameset::setSelected(KageFrame *p_frame) {
	fsm->selectAll(false);
	fsm->setCurrentLayerByID(p_frame->layerID);
	fsm->setCurrentFrameByID(p_frame->frameID);
/*	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		_frames[_currentFrameIndex]->setSelected(false);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				_frames[i]->setSelected(false);
				break;
			}
		}
	}*/
	p_frame->setSelected(true);
	_currentFrameID = p_frame->frameID;
}

vector<unsigned int> KageFrameset::raiseSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->raiseSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->raiseSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameset::lowerSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->lowerSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->lowerSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameset::raiseToTopSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->raiseToTopSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->raiseToTopSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameset::lowerToBottomSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->lowerToBottomSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->lowerToBottomSelectedShape(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageFrameset::groupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->groupSelectedShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->groupSelectedShapes(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
vector<unsigned int> KageFrameset::ungroupSelectedShapes(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->ungroupSelectedShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->ungroupSelectedShapes(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

vector<unsigned int> KageFrameset::duplicateShapes(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->duplicateShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->duplicateShapes(p_selectedShapes);
			}
		}
	}
	vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

bool KageFrameset::flipHorizontalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->flipHorizontalSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->flipHorizontalSelectedShape(p_selectedShapes);
			}
		}
	}
	
	return false;
}
bool KageFrameset::flipVerticalSelectedShape(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return doFlipVerticalSelectedShapeOn(_currentFrameIndex, p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return doFlipVerticalSelectedShapeOn(_currentFrameIndex, p_selectedShapes);
			}
		}
	}
	
	return false;
}
	bool KageFrameset::doFlipVerticalSelectedShapeOn(unsigned int p_frameIndex, vector<unsigned int> p_selectedShapes) {
		if (       _frames[p_frameIndex]->getExtension() == KageFrame::EXTENSION_NOT
				|| _frames[p_frameIndex]->getExtension() == KageFrame::EXTENSION_START) {
			return _frames[p_frameIndex]->flipVerticalSelectedShape(p_selectedShapes);
		} else {
			return doFlipVerticalSelectedShapeOnExtendedFrame(p_frameIndex-1, p_selectedShapes);
		}
	}
	bool KageFrameset::doFlipVerticalSelectedShapeOnExtendedFrame(unsigned int p_frameIndex, vector<unsigned int> p_selectedShapes) {
		while (p_frameIndex > 0) {
			if (       _frames[p_frameIndex]->getExtension() == KageFrame::EXTENSION_NOT
					|| _frames[p_frameIndex]->getExtension() == KageFrame::EXTENSION_START) {
				return _frames[p_frameIndex]->flipVerticalSelectedShape(p_selectedShapes);
			} else {
				--p_frameIndex;
			}
		}
		return false;
	}


bool KageFrameset::addDataToFrame(VectorDataManager p_vectorsData) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		_frames[_currentFrameIndex]->addDataToFrame(p_vectorsData);
		return true;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				_frames[i]->addDataToFrame(p_vectorsData);
				return true;
			}
		}
	}
	
	return false;
}

bool KageFrameset::setFrameData(VectorDataManager p_vectorsData) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		_frames[_currentFrameIndex]->setFrameData(p_vectorsData.clone());
		return true;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				_frames[i]->setFrameData(p_vectorsData.clone());
				return true;
			}
		}
	}
	
	return false;
}

VectorDataManager KageFrameset::getFrameData() {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->getFrameData();
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->getFrameData();
			}
		}
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}
VectorDataManager KageFrameset::getFrameDataAt(unsigned int p_frame) {
	if (p_frame > 0 && p_frame <= getFrameCount()) {
		--p_frame; //layer now becomes Frame Index
		return _frames[p_frame]->getFrameData();
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}
VectorDataManager KageFrameset::getPreviousFrameData(unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			return _frames[i-1]->getFrameData();
		}
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}

bool KageFrameset::setFrameDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			_frames[i-1]->setFrameData(p_vectorsData);
			return true;
		}
	}
	
	return false;
}

bool KageFrameset::addDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			_frames[i-1]->addDataToFrame(p_vectorsData);
			return true;
		}
	}
	
	return false;
}

bool KageFrameset::switchToPreviousFrame() {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == _currentFrameID) {
			///index + 1 = becomes frameNumber for current. then -1 becomes frameNumber of Previous Frame; thus `setCurrentFrame(i)'
			setCurrentFrame(i);
			focusFrame(i);
			return true;
		}
	}
	
	return false;
}

bool KageFrameset::switchToPreviousFrame(unsigned int p_frameID) {
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			///index + 1 = becomes frameNumber for current. then -1 becomes frameNumber of Previous Frame; thus `setCurrentFrame(i)'
			setCurrentFrame(i);
			focusFrame(i);
			return true;
		}
	}
	
	return false;
}

bool KageFrameset::switchToNextFrame() {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == _currentFrameID) {
			///index + 1 = becomes frameNumber for current. then +1 becomes frameNumber of Next Frame
			setCurrentFrame(i+2);
			focusFrame(i+2);
			return true;
		}
	}
	
	return false;
}

bool KageFrameset::switchToNextFrame(unsigned int p_frameID) {
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			///index + 1 = becomes frameNumber for current. then +1 becomes frameNumber of Next Frame
			setCurrentFrame(i+2);
			focusFrame(i+2);
			return true;
		}
	}
	
	return false;
}

bool KageFrameset::isCurrentFrame(unsigned int p_frameID) {
	if (_frames[_currentFrameIndex]->frameID == p_frameID
			&& _currentFrameID == p_frameID) {
		return true;
	}
	
	return false;
}

bool KageFrameset::canReUseNextFrame() {
	if (_currentFrameIndex+1 == _frames.size()) {
		return false;
	} else if (_frames[_currentFrameIndex+1]->isEmpty() == false
			&& _frames[_currentFrameIndex+1]->getExtension() == KageFrame::EXTENSION_NOT) {
		return false;
	} else if (_frames[_currentFrameIndex+1]->isEmpty() == false
			&& _frames[_currentFrameIndex+1]->getExtension() == KageFrame::EXTENSION_START) {
		return false;
	}
	
	return true;
}

/**
 * For use of Kage when loading KSF file
 * \param p_extension if start/mid/end of frame extension
 */
void KageFrameset::setFrameExtension(KageFrame::extension p_extension) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		_frames[_currentFrameIndex]->setExtension(p_extension);
		return;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_frames[_currentFrameIndex]->setExtension(p_extension);
				return;
			}
		}
	}
}
