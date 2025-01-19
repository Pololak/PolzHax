#include "CommentCell.h"

void __fastcall CommentCell::loadFromCommentH(CCLayer* self, void*, gd::GJComment* comment) {
	CommentCell::loadFromComment(self, comment);

	auto mainLayer = from<CCLayer*>(self, 0x16c);

	auto commentID = CCLabelBMFont::create("", "chatFont.fnt");
	commentID->setString(CCString::createWithFormat("#%i", comment->m_commentID)->getCString());
	commentID->setAnchorPoint({ 1.f, 0.5f });
	commentID->setPosition({ 330.f, 14.f });
	commentID->setScale(0.6f);
	commentID->setOpacity(75);
	commentID->setColor({ 0,0,0 });
	mainLayer->addChild(commentID);

	std::cout << mainLayer << std::endl;
}