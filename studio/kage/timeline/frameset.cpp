/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2020  Mj Mendoza IV
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.  Or, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "frameset.h"
#include "framesmanager.h"

#include "../../kage.h"

KageFrameset::KageFrameset(KageFramesetManager *p_fsm, unsigned int p_layerID, unsigned int p_frameCount) {
	pack_start(frame_padding, Gtk::PACK_SHRINK);
	fsm = p_fsm;
	layerID = p_layerID;
	frameCtr = 0;
	for (unsigned int i = 0; i < p_frameCount; ++i) {
		addFrame();
	}
	if (KageFramesetManager::LOADING_MODE == false && p_frameCount > 1) {
		_frames[0]->setExtension(KageFrame::EXTENSION_START);
		for (unsigned int i = 1; i < p_frameCount; ++i) {
			_frames[i]->setExtension(KageFrame::EXTENSION_MID);
		}
		_frames[p_frameCount-1]->setExtension(KageFrame::EXTENSION_END);
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
 * If KageFrame is NULL or Last Frame (but not Frame-index0), remove Frame Widget, else, do below steps.
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
				unsigned int l_tween = _frames[i+1]->getTween();
				if (i > 0) {
					switchToNextFrame();
						l_frameData = getFrameData().clone();
					l_extensionPrevious = _frames[i-1]->getExtension();
					l_extensionNext     = _frames[i+1]->getExtension();
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
						if (       l_extensionNext == KageFrame::EXTENSION_NOT
								|| l_extensionNext == KageFrame::EXTENSION_START) {
							_frames[i-1]->setExtension(KageFrame::EXTENSION_NOT);
						} else {//if (l_extensionNext == KageFrame::EXTENSION_MID
								//&& l_extensionNext == KageFrame::EXTENSION_END) {
						  //keep Previous
						}
						//simply copy Next Frame to Current
						KageFrame::extension l_temp = _frames[i+1]->getExtension();
						_frames[i  ]->setExtension(l_temp);
					} else if (l_extensionPrevious == KageFrame::EXTENSION_MID) {
						if (       l_extensionNext == KageFrame::EXTENSION_NOT
								|| l_extensionNext == KageFrame::EXTENSION_START) {
							_frames[i-1]->setExtension(KageFrame::EXTENSION_END);
						} else {//if (l_extensionNext == KageFrame::EXTENSION_MID
								//|| l_extensionNext == KageFrame::EXTENSION_END) {
							//keep Previous
						}
						//simply copy Next Frame to Current
						KageFrame::extension l_temp = _frames[i+1]->getExtension();
						_frames[i  ]->setExtension(l_temp);
					} else if (l_extensionPrevious == KageFrame::EXTENSION_END) {
						//keep Previous
						if (       l_extensionNext == KageFrame::EXTENSION_NOT
								|| l_extensionNext == KageFrame::EXTENSION_END) {
							_frames[i  ]->setExtension(KageFrame::EXTENSION_NOT);
						} else if (l_extensionNext == KageFrame::EXTENSION_START
								|| l_extensionNext == KageFrame::EXTENSION_MID) {
							_frames[i  ]->setExtension(KageFrame::EXTENSION_START);
						}
					}
				} else {
					l_extensionNext = _frames[i+1]->getExtension();
					_frames[i]->setExtension(l_extensionNext);
				}
				
				//do copy Next Frame content to This Frame
					switchToPreviousFrame();
					_frames[i]->forceSetTween(l_tween);
					if (l_tween == false ||
						l_tween == true && _frames[i+1]->getExtension() == KageFrame::EXTENSION_START) {
						setFrameData(l_frameData.clone());
					}
				switchToNextFrame();
				
				//loop to the rest of the frameset
				KageFrame::extension l_extension;
				for (unsigned int j = i+1; j < getFrameCount()-1; ++j) {
					l_tween     = _frames[j+1]->getTween();
					l_extension = _frames[j+1]->getExtension();
					_frames[j  ]->setExtension(l_extension);
					switchToNextFrame();
						l_frameData = getFrameData().clone();
					switchToPreviousFrame();
						_frames[j]->forceSetTween(l_tween);
						if (l_tween == false ||
							l_tween == true && _frames[j+1]->getExtension() == KageFrame::EXTENSION_START) {
							setFrameData(l_frameData.clone());
						}
					switchToNextFrame(); //should it be here?
				}
				break;
			} else {
				_frames[i-1]->forceSetTween(false);
				_frames[i-1]->setNull(true); //when willl Frame be NULL if not the Last Frame
				break;
			}
		}
		_frames[getFrameCount()-1]->forceSetTween(false);
		_frames[getFrameCount()-1]->setNull(true);
		_frames[getFrameCount()-1]->setExtension(KageFrame::EXTENSION_NOT); //see issue #120 -- https://sourceforge.net/p/kage/tickets/120/
		setCurrentFrame(l_currentFrame);
	} else if (getFrameCount() == 1) {
		VectorDataManager l_frameData;
		setFrameData(l_frameData);
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
	unsigned int l_frameCount = getFrameCount();
	_frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*_frames.back(), Gtk::PACK_SHRINK);
	
	if (KageFramesetManager::LOADING_MODE == true) {
		return;
	}
	
	if (l_currentFrame == l_frameCount) {
		KageFrame::extension l_extension = _frames[_frames.size()-2]->getExtension();
		unsigned int l_tween = _frames[_frames.size()-2]->getTween();
		_frames[_frames.size()-1]->forceSetTween(l_tween);
		if (l_extension == KageFrame::EXTENSION_NOT) {
			_frames[_frames.size()-2]->setExtension(KageFrame::EXTENSION_START);
		} else if (l_extension == KageFrame::EXTENSION_END) {
			_frames[_frames.size()-2]->setExtension(KageFrame::EXTENSION_MID);
		}
		_frames[_frames.size()-1]->setExtension(KageFrame::EXTENSION_END);
	} else if (l_currentFrame < l_frameCount) {
		unsigned int l_frameIndex = getFrameCount();
		while (l_frameIndex > l_currentFrame+1) {
			setCurrentFrame(l_frameIndex);
			moveToLeft();
			--l_frameIndex;
		}
		
		KageFrame::extension l_extension = _frames[l_currentFrame-1]->getExtension();
		unsigned int l_tween = _frames[l_currentFrame-1]->getTween();
		_frames[l_currentFrame]->forceSetTween(l_tween);
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

bool KageFrameset::recenterRotationPoint(vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _frames[_currentFrameIndex]->recenterRotationPoint(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return _frames[i]->recenterRotationPoint(p_selectedShapes);
			}
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

VectorDataManager KageFrameset::getFrameTweenData(unsigned int p_frameIndex) {
	unsigned int             l_tween = _frames[p_frameIndex]->getTween();
	KageFrame::extension l_extension = _frames[p_frameIndex]->getExtension();
	
	if (       l_tween > 0
			&& (l_extension == KageFrame::EXTENSION_MID
			||  l_extension == KageFrame::EXTENSION_END)) {
		unsigned int l_tweenHead = UINT_MAX;
		unsigned int l_tweenTail = UINT_MAX;
		
		for (unsigned int j = p_frameIndex-1; j >= 0 && j != UINT_MAX; --j) {
			if (_frames[j]->getExtension() == KageFrame::EXTENSION_START) {
				l_tweenHead = j;
				break;
			}
		}
		for (unsigned int j = p_frameIndex+1; j < getFrameCount(); ++j) {
			l_extension = _frames[j]->getExtension();
			if (	   l_extension == KageFrame::EXTENSION_START
					|| l_extension == KageFrame::EXTENSION_NOT) {
				l_tweenTail = j;
				break;
			}
		}
		VectorDataManager l_dataHead;
		VectorDataManager l_dataTail;
		if (l_tweenHead != UINT_MAX) {
			l_dataHead = _frames[l_tweenHead]->getFrameData();
		}
		if (l_tweenTail != UINT_MAX) {
			l_dataTail = _frames[l_tweenTail]->getFrameData();
		}
		vector<VectorData> l_vHead = l_dataHead.getVectorData();
		vector<VectorData> l_vTail = l_dataTail.getVectorData();
		unsigned int l_tweenDistance    =  l_tweenTail - l_tweenHead;
		unsigned int l_tweenInterpolate = p_frameIndex - l_tweenHead;
		double l_progress = (double) l_tweenInterpolate / (double) l_tweenDistance;
			if (p_frameIndex < l_tweenHead) l_progress = 0.0;
			if (p_frameIndex > l_tweenTail) l_progress = 1.0;
		
		if (l_vHead.size() == l_vTail.size()) {
			unsigned int l_tweenX = l_tween / 10;
			unsigned int l_tweenY = l_tween - (l_tweenX * 10);
			for (unsigned int j = 0; j < l_vHead.size(); ++j) {
				if (       (l_vHead[j].vectorType == VectorData::TYPE_INIT && l_vTail[j].vectorType == VectorData::TYPE_INIT)
						|| (l_vHead[j].vectorType == VectorData::TYPE_MOVE && l_vTail[j].vectorType == VectorData::TYPE_MOVE)) {
					double l_xDistance = l_vTail[j].points[0].x - l_vHead[j].points[0].x;
					if (l_tweenX == 1) l_vHead[j].points[0].x += l_xDistance * l_progress;
					if (l_tweenX == 2) l_vHead[j].points[0].x += l_xDistance * pow(0.66667, l_tweenDistance-l_tweenInterpolate);
					if (l_tweenX == 3) {
						double l_xMult = l_xDistance * 0.3334;
						for (unsigned int l_tweenCtr = 1; l_tweenCtr < l_tweenInterpolate; ++l_tweenCtr) {
							l_xMult = l_xMult * 0.6667;
						}
						l_vHead[j].points[0].x += (l_xDistance - l_xMult);
					}
					double l_yDistance = l_vTail[j].points[0].y - l_vHead[j].points[0].y;
					if (l_tweenY == 1) l_vHead[j].points[0].y += l_yDistance * l_progress;
					if (l_tweenY == 2) l_vHead[j].points[0].y += l_yDistance * pow(0.66667, l_tweenDistance-l_tweenInterpolate);
					if (l_tweenY == 3) {
						double l_yMult = l_yDistance * 0.3334;
						for (unsigned int l_tweenCtr = 1; l_tweenCtr < l_tweenInterpolate; ++l_tweenCtr) {
							l_yMult = l_yMult * 0.6667;
						}
						l_vHead[j].points[0].y += (l_yDistance - l_yMult);
					}
				} else if ((l_vHead[j].vectorType == VectorData::TYPE_CURVE_CUBIC     && l_vTail[j].vectorType == VectorData::TYPE_CURVE_CUBIC)
						|| (l_vHead[j].vectorType == VectorData::TYPE_CURVE_QUADRATIC && l_vTail[j].vectorType == VectorData::TYPE_CURVE_QUADRATIC)) {
					for (unsigned int k = 0; k < 3; ++k) {
						double l_xDistance = l_vTail[j].points[k].x - l_vHead[j].points[k].x;
						if (l_tweenX == 1) l_vHead[j].points[k].x += l_xDistance * l_progress;
						if (l_tweenX == 2) l_vHead[j].points[k].x += l_xDistance * pow(0.66667, l_tweenDistance-l_tweenInterpolate);
						if (l_tweenX == 3) {
							double l_xMult = l_xDistance * 0.3334;
							for (unsigned int l_tweenCtr = 1; l_tweenCtr < l_tweenInterpolate; ++l_tweenCtr) {
								l_xMult = l_xMult * 0.6667;
							}
							l_vHead[j].points[k].x += (l_xDistance - l_xMult);
						}
						double l_yDistance = l_vTail[j].points[k].y - l_vHead[j].points[k].y;
						if (l_tweenY == 1) l_vHead[j].points[k].y += l_yDistance * l_progress;
						if (l_tweenY == 2) l_vHead[j].points[k].y += l_yDistance * pow(0.66667, l_tweenDistance-l_tweenInterpolate);
						if (l_tweenY == 3) {
							double l_yMult = l_yDistance * 0.3334;
							for (unsigned int l_tweenCtr = 1; l_tweenCtr < l_tweenInterpolate; ++l_tweenCtr) {
								l_yMult = l_yMult * 0.6667;
							}
							l_vHead[j].points[k].y += (l_yDistance - l_yMult);
						}
					}
				} else if ((l_vHead[j].vectorType == VectorData::TYPE_FILL   && l_vTail[j].vectorType == VectorData::TYPE_FILL)
						|| (l_vHead[j].vectorType == VectorData::TYPE_STROKE && l_vTail[j].vectorType == VectorData::TYPE_STROKE)) {
					ColorData l_fillColor;
					if (l_vTail[j].fillColor.getR() == l_vHead[j].fillColor.getR()) {
						l_fillColor.setR(l_vTail[j].fillColor.getR());
					} else {
						if (l_tweenX == 1) l_fillColor.setR((int)(l_vTail[j].fillColor.getR() - l_vHead[j].fillColor.getR()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].fillColor.getR());
					}
					if (l_vTail[j].fillColor.getG() == l_vHead[j].fillColor.getG()) {
						l_fillColor.setG(l_vTail[j].fillColor.getG());
					} else {
						if (l_tweenX == 1) l_fillColor.setG((int)(l_vTail[j].fillColor.getG() - l_vHead[j].fillColor.getG()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].fillColor.getG());
					}
					if (l_vTail[j].fillColor.getB() == l_vHead[j].fillColor.getB()) {
						l_fillColor.setB(l_vTail[j].fillColor.getB());
					} else {
						if (l_tweenX == 1) l_fillColor.setB((int)(l_vTail[j].fillColor.getB() - l_vHead[j].fillColor.getB()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].fillColor.getB());
					}
					if (l_vTail[j].fillColor.getA() == l_vHead[j].fillColor.getA()) {
						l_fillColor.setA(l_vTail[j].fillColor.getA());
					} else {
						if (l_tweenX == 1) l_fillColor.setA((int)(l_vTail[j].fillColor.getA() - l_vHead[j].fillColor.getA()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].fillColor.getA());
					}
					
					StrokeColorData l_strokeColor;
					if (l_vTail[j].stroke.getR() == l_vHead[j].stroke.getR()) {
						l_strokeColor.setR(l_vTail[j].stroke.getR());
					} else {
						if (l_tweenX == 1) l_strokeColor.setR((int)(l_vTail[j].stroke.getR() - l_vHead[j].stroke.getR()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].stroke.getR());
					}
					if (l_vTail[j].stroke.getG() == l_vHead[j].stroke.getG()) {
						l_strokeColor.setG(l_vTail[j].stroke.getG());
					} else {
						if (l_tweenX == 1) l_strokeColor.setG((int)(l_vTail[j].stroke.getG() - l_vHead[j].stroke.getG()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].stroke.getG());
					}
					if (l_vTail[j].stroke.getB() == l_vHead[j].stroke.getB()) {
						l_strokeColor.setB(l_vTail[j].stroke.getB());
					} else {
						if (l_tweenX == 1) l_strokeColor.setB((int)(l_vTail[j].stroke.getB() - l_vHead[j].stroke.getB()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].stroke.getB());
					}
					if (l_vTail[j].stroke.getA() == l_vHead[j].stroke.getA()) {
						l_strokeColor.setA(l_vTail[j].stroke.getA());
					} else {
						if (l_tweenX == 1) l_strokeColor.setA((int)(l_vTail[j].stroke.getA() - l_vHead[j].stroke.getA()) * (double) l_tweenInterpolate / (double) l_tweenDistance + l_vHead[j].stroke.getA());
					}
					
					if (l_vHead[j].vectorType == VectorData::TYPE_STROKE) {
						if (l_vTail[j].stroke.getThickness() == l_vHead[j].stroke.getThickness()) {
							l_strokeColor.setThickness(l_vTail[j].stroke.getThickness());
						} else {
							if (l_tweenX == 1) l_strokeColor.setThickness((l_vTail[j].stroke.getThickness() - l_vHead[j].stroke.getThickness()) * l_tweenInterpolate / l_tweenDistance + l_vHead[j].stroke.getThickness());
						}
					}
					
					l_vHead[j].fillColor = l_fillColor.clone();
					l_vHead[j].stroke    = l_strokeColor.clone();
				}
			}
		}
		l_dataHead.setVectorData(l_vHead);
		return l_dataHead;
	} else {
		return _frames[p_frameIndex]->getFrameData();
	}
}

VectorDataManager KageFrameset::getFrameData() {
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return getFrameTweenData(_currentFrameIndex);
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				return getFrameTweenData(i);
			}
		}
	}
	cout << "getFrameData is returning empty";
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

