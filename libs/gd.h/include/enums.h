#ifndef __ENUMS_H__
#define __ENUMS_H__

#include <gd.h>

namespace gd {
	enum class EditCommand {
		SmallLeft = 1,
		SmallRight = 2,
		SmallUp = 3,
		SmallDown = 4,

		Left = 5,
		Right = 6,
		Up = 7,
		Down = 8,

		BigLeft = 9,
		BigRight = 10,
		BigUp = 11,
		BigDown = 12,

		FlipX = 21,
		FlipY = 22
	};

	enum class EnterEffect {};

	enum class GameObjectType {
		Solid = 0,
		Basic = 1,
		Hazard = 2,
		InverseGravityPortal = 3,
		NormalGravityPortal = 4,
		ShipPortal = 5,
		CubePortal = 6,
		Decoration = 7,
		PulsingDecoration = 8,
		YellowJumpPad = 9,
		PinkJumpPad = 10,
		GravityPad = 11,
		YellowJumpRing = 12,
		PinkJumpRing = 13,
		GravityRing = 14,
		NormalMirrorPortal = 15,
		InverseMirrorPortal = 16,
		BallPortal = 17,
		RegularSizePortal = 18,
		MiniSizePortal = 19,
		UfoPortal = 20,
		Modifier = 21,
		Breakable = 22,
		SecretCoin = 23,
		DualPortal = 24,
		SoloPortal = 25,
		Slope = 26,
		WavePortal = 27
	};

	enum class GhostType {
		Disabled = 0,
		Enabled = 1,
	};

	enum class GJCustomColorMode {
		Default = 0,
		PCol1 = 1,
		PCol2 = 2,
		Col1 = 3,
		Col2 = 4,
		LightBG = 5,
		Col3 = 6,
		Col4 = 7,
		Col3DL = 8
	};

	enum class GJLevelType {
		Local = 1,
		Editor = 2,
		Saved = 3
	};

	enum class GJScoreType {
		Unknown = 0,
		Creator = 1
	};

	enum class GJSongError {
		FailedToFetch = 1,
		NotAllowed = 2,
		DownloadCancelled = 3
	};

	enum IconType {
		Cube = 0,
		Ship = 1,
		Ball = 2,
		UFO = 3,
		Wave = 4,
		Special = 99
	};

	enum class LastGameScene {
		MenuLayer = 0,
		CreatorLayer = 1,
		MyLevelsLayer = 2,
		LevelInfoLayerOrEditLevelLayer = 3,
		SavedLevelsLayer = 5,
		AnotherMenuLayer = 7,
		LevelSelectLayer = 9
	};

	enum class LeaderboardState {
		Default = 0,
		Top100 = 1,
		Global = 2,
		Creators = 3,
		TopWeek = 4
	};

	enum class SearchType {
		Search = 0,
		Downloaded = 1,
		MostLiked = 2,
		Trending = 3,
		Recent = 4,
		UsersLevels = 5,
		Featured = 6,
		Magic = 7,
		Sends = 8,
		MapPack = 9,
		MapPackOnClick = 10,
		Sent = 27,
		MyLevels = 98,
		SavedLevels = 99
	};

	enum class TableViewCellEditingStyle {

	};

	enum class UndoCommand {
		Delete = 1,
		New = 2,
		Paste = 3,
		DeleteMulti = 4,
		Transform = 5,
		Select = 6,
	};

	enum class UpdateResponse {
		Unknown,
		UpToDate,
		GameVerOutOfDate,
		UpdateSuccess,
	};

	enum class AccountError {
		EmailsDoNotMatch = -99,
		AlreadyLinkedToDifferentSteamAccount = -13,
		AccountDisabled = -12,
		AlreadyLinkedToDifferentAccount = -10,
		TooShortLessThan3 = -9,
		TooShortLessThan6 = -8,
		PasswordsDoNotMatch = -7,
		InvalidEmail = -6,
		InvalidPassword = -5,
		InvalidUsername = -4,
		AlreadyUsedEmail = -3,
		AlreadyUsedUsername = -2
	};

	enum class BackupAccountError {
		BackupOrSyncFailed = -3,
		LoginFailed = -2,
		GenericError = -1
	};
}

#endif // !__ENUMS_H__