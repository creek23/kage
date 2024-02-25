/*
 * Kage Studio - a simple free and open source vector-based 2D animation software
 * Copyright (C) 2011~2024  Mj Mendoza IV
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

#include "layer.h"
//#include "scene.h"
#include "../../kage.h"

KageLayer::KageLayer(KageScene *p_scene, unsigned int p_layerID, unsigned int p_frameCount) {
	_scene = p_scene;
	layerID = p_layerID;
	setSelected(false);
	setVisible(true);
	setLock(false);	
	setLabel("Layer " + StringHelper::unsignedIntegerToString(p_layerID));
	frameCtr = 0;
	for (unsigned int i = 0; i < p_frameCount; ++i) {
		addFrame();
	}
	if (KageScene::LOADING_MODE == false && p_frameCount > 1) {
		Frames[0]->setExtension(KageFrame::extension::EXTENSION_START);
		for (unsigned int i = 1; i < p_frameCount; ++i) {
			Frames[i]->setExtension(KageFrame::extension::EXTENSION_MID);
		}
		Frames[p_frameCount-1]->setExtension(KageFrame::extension::EXTENSION_END);
	}
}

KageLayer KageLayer::operator=(const KageLayer &p_layer) {
	if (this == &p_layer) {
		return *this;
	}
	KageScene::LOADING_MODE = true;

	this->layerID            = p_layer.layerID;
	this->_currentFrameIndex = p_layer._currentFrameIndex;
	this->_activeFrame       = p_layer._activeFrame;
	this->_selected          = p_layer._selected;
	this->_visible           = p_layer._visible;
	this->_lock              = p_layer._lock;
	this->_label             = p_layer._label;
	this->frameCtr           = p_layer.frameCtr;
	this->_currentFrameID    = p_layer._currentFrameID;

		this->removeAllFrames();
		
		for (unsigned int l_frameIndex = 0; l_frameIndex < p_layer.Frames.size(); ++l_frameIndex) {
			KageScene::LOADING_MODE = true;
			_framePtr = new KageFrame(this, frameCtr);
			this->Frames.push_back(_framePtr);
			*(this->Frames[this->Frames.size()-1]) = *(p_layer.Frames[l_frameIndex]);
		}
	
	this->frameCtr           = p_layer.frameCtr;
	this->_currentFrameID    = p_layer._currentFrameID;

	KageScene::LOADING_MODE = false;
	//std::cout << "\t\tKageLayer KageLayer::operator= >> " << this << "\t" << this->layerID << " "<< this->_currentFrameID << std::endl;
	return *this;
}

KageLayer::~KageLayer() {
	//TODO: revisit
	/*
	for (unsigned int i = Frames.size()-1; i > 0 && i != UINT_MAX; --i) {
		delete Frames[i];
		Frames[i] = NULL;
	}
	Frames.clear();
	_scene = NULL;*/
}

bool KageLayer::addFrame() {
	++frameCtr;
	_framePtr = new KageFrame(this, frameCtr);
	Frames.push_back(_framePtr);
	
	if (KageScene::LOADING_MODE == true) {
		return true;
	}
	unsigned int l_currentFrame = getCurrentFrame();
	
	if (l_currentFrame == Frames.size()) {
		//when does currentFrame equal to Frames size when new frame is added?!?
		if (Frames.size() > 1) {
			KageFrame::extension l_extension = Frames[Frames.size()-2]->getExtension();
			if (l_extension == KageFrame::extension::EXTENSION_NOT) {
				//keep
			} else if (l_extension == KageFrame::extension::EXTENSION_END) {
				//keep
			}
			//what happens if previous is extension?
		}
	} else if (l_currentFrame < Frames.size()) {
		unsigned int l_frameIndex = Frames.size();
		while (l_frameIndex > l_currentFrame+1) {
			setCurrentFrame(l_frameIndex, false);
			moveToLeft();
			--l_frameIndex;
		}
		
		if (Frames.size() > 1) {
			KageFrame::extension l_extension = Frames[l_currentFrame-1]->getExtension();
			if (l_extension == KageFrame::extension::EXTENSION_NOT) {
				//keep
			} else if (l_extension == KageFrame::extension::EXTENSION_START) {
				Frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_NOT);
				l_extension = Frames[l_currentFrame+1]->getExtension();
				if (       l_extension == KageFrame::extension::EXTENSION_MID
						|| l_extension == KageFrame::extension::EXTENSION_END) {
					Frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_START);
				}
			} else if (l_extension == KageFrame::extension::EXTENSION_MID) {
				Frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_END);
				l_extension = Frames[l_currentFrame+1]->getExtension();
				if (       l_extension == KageFrame::extension::EXTENSION_MID
						|| l_extension == KageFrame::extension::EXTENSION_END) {
					Frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_START);
				}
			} else if (l_extension == KageFrame::extension::EXTENSION_END) {
				//keep
			}
			setCurrentFrame(l_currentFrame, false); //restore Current Frame because moveToLeft changed Current Frame
		}
	}
	
	return true;
}

