#include "GameUI.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Input/Reply.h"
#include "Widgets/Layout/SUniformGridPanel.h"


DEFINE_LOG_CATEGORY(GameUILog);

FGameUI::FGameUI(FGameLogic& GameLogicInstance)
	: m_GameInstance(GameLogicInstance)
	, m_CacheRowCounter(m_GameInstance.GetRowCount())
	, m_CacheColCounter(m_GameInstance.GetColCount())
	, m_CacheMineCounter(m_GameInstance.GetMinesCount())
{

	m_GameResetDelegate = FOnClicked::CreateRaw(this, &FGameUI::OnNewGameGenerationButtonClick);
	//m_GameBoardButtonDelegate = FOnClicked::CreateRaw(this, &FGameUI::OnGameBoardButtonClick);
}

int FGameUI::GetRowCount() const 
{
	return m_CacheRowCounter;
}

void FGameUI::SetRowCount(int NewCount)
{
	m_CacheRowCounter = NewCount;
}


int FGameUI::GetColCount() const
{
	return m_CacheColCounter;
}

void FGameUI::SetColCount(int NewCount)
{
	m_CacheColCounter = NewCount;
}


int FGameUI::GetMinesCount() const
{
	return m_CacheMineCounter;
}

void FGameUI::SetMinesCount(int NewCount)
{
	m_CacheMineCounter = NewCount;
}

TSharedRef<class SBox> FGameUI::GetWindowDetails()
{
	const FText ColumnText = FText::FromString(TEXT("Column Count:"));
	const FText RowText = FText::FromString(TEXT("Row Count:"));
	const FText MinesText = FText::FromString(TEXT("Mine Count:"));

	const FText ButtonText = FText::FromString(TEXT("Generate New Grid"));

	return 	SNew(SBox)
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(RowText)
			]

			// Row setup
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SSpacer)
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SBox)
				.MinDesiredWidth(60)
				[
					SNew(SEditableTextBox).
					MinDesiredWidth(50)
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						int NewRowCount = FCString::Atoi(*NewText.ToString());
						if (NewRowCount > 0)
						{
							this->SetRowCount(NewRowCount);
						}
					})
					.Text_Lambda([this]()
					{
						FString text = FString::Printf(TEXT("%d"), this->GetRowCount());
						return FText::FromString(text);
					})
				]
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SSpacer)
			]
		//Col Setup
		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(5)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(ColumnText)
			]
		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SSpacer)
			]
		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SBox)
				.MinDesiredWidth(60)
				[
					SNew(SEditableTextBox)
					.MinDesiredWidth(50)
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						int NewColCount = FCString::Atoi(*NewText.ToString());
						if (NewColCount > 0)
						{
							this->SetColCount(NewColCount);
						}
					})
						.Text_Lambda([this]()
					{
						FString text = FString::Printf(TEXT("%d"), this->GetColCount());
						return FText::FromString(text);
					})
				]
			]

		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SSpacer)
			]
		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(5)
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(MinesText)
			]
		//Mines setup
		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SSpacer)
			]
		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SBox)
				.MinDesiredWidth(60)
				[
					SNew(SEditableTextBox).
					MinDesiredWidth(50)
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						int NewMinesCount = FCString::Atoi(*NewText.ToString());
						if (NewMinesCount > 0 && NewMinesCount <= m_CacheColCounter*m_CacheRowCounter)
						{
							this->SetMinesCount(NewMinesCount);
						}
					})
					.Text_Lambda([this]()
					{
						FString text = FString::Printf(TEXT("%d"), this->GetMinesCount());
						return FText::FromString(text);
					})
				]
			]

		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SSpacer)
			]
		+ SHorizontalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(2)
			.AutoWidth()
			[
				SNew(SBox)
				.MinDesiredWidth(60)
				[
					SNew(SButton)
					.Text(ButtonText)
				.HAlign(HAlign_Center)
				.OnClicked(m_GameResetDelegate)
				]
			]
	];
}

FReply FGameUI::OnNewGameGenerationButtonClick()
{
	m_GameInstance.SetupNewGame(m_CacheRowCounter, m_CacheColCounter, m_CacheMineCounter);
	//TODO: Fire an event to redraw the board.
	return FReply::Handled();
}

FReply FGameUI::OnGameBoardButtonClick(int x, int y)
{
	UE_LOG(GameUILog, Log, TEXT("Cell Clicked = %d %d"),x,y);
	return FReply::Handled();
}


TSharedRef<class SUniformGridPanel> FGameUI::GetBoard()
{
	TSharedRef<class SUniformGridPanel> Container = SNew(SUniformGridPanel).SlotPadding(FMargin(5.0f));

	for (int RowCounter = 0; RowCounter < m_GameInstance.GetRowCount(); ++RowCounter)
	{
		for (int ColCounter = 0; ColCounter < m_GameInstance.GetColCount(); ++ColCounter)
		{
			FString text = FString::Printf(TEXT("%d"), m_GameInstance.GetCell(RowCounter, ColCounter).CellValue);
			bool isVisited = m_GameInstance.GetCell(RowCounter, ColCounter).VisitedByPlayer;
			Container->AddSlot(ColCounter, RowCounter)
				.HAlign(HAlign_Left)
				[
					SNew(SButton)
					.Text((isVisited) ? FText::FromString(text) : FText::FromString(""))
					.IsEnabled(!isVisited)
					.OnClicked(FOnClicked::CreateLambda([this, RowCounter, ColCounter]() ->FReply
						{
							m_GameInstance.PlayMove(RowCounter, ColCounter);
							UE_LOG(GameUILog, Log, TEXT("Cell Clicked = %d %d"), RowCounter, ColCounter);
							//TODO: Fire an event to recreate the board.
							return FReply::Handled();
						}
					))
				];
		}
	}

	return Container;
}

TSharedRef<class SBox> FGameUI::ConstructBoard()
{


	return SNew(SBox)
		[
			GetBoard()
		];
	
}
