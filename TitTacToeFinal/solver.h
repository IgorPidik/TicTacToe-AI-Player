#ifndef SOLVER_H
#define SOLVER_H

#include <QObject>
#include <QtQuick>

class Solver : public QObject
{
    Q_OBJECT
public:
    explicit Solver(QObject *parent = 0);
    ~Solver();
    void changeOwner(int index, int owner);
    int getOwner(int index);
    int chechWin(bool endGame);
    void clickable(bool clickable);
    void clickable(int index);
    QList<int> possibleMoves();
    int scoreLine(int owner1, int owner2, int owner3, bool checkEnd);
    int minimax(int depth, bool maximizingPlayer, bool firstLayer);
    void endGame(int winner);
    bool checkWinning();
    bool checkLines(int owner1, int owner2, int owner3);


public slots:
    void clicked();
    void tryAgain();

private:
    QQuickView *mView;
    bool end;
    bool firstMove;
    QList<QQuickItem*> mNodes;

};

#endif // SOLVER_H