void KageLayer::duplicateFrame() {
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	
	VectorDataManager l_frameData = getFrameData().clone();
		switchToNextFrame();
		
		unsigned int l_currentFrame = getCurrentFrame()-1;
		if (Frames.size() > 1) {
			KageFrame::extension l_extension = Frames[l_currentFrame-1]->getExtension();
			if (l_extension == KageFrame::extension::EXTENSION_NOT) {
				//keep
	//			l_extension = Frames[l_currentFrame+1]->getExtension();
	//			if (       l_extension == KageFrame::extension::EXTENSION_MID
	//					|| l_extension == KageFrame::extension::EXTENSION_END) {
	//				Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_START);
	//			}
			} else if (l_extension == KageFrame::extension::EXTENSION_START) {
				Frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_NOT);
				if (l_currentFrame+1 < Frames.size()) {
					l_extension = Frames[l_currentFrame+1]->getExtension();
					if (       l_extension == KageFrame::extension::EXTENSION_MID
							|| l_extension == KageFrame::extension::EXTENSION_END) {
						Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_START);
					} else if (l_extension == KageFrame::extension::EXTENSION_NOT
							|| l_extension == KageFrame::extension::EXTENSION_START) {
						Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_NOT);
					}
				} else {
					Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_NOT);
				}
			} else if (l_extension == KageFrame::extension::EXTENSION_MID) {
				Frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_END);
				if (l_currentFrame+1 < Frames.size()) {
					l_extension = Frames[l_currentFrame+1]->getExtension();
					if (       l_extension == KageFrame::extension::EXTENSION_MID
							|| l_extension == KageFrame::extension::EXTENSION_END) {
						Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_START);
					} else if (l_extension == KageFrame::extension::EXTENSION_NOT
							|| l_extension == KageFrame::extension::EXTENSION_START) {
						Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_NOT);
					}
				} else {
					Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_NOT);
				}
			} else if (l_extension == KageFrame::extension::EXTENSION_END) {
				//keep previous
				//std::cout << " at END with " << Frames[l_currentFrame]->getExtension() << std::endl;
				Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_NOT);
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
bool KageLayer::removeFrame() {
	unsigned int l_currentFrame = getCurrentFrame();
	unsigned int l_currentFrameIndex = l_currentFrame-1;
	if (Frames[l_currentFrameIndex]->isNull() == true
			|| (l_currentFrame == Frames.size() && l_currentFrame > 1)) {
		if (Frames.size() > 1) {
			Frames.erase (Frames.begin()+Frames.size()-1);
			setCurrentFrame(--l_currentFrame, false);
			return true;
		} else {
			std::cout << "last frame? do nothing" << std::endl;
		}
	} else if (Frames.size() > 1) {
		KageFrame::extension l_extensionPrevious;
		KageFrame::extension l_extensionNext;
		VectorDataManager l_frameData;
		for (unsigned int i = l_currentFrameIndex; i < Frames.size()-1; ++i) {
			if (Frames[i]->isNull() == false) {
				unsigned int l_tween = Frames[i+1]->getTween();
				if (i > 0) {
					switchToNextFrame();
						//l_frameData = getFrameData().clone();
						l_frameData.setVectorData(getFrameData().getVectorData());
					l_extensionPrevious = Frames[i-1]->getExtension();
					l_extensionNext     = Frames[i+1]->getExtension();
					if (l_extensionPrevious == KageFrame::extension::EXTENSION_NOT) {
						//keep Previous
						if (       l_extensionNext == KageFrame::extension::EXTENSION_START
								&& l_extensionNext == KageFrame::extension::EXTENSION_MID) {
							Frames[i  ]->setExtension(KageFrame::extension::EXTENSION_START);
						} else if (l_extensionNext == KageFrame::extension::EXTENSION_END
								&& l_extensionNext == KageFrame::extension::EXTENSION_NOT) {
							Frames[i  ]->setExtension(KageFrame::extension::EXTENSION_NOT);
						}
					} else if (l_extensionPrevious == KageFrame::extension::EXTENSION_START) {
						if (       l_extensionNext == KageFrame::extension::EXTENSION_NOT
								|| l_extensionNext == KageFrame::extension::EXTENSION_START) {
							Frames[i-1]->setExtension(KageFrame::extension::EXTENSION_NOT);
						} else {//if (l_extensionNext == KageFrame::extension::EXTENSION_MID
								//&& l_extensionNext == KageFrame::extension::EXTENSION_END) {
						  //keep Previous
						}
						//simply copy Next Frame to Current
						KageFrame::extension l_temp = Frames[i+1]->getExtension();
						Frames[i  ]->setExtension(l_temp);
					} else if (l_extensionPrevious == KageFrame::extension::EXTENSION_MID) {
						if (       l_extensionNext == KageFrame::extension::EXTENSION_NOT
								|| l_extensionNext == KageFrame::extension::EXTENSION_START) {
							Frames[i-1]->setExtension(KageFrame::extension::EXTENSION_END);
						} else {//if (l_extensionNext == KageFrame::extension::EXTENSION_MID
								//|| l_extensionNext == KageFrame::extension::EXTENSION_END) {
							//keep Previous
						}
						//simply copy Next Frame to Current
						KageFrame::extension l_temp = Frames[i+1]->getExtension();
						Frames[i  ]->setExtension(l_temp);
					} else if (l_extensionPrevious == KageFrame::extension::EXTENSION_END) {
						//keep Previous
						if (       l_extensionNext == KageFrame::extension::EXTENSION_NOT
								|| l_extensionNext == KageFrame::extension::EXTENSION_END) {
							Frames[i  ]->setExtension(KageFrame::extension::EXTENSION_NOT);
						} else if (l_extensionNext == KageFrame::extension::EXTENSION_START
								|| l_extensionNext == KageFrame::extension::EXTENSION_MID) {
							Frames[i  ]->setExtension(KageFrame::extension::EXTENSION_START);
						}
					}
				} else {
					l_extensionNext = Frames[i+1]->getExtension();
					Frames[i]->setExtension(l_extensionNext);
				}
				
				//do copy Next Frame content to This Frame
					switchToPreviousFrame();
					Frames[i]->forceSetTween(l_tween);
					if (l_tween == false ||
						l_tween == true && Frames[i+1]->getExtension() == KageFrame::extension::EXTENSION_START) {
						setFrameData(l_frameData.clone());
					}
				switchToNextFrame();
				
				//loop to the rest of the frameset
				KageFrame::extension l_extension;
				for (unsigned int j = i+1; j < Frames.size()-1; ++j) {
					l_tween     = Frames[j+1]->getTween();
					l_extension = Frames[j+1]->getExtension();
					Frames[j  ]->setExtension(l_extension);
					switchToNextFrame();
						//l_frameData = getFrameData().clone();
						l_frameData.setVectorData(getFrameData().getVectorData());
					switchToPreviousFrame();
						Frames[j]->forceSetTween(l_tween);
						if (l_tween == false ||
							l_tween == true && Frames[j+1]->getExtension() == KageFrame::extension::EXTENSION_START) {
							setFrameData(l_frameData.clone());
						}
					switchToNextFrame(); //should it be here?
				}
				break;
			} else {
				Frames[i-1]->forceSetTween(false);
				Frames[i-1]->setNull(true); //when will Frame be NULL if not the Last Frame
				break;
			}
		}
		Frames[Frames.size()-1]->forceSetTween(false);
		Frames[Frames.size()-1]->setNull(true);
		Frames[Frames.size()-1]->setExtension(KageFrame::extension::EXTENSION_NOT); //see issue #120 -- https://sourceforge.net/p/kage/tickets/120/
		setCurrentFrame(l_currentFrame, false);
	} else if (Frames.size() == 1) {
		VectorDataManager l_frameData;
		setFrameData(l_frameData);
	}
	
	return false;
}

