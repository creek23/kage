
#include "framemanager.h"
#include "frame.cpp"

KageFrameManager::KageFrameManager(KageFramesManager *p_fsm, unsigned int p_layerID, unsigned int p_frameCount) {
	fsm = p_fsm;
	layerID = p_layerID;
	frameCtr = 0;
	unsigned int i = 0;
	for (i = 0; i < p_frameCount; i++) {
		addFrame();
	}
}

KageFrameManager::~KageFrameManager() {
	//
}

void KageFrameManager::addFrame() {
	frameCtr++;
	frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*frames.back(), Gtk::PACK_SHRINK);
}

void KageFrameManager::extendFrame(unsigned int p_frameID) {
	frameCtr++;
	frames.push_back(Gtk::manage(new KageFrame(this, layerID, frameCtr)));
		pack_start(*frames.back(), Gtk::PACK_SHRINK);
}

unsigned int KageFrameManager::getID() {
	return layerID;
}

unsigned int KageFrameManager::getFrameCount() {
	return frameCtr;
}

bool KageFrameManager::selectAll(bool p_selectAll) {
	for (int i = 0; i < frameCtr; i++) {
		frames[i]->setSelected(p_selectAll);
	}
	return true;
}

KageFramesManager *KageFrameManager::getFsm() {
	return fsm;
}

void KageFrameManager::setCurrentFrame(unsigned int p_frame) {
	if (p_frame < 1 || p_frame > getFrameCount()) {
		return;
	}
	for (int i = 0; i < frameCtr; i++) {
		frames[i]->setCurrent(false);
	}
	frames[p_frame-1]->setCurrent(true);
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