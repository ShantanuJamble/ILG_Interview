#pragma once
#include "CoreMinimal.h"
#include "GameLogic.h"
#include "Logging/LogMacros.h"


DECLARE_LOG_CATEGORY_EXTERN(GameUILog, Log, All);

class FToolBarBuilder;
class FMenuBuilder;
class SUniformGridPanel;
class FReply;


class FGameUI
{
public:
	FGameUI(FGameLogic&);
	~FGameUI() =default;
	TSharedRef<class SBox> GetWindowDetails();
	TSharedRef<class SBox> ConstructBoard();

	int GetRowCount() const;
	int GetColCount() const;
	int GetMinesCount() const;

	void SetRowCount(int);
	void SetColCount(int);
	void SetMinesCount(int);

private:
	TSharedRef<class SUniformGridPanel> GetBoard();
	FReply OnGameBoardButtonClick(int x, int y);
	FReply OnNewGameGenerationButtonClick();

	FOnClicked m_GameResetDelegate;
	FOnClicked m_GameBoardButtonDelegate;

	//TSharedPtr<class SBox> m_GameGridBox;
	FGameLogic  m_GameInstance;
	int m_CacheRowCounter;
	int m_CacheColCounter;
	int m_CacheMineCounter;
};