bool KageLayer::moveToLeft() {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		if (_currentFrameIndex > 0) {
			std::swap(Frames[_currentFrameIndex], Frames[_currentFrameIndex-1]);
			--_currentFrameIndex;
			return true;
		}
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				if (_currentFrameIndex > 0) {
					std::swap(Frames[_currentFrameIndex], Frames[_currentFrameIndex-1]);
					--_currentFrameIndex;
					return true;
				}
				break;
			}
		}
	}
	return false;
}
bool KageLayer::moveToLeftAt(unsigned int p_frame) {
	bool l_return = false;
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		while (_currentFrameIndex > p_frame) {
			l_return = moveToLeft();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
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
bool KageLayer::moveToLeftMost() {
	bool l_return = false;
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		while (_currentFrameIndex > 0) {
			l_return = moveToLeft();
		}
		return l_return;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
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

void KageLayer::extendFrame() {
	++frameCtr;
	unsigned int l_currentFrame = getCurrentFrame();
	unsigned int l_frameCount = Frames.size();
	_framePtr = new KageFrame(this, frameCtr);
	Frames.push_back(_framePtr);
	
	if (KageScene::LOADING_MODE == true) {
		return;
	}
	
	if (l_currentFrame == l_frameCount) {
		KageFrame::extension l_extension = Frames[Frames.size()-2]->getExtension();
		unsigned int l_tween = Frames[Frames.size()-2]->getTween();
		Frames[Frames.size()-1]->forceSetTween(l_tween);
		if (l_extension == KageFrame::extension::EXTENSION_NOT) {
			Frames[Frames.size()-2]->setExtension(KageFrame::extension::EXTENSION_START);
		} else if (l_extension == KageFrame::extension::EXTENSION_END) {
			Frames[Frames.size()-2]->setExtension(KageFrame::extension::EXTENSION_MID);
		}
		Frames[Frames.size()-1]->setExtension(KageFrame::extension::EXTENSION_END);
	} else if (l_currentFrame < l_frameCount) {
		unsigned int l_frameIndex = Frames.size();
		while (l_frameIndex > l_currentFrame+1) {
			setCurrentFrame(l_frameIndex, false);
			moveToLeft();
			--l_frameIndex;
		}
		
		KageFrame::extension l_extension = Frames[l_currentFrame-1]->getExtension();
		unsigned int l_tween = Frames[l_currentFrame-1]->getTween();
		Frames[l_currentFrame]->forceSetTween(l_tween);
		if (l_extension == KageFrame::extension::EXTENSION_NOT) {
			Frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_START);
			Frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_END);
		} else if (l_extension == KageFrame::extension::EXTENSION_START) {
			l_extension = Frames[l_currentFrame+1]->getExtension();
			if (       l_extension == KageFrame::extension::EXTENSION_NOT
					|| l_extension == KageFrame::extension::EXTENSION_START) {
				Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_END);
			} else if (l_extension == KageFrame::extension::EXTENSION_MID
					|| l_extension == KageFrame::extension::EXTENSION_END) {
				Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_MID);
			}
		} else if (l_extension == KageFrame::extension::EXTENSION_MID) {
			l_extension = Frames[l_currentFrame+1]->getExtension();
			if (       l_extension == KageFrame::extension::EXTENSION_NOT
					|| l_extension == KageFrame::extension::EXTENSION_START) {
				Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_END);
			} else if (l_extension == KageFrame::extension::EXTENSION_MID
					|| l_extension == KageFrame::extension::EXTENSION_END) {
				Frames[l_currentFrame]->setExtension(KageFrame::extension::EXTENSION_MID);
			}
		} else if (l_extension == KageFrame::extension::EXTENSION_END) {
			Frames[l_currentFrame-1]->setExtension(KageFrame::extension::EXTENSION_MID);
			Frames[l_currentFrame  ]->setExtension(KageFrame::extension::EXTENSION_END);
		}
		setCurrentFrame(l_currentFrame, false); //restore Current Frame because moveToLeft changed Current Frame
	}
}
static int FRAME_SIZE = 0; //who is using this?
bool KageLayer::removeAllFrames() {
	frameCtr = 0;
	FRAME_SIZE = Frames.size();
	for (unsigned int i = FRAME_SIZE-1; i >= 0 && i != UINT_MAX; --i) {
		if (Frames[i]) {
			delete Frames[i];
			Frames[i] = NULL;
		}
	}
	Frames.clear();
	FRAME_SIZE = Frames.size();
	
	return true;
}

