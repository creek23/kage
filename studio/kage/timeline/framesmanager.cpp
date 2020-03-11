
#include "framesmanager.h"
#include "../../kage.h"

KageFramesManager::KageFramesManager(Kage *p_win) {
	win = p_win;
	pack_start(place_holder, Gtk::PACK_SHRINK);
		place_holder.set_label(" ");
		place_holder.set_size_request(100, 20);
	//addFrameManager(1);
}

KageFramesManager::~KageFramesManager() {
	//
}

unsigned int KageFramesManager::layerCount() {
	return framemanager.size();
}
unsigned int KageFramesManager::frameCount() {
	unsigned int l_count = 1;
	if (framemanager.size() > 0) {
		l_count = (*framemanager.back()).getFrameCount();
	}
	return l_count;
}
void KageFramesManager::addFrameManager(unsigned int p_layer) {
	unsigned int l_count = 1;
	if (framemanager.size() > 0) {
		l_count = (*framemanager.back()).getFrameCount();
	}
	framemanager.push_back(Gtk::manage(new KageFrameManager(this, p_layer, l_count)));
		pack_end(*framemanager.back(), Gtk::PACK_SHRINK);
}

void KageFramesManager::deleteFrameManager(unsigned int p_layer) {
	framemanager[p_layer]->removeAllFrames();
	remove(*framemanager[p_layer]);
	framemanager.erase (framemanager.begin() + (p_layer-1));
}

bool KageFramesManager::addFrame() {
	unsigned int l_count = 1;
	if (framemanager.size() > 0) {
		l_count = framemanager.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		framemanager[i]->addFrame();
	}
	return true;
}

bool KageFramesManager::extendFrame(unsigned int p_frameID) {
	unsigned int l_count = 1;
	if (framemanager.size() > 0) {
		l_count = framemanager.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		framemanager[i]->extendFrame(p_frameID);
	}
	return true;
}

bool KageFramesManager::removeFrame(unsigned int p_frameID) {
	unsigned int l_count = framemanager.size();
	
	for (unsigned int i = 0; i < l_count; ++i) {
		framemanager[i]->removeFrame(p_frameID);
	}
	return true;
}

bool KageFramesManager::removeAllFrames() {
	unsigned int l_count = framemanager.size();
	
	for (unsigned int i = 0; i < l_count; ++i) {
		framemanager[i]->removeAllFrames();
		remove(*framemanager[i]);
	}
	
	framemanager.clear();
	addFrameManager(0);
	
	return true;
}

unsigned int KageFramesManager::getCurrentLayer() {
	return win->getCurrentLayer();
}
void KageFramesManager::setCurrentLayer(unsigned int p_currentLayer) {
	unsigned int l_count = framemanager.size();
	
	if (p_currentLayer > l_count) {
		p_currentLayer = l_count;
	}
	if (p_currentLayer < 1) {
		p_currentLayer = 1;
	}
	
	win->setCurrentLayer(p_currentLayer);
}

unsigned int KageFramesManager::getCurrentFrame() {
	//filter and make sure value is valid
	//NOTE: do NOT call setCurrentFrame to avoid recursive call to Kage::renderFrames()
	unsigned int l_count = framemanager.size();
	if (l_count > 0) {
		unsigned int l_fcount = framemanager[0]->getFrameCount();
		if (KageFramesManager::currentFrame > l_fcount) {
			KageFramesManager::currentFrame = l_fcount;
		}
		if (KageFramesManager::currentFrame < 1) {
			KageFramesManager::currentFrame = 1;
		}
	}
	
	return KageFramesManager::currentFrame;
}
void KageFramesManager::setCurrentFrame(unsigned int p_currentFrame) {
	unsigned int l_count = framemanager.size();
	if (l_count > 0) {
		unsigned int l_fcount = framemanager[0]->getFrameCount();
		if (p_currentFrame > l_fcount) {
			p_currentFrame = l_fcount;
		}
		if (p_currentFrame < 1) {
			p_currentFrame = 1;
		}
		
		for (unsigned int i = 0; i < l_count; ++i) {
			framemanager[i]->setCurrentFrame(p_currentFrame);
		}
		
		selectAll(false); //should it be here?
		unsigned int l_currentLayer = win->getCurrentLayer();
		if (l_currentLayer < 1 || l_currentLayer > framemanager.size()) {
			//do nothing
		} else {
			KageFrame * l_frame = framemanager[l_currentLayer-1]->getFrameAt(p_currentFrame);
			if (l_frame) {
				l_frame->setSelected(true);
			}
		}
		
		KageFramesManager::currentFrame = p_currentFrame;
		win->updateFrameLabel();
		win->forceRenderFrames();
	}
}

void KageFramesManager::renderStage() {
	Kage::timestamp();
	cout << " KageFramesManager::renderStage <" << endl;
	win->forceRenderFrames();
	Kage::timestamp();
	cout << " KageFramesManager::renderStage >" << endl;
}

void KageFramesManager::selectAll(bool p_selectAll) {
	unsigned int l_count = 1;
	if (framemanager.size() > 0) {
		l_count = framemanager.size();
	}
	for (unsigned int i = 0; i < l_count; ++i) {
		framemanager[i]->selectAll(p_selectAll);
	}
}

KageFrame *KageFramesManager::getFrame() {
	unsigned int l_currentLayer = win->getCurrentLayer();
	if (l_currentLayer < 1 || l_currentLayer > framemanager.size()) {
		return NULL;
	} else {
		KageFrameManager *l_frameManager = framemanager[l_currentLayer-1];
		if (l_frameManager) {
			return l_frameManager->getFrameAt(getCurrentFrame());
		} else {
			return framemanager[l_currentLayer-1]->getFrameAt(getCurrentFrame());
		}
	}
}
