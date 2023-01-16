#include "GameLogic.h"
#include <array>
#include <deque>
DEFINE_LOG_CATEGORY(GameLogicLog);

#define BOMB -1

namespace
{
	std::array<int8_t, 8> NeighborsX = { -1,-1,-1, 0, 1, 1, 1, 0 };
	std::array<int8_t, 8> NeighborsY = { -1, 0, 1, 1, 1, 0,-1,-1 };
}


FGameLogic::FGameLogic()
	:m_MinesCount(10)
{
	SetupNewGame(10, 5, m_MinesCount);
}

int FGameLogic::GetColCount() const 
{
	return m_GameBoard[0].size(); 
}
int FGameLogic::GetRowCount() const 
{ 
	return m_GameBoard.size(); 
}

int FGameLogic::GetMinesCount() const
{
	return m_MinesCount;
}

bool FGameLogic::IsValidCell(int x, int y) const 
{
	return (x >= 0 && x < m_GameBoard.size() && y >= 0 && y < m_GameBoard[0].size());
}

bool FGameLogic::IsBombLocation(int x, int y) const
{
	return (m_GameBoard[x][y].CellValue == BOMB);
}

FCell FGameLogic::GetCell(int x, int y) const
{
	if (IsValidCell(x, y))
		return m_GameBoard[x][y];
	
	return FCell();
}

void FGameLogic::PlaceMines(int minesCount)
{
	while (minesCount)
	{
		int x = FMath::RandRange(0, m_GameBoard.size() - 1 );
		int y = FMath::RandRange(0, m_GameBoard[0].size() - 1);

		m_GameBoard[x][y].CellValue = BOMB;
		//Update neighbors to hold the count.
		BakeGameBoard(x,y);
		minesCount--;
		UE_LOG(GameLogicLog, Log, TEXT("Placed a mine at %d, %d"),x,y);
	}
}


void FGameLogic::BakeGameBoard(int mineX, int mineY)
{


	for (int i = 0; i < 8; ++i)
	{
		int RowIndex = mineX + NeighborsX[i];
		int ColIndex = mineY + NeighborsY[i];
		if (IsValidCell(RowIndex,ColIndex))
		{
			if (!IsBombLocation(RowIndex, ColIndex))
			{
				m_GameBoard[RowIndex][ColIndex].CellValue++;
			}
		}
	}

}

void FGameLogic::CleanUpBoard()
{
	if (!m_GameBoard.empty())
	{
		for (auto GameRow : m_GameBoard)
		{
			GameRow.clear();
		}
	}
	m_GameBoard.clear();
}

void FGameLogic::SetupNewGame(int RowCount, int ColCount, int MinesCount)
{

	//Need to clean up any old setup we had.
	CleanUpBoard();

	//Allocate memory for the new Game.
	for (int Itr = 0; Itr < RowCount; ++Itr)
	{
		m_GameBoard.push_back(std::vector<FCell>(ColCount));
	}

	//Put bombs everywhere & bake the game board to be used for the game.
	PlaceMines(MinesCount);
	
	UE_LOG(GameLogicLog, Log, TEXT("New game board created. %d, %d"), RowCount, ColCount);
}


bool FGameLogic::PlayMove(int Row, int Col)
{
	UE_LOG(GameLogicLog, Log, TEXT("PlayMove: %d, %d"), Row, Col);

	if (IsBombLocation(Row, Col))
	{
		return false;
	}

	std::deque<std::pair<int, int>> CellsToVisit;
	CellsToVisit.push_back({ Row,Col });

	while (!CellsToVisit.empty())
	{
		std::pair<int, int> Cell = CellsToVisit.front();
		CellsToVisit.pop_front();
		if (IsBombLocation(Cell.first, Cell.second))
		{
			continue;
		}
		
		if (m_GameBoard[Cell.first][Cell.second].CellValue > 0)
		{
			m_GameBoard[Cell.first][Cell.second].VisitedByPlayer = true;
			continue;
		}

		for (int i = 0; i < 8; ++i)
		{
			int RowIndex = Cell.first + NeighborsX[i];
			int ColIndex = Cell.second + NeighborsY[i];
			if (IsValidCell(RowIndex, ColIndex))
			{
				CellsToVisit.push_back({ RowIndex, ColIndex });
			}
		}
	}

	return true;
}