bool KageFrameset::setPreviousFrameTween(unsigned int p_frameID, unsigned int p_tween) {
	cout << " KageFrameset::setPreviousFrameTween() " << p_frameID << endl;
	for (unsigned int i = 1; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			_frames[i-1]->setTween(p_tween);
			return true;
		}
	}
	
	return false;
}
bool KageFrameset::setExtendedFrameTween(unsigned int p_frameID, unsigned int p_tween) {
	cout << " KageFrameset::setExtendedFrameTween() " << p_frameID << endl;
	for (unsigned int i = 0; i < getFrameCount(); ++i) {
		if (_frames[i]->frameID == p_frameID) {
			for (unsigned int j = i+1; j < getFrameCount(); ++j) {
				_frames[j]->forceSetTween(p_tween);
				if (_frames[j]->getExtension() == KageFrame::EXTENSION_END) {
					break;
				}
			}
			return true;
		}
	}
	
	return false;
}

bool KageFrameset::forceSetTween(unsigned int p_tween) {
	cout << " KageFrameset::forceSetTween() " << endl;
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		_frames[_currentFrameIndex]->forceSetTween(p_tween);
		return true;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				_frames[i]->forceSetTween(p_tween);
				return true;
			}
		}
	}
	
	return false;
}

bool KageFrameset::setTween(unsigned int p_tween) {
	cout << " KageFrameset::setTween() " << endl;
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		_frames[_currentFrameIndex]->setTween(p_tween);
		return true;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				_frames[i]->setTween(p_tween);
				return true;
			}
		}
	}
	
	return false;
}
unsigned int KageFrameset::getTween() {
	Kage::timestamp_IN(); std::cout << " KageFrameset::getTween() " << endl;
	if (_currentFrameIndex < getFrameCount() && _frames[_currentFrameIndex]->frameID == _currentFrameID) {
		unsigned int l_ret = _frames[_currentFrameIndex]->getTween();
		Kage::timestamp_OUT();
		return l_ret;
	} else {
		for (unsigned int i = 0; i < getFrameCount(); ++i) {
			if (_frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = _frames[i]->frameID;
				unsigned int l_ret = _frames[i]->getTween();
				Kage::timestamp_OUT();
				return l_ret;
			}
		}
	}
	
	Kage::timestamp_OUT();
	
	return 0;
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
