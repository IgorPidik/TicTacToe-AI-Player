#include "solver.h"

Solver::Solver(QObject *parent) : QObject(parent)
{
    mView = new QQuickView();
    mView->setSource(QUrl("qrc:/main.qml"));
    mNodes = mView->rootObject()->childAt(200, 200)->childItems();
    mNodes.removeLast();
    connect(mView->rootObject(), SIGNAL(clicked()), this, SLOT(clicked()));
    connect(mView->rootObject(), SIGNAL(tryAgain()), this, SLOT(tryAgain()));
    mView->setMaximumSize(mView->size());

    mView->show();
    end = false;
    firstMove = true;
}

Solver::~Solver()
{
    delete mView;
}

void Solver::changeOwner(int index, int owner)
{

    foreach (QQuickItem *itm, mNodes)
    {
        if(itm->property("myIndex").toInt() == index)
        {
            itm->setProperty("owner", owner);
        }
    }
}

int Solver::getOwner(int index)
{
    foreach (QQuickItem *itm, mNodes)
    {
        if(itm->property("myIndex").toInt() == index)
        {
            return itm->property("owner").toInt();
        }
    }
}

int Solver::chechWin(bool endGame)
{
    int score = 0;
    QList<int> owners;
    owners.clear();
    foreach (QQuickItem *itm, mNodes)
    {
        owners.append(itm->property("owner").toInt());
    }

    score += scoreLine(owners.at(0), owners.at(1), owners.at(2), endGame);
    score += scoreLine(owners.at(3), owners.at(4), owners.at(5), endGame);
    score += scoreLine(owners.at(6), owners.at(7), owners.at(8), endGame);
    score += scoreLine(owners.at(0), owners.at(4), owners.at(8), endGame);
    score += scoreLine(owners.at(2), owners.at(4), owners.at(6), endGame);
    score += scoreLine(owners.at(0), owners.at(3), owners.at(6), endGame);
    score += scoreLine(owners.at(1), owners.at(4), owners.at(7), endGame);
    score += scoreLine(owners.at(2), owners.at(5), owners.at(8), endGame);

    return score;
}

void Solver::clickable(bool clickable)
{
    foreach (QQuickItem *itm, mNodes)
    {
        itm->setProperty("clickable", clickable);
    }
}

void Solver::clickable(int index)
{
    foreach (QQuickItem *itm, mNodes)
    {
        if(itm->property("myIndex").toInt() == index)
            itm->setProperty("clickable", false);
    }
}

QList<int> Solver::possibleMoves()
{
    QList<int> freeNodes;
    foreach (QQuickItem *itm, mNodes)
    {
        if(itm->property("owner").toInt() == 0)
        {
            freeNodes.append(itm->property("myIndex").toInt());
        }
    }
    return freeNodes;
}

int Solver::scoreLine(int owner1, int owner2, int owner3, bool checkEnd)
{
    if(owner1 == owner2 && owner2 == owner3 && owner1 != 0)
    {
        if(checkEnd)
           endGame(owner2);
        else
            return 1000*owner2;
    }
    if((owner1 == owner2 && 0 == owner3) || (owner3 == owner2 && 0 == owner1))
    {
        return 10*owner2;
    }
    if((owner1 !=0 && owner2 == 0 && owner3 == 0) || (owner2 !=0 && owner1 == 0 && owner3 == 0) || (owner3 !=0 && owner1 == 0 && owner2 == 0))
    {
        if(owner1 != 0)
            return owner1;
        else if(owner2 != 0)
            return owner2;
        else
            return owner3;
    }
    return 0;

}

int Solver::minimax(int depth, bool maximizingPlayer, bool firstLayer)
{
    if(depth == 0 || possibleMoves().isEmpty() || checkWinning())
    {
        if(firstLayer)
        {
            endGame(0);
        }
        else
        {
            return chechWin(false);
        }
    }


    if(maximizingPlayer)
    {
        int bestValue = INT_MIN;
        int bestMove = 0;
        foreach (int index, possibleMoves())
        {
            changeOwner(index, 1);
            int score = minimax(depth - 1, false, false);
            changeOwner(index, 0);
            if(score > bestValue)
            {
                bestValue = score;
                bestMove = index;
            }
        }
        return firstLayer ? bestMove : bestValue;

    }
    else
    {
        int bestValue = INT_MAX;
        foreach (int index, possibleMoves())
        {
            changeOwner(index, -1);
            int score = minimax(depth - 1, true, false);
            changeOwner(index, 0);
            if(score < bestValue)
            {
                bestValue = score;

            }
        }
        return bestValue;
    }

}

void Solver::endGame(int winner)
{
    end = true;
    mView->rootObject()->setProperty("gameEnded", true);
    clickable(false);
    switch (winner) {
    case 1:
        mView->rootObject()->setProperty("winner", "PC Won");
        break;
    case -1:
        mView->rootObject()->setProperty("winner", "Player Won");
        break;
    case 0:
        mView->rootObject()->setProperty("winner", "DRAW");
        break;
    default:
        qDebug() << "end game error";
        break;
    }
}

bool Solver::checkWinning()
{
    QList<int> owners;
    owners.clear();
    foreach (QQuickItem *itm, mNodes)
    {
        owners.append(itm->property("owner").toInt());
    }
    QList<bool> winning;
    winning.append(checkLines(owners.at(0), owners.at(1), owners.at(2)));
    winning.append(checkLines(owners.at(3), owners.at(4), owners.at(5)));
    winning.append(checkLines(owners.at(6), owners.at(7), owners.at(8)));
    winning.append(checkLines(owners.at(0), owners.at(4), owners.at(8)));
    winning.append(checkLines(owners.at(2), owners.at(4), owners.at(6)));
    winning.append(checkLines(owners.at(0), owners.at(3), owners.at(6)));
    winning.append(checkLines(owners.at(1), owners.at(4), owners.at(7)));
    winning.append(checkLines(owners.at(2), owners.at(5), owners.at(8)));
    if(winning.contains(true))
        return true;
    else
        return false;

}

bool Solver::checkLines(int owner1, int owner2, int owner3)
{
    if(owner1 == owner2 && owner2 == owner3 && owner1 != 0)
    {
        return true;
    }
    return false;
}

void Solver::tryAgain()
{
    end = false;
    mView->rootObject()->setProperty("gameEnded", false);
    mView->rootObject()->setProperty("winner", "");
    clickable(true);
    foreach (QQuickItem *itm, mNodes)
    {
        itm->setProperty("owner", 0);
    }
}

void Solver::clicked()
{
    chechWin(true);
    int index = minimax(4, true, true);
    if(!end)
    {
        clickable(index);
        changeOwner(index, 1);
    }


    chechWin(true);

}