unsigned int KageLayer::getID() {
	return layerID;
}

bool KageLayer::selectAllFrame(bool p_selectAll) {
	for (unsigned int i = 0; i < Frames.size(); ++i) {
		Frames[i]->setSelected(p_selectAll);
	}
	return true;
}

KageScene *KageLayer::getScene() {
	return _scene;
}

/** For use of Kage.  When a KageLayer is clicked, KageLayerManager will
 * call this function via Kage then sets currently active Frame along Layer
 * \param p_frame is Frame Number known to KageFrame(/Layer?)
 * \param p_addSelected add more selected Frame or not
 * \sa getCurrentFrame()
*/
void KageLayer::setCurrentFrame(unsigned int p_frame, bool p_addSelected) {
	unsigned int l_fromFrameIndex = 0;
	if (p_addSelected == true) {
		//get current frame to select range of frames
		if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
			l_fromFrameIndex = _currentFrameIndex;
		} else {
			for (unsigned int i = 0; i < Frames.size(); ++i) {
				if (Frames[i]->frameID == _currentFrameID) {
					l_fromFrameIndex = i;
					break;
				}
			}
		}
	} else {
		//clear previous
		for (unsigned int i = 0; i < Frames.size(); ++i) {
//			Frames[i]->setSelected(false);
			Frames[i]->setCurrent(false);
		}
	}
	
	unsigned int l_count = Frames.size();
	if (p_frame > l_count) {
		p_frame = l_count;
	}
	if (p_frame < 1) {
		p_frame = 1;
	}
	_currentFrameIndex = p_frame-1;
	_currentFrameID = Frames[_currentFrameIndex]->frameID;
