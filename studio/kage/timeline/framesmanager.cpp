
#include "framesmanager.h"
#include "kage.h"

KageFramesManager::KageFramesManager(Kage *p_win) {
	win = p_win;
	pack_start(place_holder, Gtk::PACK_SHRINK);
		place_holder.set_label(" ");
		place_holder.set_size_request(100, 20);
	addFrameManager(1);
}

KageFramesManager::~KageFramesManager() {
	//
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

///Use KageFramesManager::currentLayer instead of calling this function
unsigned int KageFramesManager::getCurrentLayer() {
	//filter and make sure value is valid
	unsigned int l_count = 1;
	if (framemanager.size() > 0) {
		l_count = framemanager.size();
	}
	if (KageFramesManager::currentLayer > l_count) {
		KageFramesManager::currentLayer = l_count;
	}
	if (KageFramesManager::currentLayer < 1) {
		KageFramesManager::currentLayer = 1;
	}
	
	return KageFramesManager::currentLayer;
}
void KageFramesManager::setCurrentLayer(unsigned int p_currentLayer) {
	unsigned int l_count = 1;
	if (framemanager.size() > 0) {
		l_count = framemanager.size();
	}
	if (p_currentLayer > l_count) {
		p_currentLayer = l_count;
	}
	if (p_currentLayer < 1) {
		p_currentLayer = 1;
	}
	
	KageFramesManager::currentLayer = p_currentLayer;
}

///Use KageFramesManager::currentFrame instead of calling this function
unsigned int KageFramesManager::getCurrentFrame() {
	//filter and make sure value is valid
	//NOTE: do NOT call setCurrentFrame to avoid recursive call to Kage::renderFrames()
	unsigned int l_count = 1;
	unsigned int l_fcount = 1;
	if (framemanager.size() > 0) {
		l_count = framemanager.size();
	}
	
	if (l_count > 0) {
		l_fcount = framemanager[0]->getFrameCount();
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
	unsigned int l_count = 1;
	unsigned int l_fcount = 1;
	if (framemanager.size() > 0) {
		l_count = framemanager.size();
	}
	
	if (l_count > 0) {
		l_fcount = framemanager[0]->getFrameCount();
		if (p_currentFrame > l_fcount) {
			p_currentFrame = l_fcount;
		}
		if (p_currentFrame < 1) {
			p_currentFrame = 1;
		}
		
		for (unsigned int i = 0; i < l_count; ++i) {
			framemanager[i]->setCurrentFrame(p_currentFrame);
		}
		
		KageFramesManager::currentFrame = p_currentFrame;
		win->forceRenderFrames();
	}
}

void KageFramesManager::renderStage() {
	Kage::timestamp();
	cout << "KageFramesManager::renderStage <" << endl;
	win->forceRenderFrames();
	Kage::timestamp();
	cout << "KageFramesManager::renderStage >" << endl;
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
	return framemanager[getCurrentLayer()-1]->getFrameAt(getCurrentFrame());
}
