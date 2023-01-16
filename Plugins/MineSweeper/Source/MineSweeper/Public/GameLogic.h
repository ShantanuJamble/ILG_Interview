#pragma once
#include <vector>
#include <utility>
#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(GameLogicLog, Log, All);

struct FCell
{
	int CellValue;
	bool VisitedByPlayer;

	FCell()
		: CellValue(0)
		, VisitedByPlayer(false)
	{

	}
	~FCell() = default;
};

class FGameLogic
{
public:
	FGameLogic();
	~FGameLogic() = default;
	void SetupNewGame(int row, int col, int mines);

	bool PlayMove(int row, int col);

	int GetColCount() const;
	int GetRowCount() const;
	int GetMinesCount() const;
	FCell GetCell(int x, int y) const;
	
private:

	void PlaceMines(int minesCount);
	void BakeGameBoard(int mineX, int mineY);
	void CleanUpBoard();
	bool IsValidCell(int x, int y) const;
	bool IsBombLocation(int x, int y)const ;

	int m_MinesCount;
	std::vector<std::vector<FCell>> m_GameBoard;

};