//	Frames[_currentFrameIndex]->setSelected(true);
	Frames[_currentFrameIndex]->setCurrent(true);

	if (p_addSelected == true) {
		unsigned int l_start = 0;
		unsigned int l_stop = 0;
		if (_currentFrameIndex < l_fromFrameIndex) {
			l_start = _currentFrameIndex;
			l_stop = l_fromFrameIndex;
		} else {
			l_start = l_fromFrameIndex;
			l_stop = _currentFrameIndex;
		}
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (i >= l_start && i <= l_stop) {
				Frames[i]->setSelected(true);
			} else {
				Frames[i]->setSelected(false);
			}
		}
	}
}

unsigned int KageLayer::getCurrentFrame() {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return _currentFrameIndex+1;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				return i+1;
			}
		}
	}
	return 1;
}

KageFrame *KageLayer::getFrame() {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex];
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				return Frames[i];
			}
		}
	}
	return NULL;
}
KageFrame *KageLayer::getFrameAt(unsigned int p_frame) {
	if (p_frame < 1 || p_frame > Frames.size()) {
		return NULL;
	}
	
	return Frames[p_frame-1];
}

unsigned int KageLayer::getFrameNumberByID(unsigned int p_frameID) {
	for (unsigned int i = 0; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			return i+1;
		}
	}
	
	return 0;
}

/**
 * For use of KageScene when Frame is added or duplicated
 * 
 * \param p_frame is pointer to current KageFrame before Frame-add or Frame-duplicate
 */
void KageLayer::setSelected(KageFrame *p_frame) {
	_scene->selectAllLayerFrame(false);
	_scene->setCurrentLayerByID(p_frame->_layer->layerID);
	_scene->setCurrentFrameByID(p_frame->frameID);
	
	p_frame->setSelected(true);
	_currentFrameID = p_frame->frameID;
}

std::vector<unsigned int> KageLayer::raiseSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->raiseSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->raiseSelectedShape(p_selectedShapes);
			}
		}
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
std::vector<unsigned int> KageLayer::lowerSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->lowerSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->lowerSelectedShape(p_selectedShapes);
			}
		}
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
std::vector<unsigned int> KageLayer::raiseToTopSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->raiseToTopSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->raiseToTopSelectedShape(p_selectedShapes);
			}
		}
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
std::vector<unsigned int> KageLayer::lowerToBottomSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->lowerToBottomSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->lowerToBottomSelectedShape(p_selectedShapes);
			}
		}
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

