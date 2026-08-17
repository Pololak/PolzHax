#include "CocosExplorer.hpp"

#include <imgui-hook.hpp>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "utils.hpp"

static CCNode* selected_node = nullptr;
static bool reached_selected_node;
static CCNode* hovered_node = nullptr;

bool operator!=(const cocos2d::CCSize& a, const cocos2d::CCSize& b) { return a.width != b.width || a.height != b.height; }
ImVec2 operator*(const ImVec2& vec, const float m) { return { vec.x * m, vec.y * m }; }
ImVec2 operator/(const ImVec2& vec, const float m) { return { vec.x / m, vec.y / m }; }
ImVec2 operator+(const ImVec2& a, const ImVec2& b) { return { a.x + b.x, a.y + b.y }; }
ImVec2 operator-(const ImVec2& a, const ImVec2& b) { return { a.x - b.x, a.y - b.y }; }

bool operator==(const cocos2d::CCPoint& a, const cocos2d::CCPoint& b) { return a.x == b.x && a.y == b.y; }
bool operator==(const cocos2d::CCRect& a, const cocos2d::CCRect& b) { return a.origin == b.origin && a.size == b.size; }

cocos2d::CCPoint& operator-=(cocos2d::CCPoint& point, const cocos2d::CCPoint& other) {
	point = point - other;
	return point;
}

ImVec2 cocos_to_vec2(const cocos2d::CCPoint& a) {
	const auto size = ImGui::GetMainViewport()->Size;
	const auto win_size = cocos2d::CCDirector::sharedDirector()->getWinSize();
	return {
		a.x / win_size.width * size.x,
		(1.f - a.y / win_size.height) * size.y
	};
}

ImVec2 cocos_to_vec2(const cocos2d::CCSize& a) {
	const auto size = ImGui::GetMainViewport()->Size;
	const auto win_size = cocos2d::CCDirector::sharedDirector()->getWinSize();
	return {
		a.width / win_size.width * size.x,
		-a.height / win_size.height * size.y
	};
}

auto name_for_node(const CCNode* node) {
	return typeid(*node).name() + 6;
	// return "hon hon hon";
}

void render_node_tree(CCNode* node, const size_t index) {
	std::stringstream stream;
	if (!node) return;
	auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
	if (selected_node == node) {
		flags |= ImGuiTreeNodeFlags_Selected;
		reached_selected_node = true;
	}
	if (node->getChildrenCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;
	const auto children_count = node->getChildrenCount();
	const bool open = ImGui::TreeNodeEx(node, flags, "[%d] %s {%d}", index, name_for_node(node), children_count);
	if (ImGui::IsItemClicked()) {
		if (node == selected_node && ImGui::GetIO().KeyAlt) {
			selected_node = nullptr;
			reached_selected_node = false;
		}
		else {
			selected_node = node;
			reached_selected_node = true;
		}
	}
	if (ImGui::IsItemHovered()) {
		hovered_node = node;
	}
	if (open) {
		const auto children = node->getChildren();
		// for some reason its nullptr when empty lol
		if (children) {
			for (size_t i = 0; i < children->count(); ++i) {
				render_node_tree(static_cast<CCNode*>(children->objectAtIndex(i)), i);
			}
		}
		ImGui::TreePop();
	}
}

std::string get_module_name(HMODULE mod) {
	char buffer[MAX_PATH];
	if (!mod || !GetModuleFileNameA(mod, buffer, MAX_PATH))
		return "Unknown";
	return std::filesystem::path(buffer).filename().string();
}

auto format_addr(void* addr) {
	HMODULE mod;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<char*>(addr), &mod))
		mod = NULL;

	return ::format("{}.{}", get_module_name(mod),
		reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(addr) - reinterpret_cast<uintptr_t>(mod)));
}

