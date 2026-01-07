#include "pch.h"

class EditorSettingsLayer : public CCLayer {
public:
	virtual bool init();
	static EditorSettingsLayer* create();
	virtual void keyBackClicked();
	void onClose(CCObject*);
	void showCallback(CCObject*);

	void onCopyString(CCObject*);
	void onCopyStringInfo(CCObject*);

	void onSelectFilter(CCObject*);
	void onSelectFilterInfo(CCObject*);

	void onGlobalClipboard(CCObject*);
	void onGlobalClipboardInfo(CCObject*);

	void onPrevAnimations(CCObject*);
	//void onPrevAnimationsInfo(CCObject*);

	void onShowObjInfo(CCObject*);
	//void onShowObjInfoInfo(CCObject*); // InfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfoInfo

	void onUnusedObjects(CCObject*);
	void onUnusedObjectsInfo(CCObject*);

	void onLinkControls(CCObject*);
	void onLinkControlsInfo(CCObject*);

	void onDurationLines(CCObject*);

	void onHidePreviewLine(CCObject*);

	void onExperimentalLayering(CCObject*);
};