std::vector<unsigned int> KageLayer::groupSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->groupSelectedShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->groupSelectedShapes(p_selectedShapes);
			}
		}
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}
std::vector<unsigned int> KageLayer::ungroupSelectedShapes(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->ungroupSelectedShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->ungroupSelectedShapes(p_selectedShapes);
			}
		}
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

std::vector<unsigned int> KageLayer::duplicateShapes(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->duplicateShapes(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->duplicateShapes(p_selectedShapes);
			}
		}
	}
	std::vector<unsigned int> l_nullReturn;
	return l_nullReturn;
}

bool KageLayer::flipHorizontalSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->flipHorizontalSelectedShape(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->flipHorizontalSelectedShape(p_selectedShapes);
			}
		}
	}
	
	return false;
}
bool KageLayer::flipVerticalSelectedShape(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return doFlipVerticalSelectedShapeOn(_currentFrameIndex, p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return doFlipVerticalSelectedShapeOn(_currentFrameIndex, p_selectedShapes);
			}
		}
	}
	
	return false;
}
	bool KageLayer::doFlipVerticalSelectedShapeOn(unsigned int p_frameIndex, std::vector<unsigned int> p_selectedShapes) {
		if (       Frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_NOT
				|| Frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_START) {
			return Frames[p_frameIndex]->flipVerticalSelectedShape(p_selectedShapes);
		} else {
			return doFlipVerticalSelectedShapeOnExtendedFrame(p_frameIndex-1, p_selectedShapes);
		}
	}
	bool KageLayer::doFlipVerticalSelectedShapeOnExtendedFrame(unsigned int p_frameIndex, std::vector<unsigned int> p_selectedShapes) {
		while (p_frameIndex > 0) {
			if (       Frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_NOT
					|| Frames[p_frameIndex]->getExtension() == KageFrame::extension::EXTENSION_START) {
				return Frames[p_frameIndex]->flipVerticalSelectedShape(p_selectedShapes);
			} else {
				--p_frameIndex;
			}
		}
		return false;
	}

bool KageLayer::recenterRotationPoint(std::vector<unsigned int> p_selectedShapes) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return Frames[_currentFrameIndex]->recenterRotationPoint(p_selectedShapes);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return Frames[i]->recenterRotationPoint(p_selectedShapes);
			}
		}
	}
	
	return false;
}

bool KageLayer::addDataToFrame(VectorDataManager p_vectorsData) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		Frames[_currentFrameIndex]->addDataToFrame(p_vectorsData);
		return true;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				Frames[i]->addDataToFrame(p_vectorsData);
				return true;
			}
		}
	}
	
	return false;
}

bool KageLayer::setFrameData(VectorDataManager p_vectorsData) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		Frames[_currentFrameIndex]->setFrameData(p_vectorsData.clone());
		return true;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				Frames[i]->setFrameData(p_vectorsData.clone());
				return true;
			}
		}
	}
	
	return false;
}