void render_node_properties(CCNode* node) {
	if (ImGui::Button("Deselect")) {
		selected_node = nullptr;
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Child")) {
		ImGui::OpenPopup("Add Child");
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete")) {
		node->removeFromParentAndCleanup(true);
		return;
	}
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Addr: %p", node);
	ImGui::SameLine();
	if (ImGui::Button("Copy")) {
		clipboard::write(CCString::createWithFormat("%p", node)->getCString());
	}

	if (ImGui::BeginPopupModal("Add Child")) {
		static int item = 0;
		ImGui::Combo("Node", &item, "CCNode\0CCLabelBMFont\0CCLabelTTF\0CCSprite\0CCMenuItemSpriteExtra\0CCScale9Sprite\0ButtonSprite\0SearchButton\0CCMenu\0CCLayer\0");

		static int tag = -1;
		ImGui::InputInt("Tag", &tag);

		static char text[256];
		static char labelFont[256] = "bigFont.fnt";

		// ButtonSprite stuff
		static int buttonWidth;
		static int widthLimit;
		static float textScale;
		static bool limitWidth;
		static char buttonTexture[256];
		static float buttonHeight;

		// SearchButton stuff
		static char iconTexture[256];

		if (item == 1) {
			ImGui::InputText("Text", text, 256);
			ImGui::InputText("Font", labelFont, 256);
		}
		static int fontSize = 20;
		if (item == 2) {
			ImGui::InputText("Text", text, 256);
			ImGui::InputInt("Font Size", &fontSize);
		}
		static bool frame = false;
		if (item == 3 || item == 4) {
			ImGui::InputText("Texture", text, 256);
			ImGui::Checkbox("Frame", &frame);
		}
		if (item == 5) {
			ImGui::InputText("Texture", text, 256);
		}
		if (item == 6) {
			ImGui::InputText("Caption", text, 256);
			ImGui::InputInt("Button Width", &buttonWidth);
			ImGui::InputInt("Width Limit", &widthLimit);
			ImGui::InputFloat("Text Scale", &textScale);
			ImGui::Checkbox("Limit Width", &limitWidth);
			ImGui::InputText("Texture", buttonTexture, 256);
			ImGui::InputText("Font", labelFont, 256);
			ImGui::InputFloat("Button Height", &buttonHeight);
		}
		if (item == 7) {
			ImGui::InputText("Button Texture", buttonTexture, 256);
			ImGui::InputText("Label", text, 256);
			ImGui::InputText("Icon Texture", iconTexture, 256);
			ImGui::InputFloat("Text Scale", &textScale);
		}

		ImGui::Separator();

		if (ImGui::Button("Add")) {
			CCNode* _child = nullptr;
			switch (item) {
			case 0:
				_child = CCNode::create();
				break;
			case 1: {
				auto child = CCLabelBMFont::create(text, labelFont);
				_child = child;
				break;
			}
			case 2: {
				auto child = CCLabelTTF::create(text, "Arial", fontSize);
				_child = child;
				break;
			}
			case 3: {
				CCSprite* child;
				if (frame)
					child = CCSprite::createWithSpriteFrameName(text);
				else
					child = CCSprite::create(text);
				_child = child;
				break;
			}
			case 4: {
				CCSprite* sprite;
				if (frame)
					sprite = CCSprite::createWithSpriteFrameName(text);
				else
					sprite = CCSprite::create(text);
				_child = gd::CCMenuItemSpriteExtra::create(sprite, sprite, nullptr, 0);
				break;
			}
			case 5: {
				extension::CCScale9Sprite* child;
				child = extension::CCScale9Sprite::create(text);
				_child = child;
				break;
			}
			case 6: {
				gd::ButtonSprite* child;
				child = gd::ButtonSprite::create(text, buttonWidth, widthLimit, textScale, limitWidth, labelFont, buttonTexture, buttonHeight);
				_child = child;
				break;
			}
			case 7: {
				gd::SearchButton* child;
				child = gd::SearchButton::create(buttonTexture, text, textScale, iconTexture);
				_child = child;
				break;
			}
			case 8: {
				_child = CCMenu::create();
				break;
			}
			case 9: {
				_child = CCLayer::create();
				break;
			}
			}
			if (_child != nullptr) {
				_child->setTag(tag);
				node->addChild(_child);
			}

			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::Text("Tag: %d", from<int>(node, 0xb0));
	ImGui::Text("User Data: %p", from<void*>(node, 0xb4));
	{
		auto value = node->getPosition();
		ImGui::DragFloat2("Position", (float*)&value);
		if (value != node->getPosition()) node->setPosition(value);
	}
	{
		auto value = node->getAnchorPoint();
		ImGui::DragFloat2("Anchor Point", (float*)&value);
		if (value != node->getAnchorPoint()) node->setAnchorPoint(value);
	}
	{
		auto value = node->getContentSize();
		ImGui::DragFloat2("Content Size", (float*)&value);
		if (value != node->getContentSize()) node->setContentSize(value);
	}
	{
		auto value = CCSize({ node->getScaleX() * node->getContentSize().width, node->getScaleY() * node->getContentSize().height });
		ImGui::TextWrapped("Scaled Content Size: %.2fx%.2f", value.width, value.height);
	}

	if (ImGui::TreeNode("Advanced Position PRO")) {
		if (node->getParent()) {
			const auto pos = node->getParent()->convertToWorldSpace(node->getPosition());
			const auto win_size = CCDirector::sharedDirector()->getWinSize();
			ImGui::Text("Window size: %.2fx%.2f", win_size.width, win_size.height);
			ImGui::Text("World space: %.2f, %.2f", pos.x, pos.y);
		}
		ImGui::TreePop();
	}

	{
		auto value = node->getScale();
		ImGui::DragFloat("Scale", &value, 0.05f);
		if (value != node->getScale()) node->setScale(value);
	}
	{
		float value[2] = { node->getScaleX(), node->getScaleY() };
		ImGui::DragFloat2("Scale X/Y", value, 0.05f);
		if (value[0] != node->getScaleX()) node->setScaleX(value[0]);
		if (value[1] != node->getScaleY()) node->setScaleY(value[1]);
	}
	{
		auto value = node->getRotation();
		ImGui::DragFloat("Rotation", &value, 0.05f);
		if (value != node->getRotation()) node->setRotation(value);
	}
	{
		float value[2] = { node->getRotationX(), node->getRotationY() };
		ImGui::DragFloat2("Rotation X/Y", value, 0.05f);
		if (value[0] != node->getRotationX()) node->setRotationX(value[0]);
		if (value[1] != node->getRotationY()) node->setRotationY(value[1]);
	}
	{
		float value[2] = { node->getSkewX(), node->getSkewY() };
		ImGui::DragFloat2("Skew X/Y", value, 0.05f);
		if (value[0] != node->getSkewX()) node->setSkewX(value[0]);
		if (value[1] != node->getSkewY()) node->setSkewY(value[1]);
	}

	if (auto item = dynamic_cast<CCSprite*>(node)) {
		{
			auto value = item->isFlipX();
			ImGui::Checkbox("FlipX", &value);
			if (value != item->isFlipX()) item->setFlipX(value);
		}
		ImGui::SameLine();
		{
			auto value = item->isFlipY();
			ImGui::Checkbox("FlipY", &value);
			if (value != item->isFlipY()) item->setFlipY(value);
		}
	}

	if (auto item = dynamic_cast<CCMenuItem*>(node)) {
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();

		const auto selector = item->m_pfnSelector;
		if (!selector) {
			std::string addr = "N/A";
			ImGui::Text("CCMenuItem selector: %s", addr.c_str());
		}
		else {
			const auto addr = format_addr(union_cast<uintptr_t*>(selector));
			ImGui::Text("CCMenuItem selector: %s", addr.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Copy##ccMenuItem")) {
				clipboard::write(addr);
			}
		}

		if (ImGui::Button("Activate")) {
			item->activate();
		}

		{
			auto value = item->isEnabled();
			ImGui::Checkbox("Enabled", &value);
			if (value != item->isEnabled()) item->setEnabled(value);
		}

		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}

	{
		auto value = node->isVisible();
		ImGui::Checkbox("Visible", &value);
		if (value != node->isVisible()) node->setVisible(value);
	}
	{
		auto value = node->isIgnoreAnchorPointForPosition();
		ImGui::Checkbox("Ignore Anchor Point for Position", &value);
		if (value != node->isIgnoreAnchorPointForPosition()) node->ignoreAnchorPointForPosition(value);
	}
	{
		auto value = node->getZOrder();
		ImGui::InputInt("Z Order", &value);
		if (value != node->getZOrder()) node->setZOrder(value);
	}

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	if (auto layer = dynamic_cast<CCLayer*>(node)) {
		auto value = layer->m_bTouchEnabled;
		ImGui::Checkbox("Touch Enabled", &value);
		if (value != layer->m_bTouchEnabled) layer->setTouchEnabled(value);
	}

	if (auto delegate = dynamic_cast<CCTouchDelegate*>(node)) {
		if (auto handler = CCDirector::sharedDirector()->m_pTouchDispatcher->findHandler(delegate)) {
			auto priority = handler->getPriority();

			if (ImGui::DragInt("Touch Priority", &priority, .03f)) {
				CCDirector::sharedDirector()->m_pTouchDispatcher->setPriority(priority, handler->getDelegate());
			}
		}
	}

	if (auto rgb = dynamic_cast<CCNodeRGBA*>(node)) {
		const auto color = rgb->getColor();
		float colors[4] = { color.r / 255.f, color.g / 255.f, color.b / 255.f, rgb->getOpacity() / 255.f };
		if (ImGui::ColorEdit4("Color", colors))
			rgb->setColor(ccc3(colors[0] * 255.f, colors[1] * 255.f, colors[2] * 255.f));
		rgb->setOpacity((colors[3] * 255.f));
	}
	if (auto label_node = dynamic_cast<CCLabelProtocol*>(node)) {
		std::string str = label_node->getString();
		if (ImGui::InputText("Text", &str, 256))
			label_node->setString(str.c_str());
	}

	if (auto sprite_node = dynamic_cast<CCSprite*>(node); sprite_node) {
		auto* texture = sprite_node->getTexture();

		auto* texture_cache = CCTextureCache::sharedTextureCache();
		auto* cached_textures = public_cast(texture_cache, m_pTextures);
		CCDictElement* el;
		CCDICT_FOREACH(cached_textures, el) {
			if (el->getObject() == texture) {
				ImGui::TextWrapped("Texture name: %s", el->getStrKey());
				break;
			}
		}

		auto* frame_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		auto* cached_frames = public_cast(frame_cache, m_pSpriteFrames);
		const auto rect = sprite_node->getTextureRect();
		CCDICT_FOREACH(cached_frames, el) {
			auto* frame = static_cast<CCSpriteFrame*>(el->getObject());
			if (frame->getTexture() == texture && frame->getRect() == rect) {
				ImGui::Text("Frame name: %s", el->getStrKey());
				ImGui::SameLine();
				if (ImGui::Button("Copy##copyFrameName")) {
					clipboard::write(el->getStrKey());
				}
				break;
			}
		}
		float textureRect[4] = {
					sprite_node->getTextureRect().getMinX(),
					sprite_node->getTextureRect().getMinY(),
					sprite_node->getTextureRect().size.width,
					sprite_node->getTextureRect().size.height,
		};
		if (ImGui::DragFloat4("Rect", textureRect), 0.03f) {
			sprite_node->setTextureRect({ textureRect[0], textureRect[1], textureRect[2], textureRect[3] }, sprite_node->isTextureRectRotated(), sprite_node->getContentSize());
		}

		bool isRectRotated = sprite_node->isTextureRectRotated();;
		if (ImGui::Checkbox("Rotate Rect", &isRectRotated)) {
			sprite_node->setTextureRect(sprite_node->getTextureRect(), isRectRotated, sprite_node->getContentSize());
		}
	}
}

void render_node_highlight(CCNode* node, bool selected) {
	auto& foreground = *ImGui::GetForegroundDrawList();
	auto parent = node->getParent();
	auto bounding_box = node->boundingBox();
	CCPoint bb_min(bounding_box.getMinX(), bounding_box.getMinY());
	CCPoint bb_max(bounding_box.getMaxX(), bounding_box.getMaxY());

	auto camera_parent = node;
	while (camera_parent) {
		auto camera = camera_parent->getCamera();

		float off_x, off_y, off_z;
		camera->getEyeXYZ(&off_x, &off_y, &off_z);
		const CCPoint offset(off_x, off_y);
		bb_min -= offset;
		bb_max -= offset;

		camera_parent = camera_parent->getParent();
	}

	auto min = cocos_to_vec2(parent ? parent->convertToWorldSpace(bb_min) : bb_min);
	auto max = cocos_to_vec2(parent ? parent->convertToWorldSpace(bb_max) : bb_max);
	foreground.AddRectFilled(min, max, selected ? IM_COL32(200, 200, 255, 60) : IM_COL32(255, 255, 255, 70));
}

void renderCocosExplorer(bool& open) {
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);

	static bool highlight = false;
	hovered_node = nullptr;

	if (ImGui::Begin("Cocos Explorer", &open, ImGuiWindowFlags_HorizontalScrollbar/* | ImGuiWindowFlags_MenuBar*/)) {
		reached_selected_node = false;
		auto node = CCDirector::sharedDirector()->getRunningScene();

		//if (ImGui::BeginMenuBar()) {
		//	ImGui::MenuItem("Highlight", nullptr, &highlight);
		//	ImGui::EndMenuBar();
		//}

		const auto avail = ImGui::GetContentRegionAvail();

		ImGui::BeginChild("explorer.tree", ImVec2(avail.x * 0.5f, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

		render_node_tree(node, 0);

		ImGui::EndChild();

		if (!reached_selected_node)
			selected_node = nullptr;

		ImGui::SameLine();


		ImGui::BeginChild("explorer.options");

		if (selected_node)
			render_node_properties(selected_node);
		else
			ImGui::Text("Select a node to edit its properties :-)");

		ImGui::EndChild();
	}
	ImGui::End();

	if (highlight && (selected_node || hovered_node)) {
		if (selected_node) {
			render_node_highlight(selected_node, true);
		}
		if (hovered_node) {
			render_node_highlight(hovered_node, false);
		}
	}
}