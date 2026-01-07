#include "ccTouchFix.hpp"
#include "patching.hpp"

void ccTouchFixPatch() {
	sequence_patch(gd::base + 0x15f49, { 0xd4 }); // FLAlertLayer::ccTouchBegan
	sequence_patch(gd::base + 0x15f53, { 0xd8 });
	sequence_patch(gd::base + 0x15f78, { 0xd8 });

	sequence_patch(gd::base + 0x15fca, { 0xd8 }); // FLAlertLayer::ccTouchEnded
	sequence_patch(gd::base + 0x15fd2, { 0xd4 });
	sequence_patch(gd::base + 0x15fe7, { 0xd8 });

	sequence_patch(gd::base + 0x16078, { 0xd8 }); // FLAlertLayer::ccTouchMoved
	sequence_patch(gd::base + 0x16080, { 0xd4 });

	sequence_patch(gd::base + 0x16028, { 0xd8 }); // FLAlertLayer::ccTouchCancelled
	sequence_patch(gd::base + 0x16030, { 0xd4 });
}