VectorDataManager KageLayer::getFrameTweenData(unsigned int p_frameIndex) {
	unsigned int             l_tween = Frames[p_frameIndex]->getTween();
	KageFrame::extension l_extension = Frames[p_frameIndex]->getExtension();
	
	if (       l_tween > 0
			&& (l_extension == KageFrame::extension::EXTENSION_MID
			||  l_extension == KageFrame::extension::EXTENSION_END)) {
		unsigned int l_tweenHead = UINT_MAX;
		unsigned int l_tweenTail = UINT_MAX;
		
		for (unsigned int j = p_frameIndex-1; j >= 0 && j != UINT_MAX; --j) {
			if (Frames[j]->getExtension() == KageFrame::extension::EXTENSION_START) {
				l_tweenHead = j;
				break;
			}
		}
		for (unsigned int j = p_frameIndex+1; j < Frames.size(); ++j) {
			l_extension = Frames[j]->getExtension();
			if (	   l_extension == KageFrame::extension::EXTENSION_START
					|| l_extension == KageFrame::extension::EXTENSION_NOT) {
				l_tweenTail = j;
				break;
			}
		}
		VectorDataManager l_dataHead;
		VectorDataManager l_dataTail;
		if (l_tweenHead != UINT_MAX) {
			//l_dataHead = Frames[l_tweenHead]->getFrameData();
			l_dataHead.setVectorData(Frames[l_tweenHead]->getFrameData().getVectorData());
		}
		if (l_tweenTail != UINT_MAX) {
			//l_dataTail = Frames[l_tweenTail]->getFrameData();
			l_dataTail.setVectorData(Frames[l_tweenTail]->getFrameData().getVectorData());
		}
		std::vector<VectorData> l_vHead = l_dataHead.getVectorData();
		std::vector<VectorData> l_vTail = l_dataTail.getVectorData();
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
				} else if (l_vHead[j].vectorType == VectorData::TYPE_IMAGE && l_vTail[j].vectorType == VectorData::TYPE_IMAGE) {
					for (unsigned int k = 1; k < 5; ++k) {
						if (k == 2) {
							continue;
						}
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
		return Frames[p_frameIndex]->getFrameData();
	}
}

VectorDataManager KageLayer::getFrameData() {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		return getFrameTweenData(_currentFrameIndex);
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				return getFrameTweenData(i);
			}
		}
	}
	std::cout << "getFrameData is returning empty";
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}
VectorDataManager KageLayer::getFrameDataAt(unsigned int p_frame, bool p_frameOnion, bool p_layerOnion) {
	if (p_frame > 0 && p_frame <= Frames.size()) {
		--p_frame; //p_frame now becomes Frame Index
		if (p_frameOnion == true || p_layerOnion == true) {
			return getFrameTweenData(p_frame);
		}
		return Frames[p_frame]->getFrameData();
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}

VectorDataManager KageLayer::getPreviousFrameData(unsigned int p_frameID) {
	for (unsigned int i = 1; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			return Frames[i-1]->getFrameData();
		}
	}
	
	VectorDataManager l_nullReturn;
	return l_nullReturn;
}

bool KageLayer::setFrameDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID) {
	for (unsigned int i = 1; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			Frames[i-1]->setFrameData(p_vectorsData);
			return true;
		}
	}
	
	return false;
}

bool KageLayer::addDataToPreviousFrame(VectorDataManager p_vectorsData, unsigned int p_frameID) {
	for (unsigned int i = 1; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			Frames[i-1]->addDataToFrame(p_vectorsData);
			return true;
		}
	}
	
	return false;
}

bool KageLayer::setPreviousFrameTween(unsigned int p_frameID, unsigned int p_tween) {
	std::cout << " KageLayer::setPreviousFrameTween() " << p_frameID << std::endl;
	for (unsigned int i = 1; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			Frames[i-1]->setTween(p_tween);
			return true;
		}
	}
	
	return false;
}
bool KageLayer::setExtendedFrameTween(unsigned int p_frameID, unsigned int p_tween) {
	std::cout << " KageLayer::setExtendedFrameTween() " << p_frameID << std::endl;
	for (unsigned int i = 0; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			for (unsigned int j = i+1; j < Frames.size(); ++j) {
				Frames[j]->forceSetTween(p_tween);
				if (Frames[j]->getExtension() == KageFrame::extension::EXTENSION_END) {
					break;
				} else if (Frames[j]->getExtension() == KageFrame::extension::EXTENSION_NOT) {
					Frames[j]->setExtension(KageFrame::extension::EXTENSION_END);
					break;
				}
			}
			return true;
		}
	}
	
	return false;
}

bool KageLayer::forceSetTween(unsigned int p_tween) {
//	std::cout << " KageLayer::forceSetTween() " << p_tween << std::endl;
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		Frames[_currentFrameIndex]->forceSetTween(p_tween);
		return true;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				Frames[i]->forceSetTween(p_tween);
				return true;
			}
		}
	}
	
	return false;
}

bool KageLayer::setTween(unsigned int p_tween) {
	std::cout << " KageLayer::setTween() " << p_tween << std::endl;
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		Frames[_currentFrameIndex]->setTween(p_tween);
		return true;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				Frames[i]->setTween(p_tween);
				return true;
			}
		}
	}
	
	return false;
}
unsigned int KageLayer::getTween() {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		unsigned int l_ret = Frames[_currentFrameIndex]->getTween();
		return l_ret;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				_currentFrameIndex = i;
				_currentFrameID = Frames[i]->frameID;
				unsigned int l_ret = Frames[i]->getTween();
				return l_ret;
			}
		}
	}
	
	return 0;
}

bool KageLayer::switchToPreviousFrame() {
	for (unsigned int i = 1; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == _currentFrameID) {
			///index + 1 = becomes frameNumber for current. then -1 becomes frameNumber of Previous Frame; thus `setCurrentFrame(i)'
			setCurrentFrame(i, false);
			return true;
		}
	}
	
	return false;
}

bool KageLayer::switchToPreviousFrame(unsigned int p_frameID) {
	for (unsigned int i = 1; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			///index + 1 = becomes frameNumber for current. then -1 becomes frameNumber of Previous Frame; thus `setCurrentFrame(i)'
			setCurrentFrame(i, false);
			return true;
		}
	}
	
	return false;
}

bool KageLayer::switchToNextFrame() {
	for (unsigned int i = 0; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == _currentFrameID) {
			///index + 1 = becomes frameNumber for current. then +1 becomes frameNumber of Next Frame
			setCurrentFrame(i+2, false);
			return true;
		}
	}
	
	return false;
}

bool KageLayer::switchToNextFrame(unsigned int p_frameID) {
	for (unsigned int i = 0; i < Frames.size(); ++i) {
		if (Frames[i]->frameID == p_frameID) {
			///index + 1 = becomes frameNumber for current. then +1 becomes frameNumber of Next Frame
			setCurrentFrame(i+2, false);
			return true;
		}
	}
	
	return false;
}

bool KageLayer::isCurrentFrame(unsigned int p_frameID) {
	if (_currentFrameIndex > Frames.size()) {
		return false;
	}
	if (Frames[_currentFrameIndex]->frameID == p_frameID
			&& _currentFrameID == p_frameID) {
		return true;
	}
	
	return false;
}

bool KageLayer::canReUseNextFrame() {
	if (_currentFrameIndex+1 == Frames.size()) {
		return false;
	} else if (Frames[_currentFrameIndex+1]->isEmpty() == false
			&& Frames[_currentFrameIndex+1]->getExtension() == KageFrame::extension::EXTENSION_NOT) {
		return false;
	} else if (Frames[_currentFrameIndex+1]->isEmpty() == false
			&& Frames[_currentFrameIndex+1]->getExtension() == KageFrame::extension::EXTENSION_START) {
		return false;
	}
	
	return true;
}

/**
 * For use of Kage when loading KSF file
 * \param p_extension if start/mid/end of frame extension
 */
void KageLayer::setFrameExtension(KageFrame::extension p_extension) {
	if (_currentFrameIndex < Frames.size() && Frames[_currentFrameIndex]->frameID == _currentFrameID) {
		Frames[_currentFrameIndex]->setExtension(p_extension);
		return;
	} else {
		for (unsigned int i = 0; i < Frames.size(); ++i) {
			if (Frames[i]->frameID == _currentFrameID) {
				Frames[_currentFrameIndex]->setExtension(p_extension);
				return;
			}
		}
	}
}

/**
 * @brief Will return which Frame is currently active for editing or viewing
 * 
 * @return unsigned int 0 is frame 1 (?) //TODO: revisit if index 0 is frame 1
 */
unsigned int KageLayer::getActiveFrameID() {
	return _activeFrame;
}

void KageLayer::setSelected(bool p_selected) {
	_selected = p_selected;
}
bool KageLayer::isSelected() {
	return _selected;
}

void KageLayer::setVisible(bool p_visible) {
	_visible = p_visible;
}
bool KageLayer::isVisible() {
	return _visible;
}
void KageLayer::toggleVisibility() {
	if (_visible == true) {
		_visible = false;
	} else {
		_visible = true;
	}
}

void KageLayer::setLock(bool p_lock) {
	_lock = p_lock;
}
bool KageLayer::isLocked() {
	return _lock;
}
void KageLayer::toggleLock() {
	if (_lock == true) {
		_lock = false;
	} else {
		_lock = true;
	}
}

void KageLayer::setLabel(std::string p_label) {
	_label = p_label;
}
std::string KageLayer::getLabel() {
	return _label;
}